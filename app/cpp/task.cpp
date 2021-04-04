#include <iostream>
#include <time.h>

using std::cout;
using std::endl;

void printArray(int* numbersArray, int arraySize);

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	int n, m;

	do {
		n = 1 + rand() % 10;
		m = 1 + rand() % 10;
	} while (n == m);

	int* array1 = new int[n];
	int* array2 = new int[m];

	for (int i = 0; i < n; i++) array1[i] = 1 + rand() % 100;
	for (int i = 0; i < m; i++) array2[i] = 1 + rand() % 100;

	printArray(array1, n);
	printArray(array2, m);

	int* array3 = new int[n+m];
	for (int i = 0; i < n; i++) array3[i] = array1[i];
	for (int j = n; j < n+m; j++) array3[j] = array2[j - n];

	printArray(array3, n+m);

	delete[]array1;
	delete[]array2;
	delete[]array3;

	return 0;
}

void printArray(int* numbersArray, int arraySize)
{
    for (int i = 0; i < arraySize; i++) {
        cout << numbersArray[i] << " ";
    }
    cout << endl;
}
