#pragma once
#include "Renderer.h"
#include "Particle.h"


class ARoll :public StaticMesh
{
	SpriteRenderer *rollCap;
	Particle* splits[3];
	int tra = 255;
public:
	ARoll()
	{
		for (auto&arr:splits) {
			arr = GameStatics::ConstructComponent<Particle>(Vector(0, 0));
			arr->SetAttachment(root);
			arr->SetLayer(10);
			arr->Load("rockParticles", 8);
			arr->SetCapacity(8);
			arr->SetLife(0.24f);
			arr->SetFading(0.15f);
			arr->SetPattern(Particle::Center);
			arr->SetCenter(Vector(0, 0), Vector(-15, 195));
			arr->SetSpeed(1.5f);
			arr->SetGravity(2.f);
			arr->SetInterval(0.04f);
			arr->SetCyclic(true);
		}
		splits[0]->SetPosition(Vector(-5, 0));
		splits[1]->SetPosition(Vector(5, 40));
		splits[2]->SetPosition(Vector(0, 75));

		
		renderer->Load("roll");

		rollCap = GameStatics::ConstructComponent<SpriteRenderer>(Vector(0,88));
		rollCap->SetAttachment(root);
		rollCap->Load("rollCap");
	}

	virtual void Update() {
		AddPosition(Vector(4,0));
		int num = GetWorldPosition().x-8;
		renderer->SetPosition(Vector(2, num / 29));
		rollCap->SetPosition(Vector(0, 88 + num / 22));

		renderer->SetScale(Vector(68 - num / 14, 141 - num / 26));
		rollCap->SetScale(Vector(73 - num / 14, 71 - num / 14));


		if (GetWorldPosition().x > 500) 
			for (auto& arr : splits)arr->SetTransparency(tra--);
		if(GetWorldPosition().x >= 800)Destroy();
	}

	void SetLayers(int layer1,int layer2)
	{
		renderer->SetLayer(layer1); rollCap->SetLayer(layer2);
	}
};
