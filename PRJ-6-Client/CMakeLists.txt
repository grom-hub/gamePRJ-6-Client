cmake_minimum_required(VERSION 2.8)

project(PRJ-6-Client)
add_executable(${PROJECT_NAME} "main.cpp" "ncScreen.cpp" "connector.cpp" "playerController.cpp")

target_link_libraries(${PROJECT_NAME} ncurses)
