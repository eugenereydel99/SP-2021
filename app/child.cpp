#include <iostream>

using namespace std;

void findStr(string str, string sub);

int main(int argc, char *argv[])
{

    bool isEmpty;

    int i = 0;
    while (argv[++i] == NULL)
    {
        isEmpty = true;
        break;
    }

    if (!isEmpty) findStr(argv[1], argv[2]);
    else cout << "Аргументы не переданы.\n";
    
	return 0;
}

/*
    Поиск подстроки в строке (во фрагменте).
*/
void findStr(string str, string sub)
{
    size_t i = 0, fromIndex = 0;
    if (str.find(sub) != -1) // если искомая подстрока найдена
    {
        int k = 0;
        cout << "Подстрока '" << sub << "' найдена в позициях: ";
        while ((i = str.find(sub, fromIndex)) != string::npos) { // если подстрока sub найдена, начиная с позиции fromIndex
            int cmp = i + (sub.length() - 1); // для хранения индекса последнего символа найденной подстроки
            i == cmp ? cout << cmp << " " : cout << i << "-" << cmp; // если подстрока равна одному символу, иначе, если нескольким символам
            fromIndex = ++cmp; // если возможно несколько вхождений искомой подстроки => начинаем искать подстроку с другого индекса
            k++;

            k > 0 && i != cmp ? cout << " " : cout << ""; // если несколько вхождений искомой подстроки => пробел между индексами вхождений
        }
        cout << "\n";
    }
    else cout << "Подстрока '" << sub << "' не найдена.\n";
    cout << "\n";
}