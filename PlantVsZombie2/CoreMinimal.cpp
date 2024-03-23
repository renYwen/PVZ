#include "CoreMinimal.h"
#include "Resources.h"

extern set<Object*>GameObjects{};

extern set<Component*>GameComponents{};

extern set<Object*>GameObjects_{};

extern set<Component*>GameComponents_{};

extern set<DWORD*>GameLastTimes{};

extern DWORD pauseTime = 0;
extern IMAGE pauseImage{};
extern bool pause = false;

extern bool logPattern = false;

extern bool changeLevel = false;

Vector Component::GetWorldPosition()
{
	if (parent)
		return parent->GetWorldPosition() + GetLocalPosition();
	else
	{
		if (owner)return owner->GetWorldPosition();
		else return GetLocalPosition();
	}
}

float Component::GetWorldRotation()
{
	if (parent)
		return parent->GetWorldRotation() + GetLocalRotation();
	else
	{
		if (owner)return owner->GetWorldRotation();
		else return GetLocalRotation();
	}
}

extern bool cmp(VisualComponent* a, VisualComponent* b)
{
	if (a->GetLayer() == b->GetLayer())
		return a < b;
	else
		return a->GetLayer() < b->GetLayer();
}

extern set<VisualComponent*, decltype(&cmp)> GameRenders(cmp);



IMAGE VisualComponent::RotateImage(IMAGE* pImg, double radian)
{
	radian = -radian;														// 由于 y 轴翻转，旋转角度需要变负
	float fSin = (float)sin(radian), fCos = (float)cos(radian);				// 存储三角函数值
	float fNSin = (float)sin(-radian), fNCos = (float)cos(-radian);
	int left = 0, top = 0, right = 0, bottom = 0;							// 旋转后图像顶点
	int w = pImg->getwidth(), h = pImg->getheight();
	DWORD* pBuf = GetImageBuffer(pImg);
	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };					// 存储图像顶点
	for (int j = 0; j < 4; j++)												// 旋转图像顶点，搜索旋转后的图像边界
	{
		points[j] = {
			(int)(points[j].x * fCos - points[j].y * fSin),
			(int)(points[j].x * fSin + points[j].y * fCos)
		};
		if (points[j].x < points[left].x)	left = j;
		if (points[j].y > points[top].y)	top = j;
		if (points[j].x > points[right].x)	right = j;
		if (points[j].y < points[bottom].y)	bottom = j;
	}

	int nw = points[right].x - points[left].x;								// 旋转后的图像尺寸
	int nh = points[top].y - points[bottom].y;
	int nSize = nw * nh;
	int offset_x = points[left].x < 0 ? points[left].x : 0;					// 旋转后图像超出第一象限的位移（据此调整图像位置）
	int offset_y = points[bottom].y < 0 ? points[bottom].y : 0;

	IMAGE img(nw, nh);
	DWORD* pNewBuf = GetImageBuffer(&img);


	for (int i = offset_x, ni = 0; ni < nw; ++i, ++ni)						// i 用于映射原图像坐标，ni 用于定位旋转后图像坐标
	{
		for (int j = offset_y, nj = 0; nj < nh; ++j, ++nj)
		{
			int nx = (int)(i * fNCos - j * fNSin);							// 从旋转后的图像坐标向原图像坐标映射
			int ny = (int)(i * fNSin + j * fNCos);
			if (nx >= 0 && nx < w && ny >= 0 && ny < h) {				// 若目标映射在原图像范围内，则拷贝色值
				pNewBuf[nj * nw + ni] = pBuf[ny * w + nx];
			}
		}
	}
	sprite.delta = Vector((w - nw) / 2, (h - nh) / 2);
	return img;
}

void VisualComponent::FilterImage()
{
	DWORD* pNewBuf = GetImageBuffer(copy);
	int num = sprite.image->getheight() * sprite.image->getwidth();
	for (int i = 0; i < num; ++i)						
	{
		pNewBuf[i] = BGR((((pNewBuf[i] & 0xFF0000) >> 16) + GetBValue(temp.color)) >> 2,
			(((pNewBuf[i] & 0xFF00) >> 8) + GetGValue(temp.color)) >> 2,
			((pNewBuf[i] & 0xFF) + GetRValue(temp.color)) >> 2);
	}
}

void VisualComponent::SetScale(Vector scale)
{
	if (copy)delete copy; copy = new IMAGE;
	loadimage(copy, ResourcePool.FindPath(sprite.name).c_str(), scale.x, scale.y);
	sprite.image = copy;
}


extern int RandEx(int up, int down)
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(unsigned(seed.QuadPart));
	int scoop = down - up + 1;
	scoop = scoop > 0 ? scoop : -scoop;
	return rand() % scoop + up;
}

float RandRatio()
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(unsigned(seed.QuadPart));
	return float(double(rand() % 10000) / 10000);
}

