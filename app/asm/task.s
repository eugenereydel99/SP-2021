.data
print_format:
	.string "Сумма элементов массива = %d\n."

array:
	.byte 124, 97
counter:
	.byte 2

.text
.global main

main:
        movsbl counter, %ecx # Количество итераций в цикле for
        movl $array, %ebx # Указатель на текущий элемент массива
        movl $0,%edx   # Сумма элементов массива
for:
        movsbl (%ebx),%eax # Помещаем текущий элемент массива в регистр eax
        testl $1,%eax # Проверка элемента массива на четность
        jz if_even # Если элемент четный => переход в метку if_even
        jnz if_odd # Если элемент нечетный => переход в метку if_odd

if_even:  # Если четный
        shll $1,%eax  # Логический сдвиг влево на единицу
        addl %eax,%edx # Суммируем в регистр edx текущее значение массива
        addl $1,%ebx  # Перемещаем указатель на следующий элемент массива
        loop for
        jmp print


if_odd:  # Если нечетный
        sarl $1,%eax  # Арифметический сдвиг вправо на единицу
        addl %eax,%edx # Суммируем в регистр edx текущее значение массива
        addl $1,%ebx
        loop for
        jmp print

print:
        pushl %edx
	pushl $print_format
	call printf
	addl $8, %esp
        ret
