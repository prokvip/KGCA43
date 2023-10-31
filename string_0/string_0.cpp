#define _CRT_SECURE_NO_WARNINGS
#include <tchar.h>
#include <iostream>
#include <string>
#include <atlconv.h> // A2W
//https://learn.microsoft.com/ko-kr/cpp/standard-library/basic-string-class?view=msvc-170
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
// string = KGCA대한 [ 8+ null] = 9  [K,G,C,A,ㄷ,ㅐ,ㅎ,안]
// wstring = 6*2+null = 13;          [K,0,G,0,C,0,A,0,ㄷ,ㅐ,ㅎ,안]
int main()
{
    char data[] = "100";
    int iData = stoi(data);
    float fData = stof(data);
    std::string szData1 = std::to_string(iData);
    std::string szData2 = std::to_string(fData);

    std::string strData = "kgca대한민국";
    std::wstring wString = to_mw(strData);
    std::string mString = to_wm(wString);
    
    

    /// <summary>
    /// 일반 문자열 
    /// </summary>
    /// <returns></returns>
    char AString[10] = "KGCA";
    char BString[10] = "kgca";
    char copyString[10];
    int  Asize = strlen(AString); // 널문자 제외.
    //copyString = AString; // error
    //error C4996 : 'strcpy' : This function or variable may be unsafe.
    //Consider using strcpy_s instead.
    //To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    strcpy(copyString, BString);
    
    char* pointCopyString = copyString;
    //if (copyString == pointCopyString)// 주소비교
    int iRet = strcmp( AString, copyString );
    if (iRet==0)// 주소비교
    {
        std::cout << "같다!\n";
    }
    else
    {
        std::cout << "다르다!\n";
    }

    /// <summary>
    /// std::string
    /// </summary>
    /// <returns></returns>
    /// 
    /// 
    /// 
    C_STR AString2 = "KGCA";
    string BString2 = "kgca";
    string copyString2;
    copyString2 = AString2; // error    

    string aData = "ggA";
    aData += "Game";
    aData += "Academy";

    size_t pos = aData.find('A');
    string findstring = aData.substr(0, pos+1);
    std::string::size_type a = aData.find_first_of('g');
    std::string::size_type b = aData.find_first_not_of("g");

    auto c = aData.find_last_not_of("g");
    auto d = aData.find_last_of("g");

    string f = aData.substr(d, c);

    for (std::string::iterator iter=AString2.begin();
         iter != AString2.end();
         iter++)
    {
        std::cout << *iter;
    }
    //AString2.compare(); //==
    //AString2.copy(); //==

    const char* pAdd2= AString2.c_str();
    const char* pAdd3 = AString2.data();
    int ASize2 = AString2.size();
    int ASizestring = strlen(pAdd2);

    if (copyString2 == AString2)// 주소비교    
    {
        std::cout << "같다!\n";
    }
    else
    {
        std::cout << "다르다!\n";
    }
    /// <summary>
    /// 유니코드
    /// </summary>
    /// <returns></returns>
    W_STR AString3 = L"KGCA";
    std::wstring BString3 = L"kgca";// 유니코드 문자열 표현 L""
    std::wstring copyString3;
    copyString3 = AString3; // error    
    int ASize3 = AString3.size();
    const wchar_t* pAddress3 = AString3.c_str();
    int ASizestring3 = _tcslen(pAddress3);
    if (copyString3 == AString3)// 주소비교    
    {
        std::cout << "같다!\n";
    }
    else
    {
        std::cout << "다르다!\n";
    }
    std::cout << "Hello World!\n";
}
