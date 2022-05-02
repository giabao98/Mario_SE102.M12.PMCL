#pragma once

#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <d3d9types.h>


using namespace std;

void DebugOut(wchar_t *fmt, ...);
D3DCOLOR GetRGB(string r, string g, string b);
vector<string> split(string line, string delimeter = "\t");

wstring ToWSTR(string st);
LPCWSTR ToLPCWSTR(string st);

