#pragma once

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>

#include "../../Parsing/XamlParser.h"
#include "../../Rendering/Helpers/XamlEngineParserNodeVisitor.h"
#include "../../Rendering/XamlRenderEngine.h"
#include "../../Platforms/WebAssembly/Rendering/Engine/WebGLRenderSurface.h"
#include "../../Platforms/WebAssembly/Services/WebAssemblyXamlObjectFactory.h"
#include "../../UI/UIXamlResolver.h"
#include "../../Runtime/SystemXamlResolver.h"
#include "../../IO/FileManagement.h"

using namespace emscripten;
using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::Rendering::Helpers;
using namespace CoreVar::Xaml::Rendering::Engine::WebAssembly;
using namespace CoreVar::Xaml::Services::WebAssembly;
using namespace CoreVar::Xaml::UI;
using namespace CoreVar::Xaml::Runtime;

int isFirstLoop;



int getXamlCanvasCount()
{
	return EM_ASM_INT({
		var canvasCount = 0;
		var canvases = document.querySelectorAll("canvas");
		for (var i = 0; i < canvases.length; i++)
		{
			var scriptId = canvas.getAttribute("XamlScript");
			if (scriptId) {
				canvasCount++;
			}
		}
		return canvasCount;
		});
}

int getXamlScriptCount()
{
	return EM_ASM_INT({
		var scriptCount = 0;
		var scripts = document.querySelectorAll("script");

		for (var i = 0; i < scripts.length; i++)
		{
			if (scripts[i].type.toLowerCase() == "text/xaml")
			{
				scriptCount++;
			}
		}
		return scriptCount;
		});
}

std::string* getXamlScriptId(int index)
{
	return (std::string*)EM_ASM_INT({
		var scriptCount = 0;
		var scripts = document.querySelectorAll("script");

		for (var i = 0; i < scripts.length; i++)
		{
			if (scripts[i].type.toLowerCase() == "text/xaml")
			{
				if (scriptCount == $0)
				{
					if (scripts[i].id)
					{
						return allocate(intArrayFromString(scripts[i].id), 'i8', ALLOC_NORMAL);
					}
					return 0;
				}
				scriptCount++;
			}
		}
		return 0;
		}, index);
}

std::string* getDefaultXamlScriptId()
{
	return (std::string*)EM_ASM_INT({
		var scriptCount = 0;
		var scripts = document.querySelectorAll("script");

		for (var i = 0; i < scripts.length; i++)
		{
			if (scripts[i].type.toLowerCase() == "text/xaml")
			{
				var defaultAttribute = scripts[i].getAttribute("default");
				if (defaultAttribute && defaultAttribute.toLowerCase() == "true")
				{
					return allocate(intArrayFromString(scripts[i].id), 'i8', ALLOC_NORMAL);
				}
			}
		}
		return 0;
		});
}

void setXamlScriptId(int index, std::string* scriptId)
{
	int result = EM_ASM_INT({
		var scriptCount = 0;
		var scripts = document.querySelectorAll("script");

		for (var i = 0; i < scripts.length; i++)
		{
			if (scripts[i].type.toLowerCase() == "text/xaml")
			{
				if (scriptCount == $0)
				{
					scripts[i].id = XamlEngine.Pointer_stringify($1);
					break;
				}
			}
		}
		return 0;
		}, index, scriptId->c_str());
}

std::string* getAttribute(std::string* elementId, std::string* attributeName)
{
	return (std::string*)EM_ASM_INT({
		var elementId = XamlEngine.Pointer_stringify($0);
		var attributeName = XamlEngine.Pointer_stringify($1);
		var element = document.getElementById(elementId);
		if (!element)
		{
			return 0;
		}
		var attributeValue = element.getAttribute(attributeName);
		if (!attributeValue)
		{
			return 0;
		}
		return allocate(intArrayFromString(attributeValue), 'i8', ALLOC_NORMAL);
		}, elementId->c_str(), attributeName->c_str());
}

void createXamlCanvas(std::string* canvasId, std::string* scriptId)
{
	int result = EM_ASM_INT({
		var canvasId = XamlEngine.Pointer_stringify($0);
		var scriptId = XamlEngine.Pointer_stringify($1);
		var scriptElement = document.getElementById(scriptId);
		var canvas = document.createElement("CANVAS");
		canvas.id = canvasId;
		canvas.setAttribute("XamlScript", scriptId);
		canvas.oncontextmenu = function() { event.preventDefault(); };
		scriptElement.parentElement.insertBefore(canvas, scriptElement);
		return 0;
		}, canvasId->c_str(), scriptId->c_str());
}

