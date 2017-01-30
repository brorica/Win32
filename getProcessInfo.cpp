#include "header.h"
#pragma comment(lib, "user32.lib")

#define ARRAY_SIZE 100     // 배열 크기

void hardinfo(HANDLE process_handle)
{
	MEMORY_BASIC_INFORMATION MBI;
	SYSTEM_INFO siSysInfo;
	DWORD *FindData = (DWORD *)malloc(ARRAY_SIZE * 4);
	DWORD Minimum_Memory=0;
	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo);
	// Read Process
	DWORD Value=719;
	//printf("Find Value? : ");
	//scanf_s("%d", &Value);
	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				ReadProcess(process_handle, MBI, Value);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize; // 현재까지의 페이지 크기를 할당
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress); // 마지막 페이지면 반복문 탈출

	// Write Process
	DWORD addr=0;
	printf("change address's value? : ");
	scanf_s("%x", &addr);

	Minimum_Memory = (DWORD)siSysInfo.lpMinimumApplicationAddress;
	do {
		if (VirtualQueryEx(process_handle, (LPVOID)Minimum_Memory, &MBI, sizeof(MBI)) == sizeof(MBI))
		{
			if (MBI.Type == MEM_PRIVATE && MBI.State == MEM_COMMIT && MBI.RegionSize > 0)
				WriteProcess(process_handle, MBI, addr, Value);
			Minimum_Memory = (DWORD)MBI.BaseAddress + (DWORD)MBI.RegionSize; // 현재까지의 페이지 크기를 할당
		}
	} while (Minimum_Memory < (DWORD)siSysInfo.lpMaximumApplicationAddress); // 마지막 페이지면 반복문 탈출
}
