#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

ostream& operator<<(ostream& os, vector<string>& result);
vector<string> splitFile(fstream &fileStream);
void* routine(void* data);

// количество потоков
int N = 0;

// мьютекс
pthread_mutex_t mutex;

// общий результат
vector<string> RESULT;

// структура для хранения данных потока
typedef struct
{
    string _str;
    string _sub;
    int _iter;
} fragment;


int main()
{

    setlocale(LC_ALL, "Russian");

    pthread_mutex_init(&mutex, NULL); // инициализируем мьютекс

    const string path = "test.txt";
	fstream fs;
	fs.open(path, fstream::in); // режим чтение
    vector<string> fragments;

	fragments = splitFile(fs); // разделение текста в файле на фрагменты

    N = fragments.size(); // сколько элементов массива - столько и потоков
    
    pthread_t threads[N]; // массив потоков

    fragment thread_data[N]; // сколько потоков - столько и структур с потоковых данных

    string sub;
    cout << "Ввведите подстроку, которорую хотите найти во фрагментах файла: ";
    cin >> sub;
    cout << endl;
    
    vector<string> &debug_result = RESULT;
    for (size_t i = 0; i < N; i++){

        //pthread_mutex_lock(&mutex);

        string str = fragments[i];

        /*string sub;
        cout << "Ввведите подстроку, которорую хотите найти в "<< i + 1 <<" фрагменте файла: ";
        cin >> sub;
        */
        //cout << "Фрагмент " << i + 1 << " содержит: '" << str << endl;

        // передаём будущему потоку необходимые атрибуты
        thread_data[i]._str = str;
        thread_data[i]._sub = sub;
        thread_data[i]._iter = i;

        pthread_create(
            &threads[i], // указатель на идентификатор создаваемого потока
            NULL, // указатель на атрибуты потока
            &routine, // адрес функции потока
            &thread_data[i] // значение, которое возвращает функция потока
        );

        //sleep(1);

    }

    // ожидаем выполнения работы каждого из потоков
    for (size_t j = 0; j < N; j++)
    {
        pthread_join(threads[j], NULL);
    }
    
    cout << "\n----- Общий результат ----- \n";

    for (size_t i = 0; i < N; i++)  
    {
        cout << "\nРезультат работы " << i+1 << " входного потока:" << endl;
        cout << debug_result.at(i);
    }

    pthread_mutex_destroy(&mutex);
	fs.close();

    return 0;
}

/*
    Функция поиска подстроки в отдельном потоке
*/
void* routine(void* data)
{
    //pthread_mutex_lock(&mutex);

    fragment *ptr_data = (fragment*)data;

    string str = ptr_data->_str;
    string sub = ptr_data->_sub;

    string element = "";

    cout << "Фрагмент " << ptr_data->_iter + 1 << " содержит: '" << str << "'\n";
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

    //pthread_mutex_lock(&mutex);

    RESULT.push_back(element);

    //pthread_mutex_unlock(&mutex);

    return NULL;
}

/*
    Разделение файла на N фрагментов, где N = количество строк в файле
*/
vector<string> splitFile(fstream& fileStream)
{
    /*
        Создание вектора для хранения фрагментов.
    */
    vector<string> fragments;

    // получаем текст из файла
    string str;
    while (!fileStream.eof())
    {
        getline(fileStream, str);
        string tmp; // для временного хранения фрагмента
        for (int j = 0; j < str.size(); j++)
        {
            tmp += str[j];
        }
        if (tmp != "") fragments.push_back(tmp);
    }

    cout << "Количество фрагментов: " << fragments.size() << "\n\n";

    return fragments;
}

// перегружаем оператор << для вывода элементов вектора в консоль
ostream& operator<<(ostream& os, vector<string>& result)
{
    os << result.data();
    return os;
}