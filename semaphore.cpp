#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

ostream& operator<<(ostream& os, vector<string>& result);
void* splitFile(void* arg);
void* findStr(void* arg);

// семафор
sem_t semaphore;

// Разделяемый ресурс
vector<string> fragments;

// итоговый результат
vector<string> RESULT;

int main()
{

    setlocale(LC_ALL, "Russian");

    sem_init(&semaphore, 0, 0); // инициализируем семафор

    const string path = "test.txt";
	  fstream fs;
	  fs.open(path, fstream::in); // режим чтение

    pthread_t th1, th2;

    vector<string>& debug_result = RESULT;

    // поток для поиска подстроки во фрагментах
    pthread_create( &th1, NULL, &findStr, NULL );

    // поток для разделения файла на фрагменты
    pthread_create( &th2, NULL, &splitFile, &fs );

    // ожидаем выполнения работы каждого из потоков
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    cout << "\n----- Общий результат ----- \n";

    for (size_t j = 0; j < RESULT.size(); j++)  
    {
        cout << "\nРезультат работы " << j+1 << " входного потока:" << endl;
        cout << debug_result.at(j);
    }

    sem_destroy(&semaphore);
    fs.close();

    return 0;
}

/*
    Разделение файла на N фрагментов, где N = количество строк в файле
*/
void* splitFile(void* data)
{

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

    sem_post(&semaphore); // увеличивает значение семафора на единицу
 
    return NULL;
}

/*
    Функция поиска подстроки в отдельном потоке
*/
void* findStr(void* arg)
{

  /*
		Приостанавливаем выполнение текущего потока до тех пор,
		пока значение семафора не станет больше нуля, после чего значение
		семафора понижается на единицу и возвращается управление этому потоку.
	*/
	sem_wait(&semaphore);
            
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
    
    return NULL;
}

// перегружаем оператор << для вывода элементов вектора в консоль
ostream& operator<<(ostream& os, vector<string>& result)
{
    os << result.data();
    return os;
}
