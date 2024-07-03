#pragma once

#include <ZC/GUI/ZC_GUI_FontLoader.h>
#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <list>


#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>


struct ZC_GUI_TextManager
{
    struct Params
    {
        std::string fontPath;
        ulong pix_height = 18;
        ZC_GUI_FontElements fontElements = ZC_GUI_FE_Symbols | ZC_GUI_FE_English | ZC_GUI_FE_Russian;
    };

    struct Text
    {
        std::wstring wstr;
        int width = 0;                      //  text width in pixels
        ZC_GUI_UV uv;

        bool operator == (const std::wstring& _wstr) const noexcept
        {
            return wstr == _wstr;
        }

        int GetHeight()
        {
            return pTM->font.GetHeight();
        }
    };

    ~ZC_GUI_TextManager()
    {
        pTM = nullptr;
    }

        //  Change default FontParams. Must be called before ZC_SWindow create.
    static void SetParams(Params&& _fontParams)
    {
        fontParams = _fontParams;
    }

    bool Init()
    {
        if (fontParams.fontPath.empty()) fontParams.fontPath = ZC_GUI_FontLoader::GetPath(ZC_GUI_FontLoader::FontName::Arial);
        font = ZC_GUI_FontLoader::LoadFont(fontParams.fontPath.c_str(), fontParams.pix_height, fontParams.fontElements);
        if (font.characters.empty()) return false;
            //  add loaded symbols in static_texts
        for (auto ch : font.characters) texts.emplace_back(Text
            {
                .wstr = std::wstring(1, ch.symbol),
                .width = ch.width,
            });
        pTM = this;
        return true;
    }

    void Configure()
    {
        if (!pTM) return;

        const int distance_pixel = 0;  //  2 pixels between texts in texture 
        int total_width = texts.size() * distance_pixel;  //  add distances in width
        for (Text& text : texts) total_width += text.width;
        
        int font_height = pTM->font.GetHeight();
        std::vector<unsigned char> data(total_width * font_height);
        int data_index = 0;
        for (Text& text : texts)
        {
            text.uv = ZC_GUI_UV{ .bl{ (float)data_index / (float)total_width, 0.f}, .tr{ (float)(data_index + text.width) / (float)total_width, 1.f } };
            for (const wchar_t& wch : text.wstr)
            {
                auto pCh = font.FindCharacter(wch);
                if (!pCh) continue;
                if (&wch != text.wstr.data()) data_index += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
                font.AddSymbolData(data, data_index, total_width, pCh);
            }
            data_index += distance_pixel;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        texture = ZC_Texture::TextureStorage2DFill(GL_R8, ZC_GUI_Bindings::bind_tex_Text, total_width,
            font_height, data.data(), GL_RED, GL_UNSIGNED_BYTE, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    static Text* GetText(const std::wstring& wstr)
    {
        if (!pTM) return nullptr;

        Text* pExisting_text = ZC_Find(pTM->texts, wstr);
        if (pExisting_text) return pExisting_text;
        else if (pTM->IsConfigured()) return nullptr;   //  no adds after configuration

        int wstr_width = 0;
        for (const wchar_t& wch : wstr)
        {
            const typename ZC_GUI_Font::Character* pCh = pTM->font.FindCharacter(wch);
            if (!pCh) continue;     //  symbol wasn't loaded
            wstr_width += pCh->width;
            if (&wch != wstr.data()) wstr_width += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        }
        return &(pTM->texts.emplace_back(Text
            {
                .wstr = wstr,
                .width = wstr_width,
            }));
    }

    bool IsConfigured() const noexcept
    {
        return texture.GetId() != 0;
    }

    void BindTextureUnit()
    {
        texture.GLBindTextureUnit();
    }

private:
    static inline Params fontParams { .pix_height = 18, .fontElements = ZC_GUI_FE_Symbols | ZC_GUI_FE_English | ZC_GUI_FE_Russian };
    static inline ZC_GUI_TextManager* pTM;

    ZC_GUI_Font font;
    std::list<Text> texts;
    ZC_Texture texture;
};