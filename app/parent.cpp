#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

vector<string> splitFile(fstream &fileStream);

int main()
{
    setlocale(LC_ALL, "Russian");

    const string path = "test.txt";
	fstream fs;
	fs.open(path, fstream::in); // режим чтение
    vector<string> fragments;
	fragments = splitFile(fs);

    for (size_t i = 0; i < fragments.size(); i++){
        
        pid_t pid;

        const char *child = "./child";
        int *status;

        string sub;
        cout << "Фрагмент " << i + 1 << " содержит: '" << fragments[i] << endl;
	    cout << "Ввведите подстроку, которорую хотите найти в " << i + 1 << " фрагменте файла: ";
	    cin >> sub;

        pid = fork();
        if(pid == 0) // дочерний процесс
        {
            printf("\n -- Дочерний процесс --\n");
            printf("PID моего процесса = %d\n", getpid());
            printf("PID родительского процесса = %d\n\n", getppid());
            execl(child, child, fragments[i].c_str(), sub.c_str(), NULL);
        }
        else if (pid == -1){
            perror("Ошибка создания дочернего процесса.");
            exit(1);
        }
        else { // родительский процесс
            printf("\n -- Родительский процесс --\n");
            printf("PID моего процесса = %d\n", getpid());
            printf("PID моего дочернего процесса = %d\n", pid);
            wait(status);
        } 
        cout << endl;
    }

	fs.close();

    return 0;
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
        string result; // для временного хранения фрагмента
        for (int j = 0; j < str.size(); j++)
        {
            result += str[j];
        }
        if (result != "") fragments.push_back(result);
    }

    cout << "Количество фрагментов: " << fragments.size() << "\n\n";

    return fragments;
}
