#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<processenv.h>
#include<shellapi.h>



#define _WIN32_WINT 0x0500

int main()

{
	wchar_t** argv;
	int nArgs;
	int i;
	argv = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == argv)
	{
		wprintf(L"CommandLineToArgvW failed\n");
		return 0;
	}
	else {
		printf("Yes commandline working\n");
	}

	// Free memory allocated for CommandLineToArgvW arguments.

	for (i = 0; i < nArgs; i++)
	{
		if (wcscmp(argv[i], L"-m") == 0)
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			printf("Peek A BOO !\n");
			Sleep(2000);
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			Sleep(5000);
			printf("Bye Bye\n");
			Sleep(1000);
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			Sleep(5000);
			printf("Welcome Back");
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
	}
	

	

	return(1);

}
