#include<iostream>

using std::cout;
using std::endl;

int main()
{
	setlocale(LC_ALL, "Russian");

	unsigned int numbers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int arraySize = sizeof(numbers) / sizeof(*numbers);

	unsigned int sum = 0;
	for (int i = 0; i < arraySize; i++) 
	{
		if ((i) % 2 == 0) numbers[i] = numbers[i] << 1;
		else numbers[i] = numbers[i] >> 1;
		sum += numbers[i];
	}

	cout << "Сумма элементов массива = " << sum << endl;
	return 0;
}
