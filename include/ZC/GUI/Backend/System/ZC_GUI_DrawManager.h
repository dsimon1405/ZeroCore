#pragma once

// #include <ZC/Video/OpenGL/Shader/ZC_Pipeline.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/GUI/Backend/Window/ZC_GUI_Window.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

#include <list>

struct ZC_GUI_DrawManager
{
    bool isActive = true;

    std::list<ZC_GUI_Window*> mutableWins;
    ZC_GUI_Window* pImmutableWin = nullptr;   //  no metter wich of immutable wins, uses only for call virtual methods that work with static buffers

        //  DELETE
    uint vao;

    ZC_ShProg shProg;
    // ZC_Pipeline* pPipeline = nullptr;
    ZC_Texture texture;

    static inline std::string path_to_custom_icon_texture;

    ZC_GUI_DrawManager();

    void AddWindow(ZC_GUI_Window* pWindow);
    void EraseWindow(ZC_GUI_Window* pWindow);
    void Configure();
        //  reset uv in text objs (calls from ZC_GUI_TextManager::Configure())
    void Reconf_UpdateTextUV();

    void Draw(const ZC_Texture& text_texture);
    void UpdateWindowDrawState(ZC_GUI_Window* pWindow);
        //  must be called before ZC_SWindow creation. Texture coords must be defined user.
    static void SetPathToCustomIconTexture(std::string&& path);

private:
    ZC_ShProg CreateShProg();
};