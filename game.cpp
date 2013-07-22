#include "game.h"
#include "mesh.h"
#include "timer.h"
#include <iostream>

Game::Game ()
{
	SCREEN_RATIO = (float) (SCREEN_WIDTH / SCREEN_HEIGHT);
	FRAME_MILISECONDS = (float) (1000 / SCREEN_FPS);

	flags = SDL_DOUBLEBUF | SDL_OPENGL;
	
	quit_game = false;
	pause_game = false;

	bunny_file_name = "bunny.off";
	bunny_mesh = new_mesh ();
}

Game::~Game ()
{
//	del_mesh (*this->bunny_mesh);
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

	GLdouble left	= -1.0;
	GLdouble right	= +1.0;
	GLdouble bottom	= -1.0;
	GLdouble top	= +1.0;
	GLdouble zNear	= 1.0;
	GLdouble zFar	= 3.0;

	float eye_x		= 0.0;
	float eye_y		= 0.0;
	float eye_z		= 1.0;
	float center_x	= 0.0;
	float center_y	= 0.0;
	float center_z	= 0.0;
	float up_x		= 0.0;
	float up_y		= 1.0;
	float up_z		= 0.0;

	glFrustum (left, right, bottom, top, zNear, zFar);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	gluLookAt (
		eye_x, eye_y, eye_z,
		center_x, center_y, center_z,
		up_x, up_y, up_z
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

	case SDL_KEYDOWN:
		handle_event_keydown (event);
		break;
/*
	case SDL_KEYUP:
		handle_event_keyup (event);
		break;

	case SDL_MOUSEBUTTONDOWN:
		handle_event_mouse_button_down (event);
		break;

	case SDL_MOUSEBUTTONUP:
		handle_event_mouse_button_up (event);
		break;
*/
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
	render_mesh(*bunny_mesh);
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
	del_mesh (this->bunny_mesh);

	return ;
}