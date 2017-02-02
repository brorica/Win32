#include "header.h"
#include <Tlhelp32.h>
#include <stdlib.h>

int ProcessList(HANDLE hSnapShot, PROCESSENTRY32 pe);

int ProcessList(HANDLE hSnapShot, PROCESSENTRY32 pe)
{ 
	DWORD PID;
	while (Process32Next(hSnapShot, &pe))
	{
		HANDLE k = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pe.th32ProcessID);
		if (k != 0)
			printf("[%d]%ls\n", pe.th32ProcessID, pe.szExeFile);
		CloseHandle(k);
	}
	printf("\nProcess ID : ");
	scanf_s("%d", &PID);
	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	hardinfo(process_handle);

	return 0;
}

int main()
{
	PROCESSENTRY32 pe;
	DWORD pID = GetCurrentProcessId();
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pID);

	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &pe);

	printf("Process List: \n");
	ProcessList(hSnapShot, pe);
	printf("\n");
	CloseHandle(hSnapShot);
	return 0;
}