#include "GuiApp.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

#include "ImageLoader.h"
#include "AsciiConverter.h"

// Global state for drop callback
static std::string g_droppedPath = "";
static bool g_fileDropped = false;

void drop_callback(GLFWwindow* window, int count, const char** paths) {
    if (count > 0) {
        g_droppedPath = paths[0];
        g_fileDropped = true;
    }
}

// Helper for File Save Dialog on Windows
std::string GetSaveFileNameWrapper(GLFWwindow* window) {
#ifdef _WIN32
    OPENFILENAMEA ofn;
    char szFile[260] = "ascii_art.txt";
    HWND hwnd = glfwGetWin32Window(window);

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = "txt";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        return std::string(ofn.lpstrFile);
    }
#endif
    return "";
}

void SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    ImVec4* colors = style.Colors;
    // Cyberpunk-ish / Deep Dark Theme
    colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.08f, 0.08f, 0.10f, 1.00f); // Very dark blue/grey
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.12f, 0.95f);
    colors[ImGuiCol_Border]                 = ImVec4(0.25f, 0.25f, 0.30f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.40f, 0.00f, 1.00f); // Orange acccent
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.80f, 0.35f, 0.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.94f, 0.40f, 0.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.80f, 0.35f, 0.00f, 1.00f); // Bright orange active
    colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.40f, 0.40f, 0.44f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.40f, 0.44f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.94f, 0.40f, 0.00f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.94f, 0.40f, 0.00f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.94f, 0.40f, 0.00f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.10f, 0.10f, 0.12f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.94f, 0.40f, 0.00f, 0.30f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.94f, 0.40f, 0.00f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.94f, 0.40f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void GuiApp::run() {
    if (!glfwInit()) return;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 800, "ASCII Art Converter Pro", NULL, NULL);
    if (window == NULL) return;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

    SetupStyle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwSetDropCallback(window, drop_callback);

    // App State
    std::string asciiResult = "Drag an image file here to convert...";
    int targetWidth = 100;
    bool complexCharset = false;
    bool invertColors = false; 
    bool fillBackground = true; 
    int selectedPlatform = 0; // 0: Custom, 1: Discord, 2: Telegram, 3: Console
    const char* platforms[] = { "Custom", "Discord (<2000 chars)", "Telegram", "Console (Standard)" };

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Auto-Conversion on Drop or Setting Change
        static int lastWidth = targetWidth;
        static bool lastComplex = complexCharset;
        static bool lastInvert = invertColors;
        static int lastPlatform = selectedPlatform;
        
        bool settingsChanged = (lastWidth != targetWidth) || (lastComplex != complexCharset) || (lastInvert != invertColors) || (lastPlatform != selectedPlatform);

        // Platform Logic & Auto-Sizing
        if (selectedPlatform == 1) { // Discord
            // Smart Resize for Discord
            // Only recalculate if we have a file and something relevant caused a trigger
            if ((g_fileDropped || settingsChanged) && !g_droppedPath.empty()) { 
                ImageData img = ImageLoader::load(g_droppedPath);
                if (!img.pixels.empty()) {
                   float aspectRatio = (float)img.height / (float)img.width;
                   // Brute force best width from 10 to 58 (safe limit)
                   int bestW = 10;
                   for (int w = 10; w <= 58; w++) {
                       int h = (int)(w * aspectRatio * 0.5f);
                       if (h < 1) h = 1;
                       int totalChars = w * h + h; // +h for newlines
                       if (totalChars < 1900) { // Safety margin < 2000
                           bestW = w;
                       } else {
                           break;
                       }
                   }
                   targetWidth = bestW;
                   // Update settingsChanged to true effectively because we just changed targetWidth? 
                   // Or just let the flow continue.
                }
            }
        }
        
        // Update statics for next frame
        lastWidth = targetWidth; lastComplex = complexCharset; lastInvert = invertColors; lastPlatform = selectedPlatform;

        if ((g_fileDropped || settingsChanged) && !g_droppedPath.empty()) {
            g_fileDropped = false;
            ImageData img = ImageLoader::load(g_droppedPath);
            if (!img.pixels.empty()) {
                float aspectRatio = (float)img.height / (float)img.width;
                
                // Font adjustment 0.5 is standard
                int targetHeight = (int)(targetWidth * aspectRatio * 0.5f);
                if (targetHeight < 1) targetHeight = 1;

                ImageData resized = ImageLoader::resize(img, targetWidth, targetHeight);
                AsciiOptions opts;
                opts.color = false; 
                opts.invert = invertColors;

                if (fillBackground) {
                     if (complexCharset) {
                         opts.customCharset = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
                     } else {
                         opts.customCharset = " .:-=+*#%@";
                     }
                } else {
                    opts.customCharset = complexCharset ? "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. " : " .:-=+*#%@";
                }
                
                asciiResult = AsciiConverter::convert(resized, opts);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        // --- SIDEBAR ---
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoCollapse);
        
        ImGui::TextColored(ImVec4(0.9f, 0.4f, 0.0f, 1.0f), "ASCII CONVERTER PRO");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Platform Preset:");
        if (ImGui::Combo("##platform", &selectedPlatform, platforms, IM_ARRAYSIZE(platforms))) {
             if (!g_droppedPath.empty()) g_fileDropped = true; // Trigger update
        }

        ImGui::Spacing();
        ImGui::Text("Resolution (Width):");
        if (ImGui::SliderInt("##width", &targetWidth, 10, 300)) {
            selectedPlatform = 0; // Switch to custom if modified manually
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Style:");
        ImGui::Checkbox("High Detail (Complex Charset)", &complexCharset);
        ImGui::Checkbox("Invert Colors", &invertColors);
        
        ImGui::Spacing();
        ImGui::Separator();
        
        // Save Actions
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.4f, 0.0f, 1.0f));
        if (ImGui::Button("SAVE TO FILE", ImVec2(-1, 40))) {
            std::string savePath = GetSaveFileNameWrapper(window);
            if (!savePath.empty()) {
                std::ofstream out(savePath);
                out << asciiResult;
                out.close();
            }
        }
        ImGui::PopStyleColor();

        ImGui::Spacing();
        if (ImGui::Button("Copy to Clipboard", ImVec2(-1, 30))) {
            std::string textToCopy = asciiResult;
            if (selectedPlatform == 1) { // Discord: Wrap in code block
                textToCopy = "```\n" + textToCopy + "\n```";
            }
            ImGui::SetClipboardText(textToCopy.c_str());
        }

        ImGui::End(); // Controls

        // --- PREVIEW ---
        ImGui::Begin("Preview", nullptr);
        if (g_droppedPath.empty()) {
             // Draw a visual drag box
             ImVec2 avail = ImGui::GetContentRegionAvail();
             ImVec2 center = ImGui::GetCursorScreenPos();
             center.x += avail.x * 0.5f;
             center.y += avail.y * 0.5f;
             
             ImGui::GetWindowDrawList()->AddRect(
                 ImGui::GetCursorScreenPos(), 
                 ImVec2(ImGui::GetCursorScreenPos().x + avail.x, ImGui::GetCursorScreenPos().y + avail.y), 
                 IM_COL32(50, 50, 60, 255), 5.0f, 0, 5.0f 
             );

             ImGui::SetCursorPos(ImVec2(avail.x * 0.5f - 80, avail.y * 0.5f - 10));
             ImGui::Text("DROP IMAGE HERE");
        } else {
             ImGui::TextDisabled("%s", g_droppedPath.c_str());
             ImGui::Separator();
             ImGui::BeginChild("AsciiView", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
             // Use a monospace font logic or just default
             ImGui::TextUnformatted(asciiResult.c_str());
             ImGui::EndChild();
        }
        ImGui::End(); // Preview

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.08f, 0.08f, 0.10f, 1.00f); // Match window bg
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
