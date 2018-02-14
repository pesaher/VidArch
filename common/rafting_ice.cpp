#include "stdafx.h"
#include "sys.h"
#include "core.h"
#include "font.h"
#include <chrono>

#define NUM_RAFTS       3
#define RAFT_SPEED      2.f
#define CHARACTER_SPEED 4.f
#define FIRE_DURATION   500.f
#define SAFE            FIRE_DURATION
#define LAVA            0

//-----------------------------------------------------------------------------
struct Raft
{
	vec2   pos;
	vec2   vel;
	GLuint gfx;
	vec2   size;
	float  state;
};

//-----------------------------------------------------------------------------
struct Character
{
	vec2   pos;
	GLuint gfx;
	vec2   size;
};

Raft rafts[NUM_RAFTS];

bool overlap(float a0, float a1, float b0, float b1);
int overlap(Raft r1, Raft r2);
int overlap(Raft r, Character c);

//-----------------------------------------------------------------------------
int Main(void)
{
	FONT_Init();

	// Load textures
	GLuint texBackground = CORE_LoadPNG("data/lava.png",     true);
	GLuint texRaft       = CORE_LoadPNG("data/raft.png",     false);
	GLuint texIceCube    = CORE_LoadPNG("data/ice_cube.png", false);
	GLuint texFire       = CORE_LoadPNG("data/fire.png", false);

	// Init game state
	for (int i = 0; i < NUM_RAFTS; ++i) {
		rafts[i].pos = vmake(CORE_FRand(0.0, SCR_WIDTH - rafts[i].size.x), CORE_FRand(0.0, SCR_HEIGHT - rafts[i].size.y));
		rafts[i].vel = vmake(CORE_FRand(-RAFT_SPEED, +RAFT_SPEED), CORE_FRand(-RAFT_SPEED, +RAFT_SPEED));
		rafts[i].size = { 200.f, 100.f };
		rafts[i].gfx = texRaft;
		rafts[i].state = SAFE;
	}

	Character iceCube;
	iceCube.pos = rafts[0].pos;
	iceCube.size = { 50.f, 50.f };
	iceCube.gfx = texIceCube;

	Character fire;
	fire.pos = { 250.f, 30.f };
	fire.size = { 75.f, 75.f };
	fire.gfx = texFire;
	
	// Set up rendering
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping
	glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::chrono::time_point<std::chrono::steady_clock> currentTime;
	auto lastTime = std::chrono::high_resolution_clock::now();
	
	while (!SYS_GottaQuit()) {
		// Compute delta time
		currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = currentTime - lastTime;
		lastTime = currentTime;
		float deltaTime = static_cast<float>(elapsed.count() * 50);
		bool safe = false;

		// Render
		glClear( GL_COLOR_BUFFER_BIT );
		
		// Render backgground
		for (int i = 0; i <= SCR_WIDTH / 128; ++i)
			for (int j = 0; j <= SCR_HEIGHT / 128; j++)
				CORE_RenderCenteredSprite(vmake(i * 128.f + 64.f, j * 128.f + 64.f), vmake(128.f, 128.f), texBackground);
		
		// Render rafts
		for (int i = 0; i < NUM_RAFTS; ++i)
			if (rafts[i].state > LAVA) {
				if (rafts[i].state != SAFE)
					glColor4f(1.0, 0, 0, .5);
				CORE_RenderSprite(rafts[i].pos, vadd(rafts[i].pos, rafts[i].size), rafts[i].gfx);
				glColor4f(1, 1, 1, 1);
			}
		
		// Render player and fire
		glEnable(GL_BLEND);
		CORE_RenderSprite(iceCube.pos, vadd(iceCube.pos, iceCube.size), iceCube.gfx);
		if(fire.pos.x != -1.f && fire.pos.y != -1.f)
			CORE_RenderSprite(fire.pos, vadd(fire.pos, fire.size), fire.gfx);
		glDisable(GL_BLEND);
		
		// Text
		FONT_DrawString(vmake(SCR_WIDTH/2 - 6*16, 16), "HELLO WORLD!");
		
		SYS_Show();
		
		// Move player
		if (GetKeyState('W') & 0x8000) {
			iceCube.pos = vadd(iceCube.pos, { 0.f, CHARACTER_SPEED * deltaTime });
		}
		if (GetKeyState('S') & 0x8000) {
			iceCube.pos = vadd(iceCube.pos, { 0.f, -CHARACTER_SPEED * deltaTime });
		}
		if (GetKeyState('A') & 0x8000) {
			iceCube.pos = vadd(iceCube.pos, { -CHARACTER_SPEED * deltaTime, 0.f });
		}
		if (GetKeyState('D') & 0x8000) {
			iceCube.pos = vadd(iceCube.pos, { CHARACTER_SPEED * deltaTime, 0.f });
		}

		// Spawn fire
		if (fire.pos.x == -1.f && fire.pos.y == -1.f)
			if (rand() % 200 < 1){
				bool collides = true;
				while (collides) {
					collides = false;
					fire.pos = vmake(CORE_FRand(0.0, SCR_WIDTH - fire.size.x), CORE_FRand(0.0, SCR_HEIGHT - fire.size.y));
					for (int i = 0; i < NUM_RAFTS; ++i)
						if (rafts[i].state > LAVA)
							if (overlap(rafts[i], fire)) {
								collides = true;
								break;
							}
				}
			}
		
		// Run rafts
		for (int i = 0; i < NUM_RAFTS; ++i) {
			if (rafts[i].state > LAVA) {
				vec2 newpos = vadd(rafts[i].pos, vscale(rafts[i].vel, deltaTime));

				int colliding_raft = -1;
				int overlap_type = 0;
				for (int j = 0; j < NUM_RAFTS; j++) {
					if (i != j && rafts[j].state > LAVA) {
						overlap_type = overlap(rafts[i], rafts[j]);
						if (overlap_type) {
							colliding_raft = j;
							break;
						}
					}
				}

				if (!overlap_type)
					rafts[i].pos = newpos;
				else {
					// Rebound!
					rafts[i].vel = vscale(rafts[i].vel, -1.f);
					rafts[i].pos = vadd(rafts[i].pos, vscale(rafts[i].vel, deltaTime));
					rafts[colliding_raft].vel = vscale(rafts[colliding_raft].vel, -1.f);
					rafts[colliding_raft].pos = vadd(rafts[colliding_raft].pos, vscale(rafts[colliding_raft].vel, deltaTime));
				}

				// Rebound on margins
				if (rafts[i].vel.x > 0.0) {
					if (rafts[i].pos.x + rafts[i].size.x > SCR_WIDTH)
						rafts[i].vel.x *= -1.0;
				} else {
					if (rafts[i].pos.x < 0)
						rafts[i].vel.x *= -1.0;
				}
				if (rafts[i].vel.y > 0.0) {
					if (rafts[i].pos.y + rafts[i].size.y > SCR_HEIGHT)
						rafts[i].vel.y *= -1.0;
				} else {
					if (rafts[i].pos.y < 0)
						rafts[i].vel.y *= -1.0;
				}
			} else
				// Respawn sunken rafts
				if (rand() % 100 < 1) {
					bool collides = true;
					while (collides) {
						collides = false;
						rafts[i].pos = vmake(CORE_FRand(0.0, SCR_WIDTH - rafts[i].size.x), CORE_FRand(0.0, SCR_HEIGHT - rafts[i].size.y));
						for(int j = 0; j < NUM_RAFTS; ++j)
							if(i != j && rafts[j].state > LAVA)
								if (overlap(rafts[i], rafts[j])) {
									collides = true;
									break;
								}
					}
					rafts[i].state = SAFE;
				}

			// Sink rafts and check game over
			for (int i = 0; i < NUM_RAFTS; ++i) {
				if(rafts[i].state > LAVA)
					if (overlap(rafts[i], iceCube))
						safe = true;
				if (rafts[i].state == SAFE) {
					if (fire.pos.x != -1.f && fire.pos.y != -1.f)
						if (overlap(rafts[i], fire)) {
							rafts[i].state -= deltaTime;
							fire.pos = vmake(-1.f, -1.f);
						}
				} else if (rafts[i].state > LAVA) {
					rafts[i].state -= deltaTime;
				}
			}
		}

		if (!safe)
			break;
		
		// Keep system running
		SYS_Pump();
		SYS_Sleep(17);
	}
	
	CORE_UnloadPNG(texBackground);
	CORE_UnloadPNG(texRaft);
	CORE_UnloadPNG(texIceCube);
	CORE_UnloadPNG(texFire);
	FONT_End();
	
	return 0;
}

