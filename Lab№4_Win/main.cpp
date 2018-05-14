#include <iostream>
#include <list>
#include <windows.h>
#include <stdlib.h>
using namespace std;
HANDLE hMutex;

void* Thread(void* pParams){

    cout << "Create thread " <<  GetCurrentThreadId() << endl;
    while (true)
    {
        WaitForSingleObject( hMutex, INFINITE ); //ждем когда мьютекс освободится, чтобы заблок-ть его
        cout << "THREAD " <<  GetCurrentThreadId() << endl;
        ReleaseMutex( hMutex ); // разблок-ть
        Sleep(100);
    }
}

int main(int argc, char *argv[])
{
    list <HANDLE> threadIDlist;
    hMutex = CreateMutex(NULL, //атрибут безопасности0
                         FALSE, //разблок. мьютекс
                         NULL //имя мьютекса
                        );
    while(true) {
        WaitForSingleObject(hMutex, INFINITE); //ждем когда мьютекс освободится, чтобы залочить его
        char symbol;
        cout << "Print symbol" << endl;
        cin.get(symbol);

        switch(symbol) {

            //добавить поток
            case '+': {
                HANDLE hThr;
                //создание нового потока
                if((hThr=CreateThread(  NULL, //атрибут беезопасности потока
                                        0, //размер стека, который мы выдляем под поток(исп.по ум.размер исп-ой программы)
                                        (LPTHREAD_START_ROUTINE)Thread, //адрес потоковой ф-ии
                                        NULL, //параметры, которые мы передаем ему при создании
                                        0, //флаг создания потока(опции создания)
                                        NULL //указатель, по которому будет записан инентификатор созданного потока
                                        )) == 0){
                    cout << "Thread create error" << endl;
                    break;
                }
                threadIDlist.push_back(hThr); //добавить ID потока в список потоков
                Sleep(1000);
                SuspendThread(hThr);
            }break;

            //удалить поток
            case '-': {
                if (!threadIDlist.empty()) {
                    TerminateThread(threadIDlist.back(), //дескриптор потока
                                    NO_ERROR //код завершения потока
                                    );
                    CloseHandle(threadIDlist.back()); //закрывает дескриптор последнего потока
                    Sleep(1000);
                    threadIDlist.pop_back(); //удалить ID потока из списка
                } else {
                    cout <<  "List is empty." << endl;
                }
            } break;

            //выйти, удалив все потоки
            case 'q':
                if(!threadIDlist.empty())
                {
                    for(HANDLE &childID: threadIDlist) {
                        CloseHandle(childID); //закрывает дескриптор последнего потока
                    }
                    threadIDlist.clear(); //очистить список
                } exit(EXIT_SUCCESS);   // завершить программу с кодом 0
            default:
                continue;
        }
        for(HANDLE &childID: threadIDlist) {
            ResumeThread(childID);
            ReleaseMutex( hMutex );                                        // освободить
            Sleep(100);
            WaitForSingleObject( hMutex, INFINITE );                       // заблоикровать
            SuspendThread(childID);
        }
        cin.ignore();
        ReleaseMutex( hMutex );                              // освободить
        //Sleep(1000);
    }
    return 0;
}