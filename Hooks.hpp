namespace H
{
	extern void Init();
	extern void UnHook();
	extern void Eject();
	extern std::vector < std::string> console;

	//typedef
	typedef long(__stdcall* EndScene)(IDirect3DDevice9* device);
	typedef long(__stdcall* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	typedef void(__thiscall* CreateMove)(void*, float, CUserCmd*);
	typedef void(__thiscall* PaintTraverse)(void*, int, bool, bool);
	typedef void(__stdcall* FrameStageNotify)(int);
	typedef void(__thiscall* LockCursor)(void*);
	typedef bool(__thiscall* FireEventClientSide)(void*, GameEvent*);
	typedef bool(__thiscall* FireEvent)(void*, GameEvent*, bool);
    typedef void(__thiscall* hkCamToFirstPeron)(void*);
    typedef void(__thiscall* DoPostScreenEffects)(void*, int);
	typedef void(__thiscall* DrawModelExecute)(void*, void*, void*, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);

	struct SoundData {
		std::byte pad[4];
		int entityIndex;
		int channel;
		const char* soundEntry;
		std::byte pad1[8];
		float volume;
		std::byte pad2[44];
	};
	typedef void(__thiscall* EmitSound)(void*, SoundData);

	//our functions
	extern long __stdcall EndSceneHook(IDirect3DDevice9* device);
	extern long __stdcall ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall PaintTraverseHook(int vguiID, bool force, bool allowForcing);
	extern void __stdcall FrameStageNotifyHook(int curStage);
	extern void __stdcall LockCursorHook();
	extern bool __stdcall FireEventClientSideHook(GameEvent* event);
	extern bool __stdcall FireEventHook(GameEvent* event, bool bDontBroadcast);
    extern void __fastcall hkCamToFirstPeronHook();
    extern void __stdcall DoPostScreenEffectsHook(int param);
	extern void __fastcall DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);
	
	
	extern void __stdcall EmitSoundHook(SoundData data);
}