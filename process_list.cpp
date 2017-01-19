#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <malloc.h>
#include <stdlib.h>
#include <tchar.h>  

int main()
{
	PROCESSENTRY32 pe;
	DWORD pID = GetCurrentProcessId();
	//ready to extract process name
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pID); 

	printf("process name : (do not write '.exe') : ");
	char process_name[256];
	fgets(process_name, (sizeof(process_name) - 1), stdin);

	// process name match correctly at w_char type
	size_t wstring_newsize = strlen(process_name) + 1;
	size_t convertedChars = 0;
	wchar_t * wcstring = new wchar_t[wstring_newsize];

	mbstowcs_s(&convertedChars, wcstring, (wstring_newsize + 1), process_name, _TRUNCATE);

	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapShot, &pe); // process search start

	printf("Process List: \n");

	while (Process32Next(hSnapShot, &pe))
	{ // process searching
		HANDLE k = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pe.th32ProcessID);

		if (k != 0)
		{
			if (wcsncmp(wcstring, pe.szExeFile, (wcslen(wcstring)-1)) == 0)
				printf("[%d]%ls\n", pe.th32ProcessID, pe.szExeFile);
		}
		CloseHandle(k);
	}
	CloseHandle(hSnapShot); // process search end
}