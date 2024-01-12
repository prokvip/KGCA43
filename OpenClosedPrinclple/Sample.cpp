#include <iostream>
//enum SoundFileID { WAVE_FILE, MP3_FILE, };
//enum TextureFileID { JPG_FILE, BMP_FILE, };
// 개방-폐쇄 원칙 : 사전에 구현된 소스 수정없이(폐쇄) 새로운 객체로 재정의(개방)하는 원칙
// 변할수 있는 기능과 변할 수 없는 기능으로 분리해서 설계한다.
class TSound
{
public:
    std::wstring m_csFileName;
    virtual void  LoadFile(std::wstring filename) = 0;
    virtual void  PlayFile() = 0;
};
class TSoundWave : public TSound
{
public:
    // 개방
    virtual void  LoadFile(std::wstring filename)  { std::cout << "사운드파일 Wave로드" << std::endl; } ;
    virtual void  PlayFile()  { std::cout << "사운드파일 Wave플레이" << std::endl; };
    TSoundWave() = default;    
    TSoundWave(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};
class TSoundMP3 : public TSound
{
public:
    virtual void  LoadFile(std::wstring filename) override { std::cout << "사운드파일 MP3로드" << std::endl; };
    virtual void  PlayFile()override { std::cout << "사운드파일 MP3" << std::endl; };
    TSoundMP3() = default;
    TSoundMP3(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};
class TSoundMID : public TSound
{
public:
    virtual void  LoadFile(std::wstring filename) override { std::cout << "사운드파일 mid로드" << std::endl; };
    virtual void  PlayFile()override { std::cout << "사운드파일 MID" << std::endl; };
    TSoundMID() = default;
    TSoundMID(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};
class TTexture
{
public:
    std::wstring m_csFileName;
    virtual void  LoadFile(std::wstring filename) = 0;
    virtual void  DrawFile() = 0;
};
class TTextureJPG : public TTexture
{
public:
    virtual void  LoadFile(std::wstring filename) { std::cout << "텍스처파일 JPG로드" << std::endl; };
    virtual void  DrawFile()  { std::cout << "텍스처파일 JPG드로우" << std::endl; };
    TTextureJPG() = default;
    TTextureJPG(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};
class TTextureBMP : public TTexture
{
public:
    virtual void  LoadFile(std::wstring filename)override { std::cout << "텍스처파일 BMP로드" << std::endl; };
    virtual void  DrawFile() override { std::cout << "텍스처파일 BMP드로우" << std::endl; };
    TTextureBMP() = default;
    TTextureBMP(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};
class TTexturePNG : public TTexture
{
public:
    virtual void  LoadFile(std::wstring filename)override { std::cout << "텍스처파일 PNG로드" << std::endl; };
    virtual void  DrawFile() override { std::cout << "텍스처파일 PNG드로우" << std::endl; };
    TTexturePNG() = default;
    TTexturePNG(std::wstring filename)
    {
        m_csFileName = filename;
        LoadFile(filename);
    }
};

// 단일 책임 원칙 적용 (전)
class TCharacterImpl
{
    TSoundWave*     m_Sound = nullptr;
    TTextureJPG*    m_Texture = nullptr;
public:
    void  Move() { };
public:
    void  LoadWaveFile(std::wstring filename ) {
        m_Sound = new TSoundWave;
        m_Sound->LoadFile(filename);
    };
    // MP3File 기능 추가  될때마다 함수를 추가해야 하는 문제가 발생한다.
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
    // MP3File 기능 추가  될때마다 함수를 추가해야 하는 문제가 발생한다.
    void  PlayMP3File() { m_Sound->PlayFile(); };

    void  LoadJPGFile(std::wstring filename) { m_Texture->LoadFile(filename); };
    void  DrawJPGFile() { m_Texture->DrawFile(); };

    // BMPFile 기능 추가
    void  LoadBMPFile(std::wstring filename) { m_Texture->LoadFile(filename); };
   
    // BMPFile 기능 추가
    void  DrawBMPFile() { m_Texture->DrawFile(); };

};

// 단일 책임 원칙 적용 (후)
// TCharacterImplOP는 신규 기능이 추가되더라도 변경할 필요가 없다.(폐쇄)
class TCharacterImplOP
{
public:
   std::shared_ptr<TSound>     m_pSound = nullptr;
   std::shared_ptr<TTexture>   m_pTexture = nullptr;
   //TSound*     m_pSound = nullptr;
   //TTexture*   m_pTexture = nullptr;
public:
    void  Move() { };
public:  
    /// <summary>
    /// 전략(Strategy)패턴
    /// </summary>
    /// <param name="pSound"></param>
    void  PlaySoundFile() {
        if (m_pSound == nullptr) return;
        m_pSound->PlayFile();
    };   
    void  DrawTextureFile() {
        if (m_pTexture == nullptr) return;
        m_pTexture->DrawFile(); 
    };
    /// <summary>
    /// 전략(Strategy)패턴
    /// </summary>
    /// <param name="pSound"></param>
    void  PlaySoundFile(TSound*     pSound) {
        if (pSound == nullptr) return;
        pSound->PlayFile();
    };
    void  DrawTextureFile(TTexture*   pTexture) {
        if (pTexture == nullptr) return;
        pTexture->DrawFile();
    };

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

    TCharacterImplOP chImplA;
    chImplA.m_pSound = std::make_shared<TSoundWave>(L"Demo.wav");

    TCharacterImplOP chImplB;
    chImplB.m_pSound = std::make_shared<TSoundMP3>(L"Demo.mp3");

    TCharacterImplOP chImplC;
    chImplC.m_pTexture = std::make_shared<TTextureJPG>(L"Demo.jpg");

    TCharacterImplOP chImplD;
    chImplD.m_pTexture = std::make_shared<TTextureBMP>(L"Demo.bmp");

    chImplA.PlaySoundFile();
    chImplB.PlaySoundFile();
    chImplC.DrawTextureFile();
    chImplD.DrawTextureFile();


    TCharacterImplOP strategy;
    strategy.PlaySoundFile(chImplA.m_pSound.get());
    strategy.DrawTextureFile(chImplD.m_pTexture.get());
    std::cout << "Hello World!\n";
}
