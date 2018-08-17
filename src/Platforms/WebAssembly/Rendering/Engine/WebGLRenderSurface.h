#pragma once

#ifndef XAML_WEBGL
#define XAML_WEBGL
#endif

#include <stdlib.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>

#include "GL/glew.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include <vector>

#include "../../../../Rendering/Engine/RenderSurface.h"
#include "UserData.h"
#include "StringHelpers.h"
#include "WebGLRoundedRectangleRenderRegion.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Engine
			{
				namespace WebAssembly
				{
					class WebGLRenderSurface : public RenderSurface
					{
					private:
						bool _initialized = false;
						GLuint _programObject = 0;
						GLuint _textureId = 0;
						EMSCRIPTEN_WEBGL_CONTEXT_HANDLE _context = 0;
						const char* _canvasId = 0;
						double _width = 0;
						double _height = 0;
						double _devicePixelRatio = 0;
						SDL_Window* _window = 0;
						SDL_Renderer* _renderer = 0;
						SDL_Surface* _screen = 0;
						
						void updateSize()
						{
							double window_dpr = EM_ASM_DOUBLE({
								return window.devicePixelRatio;
								});

							double css_width;
							double css_height;

							int canvas_width;
							int canvas_height;

							emscripten_get_element_css_size(_canvasId, &css_width, &css_height);

							// FIXME: the css size returned above might in fact be the canvas render
							// size, if no CSS size is set. This means that setting the canvas render
							// size below will feed back into the next read of the CSS size, which is
							// not what we want.

							double pixel_width = css_width * window_dpr;
							double pixel_height = css_height * window_dpr;

							set_width(pixel_width);
							set_height(pixel_height);
							set_devicePixelRatio(window_dpr);

							emscripten_set_canvas_element_size(_canvasId, (int)pixel_width, (int)pixel_height);
						}

						void initializeCanvas()
						{
							double window_dpr = EM_ASM_DOUBLE({
								return window.devicePixelRatio;
								});

							double css_width;
							double css_height;

							int canvas_width;
							int canvas_height;

							_screen = SDL_SetVideoMode(0, 0, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE);

							emscripten_get_element_css_size(_canvasId, &css_width, &css_height);

							// FIXME: the css size returned above might in fact be the canvas render
							// size, if no CSS size is set. This means that setting the canvas render
							// size below will feed back into the next read of the CSS size, which is
							// not what we want.

							double pixel_width = css_width * window_dpr;
							double pixel_height = css_height * window_dpr;

							set_width(pixel_width);
							set_height(pixel_height);
							set_devicePixelRatio(window_dpr);

							emscripten_set_canvas_element_size(_canvasId, (int)pixel_width, (int)pixel_height);
						}

						bool initializeOpenGLProgram()
						{
							GLint ok;

							glewInit();	

							const char *vertexShader = "void main(void) \n"
								"{ \n"
								"    gl_Position = ftransform(); \n"
								"    gl_TexCoord[0] = gl_MultiTexCoord0; \n"
								"    gl_FrontColor = gl_Color; \n"
								"} \n";
							const char *fragmentShader = "uniform sampler2D tex0; \n"
								"void main(void) \n"
								"{ \n"
								"    gl_FragColor = gl_Color * texture2D(tex0, gl_TexCoord[0].xy); \n"
								"} \n";

							GLuint vs = glCreateShader(GL_VERTEX_SHADER);
							glShaderSource(vs, 1, &vertexShader, NULL);
							glCompileShader(vs);
							glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);
							//assert(ok);

							GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
							glShaderSource(fs, 1, &fragmentShader, NULL);
							glCompileShader(fs);
							glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
							//assert(ok);

							// Create the program object
							GLuint programObject = glCreateProgram();

							if (programObject == 0)
								return 0;

							glAttachShader(programObject, vs);
							glAttachShader(programObject, fs);

							// Link the program
							glLinkProgram(programObject);

							GLint linked;

							// Check the link status
							glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

							if (!linked)
							{
								GLint infoLen = 0;

								glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

								if (infoLen > 1)
								{
									char* infoLog = (char*)malloc(sizeof(char) * infoLen);

									glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
									printf("Error linking program:\n%s\n", infoLog);

									free(infoLog);
								}

								glDeleteProgram(programObject);
								return GL_FALSE;
							}

							glDeleteShader(vs);
							glDeleteShader(fs);

							// Store the program object
							_programObject = programObject;

							void *data = this;
							bool capture = true;

							auto callback = [](int eventType, const EmscriptenUiEvent *uiEvent, void *userData)->EM_BOOL
							{
								WebGLRenderSurface* surface = (WebGLRenderSurface*)userData;
								surface->canvasSizeChanged(eventType, uiEvent);
								return true;
							};

							emscripten_set_resize_callback(0, data, capture, callback);

							glEnable(GL_BLEND);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

							return GL_TRUE;
						}

						void canvasSizeChanged(int eventType, const EmscriptenUiEvent* uiEvent)
						{
							updateSize();

							if (events != 0)
							{
								events->surfaceSizeChanged();
							}
						}

					protected:
						void set_width(double value)
						{
							_width = value;
						}

						void set_height(double value)
						{
							_height = value;
						}

						void set_devicePixelRatio(double value)
						{
							_devicePixelRatio = value;
						}

						RenderSurfaceContext* onCreateContext()
						{
							WebGLRenderSurfaceContext* renderSurfaceContext = new WebGLRenderSurfaceContext();
							renderSurfaceContext->screen = _screen;
							return (RenderSurfaceContext*)renderSurfaceContext;
						}

					public:

						const char* get_canvasId()
						{
							return _canvasId;
						}

						void set_canvasId(const char* value)
						{
							if (_canvasId)
							{
								terminate();
							}
							
							_canvasId = value;

							_initialized = false;
						}

						double get_width()
						{
							return _width;
						}

						double get_height()
						{
							return _height;
						}

						double get_devicePixelRatio()
						{
							return _devicePixelRatio;
						}

						void initialize()
						{
							if (_initialized)
							{
								return;
							}
							_initialized = true;

							if (SDL_Init(SDL_INIT_VIDEO) != 0)
							{
								printf("unable to initialize\n");
								return;
							}

							TTF_Init();

							SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
							SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
							SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
							SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
							//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

							SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


							// updateSize();
							initializeCanvas();

							initializeOpenGLProgram();
						}

						void terminate()
						{

						}

						void render(RenderSurfaceContext* context)
						{
							// Set the viewport
							glViewport(0, 0, _width, _height);

							// Use the program object
							glUseProgram(_programObject);

							glClearColor(0x00, 0x00, 0x00, 0x00);

							glEnable(GL_TEXTURE_2D); // Need this to display a texture XXX unnecessary in OpenGL ES 2.0/WebGL

							glMatrixMode(GL_PROJECTION);

							GLfloat matrixData[] = { 2.0f / (float)_width,        0,  0,  0,
								0, -2.0f / (float)_height,  0,  0,
								0,        0, -1,  0,
								-1,        1,  0,  1 };

							glLoadMatrixf(matrixData); // test loadmatrix

							glMatrixMode(GL_MODELVIEW);
							glLoadIdentity();
						}

						void finalizeRender(RenderSurfaceContext* context)
						{
							//SDL_Flip(_screen);

							SDL_GL_SwapBuffers();
						}

					};
				}
			}
		}
	}
}