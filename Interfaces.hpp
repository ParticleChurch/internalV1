class Sound;

namespace I
{
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
	extern ClientState* clientstate;
	extern IMemAlloc* memalloc;
	extern CEffects* effects;

	typedef void* (*InterfaceCreator)(const char*, int);
	template <typename T>
	T* GetInterface(const char* MoudleName, const char* InterfaceName)
	{
		InterfaceCreator createInterface = (InterfaceCreator)GetProcAddress(GetModuleHandleA(MoudleName), "CreateInterface");
		return (T*)createInterface(InterfaceName, 0);
	}
	extern void GUIInit();
	extern void Init();
}