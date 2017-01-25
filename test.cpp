#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>

#define ARRAY_EACH_SIZE 100     // 배열 크기

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
	SYSTEM_INFO si; // 메모리 주소 최소값, 최대값을 출력
	MEMORY_BASIC_INFORMATION mbi;   // 페이지 정보 출력
	DWORD nMem = 0, i, j, result_ct = 0;        // 현재 메모리 주소 변수와 그 외 연산에 필요한 변수

	BYTE *srcArray; // 메모리에서 찾아낼 것
	BYTE *destArray;    // 메모리에서 읽어낸 것
	DWORD *FindData = (DWORD *)malloc(ARRAY_EACH_SIZE * 4);   // 찾아낸 것을 저장
	srcArray = byteArray(nFind, nSize); // 변환

	GetSystemInfo(&si);
	nMem = (DWORD)si.lpMinimumApplicationAddress; //메모리 주소의 최소값을 구한다.

	do 
	{
		// Show page Info
		if (VirtualQueryEx(hProc, (LPVOID)nMem, &mbi, sizeof(mbi)) == sizeof(mbi))
		{
			// check usable page
			if (mbi.RegionSize > 0 && mbi.Type == MEM_PRIVATE && mbi.State == MEM_COMMIT) 
			{ 
				// RegionSize : 기본 주소에서 시작하는 영역의 크기
				destArray = new BYTE[mbi.RegionSize];      // ready to read memory
				// read memory
				if (ReadProcessMemory(hProc, mbi.BaseAddress, destArray, mbi.RegionSize, NULL) != 0)
				{
					// 읽은 메모리와 찾을 메모리를 비교한다
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
								if (result_ct % ARRAY_EACH_SIZE == 0)			  // 배열의 크기를 조절한다
									FindData = (DWORD *)realloc(FindData, nSize*ARRAY_EACH_SIZE*(result_ct / ARRAY_EACH_SIZE + 1));
								FindData[result_ct] = (DWORD)mbi.BaseAddress + i; // 배열에 주소를 저장한다
								result_ct++;
							}
						}
					}
				}
				delete destArray;       // 메모리를 읽었으니 해제
			}
			nMem = (DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize;  //현재 페이지 주소 계산
		}
	} while (nMem < (DWORD)si.lpMaximumApplicationAddress);       // 최대 주소를 넘어갔으면 루프에서 빠져나옴
	delete srcArray;    // 메모리 해제
	return FindData;    // 결과값 리턴
}


void main() {
	PROCESSENTRY32 pe;
	DWORD pID = GetCurrentProcessId();
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pID); //프로세스 리스트를 뽑아낼 준비를 한다.

	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &pe); // 프로세스 검색 시작

	printf("Process List: \n");

	while (Process32Next(hSnapShot, &pe)) { // 프로세스 검색
		HANDLE k = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		if (k != 0)
			printf("    [%d]%ws\n", pe.th32ProcessID, pe.szExeFile);
		CloseHandle(k);
	}
	CloseHandle(hSnapShot); // 프로세스 검색 끝

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