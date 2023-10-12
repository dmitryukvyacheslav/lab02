// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl2_opengl3/ folder**
// See imgui_impl_sdl2.cpp for details.

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <streambuf>
#include <istream>
#include "rb.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <chrono>

struct membuf : std::streambuf {
    membuf(char const* base, size_t size) {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }
};

struct imemstream : virtual membuf, std::istream {
    imemstream(char const* base, size_t size)
        : membuf(base, size)
        , std::istream(static_cast<std::streambuf*>(this)) {
    }
};

bool LoadTextureFromBmpBuffer(const char* buffer, int len, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    short image_bpp = 0;
    unsigned char* image_data;

    imemstream in(buffer, len);
    char info[54];
    in.read(info, 54);
    image_width = *(int*)&info[18];
    image_height = *(int*)&info[22];
    image_bpp = *(short*)&info[28];

    printf("image_bpp: %d\n", image_bpp);
    // allocate image 3 bytes per pixel
    int size = 3 * image_width * image_height;
    image_data = new (std::nothrow) unsigned char[size];
    if (!image_data)
        return false;
    in.read((char*)image_data, size);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  //Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_BGR, GL_UNSIGNED_BYTE, image_data);
    //stbi_image_free(image_data);
    delete image_data;
    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Data Structure Looking Glass", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    RedBlackTree* t = new RedBlackTree();
    GLuint tex = 0;
    int tex_w = 0;
    int tex_h = 0;
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        static float zoom_factor = 1;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_MOUSEWHEEL)
                event.wheel.y > 0 ? zoom_factor += 0.25 : zoom_factor += -0.25;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static ImVec2 oldpos;
            static int key = 1;
            static int minmaxval = 0;
            ImGuiWindowFlags flags = 0 + ImGuiWindowFlags_NoCollapse;

            

            ImGui::Begin("dmt entertainment software ltd.", nullptr, flags);
            if (ImGui::BeginPopup("NodeNotFound")) {
                ImGui::Text("Node not found.");
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("NodeFound")) {
                ImGui::Text("Node exists.");
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("MinKey")) {
                ImGui::Text("Min key is %d.", minmaxval);
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("MaxKey")) {
                ImGui::Text("Max key is %d.", minmaxval);
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("EmptyTree")) {
                ImGui::Text("Tree is empty.");
                if (ImGui::Button("OK"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            const char* items[] = { "Red-black tree" };
            static int item_current = 0;

            ImGui::Combo("Type", &item_current, items, IM_ARRAYSIZE(items), 5);
            if (ImGui::InputInt("Key", &key, 1, 5)) {
                if (key < 0)
                    key = 0;
            }
            if (ImGui::Button("Add")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
                if (t->lookup(key) == nullptr) {
                    t->add(key);
                    void *ptr = t->render_dfs();
                    LoadTextureFromBmpBuffer((const char*)ptr, t->m_rd_len, &tex, &tex_w, &tex_h);
                    t->free_renderdata();
                }
                else
                    ImGui::OpenPopup("NodeFound");
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove")) {                           // Buttons return true when clicked (most widgets return true when edited/activated)
                if (t->lookup(key) != nullptr) {
                    t->remove(key);
                    void* ptr = t->render_dfs();
                    LoadTextureFromBmpBuffer((const char*)ptr, t->m_rd_len, &tex, &tex_w, &tex_h);
                    t->free_renderdata();
                }
                else
                    ImGui::OpenPopup("NodeNotFound");
            }
            ImGui::SameLine();
            if (ImGui::Button("Lookup")) {                        // Buttons return true when clicked (most widgets return true when edited/activated)
                if (!(t->lookup(key)))
                    ImGui::OpenPopup("NodeNotFound");
                else
                    ImGui::OpenPopup("NodeFound");
            }
            ImGui::SameLine();
            if (ImGui::Button("Min")) {                         // Buttons return true when clicked (most widgets return true when edited/activated)
                if (t->min() == nullptr) {
                    ImGui::OpenPopup("EmptyTree");
                }
                else {
                    minmaxval = t->min()->key;
                    ImGui::OpenPopup("MinKey");
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Max")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
                if (t->max() == nullptr) {
                    ImGui::OpenPopup("EmptyTree");
                }
                else {
                    minmaxval = t->max()->key;
                    ImGui::OpenPopup("MaxKey");
                }
            }
            if (ImGui::Button("New tree")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
                //t->~RedBlackTree();
                delete t;
                t = new RedBlackTree();
                t->add(key);

                void* ptr = t->render_dfs();
                LoadTextureFromBmpBuffer((const char*)ptr, t->m_rd_len, &tex, &tex_w, &tex_h);
                t->free_renderdata();
            }
            ImGui::SeparatorText("Quick trees");
            if (ImGui::Button("14 Tree")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
                delete t;
                t = new RedBlackTree();
                t->add(14);
                t->add(4);
                t->add(18);
                t->add(3);
                t->add(8);
                t->add(21);
                t->add(6);
                t->add(11);
                void* ptr = t->render_dfs();
                LoadTextureFromBmpBuffer((const char*)ptr, t->m_rd_len, &tex, &tex_w, &tex_h);
                t->free_renderdata();
            }
            ImGui::End();
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Set window background to white
            //ImGui::PushStyleColor(ImGuiCol_Separator, { 1.0f, 1.0f, 1.0f, 1.0f });
            ImGui::SetNextWindowPos({ 10, 10 });
            ImGui::Begin("Tree view", nullptr, ImGuiWindowFlags_NoCollapse );
            ImGui::BeginChild(
                "panning_window",
                ImVec2(0.f, 0.f),   
                true,
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::PopStyleColor();
            static ImVec2 pan_offset = { 0,0 };
            ImVec2 pos = ImGui::GetMousePos();
            ImVec2 wpos = ImGui::GetWindowPos();
            ImVec2 wsize = ImGui::GetWindowSize();
            ImVec2 mdelta = { pos.x - oldpos.x, pos.y - oldpos.y };

            oldpos = pos;
    
            if (ImGui::IsWindowFocused()) {
                

                // todo: bad code, find something that would yield me all pressed keys
                if (ImGui::IsKeyDown(ImGuiKey_MouseLeft)) {
                    pan_offset.x += mdelta.x;
                    pan_offset.y += mdelta.y;
                }
                if (ImGui::IsKeyPressed(ImGuiKey_C)) 
                    pan_offset = { 0, 0 };
               
            }
            
            ImGui::Text("panning: (%.0f;%.0f)", pan_offset.x, pan_offset.y);
            ImGui::Text("zoom: %1.2f", zoom_factor);
            
            ImVec2 imagepos = ImVec2((wpos.x + wsize.x/2 + pan_offset.x), (wpos.y + wsize.y/2 + pan_offset.y));
           
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            if (tex) {
                draw_list->AddImage((ImTextureID)tex, { (imagepos.x - tex_w * zoom_factor / 2), (imagepos.y - tex_h * zoom_factor / 2) }, { (imagepos.x + tex_w * zoom_factor / 2) , (imagepos.y + tex_h * zoom_factor / 2) }, { 0, 1 }, { 1, 0 });
            }
            //ImGui::PopStyleColor();
            ImGui::EndChild();
            ImGui::End();

            //  ImGui::ShowDemoWindow();
        }
        // Rendering
        ImGui::Render();
        
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
