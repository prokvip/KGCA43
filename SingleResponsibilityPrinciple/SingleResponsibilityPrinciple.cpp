// 단일 책임 원칙을 적용하는 샘플이다.
// -다형성이 없는 클래스들에 해당한다.
#include <iostream>

class TSoundWave
{
public:
    void  LoadFile(std::wstring filename) { std::cout << "사운드파일 Wave로드" << std::endl; };
    void  PlayFile() { std::cout << "사운드파일 Wave플레이" << std::endl; };    
};
class TTextureJPG
{
public:
    void  LoadFile(std::wstring filename) { std::cout << "텍스처파일 JPG로드" << std::endl; };
    void  DrawFile() { std::cout << "텍스처파일 JPG드로우" << std::endl; };
};
// 단일 책임 원칙 적용 (전)
class TCharacter
{
    void  Move() { };
public:    
    void  LoadWaveFile(std::wstring filename) { std::cout << "사운드파일 Wave로드" << std::endl; };
    void  PlayWaveFile() { std::cout << "사운드파일 플레이" << std::endl; };
  
    void  LoadJPGFile(std::wstring filename) { std::cout << "텍스처파일 JPG로드" << std::endl; };
    void  DrawJPGFile() { std::cout << "텍스처파일 JPG드루우" << std::endl; }; 
};

// 단일 책임 원칙 적용 (후)
class TCharacterImpl
{    
    TSoundWave    m_SoundWave;
    TTextureJPG   m_TextureJPG;
public:
    void  Move() { };
public:
    // 단일 책임 원칙 적용 (전)
    void  LoadWaveFile(std::wstring filename) { m_SoundWave.LoadFile(filename); };
    void  PlayWaveFile() { m_SoundWave.PlayFile(); };

    void  LoadJPGFile(std::wstring filename) { m_TextureJPG.LoadFile(filename); };
    void  DrawJPGFile() { m_TextureJPG.DrawFile(); };
};
int main()
{
    TCharacter ch;
    ch.LoadWaveFile(L"Demo.wav");
    ch.LoadJPGFile(L"Demo.jpg");
    ch.PlayWaveFile();
    ch.DrawJPGFile();

    TCharacterImpl chImpl;
    ch.LoadWaveFile(L"Demo.wav");
    ch.LoadJPGFile(L"Demo.jpg");
    ch.PlayWaveFile();
    ch.DrawJPGFile();

    std::cout << "Hello World!\n";
}
