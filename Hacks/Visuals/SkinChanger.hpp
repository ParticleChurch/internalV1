#pragma once
class SkinChanger
{
private:
	void ForceSkin(Entity* weap, int paintkit);
	void Apply(Entity* Ent);
	
public:
	std::map<int, int> CurSkins;
	std::map<std::string, int> Skins;

	void Update();

	SkinChanger();
	void SetVal(int, std::string);

	void Menu();
	void run(int stage);
};

extern SkinChanger* skinchanger;

