cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_FLAGS "-std=c++11")

project(vtkdemo)

# Путь к Gmsh SDK
set(GMSH_SDK_PATH "/home/ray/gmsh-sdk")

# Проверка существования Gmsh
if(NOT EXISTS ${GMSH_SDK_PATH}/lib/libgmsh.so)
    message(FATAL_ERROR "Gmsh library not found at ${GMSH_SDK_PATH}/lib/libgmsh.so")
endif()
if(NOT EXISTS ${GMSH_SDK_PATH}/include/gmsh.h)
    message(FATAL_ERROR "Gmsh header not found at ${GMSH_SDK_PATH}/include/gmsh.h")
endif()

message(STATUS "Found Gmsh at: ${GMSH_SDK_PATH}")

add_executable(cubic2d cubic2d.cpp)
add_executable(cubic3d cubic3d.cpp)
add_executable(tetr3d tetr3d.cpp)
add_executable(dragon dragon.cpp)
add_executable(dragon_with_wings dragon_with_wings.cpp)

# --- Подключаем gmsh для третьего примера
add_library(gmsh SHARED IMPORTED)
set_target_properties(gmsh PROPERTIES
    IMPORTED_LOCATION "${GMSH_SDK_PATH}/lib/libgmsh.so"
    INTERFACE_INCLUDE_DIRECTORIES "${GMSH_SDK_PATH}/include/"
)
set(GMSH_LIBRARIES gmsh)

# --- Подключаем VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Укажем библиотеки для линковки
target_link_libraries(cubic2d ${VTK_LIBRARIES})
target_link_libraries(cubic3d ${VTK_LIBRARIES})
target_link_libraries(tetr3d ${VTK_LIBRARIES} ${GMSH_LIBRARIES})
target_link_libraries(dragon ${VTK_LIBRARIES} ${GMSH_LIBRARIES})
target_link_libraries(dragon_with_wings ${VTK_LIBRARIES} ${GMSH_LIBRARIES})
