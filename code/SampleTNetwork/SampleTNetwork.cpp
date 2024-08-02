#include <iostream>
#include "TNetServer.h"
#include "TNetIOCPServer.h"
#ifdef _DEBUG
#pragma comment(lib, "TNetwork_d.lib")
#else
#pragma comment(lib, "TNetwork.lib")
#endif
// TNetwork 폴더 -> inc, lib 폴더를 먼저 생성 후 작업한다.
 
// 라이브러리 프로젝트 속성->빌드이벤트->빌드 후 이벤트
// copy "소스대상" "목적지 복사"
// copy "*.h" "..\\..\\TNetwork\\Inc\\"
// copy "..\\..\\output\\TNetworkLib.lib" "..\\..\\TNetwork\\lib\\"

// malloc.h
//#define _mm_free(a)      _aligned_free(a)
//#define _mm_malloc(a, b) _aligned_malloc(a, b)

int main()
{ 
    /// 정적 메모리 배치 및 정렬
    // 기준주소가 align(8) 배치,  배열요소는 데이형(4byte) 만큼 정렬.
    __declspec(align(16)) short arr1[4];
    printf("\t%d\t%d\t", sizeof(arr1),   __alignof(arr1));

    for (int i = 0; i< 4; i++)
    {
        printf("0x%p\t", &arr1[i]);
    }
    printf("\n");
    // 기준주소가 align(8) 배치,  배열요소는 align(8) 만큼 정렬.
    typedef __declspec(align(128)) struct { short a;} S_align;
    S_align arr2[4];
    printf("\t%d\t%d\t", sizeof(arr2), __alignof(arr2));
    for (int i = 0; i < 4; i++)
    {
        printf("0x%p\t", &arr2[i]);
    }


    printf("\n");
    
    
    /// 동적 메모리 배치 및 정렬
    //_aligned_offset_malloc
    //_aligned_offset_malloc
    //_aligned_offset_realloc
    // 기준 주소만 정렬
    int* arr3 = (int*)_mm_malloc(sizeof(int) * 4, 8);
    printf("\t%d\t%d\t", sizeof(arr3), __alignof(arr3));
    for (int i = 0; i < 4; i++)
    {
        printf("0x%p\t", &arr3[i]);
    }
    printf("\n");
    // 기준주소가 align(8) 배치,  배열요소는 align(8) 만큼 정렬.
    typedef __declspec(align(8)) struct { int a; } M_align;
    M_align* arr4 = (M_align*)_mm_malloc(sizeof(M_align) * 4, 8);
    printf("\t%d\t%d\t", sizeof(M_align), __alignof(arr4));
    for (int i = 0; i < 4; i++)
    {
        printf("0x%p\t", &arr4[i]);
    }
    // 16의미는 주소값 % aligment(16) == 0 
    int remainder = (int)&arr4 % 8;
    if (remainder == 0)
    {
        printf("메모리 정렬");
    }
    else
    {
        printf("메모리 비 정렬");
    }

    
    _mm_free(arr3);
    _mm_free(arr4);



    const int volatile k = 0;
    if (0)
    {
        std::cout << "true";
    }

    bool bSelectModel = false;
    std::cout << "Select Server starting!" << std::endl;
    if (bSelectModel)
    {        
        TNetServer net;
        net.CreateServer("192.168.0.12", 10000);
        net.m_pSelectIO = std::make_shared<TStdSelect>(&net);        
        net.Run();
        net.Release();
    }
    else
    {
        TNetIOCPServer net;
        net.CreateServer("192.168.0.12", 10000);        
        net.Run();
        net.Release();
    }

}
