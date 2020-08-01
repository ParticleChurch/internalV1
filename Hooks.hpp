namespace H
{
	extern void Init();
	extern void UnHook();
	extern void Eject();

	//typedef
	typedef long(__stdcall* EndScene)(IDirect3DDevice9* device);
	typedef long(__stdcall* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	typedef void(__thiscall* CreateMove)(void*, float, CUserCmd*);
	typedef void(__thiscall* PaintTraverse)(void*, int, bool, bool);
	typedef void(__stdcall* FrameStageNotify)(int);

	//our functions
	extern long __stdcall EndSceneHook(IDirect3DDevice9* device);
	extern long __stdcall ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern bool __stdcall CreateMoveHook(float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall PaintTraverseHook(int vguiID, bool force, bool allowForcing);
	extern void __stdcall FrameStageNotifyHook(int curStage);
}