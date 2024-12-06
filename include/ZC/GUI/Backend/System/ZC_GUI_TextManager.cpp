#include "ZC_GUI_TextManager.h"

#include <ZC/GUI/Backend/System/ZC_GUI.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/ZC__System.h>

#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
ZC_GUI_TextManager::ZC_GUI_TextManager(ZC_Texture* _pTexture)
    : pTexture(_pTexture)
{
    int display_width = 0;
    int display_height = 0;
    ZC__Window::GetDisplaySize(display_width, display_height);      //  try to make texture row width equal to display, no sense to make large... 
    if (display_width == 0)
    {
        assert(false);  //  why don't get disoplay width        //  can't get display width, so use realy large rows from gl max texture width
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &display_width);     //  get max posible dimension (texture_max_dimension x texture_max_dimension) like (1024 x 1024)
        TextureRow::row_width = display_width;          //  make width and height with maximum dimaension of the texture
        TextureRow::max_texture_height = display_width;     //  make width and height with maximum dimaension of the texture
    }
    else
    {
        TextureRow::row_width = display_width;      //  make texture with display width
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TextureRow::max_texture_height);     //  get max posible dimension (texture_max_dimension x texture_max_dimension) like (1024 x 1024)
    }
    
    pTM = this;

    //      //  DRAW pTexture
    // static ZC_VAO vao;
    // static ZC_ShProg ShP = ZC_ShProg(
    //     ZC_Shader(ZC_Shader::ReadShaderFile("C:/Users/simon007/source/repos/Game1/Game/external/ZeroCore/assets/ZC/shaders/GUI/gui_text_test.vs", 0).pHead, GL_VERTEX_SHADER).id,
    //     ZC_Shader(ZC_Shader::ReadShaderFile("C:/Users/simon007/source/repos/Game1/Game/external/ZeroCore/assets/ZC/shaders/GUI/gui_text_test.fs", 0).pHead, GL_FRAGMENT_SHADER).id,
    //     ZC_Shader(ZC_Shader::ReadShaderFile("C:/Users/simon007/source/repos/Game1/Game/external/ZeroCore/assets/ZC/shaders/GUI/gui_text_test.glsl", 0).pHead, GL_GEOMETRY_SHADER).id
    // );
    // static ZC_DrawArrays upGLDraw(GL_POINTS, 0, 1);
    // static ZC_TexturesHolder th{ .pTexture = pTexture, .texturesCount = 1 };
    // static ZC_DSController ds_con(&ShP, &upGLDraw, &vao, th, {}, {{ ZC_RL_Default }}, {}, {});
    // ds_con.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);
}

ZC_GUI_TextManager::~ZC_GUI_TextManager()
{
    pTM = nullptr;
}

bool ZC_GUI_TextManager::IsConfigured() const noexcept
{
    return pTexture->GetId() != 0;
}

