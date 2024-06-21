#include <iostream>
#include <Windows.h>
#include <fileapi.h>
#include <Sysinfoapi.h>
#include "node.h"
#define MAP_NAME "ILL:D"
#define FILEPATH "Yellow.llfm"
#define MAP_SIZE buffSize
#define MUTEX_NAME "No no zone"


Node*& buffer = PNode<Node>::buffer;
SYSTEM_INFO r;
DWORD buffSize;
HANDLE hStdin, hStdout;


int main1();
int main2n3(const bool addOrRem);
int main2() {
    SetConsoleTitleA("Linked-List TM (Adder)");// why not?!
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN); // setting text collor
    SetConsoleTextAttribute(hStdin, FOREGROUND_INTENSITY | COMMON_LVB_LEADING_BYTE);
    return main2n3(false);
}
int main3() {
    SetConsoleTitleA("Linked-List TM (Remover)");// why not?!
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED); // setting text collor
    SetConsoleTextAttribute(hStdin, FOREGROUND_INTENSITY | COMMON_LVB_LEADING_BYTE);
    return main2n3(true);
}

int main(const int argc, const char* argv[])
{
    GetSystemInfo(&r);
    buffSize = r.dwAllocationGranularity;
    hStdin = GetStdHandle(STD_INPUT_HANDLE); //get the handle of in
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); // get the handle of out
    if (hStdin == INVALID_HANDLE_VALUE ||
        hStdout == INVALID_HANDLE_VALUE)
    { // error if not got
        MessageBox(NULL, TEXT("GetStdHandle"), TEXT("Console Error"),
            MB_OK);
        return 1;
    }
    typedef int(*mainT)();
    mainT mains[] = { main1, main2, main3 };
    if (atoi(argv[0]) < 0 || atoi(argv[0]) > sizeof(mains) / sizeof(mains[0])) { std::cout << "Invalid argument!\n 1 for create the memory thing.  2 for append  3 for remove" << std::endl; return 1; }
    return mains[atoi(argv[0]) - 1]();
}

int main1() {
    int errorCode = 0;
    SetConsoleTitleA("Linked-List TM (Setter)");// why not?!

    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE); //set color
    SetConsoleTextAttribute(hStdin, FOREGROUND_INTENSITY | COMMON_LVB_LEADING_BYTE);
    std::cout << "Open! now do your thing" << std::endl;
    
    HANDLE hMapping,hFile,mutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    if (!mutex) { std::cout << "Error openning the mutex" << std::endl; errorCode = GetLastError(); goto end; }
    hFile = CreateFileA(FILEPATH, // The path of the file
        GENERIC_WRITE | GENERIC_READ, // Open for reading & writing on the file
        FILE_SHARE_WRITE | FILE_SHARE_READ, // allow other processes to open for reading this file
        NULL, // no security attributes
        OPEN_ALWAYS, // always creates it
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, // The file is temporary and deleted when closed
        NULL); // No template handle
    if (hFile == INVALID_HANDLE_VALUE) { std::cout << "Could not open the file: " << FILEPATH << std::endl; errorCode = GetLastError(); goto fileFail;} // check for seccessfull openning

    hMapping = CreateFileMappingA(hFile, // map the file
        NULL, // no security attributes
        PAGE_READWRITE, // Views can read & write
        0, // the high number of the map size
        MAP_SIZE, // the low number of the map size                      
        MAP_NAME // the name of the mapping
    );
    if (hMapping == NULL) { std::cout << "Could not create the mapping\nError number: "<< GetLastError() << std::endl; errorCode = GetLastError(); goto mapFail;}
    buffer = (Node*)MapViewOfFile(hMapping, // view of this map
        FILE_MAP_READ | FILE_MAP_WRITE,  // the view is for reading | write
        0,  // The start (high)
        0, // the start (low)
        sizeof(Node)); // the size to read
    if (buffer == NULL) { // if failed
        std::cout << "Failed at map view\nError number: " << GetLastError() << std::endl;errorCode = GetLastError(); goto fileFail;
    
    }
    buffer->_value = 0; // hold the size of the list
    buffer->_next = (DWORD)0; // the pointer to the next
    while (!buffer->_value); // wait for change and then for full detetion of list
    while (buffer->_value);

    
    UnmapViewOfFile(buffer);
    std::cout << "Bye!!!" << std::endl;
    
    
viewFail:
    CloseHandle(hMapping);
mapFail:
    CloseHandle(hFile);    
fileFail:
    CloseHandle(mutex);
end:
    return errorCode;
}

int main2n3(const bool addOrRem)
{
    int index, value,op, errorCode = 0;
    HANDLE hMapping, mutex = OpenMutexA(SYNCHRONIZE, // use minimum access to the mutex
        FALSE, // child processes canot inherit
        MUTEX_NAME); // get mutex named
    if (!mutex) { std::cout << "Error Eccessing the mutex" << std::endl; errorCode = GetLastError(); goto Close; }

    hMapping = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, // open the mapping for write
        FALSE, // child processes cannot inherit this
        MAP_NAME); // open mapping with this name
    if (hMapping == NULL) {
        std::cout << "Could not open the mapping" << std::endl;
        errorCode = GetLastError(); goto Close;
    }
    buffer = (Node*)MapViewOfFile(hMapping, // view of this map
        FILE_MAP_WRITE | FILE_MAP_READ, // the view is for writing & reading
        0,  // The start (high)
        0, // the start (low)
        buffSize); // the size of the buffer
    if (buffer == NULL) { // if failed
        std::cout << "Failed at map view" << std::endl;
        errorCode = GetLastError(); goto mapC;
    }

    do
    {
        std::cout << "\n\n\n"<<buffer[0].print() << std::endl;
        std::cout << "index:" << std::endl;
        std::cin >> index;
        std::cin.clear();
        if (!addOrRem) {
            std::cout << "Enter value " << std::endl;
            std::cin >> value;
            std::cin.clear();
            WaitForSingleObject(mutex, INFINITE); // wait to do the insertion without interput
                insert(buffer[0]._next, value, index);
            ReleaseMutex(mutex);
        }
        else {
            WaitForSingleObject(mutex, INFINITE); // wait to do the insertion without interput
                remove(buffer[0]._next, index);
            ReleaseMutex(mutex);
        }
        std::cout << "More? (0 for no)" << std::endl;
        std::cin >> op;
        std::cin.clear();
    } while (op && buffer->_value);

    UnmapViewOfFile(buffer);
mapC:
    CloseHandle(hMapping);
Close:
    return errorCode;
}
