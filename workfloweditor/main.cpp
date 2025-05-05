#include "imgui.h"
#include "imnodes.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include <string_view>

#include "workflow/workflow.h"
#include "task/fetchdatatask.h"
#include "pathmanager.h"
#include "datetime.h"
#include "attributes.h"
#include "context.h"
#include "logger.h"
#include "exceptions.h"

using namespace eden;

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

std::string getJsonFile(PathManager& pm, const EdenFileType& fileType, const std::string& filename) {
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
}

WorkflowUPtr loadWorkflow() {
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

    try {
        const auto workflowFile = getJsonFile(pathManager, EdenFileType::Workflow, "workflow_test.json");
        log.logInfo("Workflow file: {}", workflowFile);
    
        const auto contextFile = getJsonFile(pathManager, EdenFileType::Workflow, "context_test.json");
        log.logInfo("Context file: {}", contextFile);

        wfContext = std::make_shared<JsonContext>(contextFile);

        log.logInfo("Context  successfully created");

        wf = std::make_unique<Workflow>("MyFlow", attr, wfContext);
        wf->loadJsonFile(workflowFile);

    } catch (const MissingFileException& e) {
        log.logError("Missing file: {}", e.what());
        exit(-1);
    }

    log.log("Workflow successfully created");
    log.log("Workflow name: {} - COB: {}", wf->name(), strCob);

    return wf;
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

void ShowWorkflowEditorExample(std::vector<TaskNode>& nodes, std::vector<Link>& links) {
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

    auto workflow = loadWorkflow();

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

        // ShowWorkflowEditorExample(nodes, links);
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
