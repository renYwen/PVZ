#pragma once
#include"CoreMinimal.h"


//��Ⱦ���
class SpriteRenderer :public VisualComponent
{
	bool bFetchAni = true;//��ȡ������Ϣ������У�	
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

	void Load(string name);//ȫ�־�̬����

	void SetFetchAni(bool bAni) { bFetchAni = bAni; }
	void Render();
};


//��̬������
class StaticMesh:public Object
{
protected:
	SpriteRenderer* renderer = nullptr;
public:
	StaticMesh();

	virtual void Update() { ; }

	SpriteRenderer* GetRenderer() { return renderer; }
};