#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>

#define ARRAY_EACH_SIZE 100     // size of array

BYTE *byteArray(unsigned int n, unsigned int size) {
	BYTE *arr = new BYTE[size];
	unsigned int i;
	for (i = 0; i<size; i++) {
		arr[i] = n % 256;
		n /= 256;
	}
	return arr;
}

PDWORD FindMem(HANDLE hProc, unsigned int nFind, unsigned int nSize) {
	SYSTEM_INFO si; // print memory min, max address
	MEMORY_BASIC_INFORMATION mbi;   // print page info
	DWORD nMem = 0, i, j, result_ct = 0;        // current memory address variable and etc

	BYTE *srcArray; // find it in memory
	BYTE *destArray;    // read from memory
	DWORD *FindData = (DWORD *)malloc(ARRAY_EACH_SIZE * 4);   // save the found
	srcArray = byteArray(nFind, nSize); // convert

	GetSystemInfo(&si);
	nMem = (DWORD)si.lpMinimumApplicationAddress; // find the minimum value in memory

	do 
	{
		// Show page Info
		if (VirtualQueryEx(hProc, (LPVOID)nMem, &mbi, sizeof(mbi)) == sizeof(mbi))
		{
			// check usable page
			if (mbi.RegionSize > 0 && mbi.Type == MEM_PRIVATE && mbi.State == MEM_COMMIT) 
			{ 
				// RegionSize : starting from base address
				destArray = new BYTE[mbi.RegionSize];      // ready to read memory
				// read memory
				if (ReadProcessMemory(hProc, mbi.BaseAddress, destArray, mbi.RegionSize, NULL) != 0)
				{
					for (i = 0; i < (DWORD)mbi.RegionSize; i++) // (DWORD)mbi.RegionSize : 4096 (default)
					{
						for (j = 0; j < nSize; j++) 
						{
							if (i + nSize + 1 > mbi.RegionSize)	// don't over the page size
								break;

							if (destArray[i + j] != srcArray[j])
								break;
							else if (j == nSize - 1)  // 값을 찾아냄
							{
								if (result_ct % ARRAY_EACH_SIZE == 0)			  // resize array
									FindData = (DWORD *)realloc(FindData, nSize*ARRAY_EACH_SIZE*(result_ct / ARRAY_EACH_SIZE + 1));
								FindData[result_ct] = (DWORD)mbi.BaseAddress + i; // store address in array
								result_ct++;
							}
						}
					}
				}
				delete destArray;
			}
			nMem = (DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize;  // calculate current page address
		}
	} while (nMem < (DWORD)si.lpMaximumApplicationAddress);       // escpae the loop when over the maximum address
	delete srcArray;
	return FindData; 
}


void main() {
	PROCESSENTRY32 pe;
	DWORD pID = GetCurrentProcessId();
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pID); //프로세스 리스트를 뽑아낼 준비를 한다.

	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &pe); // start process searching

	printf("Process List: \n");

	while (Process32Next(hSnapShot, &pe)) { // searching process
		HANDLE k = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		if (k != 0)
			printf("    [%d]%ws\n", pe.th32ProcessID, pe.szExeFile);
		CloseHandle(k);
	}
	CloseHandle(hSnapShot);

	DWORD nFind;
	DWORD nSize;

	printf("Select Process(Id): ");
	scanf_s("%d", &pID);
	printf("Value to Find: ");
	scanf_s("%d", &nFind);
	printf("Size of Value: ");
	scanf_s("%d", &nSize);

	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	DWORD *p = FindMem(h, nFind, nSize);

	for (int i = 0; i<200; i++)
		printf("%d: 0x%X\n", i + 1, p[i]);
	printf("And %d Addresses more..\n", _msize(p) - 200);

	free(p); //delete p;
}