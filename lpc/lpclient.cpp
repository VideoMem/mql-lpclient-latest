#include "lpclient.hpp"
#include "lpclient/lpcapi.hpp"

int_t LPCCALL int_echo(int_t a) {
    return a;
}

size_t LPCCALL string_echo(wchar_t* src, wchar_t* dst) {
    string stdstring;
    string_marshall(src, stdstring);
    wchar_t wstring[string_size(stdstring)] = {0};
    string_unmarshall(stdstring, wstring);
    wcscpy(dst,wstring);
    dst[wcslen(src)] = 0;
    return wcslen(src);
}

size_t LPCCALL char_echo(const char* src, char* dst, int_t len) {
    strncpy(dst, src, len);
    dst[len] = 0;
}