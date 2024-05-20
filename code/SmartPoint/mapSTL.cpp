#include <iostream>
#include <string>
#include <map>
#include <memory>

struct TData
{
    int iValue;
    TData() = default;
    TData(int i) :iValue(i)
    {
    }
};

class TManager
{
private:
    // TManager 내부에서만 사용하고 외부에는 접근불가 할 경우
    //TData* m_pDataUnique=nullptr;
    std::unique_ptr<TData> m_pDataUnique;
    // 내부 및 외부에서 공유 사용할 때.
    std::shared_ptr<TData> m_pDataShared;
public:
    void  Show()
    {
        if (m_pDataUnique != nullptr)
        {
            std::cout << m_pDataUnique->iValue;
        }
        if (m_pDataShared != nullptr)
        {
            std::cout << m_pDataShared->iValue;
        }
    }
    std::shared_ptr<TData> GetPtr() { 
        return m_pDataShared; 
    }
    std::unique_ptr<TData> GetPtrMove()
    {
        return std::move(m_pDataUnique);
    }
    // row pointer
    //const TData* const GetPtrRow() { return m_pDataShared.get(); }
    //TData* v() { return m_pDataUnique.get(); }
    
    TManager()
    {
        m_pDataUnique = std::make_unique<TData>(44);
        m_pDataShared = std::make_shared<TData>(33);
    }
}; 

int main()
{ 
    {
        // weak_ptr 유일한 방법은 lock()  쉐어드포인터를 가져와 활용하는 것이다.
        std::weak_ptr<TData> pWeakData;
        {
            TManager mgr;
            mgr.Show();
            std::shared_ptr<TData> pSharedData = mgr.GetPtr();
            int i = pSharedData->iValue;
            pWeakData = mgr.GetPtr();
            if (!pWeakData.expired())
            {
                try
                {
                    auto sharedPt = pWeakData.lock();
                    i = sharedPt->iValue;
                }        
                catch (std::bad_weak_ptr b)
                {
                    std::cout << "null";
                }
            }
        }
        {            
            if (!pWeakData.expired())
            {
                auto sharedPt = pWeakData.lock();
                if (sharedPt != nullptr)
                {
                    int i = sharedPt->iValue;
                }
            }
        }
    }

    {
        TManager mgr;
        mgr.Show();
        // 소유권 이전만 가능하다.
        auto data = mgr.GetPtrMove();
        std::cout << data->iValue;
        mgr.Show();
        //TData* pData = mgr.GetPtr();
        //delete pData;// 절대 사용하면 안된다.
        //delete data.get();
    }
    {
        TManager mgr;
        mgr.Show();
        //const TData* const pData = mgr.GetPtrRow();
        //error C3490 : 'iValue'은(는) const 개체를 통해 액세스되고 있으므로 수정할 수 없습니다.
        // error C3892 : 'pData' : const인 변수에 할당할 수 없습니다.
        //pData->iValue = 9; // error
        //pData = nullptr;   // error
        //delete pData;      // ERROR
        //std::cout << pData->iValue;
        // 소유권 이전만 가능하다.
        {            
            auto data1 = mgr.GetPtr();           
            std::cout << data1->iValue;

            {
                auto data2 = mgr.GetPtr();
                std::cout << data2->iValue;
            }
            auto data3 = mgr.GetPtr();
            std::cout << data3->iValue;
        }
        mgr.Show();
        //TData* pData = mgr.GetPtr();
        //delete pData;// 절대 사용하면 안된다.
        //delete data.get();
    }
    int iKey = 0;
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
