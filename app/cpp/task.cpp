#include<iostream>

using std::cout;
using std::endl;

int main()
{
	setlocale(LC_ALL, "Russian");

	unsigned char numbers[] = { 124, 197 };
	int arraySize = sizeof(numbers) / sizeof(*numbers);

	unsigned char sum = 0;
	for (int i = 0; i < arraySize; i++) 
	{
		if (numbers[i] % 2 == 0) numbers[i] = numbers[i] << 1;
		else numbers[i] = numbers[i] >> 1;
		sum += numbers[i];
	}

	cout << "Сумма элементов массива = " << int(sum) << endl;
	return 0;
}
