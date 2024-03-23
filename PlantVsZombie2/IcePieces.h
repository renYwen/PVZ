
#pragma once
#include"Renderer.h"
#include"Timer.h"
#include"Particle.h"
#include"Animation.h"


class AIcePieces :public StaticMesh
{
	Timer<AIcePieces>DestroyTimeHandle;
	Particle* particle;
	Animator* ani;
	Animation splat;
	int num = 250;
public:
	AIcePieces()
	{
		renderer->SetLayer(10);
		renderer->SetPosition(Vector(25, 0));

		splat.Load("snowSplats", 1, 4, 4);
		splat.SetInterval(0.05f);

		ani = GameStatics::ConstructComponent<Animator>(Vector(0, 0));
		ani->SetAttachment(root);
		ani->Insert("idle", splat);
		ani->SetNode("idle");

		DestroyTimeHandle.Bind(0.2f, &AIcePieces::Destroy, this, false);
		particle = GameStatics::ConstructComponent<Particle>(Vector(25, 15));
		particle->SetAttachment(root);
		particle->SetLayer(10);
		particle->Load("snowParticles", 3);
		particle->SetCapacity(7);
		particle->SetLife(0.2f);
		particle->SetPattern(Particle::Center);
		particle->SetSpeed(1.5f);
		particle->SetGravity(2.f);
	}

	virtual void Update() { 
		DestroyTimeHandle.Tick(); 
		if (ani->GetNode()->GetAni().index == 2)
			renderer->SetTransparency(num -= 25);
	}
};