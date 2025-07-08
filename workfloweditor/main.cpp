#include "imgui.h"
#include "imnodes.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>

#include "workflow/workflow.h"
#include "workflow/workflowserializer.h"
#include "task/fetchdatatask.h"
#include "pathmanager.h"
#include "datetime.h"
#include "attributes.h"
#include "context.h"
#include "logger.h"
#include "exceptions.h"

using namespace eden;

static bool running = true;

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

// nlohmann::json saveNodeLayout(const WorkflowUPtr& wf) {
//     nlohmann::json j;
//     for (auto& t : wf->tasks()) {
//         ImVec2 pos = ImNodes::GetNodeEditorSpacePos(t.first);
//         j["nodes"].push_back({
//             {"id", t.first},
//             {"x", pos.x},
//             {"y", pos.y}
//         });
//     }
//     return j;
// }

// void loadNodeLayout(const nlohmann::json& j) {
//     if (!j.contains("nodes")) return;
//     for (const auto& node : j["nodes"]) {
//         int id = node["id"];
//         float x = node["x"];
//         float y = node["y"];
//         ImNodes::SetNodeEditorSpacePos(id, ImVec2(x, y));
//     }
// }

// Optional: Load from file
// void loadLayoutFromFile(const std::string& filename) {
//     std::ifstream file(filename);
//     if (!file) return;
//     nlohmann::json j;
//     file >> j;
//     loadNodeLayout(j);
// }

WorkflowUPtr loadWorkflow(const std::string& jsonFile) {
    auto& log = LoggerManager::getInstance();
    log.addLogger(std::make_unique<LoggerConsole>(LOG_LEVEL::LOG_DEBUG));
    
    auto& pathManager = PathManager::getInstance();

    // Inputs of the binary - harcoded here
    auto cob = DateTime(2024, 6, 3);
    // Attributes cannot be modified further but copied among other task instances
    const AttributeSPtr& attr = std::make_shared<Attributes>(cob);
    std::string strCob = attr->cob().yearMonthDayHyphen();
    log.log("COB: {}", strCob);

    ContextSPtr wfContext;
    WorkflowUPtr wf;

    auto getJsonFile = [](PathManager& pm, const EdenFileType& fileType, const std::string& filename) -> std::string {
        auto path = pm.getFilePath(fileType, filename);
        if (!path) {
            std::string label;
            switch (fileType) {
                case EdenFileType::Workflow: label = "Workflow"; break;
                case EdenFileType::Config: label = "Config"; break;
                default: label = "Unknown"; break;
            }
            throw MissingFileException(label, filename);
        }
        return path->string();
    };


    try {
        const auto workflowFile = getJsonFile(pathManager, EdenFileType::Workflow, jsonFile);
        log.logInfo("Workflow file: {}", workflowFile);
    
        const auto contextFile = getJsonFile(pathManager, EdenFileType::Workflow, "context_test.json");
        log.logInfo("Context file: {}", contextFile);

        wfContext = std::make_shared<JsonContext>(contextFile);

        log.logInfo("Context  successfully created");

        wf = std::make_unique<Workflow>("MyFlow", attr, wfContext);
        WorkflowSerializer wfSerializer;
        wfSerializer.load(wf, workflowFile);

    } catch (const MissingFileException& e) {
        log.logError("Missing file: {}", e.what());
        exit(-1);
    }

    log.log("Workflow successfully created");
    log.log("Workflow name: {} - COB: {}", wf->name(), strCob);

    return wf;
}

// Function to create the menu
void ShowMenuBar(WorkflowUPtr& workflow) {
    auto& log = LoggerManager::getInstance();
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Layout")) {
                log.log("Load Layout button clicked");
                IGFD::FileDialogConfig config;config.path = ".";
		        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyLoad", "Choose File", ".json", config);
            }
            if (ImGui::MenuItem("Save Layout As...")) {
                // Open the Save As dialog when clicked
                // ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Save As", ".json", ".");
                log.log("Save Layout button clicked");
                IGFD::FileDialogConfig config;config.path = ".";
		        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeySave", "Choose File", ".json", config);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) {
                // Set running to false to exit the loop and close the app
                running = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyLoad")) {
        log.log("File dialog displayed");
    
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            log.log("Selected file: {}", filePathName);
            workflow = loadWorkflow(filePathName);
        }
    
        // close
        ImGuiFileDialog::Instance()->Close();
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeySave")) {
        log.log("File dialog displayed");
    
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            log.log("Selected file: {}", filePathName);
            WorkflowSerializer serializer;
            serializer.save(workflow, filePathName);
            log.log("Workflow saved to file: {}", filePathName);
        }
    
        ImGuiFileDialog::Instance()->Close();
    }
}

void ShowWorkflowEditor(WorkflowUPtr& workflow) {
    ImGui::Begin("Workflow Editor");

    ImNodes::BeginNodeEditor();

    auto& tasks = workflow->tasks();
    for (const auto& [id, task] : tasks) {
        // std::cout << "Task ID: " << id << ", Name: " << task->name() << "\n";

        switch (task->status) {
            case ITask::Status::Completed:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightGreen));
                break;
            case ITask::Status::Running:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightOrange));
                break;
            case ITask::Status::Failed:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightRed));
                break;
            default:
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, ColorFromName(NamedColor::LightGrey));
                break;
        }

        ImNodes::BeginNode(task->ID());

        // This needs to be called once 
        static bool layoutOnce = true;
        if (layoutOnce) {
            for (const auto& [id, task] : tasks) {
                ImNodes::SetNodeEditorSpacePos(id, ImVec2(250.0f * id, 100.0f + 50.0f * id));
            }
            layoutOnce = false;
        }

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(std::format("ID {} {}", task->ID(), task->name().c_str()).c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(task->inputID());
        ImGui::Text("In");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(task->outputID());
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::PopColorStyle();
    }

    auto& links = workflow->links();
    // for (const auto& link : links) {
    for (const auto& [taskId, pairs] : links) {
        for (const auto& [from, to] : pairs) {
            ImNodes::Link(taskId, from, to);
        }
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
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    auto workflow = loadWorkflow("workflow_test.json");

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

        // Show the menu bar
        ShowMenuBar(workflow);

        ShowWorkflowEditor(workflow);

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
