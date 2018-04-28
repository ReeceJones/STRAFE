#include "Util.h"
#include "Features.h"
#include "Interfaces.h"


bool CVisuals::WorldToScreen(Vector& vOrigin, Vector& vScreen)
{
	int screenWidth, screenHeight;
	Interfaces::Engine->GetScreenSize(screenWidth, screenHeight);
	const VMatrix& worldToScreen = Interfaces::Engine->WorldToScreenMatrix();
	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
	vScreen.z = 0;
	if (w > 0.01)
	{
		float inverseWidth = 1 / w;
		vScreen.x = (screenWidth / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * inverseWidth) * screenWidth + 0.5);
		vScreen.y = (screenHeight / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * inverseWidth) * screenHeight + 0.5);
		return true;
	}
	return false;
}