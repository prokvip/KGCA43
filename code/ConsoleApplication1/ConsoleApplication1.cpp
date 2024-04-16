#include <stdio.h>
// 1, 재귀호출
int Factorial(int n);
int g_iCounter = 0;
void HanioTower(int n, char from, char temp, char to)
{
    if (n == 1)
    {
        printf("[%d]번 %c -> %c\n", n, from, to);
        g_iCounter++;
    }
    else
    {
        //a를 c을 임시저장소로 활용하여 b로 놓는다.
        HanioTower(n - 1, from, to, temp);
        printf("[%d]번 %c -> %c\n", n, from, to);
        g_iCounter++;
        //b를 a을 임시저장소로 활용하여 c로 놓는다.
        HanioTower(n - 1, temp, from, to);
    }
}
int main()
{
    // 3! = 3*2*1;
    // 5! = 5*4*3*2*1;
    int iSum = 1;
    int n = 3;

    printf("팩토리얼 결과값 = %d\n", Factorial(n));
    
    // 1      
    // 2      
    // 3    2     1
    // 4    1     2
    // A    B     C
    //from temp to
    HanioTower(3, 'A','B','C');

    for (int i = n;
        i > 0;
        i--)
    {
        iSum = iSum * i;
    }

    /*for ( int i = 0; 
          i < n; 
          i++)
    {
        iSum = iSum * (i+1);
    }*/
    /*for ( int i = 1;
          i <= n;
          i++)
    {
        iSum = iSum * i;
    }*/
    printf("팩토리얼 결과값 = %d", iSum);
}
//재귀함수 버전
int Factorial(int n)
{
    if (n == 1)
    {
        return 1;
    }
    else
    {
        int iFact = n - 1;
        int iSum = Factorial(iFact);
        return n * iSum;
    }    
}