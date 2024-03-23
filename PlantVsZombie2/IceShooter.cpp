#include "IceShooter.h"
#include "GameStatics.h"
#include "IcePea.h"
#include "Audio.h"
#include "BattleController.h"

AIceShooter::AIceShooter()
{
	
	price = 175;

	temp.Load("iceshooter", 15, 1, 15);
	temp.SetInterval(0.09f);
	renderer->AddPosition(Vector(5, 0));
	shade->AddPosition(Vector(0, -5));

	ani->Insert("idle", temp);
	ani->SetNode("idle");

}

void AIceShooter::Update()
{
	APlant::Update();

	if (ani->GetNode()->GetAni().index == 0)
		attackFlag = true;
	if (ani->GetNode()->GetAni().index == 1 && attackFlag) {
		Shoot(); attackFlag = false;
	}
}

void AIceShooter::Shoot()
{
	if (zombieNumPerRow[row] > 0) {
		GameStatics::CreateObject<AIcePea>(Vector(GetWorldPosition().x + 50, GetWorldPosition().y + 1));
		player->Play("iceShoot");
	}
}
