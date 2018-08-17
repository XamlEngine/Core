cmake_minimum_required(VERSION 3.0.0)

project (XamlEngine CXX)

# Define project.

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -o XamlEngine.html -s NO_EXIT_RUNTIME=1 -std=c++11 -s MODULARIZE=1 -s EXPORT_NAME=\"XamlEngine\" -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"Pointer_stringify\"] --use-preload-plugins -s ALLOW_MEMORY_GROWTH=1 -lGL -lSDL -s LEGACY_GL_EMULATION=1 -s USE_FREETYPE=1 -s USE_GLFW=3 --bind")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_WEBGL2=1 -o XamlEngine.html -s NO_EXIT_RUNTIME=1 -std=c++11 -s MODULARIZE=1 -s EXPORT_NAME=\"XamlEngine\" -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"Pointer_stringify\"] -s LEGACY_GL_EMULATION=1 --js-library ${PROJECT_SOURCE_DIR}/PostFix.js -lglfw --bind")

SET(SOURCE_FILES
	IO/FileManagement.cpp
	WebAssemblySetup.cpp
	Rendering/WebAssemblyFontManager.cpp
)

add_executable(XamlEngine ${SOURCE_FILES})

string(REPLACE / \\ SOURCE_XAMLENGINE_JS_FILE ${PROJECT_SOURCE_DIR}/../../../build/WebAssembly-${CMAKE_BUILD_TYPE}/src/Platforms/WebAssembly/XamlEngine.js)
string(REPLACE / \\ SOURCE_XAMLENGINEOUT_JS_FILE ${PROJECT_SOURCE_DIR}/../../../build/WebAssembly-${CMAKE_BUILD_TYPE}/src/Platforms/WebAssembly/XamlEngineOut.js)
string(REPLACE / \\ SOURCE_POSTFIX_JS_FILE ${PROJECT_SOURCE_DIR}/PostFix.js)

add_custom_command(TARGET XamlEngine POST_BUILD
	COMMAND copy /b ${SOURCE_XAMLENGINE_JS_FILE} + ${SOURCE_POSTFIX_JS_FILE}
)