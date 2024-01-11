#include <iostream>
// 개방-폐쇄 원칙 : 사전에 구현된 소스 수정없이(폐쇄) 새로운 객체로 재정의(개방)하는 원칙
// 변할수 있는 기능과 변할 수 없는 기능으로 분리해서 설계한다.
class TSoundWave 
{
public:
    // 개방
    virtual void  LoadFile(std::wstring filename)  { std::cout << "사운드파일 Wave로드" << std::endl; } ;
    virtual void  PlayFile()  { std::cout << "사운드파일 Wave플레이" << std::endl; };
};
class TSoundMP3 : public TSoundWave
{
public:
    virtual void  LoadFile(std::wstring filename) override { std::cout << "사운드파일 MP3로드" << std::endl; };
    virtual void  PlayFile()override { std::cout << "사운드파일 MP3" << std::endl; };
};

class TTextureJPG 
{
public:
    virtual void  LoadFile(std::wstring filename) { std::cout << "텍스처파일 JPG로드" << std::endl; };
    virtual void  DrawFile()  { std::cout << "텍스처파일 JPG드로우" << std::endl; };
};
class TTextureBMP : public TTextureJPG
{
public:
    virtual void  LoadFile(std::wstring filename)override { std::cout << "텍스처파일 BMP로드" << std::endl; };
    virtual void  DrawFile() override { std::cout << "텍스처파일 BMP드로우" << std::endl; };
};


// 단일 책임 원칙 적용 (후)
class TCharacterImpl
{
    TSoundWave*     m_Sound = nullptr;
    TTextureJPG*    m_Texture = nullptr;
public:
    void  Move() { };
public:
    void  LoadWaveFile(std::wstring filename) {
        m_Sound = new TSoundWave;
        m_Sound->LoadFile(filename);
    };
    void  LoadMP3File(std::wstring filename) {        
        TSoundMP3* pSound = new TSoundMP3;
        // 파생클래스는 언제나 자신의 기반클래스로 교체 가능해야 한다.
        m_Sound = (TSoundWave*)pSound;
        m_Sound->LoadFile(filename);
    };
    void  PlayWaveFile() { 
        // 리스코프 교체의 원칙이 사용된다.
        TSoundWave* pSound = (TSoundWave*)m_Sound;
        pSound->PlayFile(); 
    };
    void  PlayMP3File() { m_Sound->PlayFile(); };

    void  LoadJPGFile(std::wstring filename) { m_Texture->LoadFile(filename); };
    void  LoadBMPFile(std::wstring filename) { m_Texture->LoadFile(filename); };
    void  DrawJPGFile() { m_Texture->DrawFile(); };
    void  DrawBMPFile() { m_Texture->DrawFile(); };

};
int main()
{
    TCharacterImpl chImpl;
    chImpl.LoadWaveFile(L"Demo.wav");
    chImpl.LoadMP3File(L"Demo.mp3");
    chImpl.LoadJPGFile(L"Demo.jpg");
    chImpl.LoadBMPFile(L"Demo.bmp");
    chImpl.PlayWaveFile();
    chImpl.PlayMP3File();
    chImpl.DrawJPGFile();
    chImpl.DrawBMPFile();
    std::cout << "Hello World!\n";
}
