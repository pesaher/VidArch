#include "levelManager.h"
#include <chrono>
#include "base.h"

// Starting point of the game
int Main(void)
{
	CLevelManager game;
	std::chrono::high_resolution_clock timer;
	auto lastTime = timer.now();
	while (!SYS_GottaQuit())
	{
		auto currentTime = timer.now();
		using ms = std::chrono::duration<float, std::milli>;
		float deltaTime = std::chrono::duration_cast<ms>(currentTime - lastTime).count() / 1000.f;
		if (deltaTime > 0.1f)
			deltaTime = 0.f;
		lastTime = currentTime;
		game.update(deltaTime);
		SYS_Show();
		SYS_Pump();
		SYS_Sleep(17);
	}
	return 0;
}
