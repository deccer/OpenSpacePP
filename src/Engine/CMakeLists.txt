add_library(Engine
    Io.cpp
    Application.cpp
    Device.cpp
    Buffer.cpp
    Pipeline.cpp
    GraphicsPipeline.cpp
    GraphicsPipelineBuilder.cpp
)
set_target_properties(Engine
    PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON)
target_include_directories(Engine PUBLIC Include)
target_link_libraries(Engine PRIVATE glfw glad spdlog debugbreak stb_image)
