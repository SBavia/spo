#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <locale.h>
#include <conio.h>
#include "header.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	char *MEMORY_POINTER;
	cout << "Есть функции выделения памяти, перевыделения и очистки" << endl << endl;

	MEMORY_POINTER = (char *)Memory::malloc(100);

	cout << "Адрес выделенной памяти под указатель: " << int(MEMORY_POINTER) << endl << endl;

	cout << "Помещаем значение по указателю" << endl;

	strcpy(MEMORY_POINTER, "Копируем эту(да-да, именно вот эту, которую вы читаете) строку, чтобы показать, что память используется");

	cout << "Информация о значении указателя и его адресе: "<< endl;
	cout << MEMORY_POINTER << endl;
	cout << int(MEMORY_POINTER) << endl << endl;

	MEMORY_POINTER = (char *)Memory::realloc(MEMORY_POINTER, 30);

	cout << "Информация о значении указателя и его адресе: " << endl;
	cout << MEMORY_POINTER << endl;
	cout << int(MEMORY_POINTER) << endl << endl;

	Memory::free(MEMORY_POINTER);
	_getch();
	//Sleep(10000);
	return 0;
}