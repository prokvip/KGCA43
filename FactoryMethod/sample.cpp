// AbstractFactory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#include <iostream>
#include <memory>

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
    GameMap() {
        std::cout << "GameMap";
    };
    virtual ~GameMap()
    {
        std::cout << "~GameMap";
    };
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
    virtual ~GameUI() = default;
};
class GameUI2D : public GameUI
{
public:
    virtual void Load() { std::cout << "GameUI2D::Load()!\n"; };
    virtual void Draw() { std::cout << "GameUI2D::Draw()!\n"; };\
};
class GameUI3D : public GameUI
{
public:
    virtual void Load() { std::cout << "GameUI3D::Load()!\n"; };
    virtual void Draw() { std::cout << "GameUI3D::Draw()!\n"; };
};

struct TGame
{    
    std::unique_ptr<GameMap> map;
    std::unique_ptr<GameUI> ui;
};
class GameGenerator 
{
public:
    std::unique_ptr<TGame> CreateGame(int id) {
        auto tGame = std::make_unique<TGame>();
        if (id == D2D)
        {
            tGame->map = std::make_unique<GameMap2D>();
            tGame->ui = std::make_unique<GameUI2D>();
            return tGame;
        }
        if (id == D3D)
        {
            tGame->map = std::make_unique<GameMap3D>();
            tGame->ui = std::make_unique<GameUI3D>();
            return tGame;
        }
        return nullptr;
    };

    std::unique_ptr<GameMap> CreateMap(int id) {
        if (id == D2D) return std::make_unique<GameMap2D>();
        if (id == D3D) return std::make_unique<GameMap3D>();
        return nullptr;
    }; 
    using T_UI = std::unique_ptr<GameUI>;
    static T_UI CreateUI(int id) {
        if (id == D2D) return std::make_unique<GameUI2D>();
        if (id == D3D) return std::make_unique<GameUI3D>();
        return nullptr;
    };
    virtual ~GameGenerator() = default;
};


int main()
{   
    // std::unique_ptr는 복사, 대입 불가하면 이동(전용)만 가능함.
    // std::unique_ptr = std::shared_ptr            // error
    // std::shared_ptr = std::unique_ptr            // error
    // std::unique_ptr = std::move(std::shared_ptr) // error 
    // std::shared_ptr = std::move(std::unique_ptr) // ok 
    std::unique_ptr<GameGenerator> GameGen = std::make_unique<GameGenerator>();
    std::unique_ptr<TGame> game2D =  GameGen->CreateGame(D2D);
    std::unique_ptr<TGame> game3D = GameGen->CreateGame(D3D);

    std::unique_ptr<GameMap> map= GameGen->CreateMap(D2D);
    GameGenerator::T_UI ui      = GameGenerator::CreateUI(D2D);
    //GameGenerator::T_UI ui = GameGen->CreateUI(D2D);
    map->Load();
    ui->Load();
    map->Render();
    ui->Draw();

    std::cout << "Hello World!\n";
}
