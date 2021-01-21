class Sound;

namespace I
{
	//tool for creating new interfaces
	extern InterfaceTools* NewInterface;

	//interfaces
	extern IBaseClientDLL* client;
	extern ClientMode* clientmode;
	extern GlobalVars* globalvars;
	extern IVEngineClient* engine;
	extern IClientEntityList* entitylist;
	extern Surface* surface;
	extern Panel* panel;
	extern IVModelInfo* modelinfo;
	extern IEngineTrace* enginetrace;
	extern PhysicsSurfaceProps* physicssurfaceprops;
	extern GameEventManager* gameeventmanager;
	extern InputSystem* inputsystem;
	extern CVar* cvar;
	extern CInput* input;
	extern ModelRender* modelrender;
	extern StudioRender* studiorender;
	extern MaterialSystem* materialsystem;
	extern Sound* sound;

	extern IVDebugOverlay* debugoverlay;
}