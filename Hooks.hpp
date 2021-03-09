namespace H
{
	extern void Init();
	extern void GUIInit();
	extern void UnHook();
	extern void Free();
	extern std::vector < std::string> console;

	//typedef
	typedef long(__stdcall* EndScene)(IDirect3DDevice9* device);
	typedef long(__stdcall* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	typedef bool(__stdcall* CreateMove)(float, CUserCmd*);
	typedef void(__thiscall* PaintTraverse)(void*, int, bool, bool);
	typedef void(__stdcall* FrameStageNotify)(int);
	typedef void(__thiscall* LockCursor)(void*);
    typedef void(__thiscall* hkCamToFirstPeron)(void*);
    typedef void(__thiscall* DoPostScreenEffects)(void*, int);
	typedef void(__thiscall* DrawModelExecute)(void*, void*, void*, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);
	typedef void(__thiscall* EmitSound)(void*, SoundData);

	extern EndScene oEndScene;
	extern Reset oReset;
	extern CreateMove oCreateMove;
	extern PaintTraverse oPaintTraverse;
	extern FrameStageNotify oFrameStageNotify;
	extern LockCursor oLockCursor;
	extern hkCamToFirstPeron ohkCamToFirstPeron;
	extern DoPostScreenEffects oDoPostScreenEffects;
	extern DrawModelExecute oDrawModelExecute;
	extern EmitSound oEmitSound;

	//our functions
	extern long __stdcall EndSceneHook(IDirect3DDevice9* device);
	extern long __stdcall ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall PaintTraverseHook(int vguiID, bool force, bool allowForcing);
	extern void __stdcall FrameStageNotifyHook(int curStage);
	extern void __stdcall LockCursorHook();
    extern void __fastcall hkCamToFirstPeronHook();
    extern void __stdcall DoPostScreenEffectsHook(int param);
	extern void __fastcall DrawModelExecuteHook(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo& info, Matrix3x4* customBoneToWorld);
	extern void __stdcall EmitSoundHook(SoundData data);


	//DT???
	typedef bool(__thiscall* WriteUsercmdDeltaToBuffer)(void*, int, void*, int, int, bool);
	extern WriteUsercmdDeltaToBuffer oWriteUsercmdDeltaToBuffer;
	extern	bool __fastcall WriteUsercmdDeltaToBufferHook(void* ecx, void* edx, int slot, void* buffer, int from, int to, bool isnewcommand);
}
