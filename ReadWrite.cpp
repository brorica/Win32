#include "header.h"
#include <Psapi.h>

DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value)
{
	BYTE *readArray;   // 메모리에서 읽어낸 것
	readArray = new BYTE[MBI.RegionSize]; // 4byte
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (unsigned int i=0; i < (DWORD)MBI.RegionSize; i++)
		{
			if (*(DWORD*)((char*)readArray + i) == Value)
			{		
				printf("0x%x\t%d\n", ((unsigned int)MBI.BaseAddress+i), *(DWORD*)((char*)readArray + i));
			}
		}
	}
	delete readArray;
	return 0;
}


DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD addr, DWORD Value)
{
	BYTE *readArray;   // 메모리에서 읽어낸 것
	readArray = new BYTE[MBI.RegionSize]; // 4byte
	DWORD SetValue = 1234;
	//printf("Set Value : ");
	//scanf_s("%d", &SetValue);
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (unsigned int i = 0; i < (DWORD)MBI.RegionSize; i++)
		{
			if ((unsigned int)MBI.BaseAddress == addr && *(DWORD*)((char*)readArray + i) == Value)
			{
			/*
				DWORD VirtualAddr = 0;
				if((VirtualAddr = (DWORD)VirtualAllocEx(process_handle, NULL, sizeof(DWORD), MEM_COMMIT, PAGE_READWRITE)) == 0)
					printf("failed!\n");
			 */

				if (WriteProcessMemory(process_handle, (LPVOID)addr, (LPCVOID)&SetValue, sizeof(DWORD), NULL) == 0)
					printf("WriteProcessMemory failed!\n");
				//printf("0x%x\t%d\n", (unsigned int)MBI.BaseAddress, *(DWORD*)((char*)readArray + i));
				/*
				if (WriteProcessMemory(process_handle, MBI.BaseAddress, (LPCVOID)SetValue, sizeof(DWORD), NULL) == 0)
					printf("failed!\n");
				*/
				//VirtualFree(process_handle, sizeof(DWORD), MEM_DECOMMIT);
				delete readArray;
				return 0; // 찾으면 read process 탈출
			}
		}
	}
	delete readArray;
	return 0;
}