#include <Windows.h>

#include "Utils.h"

void DebugOut(wchar_t *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}
// TODO: fromTextFile(wstring path)
// to parse object from text, interface for all rectangle object( objects, map, camera, hud, ...)
D3DCOLOR GetRGB(string r, string g, string b)
{
	int R = atoi(r.c_str());
	int G = atoi(g.c_str());
	int B = atoi(b.c_str());
	return D3DCOLOR_XRGB(R, G, B);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0;
	size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t *wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ?
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t *wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring *w = new wstring(wcstring);

	// delete wcstring   // << can I ?
	return w->c_str();
}