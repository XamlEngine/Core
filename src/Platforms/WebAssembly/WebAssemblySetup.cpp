#include "WebAssemblySetup.h"

void test(void* arg)
{
	printf("main loop\n");
}

int main(int argc, char *argv[])
{

	XamlRenderEngine* renderEngine = XamlInit();

	XamlRenderContext* context = renderEngine->create_context();

	renderEngine->render(context);

	delete context;

	//emscripten_set_main_loop_arg(renderLoop, renderEngine, 0, 0);

	emscripten_async_call(renderLoop, renderEngine, 30);

	return 1;
}