#include "Animation.h"
#include"Resources.h"



void Animation::Load(string name, int r, int c, int n, Vector delta)
{
	ani.image = ResourcePool.Fetch(name);
	ani.column = c, ani.row = r, num = n, ani.delta = delta;
}
