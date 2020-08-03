#pragma once

//CUSTOM
#include "Custom/Matrix3x4.hpp"
#include "Custom/Vec.hpp"
#include "Custom/Color.hpp"

//SDK
#include "SDK/Prototypes.hpp"
#include "SDK/Constants.hpp"
#include "SDK/BSPFlags.hpp"
#include "SDK/CUserCmd.hpp"
#include "SDK/ClientFrameStage_t.hpp"
#include "SDK/EFontFlags.hpp"
#include "SDK/player_info_t.hpp"
#include "SDK/RecvProp.hpp"
#include "SDK/RecvTable.hpp"
#include "SDK/ClientClass.h"
#include "SDK/WeaponData.hpp"
#include "SDK/NetChannel.hpp"
#include "SDK/Studio.hpp"
#include "SDK/Ray_t.hpp"
#include "SDK/CTraceFilter.hpp"
#include "SDK/trace_t.hpp"
#include "SDK/SurfaceData.hpp"
#include "SDK/AnimState.hpp"

//INTERFACES
#include "Interfaces/IBaseClientDLL.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/GlobalVars.hpp"
#include "Interfaces/Surface.hpp"
#include "Interfaces/Panel.hpp"
#include "Interfaces/IVModelInfo.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/PhysicsSurfaceProps.hpp"
#include "Interfaces/GameEventManager.hpp"