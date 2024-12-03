#pragma once

#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/GUI/Backend/ZC_GUI_ObjData.h>
#include <ZC/GUI/Backend/Text/ZC_GUI_FontLoader.h>
#include <ZC/Tools/Math/ZC_Math.h>

#include <list>

struct ZC_GUI_ChData
{
    const typename ZC_GUI_Font::Character* pCh;
    int start_index;

    bool operator == (const ZC_GUI_ChData* pChData) const noexcept;
};

class ZC_GUI_TextManager
{
public:
    static inline ZC_GUI_TextManager* pTM;

    struct TextureRow;

    struct Text
    {
        enum Alignment  //  alignment data into the texture
        {
            Left,
            Center,
            Right
        };

        const ZC_GUI_Font* pFont = nullptr;   //  if nullptr then this is an empty space in TextureRow::texts
        TextureRow* pRow = nullptr;     //  row from wich current text
        bool isImmutable = true;
        std::wstring wstr;
        int width = 0;          //  width of texture in pixels (reserved width for texture could be larger then pixel width of wstr). Don't include max_texture_height.
        ZC_GUI_UV uv;
        int start_index_in_row = 0;    //  start index in texture's row. Range [0, pTexture::width]
        Alignment alignment;    //  if in texture reserved width more then pixel width of the wstr, may be used alignment

        int GetHeight() const;
            //  erase text from mutable_tex_rows
        void Erase();
            //  calculates pixel width of the wstr using pFont, if pFont not nullptr
        int CalculateWstrWidth() const;
    };

    struct FreeSpace    //  free space in texture
    {
        int start_index;    //  start index (pixel on bottom line)
        int width;          //  count free pixels on botton line
        int height;         //  height of the row
    };

    ZC_GUI_TextManager(ZC_Texture* _pTexture);
    ~ZC_GUI_TextManager();

    bool IsConfigured() const noexcept;
    void Configure();
    const ZC_Texture& GetTexture() const noexcept;

        //  find or create Text
    static Text* GetText(const ZC_GUI_Font* pFont, const std::wstring& wstr, bool isImmutable, int reserveWidth, Text::Alignment alignment, int* pWSTR_width = nullptr);
        //  update (mutable text) texture with new wstr
    static bool UpdateText(Text*& pText, int total_width, bool brootForceUpdate, const std::wstring& wstr);
        //  update (mutable text) texture with new wstr
    static bool UpdateText(Text* pText, const std::list<ZC_GUI_ChData>& chDatas);
        //  calculates wstr width
    static int CalculateChDataWidth(const std::list<ZC_GUI_ChData>& chDatas);

private:
        //  Rows of the texture of texts. One row can include different font with the same height.
    struct TextureRow
    {
        static inline int row_width;    //  all row in the texture have same width, sets in ctr ZC_GUI_TextManager(), takes width of the display or maximum openGL texture width
        static inline int max_texture_height;   //  maximum height of the openGL texture

        int row_height;     //  row's height
        std::list<Text> texts;

        int height_from_bottom_to_row = 0;   //  height from buttom of the texture to the current row. sets in Configure()

        TextureRow(i_zc _height);

        Text* FindText(const ZC_GUI_FontSrc& font_src, const std::wstring& wstr);
        Text* TryAddToEmptyPlace(Text& text);
        void EraseText(Text* pText);
    };

    static const int text_distance_pixel = 1;  //  pixels in texture, on rows between texts for width, and between rows for height

    ZC_Texture* pTexture = nullptr;
    std::list<TextureRow> immutable_tex_rows;
    std::list<TextureRow> mutable_tex_rows;

    /*
    Fills concrete quad part part of 2d array, with some add data. Both data and add are in 1d arrays, but interprete as 2d arrays.

    Params:
    - data - destination array (interpreted as a 2d array).
    - rData_index - start index in desination (data) (for eteration uses only first line and it's indexes).
    - data_width - length of the row in desination (data) (length of the 1d array in 2d array).
    - add - source array (interpreted as a 2d array).
    - add_width - length of the row in source (add) (length of the 1d array in 2d array).
    - add_height - count of not empty rows in source (add) (count of 1d arrays in 2d array).
    - add_startRow - index of the first not empty row in source (add).
    */
    static void AddSymbolData(std::vector<unsigned char>& data, int& rData_index, int data_width, const ZC_GUI_Font::Character* pCh);
    ZC_GUI_UV CalculateUV(Text* pText);
    static std::vector<unsigned char> CreateWstrData(Text* pText, int wstr_width);
    static std::vector<unsigned char> CreateChDataData(const std::list<ZC_GUI_ChData>& chDatas, Text* pText);
    static void MapTexture(Text* pText, const unsigned char* data);
};

typedef typename ZC_GUI_TextManager::Text::Alignment ZC_GUI_TextAlignment;