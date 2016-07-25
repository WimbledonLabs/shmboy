// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "shm_emu_io.h"

int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *window = SDL_CreateWindow("Shmboy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    ImGui_ImplSdl_Init(window);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    //Screen lcd(255, 255);
    SDL_Surface* logo = SDL_LoadBMP("logo.bmp");
    if (logo == nullptr) {
        std::cout << "Could not load logo :(" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        exit(-1);
    }

    GLuint logoTextureId = 0;

    glGenTextures(1, &logoTextureId);
    glBindTexture(GL_TEXTURE_2D, logoTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 logo->w, logo->h,0, 
                 GL_RGB, GL_UNSIGNED_BYTE, 
                 logo->pixels);

    int scale = 2;

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdl_NewFrame(window);

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Game Boy Screen Scale");
            ImGui::RadioButton("x1", &scale, 1); ImGui::SameLine();
            ImGui::RadioButton("x2", &scale, 2); ImGui::SameLine();
            ImGui::RadioButton("x3", &scale, 3); ImGui::SameLine();
            ImGui::RadioButton("x4", &scale, 4);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
        }

        {
            ImGui::SetNextWindowSize(ImVec2(450,300), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Disassembly", nullptr);
            ImGui::Columns(3, "mycolumns3", true); // 3 columns, no border
            ImGui::Separator();
            ImGui::Text("One"); ImGui::NextColumn();
            ImGui::Text("2"); ImGui::NextColumn();
            ImGui::Text("3"); ImGui::NextColumn();
            ImGui::Text("4"); ImGui::NextColumn();
            ImGui::Text("5"); ImGui::NextColumn();
            ImGui::Text("6"); ImGui::NextColumn();
            ImGui::Text("7"); ImGui::NextColumn();
            ImGui::Text("8"); ImGui::NextColumn();
            ImGui::Text("9"); ImGui::NextColumn();
            ImGui::Text("10"); ImGui::NextColumn();
            ImGui::Text("11"); ImGui::NextColumn();
            ImGui::Text("12"); ImGui::NextColumn();
            ImGui::Text("13"); ImGui::NextColumn();
            ImGui::Text("14"); ImGui::NextColumn();
            ImGui::Text("15"); ImGui::NextColumn();
            ImGui::Text("16"); ImGui::NextColumn();
            ImGui::Text("17"); ImGui::NextColumn();
            ImGui::Text("18"); ImGui::NextColumn();
            ImGui::End();
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        {
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoScrollbar;
            window_flags |= ImGuiWindowFlags_NoCollapse;
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
            ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("SHMBoy", &show_another_window, window_flags);
            ImGui::Image( (void *) logoTextureId, ImVec2(160*scale, 144*scale), ImVec2(0,0), ImVec2(1,1),
                         ImColor(255,255,255,255), ImColor(255,255,255,128));
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_FreeSurface(logo);
    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
