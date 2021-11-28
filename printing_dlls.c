#include<stdio.h>
#include<windows.h>
#include<winnt.h>
#include<winternl.h>


inline PEB* NtCurrentPeb() {
#ifdef _M_X64
	return (PEB*)(__readgsqword(0x60));
#elif _M_IX86
	return (PEB*)(__readfsdword(0x30));
#else
#error "This architecture is currently unsupported"
#endif
}



int main() {

	PPEB ProcessEnvironmentBlock = NtCurrentPeb();
	PPEB_LDR_DATA PebLdrData = { 0 };
	PLDR_DATA_TABLE_ENTRY LdrDataTableEntry = { 0 };
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
				LdrDataTableEntry = CONTAINING_RECORD(ForwardLink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
				if (LdrDataTableEntry)
				{
					printf("%ws\n", LdrDataTableEntry->FullDllName.Buffer);
				}
				ForwardLink = ForwardLink->Flink;
			}
		}
	}


	return 0;
}
