#include <ZC/GUI/Text/ZC_GUI_TextManager.h>

#include <ZC/GUI/ZC_GUI.h>

ZC_GUI_TextManager::ZC_GUI_TextManager()
{
    if (fontParams.fontPath.empty()) fontParams.fontPath = ZC_GUI_FontLoader::GetPath(ZC_GUI_FontLoader::FontName::Arial);
    font = ZC_GUI_FontLoader::LoadFont(fontParams.fontPath.c_str(), fontParams.pix_height, fontParams.fontElements);
    pTM = this;
}

ZC_GUI_TextManager::~ZC_GUI_TextManager()
{
    pTM = nullptr;
}

bool ZC_GUI_TextManager::IsConfigured() const noexcept
{
    return texture.GetId() != 0;
}

void ZC_GUI_TextManager::BindTextureUnit()
{
    texture.GLBindTextureUnit();
}

int ZC_GUI_TextManager::GetFontHeight()
{
    return font.GetHeight();
}

void ZC_GUI_TextManager::Configure(bool doubleWidth)
{
    if (!pTM) return;

    int total_width = (immutable_texts.size() * text_distance_pixel) + (mutable_texts.size() * text_distance_pixel);  //  add distances in width
    for (Text& text : immutable_texts) total_width += text.width;
    for (Text& text : mutable_texts) total_width += text.width;
    if (doubleWidth) total_width *= 2;  //  need more space

    int font_height = pTM->font.GetHeight();
    std::vector<unsigned char> data(total_width * font_height);
    int data_index = 0;
    auto lambFillData = [this, &data, &data_index, total_width](std::list<Text>& texts)
    {
        for (Text& text : texts)
        {
            text.start_index = data_index;
            text.uv = ZC_GUI_UV{ .bl{ (float)data_index / (float)total_width, 0.f }, .tr{ float(data_index + text.width) / (float)total_width, 1.f } };
            for (const wchar_t& wch : text.wstr)
            {
                auto pCh = font.GetCharacter(wch);
                if (!pCh) continue;
                if (&wch != text.wstr.data()) data_index += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
                font.AddSymbolData(data, data_index, total_width, pCh);
            }
            if (!(text.isImmutable)) data_index += text.width - CalculateWstrWidth(text.wstr);  //  text width coud be longer then his current wstr, in that case move data_index
            data_index += text_distance_pixel;
        }
    };
    lambFillData(immutable_texts);
    lambFillData(mutable_texts);

    bool isConfigured = IsConfigured();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture = ZC_Texture::TextureStorage2DFill(GL_R8, ZC_GUI_Bindings::bind_tex_Text, total_width,
        font_height, data.data(), GL_RED, GL_UNSIGNED_BYTE, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        //  update freeSpaces
    freeSpaces.clear();
    if (data_index != total_width)
        freeSpaces.emplace_back(FreeSpace{ .start_index = data_index, .width = total_width - data_index + 1 });
        //  if allready configured, reconfigure - update uv in text objs
    if (isConfigured) ZC_GUI::pGUI->drawManager.Reconf_UpdateTextUV();
}

void ZC_GUI_TextManager::SetParams(Params&& _fontParams)
{
    fontParams = _fontParams;
}

typename ZC_GUI_TextManager::Text* ZC_GUI_TextManager::GetText(const std::wstring& wstr, bool isImmutable, int reserveWidth)
{
    if (!pTM) return nullptr;

    if (isImmutable)
    {
        Text* pExisting_text = ZC_Find(pTM->immutable_texts, wstr);
        if (pExisting_text) return pExisting_text;
        else if (pTM->IsConfigured()) return nullptr;   //  no adds after configuration
    }

    int wstr_width = CalculateWstrWidth(wstr);
    if (reserveWidth > wstr_width) wstr_width = reserveWidth;   //  reserved more width then in wstr
    
    Text* pText = isImmutable ? &(pTM->immutable_texts.emplace_back(Text{ .isImmutable = isImmutable, .wstr = wstr, .width = wstr_width, }))
        : &(pTM->mutable_texts.emplace_back(Text{ .isImmutable = isImmutable, .wstr = wstr, .width = wstr_width, }));
    
    if (pTM->IsConfigured()) ProcessDeletableText(wstr_width, pText);    //  text not configured yet
    
    return pText;
}

void ZC_GUI_TextManager::ProcessDeletableText(int wstr_width, Text* pText)
{
        //  find free space in texture
    const std::list<FreeSpace>::iterator emptyIter;
    typename std::list<FreeSpace>::iterator freeSpaceIter = emptyIter;
    for (auto iter = pTM->freeSpaces.begin(); iter != pTM->freeSpaces.end(); ++iter)    //  try to find free space with a width as close as possible to wstr_width
    {
        if (iter->width == wstr_width)  //  find same width (perfect)
        {
            freeSpaceIter = iter;
            break;
        }
        else if (iter->width > wstr_width)  //  larger then need width
        {
            if (freeSpaceIter == emptyIter) freeSpaceIter = iter;   //  first freeSpace
            else if (iter->width < freeSpaceIter->width) freeSpaceIter = iter;  //  space with more closer width
        }
    }
        //  fill texture data and update pText data
    int fontHeight = pTM->font.GetHeight();
    auto lambFillTextureAndpTextData = [&pText, fontHeight, &freeSpaceIter]()
    {
        std::vector<unsigned char> data = CreateWstrData(pText->wstr, pText->width);    //  prepare tuxture data
        MapTexture(freeSpaceIter->start_index, pText->width, data.data());    //  fill texture
            //  add start index
        pText->start_index = freeSpaceIter->start_index;
            //  update uv
        pText->uv = ZC_GUI_UV{ .bl{ (float)freeSpaceIter->start_index / float(pTM->texture.GetWidth()), 0.f },
            .tr{ float(freeSpaceIter->start_index + pText->width) / float(pTM->texture.GetWidth()), 1.f } };
    };

    if (freeSpaceIter == emptyIter) pTM->Configure(true);     //  can't find freespace, need reconfig and double texture size
    else if (freeSpaceIter->width == wstr_width)     //  same width
    {
        lambFillTextureAndpTextData();
        pTM->freeSpaces.erase(freeSpaceIter);   //  delete taken free space
    }
    else    //  free space large then need (split it)
    {
        lambFillTextureAndpTextData();
        int length = pText->width + text_distance_pixel;
        freeSpaceIter->start_index += length;   //  move start index
        freeSpaceIter->width -= length;     //  reduce width
    }
}

    //  erase only deletable text (not stacionar), to free space in texture
void ZC_GUI_TextManager::EraseText(Text* pText)
{
    if (pText->isImmutable) return; //  delete only deletable
    if (pTM->freeSpaces.empty())    //  first empty space
    {
        pTM->freeSpaces.emplace_back(FreeSpace{ .start_index = pText->start_index, .width = pText->width });
        return;
    }
        //  find a place in freeSpaces (sequence -> start_index ascending)
    typename std::list<FreeSpace>::iterator iter = pTM->freeSpaces.begin();     //  iter on next element, to emplace() before iter
    for ( ; iter != pTM->freeSpaces.end(); ++iter)
        if (pText->start_index < iter->start_index) break;

    auto nextIter = iter;   //  iter is next iter (nextIter uses for emplace in freeSpaces). Only nextIter have correct itertor from list
    auto prevIter = --iter;  //  now iter is prev iter
        //  if nextIter is begin, no data in prevIter
    bool adjacentPrevious = nextIter != pTM->freeSpaces.begin() && (prevIter->start_index + prevIter->width + text_distance_pixel == pText->start_index);
        //  if nextIter is end, no data in nextIter
    bool adjacentNext = nextIter != pTM->freeSpaces.end() && (pText->start_index + pText->width + text_distance_pixel == nextIter->start_index);

    if (adjacentPrevious && adjacentNext)   //  merge: prevIter, pText, nextIter
    {   //  all merge in prevIter
        prevIter->width += text_distance_pixel + pText->width + text_distance_pixel + nextIter->width;  //  3 spaces are adjacent, merge all
        pTM->freeSpaces.erase(nextIter);    //  where merge in prevIter    
    }
    else if (adjacentPrevious) prevIter->width += text_distance_pixel + pText->width;     //  add pText to prevIter
    else if (adjacentNext)  //  add pText to nextIter
    {
        nextIter->start_index = pText->start_index;
        nextIter->width += pText->width + text_distance_pixel;
    }
    else pTM->freeSpaces.emplace(nextIter, FreeSpace{ .start_index = pText->start_index, .width = pText->width });
}

bool ZC_GUI_TextManager::UpdateText(Text* pText, const std::wstring& wstr)
{
    if (pText->width < CalculateWstrWidth(wstr)) return false;   //  new length can't be longer then current
    MapTexture(pText->start_index, pText->width, CreateWstrData(wstr, pText->width).data());
    return true;
}

bool ZC_GUI_TextManager::UpdateText(Text* pText, const std::list<ZC_GUI_ChData>& chDatas)
{
    if (pText->width < CalculateChDataWidth(chDatas)) return false;   //  new length can't be longer then current
    MapTexture(pText->start_index, pText->width, CreateChDataData(chDatas, pText->width).data());
    return true;
}

int ZC_GUI_TextManager::CalculateWstrWidth(const std::wstring& wstr)
{
    int wstr_width = 0;
    for (const wchar_t& wch : wstr)
    {
        const typename ZC_GUI_Font::Character* pCh = pTM->font.GetCharacter(wch);
        if (!pCh) continue;     //  symbol wasn't loaded
        if (&wch != wstr.data()) wstr_width += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        wstr_width += pCh->width;
    }
    return wstr_width;
}

int ZC_GUI_TextManager::CalculateChDataWidth(const std::list<ZC_GUI_ChData>& chDatas)
{
    int chDatas_width = 0;
    for (const ZC_GUI_ChData& chData : chDatas)
    {
        if (&chData != &(chDatas.front())) chDatas_width += chData.pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        chDatas_width += chData.pCh->width;
    }
    return chDatas_width;
}

std::vector<unsigned char> ZC_GUI_TextManager::CreateWstrData(const std::wstring& wstr, int width)
{
    std::vector<unsigned char> data(width * pTM->font.GetHeight());
    int data_index = 0;
    for (const wchar_t& wch : wstr)
    {
        auto pCh = pTM->font.GetCharacter(wch);
        if (!pCh) continue;
        if (&wch != wstr.data()) data_index += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        pTM->font.AddSymbolData(data, data_index, width, pCh);
    }
    return data;
}

std::vector<unsigned char> ZC_GUI_TextManager::CreateChDataData(const std::list<ZC_GUI_ChData>& chDatas, int width)
{
    std::vector<unsigned char> data(width * pTM->font.GetHeight());
    int data_index = 0;
    for (const ZC_GUI_ChData& chData : chDatas)
    {
        if (&chData != &(chDatas.front())) data_index += chData.pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        pTM->font.AddSymbolData(data, data_index, width, chData.pCh);
    }
    return data;
}

void ZC_GUI_TextManager::MapTexture(int start_index, int width, const unsigned char* data)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    pTM->texture.GLTextureSubImage2D(start_index, 0, width, pTM->font.GetHeight(), GL_RED,  GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}


//  ZC_GUI_TextManager::Text

bool ZC_GUI_TextManager::Text::operator == (const std::wstring& _wstr) const noexcept
{
    return wstr == _wstr;
}

int ZC_GUI_TextManager::Text::GetHeight()
{
    return pTM->font.GetHeight();
}