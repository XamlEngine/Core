cmake_minimum_required(VERSION 3.0.0)

project (DemoSite)



add_custom_target(DemoSite-Web ALL DEPENDS XamlEngine
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/../../../build/WebAssembly-${CMAKE_BUILD_TYPE}/src/Platforms/WebAssembly/XamlEngine.js
	${PROJECT_SOURCE_DIR}/bin/script/XamlEngine.js
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/../../../build/WebAssembly-${CMAKE_BUILD_TYPE}/src/Platforms/WebAssembly/XamlEngine.wasm
	${PROJECT_SOURCE_DIR}/bin/script/XamlEngine.wasm
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/Index.html
	${PROJECT_SOURCE_DIR}/bin/Index.html
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/Verification01.png
	${PROJECT_SOURCE_DIR}/bin/Verification01.png
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/StoreLogo.png
	${PROJECT_SOURCE_DIR}/bin/StoreLogo.png
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/screenshot.png
	${PROJECT_SOURCE_DIR}/bin/screenshot.png
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/Logo.png
	${PROJECT_SOURCE_DIR}/bin/Logo.png
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/Logo.jpg
	${PROJECT_SOURCE_DIR}/bin/Logo.jpg
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/Logo.jpeg
	${PROJECT_SOURCE_DIR}/bin/Logo.jpeg
	COMMAND ${CMAKE_COMMAND} -E copy
	${PROJECT_SOURCE_DIR}/src/ARIAL.TTF
	${PROJECT_SOURCE_DIR}/bin/ARIAL.TTF
	)


#add_custom_command(TARGET DemoSite-Web POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy
#	${PROJECT_SOURCE_DIR}/../../../out/WebAssembly-Release/src/Platforms/WebAssembly/XamlEngine.js
#	../bin/XamlEngine.js
#	COMMENT "Copying DemoSite web output.")

#add_custom_target(DemoSite-Web)
#add_custom_command(TARGET DemoSite-Web POST_BUILD copy ${PROJECT_SOURCE_DIRECTORY}/out/WebAssembly-Release/src/Platforms/WebAssembly/XamlEngine.js bin)

#add_custom_command(TARGET DemoSite-Web POST_BUILD COMMAND copy ARGS ${PROJECT_SOURCE_DIRECTORY}/out/WebAssembly-Release/src/Platforms/WebAssembly/XamlEngine.js bin)
#add_custom_command(TARGET DemoSite-Web POST_BUILD COMMAND copy ARGS ${PROJECT_SOURCE_DIRECTORY}/out/WebAssembly-Release/src/Platforms/WebAssembly/XamlEngine.wasm bin)