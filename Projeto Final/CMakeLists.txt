cmake_minimum_required(VERSION 3.10)
project(Visualizador3D)

set(CMAKE_CXX_STANDARD 17)

# Usar o toolchain do vcpkg para achar libs instaladas
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Adiciona os arquivos do projeto
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Cria o executável
add_executable(Visualizador3D ${SRC_FILES})

# Encontra e linka as dependências
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(Visualizador3D PRIVATE glfw glad::glad glm::glm-header-only)

