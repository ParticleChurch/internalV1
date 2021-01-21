#pragma once

class Doubletap
{
public:
	int shift_ticks = 15; //add slider maybe in gui
	int				 m_tick_to_shift;
	int				 m_tick_to_recharge;
	bool			 m_charged;

	void DoubleTap();
	bool CanFireWeapon(float curtime);
	bool CanFireWithExploit(int m_iShiftedTick);
	bool CanDT();
	void start();
	void end();
};


extern Doubletap* doubletap;