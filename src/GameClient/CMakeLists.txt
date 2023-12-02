add_custom_target(CopyData
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CURRENT_BINARY_DIR}/Data
)

add_executable(GameClient
    GameApplication.cpp
    Main.cpp
)
add_dependencies(GameClient CopyData)

if (MSVC)
    target_compile_options(GameClient PRIVATE /W3 /WX)
else()
    target_compile_options(GameClient PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(GameClient PRIVATE Include)
target_link_libraries(GameClient PRIVATE Engine glad glfw glm spdlog)

target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)
