#include "header.h"
#include <iostream>
using namespace std;
#pragma comment(lib, "user32.lib")

void getProcessInfo(HANDLE process_handle)
{
	MEMORY_BASIC_INFORMATION MBI;
	SYSTEM_INFO siSysInfo;
	DWORD Minimum_Memory = 0;
	GetSystemInfo(&siSysInfo);
	list<size_t> addressList;
	size_t nextCheck = 0;
	system("cls");
	// -----------------------------  first search -------------------------------
	DWORD Value = 0;
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

	// -----------------------------  second search -------------------------------
	DWORD nextValue;
	do
	{
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
		printf("Next? (1 or any key) No? (0) : ");
		scanf_s("%d", &nextCheck);
	} while (nextCheck);

	// -----------------------------  write process -------------------------------
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
