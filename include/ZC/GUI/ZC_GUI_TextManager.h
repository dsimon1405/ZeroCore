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
        std::string fontPath = ZC_GUI_FontLoader::GetPath(ZC_GUI_FontLoader::FontName::Arial);
        ulong pix_height = 18;
        ZC_GUI_FontElements fontElements = ZC_GUI_FE_Symbols | ZC_GUI_FE_English | ZC_GUI_FE_Russian;
    };

    struct Text
    {
        std::wstring wstr;
        bool isStatic;                      //  in wich list static_texts or non_static_texts
        ZC_GUI_UV uv;
        int start_buf_index = 0;            //  start index in buffer (texture)
        std::vector<unsigned char> data;    //  texture data (alpha channel)
        int width = 0;                      //  pixel width of the texture

        int start_row = 0;  //  index of the first row with data (not spaces)
        int height = 0;     //  rows count from stat_row to the last row with the not white space data

        bool operator == (const std::wstring& _wstr) const noexcept
        {
            return wstr == _wstr;
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

    void Init()
    {
        font = ZC_GUI_FontLoader::LoadFont(fontParams.fontPath.c_str(), fontParams.pix_height, fontParams.fontElements);
            //  add loaded symbols in static_texts
        for (auto ch : font.characters) static_texts.emplace_back(Text
            {
                .wstr{ 1, ch.symbol },
                .isStatic = true,
                .uv{},  //  fills on configure
                .start_buf_index = 0,   //  fills on configure
                .data = ch.data,
                .width = ch.width,
                .start_row = ch.start_row,
                .height = ch.height
            });
        pTM = this;        
    }
    static Text* AddText(const std::wstring& wstr, bool isStatic, int width)
    {
        if (!pTM) return nullptr;
        
        if (isStatic)
        {
            if (pTM->IsConfigured()) return nullptr;     //  can't add static_text after configuration
            Text* pExisting_text = ZC_Find(pTM->static_texts, wstr);
            if (pExisting_text) return pExisting_text;
            int start_row = 0;
            int height = 0;
            std::vector<unsigned char> data = pTM->font.FillWStrData(wstr, width, start_row, height);
            pTM->static_texts.emplace_back(Text
                {
                    .wstr = wstr,
                    .isStatic = true,
                    .uv{},  //  fills on configure
                    .start_buf_index = 0,   //  fills on configure
                    .data = std::move(data),
                    .width = width,
                    .start_row = start_row,
                    .height = height
                });
        }
    }

    bool IsConfigured() const noexcept
    {
        return textureManager.tex_static.GetId() != 0;
    }

private:
    static inline Params fontParams;
    static inline ZC_GUI_TextManager* pTM;

    ZC_GUI_Font font;
    std::list<Text> static_texts;
    std::list<Text> non_static_texts;
    std::list<Text*> non_static_free_place;

    struct TextureManager
    {
        ZC_Texture tex_static;
        ZC_Texture tex_non_static;

        void Configure()
        {
            CreateTexture(pTM->static_texts, 1);
            CreateTexture(pTM->non_static_texts, 2);

    //  check how looks texture
            //  create non_static_texts managment
        }
            //  bufferCoef multiply place in buffer
        void CreateTexture(std::list<Text>& texts, int bufferCoef)
        {
            const int text_distance = 2;  //  2 pixels between texts in texture 
            int total_width = texts.size() * text_distance;  //  add distances in width
            for (Text& text : texts) total_width += text.width;
            
            int font_height = pTM->font.GetHeight();
            std::vector<unsigned char> data((total_width * font_height) * bufferCoef);
            int data_index = 0;
            for (Text& text : texts)
            {
                text.uv = ZC_GUI_UV{ .bl{ (float)text_distance / (float)total_width, 0.f}, .tr{ (float)(text_distance + text.width) / (float)total_width, 1.f } };
                text.start_buf_index = data_index;
                ZC_GUI_Font::FillQuad2D(data, data_index, total_width, text.data, text.width, text.height, text.start_row);
                data_index += text_distance;
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            tex_static = ZC_Texture::TextureStorage2DFill(GL_R8, ZC_GUI_Bindings::bind_Texture_text_static, total_width,
                font_height, data.data(), GL_RED, GL_UNSIGNED_BYTE, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    };
    TextureManager textureManager;
};