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
				printf("0x%x\t%d\n", (unsigned int)MBI.BaseAddress, *(DWORD*)((char*)readArray + i));
				return 0; // 찾으면 read process 탈출
			}
		}
	}
	return 0;
}
DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD addr, DWORD Value)
{
	BYTE *readArray;   // 메모리에서 읽어낸 것
	readArray = new BYTE[MBI.RegionSize]; // 4byte
	DWORD SetValue = 111;
	//printf("Set Value : ");
	//scanf_s("%d", &SetValue);
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (unsigned int i = 0; i < (DWORD)MBI.RegionSize; i++)
		{
			if ((unsigned int)MBI.BaseAddress == addr && *(DWORD*)((char*)readArray + i) == Value)
			{
				if (WriteProcessMemory(process_handle, MBI.BaseAddress, (LPCVOID)SetValue, sizeof(DWORD), NULL) == 0)
					printf("failed!\n");
				//printf("0x%x\t%d\n", (unsigned int)MBI.BaseAddress, *(DWORD*)((char*)readArray + i));
				return 0; // 찾으면 read process 탈출
			}
		}
	}
	return 0;
}