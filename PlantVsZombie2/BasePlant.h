#pragma once
#include"Renderer.h"
#include"Timer.h"






class APlant:public StaticMesh
{
public:
	//植物种类
	enum PlantName
	{
		Default,Shooter, Producer, Nut, Disposable
	};

protected:
	PlantName name = Default;

	int price = 0;

	class BoxCollider* box;
	SpriteRenderer* shade;
	class Animator* ani;
	class AudioPlayer* player;
	Timer<APlant>BlinkTimeHandle;

	bool enabled = false;
	int blood = 10;


	bool flag = true;//辅助enable的标记
public:
	int row = 0;
	int column = 0;

	APlant();

	virtual void Update();

	void Enabled(bool en) { enabled = en; }//启用动画
	bool GetEnable() { return enabled; }

	int GetPrice() { return price; }
	PlantName GetName() { return name; }
	void GetAttack(int harm) { blood -= harm; renderer->SetTransparency(200); BlinkTimeHandle.ReSet(); }
};
