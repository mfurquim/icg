#include "game.h"
#include "mesh.h"
#include "timer.h"
#include "map.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Game::Game ()
{
	SCREEN_RATIO = (float) (SCREEN_WIDTH / SCREEN_HEIGHT);
	FRAME_MILISECONDS = (float) (1000 / SCREEN_FPS);

	flags = SDL_DOUBLEBUF | SDL_OPENGL;
	
	quit_game = false;
	pause_game = false;

	bunny_file_name = "bunny.off";
	bunny_mesh = new_mesh ();

	mapa_file_name = "arquitetura.txt";
	mapa_architecture = new_map ();

}

Game::~Game ()
{
	del_mesh (this->bunny_mesh);
	del_map (this->mapa_architecture);
}

int
Game::init ()
{
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Erro na inicializacao da SDL: %s\n", SDL_GetError ());
		return -1;
	}

	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_SetVideoMode (this->SCREEN_WIDTH, this->SCREEN_HEIGHT, this->SCREEN_BPP, this->flags) == NULL)
	{
		fprintf(stderr, "Erro na inicializacao do video: %s\n", SDL_GetError ());
		shut_down ();
		return -2;
	}

	glClearColor (0.0, 0.0, 0.0, 1.0);
	glViewport (0, 0, this->SCREEN_WIDTH, this->SCREEN_HEIGHT);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	cam_left		= -1.0;
	cam_right		= +1.0;
	cam_bottom		= -1.0;
	cam_top			= +1.0;
	cam_zNear		= 1.0;
	cam_zFar		= 7.0;

	cam_eye_x		= 0.0;
	cam_eye_y		= 0.0;
	cam_eye_z		= -2.0;
	cam_center_x	= 0.0;
	cam_center_y	= 0.0;
	cam_center_z	= 0.0;
	cam_up_x		= 0.0;
	cam_up_y		= 1.0;
	cam_up_z		= 0.0;

	glFrustum (cam_left, cam_right, cam_bottom, cam_top, cam_zNear, cam_zFar);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	gluLookAt (
		cam_eye_x, cam_eye_y, cam_eye_z,
		cam_center_x, cam_center_y, cam_center_z,
		cam_up_x, cam_up_y, cam_up_z
		);

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = {-1.5, 1.0, -4.0, 1.0};
	glLightfv (GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv (GL_LIGHT0, GL_POSITION, position);

	GLenum error = glGetError ();
	if (error != GL_NO_ERROR)
	{
		fprintf(stderr, "Error initializing OpenGL: %s\n", gluErrorString (error));
		shut_down ();
		return -3;
	}

	return 0;
}

void
Game::loop ()
{
	load_game_resources ();
	while (is_game_finished () == false)
	{
		update_time_step ();
		handle_events ();
		run_physics ();
		update ();
		render ();
	}
	release_game_resources ();

	return ;
}

void
Game::shut_down ()
{
	SDL_Quit ();

	return ;
}

void
Game::load_game_resources ()
{
	load_mesh(*this->bunny_mesh, this->bunny_file_name);
	load_map(*this->mapa_architecture, this->mapa_file_name);
	return ;
}

bool
Game::is_game_finished ()
{
	return this->quit_game;
}

void
Game::update_time_step ()
{
	check_if_skip ();
	this->frame_time.start ();

	return ;
}

void
Game::handle_events ()
{
	while (SDL_PollEvent (&event))
	{
		handle_event_type (event);
	}

	return ;
}

void
Game::handle_event_type (SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		this->quit_game = true;
		break;

	case SDL_MOUSEMOTION:
		cam_eye_x = (event.motion.x - SCREEN_WIDTH) / (SCREEN_WIDTH / 4);
		cam_eye_y = (event.motion.y - SCREEN_HEIGHT) / (SCREEN_HEIGHT / 4);
		break;

	case SDL_KEYDOWN:
		handle_event_keydown (event);
		break;

	default:
		break;
	}

	return ;
}

void
Game::handle_event_keydown (SDL_Event& event)
{
	if (is_game_paused ())
	{
		if (event.key.keysym.sym == SDLK_p)
		{
			unpausing_game ();
		}

		return ;
	}

	switch (event.key.keysym.sym)
	{
	case (SDLK_ESCAPE):
		this->quit_game = true;
		break;

	case (SDLK_q):
		this->quit_game = true;
		break;

	case (SDLK_p):
		pausing_game ();
		break;

/*
	case (SDLK_w):
		cam_eye_y		+= 0.2;
		break;

	case (SDLK_a):
		cam_eye_x		+= 0.2;
		break;

	case (SDLK_s):
		cam_eye_y		-= 0.2;
		break;

	case (SDLK_d):
		cam_eye_x		-= 0.2;
		break;
*/
	case (SDLK_w):
		cam_center_y	+= 1.0;
		break;

	case (SDLK_a):
		cam_center_x	+= 1.0;
		break;

	case (SDLK_s):
		cam_center_y	-= 1.0;
		break;

	case (SDLK_d):
		cam_center_x	-= 1.0;
		break;
	}

	return ;
}

bool
Game::is_game_paused ()
{
	return this->pause_game;
}

void
Game::pausing_game ()
{
	this->pause_game = true;

	return ;
}

void
Game::unpausing_game ()
{
	this->pause_game = false;

	return ;
}

void
Game::run_physics ()
{
	return ;
}

void
Game::update ()
{
	if (is_game_paused ())
	{
		return ;
	}

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	gluLookAt (
		cam_eye_x, cam_eye_y, cam_eye_z,
		cam_center_x, cam_center_y, cam_center_z,
		cam_up_x, cam_up_y, cam_up_z
		);

	return ;
}

void
Game::render ()
{
	if (check_if_skip () < 0)
	{
		return ;
	}

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (bunny_mesh != NULL)
	{
		render_mesh(*bunny_mesh);
	}

	if (mapa_architecture != NULL)
//		render_map(*mapa_architecture);
/*
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
//	glTranslatef (854 / 2.0, 480 / 2.0, 0.0);

	glBegin (GL_QUADS );
	
	glColor3f (0.0, 1.0, 1.0);
	
	glVertex2f (-5.0, -5.0);
	glVertex2f (5.0, -5.0);
	glVertex2f (5.0, 5.0);
	glVertex2f (-5.0, 5.0);
	
	glEnd();
*/
	SDL_GL_SwapBuffers();

	return ;
}

int
Game::check_if_skip ()
{
	if (frame_time.get_ticks () < FRAME_MILISECONDS)
	{
		frame_time.wait_diff (FRAME_MILISECONDS);
		return 0;
	}

	return -1;
}

void
Game::release_game_resources ()
{
//	del_mesh (this->bunny_mesh);
//	del_map (this->mapa_architecture);

	return ;
}