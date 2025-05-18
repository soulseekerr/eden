# 🧪 Eden Project

Eden is a modern C++ workflow engine designed for dependency-based task execution, featuring a visual editor built with ImGui and ImNodes. It is modular, extensible, and suitable for financial analytics or general-purpose orchestration pipelines.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Tested with Google Test](https://img.shields.io/badge/tested-Google%20Test-success)


## 📦 Features

✅ Core Engine
- Workflow Execution with task dependencies  
- Multithreaded Scheduler using ThreadPool with std::jthread  
- Task Interface (ITask) with lifecycle tracking  
- Context Injection to provide task-specific configuration  
- Flexible Logging via LoggerManager, with support for console and file output  
- PathManager for robust file handling  
- JSON-based Serialization for workflows and context  


✅ Visual Workflow Editor
- Built with ImGui + ImNodes
- Drag-and-drop graph layout
- Dynamic status coloring (Running, Completed, Failed, etc.)
- Save/load workflow layout with preserved node positions


                        +---------------------+  
                        |     Main Engine     |  
                        +---------------------+  
                                |  
    +---------------------------+----------------------------+  
    |                            |                            |  
+--------+              +---------------+             +--------------+  
| Logger | <--------->  | Thread Pool   |  <--------> | Workflow     |  
+--------+              +---------------+             +--------------+  
     |                          ^                             |  
     v                          |                             v  
Console / File           IExecutor Interface         +----------------+   
                                                 --> | Task Instances |   
                                                 --> |  (ITask impls) |   
                                                     +----------------+   
  

🧑‍💻 Requirements

C++20 compiler (GCC ≥ 11, Clang ≥ 13, MSVC ≥ 2022)  
CMake ≥ 3.20  

Dependencies:  
ImGui  
ImNodes  
nlohmann/json  
fmt  
SDL2 + OpenGL3 (for editor)  


outputs:
    build/Debug/bin
    build/Debug/lib

Build


🚀 Getting Started

git clone https://github.com/yourusername/eden.git
cd eden
mkdir build && cd build
cmake ..
make -j
