#include "header.h"
#include <iostream>
#include <Psapi.h>

DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, vector<size_t> &vector_addr)
{
	int v = 0;
	auto readArray = new BYTE[MBI.RegionSize];
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (size_t i=0; i < MBI.RegionSize; ++i)
		{
			if (readArray[i] == Value)
			{
				vector_addr.push_back((size_t)MBI.BaseAddress + i);
			//	printf("0x%p\t%d\n", vector_addr[v++], readArray[i]);
			}
		}
	}
	delete[] readArray;
	return 0;
}

DWORD second_ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, vector<size_t> &vector_addr)
{
	size_t size = vector_addr.size();
	size_t i = 0;
	auto readArray = new BYTE[MBI.RegionSize];
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (auto it_addr = vector_addr.begin(); it_addr != vector_addr.end(); it_addr++)
		{
			// bigger than base address
			if (((size_t)MBI.BaseAddress <= *it_addr))
			{
				// bigger than page size
				if (*it_addr >= ((size_t)MBI.BaseAddress + MBI.RegionSize))
					continue;
				else
					if (readArray[*it_addr - (size_t)MBI.BaseAddress] == Value)
						printf("0x%p\t%d\n", *it_addr, readArray[*it_addr- (size_t)MBI.BaseAddress]);
					
			}
			// lower than base address
			else
				continue;
		}
	}
	delete[] readArray;
	return 0;
}


DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, size_t addr, DWORD Value, DWORD SetValue)
{
	auto readArray = new BYTE[MBI.RegionSize]; // 4byte
	
	DWORD original_Protect=0;
	DWORD NewProtect_Protect = PAGE_EXECUTE_READWRITE;
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, nullptr) != 0)
	{
		for (size_t i = 0; i < MBI.RegionSize; ++i)
		{
			if ((size_t)MBI.BaseAddress+i == addr )
			{
				VirtualProtectEx(process_handle, MBI.BaseAddress,MBI.RegionSize, NewProtect_Protect, &original_Protect);
				WriteProcessMemory(process_handle, (LPVOID)addr, (LPCVOID)&SetValue, sizeof(DWORD), nullptr);
				NewProtect_Protect = original_Protect;
				VirtualProtectEx(process_handle, MBI.BaseAddress,MBI.RegionSize, original_Protect, &NewProtect_Protect);		
				delete[] readArray;
				return ((DWORD)MBI.BaseAddress + i);
			}
		}
	}
	delete[] readArray;
	return 0;
}