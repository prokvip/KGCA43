#include "TDataManager.h"

enum WORK {
    SCREEN_PRINT = 0,
    NEW_DATA,
    FILE_SAVE,
    FILE_LOAD,
    DATA_UPDATE,
    DEL_DATA,
    SORT_DATA,
    CREATE_SAMPLE_DATA = 9,
    EXIT = 99
};


void TitlePrint()
{
    printf("\n<<--------(점수)관리프로그램-------->>\n");
}
void MenuPrint()
{
    printf("\n출력(0),신규(1),저장(2),로드(3),수정(4),삭제(5),정렬(6),샘플용(9),종료(99)");
}
int main()
{   
    //TLinkedlist<TItem> itemlist;
    //TLinkedlist<TCharacter> charlist;
   // TDataManager<TItem> dataManager;

    TDataManager tData;
    int iSelect = 0;
    tData.Load();
    TitlePrint();

    while (1)
    {
        MenuPrint();
        scanf("%d", &iSelect);
        if (iSelect == EXIT) break;
        system("cls");
        // 작업리스트
        switch (iSelect)
        {
        case SCREEN_PRINT:
        {
            TitlePrint();
            tData.print(true);
        }break;
        case NEW_DATA:
        {
            tData.NewData();            
        }break;

        case FILE_SAVE:
        {
            tData.Save();
        }break;
        case FILE_LOAD:
        {
            tData.AllDeleteData();
            tData.Load();
        }break;
        case DATA_UPDATE:
        {
        }break;
        case DEL_DATA:
        {
            printf("값을 입력하시오: ");
            int data;
            scanf_s("%d", &data);
            tData.DelData(data);
        }break;
        case SORT_DATA:
        {
            tData.sort();
        }break;
        case CREATE_SAMPLE_DATA:
        {
            tData.Sample();
        }break;
        }
    }

    tData.AllDeleteData();
    std::cout << "Hello World!\n";
}
