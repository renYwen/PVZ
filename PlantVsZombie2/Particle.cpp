#include "Particle.h"
#include "Resources.h"
#include "Camera.h"


void Particle::Load(string name, int num)
{
	sprite.image = ResourcePool.Fetch(name);
	sprite.name = name; 
	number = num;
}

void Particle::Render()
{
	Vector pos = GetWorldPosition() - MainCamera->GetWorldPosition() + sprite.delta;
	HDC dstDC = GetImageHDC(NULL);
	HDC srcDC; 
	int w = sprite.image->getwidth()/number; 
	int h = sprite.image->getheight();
	if (!filter) srcDC = GetImageHDC(sprite.image);
	else srcDC = GetImageHDC(copy);
		
	for (auto& each : particles) {
		Vector buf(pos + each.second.offset);
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, (BYTE)each.second.transparency, AC_SRC_ALPHA };
		AlphaBlend(dstDC, (int)buf.x, (int)buf.y, w, h, srcDC, each.second.index * w, 0, w, h, bf);
	}
}
