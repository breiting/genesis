cmake_minimum_required(VERSION 4.1.1)
project(genesis LANGUAGES CXX)

# Produce json file for neovim clangd completion
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Load compiler settings target
include(cmake/CompilerSettings.cmake)

# Load embed text functionality and create assets folder
include(cmake/EmbedText.cmake)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated/assets")

# -------- Dependencies (fetch once, available everywhere) --------
include(FetchContent)

# ----------------------------------------------------------
# GLM
# ----------------------------------------------------------
FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# ----------------------------------------------------------
# GLFW
# ----------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# ----------------------------------------------------------
# IMGUI
# ----------------------------------------------------------
FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG v1.92.3
)

FetchContent_MakeAvailable(imgui)

# ----------------------------------------------------------
# GLAD
# ----------------------------------------------------------
add_library(glad STATIC extern/glad/src/gl.cpp)
target_include_directories(glad PUBLIC extern/glad/include)


# ----------------------------------------------------------
# TBB
# ----------------------------------------------------------
set(TBB_TEST OFF CACHE BOOL "Disable TBB tests")
set(TBB_BUILD_TESTS OFF CACHE BOOL "Disable TBB tests")
FetchContent_Declare(
		TBB
		GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
		GIT_TAG        v2022.2.0
)

FetchContent_MakeAvailable(TBB)

# ----------------------------------------------------------
# Google Test
# ----------------------------------------------------------
FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# ----------------------------------------------------------
# Start with BUILD DEFINITION
# ----------------------------------------------------------

# IMGUI
set(IMGUI_SRC
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
				${imgui_SOURCE_DIR}
				${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

# Common include dir
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Lib: genesis
add_library(genesis
	src/core/Logger.cpp
	src/core/Runner.cpp
	src/core/Time.cpp
	src/core/Window.cpp

	src/model/Agent.cpp
	src/model/Capability.cpp
	src/model/FitnessTracker.cpp
	src/model/Genome.cpp
	src/model/MovementBehavior.cpp
	src/model/MovementEvaluator.cpp
	src/model/NeuralNetwork.cpp
	src/model/Trainer.cpp

	src/modules/BrownianSim.cpp
	src/modules/NeuroEvolutionSim.cpp

	src/view/AgentView.cpp
	src/view/Camera2D.cpp
	src/view/CanvasView.cpp
	src/view/Shader.cpp
	src/view/TargetView.cpp

	src/ui/ImGuiLayer.cpp
)


# -------------------------
# Shaders (embed to headers)
# -------------------------
set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")
set(GEN_INC_DIR    "${CMAKE_BINARY_DIR}/generated")

embed_text_to_header("${SHADER_SRC_DIR}/agent.vert"  "${GEN_INC_DIR}/assets/agent_vert.h"  agent_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/agent.frag"  "${GEN_INC_DIR}/assets/agent_frag.h"  agent_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/target.vert"  "${GEN_INC_DIR}/assets/target_vert.h"  target_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/target.frag"  "${GEN_INC_DIR}/assets/target_frag.h"  target_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/canvas.vert"  "${GEN_INC_DIR}/assets/canvas_vert.h"  canvas_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/canvas.frag"  "${GEN_INC_DIR}/assets/canvas_frag.h"  canvas_frag_glsl)

target_link_libraries(genesis
PUBLIC
	imgui
	project_settings
	glfw glad glm::glm
	TBB::tbb
)

target_compile_features(genesis PUBLIC cxx_std_17)

target_include_directories(genesis
PUBLIC
	inc
	extern/stb
	extern
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
	${GEN_INC_DIR}
)

# Apps
add_executable(evolution
			apps/evolution/EvolutionApp.cpp
			apps/evolution/main.cpp
	)
target_link_libraries(evolution
			PRIVATE genesis glm::glm
	)
target_include_directories(evolution
	PRIVATE
		${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)

target_compile_features(evolution PRIVATE cxx_std_17)

# TESTING
enable_testing()
add_subdirectory(tests)
