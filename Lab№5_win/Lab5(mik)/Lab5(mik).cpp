// Lab5(mik).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <string.h>
#include <filesystem>
#include <list>
#include <windows.h>
#include <assert.h>
#include <comutil.h>
#pragma comment(lib, "comsuppw" )

using namespace std;

OVERLAPPED o;
list <char*> MyList;
HANDLE hEvent;

wchar_t* charwchart(const char *array)
{
	wchar_t *wr = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, array, -1, wr, 4096);
	return wr;
}

char* readFromFile(LPCWSTR filePath)
{
	HANDLE hFile;
	LARGE_INTEGER fSize;
	char *str = nullptr;
	hFile = CreateFile(filePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		assert(0);
	}
	GetFileSizeEx(hFile, &fSize);
	DWORD toRead = fSize.QuadPart;
	str = new char[toRead];
	DWORD byteWasRead = 0;
	bool boolean = ReadFile(hFile, str, toRead, &byteWasRead, &o);		
	//WaitForSingleObject(hEvent, INFINITE);
	str[toRead] = '\0';
	CloseHandle(hFile);
	if (hFile == INVALID_HANDLE_VALUE)
		assert(0);
	cout << endl << "R" << endl;
	return str;
}

void myRead(LPCWSTR folderPath)
{
	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;							//строка для поиска файлов
	HANDLE hf = FindFirstFile(folderPath, &FindFileData);
	CHAR fileNames[200][MAX_PATH];							//здесь файлы, которые мы нашли
	INT CountOfFiles = 0, j = 0;
	_bstr_t LpcwstrToChar(folderPath);						//перевод в символ
	char *temp;
	int folderPathLength = strlen(LpcwstrToChar) - 1, fileNameLength;
	if (hf == INVALID_HANDLE_VALUE)
	{
		cout << "Path not found" << endl;
		return;
	}
	do
	{
		LpcwstrToChar = _bstr_t(folderPath);
		strcpy_s(fileNames[CountOfFiles], LpcwstrToChar);
		j = folderPathLength;
		LpcwstrToChar = _bstr_t(FindFileData.cFileName);
		fileNameLength = strlen(LpcwstrToChar);
		temp = LpcwstrToChar;
		while (j < folderPathLength + fileNameLength)
		{
			fileNames[CountOfFiles][j] = temp[j - folderPathLength];
			j++;
		}
		if (j<MAX_PATH)
			fileNames[CountOfFiles][j] = '\0';
		puts(fileNames[CountOfFiles]);
		++CountOfFiles;
	} while (FindNextFile(hf, &FindFileData));
	FindClose(hf);
	int i = 2;
	while (i < CountOfFiles)
	{
		LPCWSTR filePath = charwchart(fileNames[i]);
		char *str = readFromFile(filePath);
		MyList.push_back(str);
		i++;
		cout << MyList.back() << endl;
	}
	SetEvent(hEvent);
}

void writeToFile(LPCWSTR fPath, char *str)
{
	HANDLE fHandle;
	LARGE_INTEGER fileSize;
	fHandle = CreateFile(fPath, GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (fHandle == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		assert(0);
	}
	GetFileSizeEx(fHandle, &fileSize);
	DWORD toWrite = fileSize.QuadPart;
	o.Offset = toWrite;
	DWORD byteWasWrite = 0;
	toWrite = strlen(str);
	bool boolean = WriteFile(fHandle, str, toWrite, &byteWasWrite, &o);
	//WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(fHandle);
	if (fHandle == INVALID_HANDLE_VALUE)
		assert(0);
	cout << "W" << endl;
}

void write()
{
	HANDLE hFile;
	LPCWSTR fPath =
		charwchart("E:\\test\\In.txt");
	WaitForSingleObject(hEvent, INFINITE);
	while (!MyList.empty())
	{
		char *str = MyList.front();
		writeToFile(fPath, str);
		MyList.pop_front();
	}
}

void main()
{
	o.Offset = 0;												
	o.OffsetHigh = 0;											
	hEvent = CreateEvent(NULL, false, false, NULL);					
	//ResetEvent(hEvent);												//сброс события(чтобы было в несигнальном состоянии)
	thread readerThread(myRead, charwchart("E:\\test\\Out\\*"));
	readerThread.joinable();
	thread writerThread(write);
	writerThread.joinable();
	readerThread.join();										//ждем выполнение родительского потока пока не будет завершен дочерний
	writerThread.join();
	readerThread.~thread();
	writerThread.~thread();
	system("pause");
}