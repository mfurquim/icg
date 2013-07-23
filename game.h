#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "timer.h"
#include "mesh.h"
#include "map.h"
#include "camera.h"

class Game {

public:
	Game ();
	~Game ();

	int init ();
	void loop ();
	void shut_down ();


private:

	static const int SCREEN_WIDTH = 854;
	static const int SCREEN_HEIGHT = 480;
	static const int SCREEN_BPP = 16;
	static const int SCREEN_FPS = 60;

	float SCREEN_RATIO;
	float FRAME_MILISECONDS;

	int flags;
	Timer frame_time;
	SDL_Event event;

	bool quit_game;
	bool pause_game;

	string bunny_file_name;
	Mesh* bunny_mesh;

	string mapa_file_name;
	Map *mapa_architecture;

	GLdouble cam_left;
	GLdouble cam_right;
	GLdouble cam_bottom;
	GLdouble cam_top;
	GLdouble cam_zNear;
	GLdouble cam_zFar;

	float cam_eye_x;
	float cam_eye_y;
	float cam_eye_z;
	float cam_center_x;
	float cam_center_y;
	float cam_center_z;
	float cam_up_x;
	float cam_up_y;
	float cam_up_z;

	/*Used in Game Loop*/
	void load_game_resources ();
	bool is_game_finished ();
	void update_time_step ();
	void handle_events ();
	void run_physics ();
	void update ();
	void render ();
	void release_game_resources ();
	/*End Game Loop*/

	/*Used in Load Game
	Mesh* new_mesh ()
	void load_mesh (Mesh& mesh, const string& file_name)
	**End Load Game*/

	/*Used in Update Time Step
	void Timer::start ()
	**End Update Time Step*/

	/*Used in Handle Events*/
	void handle_event_type (SDL_Event& event);
	/*End Handle Events*/

	/*Used in Handle Event Type*/
	void handle_event_keydown (SDL_Event& event);
	void handle_event_keyup (SDL_Event& event);
	void handle_event_mouse_button_down (SDL_Event& event);
	void handle_event_mouse_button_up (SDL_Event& event);
	/*End Handle Event Type*/

	/*Used in Handle Event Keydown*/
	bool is_game_paused ();
	void pausing_game ();
	void unpausing_game ();
	/*End Handle Event Keydown*/

	/*Used in Render*/
	int check_if_skip ();
/*	void Mesh::render_mesh (Mesh& mesh)*/
	/*End Render*/

	/*Used in Check If Skip
	int Timer::get_ticks ()
	void Timer::wait_diff (float ratio_fps)
	**End Check If Skip*/

};

#endif