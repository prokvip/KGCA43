// CLang_0.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

int   Fun1() {
    int a = 10;
    a = 17 - a;
    return a;
}
int Fun2(){
    int a, b, c;
    a = 5;
    b = 3;
    c = 7;
    int iSum = a + b * c;
    return iSum;
}
int Fun3(){
    int a = 5;
    int b = 2;
    int c = a % b;
    if (c == 1)     {
        return 1;
    }
    else    {
        return 0;
    }
}
void Fun4(){
    int ret1 = Fun1();
    int ret2 = Fun2();
    int ret3 = Fun3();
    //char buffer1[3] = { 'a', 'b','c', '\n' }; // error
    char buffer2[4] = { 'a', 'b','c', '\n' };
    char buffer3[] = { 'a', 'b','c', '\n' };
    char buffer4[3] = { 'a' };
    char buffer5[3] = "a";
    char buffer6[] = "";
}
int  Fun5()
{
    int number[3] = { 1,2,3 };
    return number[1];
}
void  Fun6()
{
    // int(정수형 변수, 4바이트)
    int a = 1; // 메모리에 4바이트를 할당할수 있는 
    //임의의 주소를 사용한다.  
    std::cout << a << std::endl;
    a = 2;
    std::cout << a << std::endl;
    a = 3;
    std::cout << a << std::endl;    
    int b = 4;
    int c = 5;
    int* d = nullptr;
    d = &a;     std::cout << *d << std::endl;
    d = &b;     std::cout << *d << std::endl;
    d = &c;     std::cout << *d << std::endl;
    std::cout << &d << std::endl;
    std::cout << d << std::endl;
    std::cout << *d << std::endl;
    
    

    int num[5] ;
    num[0] = 1; num[1] = 3; num[2] = 5; num[3] = 7; num[4] = 9;
    //int num[] = { 1, 3, 5, 7, 9 };
    // 포인터 변수 선언(동일한 데이터형의 변수의 주소를 저장한다.)
    int* ptr0 = &num[0];
    int* ptr1 = &num[1];
    int* ptr2 = &num[2];
    int* ptr3 = &num[3];
    int* ptr4 = &num[4];
    int* ptr5 = num+0;// &num[0]
    int* ptr6 = num+1;// &num[1]
    int* ptr7 = num+2;// &num[2]
    int* ptr8 = num+3;// &num[3]
    int* ptr9 = num+4;// &num[4]
    //int ptr1 = *(num + 0);// num[0]
    //int ptr3 = *(num + 1);// num[1]
    //int ptr5 = *(num + 2);// num[2]
    //int ptr7 = *(num + 3);// num[3]
    //int ptr9 = *(num + 4);// num[4]

    int numA;
    int numB[4];
    int* ptrAddressA = &numA;
    int* ptrAddressB = numB;

    int* ptrAddressC = num;
    int*  a1Address = ptrAddressC;
    int   a1Value = *ptrAddressC;

    //ptrAddressC = ptrAddressC + 0;
    //int a1 = *ptrAddressC;
    //ptrAddressC = ptrAddressC + 1;
    //int a2 = *ptrAddressC;
    //ptrAddressC = ptrAddressC + 1;
    //int a3 = *ptrAddressC;
    //ptrAddressC = ptrAddressC + 1;
    //int a4 = *ptrAddressC;

    ptrAddressC++;//ptrAddressC[1]
    int a1 = *ptrAddressC;
    ptrAddressC = ptrAddressC+=2;//ptrAddressC[1+2]
    int a2 = *ptrAddressC;
    ptrAddressC = ptrAddressC--;//ptrAddressC[1+2-1]
    int a3 = *ptrAddressC;
    ptrAddressC = ptrAddressC--;//ptrAddressC[1+2-1-1]
    int a4 = *ptrAddressC;
    ptrAddressC = ptrAddressC+2;//ptrAddressC[1+2-1-1+2]
    int a5 = *ptrAddressC;
    ptrAddressC = ptrAddressC - 2;//ptrAddressC[1+2-1-1+2-2]
    int a6 = *ptrAddressC;

    std::cout << "Hello World3!\n";
   /* int* ptr = num;
    int* ptr2 = nullptr;
    ptr2 = ptr + 2;// ptr[0+2]
    int  a = *ptr2;
    int b = (*ptr2) + 1;
    std::cout << "* ptr=" << a << std::endl;
    std::cout << "* ptr+1=" << b << std::endl;*/
}

int main() {  
    printf("HELLOWORLD"); // 줄바꿈(개행) 없다.
    printf("HELLO WORLD");
    printf("HELLO WORLD\a");// 경고음.
    printf("HELLO WORLD\n");// 줄바꿈 있다.
    printf("HELLO WORLD");

    /*int inputData;
    scanf("%d", &inputData);
    char inputChar;
    scanf("%c", &inputChar);
    char inputCharArray[3];
    scanf("%s", inputCharArray);*/

    //int a = 1;
    //if(a < 1) {} // false
    //if (a > 1) {} // false
    //if (a > -1 && a < 1) {} // false   and연산
    //if (a > -1 || a < 1) {} // true   or연산
    // and  0  1  논리곱(&&)
    //   0  0  0
    //   1  0  1
    // or   0  1  논리합(||)
    //   0  0  1
    //   1  1  1
    //Fun4();    
    // 
    //int a= 2;
    //int b=3; 
    //int c = 1;
    //a *= (b++) * (++c);
    //std::cout << a;
    //std::cout << b;
    //std::cout << c;

    int i;
    int k[10];
    int a = 0;
    int b = 0;
    for (i = 0; i < 10; i++)
    {
        k[i] = 10 - i;
    }
    int* p = k;
    p += 5;
    a = *(p + 1);
    b = p[3];// p+ 3

    //std::cout << Fun5() << std::endl;
    Fun6();
    std::cout << "Hello World3!\n";
}
