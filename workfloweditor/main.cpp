#include "imgui.h"
#include "imnodes.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include <string_view>

// TOCHANGE: only for example purposes
enum class TaskStatus { Pending, Running, Done, Failed };

struct TaskNode {
    int id;
    std::string name;
    TaskStatus status;
    int input_id;
    int output_id;
};

struct Link {
    int id;
    int from;
    int to;
};
// TOCHANGE: only for example purposes

enum class NamedColor {
    Blue,
    LightBlue,
    Green,
    LightGreen,
    Grey,
    LightGrey,
    Orange,
    LightOrange,
    Red,
    LightRed,
    Default
};

ImU32 ColorFromName(NamedColor color) {
    switch (color) {
        case NamedColor::Blue:        return IM_COL32(0, 122, 204, 255);
        case NamedColor::LightBlue:   return IM_COL32(173, 216, 230, 255);
        case NamedColor::Green:       return IM_COL32(0, 200, 0, 255);
        case NamedColor::LightGreen:  return IM_COL32(144, 238, 144, 255);
        case NamedColor::Grey:        return IM_COL32(128, 128, 128, 255);
        case NamedColor::LightGrey:   return IM_COL32(200, 200, 200, 255);
        case NamedColor::Orange:       return IM_COL32(255, 140, 0, 255);
        case NamedColor::LightOrange:  return IM_COL32(255, 200, 100, 255);
        case NamedColor::Red:          return IM_COL32(255, 70, 70, 255);
        case NamedColor::LightRed:     return IM_COL32(255, 160, 160, 255);
        default:                      return IM_COL32(255, 255, 255, 255);
    }
}

void ShowWorkflowEditor(std::vector<TaskNode>& nodes, std::vector<Link>& links) {
    ImGui::Begin("Workflow Editor");
    ImNodes::BeginNodeEditor();

    for (const auto& node : nodes) {

        // Optional: style by task status
        switch (node.status) {
            case TaskStatus::Done:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightGreen));
                break;
            case TaskStatus::Running:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightOrange));
                break;
            case TaskStatus::Failed:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightRed));
                break;
            default:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightGrey));
                break;
        }

        ImNodes::BeginNode(node.id);

        // This needs to be called once 
        static bool layoutOnce = true;
        if (layoutOnce) {
            for (const auto& node : nodes) {
                ImNodes::SetNodeEditorSpacePos(node.id, ImVec2(300.0f * node.id, 100.0f + 50.0f * node.id));
            }
            layoutOnce = false;
        }

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(node.input_id);
        ImGui::Text("In");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(node.output_id);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::PopColorStyle();
    }

    for (const auto& link : links) {
        ImNodes::Link(link.id, link.from, link.to);
    }

    ImNodes::EndNodeEditor();
    ImGui::End();
}

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    // Request OpenGL 3.2 Core context (macOS minimum for modern features)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("Workflow Editor", 100, 100, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    // ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    std::vector<TaskNode> nodes = {
        {1, "Load Market Data", TaskStatus::Done, 10, 11},
        {2, "Calibrate Market Data", TaskStatus::Running, 20, 21},
        {3, "Compute PV", TaskStatus::Pending, 30, 31},
        {4, "Save PV", TaskStatus::Pending, 40, 41}
    };
    
    std::vector<Link> links = {
        {1, 11, 20},  // Load Market Data → Calibrate Market Data
        {2, 21, 30},  // Calibrate Market Data → Compute PV
        {3, 31, 40}   // Compute PV → Save PV
    };

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplSDL2_NewFrame(window);
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ShowWorkflowEditor(nodes, links);

        ImGui::Render();
        glViewport(0, 0, 1280, 720);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImNodes::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
