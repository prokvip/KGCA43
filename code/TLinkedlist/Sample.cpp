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
    printf("\n<<--------(����)�������α׷�-------->>\n");
}
void MenuPrint()
{
    printf("\n���(0),�ű�(1),����(2),�ε�(3),����(4),����(5),����(6),���ÿ�(9),����(99)");
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
        // �۾�����Ʈ
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
            printf("���� �Է��Ͻÿ�: ");
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