void ZC_GUI_TextManager::Configure()
{
    if (!pTM) return;
        //  calculate texture's height
    int total_height = 0;
    auto lamb_UpdateTexRowLimits = [&total_height](std::list<TextureRow>& tex_rows)
    {
        for (TextureRow& tr : tex_rows)
        {
            tr.height_from_bottom_to_row = total_height;
            total_height += tr.row_height + text_distance_pixel;
        }
    };
    lamb_UpdateTexRowLimits(immutable_tex_rows);
    lamb_UpdateTexRowLimits(mutable_tex_rows);
    
    bool isConfigured = IsConfigured();     //  must be before first texture creation
        //  create texture here, fill later, caurse CalculateUV() take width and height fro mthe texture
    *pTexture = ZC_Texture::TextureStorage2D(GL_R8, ZC_GUI_Bindings::location_tex_Text, TextureRow::row_width, total_height, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

    std::vector<unsigned char> data(TextureRow::row_width * total_height, 0);   //  texture data in 1d array
    auto lambFillData = [this, &data](TextureRow& tex_row)
    {
        int data_row_start_index = tex_row.height_from_bottom_to_row * tex_row.row_width;    //  start index of the row in the texture intepreted as 1d array  
        for (Text& text : tex_row.texts)
        {
            if (!(text.pFont)) continue;    //  don't have pFont, thet is empty space
            
            text.uv = pTM->CalculateUV(&text);  //  height_from_bottom_to_row was set higher, now we have all the data to calculate uv
            
            int text_data_index = data_row_start_index + text.start_index_in_row;     //  start index of the text in the texture interpreted as 1d array
            switch (text.alignment)    //  start index for text, may differ from data_index depending on alignment
            {
            case ZC_GUI_TextAlignment::Left: break;
            case ZC_GUI_TextAlignment::Center: text_data_index += (text.width - text.CalculateWstrWidth()) / 2.f; break;
            case ZC_GUI_TextAlignment::Right: text_data_index += text.width - text.CalculateWstrWidth(); break;
            }

            for (const wchar_t& wch : text.wstr)
            {
                auto pCh = text.pFont->GetCharacter(wch);
                if (!pCh) continue;
                if (&wch != text.wstr.data()) text_data_index += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
                AddSymbolData(data, text_data_index, TextureRow::row_width, pCh);
            }
        }
    };
    for (TextureRow& tr : immutable_tex_rows) lambFillData(tr);
    for (TextureRow& tr : mutable_tex_rows) lambFillData(tr);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    pTexture->GLTextureSubImage2D(0, 0, TextureRow::row_width, total_height, GL_RED, GL_UNSIGNED_BYTE, data.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        //  if allready configured, reconfigure - update uv in text objs
    if (isConfigured) ZC_GUI::pGUI->drawManager.Reconf_UpdateTextUV();
}

const ZC_Texture& ZC_GUI_TextManager::GetTexture() const noexcept
{
    return *pTexture;
}

typename ZC_GUI_TextManager::Text* ZC_GUI_TextManager::GetText(const ZC_GUI_Font* pFont, const std::wstring& wstr, bool isImmutable,
    int reserveWidth, Text::Alignment alignment, int* pWSTR_width)
{
    if (!pTM) return nullptr;
    // assert(pFont);
    if (!pFont) return nullptr;

    int wstr_width = pWSTR_width ? *pWSTR_width : pFont->CalculateWstrWidth(wstr);
    if (!isImmutable && reserveWidth > wstr_width) wstr_width = reserveWidth;   //  reserveWidth more then wstr width (only for mutable texts)
    
        //  try to find in existing texts
    for (TextureRow& tex_row : pTM->immutable_tex_rows)
    {
        Text* pExisting_text = tex_row.FindText(pFont->font_src, wstr);
        if (pExisting_text && (pExisting_text->alignment == alignment && (alignment == ZC_GUI_TextAlignment::Left || wstr_width == pExisting_text->width)))     //  if all true no metter mutable or not, may be used
            return pExisting_text;
    }
    
    if (isImmutable && pTM->IsConfigured()) return nullptr;   //  no adds after configuration (for immutable text)
        
        //  try to find empty space in rows
    Text text
        {
            .pFont = pFont,
            .pRow = nullptr,
            .isImmutable = isImmutable, 
            .wstr = wstr,
            .width = wstr_width,
            .uv = {},
            .start_index_in_row = 0, 
            .alignment = isImmutable ? Text::Alignment::Left : alignment     //  mutable text could have only left alignment (they can't be changed, no need to fill texture with empty spases on front)
        };
    Text* pText = nullptr;
    for (TextureRow& tex_row : (isImmutable ? pTM->immutable_tex_rows : pTM->mutable_tex_rows))
    {
        pText = tex_row.TryAddToEmptyPlace(text);
        if (pText)      //  found place 
        {
            if (pTM->IsConfigured())
            {
                pText->uv = pTM->CalculateUV(pText);
                MapTexture(pText, CreateWstrData(pText, wstr_width).data());
            }
            // if (pTM->IsConfigured()) MapTexture(pText, CreateWstrData(pText, nullptr).data());       //  ??????
            return pText;
        }
    }

    if (!pText)     //  if not found place, finally create new row and add text
    {
        pText = (isImmutable ? pTM->immutable_tex_rows.emplace_back(text.pFont->font_src.pixels_height) : pTM->mutable_tex_rows.emplace_back(text.pFont->font_src.pixels_height))
            .TryAddToEmptyPlace(text);
        if (pTM->IsConfigured()) pTM->Configure();      //  texture allready configured, need reconfigure
    }
    
    return pText;
}

bool ZC_GUI_TextManager::UpdateText(Text*& pText, int total_width, bool brootForceUpdate, const std::wstring& wstr)
{
    if (!pTM) return false;

    int wstr_width = pText->pFont->CalculateWstrWidth(wstr);
    if (pText->isImmutable) pText = GetText(pText->pFont, wstr, false, total_width, pText->alignment, &wstr_width);     //  pText is immutable. Get new text
    else if (!brootForceUpdate) //  pText mutable and don't need broot force map
    {
        pText->Erase();   //  delete existing mutable (free space in texture)
        pText = GetText(pText->pFont, wstr, false, total_width, pText->alignment, &wstr_width);  //  Get new text
    }
    else
    {
        pText->wstr = wstr;
        MapTexture(pText, CreateWstrData(pText, wstr_width).data());   //  pText is mutable and need brootForceUpdate. Map part of existing texture
    }
    return true;
}

bool ZC_GUI_TextManager::UpdateText(Text* pText, const std::list<ZC_GUI_ChData>& chDatas)
{
    if (!pTM) return false;

    if (pText->width < CalculateChDataWidth(chDatas)) return false;   //  new length can't be longer then current
    MapTexture(pText, CreateChDataData(chDatas, pText).data());
    return true;
}

int ZC_GUI_TextManager::CalculateChDataWidth(const std::list<ZC_GUI_ChData>& chDatas)
{
    if (!pTM) return 0;

    int chDatas_width = 0;
    for (const ZC_GUI_ChData& chData : chDatas)
    {
        if (&chData != &(chDatas.front())) chDatas_width += chData.pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        chDatas_width += chData.pCh->width;
    }
    return chDatas_width;
}

ZC_GUI_UV ZC_GUI_TextManager::CalculateUV(Text* pText)
{
        //  uv calculates in 2d array wisth textures width and height, not as in 1d array in wich stores data to be glMap
    float bl_x = pText->start_index_in_row;   //  distance from left edge of the row to the text
    float tr_x = bl_x + pText->width;
    float bl_y = pText->pRow->height_from_bottom_to_row;
    float tr_y = bl_y + pText->pRow->row_height;
    return ZC_GUI_UV
        {
            .bl = { bl_x / pTexture->GetWidth(), bl_y / pTexture->GetHeight() },
            .tr = { tr_x / pTexture->GetWidth(), tr_y / pTexture->GetHeight() }
        };
}

void ZC_GUI_TextManager::AddSymbolData(std::vector<unsigned char>& data, int& rData_index, int data_width, const ZC_GUI_Font::Character* pCh)
{
    int data_i = rData_index + (pCh->start_row * data_width);     //  start index in data vector, miss white space rows in add vector
    int add_i = pCh->start_row * pCh->width;   //  start index in add vector, miss space rows
    int height = pCh->height;
    for (; height > 0; data_i += data_width, --height)     //  data_i += data_width - get same position in the next row. Looping while have height of add (in 2d array)
        for (int add_line_width = 0; add_line_width < pCh->width; ++add_line_width)   //  fill rows (same start pos for each row in 2d array)
            data[data_i + add_line_width] = pCh->data[add_i++];

    rData_index += pCh->width;
}

std::vector<unsigned char> ZC_GUI_TextManager::CreateWstrData(Text* pText, int wstr_width)
{
    std::vector<unsigned char> data(pText->width * pText->pFont->font_src.pixels_height, 0);
    
    int data_index = pText->alignment == ZC_GUI_TextAlignment::Center ? (pText->width - wstr_width) / 2.f
                    : pText->alignment == ZC_GUI_TextAlignment::Right ? pText->width - wstr_width
                    : 0;
    for (const wchar_t& wch : pText->wstr)
    {
        auto pCh = pText->pFont->GetCharacter(wch);
        if (!pCh) continue;
        if (&wch != pText->wstr.data()) data_index += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        AddSymbolData(data, data_index, pText->width, pCh);
    }
    return data;
}

std::vector<unsigned char> ZC_GUI_TextManager::CreateChDataData(const std::list<ZC_GUI_ChData>& chDatas, Text* pText)
{
    std::vector<unsigned char> data(pText->width * pText->pFont->font_src.pixels_height);
    int data_index = 0;
    std::wstring new_wstr;
    new_wstr.reserve(chDatas.size());
    for (const ZC_GUI_ChData& chData : chDatas)
    {
        if (&chData != &(chDatas.front())) data_index += chData.pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        AddSymbolData(data, data_index, pText->width, chData.pCh);
        new_wstr.append(1, chData.pCh->character);     //  add symbol in new wstring
    }
    pText->wstr = std::move(new_wstr);
    return data;
}

void ZC_GUI_TextManager::MapTexture(Text* pText, const unsigned char* data)
{
    if (!pTM->IsConfigured()) return;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    pTM->pTexture->GLTextureSubImage2D(pText->start_index_in_row, pText->pRow->height_from_bottom_to_row, pText->width, pText->pFont->font_src.pixels_height, GL_RED,  GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}


    //  ZC_GUI_TextManager::Text

int ZC_GUI_TextManager::Text::GetHeight() const
{
    return pFont ? pFont->font_src.pixels_height : 0;
}

void ZC_GUI_TextManager::Text::Erase()
{
    assert(pRow);   //  what with row?
    if (!isImmutable) pRow->EraseText(this);    //  erase only muttable, only from mutable_tex_rows
}

int ZC_GUI_TextManager::Text::CalculateWstrWidth() const
{
    return pFont ? pFont->CalculateWstrWidth(wstr) : 0;
}

    //  ZC_GUI_ChData

bool ZC_GUI_ChData::operator == (const ZC_GUI_ChData* pChData) const noexcept
{
    return pChData == this;
}


    //  ZC_GUI_TextManager::TextureRow

ZC_GUI_TextManager::TextureRow::TextureRow(i_zc _height)
    : row_height(_height),
    texts{ Text{ .pRow = this, .width = TextureRow::row_width } }   //  create Text with empty space. Other params fill in TextureRow::TryToAdText or ZC_GUI_TextManager::Configure(uv, start_index)
{}

ZC_GUI_TextManager::Text* ZC_GUI_TextManager::TextureRow::FindText(const ZC_GUI_FontSrc& font_src, const std::wstring& wstr)
{
    if (font_src.pixels_height != row_height) return nullptr;     //  row have another height
    for (Text& t : texts)
        if (t.pFont && t.pFont->font_src == font_src && t.wstr == wstr) return &t;    //  if pFont nullptr, is empty space
    return nullptr;
}

ZC_GUI_TextManager::Text* ZC_GUI_TextManager::TextureRow::TryAddToEmptyPlace(ZC_GUI_TextManager::Text& text)
{
    if (text.pFont->font_src.pixels_height != row_height) return nullptr;     //  row have another height
    
    int request_width = text.width + text_distance_pixel;
    std::list<Text>::iterator add_iter = texts.end();
    for (auto iter = texts.begin(); iter != texts.end(); ++iter)
    {
        if (!(iter->pFont))     //  found nullptr, empty space
        {
            if (iter->width == request_width)  //  perfect, a place with the same width has been found, add data here
            {
                iter->alignment = text.alignment;
                iter->isImmutable = text.isImmutable;
                iter->pFont = text.pFont;
                iter->wstr = text.wstr;
                iter->pRow = this;
                iter->width -= text_distance_pixel;     //  one pixel were for distance
                return &*iter;
            }   //  can be place in this wisth;   first one found, just add;    new iter have better width then previous, change
            else if (iter->width > request_width && (add_iter == texts.end() || add_iter->width > iter->width)) add_iter = iter;
        }
    }
    
    if (add_iter == texts.end()) return nullptr;    //  couldn't find the place

        //  iter is larger than needed, it needs to be divided
    add_iter->width -= request_width;    //  new data will be from the right, left part is empty space make smaller
    text.start_index_in_row = add_iter->start_index_in_row + add_iter->width;     //  get start index for new text
    text.pRow = this;
    return &*(texts.emplace(++add_iter, text));
}

void ZC_GUI_TextManager::TextureRow::EraseText(Text* pText)
{
    if (pText->isImmutable) return;     //  can't erase immutable
    for (auto iter = texts.begin(); iter != texts.end(); ++iter)
    {
        if (&*iter == pText)    //  found
        {
            auto iter_cur = iter;
            iter_cur->pFont = nullptr;      //  mark as empty space
            iter_cur->uv = {};      //  must be recalculated if will be used space
            iter_cur->width += text_distance_pixel;     //  return distanced pixel waisted in TextureRow::TryAddToEmptyPlace() ... iter->width -= text_distance_pixel_x
            if (iter != texts.begin())      //  cur_iter not first, check previous iterator
            {
                auto iter_prev = --iter;
                if (!(iter_prev->pFont))    //  empty space, unite with current
                {
                    iter_cur->start_index_in_row -= iter_prev->width;
                    iter_cur->width += iter_prev->width;
                    texts.erase(iter_prev);     //  previous iter was absorbed, must be erased
                }
            }
            auto iter_next = iter_cur;
            if (++iter_next != texts.end())     //  cur iter not last, check next iter
            {
                if (!(iter_next->pFont))    //  empty space, unite with current
                {
                    iter_cur->width += iter_next->width;    //  need update only width of the current iter
                    texts.erase(iter_next);     //  next iter was absorbed, must be erased
                }
            }
            break;
        }
    }
}