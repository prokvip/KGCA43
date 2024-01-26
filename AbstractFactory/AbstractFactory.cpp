// AbstractFactory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#include <iostream>
enum { DX, OPENGL, };
enum { WIN_OS, UNIX_OS, };
enum { PC, NOTEBOOK, };
enum { KO, JP, EN, };
enum { KEYBOARED, MOUSE, PED, };
enum { CON_PLAT, APP_PLAT, };
enum { D2D, D3D, };

class GameMap
{
public:
    virtual void Load() = 0;
    virtual void Render() = 0;
};
class GameMap2D : public GameMap
{
public:
    virtual void Load() { std::cout << "GameMap2D::Load()!\n"; };
    virtual void Render() { std::cout << "GameMap2D::Render()!\n"; };
};
class GameMap3D : public GameMap
{
public:
    virtual void Load()   { std::cout << "GameMap3D::Load()!\n"; };
    virtual void Render() { std::cout << "GameMap3D::Render()!\n"; };
};
class GameUI
{
public:
    virtual void Load() = 0;
    virtual void Draw() = 0;
};
class GameUI2D : public GameUI
{
public:
    virtual void Load() { std::cout << "GameUI2D::Load()!\n"; };
    virtual void Draw() { std::cout << "GameUI2D::Draw()!\n"; };
};
class GameUI3D : public GameUI
{
public:
    virtual void Load() { std::cout << "GameUI3D::Load()!\n"; };
    virtual void Draw() { std::cout << "GameUI3D::Draw()!\n"; };
};

class GameGenerator {
public:
    virtual GameMap* CreateMap()=0;
    virtual GameUI* CreateUI()=0;
};
class Game2D : public GameGenerator
{
public:
    virtual GameMap* CreateMap() {
        return  new GameMap2D;
    }
    virtual GameUI* CreateUI() {
        return  new GameUI2D;
    }
};
class Game3D : public GameGenerator
{
public:
    virtual GameMap* CreateMap() {
        return  new GameMap3D;
    }
    virtual GameUI* CreateUI() {
        return  new GameUI3D;
    }
};


int main()
{
    int iGame = D3D;
    GameGenerator* Game = nullptr;
    if(iGame == D2D)
    {
        Game = new Game2D;        
    }
    if (iGame == D3D)
    {
        Game = new Game3D;        
    }   
    
    GameMap* map = Game->CreateMap();
    GameUI* ui   = Game->CreateUI();
    map->Load();
    ui->Load();
    map->Render();
    ui->Draw();

    std::cout << "Hello World!\n";
}
