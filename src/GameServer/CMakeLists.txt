add_executable(GameServer
    Main.cpp
)

if (MSVC)
    target_compile_options(GameServer PRIVATE /W3 /WX)
else()
    target_compile_options(GameServer PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(GameServer PRIVATE Include)
target_link_libraries(GameServer PRIVATE Engine glad glfw glm spdlog)

target_include_directories(spdlog PUBLIC include)
