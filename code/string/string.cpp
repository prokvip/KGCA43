#define _CRT_SECURE_NO_WARNINGS
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <tchar.h>//_tcscpy
//#include <windows.h>
#include <atlconv.h> // A2W

#define tUNICODE
void fun(char* a)
{

}
//static std::wstring mtw(std::string str)
//{
//    std::wstring ret = std::wstring(str.begin(), str.end());
//    return  ret;
//}
//static std::string wtm(std::wstring str)
//{
//    return  std::string(str.begin(), str.end());
//}

//template <class _FwdIt, class _Fn>
//void generateA(_FwdIt _First, _FwdIt _Last, _Fn _Func)
//{
//    for (; _First != _Last; ++_First)
//    {
//        *_First = _Func();
//    }
//}
//template <class INPUT1, class _OutputIt, class _Fn>
//_OutputIt transformA(INPUT1 _First, INPUT1 _Last, _OutputIt _Result, _Fn _Func)
//{
//    for (; _First != _Last; ++_First, ++_Result)
//    {
//        *_Result = _Func(_First);
//    }
//    return _Result;
//}
//template <class INPUT1, class INPUT2, class _OutputIt, class _Fn>
//_OutputIt transformB(INPUT1 _First, INPUT1 _Last,
//                     INPUT2 _First2,  
//                     _OutputIt _Result, _Fn _Func)
//{
//    for (; _First != _Last; ++_First, ++_First2, ++_Result)
//    {
//        *_Result = _Func(_First, _First2);
//    }
//    return _Result;
//}
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


int Fun(const int& data)
{     
   return data + 1;
}
int FunRef()
{
    return 1;
}
int main()
{
//using string = basic_string<char, char_traits<char>, allocator<char>>;
//using wstring = basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>>;

    std::string a;
    std::wstring b;
    std::basic_string<char> c;
    std::basic_string<wchar_t> d;
    //std::basic_string<CHAR> c;
    //std::basic_string<WCHAR> d;

// // 영어,문자=>1바이트로 표현, 
// // 한글 => 2바이트로 표현
// // 1)싱글바이트 코드 // 모든문자를 1바이트로 표현    //   
// // 2)멀티바이트 코드 // 1 또는 2바이트로 표현
// // 3)(와이드)유니코드// 모든문자를 2바이트로 표현
//   멀티바이트 용도의 문자열 함수가 있고
//   유니코드용도의 분자열 함수가 있다.
// //멀티바이트 코드(std::string)
#ifndef tUNICODE
    std::string str1 = "abcd";    
    size_t len = str1.capacity();    
    std::string str2 = "ABCD";
    std::string str3 = str1 + " " + str2;
    int iNumLength = str1.size() * sizeof(char); 

    std::string::size_type pos = str3.find(" ");
    std::string str4 = str3.substr(pos+1);
    std::string str5 = str3.substr(0, pos);


    str1.push_back('K');

    std::cout << str1 << std::endl;
    if (str1 == str2)// strcmp(a,b)
    {
        std::cout << "str1 == str2" << std::endl;
    }
    if (str1 != str2)
    {
        std::cout << "str1 != str2" << std::endl;
    }
    if (str1 > str2)
    {
        std::cout << "str1 > str2" << std::endl;
    }
    const char* pStr1 = str1.c_str();
    std::cout << str1 << std::endl;
#else
    std::wstring str1(L"abcd");
    std::wstring str2(L"ABCD");
    const wchar_t* cs = str1.c_str();
    wchar_t* pcs = const_cast<wchar_t*>(cs);    
    std::wstring str3 = str1 + L" " + str2;
    
    int iNumLength = str3.size() * sizeof(str3[0]);
    char* copyString = new char[iNumLength];
    std::string strcopy = to_wm(str3);
    strcpy(copyString, strcopy.c_str());

    auto pos = str3.find(L" ");
    auto str4 = str3.substr(pos + 1);
    auto str5 = str3.substr(0, pos);
   
    
    //  모든 요소를 함수의 인자값으로 호출한다. 그리고 결과값에 저장한다.
      
    //void'에서 'wchar_t'(으)로 변환할 수 없습니다
    std::transform(str3.begin(), str3.end(),
                   str3.begin(),
                   std::toupper);
    std::wstring result;
    
    result.reserve(30);
    //push_front': 'std::basic_string < wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>'의 멤버가 아닙니다.
    std::transform( str3.begin(), str3.end(),        
                    std::back_inserter(result), 
                    std::toupper);
    std::transform(str3.begin(), str3.end(),
        std::back_inserter(result),
        [](int i) -> int
        {
            return i + 1;
        });

    result.clear();
    std::transform(str3.begin(), str3.end(),
        std::back_inserter(result),
        Fun);
    result.clear();
    //error C2780 : 'unknown-type std::plus<void>::operator ()(_Ty1 &&,_Ty2 &&) noexcept(<expr>) const' : 인수가 2개 있어야 하는데 1개를 사용했습니다.
    std::wstring vecListA;
    vecListA.push_back(65);
    vecListA.push_back(66);
    vecListA.push_back(67);
    std::wstring vecListB;
    vecListB.push_back(65);
    vecListB.push_back(66);
    vecListB.push_back(67);
    
    //'_OutIt'이(가) 이 연산자를 정의하지 않거나 미리 정의된 
    std::transform( vecListA.begin(), vecListA.end(),
                    vecListB.begin(), 
                    vecListB.begin(),
                    std::plus<>{} );// "a + b"
    std::vector<int> vecListResult;
    std::transform(vecListA.begin(), vecListA.end(),
                    std::back_inserter(vecListResult),
                    Fun);// "a+1"
    
    // 모든요소를 순회하면서 함수의 결과로 체운다. 
    std::string v1 = "aaa";
    std::generate(v1.begin(), v1.end(), FunRef);
    /*struct TData
    {
        int ivalue;
    };*/
    //std::vector<TData*> v2(2);
    //std::generate(v2.begin(), v2.end(), []() { return new TData; });

    std::wcout << str1 << std::endl;
    if (str1 == str2)// strcmp(a,b)
    {
        std::cout << "str1 == str2" << std::endl;
    }
    if (str1 != str2)
    {
        std::cout << "str1 != str2" << std::endl;
    }
    if (str1 > str2)
    {
        std::wcout << "str1 > str2" << std::endl;
    }
    const wchar_t* pStr1 = str1.c_str();
    std::wcout << str1 << std::endl;

#endif
    std::cout << "Hello World!\n";
}
