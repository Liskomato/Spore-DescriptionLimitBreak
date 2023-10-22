// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

member_detour(UILayoutLoadDetour,UTFWin::UILayout,bool(const ResourceKey&,bool,uint32_t)) 
{
	bool detoured(const ResourceKey& key, bool boolean,uint32_t parameter) 
	{
		bool b = original_function(this, key, boolean, parameter);
		
		if (this->FindWindowByID(0xADDC11EF) != nullptr) {
			ITextEditPtr descEdit = object_cast<UTFWin::ITextEdit>(this->FindWindowByID(0xADDC11EF));
			descEdit->SetMaxTextLength(-1);
		}
		if (this->FindWindowByID(0x05415E48) != nullptr) {
			ITextEditPtr descEdit = object_cast<UTFWin::ITextEdit>(this->FindWindowByID(0x05415E48));
			descEdit->SetMaxTextLength(-1);
		}
		return b;
	}
};


void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	UILayoutLoadDetour::attach(GetAddress(UTFWin::UILayout, Load));
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

