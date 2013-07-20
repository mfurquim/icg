#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	Timer ();

	void start ();
	void stop ();
	void pause ();
	void unpause ();
	void wait_diff (float ratio_fps);

	int get_ticks ();

	bool is_started ();
	bool is_paused ();

private:
	int start_ticks;
	int paused_ticks;

	bool paused;
	bool started;
};

#endif