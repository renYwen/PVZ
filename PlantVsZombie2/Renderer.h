#pragma once
#include"CoreMinimal.h"


//渲染组件
class SpriteRenderer :public VisualComponent
{
	bool bFetchAni = true;//获取动画信息（如果有）	
public:
	SpriteRenderer() {;}
	virtual ~SpriteRenderer() {;}

	virtual void Update() {
		VisualComponent::Update();
		if (bFetchAni && GetOwner() && GetOwner()->GetAniSource().flag)
		{ 
			sprite = GetOwner()->GetAniSource(); 
		}
	}

	void Load(string name);//全局静态加载

	void SetFetchAni(bool bAni) { bFetchAni = bAni; }
	void Render();
};


//静态网格类
class StaticMesh:public Object
{
protected:
	SpriteRenderer* renderer = nullptr;
public:
	StaticMesh();

	virtual void Update() { ; }

	SpriteRenderer* GetRenderer() { return renderer; }
};