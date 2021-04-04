#include <iostream>
#include <time.h>

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

    int sum = n+m;

	int* array1 = new int[n];
	int* array2 = new int[m];

	for (int i = 0; i < n; i++) array1[i] = 1 + rand() % 100;
	for (int i = 0; i < m; i++) array2[i] = 1 + rand() % 100;

	printArray(array1, n);
	printArray(array2, m);

	int* array3 = new int[sum];

	/*for (int i = 0; i < n; i++) array3[i] = array1[i];
	for (int j = n; j < sum; j++) array3[j] = array2[j - n];
    */

    __asm__(
        "step_1:\n\t" // first for loop
        "movl %[n], %%ecx\n\t" // counter for loop "for_1"
        "movl %[array1], %%ebx\n\t" // pointer to first element of array1
        "movl %[array3], %%edx\n\t" // pointer to first element of array3

        "for_1:\n\t" // loop "for_1"
        "movl (%%ebx), %%eax\n\t" // put element of array1 in eax
        "push %%ecx\n\t" // put counter in stack
        "movl (%%edx), %%ecx\n\t" // put element of array3 in ecx
        "movl %%eax, %%ecx\n\t" // put eax in ecx
        "movl %%ecx, (%[array3])\n\t"  // put ecx in array3
        "pop %%ecx\n\t" // fetch ecx from stack

        "addl $4, %%ebx\n\t" // shift pointer on next element of array1
        "addl $4, %%edx\n\t" // shift pointer on next element of array3
        "decl %%ecx\n\t" // decrement counter for loop
        "cmpl $0,%%ecx\n\t" // if counter = 0, leave from for loop
        "je step_2\n\t"
        "jne for_1\n\t"

        "step_2:\n\t" // second for loop
        "movl %[sum], %%ecx\n\t" // counter for loop "for_2"
        "movl %[array2], %%ebx\n\t" // pointer to first element of array2

        "for_2:\n\t" // loop "for_2"
        "movl (%%ebx), %%eax\n\t" // put element of array2 in eax
        "push %%ecx\n\t" // put counter in stack
        "movl (%%edx), %%ecx\n\t" // put element of array3 in ecx
        "movl %%eax, %%ecx\n\t" // put eax in ecx
        "movl %%ecx, (%[array3])\n\t" // put ecx in array3
        "pop %%ecx\n\t" // fetch ecx from stack

        "addl $4, %%ebx\n\t" // shift pointer on next element of array2
        "addl $4, %%edx\n\t" // shift pointer on next element of array3
        "decl %%ecx\n\t" // decrement counter for loop
        "cmpl %[n],%%ecx\n\t" // if counter = n, leave from for loop
        "je end_asm\n\t"
        "jne for_2\n\t"

        "end_asm:\n\t"
        :
        :[n]"m"(n), [sum]"m"(sum),
        [array1]"m"(array1), [array2]"m"(array2), [array3]"d"(array3)
        :"%ecx", "%ebx"
    );
	
    printArray(array3, sum);

    delete[] array1;
    delete[] array2;
    delete[] array3;
    
    return 0;
}

void printArray(int *numbersArray, int arraySize)
{
    for (int i = 0; i < arraySize; i++) {
        std::cout << numbersArray[i] << " ";
    }
    std::cout << "\n";
}