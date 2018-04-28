#pragma once
#include "Util.h"
#include <iostream>

class CSetup
{
public:
	void Initialize();

private:
	void SetupInterfaces();
	void SetupHooks();
	void SetupHookManagers();
	void SetupFonts();
	void SetupTextures();
}; extern CSetup* Setup;