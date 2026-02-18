set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(gmshdemo)

# --- ПУТЬ К GMSH SDK (ВАШ!) ---
set(GMSH_SDK "/home/ray/gmsh-sdk")

# --- ЯВНО УКАЗЫВАЕМ ПУТИ ---
include_directories(${GMSH_SDK}/include)
link_directories(${GMSH_SDK}/lib)

# --- ИСПОЛНЯЕМЫЕ ФАЙЛЫ ---
add_executable(sample01 sample01.cpp)
add_executable(sample02 sample02.cpp)
add_executable(sample03 sample03.cpp)
add_executable(sample04 sample04.cpp)
add_executable(cube cube.cpp)
add_executable(circle circle.cpp)
#add_executable(cyl cyl.cpp)
add_executable(torus torus.cpp)
add_executable(dragon dragon.cpp)

# --- ЯВНАЯ ЛИНКОВКА ---
target_link_libraries(sample01 ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(sample02 ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(sample03 ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(sample04 ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(cube ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(circle ${GMSH_SDK}/lib/libgmsh.so)
#target_link_libraries(cyl ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(torus ${GMSH_SDK}/lib/libgmsh.so)
target_link_libraries(dragon ${GMSH_SDK}/lib/libgmsh.so)
