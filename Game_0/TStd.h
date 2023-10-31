#pragma once
#include <iostream> 
#include <vector> 
#include <list> 
#include <map> 
#include <string> 
#include <atlconv.h> // A2W

using namespace std;

static std::wstring to_mw(const std::string& _src)
{
    USES_CONVERSION;
    return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
    USES_CONVERSION;
    return std::string(W2A(_src.c_str()));
};

using C_STR = std::basic_string<char>;  //  멀티바이트(영문:1바이트, 한글:2바이트) 
using W_STR = std::basic_string<wchar_t>;// 유니코드(무조건 2바이트로 문자 표현)