bool overlap(float a0, float a1, float b0, float b1) {
	bool overlapResult = false;
	if (a0 <= b0) {
		if (a1 >= b0)
			overlapResult = true;
	}
	else if (b1 >= a0)
		overlapResult = true;
	return overlapResult;
}

int overlap(Raft r1, Raft r2) {
	int overlapResult = 0;
	if (overlap(r1.pos.x, r1.pos.x + r1.size.x, r2.pos.x, r2.pos.x + r2.size.x)
		&&
		overlap(r1.pos.y, r1.pos.y + r1.size.y, r2.pos.y, r2.pos.y + r2.size.y)) {
		if ((r1.vel.x > 0 && r2.vel.x < 0) || (r1.vel.x < 0 && r2.vel.x > 0))
			overlapResult = 1;
		else if ((r1.vel.y > 0 && r2.vel.y < 0) || (r1.vel.y < 0 && r2.vel.y > 0))
			overlapResult = 2;
		else
			overlapResult = 3;
	}
	return overlapResult;
}

int overlap(Raft r, Character c) {
	int overlapResult = 0;
	if (overlap(r.pos.x, r.pos.x + r.size.x, c.pos.x, c.pos.x + c.size.x)
		&&
		overlap(r.pos.y, r.pos.y + r.size.y, c.pos.y, c.pos.y + c.size.y))
			overlapResult = 1;
	return overlapResult;
}