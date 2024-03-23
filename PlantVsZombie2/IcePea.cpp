#include "IcePea.h"
#include"Animation.h"
#include"BoxCollider.h"
#include"BaseZombie.h"
#include"GameStatics.h"
#include "Particle.h"
#include "IcePieces.h"

AIcePea::AIcePea()
{
	damage = 1;
	speed = Vector(3.f, 0);

	renderer->Load("icepea");
	renderer->SetLayer(8);
	renderer->SetFetchAni(false);

	shade = GameStatics::ConstructComponent<SpriteRenderer>(Vector(0, 50));
	shade->SetAttachment(root);
	shade->Load("smallShadow");
	shade->SetFetchAni(false);
	shade->SetTransparency(200);


	box->SetSize(Vector(25, 25));
	box->SetColor(LIGHTBLUE);

	flake = GameStatics::ConstructComponent<Particle>(Vector(0, 20));
	flake->SetAttachment(root);
	flake->SetLayer(10);
	flake->Load("snowFlakes",3);
	flake->SetCapacity(5);
	flake->SetLife(0.6f);
	flake->SetPattern(Particle::Line);
	flake->SetLine(20,180);
	flake->SetSpeed(0.6f);
	flake->SetGravity(0.3f);
	flake->SetInterval(0.15f);
	flake->SetFading(0.5f);

	puff = GameStatics::ConstructComponent<SpriteRenderer>(Vector(-25, -10));
	puff->SetAttachment(root);
	puff->SetLayer(7);
	puff->SetTransparency(75);

	snowPuff.Load("snowPuff", 1, 6, 6);
	snowPuff.SetInterval(0.08f);

	ani = GameStatics::ConstructComponent<Animator>(Vector(0, 0));
	ani->SetAttachment(root);
	ani->Insert("idle", snowPuff);
	ani->SetNode("idle");
}

void AIcePea::Update()
{
	AddPosition(speed);

	if (puff && ani->GetNode()->GetAni().index == 5)
	{
		puff->Destruct(); puff = nullptr;
	}

	if (GetWorldPosition().x > 825)Destroy();

	vector<Object*>buf = box->GetCollisions("Zombie");

	if (!buf.empty()) {
		if (AZombie* pZombie = Cast<AZombie>(buf[0])) {
			pZombie->GetAttack(damage);
			if(pZombie->GetName()!= AZombie::BossZombie)pZombie->Freeze();
			this->Destroy();
			GameStatics::CreateObject<AIcePieces>(GetWorldPosition());
		}
	}
}
