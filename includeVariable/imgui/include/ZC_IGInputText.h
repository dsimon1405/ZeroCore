#pragma once

#include <ZC/Tools/ZC_string.h>
#include <ZC/Tools/Container/ZC_DA.h>

#include <imgui.h>

class ZC_IGInputText
{
public:
    virtual ~ZC_IGInputText()
    {
        if (pFocused == this)
        {
            pFocused = nullptr;
            isFocused = false;
        }
    }

    static bool IsOneOfInputTextFocused()
    {
        return isFocused;
    }

    void DrawIGIT()
    {
        pCallingNow = this;
        /*
        Only one InputText object could be focus at a time. So if in prevous frame current object were in focuse, sets isFocused to false. If ImGui::InputText()
        will call ZC_IGInputText::FlagCallback() there will updated isFocused state, if it doesn't heppens then object not in focus any more. ZC_IGInputText::FlagCallback()
        will be called if object in focuse, becaurse -> flags, includes ImGuiInputTextFlags_CallbackAlways, no metter set them user or not, we set it in each case to have
        focuse event callback.
        */
        if (this == pFocused) isFocused = false;
        if (ImGui::InputText(label.c_str(), inputTextBuffer.pHead, inputTextBuffer.size, flags, FlagCallback)) VInputCallbackIGIT(inputTextBuffer);
        if (this == pFocused && !isFocused)     //  focus losed
        {
            VLostFocuseIGIT(inputTextBuffer);
            pFocused = nullptr;
        }
        pCallingNow = nullptr;
    }

protected:
    std::string label;
    ZC_DA<char> inputTextBuffer;

    ZC_IGInputText(std::string&& _label, size_t bufferSize, int _flags = 0)
        : label(std::move(_label)),
        inputTextBuffer(bufferSize),
        userSetCallbackAllways(_flags & ImGuiInputTextFlags_CallbackAlways),
        flags(_flags | ImGuiInputTextFlags_CallbackAlways)
    {}

    //  Calls if InputText() returns true. Parameter: buffer is protected variable(container) inputTextBuffer.
    virtual void VInputCallbackIGIT(ZC_DA<char>& buffer) {
    }
    //  Called if one of the flag events is raised. Parameter: data - can be static_cast to ImGuiInputTextCallbackData* wich contain protected variable(container) inputTextBuffer in different variables.
    virtual void VFlagEventCallbackIGIT(void* data) {}
    //  Called if focus is losed. Parameter: buffer is protected variable(container) inputTextBuffer.
    virtual void VLostFocuseIGIT(ZC_DA<char>& buffer) {}

private:
    static inline bool isFocused = false;
    static inline ZC_IGInputText* pCallingNow = nullptr,
        * pFocused = nullptr;

    bool userSetCallbackAllways;
    int flags;

    //  ImGui has a static callback, so to call a specific object you need to call VFlagEventCallbackIGIT() via a pointer to the object that is currently being called
    static int FlagCallback(ImGuiInputTextCallbackData *data)
    {
        pFocused = pCallingNow;
        isFocused = true;
        if (data->EventFlag & pCallingNow->flags)
        {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways && !(pCallingNow->userSetCallbackAllways)) return 0;    //  user don't set ImGuiInputTextFlags_CallbackAlways
            pCallingNow->VFlagEventCallbackIGIT(data);
        }
        return 0;
    }
};