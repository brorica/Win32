#pragma once

#include <windows.h>
#include <stdio.h>
#include <vector>
using namespace std;

void hardinfo(HANDLE process_handle);
DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, vector<size_t> &vector_addr);
DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, size_t addr, DWORD Value, DWORD SetValue);
DWORD second_ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, vector<size_t> &vector_addr);