#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

ostream& operator<<(ostream& os, vector<string>& result);
void* splitFile(void* arg);
void* findStr(void* arg);

// мьютекс
pthread_mutex_t mutex;

// условная переменная
pthread_cond_t cond;

// Разделяемый ресурс
vector<string> fragments;

// итоговый результат
vector<string> RESULT;

int main()
{

    setlocale(LC_ALL, "Russian");

    pthread_mutex_init(&mutex, NULL); // инициализируем мьютекс
    pthread_cond_init(&cond, NULL); // инициализируем условную переменную

    const string path = "test.txt";
	fstream fs;
	fs.open(path, fstream::in); // режим чтение

    pthread_t th[2]; // массив потоков

    vector<string>& debug_result = RESULT;
    for (size_t i = 0; i < 2; i++)
    {
        if (i == 1){
            // поток для разделения файла на фрагменты
            pthread_create( &th[i], NULL, &splitFile, &fs);
        } 
        else
        {
            // поток для поиска подстроки во фрагментах
            pthread_create( &th[i], NULL, &findStr, NULL);
        }
        
    }

    // ожидаем выполнения работы каждого из потоков
    for (size_t j = 0; j < 2; j++)
    {
        pthread_join(th[j], NULL);
    }

    cout << "\n----- Общий результат ----- \n";

    for (size_t j = 0; j < RESULT.size(); j++)  
    {
        cout << "\nРезультат работы " << j+1 << " входного потока:" << endl;
        cout << debug_result.at(j);
    }

    pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
    fs.close();

    return 0;
}

/*
    Разделение файла на N фрагментов, где N = количество строк в файле
*/
void* splitFile(void* data)
{
    pthread_mutex_lock(&mutex);

    //sleep(3); // имитация выполнения какой-то работы

    fstream *fs= (fstream*)data; // получаем файл из функции создания потока

    // получаем текст из файла
    string str;
    while (!fs->eof())
    {
        getline(*fs, str);
        string tmp; // для временного хранения фрагмента
        for (int j = 0; j < str.size(); j++)
        {
            tmp += str[j];
        }
        if (tmp != "") fragments.push_back(tmp);
    }

    cout << "Количество фрагментов: " << fragments.size() << "\n\n";

    pthread_mutex_unlock(&mutex);

    pthread_cond_signal(&cond);
 
    return NULL;
}

/*
    Функция поиска подстроки в отдельном потоке
*/
void* findStr(void* arg)
{

    pthread_mutex_lock(&mutex);
    /*
        Освобождаем мьютекс и ожидаем вызова 
        функции pthread_cond_signal() другим потоком.
    */
    if (fragments.empty()) pthread_cond_wait(&cond, &mutex); // если нет ни одного фрагмента => файл ещё не был прочитан
            
    string sub;
    cout << "Ввведите подстроку, которорую хотите найти во фрагментах файла: ";
    cin >> sub;
    cout << endl;
    
    for (size_t j = 0; j < fragments.size(); j++)
    {   
        string element = "";
        string str = fragments[j];

        cout << "Фрагмент " << j + 1 << " содержит: '" << str << "'\n";
        size_t i = 0, fromIndex = 0;
        if (str.find(sub) != -1) // если искомая подстрока найдена
        {
            int k = 0;
            element.append("Подстрока '" + sub + "' найдена в позициях: ");
            while ((i = str.find(sub, fromIndex)) != string::npos) { // если подстрока sub найдена, начиная с позиции fromIndex
                int cmp = i + (sub.length() - 1); // для хранения индекса последнего символа найденной подстроки
                if (i == cmp){
                    element.append(to_string(cmp) + " ");
                } else element.append(to_string(i) + "-" + to_string(cmp));
            
                fromIndex = ++cmp; // если возможно несколько вхождений искомой подстроки => начинаем искать подстроку с другого индекса
                k++;

                if (k > 0 && i != cmp){
                    element.append(" ");
                } else element.append("");

            }
            element.append("\n");
        } 
        else element.append("Подстрока '" + sub + "' не найдена.\n");
        
        RESULT.push_back(element);
    }
    
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// перегружаем оператор << для вывода элементов вектора в консоль
ostream& operator<<(ostream& os, vector<string>& result)
{
    os << result.data();
    return os;
}