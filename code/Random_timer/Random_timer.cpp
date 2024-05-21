#include <iostream>
#include <functional> //  c+++11 함수포인터 기능
#include <random>
#include <chrono>  //  시간 측정 기능
#include <vector>
#include <algorithm>

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

int  Random(int iMax)
{
    return rand() % iMax ; // 65535
}

int main()
{
    //  5초 경과후에 반환된다.
    std::clock_t start = std::clock();
    double time = 0.0;
    while (time < 5.0 )
    {
        time = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        std::cout << time << std::endl;
    }
    std::clock_t end = std::clock();
    time = (end - start) / (double)CLOCKS_PER_SEC;
    std::cout << time << std::endl;
    
    
    //  특정 함수 및 기능의 시간 측정 방법
    using namespace  std::chrono;
    system_clock::time_point S = system_clock::now();
        for (int i = 0; i < 10000000; i++)
        {
            std::sqrt(i);
        }

    //system_clock은 컴퓨터 시간과 동기화 작업수행. 
    //steady_clock::time_point E = steady_clock::now();
    system_clock::time_point E = system_clock::now();
    duration<double> secDuration = E - S;
    std::cout << secDuration.count() << std::endl;

    std::chrono::seconds sec =
        std::chrono::duration_cast<std::chrono::seconds>(E - S);
    std::chrono::minutes min =
        std::chrono::duration_cast<std::chrono::minutes>(E - S);
    std::chrono::hours hour =
        std::chrono::duration_cast<std::chrono::hours>(E - S);


    std::chrono::milliseconds mill =
        std::chrono::duration_cast<std::chrono::milliseconds>(E - S);
    std::chrono::microseconds micro =
        std::chrono::duration_cast<std::chrono::microseconds>(E - S);
    std::chrono::nanoseconds nano =
        std::chrono::duration_cast<std::chrono::nanoseconds>(E - S);


    std::cout << "----------------" << std::endl;
    std::cout << sec.count() << std::endl;
    std::cout << min.count() << std::endl;
    std::cout << hour.count() << std::endl;
    std::cout << mill.count() << std::endl;
    std::cout << micro.count() << std::endl;
    std::cout << nano.count() << std::endl;

    /// <summary>
    /// 응용
    /// </summary>
    /// <returns></returns>
    using DefaultVector = std::vector<int>;
    DefaultVector v(10);
        std::random_device rd1;
        std::mt19937 engine1(rd1()); // 난수엔진
        std::uniform_int_distribution<int> dis1(0, 100); // 분포함수
        auto fun = std::bind(dis1, engine1);
    std::generate(v.begin(), v.end(), fun);

    DefaultVector result;
    std::transform(v.begin(), v.end(), 
        std::back_inserter(result), [](int i) -> int {
            return i % 2;
        }
    );


    float a = randf(100);     // 0 ~ 100 
    float b = randf2(50, 50); // 50 + (0 ~ 50)
    float c = randstep(-1.0f, +1.0f);
    int d = 50;
    clamp(a, 40, 60);
    std::cout << a << " " << b << " " << c << " " << d <<" ";

    srand(std::time(NULL));
    for (int i = 0; i < 10; i++)
    {
        std::cout << Random(100) << " ";
    }
    std::cout << std::endl;

    /// <summary>
    /// 모던C++ 난수생성기
//  지정된 범위에 균등하게 분포(URNG:균등난수생성기)
//  모던C++ 난수생성기를 사용해야 하는 이유
//   1) 균등하게 분포된다. rand() 함수에 비해서
//   2) 고품질의 난수 생성기 + 분포 클래스를 사용해서
//   3) 난수의 데이터형&범위&분포 형태 조절 가능하다.
    /// </summary>
    /// <returns></returns>
    std::random_device rd;
    std::mt19937 engine(rd()); // 난수엔진
    //std::mt19937 engine(time(NULL)); // 난수엔진
    std::uniform_int_distribution<int> dis(0, 100); // 분포함수
    auto generator = std::bind(dis, engine);
    for (int i = 0; i < 10; i++)
    {      
        int iRand = generator();
        std::cout << iRand << " ";
    }
    std::cout << std::endl;

    // typedef mt19937 default_random_engine -> 메르센 트위스트
    // 엔진 종류 = default_random_engine 기본엔진,쿠누스엔진, 최소 표준엔진, 메르센트위스터엔진, Ranlux엔진, 
    // 분포 종류 = 베르누이분포, 이항 분포, 기하분포, 음이항분포 ...
    /// 1)엔진형식 정의
    std::random_device rd3; 
    // =std::default_random_engine aa;
    //std::knuth_b k_b; // 쿠누스엔진
    /// 2)엔진 선택
    std::mt19937 engine2(rd3()); // 난수엔진    
    /// 분포 선택
    std::uniform_int_distribution<int> dis2(0, 100); // 분포함수
    std::uniform_real_distribution<float> dis3(0.0f, 100.0f);
    ///*std::bernoulli_distribution  dis3;
    //std::binomial_distribution<int> dis4;
    //std::negative_binomial_distribution<int> dis5;*/

    auto generator2 = std::bind(dis2, engine2);
    for (int i = 0; i < 10; i++)
    {
        float iRand = generator2();
        std::cout << iRand << " ";
    }
    std::cout << "Hello World!\n";
}
