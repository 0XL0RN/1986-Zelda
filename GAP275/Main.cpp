#include "Engine.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Engine engine;  // Engine Instance
	engine.Awake(); // Engine On
	engine.Run();   // Engine Run
	engine.Sleep(); // Engine ShutDown

	return 0;
}