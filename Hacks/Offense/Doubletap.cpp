#include "../../Include.hpp"

Doubletap* doubletap = new Doubletap();

//static float TicksToTime(int ticks) {
//	return I::globalvars->m_intervalPerTick * (float)(ticks);
//}

void Doubletap::DoubleTap()
{

	static Config::CFloat* ShiftAmount = Config::GetFloat("tickbase-shift");
	static Config::CFloat* RechargeAmount = Config::GetFloat("tickbase-recharge");
	shift_ticks = (int)ShiftAmount->Get();

	static bool did_shift_before = false;
	static int double_tapped = 0;
	static int prev_shift_ticks = 0;
	static bool reset = false;

	m_tick_to_shift = 0;

	
	if (CanDT())	// is choked commands okay
	{
		prev_shift_ticks = 0;

		// Can I shift 15 ticks forward and fire the weapon...
		auto can_shift_shot = CanFireWithExploit(shift_ticks);

		// Can I shift 1 tick forward and fire the weapon?
		auto can_shot = CanFireWithExploit(abs(-1 - prev_shift_ticks));

		// If I can Shift 15 ticks forward and shoot or..
		// If I can't shift one tick forward, and I haven't shifted before...
		if (can_shift_shot || (!can_shot && !did_shift_before))
		{
			prev_shift_ticks = shift_ticks;
			double_tapped = 0;
		}
		// If I can't shift 15 ticks forward and shoot
		// I can still shift forward, or I have shifted before
		else {
			double_tapped++;
			prev_shift_ticks = 0;
		}

		// If I can shift 15 ticks and shoot, or 
		// I can't shift one tick forward, and I haven't shifted before

		// AKA if I did shift before and haven't finished shifting OR
		// I haven't shifted yet?
		if (prev_shift_ticks > 0)
		{
			// Can I shift the previously shifted ticks
			if (CanFireWithExploit(prev_shift_ticks))
			{
				if (G::cmd->buttons & IN_ATTACK)
				{
					m_tick_to_shift = prev_shift_ticks;
					*G::pSendPacket = true;
					fakelag->PredictedVal = true;
					reset = true;
				}
				else {
					Entity* weap = G::LocalPlayer->GetActiveWeapon();
					if ((!(G::cmd->buttons & IN_ATTACK)) && reset
						&& fabsf(weap->GetLastShotTime() - TicksToTime(G::LocalPlayer->GetTickBase())) > 0.5f) {
						*G::pSendPacket = true;
						m_charged = false;
						m_tick_to_recharge = (int)RechargeAmount->Get();
						reset = false;
					}
				}
			}
		}
		did_shift_before = prev_shift_ticks != 0;
	}
}

bool Doubletap::CanFireWeapon(float curtime)
{
	if (!G::LocalPlayer)
		return false;

	if (!G::LocalPlayerAlive)
		return false;

	Entity* ActiveWeap = G::LocalPlayer->GetActiveWeapon();
	if (!ActiveWeap)
		return false;

	if (curtime >= ActiveWeap->GetNextPrimaryAttack())
		return true;

	return false;
}

bool Doubletap::CanFireWithExploit(int m_iShiftedTick)
{
	// curtime before shift
	float curtime = TicksToTime(G::LocalPlayer->GetTickBase() - m_iShiftedTick);
	return CanFireWeapon(curtime);
}

bool Doubletap::CanDT()
{
	return G::LocalPlayerAlive;
}

void Doubletap::start()
{
	if (!G::DoubleTap)
		return;
	
	//DT recharge
	if (G::LocalPlayer && G::LocalPlayerAlive && m_tick_to_recharge > 0 && !m_charged) {
		if (*G::pSendPacket)
			m_tick_to_recharge--;
		if (m_tick_to_recharge == 0) {
			m_charged = true;
		}
		G::cmd->tick_count = INT_MAX;
		
		*G::pSendPacket = true;
		fakelag->PredictedVal = true;
	}

	/*if (m_tick_to_shift > 0)
	{
		*G::pSendPacket = true;
		fakelag->PredictedVal = true;
	}*/
}

void Doubletap::end()
{

	DoubleTap();
}
