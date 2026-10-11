// This file is included separately for each engine version

#define PrintConsole(msg) Union::String::Format(msg).StdPrintLine()

namespace GOTHIC_NAMESPACE
{

	void Game_Init()
	{
		PrintConsole("Gomez didn't know");
	}

	void __fastcall oCGame_Init(oCGame* self, void* vtable);
	auto Hook_oCGame_Init = Union::CreateHook(SIGNATURE_OF(&oCGame::Init), &oCGame_Init, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Init(oCGame* self, void* vtable)
	{
		Hook_oCGame_Init(self, vtable);
		Game_Init();
	}
}
