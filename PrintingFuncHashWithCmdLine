#include<stdio.h>
#include<windows.h>
#include<winnt.h>
#include<winternl.h>
#include<shellapi.h>

#define RESOLVE_NAME_MAX 4096

typedef struct _MY_LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} MY_LDR_DATA_TABLE_ENTRY, * PMY_LDR_DATA_TABLE_ENTRY;

UINT32 resolve_hash_name(LPCSTR cszName)
{
	if (cszName == NULL)
	{
		return 0;
	}

	SIZE_T uNameLen = strnlen_s(cszName, RESOLVE_NAME_MAX);
	if (uNameLen == 0)
	{
		return 0;
	}

	UINT32 u32Hash = 0, u32Buf = 0;
	PBYTE pbData = (PBYTE)cszName;
	INT iRemain = (uNameLen & 3);

	uNameLen >>= 2;

	for (SIZE_T i = uNameLen; i > 0; i--)
	{
		u32Hash += *(const UINT16*)pbData;
		u32Buf = (*(const UINT16*)(pbData + 2) << 11) ^ u32Hash;
		u32Hash = (u32Hash << 16) ^ u32Buf;
		pbData += (2 * sizeof(UINT16));
		u32Hash += u32Hash >> 11;
	}

	switch (iRemain)
	{
	case 1:
		u32Hash += *pbData;
		u32Hash ^= u32Hash << 10;
		u32Hash += u32Hash >> 1;
		break;

	case 2:
		u32Hash += *(const UINT16*)pbData;
		u32Hash ^= u32Hash << 11;
		u32Hash += u32Hash >> 17;
		break;

	case 3:
		u32Hash += *(const UINT16*)pbData;
		u32Hash ^= u32Hash << 16;
		u32Hash ^= pbData[sizeof(UINT16)] << 18;
		u32Hash += u32Hash >> 11;
		break;
	}

	u32Hash ^= u32Hash << 3;
	u32Hash += u32Hash >> 5;
	u32Hash ^= u32Hash << 4;
	u32Hash += u32Hash >> 17;
	u32Hash ^= u32Hash << 25;
	u32Hash += u32Hash >> 6;

	return u32Hash;
	}



inline PEB* NtCurrentPeb() {
#ifdef _M_X64
	return (PEB*)(__readgsqword(0x60));
#elif _M_IX86
	return (PEB*)(__readfsdword(0x30));
#else
#error "This architecture is currently unsupported"
#endif
}

void PrintFuncHash()
{
	PPEB ProcessEnvironmentBlock = NtCurrentPeb();
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD dwExportDirRVA;
	PDWORD pdwFunctionNameBase;
	PCSTR pFunctionName;
	DWORD dwNumFunctions;
	PPEB_LDR_DATA PebLdrData = { 0 };
	PMY_LDR_DATA_TABLE_ENTRY LdrDataTableEntry = { 0 };
	PLIST_ENTRY ModuleList = { 0 },
		ForwardLink = { 0 };

	if (ProcessEnvironmentBlock)
	{
		PebLdrData = ProcessEnvironmentBlock->Ldr;
		if (PebLdrData)
		{
			ModuleList = &PebLdrData->InMemoryOrderModuleList;
			ForwardLink = ModuleList->Flink;
			while (ModuleList != ForwardLink)
			{
				LdrDataTableEntry = CONTAINING_RECORD(ForwardLink, MY_LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
				if (LdrDataTableEntry)
				{
					if (_wcsicmp(LdrDataTableEntry->BaseDllName.Buffer, L"kernel32.dll") == 0) {
						pNTHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)LdrDataTableEntry->DllBase + ((PIMAGE_DOS_HEADER)LdrDataTableEntry->DllBase)->e_lfanew);
						dwExportDirRVA = pNTHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
						pExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)LdrDataTableEntry->DllBase + dwExportDirRVA);
						dwNumFunctions = pExportDir->NumberOfNames;
						pdwFunctionNameBase = (PDWORD)((PCHAR)LdrDataTableEntry->DllBase + pExportDir->AddressOfNames);
						for (int i = 0; i < dwNumFunctions; i++) {
							pFunctionName = (PCSTR)(*pdwFunctionNameBase + (ULONG_PTR)LdrDataTableEntry->DllBase);
							printf("%x\n", resolve_hash_name((LPCSTR)pdwFunctionNameBase));
							pdwFunctionNameBase++;

						}
					}
					
				}
				ForwardLink = ForwardLink->Flink;
			}
		}
	}
}


int main() {

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
				Sleep(1000);
				PrintFuncHash();
			}
		}
	}
	
	LocalFree(argv);



	return(1);
}
