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
	cout << "���� ������� ��������� ������, ������������� � �������" << endl << endl;

	MEMORY_POINTER = (char *)Memory::malloc(100);

	cout << "����� ���������� ������ ��� ���������: " << int(MEMORY_POINTER) << endl << endl;

	cout << "�������� �������� �� ���������" << endl;

	strcpy(MEMORY_POINTER, "�������� ���(��-��, ������ ��� ���, ������� �� �������) ������, ����� ��������, ��� ������ ������������");

	cout << "���������� � �������� ��������� � ��� ������: "<< endl;
	cout << MEMORY_POINTER << endl;
	cout << int(MEMORY_POINTER) << endl << endl;

	MEMORY_POINTER = (char *)Memory::realloc(MEMORY_POINTER, 30);

	cout << "���������� � �������� ��������� � ��� ������: " << endl;
	cout << MEMORY_POINTER << endl;
	cout << int(MEMORY_POINTER) << endl << endl;

	Memory::free(MEMORY_POINTER);
	_getch();
	//Sleep(10000);
	return 0;
}