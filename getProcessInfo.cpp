#include "header.h"
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
	list<size_t> addressList;
	size_t nextCheck = 0;
	do
	{
		system("cls");
		// -----------------------------  first search -------------------------------
		DWORD Value=0;
		printf("Find Value? : ");
		scanf_s("%d", &Value);

		Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
		do {
			if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
			{
				if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
					ReadProcess(process_handle, MBI, Value, addressList);
				Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
			}
		} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
	
		cout << endl << endl << "----------list array------- " << endl;
		// -----------------------------  second search -------------------------------
		DWORD nextValue;
		printf("Next Value : ");
		scanf_s("%d", &nextValue);
		
		Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
		do {
			if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
			{
				if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
					NextReadProcess(process_handle, MBI, nextValue, addressList);
				Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
			}
		} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
		//for (auto it_addr = addressList.begin(); it_addr != addressList.end(); it_addr++)
		//	cout << *it_addr << endl;
		printf("Next? (1 or any key) No? (0) : ");
		scanf_s("%d", &nextCheck);
	} while (nextCheck);

	// Write Process
	size_t addr;
	size_t SetValue;
	printf("set value : ");
	scanf_s("%d", &SetValue);
	printf("chagne address : ");
	scanf_s("%x", &addr);

	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				WriteProcess(process_handle, MBI, addr, SetValue);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize;
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress);
}
