#include <iostream>
// 리스코프 치환  원칙
class TSound // 인터페이스 
{
public:
    // 다형성을 사용하는 것은 "개방-폐쇄 원칙"을 사용하는 것인다.)
    virtual void  LoadFile(std::wstring filename) { std::cout << "기본 사운드 로드" << std::endl; };
    virtual void  PlayFile() { std::cout << "기본 사운드파일 플레이" << std::endl; };
};
class TSoundWave : public TSound
{
public:
    // 개방
    virtual void  LoadFile(std::wstring filename) override  { std::cout << "사운드파일 Wave로드" << std::endl; } ;
    virtual void  PlayFile() override  { std::cout << "사운드파일 Wave플레이" << std::endl; };
};
class TSoundMP3 : public TSound
{
public:
    virtual void  LoadFile(std::wstring filename) override { std::cout << "사운드파일 MP3로드" << std::endl; };
    virtual void  PlayFile()override { std::cout << "사운드파일 MP3" << std::endl; };
};

class TTexture
{
public:
    virtual void  LoadFile(std::wstring filename) { std::cout << "텍스처파일 JPG로드" << std::endl; };
    virtual void  DrawFile() { std::cout << "텍스처파일 JPG드로우" << std::endl; };
};
class TTextureJPG : public TTexture
{
public:
    virtual void  LoadFile(std::wstring filename) override{ std::cout << "텍스처파일 JPG로드" << std::endl; };
    virtual void  DrawFile() override { std::cout << "텍스처파일 JPG드로우" << std::endl; };
};
class TTextureBMP : public TTexture
{
public:
    virtual void  LoadFile(std::wstring filename)override { std::cout << "텍스처파일 BMP로드" << std::endl; };
    virtual void  DrawFile() override { std::cout << "텍스처파일 BMP드로우" << std::endl; };
};


// 단일 책임 원칙 적용 (후)
class TCharacterImpl
{
    TSound*     m_SoundWave = nullptr;
    TSound*     m_SoundMP3 = nullptr;
    TTexture*   m_TextureJPG = nullptr;
    TTexture*   m_TextureBMP = nullptr;
public:
    void  Move() { };
public:
    void  LoadWaveFile(std::wstring filename) {
        m_SoundWave = new TSoundWave;
        m_SoundWave->LoadFile(filename);
    };
    void  LoadMP3File(std::wstring filename) {
        m_SoundMP3 = new TSoundMP3;
        m_SoundMP3->LoadFile(filename);
    };
    void  PlayWaveFile() { m_SoundWave->PlayFile(); };
    void  PlayMP3File() { m_SoundMP3->PlayFile(); };

    void  LoadJPGFile(std::wstring filename) { m_TextureJPG->LoadFile(filename); };
    void  LoadBMPFile(std::wstring filename) { m_TextureBMP->LoadFile(filename); };
    void  DrawJPGFile() { m_TextureJPG->DrawFile(); };
    void  DrawBMPFile() { m_TextureBMP->DrawFile(); };

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
