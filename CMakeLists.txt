cmake_minimum_required(VERSION 3.0.0)

if (${EMSCRIPTEN})

	add_subdirectory(src/Platforms/WebAssembly)

	add_subdirectory(samples/WebAssembly/DemoSite)
else()
	add_subdirectory(OpenGL/external/glfw-3.1.2)
	
	add_subdirectory(OpenGL/external/glew-1.13.0/build/cmake)
	
	add_subdirectory(OpenGL/external/glm-0.9.7.1)

	add_subdirectory(samples/OpenGL/SimpleWindow)
endif()