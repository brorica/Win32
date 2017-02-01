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
				printf("0x%x\t%d\n", ((unsigned int)MBI.BaseAddress+i), *(DWORD*)((char*)readArray + i));
		}
	}
	delete readArray;
	return 0;
}


DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD addr, DWORD Value)
{
	BYTE *readArray;   // 메모리에서 읽어낸 것
	readArray = new BYTE[MBI.RegionSize]; // 4byte
	
	DWORD SetValue = 10;
	DWORD original_Value=0;
	DWORD NewProtect_Value= PAGE_EXECUTE_READWRITE;
	//printf("Set Value : ");
	//scanf_s("%d", &SetValue);
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (unsigned int i = 0; i < (DWORD)MBI.RegionSize; i++)
		{
			if ( (DWORD)MBI.BaseAddress + i == addr )
			{
				VirtualProtectEx(process_handle, MBI.BaseAddress,MBI.RegionSize, NewProtect_Value, &original_Value);
				WriteProcessMemory(process_handle, (LPVOID)addr, (LPCVOID)&SetValue, sizeof(DWORD), NULL);
				NewProtect_Value = original_Value;
				VirtualProtectEx(process_handle, MBI.BaseAddress,MBI.RegionSize,  original_Value, &NewProtect_Value);
				
				delete readArray;
				return 0; // 찾으면 read process 탈출
			}
		}
	}
	delete readArray;
	return 0;
}