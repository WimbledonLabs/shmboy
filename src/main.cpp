// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_sdl.h"
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include <sys/time.h>
#include "shmboy_common.h"
#include "shm_emu_io.h"
#include "cpu.h"
#include "cart.h"

#include "opcodeDisassembleSwitch.c"

typedef enum {
    FREE_RUN,
    STEPPING
} EmulationState;

// Video Context
SDL_Window *window;
SDL_GLContext glcontext;
GLuint logoTextureId;
SDL_Surface* logo;

// Misc drawing settings
bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImColor(114, 144, 154);
int scale = 2;

// Emulator data
Cpu *cpu;
Cartridge *cart;
EmulationState state = STEPPING;
bool doStep = false;

bool initVideo() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window = SDL_CreateWindow("Shmboy", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 720,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    glcontext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    ImGui_ImplSdl_Init(window);

    //Screen lcd(255, 255);
    logo = SDL_LoadBMP("logo.bmp");
    if (logo == nullptr) {
        std::cout << "Could not load logo :(" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        exit(-1);
    }

    logoTextureId = 0;

    glGenTextures(1, &logoTextureId);
    glBindTexture(GL_TEXTURE_2D, logoTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 logo->w, logo->h,0, 
                 GL_RGB, GL_UNSIGNED_BYTE, 
                 logo->pixels);
}

void cleanupVideo() {
    SDL_FreeSurface(logo);
    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void frameStart() {
}

void frameEnd() {
    // Rendering
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x,
               (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    SDL_GL_SwapWindow(window);
}

void createDisassemblyWindow() {
    // Show disassembly window
    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

    //ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),
    //                                    ImVec2(-1, FLT_MAX));
    //                                    // Vertical only

    char buf[64] = {0};
    OpCode op;
    op.value = 0;

    ImGui::SetNextWindowSize(ImVec2(200,300),
                             ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Disassembly", nullptr, window_flags);
    ImGui::BeginChild("item view",
            // Leave room for 1 line below us
            ImVec2(0, -(1.0*ImGui::GetItemsLineHeightWithSpacing())));

    int start = cpu->pc - 0x10;
    if (start < 0)
        start = 0;
    for (int i=start; i < cpu->pc + 0x10; i++) {
        if (i==cpu->pc) {
            ImGui::TreeNodeEx("",
                    ImGuiTreeNodeFlags_CollapsingHeader |
                    ImGuiTreeNodeFlags_NoTreePushOnOpen);
            ImGui::SameLine();
        }
        ImGui::Text("0x%04X", i);
        u8 *data = cart->fetch(i);
        ImGui::SameLine(); ImGui::Text("%02X", *data);
        ImGui::SameLine(); ImGui::Text("%c", *data);
        op.value = *data;
        disassembleOpCode(buf, op, *(data+1), *(u16 *)(data+1));
        ImGui::SameLine(); ImGui::Text(buf);
    }

    ImGui::EndChild();

    ImGui::Separator();

    if (ImGui::Button("Step")) {
        doStep = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        cpu->reset();
    }
    ImGui::SameLine();

    static char input_buffer[64] = "";
    ImGui::InputText("", input_buffer, 64);

    ImGui::End();
}

void createEmulatorWindow() {
    // Show emulator screen texture
    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f);

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

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void createCpuStatusWindow() {
    ImGui::Begin("CPU Status", nullptr);
    ImGui::Text("AF: 0x%02X 0x%02X", cpu->reg[REG_A], cpu->reg[REG_F]);
    ImGui::Text("BC: 0x%02X 0x%02X", cpu->reg[REG_B], cpu->reg[REG_C]);
    ImGui::Text("DE: 0x%02X 0x%02X", cpu->reg[REG_D], cpu->reg[REG_E]);
    ImGui::Text("HL: 0x%02X 0x%02X", cpu->reg[REG_H], cpu->reg[REG_L]);
    ImGui::Text("SP: 0x%04X", cpu->sp);
    ImGui::Text("PC: 0x%04X", cpu->pc);
    ImGui::Text(" ");
    ImGui::Text("imm16: 8F 04 (Fake)");

    ImGui::End();
}

void createApplicationSettingsWindow() {
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears
    // in a window automatically called "Debug"
    static float f = 0.0f;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Text("Game Boy Screen Scale");
    ImGui::RadioButton("x1", &scale, 1); ImGui::SameLine();
    ImGui::RadioButton("x2", &scale, 2); ImGui::SameLine();
    ImGui::RadioButton("x3", &scale, 3); ImGui::SameLine();
    ImGui::RadioButton("x4", &scale, 4);

    if (ImGui::TreeNode("ROM Details")) {
        ImGui::Columns(2, "mixed");
        ImGui::Separator();

        ImGui::Text("ROM NAME");
        ImGui::NextColumn();
        ImGui::Text("%.16s", cart->getTitle());
        ImGui::NextColumn();

        ImGui::Text("Cart Type"); 
        ImGui::NextColumn();
        ImGui::Text(cart->getCartridgeTypeName());
        ImGui::NextColumn();

        ImGui::Text("Mode");
        ImGui::NextColumn();
        ImGui::Text(cart->isColorMode() ? "CGB" : "DMG");
        ImGui::NextColumn();

        ImGui::Text("Region");
        ImGui::NextColumn();
        ImGui::Text("%u", cart->getRegionCode());
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::TreePop();
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cout << "Must specify path to ROM" << std::endl;
        exit(1);
    }

    std::ifstream romFile(argv[1], std::ifstream::in);

    if (!romFile.is_open()) {
        std::cout << "Error: Opening ROM failed" << std::endl;
        exit(1);
    }

    // LOAD ROM HERE
    cart = new Cartridge(&romFile);
    cpu = new Cpu();
    cpu->loadCart(cart);

    initVideo();

    // Main loop
    bool done = false;
    while (!done)
    {
        while ( (state == STEPPING && doStep) || state == FREE_RUN ) {
            cpu->step();
            doStep = false;
        }

        frameStart();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdl_NewFrame(window);

        createApplicationSettingsWindow();
        createDisassemblyWindow();
        createEmulatorWindow();
        createCpuStatusWindow();

        // Show the ImGui test window. Most of the sample code is in
        // ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        frameEnd();
    }

    cleanupVideo();

    delete cpu;
    cpu = nullptr;
    delete cart;
    cart = nullptr;
    return 0;
}
