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
	radian = -radian;														// ���� y �ᷭת����ת�Ƕ���Ҫ�为
	float fSin = (float)sin(radian), fCos = (float)cos(radian);				// �洢���Ǻ���ֵ
	float fNSin = (float)sin(-radian), fNCos = (float)cos(-radian);
	int left = 0, top = 0, right = 0, bottom = 0;							// ��ת��ͼ�񶥵�
	int w = pImg->getwidth(), h = pImg->getheight();
	DWORD* pBuf = GetImageBuffer(pImg);
	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };					// �洢ͼ�񶥵�
	for (int j = 0; j < 4; j++)												// ��תͼ�񶥵㣬������ת���ͼ��߽�
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

	int nw = points[right].x - points[left].x;								// ��ת���ͼ��ߴ�
	int nh = points[top].y - points[bottom].y;
	int nSize = nw * nh;
	int offset_x = points[left].x < 0 ? points[left].x : 0;					// ��ת��ͼ�񳬳���һ���޵�λ�ƣ��ݴ˵���ͼ��λ�ã�
	int offset_y = points[bottom].y < 0 ? points[bottom].y : 0;

	IMAGE img(nw, nh);
	DWORD* pNewBuf = GetImageBuffer(&img);


	for (int i = offset_x, ni = 0; ni < nw; ++i, ++ni)						// i ����ӳ��ԭͼ�����꣬ni ���ڶ�λ��ת��ͼ������
	{
		for (int j = offset_y, nj = 0; nj < nh; ++j, ++nj)
		{
			int nx = (int)(i * fNCos - j * fNSin);							// ����ת���ͼ��������ԭͼ������ӳ��
			int ny = (int)(i * fNSin + j * fNCos);
			if (nx >= 0 && nx < w && ny >= 0 && ny < h) {				// ��Ŀ��ӳ����ԭͼ��Χ�ڣ��򿽱�ɫֵ
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

