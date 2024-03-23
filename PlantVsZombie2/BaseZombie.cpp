#include"BaseZombie.h"
#include"GameStatics.h"
#include"BoxCollider.h"
#include"Audio.h"
#include"BasePlant.h"
#include "BattleController.h"
#include "Head.h"
#include"NutPieces.h"


AZombie::AZombie()
{
	ani = GameStatics::ConstructComponent<Animator>(Vector(0, 0));
	ani->SetAttachment(root);

	ash.Load("ash", 20, 1, 20, Vector(-3, -5));
	ash.SetInterval(0.1f);
	ani->Insert("ash", ash);

	shade = GameStatics::ConstructComponent<SpriteRenderer>(Vector(20, 105));
	shade->SetAttachment(root);
	shade->Load("bigShadow");
	shade->SetFetchAni(false);
	shade->SetTransparency(225);

	box = GameStatics::ConstructComponent<BoxCollider>(Vector(20, 30));
	box->SetAttachment(root);
	box->SetType("Zombie");
	box->SetSize(Vector(35, 85));
	box->SetColor(WHITE);
	
	player = GameStatics::ConstructComponent<AudioPlayer>(Vector(0, 0));
	player->SetAttachment(root);

	EatTimeHandle.Bind(0.6f, &AZombie::Eat, this, true);
	RecoverTimeHandle.Bind(5.f, &AZombie::Recover, this, true);
}

void AZombie::Update()
{
	Judge();

	if (state==0 || state > 2)AddPosition(speed*slowFlag);

	EatTimeHandle.Tick();

	if(slowFlag==1)RecoverTimeHandle.Tick();

	if (blood <= 0)
	{
		if (state != 2) {
			state = 2;
			box->Destruct(); box = nullptr;
			if (bAsh)ani->SetNode("ash");
			else {
				AHead* pHead = GameStatics::CreateObject<AHead>(GetWorldPosition() - Vector(0,5));
				if (slowFlag == 2)ani->SetNode("die");
				else { ani->SetNode("die_1"); pHead->Func();}
				pHead->GetRenderer()->SetLayer(row + 2);
				player->Play("zombie_die");
			}
		}
		if (ani->GetNode()->GetAni().index >= ani->GetNode()->GetNum() - 1) {
			Destroy(); zombieNumPerRow[row]--;
		}
	}
		

	if (BlinkTimeHandle.GetDelay() > 0.25)
		renderer->SetTransparency(255);
}

void AZombie::Judge()
{
	if (box) {
		vector<Object*>buf = box->GetCollisions("Plant");
		collisions.clear();

		if (!buf.empty()) {
			for (auto it = buf.begin(); it != buf.end(); ++it) {

				if (APlant* pPlant = Cast<APlant>(*it)) {
					if (pPlant->GetEnable())collisions.push_back(pPlant);
				}
			}
		}

		if (!collisions.empty()) {
			if (state == 0) {
				state = 1;
				if(slowFlag==2)ani->SetNode("eat");
				else ani->SetNode("eat_1");
			}
		}
		else if(state == 1)
		{
			if (slowFlag == 2)ani->SetNode("walk");
			else ani->SetNode("walk_1");
			state = 0;
		}
	}
}

void AZombie::Eat()
{
	if (box && !collisions.empty()) {
		collisions[0]->GetAttack(1);
		if (collisions[0]->GetName() == APlant::Nut)
			GameStatics::CreateObject<ANutPieces>(GetWorldPosition() + Vector(10,25));
		int flag = RandEx(0, 5);
		if (flag>1)player->Play("zombie_eat");
		else player->Play("zombie_eat_1");
	}
}

void AZombie::GetAttack(int harm,bool boom)
{
	blood -= harm;  renderer->SetTransparency(200); BlinkTimeHandle.ReSet();
	bAsh = blood <= 0 ? boom : false;
	if (name == BucketZombie && blood>10) {
		if (RandEx(0, 7) > 2)player->Play("bucket");
		else player->Play("bucket_1");
	}
	else if (name == HatZombie && blood>10)
	{
		if (RandEx(0, 7) > 3)player->Play("hat");
		else player->Play("hat_1");
	}
	else player->Play("pea");
}

void AZombie::Recover()
{
	slowFlag = 2;
	EatTimeHandle.SetDelay(0.6f);
	if (state == 0)ani->SetNode("walk");
	else if (state == 1)ani->SetNode("eat");
	else if (state == 3)ani->SetNode("run");
}

void AZombie::Freeze()
{
	RecoverTimeHandle.ReSet();
	if (slowFlag == 2) {
		if(state==0)ani->SetNode("walk_1");
		else if(state==1)ani->SetNode("eat_1");
		else if(state==3)ani->SetNode("run_1");
		slowFlag = 1;
		player->Play("freeze");
		EatTimeHandle.SetDelay(1.2f);
	}
}



