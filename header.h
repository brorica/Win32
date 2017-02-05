#pragma once

#include <windows.h>
#include <stdio.h>
#include <list>
using namespace std;

void getProcessInfo(HANDLE process_handle);
DWORD ReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, list<size_t> &addressList);
DWORD WriteProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, size_t addr, DWORD SetValue);
DWORD NextReadProcess(HANDLE process_handle, MEMORY_BASIC_INFORMATION MBI, DWORD Value, list<size_t> &addressList);