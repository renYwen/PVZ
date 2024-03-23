#pragma once
#include"Renderer.h"
#include"Timer.h"
#include"Particle.h"

class ABoom :public StaticMesh
{
	Timer<ABoom>DestroyTimeHandle;
	Particle* particle;
public:
	ABoom()
	{
		DestroyTimeHandle.Bind(1.f, &ABoom::Destroy, this, false);
		renderer->Load("boom");
		renderer->SetLayer(10);

		particle = GameStatics::ConstructComponent<Particle>(Vector(50, 50));
		particle->SetAttachment(root);
		particle->SetLayer(10);
		particle->Load("mineParticles", 6);
		particle->SetCapacity(25);
		particle->SetLife(0.6f);
		particle->SetPattern(Particle::Center);
		particle->SetCenter(Vector(75,125), Vector(40, 140));
		particle->SetSpeed(2.5f);
		particle->SetGravity(10.f);
	}

	virtual void Update() { DestroyTimeHandle.Tick(); }
};