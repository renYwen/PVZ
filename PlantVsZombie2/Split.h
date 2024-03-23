#pragma once
#include"Particle.h"
#include"GameStatics.h"
#include"Timer.h"


class ASplit :public Object
{
	Particle* particle;
	Timer<ASplit>DestroyTimeHandle;
public:
	ASplit()
	{
		particle = GameStatics::ConstructComponent<Particle>(Vector(0, 0));
		particle->SetAttachment(root);
		particle->SetLayer(10);
		particle->Load("rockParticles", 8);
		particle->SetCapacity(7);
		particle->SetLife(0.25f);
		particle->SetFading(0.1f);
		particle->SetPattern(Particle::Center);
		particle->SetCenter(Vector(0, 0), Vector(15, 165));
		particle->SetSpeed(1.5f);
		particle->SetGravity(2.f);

		DestroyTimeHandle.Bind(0.25f, &ASplit::Destroy, this, false);
	}

	virtual void Update()
	{
		DestroyTimeHandle.Tick();
	}
};
