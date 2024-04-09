// PointProject1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
int rambo(); /* 함수머리(prototype) 선언 */
int (*p)(); /* 함수 포인터 변수 선언 */

void main()
{
    rambo();
    p = rambo;
    p();
}

int rambo()
{
    puts("good");
    return 0;
}

