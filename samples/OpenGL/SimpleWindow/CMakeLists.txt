# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.0)

find_package(OpenGL REQUIRED)

project(SimpleWindow)

# On Visual 2005 and above, this module can set the debug working directory
#cmake_policy(SET CMP0026 OLD)
#list(APPEND CMAKE_MODULE_PATH "../../../OpenGL/external/rpavlik-cmake-modules-fe2273")
#include(CreateLaunchers)
#include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(../../../OpenGL/distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	../../../OpenGL/external/AntTweakBar-1.16/include/
	../../../OpenGL/external/glfw-3.1.2/include/
	../../../OpenGL/external/glm-0.9.7.1/
	../../../OpenGL/external/glew-1.13.0/include/
	../../../OpenGL/external/assimp-3.0.1270/include/
	../../../OpenGL/external/bullet-2.81-rev2613/src/
	../../../OpenGL
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	disglfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(SimpleWindow
	src/main.cpp
	../../../OpenGL/common/shader.cpp
	../../../OpenGL/common/shader.hpp
	
	src/SimpleFragmentShader.fragmentshader
	src/SimpleVertexShader.vertexshader)

target_link_libraries(SimpleWindow
	${ALL_LIBS}
)

set_target_properties(SimpleWindow PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SimpleWindow/")
#create_target_launcher(SimpleWindow WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SimpleWindow/")

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET SimpleWindow POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/SimpleWindow{CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/SimpleWindow/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
