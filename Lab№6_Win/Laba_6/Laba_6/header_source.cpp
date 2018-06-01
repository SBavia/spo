#include "header.h"

void *Memory::malloc(size_t size)
{
	HLOCAL MEMORY_BLOCK;
	void*  new_ptr;

	cout << "Выделение памяти: " << endl;

	MEMORY_BLOCK = LocalAlloc(LHND, size);						//получаем блок памяти и заполняем его нулями

	// LHND = LMEM_MOVEABLE + LMEM_ZEROINIT
	// LMEM_MOVEABLE - Выделяет подвижную память.
	// LMEM_ZEROINIT - Инициализирует содержимое памяти до нуля.

	if (MEMORY_BLOCK == NULL)
	{
		cout << "Ошибка в выделении памяти." << endl;
		new_ptr = nullptr;
	}
	else
	{
		new_ptr = LocalLock(MEMORY_BLOCK);						//Блокируем блок памяти чтоб получить доступ из main
	}

	return new_ptr;
}

void *Memory::realloc(void *ptr, size_t newSize)
{
	HLOCAL MEMORY_BLOCK = LocalHandle(ptr);							//Определяем идентификатор локального блока памяти

	cout << "Перевыделение памяти" << endl;

	LocalUnlock(MEMORY_BLOCK);										//Разблокируем наш блок памяти для перевыделения памяти

	MEMORY_BLOCK = LocalReAlloc(MEMORY_BLOCK, newSize, LMEM_ZEROINIT);	//Перевыделяем память под наш блок, большее заполняем нулями

	if (MEMORY_BLOCK == NULL)
	{
		cout << "Перевыделение не удалось" << endl;
		ptr = nullptr;
	}
	else
	{
		ptr = LocalLock(MEMORY_BLOCK);								//Блокируем блок чтобы получить доступ к переменной в этой памяти
	}

	return ptr;
}

void Memory::free(void *ptr)
{
	cout << "Очистка памяти." << endl;
	HLOCAL MEMORY_BLOCK = LocalHandle(ptr);

	LocalUnlock(MEMORY_BLOCK);

	if (LocalFree(MEMORY_BLOCK) == NULL)
		cout << "Память успешно очищена." << endl;
	else
		cout << "Упс...." << endl;
}