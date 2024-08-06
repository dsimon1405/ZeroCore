#pragma once

#include <ZC/GUI/Text/ZC_GUI_FontLoader.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <list>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>

#include <cassert>

struct ZC_GUI_ChData
{
    const typename ZC_GUI_Font::Character* pCh;
    int start_index;

    bool operator == (const ZC_GUI_ChData* pChData) const noexcept;
};


struct ZC_GUI_TextManager
{
    static inline ZC_GUI_TextManager* pTM;
    ZC_GUI_Font font;

    struct Params
    {
        std::string fontPath;
        ulong pix_height;
        ZC_GUI_FontElements fontElements;
    };

    struct Text
    {
        enum Alignment
        {
            Left,
            Center,
            Right
        };

        bool isImmutable;
        std::wstring wstr;
        int width = 0;          //  width of texture in pixels (reserved width for texture could be larger then pixel width of wstr)
        ZC_GUI_UV uv;
        int start_index = 0;    //  start index on the bottom line on the texture (start from bl, end is br)
        Alignment alignment;    //  if in texture reserved width more then pixel width of the wstr, may be used alignment

        bool operator == (const std::wstring& _wstr) const noexcept;
        int GetHeight();
    };

    struct FreeSpace    //  free space in texture
    {
        int start_index;    //  start index (pixel on bottom line)
        int width;          //  count free pixels on botton line 
    };

    ZC_GUI_TextManager();
    ~ZC_GUI_TextManager();

    bool IsConfigured() const noexcept;
    void BindTextureUnit();
    void Configure(bool doubleWidth);

    static int GetFontHeight();

        //  Change default FontParams. Must be called before ZC_SWindow create.
    static void SetParams(Params&& _fontParams);
        //  find or create Text
    static Text* GetText(const std::wstring& wstr, bool isImmutable, int reserveWidth, Text::Alignment alignment, int* pWSTR_width = nullptr);
    static void ProcessDeletableText(int wstr_width, Text* pText);
        //  erase only deletable text (not stacionar), to free space in texture
    static void EraseText(Text* pText);
        //  update texture with new wstr
    static bool UpdateText(Text*& pText, int total_width, bool brootForceUpdate, const std::wstring& wstr);
        //  update texture with new wstr
    static bool UpdateText(Text* pText, const std::list<ZC_GUI_ChData>& chDatas);
        //  calculates wstr width
    static int CalculateWstrWidth(const std::wstring& wstr);
    static int CalculateChDataWidth(const std::list<ZC_GUI_ChData>& chDatas);

private:
    static inline Params fontParams { .pix_height = 14, .fontElements = ZC_GUI_FE__Symbols | ZC_GUI_FE__English | ZC_GUI_FE__Russian };
    static const int text_distance_pixel = 0;  //  pixels between texts in texture

    std::list<Text> immutable_texts;
    std::list<Text> mutable_texts;
    std::list<FreeSpace> freeSpaces;    //  free spaces in texture (space for texts from mutable_texts, created after configuration)
    ZC_Texture texture;

    static std::vector<unsigned char> CreateWstrData(Text* pText, int* pWSTR_width);
    static std::vector<unsigned char> CreateChDataData(const std::list<ZC_GUI_ChData>& chDatas, Text* pText);
    static void MapTexture(int start_index, int width, const unsigned char* data);
};

typedef typename ZC_GUI_TextManager::Text::Alignment ZC_GUI_TextAlignment;