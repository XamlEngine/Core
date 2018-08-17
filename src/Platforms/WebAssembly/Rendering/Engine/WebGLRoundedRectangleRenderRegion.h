#pragma once

#include "GL/glew.h"

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <emscripten.h>
#include <emscripten/html5.h>

#include "../../../../Rendering/Engine/RoundedRectangleRenderRegion.h"
#include "WebGLRenderSurfaceContext.h"

#include "../WebAssemblyFontFamily.h"

using namespace CoreVar::Xaml::Rendering::WebAssembly;

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

					/**
					* Overload/wrapper around OpenGL's glVertexAttribIPointer, which avoids
					* code smell due to shady pointer arithmetic at the place of call.
					*
					* See also:
					* https://www.opengl.org/registry/specs/ARB/vertex_buffer_object.txt
					* https://www.opengl.org/sdk/docs/man/html/glVertexAttribPointer.xhtml
					*/
					void glVertexAttribPointer2(GLuint index, GLint size, GLenum type, GLboolean normalize, GLsizei stride, size_t offset)
					{
						GLvoid const* pointer = static_cast<char const*>(0) + offset;
						return glVertexAttribPointer(index, size, type, normalize, stride, (GLvoid*)pointer);
					}

					enum class TextLayoutCharType
					{
						Tab,
						Space,
						NewLine,
						CarriageReturn,
						Other,
						NullTermination
					};

					struct WordMeasurement
					{
						int begin;
						int end;
						double measuredWidth;
						double measuredHeight;
						TextLayoutCharType type;
					};

					struct LineMeasurement
					{
						int wordCount = 0;
						double lineWidth = 0;
					};

					struct RoundedCornerVertexData
					{
						int EdgeCount;
						int OuterVertexCount;
						bool StartsOutside;
						bool EndsOutside;
						int StartingOuterVertexCount;
						int EndingOuterVertexCount;
						int DoubledVertexCount;
						int InnerVertexCount;
						int TotalVertexCount;
					};

					TextLayoutCharType GetTextLayoutCharType(char character)
					{
						switch (character)
						{
						case ' ':
							return TextLayoutCharType::Space;
						case '\t':
							return TextLayoutCharType::Tab;
						case '\n':
							return TextLayoutCharType::NewLine;
						case '\r':
							return TextLayoutCharType::CarriageReturn;
						case 0:
							return TextLayoutCharType::NullTermination;
						default:
							return TextLayoutCharType::Other;
						}
					}

					class WebGLRoundedRectangleRenderRegion : public RoundedRectangleRenderRegion
					{
						GLuint vertexPosObject;
						GLuint textureObject;
						GLuint indexObject;
						Color* _currentSolidColorFill = 0;
						SDL_Surface* _currentTextureSurface = 0;
						FT_GlyphSlot slot;
						std::vector<WordMeasurement> _words = std::vector<WordMeasurement>();
						std::vector<LineMeasurement> _lines = std::vector<LineMeasurement>();
						
						void createSolidColorTexture(RenderSurfaceContext* context)
						{
							GLubyte pixels[4] =
							{
								solidColorFill->red, solidColorFill->green, solidColorFill->blue, solidColorFill->alpha
							};

							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

							glGenTextures(1, &textureObject);

							glBindTexture(GL_TEXTURE_2D, textureObject);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
						}

						void createTextureSurfaceTexture(RenderSurfaceContext* context)
						{
							glGenTextures(1, &textureObject);

							glBindTexture(GL_TEXTURE_2D, textureObject);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSurface->w, textureSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface->pixels);

							SDL_FreeSurface(textureSurface);
						}

						void draw_glyph(GLubyte* target, int targetSize, double targetX, double targetY, FT_GlyphSlot slot)
						{
							targetX = floor(targetX);
							targetY = floor(targetY);
							FT_Bitmap bitmap = slot->bitmap;
							int bitmapLeft = slot->bitmap_left >> 6;
							int bitmapTop = slot->bitmap_top >> 6;
							int hBearingX = slot->metrics.horiBearingX >> 6;
							int hBearingY = slot->metrics.horiBearingY >> 6;
							int hAdvance = slot->metrics.horiAdvance >> 6;

							FT_Face face = ((WebAssemblyFontFamily*)font)->face;
							
							int yMin = (int)face->bbox.yMin;
							yMin = yMin >> 6;

							int fontHeight = (int)(face->height >> 6);
							int glyphHeight = (int)(slot->metrics.height >> 6);

							if (hBearingX > 64)
								hBearingX = (hBearingX >> 6) / 2;
							/*else if (hBearingX < 0)
								hBearingX = 0;*/

							double maxAlpha = 1;
							if (solidColorFill != NULL)
							{
								maxAlpha = solidColorFill->alpha / 255;
							}

							for (int y = 0; y < bitmap.rows; y++)
							{
								for (int byte_index = 0; byte_index < bitmap.pitch; byte_index++)
								{
									int sourceIndex = y * bitmap.pitch + byte_index;
									int targetIndex = ((targetY + y + fontHeight + yMin - hBearingY) * width + (targetX + byte_index + hBearingX)) * 4 + 3;
									
									if (targetIndex >= 0 && targetIndex < targetSize)
									{
										target[targetIndex] = fmax((int)(bitmap.buffer[sourceIndex]), target[targetIndex]);
									}
								}
							}
						}

						void createFreeTypeTextTexture(RenderSurfaceContext* context)
						{
							if (font == NULL || textToRender == NULL)
							{
								return;
							}
							FT_Face face = ((WebAssemblyFontFamily*)font)->face;
							FT_GlyphSlot slot = face->glyph;
							
							int fitWidth = (int)ceil(width);
							int fitHeight = (int)ceil(height);
							int pixelsLength = fitWidth * fitHeight * 4;
							GLubyte pixels[pixelsLength];
							
							if (solidColorFill == NULL)
							{
								memset(pixels, 0x00, sizeof(pixels));
							}
							else
							{
								for (int i = 0; i < pixelsLength; i += 4)
								{
									pixels[i] = solidColorFill->red;
									pixels[i + 1] = solidColorFill->green;
									pixels[i + 2] = solidColorFill->blue;
									pixels[i + 3] = 0x00;
								}
							}

							int finalFontSize = (int)(fontSize * 64);
							int dpiWidth = (int)(96);
							int dpiHeight = (int)(96);

							int error = FT_Set_Char_Size(face, 0, finalFontSize, dpiWidth, dpiHeight);
							if (error)
							{
								printf("error setting pixel size\n");
								return;
							}

							int wordsLength = _words.size();
							double xOffset = 0;
							double yOffset = 0;
							double currentLineHeight = 0;
							bool hasWord = false;
							int currentLineIndex = 0;
							LineMeasurement currentLine = _lines.at(currentLineIndex);
							double wordSeparation = 0;
							switch (textAlignment)
							{
							case TextAlignment::Center:
								xOffset = (fitWidth - currentLine.lineWidth) / 2;
								break;
							case TextAlignment::Right:
								xOffset = fitWidth - currentLine.lineWidth;
								break;
							case TextAlignment::Justify:
								wordSeparation = currentLine.lineWidth / currentLine.wordCount;
								break;
							default:
								break;
							}
							for (int w = 0; w < wordsLength; w++)
							{
								WordMeasurement currentWordMeasurement = _words.at(w);
								
								if (textWrapping == TextWrapping::Wrap && xOffset + currentWordMeasurement.measuredWidth > fitWidth)
								{
									yOffset += currentLineHeight;
									xOffset = 0;
									currentLineHeight = currentWordMeasurement.measuredHeight;
									currentLine = _lines.at(++currentLineIndex);

									switch (textAlignment)
									{
									case TextAlignment::Center:
										xOffset = (fitWidth - currentLine.lineWidth) / 2;
										break;
									case TextAlignment::Right:
										xOffset = fitWidth - currentLine.lineWidth;
										break;
									case TextAlignment::Justify:
										wordSeparation = currentLine.lineWidth / currentLine.wordCount;
										break;
									default:
										break;
									}
								}
								else
								{
									currentLineHeight = fmax(currentWordMeasurement.measuredHeight, currentLineHeight);
								}

								if (hasWord && currentWordMeasurement.type == TextLayoutCharType::Other)
								{
									xOffset += wordSeparation;
								}

								switch (currentWordMeasurement.type)
								{
								case TextLayoutCharType::Other:
									hasWord = true;
									break;
								default:
									break;
								}

								for (int i = currentWordMeasurement.begin; i <= currentWordMeasurement.end; i++)
								{
									if (i > 0)
									{
										FT_Vector kerningVector;
										error = FT_Get_Kerning(face, textToRender[i], textToRender[i - 1], FT_KERNING_DEFAULT, &kerningVector);
										int kerning = (int)kerningVector.x >> 6;
									}

									FT_UInt glyphIndex = FT_Get_Char_Index(face, (FT_ULong)textToRender[i]);
									error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL);
									if (error)
									{
										printf("error loading glyph for char %c %f\n", textToRender[i], fontSize);
									}

									error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
									if (error)
									{
										printf("error rendering glyph for char %d\n", textToRender[i]);
									}

									draw_glyph(pixels, pixelsLength, xOffset, yOffset, slot);

									xOffset += (double)(slot->advance.x >> 6);
								}
							}

							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

							glGenTextures(1, &textureObject);

							glBindTexture(GL_TEXTURE_2D, textureObject);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)width, (int)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

							error = glGetError();
						}


					public:

						void initialize(RenderSurfaceContext* context)
						{
							if (initialized)
							{
								return;
							}
							initialized = true;
							
							if (solidColorFill != 0)
							{
								createSolidColorTexture(context);
								_currentSolidColorFill = solidColorFill;
							}
							else if (textureSurface != 0)
							{
								createTextureSurfaceTexture(context);
								//createTextTexture(context);
								_currentTextureSurface = textureSurface;
							}
							else if (textToRender != NULL && font != NULL)
							{
								
							}
						}

						void terminate()
						{

						}

						void updateLayout(RenderSurfaceContext* context)
						{
							if (solidColorFill != 0 && solidColorFill != _currentSolidColorFill)
							{
								createSolidColorTexture(context);
								_currentSolidColorFill = solidColorFill;
							}
							else if (textureSurface != 0 && textureSurface != _currentTextureSurface)
							{
								createTextureSurfaceTexture(context);
								//createTextTexture(context);
								_currentTextureSurface = textureSurface;
							}
							else if (textToRender != NULL)
							{
								createFreeTypeTextTexture(context);
							}
						}

						void assignVertexData(GLfloat* vertexData, int offset, double texX, double texY, double x, double y, double z)
						{
							offset *= 5;
							vertexData[0 + offset] = texX;
							vertexData[1 + offset] = texY;
							vertexData[2 + offset] = x;
							vertexData[3 + offset] = y;
							vertexData[4 + offset] = z;
						}

						void generateStrokeVertexData(GLfloat** vertexData, int* vertexCount)
						{

						}

						void generateRectangularVertexData(GLfloat** vertexData, int* vertexCount)
						{
							float left = (float)x;
							float top = (float)y;
							float right = (float)x + width;
							float bottom = (float)y + height;

							*vertexData = (GLfloat*)malloc(4 * 5 * sizeof(GLfloat));

							assignVertexData(*vertexData, 0, 0, 0, left, top, 0);
							assignVertexData(*vertexData, 1, 0, 1, left, bottom, 0);
							assignVertexData(*vertexData, 2, 1, 0, right, top, 0);
							assignVertexData(*vertexData, 3, 1, 1, right, bottom, 0);

							*vertexCount = 4;
						}

						void calculateRoundedCornerVertexData(bool startsOutside, double cornerRadius, RoundedCornerVertexData* vertexData)
						{
							double radiusDenominator = 4;
							vertexData->EdgeCount = fmax((int)ceil(cornerRadius / radiusDenominator), 3);
							vertexData->OuterVertexCount = vertexData->EdgeCount + 1;
							if (vertexData->StartsOutside = startsOutside)
							{
								vertexData->StartingOuterVertexCount = 1;
								if (vertexData->EndsOutside = vertexData->EdgeCount % 2 == 1)
								{
									vertexData->DoubledVertexCount = vertexData->OuterVertexCount - 2;
									vertexData->EndingOuterVertexCount = 1;
								}
								else
								{
									vertexData->DoubledVertexCount = vertexData->OuterVertexCount - 1;
									vertexData->EndingOuterVertexCount = 0;
								}
							}
							else
							{
								vertexData->StartingOuterVertexCount = 0;
								if (vertexData->EndsOutside = vertexData->EdgeCount % 2 == 0)
								{
									vertexData->DoubledVertexCount = vertexData->OuterVertexCount - 1;
									vertexData->EndingOuterVertexCount = 1;
								}
								else
								{
									vertexData->DoubledVertexCount = vertexData->OuterVertexCount;
									vertexData->EndingOuterVertexCount = 0;
								}
							}
							vertexData->InnerVertexCount = vertexData->DoubledVertexCount / 2 + 1;

							vertexData->TotalVertexCount = vertexData->InnerVertexCount + vertexData->OuterVertexCount;
						}

						void generateTopLeftRoundedCornerVertexData(GLfloat** vertexData, int startingOffset, RoundedCornerVertexData* cornerVertexData)
						{
							double currentAngle = 0;
							double angleIncrements = (cornerVertexData->OuterVertexCount - 1) / 90d;
							if (cornerVertexData->StartingOuterVertexCount == 1)
							{
								assignVertexData(*vertexData, startingOffset, 0, topLeftCornerRadius / height, left, top + topLeftCornerRadius, 0);
								currentAngle += angleIncrements;
								startingOffset += 1;
							}
							float innerCornerTextureX = (float)topLeftCornerRadius / width;
							float innerCornerTextureY = (float)topLeftCornerRadius / height;
							float innerCornerX = left + topLeftCornerRadius;
							float innerCornerY = top + topLeftCornerRadius;
							assignVertexData(*vertexData, startingOffset, innerCornerTextureX, innerCornerTextureY, innerCornerX, innerCornerY, 0);
							startingOffset += 1;
							
							int sequenceCount = cornerVertexData->DoubledVertexCount / 2;
							for (int i = 0; i < sequenceCount; i++)
							{

							}
						}

						void generateRoundedRectangularVertexData(GLfloat** vertexData, int* vertexCount)
						{
							RoundedCornerVertexData topLeftCornerVertexData;
							RoundedCornerVertexData topRightCornerVertexData;
							RoundedCornerVertexData bottomLeftCornerVertexData;
							RoundedCornerVertexData bottomRightCornerVertexData;

							calculateRoundedCornerVertexData(false, topLeftCornerRadius, &topLeftCornerVertexData);
							calculateRoundedCornerVertexData(!topLeftCornerVertexData.EndsOutside, topRightCornerRadius, &topRightCornerVertexData);
							calculateRoundedCornerVertexData(!topRightCornerVertexData.EndsOutside, bottomRightCornerRadius, &bottomRightCornerVertexData);
							calculateRoundedCornerVertexData(!bottomRightCornerVertexData.EndsOutside, bottomLeftCornerRadius, &bottomLeftCornerVertexData);

							int cornerVertexCount =
								topLeftCornerVertexData->TotalVertexCount +
								topRightCornerVertexData->TotalVertexCount +
								bottomRightCornerVertexData->TotalVertexCount +
								bottomLeftCornerVertexData->TotalVertexCount;

							int totalVertexCount = cornerVertexCount + 5;

							*vertexData = (GLfloat*)malloc(totalVertexCount * 5 * sizeof(GLfloat));

							float left = (float)x;
							float top = (float)y;
							float right = (float)x + width;
							float bottom = (float)y + height;

							float innerTopLeftX = (float)left + topLeftCornerRadius;
							float innerTopLeftY = (float)top + topLeftCornerRadius;
							float innerTopRightX = (float)right - topRightCornerRadius;
							float innerTopRightY = (float)top + topRightCornerRadius;
							float innerBottomRightX = (float)right - bottomRightCornerRadius;
							float innerBottomRightY = (float)bottom - bottomRightCornerRadius;
							float innerBottomLeftX = (float)left + bottomLeftCornerRadius;
							float innerBottomLeftY = (float)bottom - bottomLeftCornerRadius;
							
							assignVertexData(*vertexData, 0, topLeftCornerRadius / width, topRightCornerRadius / height, innerTopRightX, innerTopRightY, 0);
							assignVertexData(*vertexData, 1, bottomRightCornerRadius / width, bottomRightCornerRadius / height, innerBottomRightX, innerBottomRightY, 0);
							assignVertexData(*vertexData, 2, topLeftCornerRadius / width, topLeftCornerRadius / height, innerTopLeftX, innerTopLeftY, 0);
							assignVertexData(*vertexData, 3, innerBottomLeftX / width, innerBottomLeftX / height, innerBottomLeftX, innerBottomLeftY, 0);
							assignVertexData(*vertexData, 4, 0, innerBottomLeftX / height, left, innerBottomLeftY, 0);


						}

						void generateTextureVertexData(GLfloat** vertexData, int* vertexCount)
						{
							if (topLeftCornerRadius <= 0 &&
								topRightCornerRadius <= 0 &&
								bottomLeftCornerRadius <= 0 &&
								bottomRightCornerRadius <= 0)
							{
								generateRectangularVertexData(vertexData, vertexCount);
							}
							else
							{
								generateRoundedRectangularVertexData(vertexData, vertexCount);
							}
						}

						void render(RenderSurfaceContext* context)
						{
							
							if (solidColorFill == 0 && textureSurface == 0 && (textToRender == 0 || font == 0))
							{
								return;
							}
							
							WebGLRenderSurfaceContext* webGLContext = (WebGLRenderSurfaceContext*)context;

							glBindTexture(GL_TEXTURE_2D, textureObject);

							GLfloat* vertexData;
							int vertexCount;
							generateTextureVertexData(&vertexData, &vertexCount);

							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glTexCoordPointer(2, GL_FLOAT, vertexCount * 5, &vertexData[0]);
							glEnableClientState(GL_VERTEX_ARRAY);
							glVertexPointer(3, GL_FLOAT, vertexCount * 5, &vertexData[2]);

							glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

							glDisableClientState(GL_TEXTURE_COORD_ARRAY);
							glDisableClientState(GL_VERTEX_ARRAY);

							delete vertexData;
						}

						

						Size measureText(RenderSurfaceContext* context, Size availableSize)
						{
							if (font == NULL || textToRender == NULL)
							{
								return Size();
							}

							_words.clear();
							_lines.clear();

							FT_Face face = ((WebAssemblyFontFamily*)font)->face;
							FT_GlyphSlot slot = face->glyph;
							
							int actualFontSize = (int)(fontSize * 64);
							int dpiW = (int)(96);
							int dpiH = (int)(96);

							int error = FT_Set_Char_Size(face, 0, actualFontSize, dpiW, dpiH);
							if (error)
							{
								printf("error setting pixel size\n");
								return Size();
							}

							int textLength = strlen(textToRender);
							double faceHeight = (int)(face->height >> 6);
							int currentWordStartIndex = 0;
							double currentWordWidth = 0;
							for (int i = 0; i < textLength; i++)
							{
								char currentChar = textToRender[i];
								char nextChar = textToRender[i + 1];

								TextLayoutCharType currentCharType = GetTextLayoutCharType(currentChar);
								TextLayoutCharType nextCharType = GetTextLayoutCharType(nextChar);
								bool newWord = true;
								switch (currentCharType)
								{
								case TextLayoutCharType::NewLine:
								case TextLayoutCharType::CarriageReturn:
								case TextLayoutCharType::Tab:
								case TextLayoutCharType::Space:
									break;
								case TextLayoutCharType::Other:
									newWord = currentCharType != nextCharType;
									break;
								default:
									throw;
								}

								switch (currentCharType)
								{
								case TextLayoutCharType::NewLine:
								case TextLayoutCharType::CarriageReturn:
									break;
								default:
									FT_UInt glyphIndex = FT_Get_Char_Index(face, (FT_ULong)textToRender[i]);

									error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
									if (error)
									{
										printf("error loading glyph for char %c\n", textToRender[i]);
									}

									int xAdvance = slot->advance.x >> 6;

									printf("x advance %d\n", xAdvance);

									currentWordWidth += (double)xAdvance;
									break;
								}

								if (newWord || i == textLength - 1)
								{
									WordMeasurement measurement = WordMeasurement();
									measurement.begin = currentWordStartIndex;
									measurement.end = i;
									measurement.measuredWidth = currentWordWidth;
									measurement.measuredHeight = faceHeight;
									measurement.type = currentCharType;
									_words.push_back(measurement);

									currentWordWidth = 0;
									currentWordStartIndex = i + 1;
								}
							}

							int wordCount = _words.size();
							double totalHeight = 0;
							double totalWidth = 0;
							double currentLineWidth = 0;
							double currentLineHeight = 0;
							LineMeasurement currentLine = LineMeasurement();
							for (int i = 0; i < wordCount; i++)
							{
								WordMeasurement currentWordMeasurement = _words.at(i);
								printf("word width: %f\n", currentWordMeasurement.measuredWidth);
								double newLineWidth = currentLineWidth + currentWordMeasurement.measuredWidth;
								currentLineHeight = fmax(currentLineHeight, currentWordMeasurement.measuredHeight);
								if (textWrapping == TextWrapping::Wrap && currentLineWidth > 0 && newLineWidth > availableSize.width)
								{
									totalHeight += currentLineHeight;
									totalWidth = fmax(totalWidth, currentLineWidth);

									_lines.push_back(currentLine);
									currentLine = LineMeasurement();

									currentLine.lineWidth = currentLineWidth = currentWordMeasurement.measuredWidth;
									currentLineHeight = currentWordMeasurement.measuredHeight;
								}
								else
								{
									currentLine.lineWidth = currentLineWidth = newLineWidth;
								}
								currentLine.wordCount++;
							}

							_lines.push_back(currentLine);

							totalWidth = fmax(currentLineWidth, totalWidth);
							totalHeight = currentLineHeight + totalHeight;

							Size result = Size();
							result.width = totalWidth;
							result.height = totalHeight;

							return result;
						}

					};
				}
			}
		}
	}
}