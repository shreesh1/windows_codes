#include<windows.h>
#include<stdio.h>



int main()
{
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD dwExportDirRVA;
	PDWORD pdwFunctionNameBase;
	PCSTR pFunctionName;
	DWORD dwNumFunctions;


	typedef INT(__stdcall* FARPROC)(HWND, LPCTSTR, LPCTSTR, UINT); //need to define the function that we are calling
	
	HMODULE hLib = LoadLibraryW(L"user32.dll"); // Loading Library
	
	if (hLib == nullptr)
	{
		printf("not loaded\n");
		return EXIT_FAILURE;
	}
	
	printf("DLL Loaded!\n");
	
	pNTHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)hLib + ((PIMAGE_DOS_HEADER)hLib)->e_lfanew);
	dwExportDirRVA = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)hLib + dwExportDirRVA);
	dwNumFunctions = pExportDir->NumberOfNames;
	printf("%d", dwNumFunctions);
	pdwFunctionNameBase = (PDWORD)((PCHAR)hLib + pExportDir->AddressOfNames);
	for (int i = 0; i < dwNumFunctions; i++) {
		pFunctionName = (PCSTR)(*pdwFunctionNameBase + (ULONG_PTR)hLib);
		printf("%s\n", pFunctionName);
		pdwFunctionNameBase++;

	}
	/*
	FARPROC hFunc = (FARPROC)GetProcAddress(hLib, "MessageBoxW");
	
	if (hFunc == nullptr) {
		printf("Cant Load Function\n");
		return EXIT_FAILURE;
	}

	FreeLibrary(hLib);
	
	printf("Function Loaded\n");
	
	hFunc(NULL, L"It works!", L"Hello World!", 1); //calling the function
	*/
	return 0;
}
