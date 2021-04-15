#pragma once

namespace SkinChanger
{
	extern RecvVarProxyFn oModelIndexProxy;
	extern RecvVarProxyFn oSequenceProxy;

	extern WeaponId OriginalKnife;
	extern WeaponId AppliedKnife;

	extern void Hook();
	extern void UnHook();
	extern void ClearSkin(Entity* Weapon);
	extern void ForceSkin(Entity* Weapon, int PaintKit);
	extern void RunFSN();
}

