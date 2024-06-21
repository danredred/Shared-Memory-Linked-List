// Bonus Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#define APP "BonusLinkedList.exe"
#define BUFF_SIZE 20

int main()
{
    int errorCode = 0;
    STARTUPINFOA si;
    PROCESS_INFORMATION piAdder, piRemover, piSetter;
    GetStartupInfoA(&si);
    char cBuff[BUFF_SIZE];
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&piSetter, sizeof(piSetter));
    ZeroMemory(&piAdder, sizeof(piAdder));
    ZeroMemory(&piRemover, sizeof(piRemover));
    strcpy_s(cBuff, BUFF_SIZE, "1");
    if (!CreateProcessA(APP, //the app to run process
        cBuff, // the arguments
        NULL, //something security 
        NULL, // ___'____ ___'___
        FALSE, // canot take this pro's hendles
        CREATE_NEW_CONSOLE, // create new console
        NULL, // run this enviro
        NULL, // run this dir
        &si, // secur.. info
        &piSetter))//se..pro.. info
    {  // case error
        std::cout << "Not good" << std::endl;
        errorCode = GetLastError();
        goto end;
    }
    strcpy_s(cBuff, BUFF_SIZE, "2");
    if (!CreateProcessA(APP, //the app to run process
        cBuff, // the arguments
        NULL, //something security 
        NULL, // ___'____ ___'___
        FALSE, // canot take this pro's hendles
        CREATE_NEW_CONSOLE, // create new console
        NULL, // run this enviro
        NULL, // run this dir
        &si, // secur.. info
        &piRemover))//se..pro.. info
    {  // case error
        std::cout << "Not good" << std::endl;
        errorCode = GetLastError();
        goto remEnd;
    }
    strcpy_s(cBuff, BUFF_SIZE, "3");
    if (!CreateProcessA(APP, //the app to run process
        cBuff, // the arguments
        NULL, //something security 
        NULL, // ___'____ ___'___
        FALSE, // canot take this pro's hendles
        CREATE_NEW_CONSOLE, // create new console
        NULL, // run this enviro
        NULL, // run this dir
        &si, // secur.. info
        &piAdder))//se..pro.. info
    {  // case error
        std::cout << "Not good" << std::endl;
        errorCode = GetLastError();
        goto remEnd;
    }


    WaitForSingleObject(piRemover.hThread, INFINITE);
    WaitForSingleObject(piAdder.hThread, INFINITE);

    if (WaitForSingleObject(piSetter.hThread, 3) == WAIT_TIMEOUT) {
        TerminateProcess(piSetter.hProcess, ERROR_SEM_TIMEOUT); // if the others were closed and this not close this
    }

    CloseHandle(piAdder.hProcess);
    CloseHandle(piAdder.hThread);
addEnd:
    CloseHandle(piRemover.hProcess);
    CloseHandle(piRemover.hThread);
remEnd:
    CloseHandle(piSetter.hProcess);
    CloseHandle(piSetter.hThread);
end:
    return errorCode;
}