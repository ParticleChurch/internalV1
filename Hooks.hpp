namespace H
{
	extern void Init();
	extern HookManager EndScene;
	extern HookManager Reset;

	extern long __stdcall EndSceneHook(IDirect3DDevice9* device);
	extern long __stdcall ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
}