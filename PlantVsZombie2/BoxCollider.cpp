#include "BoxCollider.h"
#include "Controller.h"


extern set<BoxCollider*>GameColliders{};
extern bool collisionPattern = false;
extern vector<BoxCollider*>ColliderZones[8][6]{};

bool BoxCollider::IsUnderCursor()
{
	Vector pos = MainController->GetCursorWorldPosition();
	Vector loc = GetWorldPosition();
	return (pos.x<size.x + loc.x && pos.x> loc.x && 
		pos.y<size.y + loc.y && pos.y> loc.y);
}
