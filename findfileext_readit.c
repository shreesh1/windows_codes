#include<windows.h>
#include<stdio.h>
#include<tchar.h>
#include<shlwapi.h>
#include<strsafe.h>
#include<iostream>

#pragma comment(lib, "Shlwapi.lib")


#define BUFFER_SIZE 200
#define BUFFERSIZE 200
DWORD g_BytesTransferred = 0;

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped)
{
    _tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
    _tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
    g_BytesTransferred = dwNumberOfBytesTransfered;
}

int main() {
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR infoBuf[BUFFER_SIZE];
    HANDLE hFile;
    char   ReadBuffer[BUFFERSIZE] = { 0 };
    OVERLAPPED ol = { 0 };
    DWORD  dwBytesRead = 0;




	GetCurrentDirectory(BUFFER_SIZE, infoBuf);
	//printf("Your current directory is: %S\n", infoBuf);

    StringCchCat(infoBuf, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(infoBuf, &ffd);
    
    
    do
    {
        
            if (PathMatchSpecW(ffd.cFileName, L"*.txt")) 
            {
                _tprintf(TEXT("  %s   \n"), ffd.cFileName);
                hFile = CreateFile(ffd.cFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
                ReadFileEx(hFile, ReadBuffer, BUFFERSIZE - 1, &ol, FileIOCompletionRoutine);
                dwBytesRead = g_BytesTransferred;
                printf("%s\n", ReadBuffer);

            }
            
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);

	return 0;
}
