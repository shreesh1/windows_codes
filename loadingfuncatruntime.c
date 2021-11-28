#include<windows.h>
#include<stdio.h>



int main()
{
	typedef INT(__stdcall* FARPROC)(HWND, LPCTSTR, LPCTSTR, UINT); //need to define the function that we are calling
	
	HMODULE hLib = LoadLibraryW(L"user32.dll"); // Loading Library
	
	if (hLib == nullptr)
	{
		printf("not loaded\n");
		return EXIT_FAILURE;
	}
	
	printf("DLL Loaded!\n");

	FARPROC hFunc = (FARPROC)GetProcAddress(hLib, "MessageBoxW");
	
	if (hFunc == nullptr) {
		printf("Cant Load Function\n");
		return EXIT_FAILURE;
	}

	FreeLibrary(hLib);
	
	printf("Function Loaded\n");
	
	hFunc(NULL, L"It works!", L"Hello World!", 1); //calling the function
	
	return 0;
}
