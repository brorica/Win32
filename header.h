#pragma once

#include <windows.h>
#include <stdio.h>

void hardinfo(HANDLE process_handle);
DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value);
DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD addr, DWORD Value);