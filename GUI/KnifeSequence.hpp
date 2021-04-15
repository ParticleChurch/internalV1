#pragma once
#include <cstdlib>

namespace Skins
{
	namespace Sequence
	{
		enum class Generic : char {
			Unknown = 0,
			Draw,
			Idle,
			LightMiss,
			LightHit,
			LightBackstab,
			HeavyMiss,
			HeavyHit,
			HeavyBackstab,
			Inspect
		};
		extern Generic SequenceGroup1[];
		extern Generic SequenceGroup2[];
		extern Generic SequenceGroup3[];
		extern Generic SequenceGroup4[];
		extern Generic SequenceGroup5[];
		extern Generic SequenceGroup6[];
		extern Generic SequenceGroup7[];
		extern Generic SequenceGroup8[];
		extern Generic SequenceGroup9[];

		extern Generic* Nomad		  ;
		extern Generic* Skeleton	  ;
		extern Generic* Survival	  ;
		extern Generic* Paracord	  ;
		extern Generic* Classic		  ;
		extern Generic* Bowie		  ;
		extern Generic* Butterfly	  ;
		extern Generic* Falchion	  ;
		extern Generic* Flip		  ;
		extern Generic* Gut			  ;
		extern Generic* Huntsman	  ;
		extern Generic* Karambit	  ;
		extern Generic* Bayonet		  ;
		extern Generic* M9Bayonet	  ;
		extern Generic* Navaja		  ;
		extern Generic* ShadowDaggers ;
		extern Generic* Stiletto	  ;
		extern Generic* Talon		  ;
		extern Generic* Ursus		  ;
		extern Generic* TDefault	  ;
		extern Generic* CTDefault	  ;

		extern int GetSequenceFromGeneric(Generic seq, int GroupNumber);
	}
}
