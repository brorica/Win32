#include "header.h"
#include <iostream>
#include <Psapi.h>

DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, list<size_t> &addressList)
{
	auto readArray = new BYTE[MBI.RegionSize];
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (size_t i=0; i < MBI.RegionSize; ++i)
		{
			if (readArray[i] == Value)
				addressList.push_back((size_t)MBI.BaseAddress + i);
		}
	}
	delete[] readArray;
	return 0;
}

DWORD NextReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, list<size_t> &addressList)
{
	size_t size = addressList.size();
	size_t i = 0;
	auto readArray = new BYTE[MBI.RegionSize];
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, NULL) != 0)
	{
		for (auto it_addr = addressList.begin(); it_addr != addressList.end(); it_addr++)
		{
			// bigger than base address
			if (((size_t)MBI.BaseAddress <= *it_addr))
			{ // bigger than total page size		
				if (*it_addr <= ((size_t)MBI.BaseAddress + MBI.RegionSize))
					if (readArray[*it_addr - (size_t)MBI.BaseAddress] == Value)
							printf("0x%p\t%d\n",*it_addr, readArray[*it_addr- (size_t)MBI.BaseAddress]);					
			}
			// lower than base address
		}
	}
	delete[] readArray;
	return 0;
}


DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, size_t addr, DWORD SetValue)
{
	auto readArray = new BYTE[MBI.RegionSize]; // 4byte
	
	DWORD original_Protect=0;
	DWORD NewProtect_Protect = PAGE_EXECUTE_READWRITE;
	if (ReadProcessMemory(process_handle, MBI.BaseAddress, readArray, MBI.RegionSize, nullptr) != 0)
	{
		// bigger than BaseAddress
		if (addr > size_t(MBI.BaseAddress))
		{
			// lower than total page size
			if (addr < size_t(MBI.BaseAddress) + MBI.RegionSize)
			{
				VirtualProtectEx(process_handle, MBI.BaseAddress, MBI.RegionSize, NewProtect_Protect, &original_Protect);
				WriteProcessMemory(process_handle, (LPVOID)addr, (LPCVOID)&SetValue, sizeof(DWORD), nullptr);
				NewProtect_Protect = original_Protect;
				VirtualProtectEx(process_handle, MBI.BaseAddress, MBI.RegionSize, original_Protect, &NewProtect_Protect);
				printf("Set Value success!\n");
				delete[] readArray;
				return 0;
			}
		}
	}
	delete[] readArray;
	return 0;
}