void setupFullPageLayout(std::string* canvasId)
{
	int result = EM_ASM_INT({
		var canvas = document.getElementById(XamlEngine.Pointer_stringify($0));

		var htmlStyle = document.documentElement.style;
		htmlStyle.margin = "0";
		htmlStyle.padding = "0";
		htmlStyle.width = "100%";
		htmlStyle.height = "100%";
		htmlStyle.overflow = "hidden";

		var bodyStyle = document.body.style;
		bodyStyle.margin = "0";
		bodyStyle.padding = "0";
		bodyStyle.width = "100%";
		bodyStyle.height = "100%";
		bodyStyle.overflow = "hidden";

		var canvasStyle = canvas.style;
		canvasStyle.width = "100%";
		canvasStyle.height = "100%";
		return 0;
		}, canvasId->c_str());
}

std::string* getInnerText(std::string* elementId)
{
	return (std::string*)EM_ASM_INT({
		var elementId = XamlEngine.Pointer_stringify($0);
		var element = document.getElementById(elementId);
		return allocate(intArrayFromString(element.innerText), 'i8', ALLOC_NORMAL);
		}, elementId->c_str());
}

void setDefaultCanvas(std::string* canvasId)
{
	EM_ASM_INT({
		var elementId = XamlEngine.Pointer_stringify($0);
		Module.canvas = document.getElementById(elementId);
		}, canvasId->c_str());
}

XamlRenderEngine* XamlInit()
{
	XamlObjectFactory* objectFactory = (XamlObjectFactory*)new WebAssemblyXamlObjectFactory();
	WebGLRenderSurface* renderSurface = (WebGLRenderSurface*)objectFactory->createRenderSurface();
	XamlRenderEngine* renderEngine = new XamlRenderEngine(renderSurface, objectFactory);

	((WebAssemblyFontManager*)renderEngine->get_services()->fontManager)->begin_load_font(new std::string("arial.ttf"), new std::string("Arial"));

	renderEngine->add_resolver(new UIXamlResolver());
	renderEngine->add_resolver(new SystemXamlResolver());

	int canvasCount = getXamlCanvasCount();
	if (canvasCount == 0)
	{
		int scriptCount = getXamlScriptCount();

		for (int i = 0; i < scriptCount; i++)
		{
			std::string* scriptId = getXamlScriptId(i);

			if (scriptId == 0)
			{
				scriptId = new std::string("MainXaml_Script_" + std::to_string(i + 1));

				setXamlScriptId(i, scriptId);

				delete scriptId;
			}
		}

		std::string* defaultXamlScriptId = getDefaultXamlScriptId();
		std::string* defaultXamlCanvasId = 0;

		if (defaultXamlScriptId == 0)
		{
			if (scriptCount == 1)
			{
				defaultXamlScriptId = getXamlScriptId(0);
			}
		}

		if (defaultXamlScriptId != 0)
		{
			std::string* canvasId = new std::string("MainXaml_Canvas");
			std::string* pageLayoutName = new std::string("pageLayout");
			std::string* pageLayout = getAttribute(defaultXamlScriptId, pageLayoutName);

			createXamlCanvas(canvasId, defaultXamlScriptId);

			if (pageLayout == 0 || strcmp("full", pageLayout->c_str()) == 0)
			{
				setupFullPageLayout(canvasId);
			}
			else
			{
				// Normal page layout ???
			}

			/*emscripten_set_main_loop(renderLoop, -1, 0);*/

			std::string* xaml = getInnerText(defaultXamlScriptId);

			setDefaultCanvas(canvasId);
			
			renderSurface->initialize();

			XamlEngineParserNodeVisitor* nodeVisitor = new XamlEngineParserNodeVisitor(renderEngine, new std::string("CoreVar.Xaml.UI"));
			XamlParser* parser = new XamlParser();
			parser->parse((char*)xaml->c_str(), nodeVisitor);

			renderEngine->set_rootVisual((UIElement*)nodeVisitor->get_result());
			
			// Render
			XamlRenderContext* renderContext = renderEngine->create_context();
			renderEngine->render(renderContext);

			delete renderContext;
		}

	}

	return renderEngine;
}

void renderLoop(void* arg)
{
	XamlRenderEngine* renderEngine = (XamlRenderEngine*)arg;
	
	XamlRenderContext* renderContext = renderEngine->create_context();

	renderEngine->render(renderContext);

	delete renderContext;

	emscripten_async_call(renderLoop, renderEngine, 30);
}