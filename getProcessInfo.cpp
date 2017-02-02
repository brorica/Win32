#include "header.h"
#include <vector>
#include <iostream>
using namespace std;
#pragma comment(lib, "user32.lib")

#define ARRAY_SIZE 100     // 배열 크기
void hardinfo(HANDLE process_handle)
{
	MEMORY_BASIC_INFORMATION MBI;
	SYSTEM_INFO siSysInfo;
	DWORD *FindData = (DWORD *)malloc(ARRAY_SIZE * 4);
	DWORD Minimum_Memory=0;
	GetSystemInfo(&siSysInfo);
	vector<size_t> vector_addr;
   
	// Read Process
	DWORD Value=0;
	size_t TEST = 100;
	printf("Find Value? : ");
	scanf_s("%d", &Value);

	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				ReadProcess(process_handle, MBI, Value, vector_addr);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
	
	cout << endl << endl << "----------vector array------- " << endl;
	for (auto it_addr = vector_addr.begin(); it_addr != vector_addr.end(); it_addr++)
	{
		cout << "0x" << (PVOID)*it_addr << endl;
	}

	DWORD secondValue;
	printf("second Value : ");
	scanf_s("%d", &secondValue);
	// second search
	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				second_ReadProcess(process_handle, MBI, secondValue, vector_addr);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
	/*
	for (auto it_addr = vector_addr.begin(); it_addr != vector_addr.end(); it_addr++)
	{
		cout << *it_addr << endl;
	}
	*/

	// Write Process
	size_t addr;
	size_t SetValue;
	printf("set value : ");
	scanf_s("%d", &SetValue);
	//printf("\nTEST : 0x%p\n", &TEST);
	printf("TEST : %d\n", TEST);
	printf("chagne address : ");
	scanf_s("%x", &addr);

	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				WriteProcess(process_handle, MBI, addr, Value, SetValue);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
	//printf("TEST : %d\n", TEST);
}
