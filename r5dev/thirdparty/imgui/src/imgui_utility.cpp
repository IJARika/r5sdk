/*-----------------------------------------------------------------------------
 * _imgui_utility.cpp
 *-----------------------------------------------------------------------------*/

#include "core/stdafx.h"
#include "tier0/commandline.h"
#include "tier0/memstd.h"
#include "filesystem/filesystem.h"
#include "thirdparty/imgui/include/imgui_utility.h"

void ImGuiConfig::Load()
{
    const string svPath = fmt::format("{:s}{:s}", IMGUI_BIND_PATH, IMGUI_BIND_FILE);
    DevMsg(eDLL_T::MS, "Loading ImGui config file '%s'\n", svPath.c_str());

    FileSystem()->CreateDirHierarchy(IMGUI_BIND_PATH, "PLATFORM"); // Create directory, so ImGui can load/save 'layout.ini'.
    FileHandle_t hFile = FileSystem()->Open(svPath.c_str(), "rt", "PLATFORM");

    if (!hFile)
    {
        return;
    }

    uint32_t nLen = FileSystem()->Size(hFile);
    uint8_t* pBuf = MemAllocSingleton()->Alloc<uint8_t>(nLen);

    int nRead = FileSystem()->Read(pBuf, nLen, hFile);
    FileSystem()->Close(hFile);

    pBuf[nRead] = '\0';

    try
    {
        nlohmann::json jsIn = nlohmann::json::parse(pBuf);

        if (!jsIn.is_null() || !jsIn["config"].is_null())
        {
            // IConsole
            m_ConsoleConfig.m_nBind0 = jsIn["config"]["GameConsole"]["bind0"].get<int>();
            m_ConsoleConfig.m_nBind1 = jsIn["config"]["GameConsole"]["bind1"].get<int>();

            // IBrowser
            m_BrowserConfig.m_nBind0 = jsIn["config"]["GameBrowser"]["bind0"].get<int>();
            m_BrowserConfig.m_nBind1 = jsIn["config"]["GameBrowser"]["bind1"].get<int>();
        }
    }
    catch (const std::exception& ex)
    {
        Warning(eDLL_T::MS, "Exception while parsing ImGui config file:\n%s\n", ex.what());
    }

    MemAllocSingleton()->Free(pBuf);
}

void ImGuiConfig::Save()
{
    const string svPath = fmt::format("{:s}{:s}", IMGUI_BIND_PATH, IMGUI_BIND_FILE);
    DevMsg(eDLL_T::MS, "Saving ImGui config file '%s'\n", svPath.c_str());

    FileSystem()->CreateDirHierarchy(IMGUI_BIND_PATH, "PLATFORM");
    FileHandle_t hFile = FileSystem()->Open(svPath.c_str(), "wt", "PLATFORM");

    if (!hFile)
    {
        Error(eDLL_T::MS, NO_ERROR, "%s - Unable to write to '%s' (read-only?)\n", __FUNCTION__, svPath.c_str());
        return;
    }

    nlohmann::json jsOut;

    // IConsole
    jsOut["config"]["GameConsole"]["bind0"] = m_ConsoleConfig.m_nBind0;
    jsOut["config"]["GameConsole"]["bind1"] = m_ConsoleConfig.m_nBind1;

    // IBrowser
    jsOut["config"]["GameBrowser"]["bind0"] = m_BrowserConfig.m_nBind0;
    jsOut["config"]["GameBrowser"]["bind1"] = m_BrowserConfig.m_nBind1;

    const string svOutFile = jsOut.dump(4);

    FileSystem()->Write(svOutFile.data(), svOutFile.size(), hFile);
    FileSystem()->Close(hFile);
}

