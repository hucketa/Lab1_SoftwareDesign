cmake_minimum_required(VERSION 3.15)
project(lab1)

set(Matplot++_DIR "C:/matplotpp-install/lib/cmake/Matplot++")
find_package(Matplot++ REQUIRED)

# ===== Перше завдання =====
add_executable(task1 task1.cpp)
target_link_libraries(task1 PUBLIC Matplot++::matplot)

# ===== Друге завдання =====
add_executable(task2 task2.cpp)
target_link_libraries(task2 PUBLIC Matplot++::matplot)

# Щоб можна було перезбирати, навіть якщо програма запущена
add_custom_command(TARGET task1 PRE_LINK
    COMMAND cmd /c "taskkill /IM task1.exe /F >nul 2>&1 || exit 0"
)

add_custom_command(TARGET task2 PRE_LINK
    COMMAND cmd /c "taskkill /IM task2.exe /F >nul 2>&1 || exit 0"
)