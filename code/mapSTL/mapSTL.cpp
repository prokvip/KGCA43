#include <iostream>
#include <string>
#include <map>
#include <memory>

int main()
{
   
    std::map<std::string, int>  map1;
    // 오늘 이후로 사용 금지한다.
    map1["홍오군"] = 99;    
    map1["홍상무"] = 88;
    map1["홍길동"] = 88;

    std::pair< std::string, int> data("영순이",100);
    map1.insert(data);
    map1.insert(std::make_pair("김철수", 19));
    map1.insert(std::make_pair("김철수", 22));

    for (auto ele = map1.begin();
        ele != map1.end();
        ele++)
    {
        std::pair< std::string, int> data = *ele;
        std::cout << "key="  << data.first 
                  << "value="<< data.second << std::endl;
    }

    auto iter = map1.find("영순이");
    if (iter != map1.end())
    {

        std::pair< std::string, int> d = *iter;
        std::cout   << "key=" << d.first
                    << "value=" << (*iter).second << std::endl;
    }
    struct TData
    {
        int iValue;
        TData() = default;
        TData(int i):iValue(i) 
        {            
        }
    };
    int iKey = 0;
    std::map<int, TData*>  map2;
    // 오늘 이후로 사용 금지한다.
    map2[iKey++] = new TData(22);
    map2[iKey++] = new TData(33);
    map2[iKey++] = new TData(44);

    std::pair< int, TData*> data2(iKey++, new TData(100));
    map2.insert(data2);
    map2.insert(std::make_pair(iKey++, new TData(19)));
    map2.insert(std::make_pair(iKey++, new TData(22)));

    for (auto ele = map2.begin();
        ele != map2.end();
        ele++)
    {
        std::pair<int, TData*> data = *ele;
        TData* pData = data.second;
        std::cout << "key=" << data.first
            << "value=" << pData->iValue << std::endl;
    }
    for (int i= 0; i < map2.size(); i++)
    {
        TData* pData = map2[i];
        std::cout << "key=" << i
               << "value=" << pData->iValue << std::endl;
    }
    auto iter2 = map2.find(5);
    if (iter2 != map2.end())
    {

        std::pair< int, TData*> d = *iter2;
        std::cout << "key=" << d.first
            << "value=" << (*iter2).second->iValue << std::endl;
    }

    for (auto& r : map2)
    {
        delete r.second;
    }
    map2.clear();

    using TDATA = std::unique_ptr<TData>;
    {
        TDATA a(new TData(7));
     /*   std::unique_ptr<TData> d = a; // error
        std::unique_ptr<TData> e;  // error
        e = a;*/
        TDATA c = std::move(a);
        TData* pData = c.get(); // 될수 있으면 쓰지 말라.
        std::cout << c->iValue;// == a.get()->iValue
        std::cout << pData->iValue;
        //delete pData; // error

        std::unique_ptr<TData> b = 
                std::make_unique<TData>(44);
        std::cout << b->iValue;
    }
    
    std::cout << "Hello World!\n";


    using TDATA = std::unique_ptr<TData > ;
    using TDATA_MAP = std::map<int, TDATA>;
    int g_iKey = 0;
    std::map<int, TDATA> map3;
    map3.insert(std::make_pair(iKey++, new TData(22)));
    map3.insert(std::make_pair(iKey++,
                    std::make_unique<TData>(33)));
    map3.insert(std::make_pair(iKey++,
                    std::make_unique<TData>(44)));
    map3.clear();

    std::cout << "Hello World!\n";
}
