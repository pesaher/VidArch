#include "stdafx.h"
#include "sys.h"
#include "core.h"
#include "font.h"

//-----------------------------------------------------------------------------
struct Raft
{
	vec2   pos;
	vec2   vel;
	GLuint gfx;
	vec2   size;
};

//-----------------------------------------------------------------------------
struct IceCube
{
	vec2   pos;
	GLuint gfx;
	vec2   size;
};

#define NUM_RAFTS       3
#define RAFT_SPEED      8.f
#define CHARACTER_SPEED 8.f

Raft rafts[NUM_RAFTS];


//-----------------------------------------------------------------------------
int Main(void)
{
  FONT_Init();

  // Load textures
  GLuint texBackground = CORE_LoadPNG("data/lava.png",     true);
  GLuint texRaft       = CORE_LoadPNG("data/raft.png",     false);
  GLuint texIceCube    = CORE_LoadPNG("data/ice_cube.png", false);

  // Init game state
  for (int i = 0; i < NUM_RAFTS; i++)
  {
	  rafts[i].pos = vmake(CORE_FRand(0.0, SCR_WIDTH), CORE_FRand(0.0, SCR_HEIGHT));
	  rafts[i].vel = vmake(CORE_FRand(-RAFT_SPEED, +RAFT_SPEED), CORE_FRand(-RAFT_SPEED, +RAFT_SPEED));
	  rafts[i].size = { 200.f, 100.f };
	  rafts[i].gfx = texRaft;
  }

  IceCube iceCube;
  iceCube.pos = { 30.f, 30.f };
  iceCube.size = { 25.f, 25.f };
  iceCube.gfx = texIceCube;

  // Set up rendering
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping
  glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!SYS_GottaQuit())
  {
    // Render
    glClear( GL_COLOR_BUFFER_BIT );

    // Render backgground
    for (int i = 0; i <= SCR_WIDTH / 128; i++)
      for (int j = 0; j <= SCR_HEIGHT / 128; j++)
        CORE_RenderCenteredSprite(vmake(i * 128.f + 64.f, j * 128.f + 64.f), vmake(128.f, 128.f), texBackground);

    // Render rafts
    for (int i = 0; i < NUM_RAFTS; i++)
		CORE_RenderSprite(rafts[i].pos, vadd(rafts[i].pos, rafts[i].size), rafts[i].gfx);

	CORE_RenderSprite(iceCube.pos, vscale(iceCube.size, 2.f), iceCube.gfx);

    // Text
    FONT_DrawString(vmake(SCR_WIDTH/2 - 6*16, 16), "HELLO WORLD!");
   
    SYS_Show();

    // Run balls
    for (int i = 0; i < NUM_RAFTS; i++)
    {
      vec2 oldpos = rafts[i].pos;
      vec2 newpos = vadd(oldpos, rafts[i].vel);

      bool collision = false;
      int colliding_ball = -1;
      for (int j = 0; j < NUM_RAFTS; j++)
      {
        if (i != j)
        {
          float limit2 = (rafts[i].size.x + rafts[j].size.x) * (rafts[i].size.y + rafts[j].size.y);
          if (vlen2(vsub(oldpos, rafts[j].pos)) > limit2 && vlen2(vsub(newpos, rafts[j].pos)) <= limit2)
          {
            collision = true;
            colliding_ball = j;
            break;
          }
        }
      }

      if (!collision)
		  rafts[i].pos = newpos;
      else
      {
        // Rebound!
		  rafts[i].vel = vscale(rafts[i].vel, -1.f);
		  rafts[colliding_ball].vel = vscale(rafts[colliding_ball].vel, -1.f);
      }

      // Rebound on margins
      if (rafts[i].vel.x > 0.0)
      {
        if (rafts[i].pos.x > SCR_WIDTH)
			rafts[i].vel.x *= -1.0;
      } else {
        if (rafts[i].pos.x < 0)
			rafts[i].vel.x *= -1.0;
      }
      if (rafts[i].vel.y > 0.0)
      {
        if (rafts[i].pos.y > SCR_HEIGHT)
			rafts[i].vel.y *= -1.0;
      } else {
        if (rafts[i].pos.y < 0)
			rafts[i].vel.y *= -1.0;
      }
    }
   
    // Keep system running
    SYS_Pump();
    SYS_Sleep(17);
  }

  CORE_UnloadPNG(texBackground);
  CORE_UnloadPNG(texRaft);
  CORE_UnloadPNG(texIceCube);
  FONT_End();

  return 0;
}