ImGuiStyle_t ImGuiConfig::InitStyle() const
{
    ImGuiStyle_t result                   = ImGuiStyle_t::NONE;
    ImGuiStyle&  style                    = ImGui::GetStyle();
    ImVec4*      colors                   = style.Colors;

    if (strcmp(CommandLine()->ParmValue("-imgui_theme", ""), "legacy") == 0)
    {
        colors[ImGuiCol_Text]                   = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.12f, 0.37f, 0.75f, 0.50f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.04f, 0.04f, 0.04f, 0.64f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.53f, 0.53f, 0.57f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);

        style.WindowBorderSize  = 0.0f;
        style.FrameBorderSize   = 1.0f;
        style.ChildBorderSize   = 1.0f;
        style.PopupBorderSize   = 1.0f;
        style.TabBorderSize     = 1.0f;

        style.WindowRounding    = 4.0f;
        style.FrameRounding     = 1.0f;
        style.ChildRounding     = 1.0f;
        style.PopupRounding     = 3.0f;
        style.TabRounding       = 1.0f;
        style.ScrollbarRounding = 1.0f;

        result = ImGuiStyle_t::LEGACY;
    }
    else if (strcmp(CommandLine()->ParmValue("-imgui_theme", ""), "modern") == 0)
    {
        colors[ImGuiCol_Text]                   = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.12f, 0.37f, 0.75f, 0.50f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.11f, 0.13f, 0.17f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.61f, 0.61f, 0.61f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.04f, 0.04f, 0.04f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.04f, 0.06f, 0.10f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.04f, 0.07f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.26f, 0.51f, 0.78f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.26f, 0.51f, 0.78f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.13f, 0.17f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.16f, 0.20f, 0.24f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.23f, 0.36f, 0.51f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.30f, 0.46f, 0.65f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.31f, 0.49f, 0.69f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.31f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.41f, 0.56f, 0.57f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.31f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.38f, 0.52f, 0.53f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.41f, 0.56f, 0.57f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.31f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.38f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.41f, 0.56f, 0.57f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.53f, 0.53f, 0.57f, 0.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.81f, 0.81f, 0.81f, 0.50f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.31f, 0.43f, 0.43f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.38f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.41f, 0.56f, 0.57f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.14f, 0.19f, 0.24f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.20f, 0.26f, 0.33f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.22f, 0.29f, 0.37f, 1.00f);

        style.WindowBorderSize  = 1.0f;
        style.FrameBorderSize   = 0.0f;
        style.ChildBorderSize   = 0.0f;
        style.PopupBorderSize   = 1.0f;
        style.TabBorderSize     = 1.0f;

        style.WindowRounding    = 4.0f;
        style.FrameRounding     = 1.0f;
        style.ChildRounding     = 1.0f;
        style.PopupRounding     = 3.0f;
        style.TabRounding       = 1.0f;
        style.ScrollbarRounding = 3.0f;

        result = ImGuiStyle_t::MODERN;
    }
    else
    {
        colors[ImGuiCol_Text]                   = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.12f, 0.37f, 0.75f, 0.50f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.27f, 0.29f, 0.31f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.04f, 0.06f, 0.08f, 1.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.27f, 0.29f, 0.31f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.13f, 0.15f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.21f, 0.23f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.41f, 0.43f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.53f, 0.55f, 0.57f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.63f, 0.65f, 0.67f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.61f, 0.63f, 0.65f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.41f, 0.43f, 0.45f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.53f, 0.55f, 0.57f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.35f, 0.37f, 0.39f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.45f, 0.47f, 0.49f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.52f, 0.54f, 0.56f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.35f, 0.37f, 0.39f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.47f, 0.49f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.55f, 0.57f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.08f, 0.10f, 0.12f, 0.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.53f, 0.55f, 0.57f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.63f, 0.65f, 0.67f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.41f, 0.43f, 0.45f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.52f, 0.54f, 0.56f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.63f, 0.65f, 0.67f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.39f, 0.41f, 0.43f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.39f, 0.41f, 0.43f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.21f, 0.23f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.33f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.25f, 0.27f, 1.00f);

        style.WindowBorderSize  = 1.0f;
        style.FrameBorderSize   = 1.0f;
        style.ChildBorderSize   = 0.0f;
        style.PopupBorderSize   = 1.0f;
        style.TabBorderSize     = 1.0f;

        style.WindowRounding    = 4.0f;
        style.FrameRounding     = 1.0f;
        style.ChildRounding     = 2.0f;
        style.PopupRounding     = 4.0f;
        style.TabRounding       = 1.0f;
        style.GrabRounding      = 1.0f;
        style.ScrollbarRounding = 1.0f;

        result = ImGuiStyle_t::DEFAULT;
    }

    style.ItemSpacing       = ImVec2(5, 4);
    style.FramePadding      = ImVec2(4, 4);
    style.WindowPadding     = ImVec2(5, 5);

    return result;
}

ImGuiConfig* g_pImGuiConfig = new ImGuiConfig();
