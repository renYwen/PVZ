#pragma once
#include"Particle.h"
#include"GameStatics.h"
#include"Timer.h"

class ANutPieces :public Object
{
	Particle* particle;
	Timer<ANutPieces>DestroyTimeHandle;
public:
	ANutPieces()
	{
		DestroyTimeHandle.Bind(0.2f,&ANutPieces::Destroy,this,false);
		particle = GameStatics::ConstructComponent<Particle>(Vector(15, 15));
		particle->SetAttachment(root);
		particle->SetLayer(10);
		particle->Load("nutParticles", 5);
		particle->SetCapacity(6);
		particle->SetLife(0.2f);
		particle->SetPattern(Particle::Center);
		particle->SetSpeed(1.f);
		particle->SetGravity(2.f);
	}

	virtual void Update() {
		DestroyTimeHandle.Tick();
	}
};