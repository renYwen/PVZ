#include "MenuController.h"
#include "MenuUI.h"
#include "GameStatics.h"
#include "Audio.h"
#include "BattleController.h"
#include "ZombieHand.h"

AMenuController::AMenuController()
{
	ui = GameStatics::CreateUI<UMenuUI>();
	ui->ShowOnScreen();
	ui->HideFromScreen();
	ui->Logo->SetVisibility(true);
 

	player = GameStatics::ConstructComponent<AudioPlayer>(Vector(0, 0));
	player->SetAttachment(root);
}

void AMenuController::Update()
{
	static bool flag = true;
	if (flag) {
		player->Play("CrazyDave", true); flag = false;
	}

	if (ui->NearBg->GetVisibility() && ui->NearBg->GetAbsoluteLocation().y > 40) {
		ui->NearBg->AddLocation(Vector(0, -20));
		ui->FarBg->AddLocation(Vector(0, -3));
		ui->MiddleBg->AddLocation(Vector(0, -4));
	}
	else if (ui->WoodSign->GetVisibility() && ui->WoodSign->GetAbsoluteLocation().y < 0)
		ui->WoodSign->AddLocation(Vector(0, 5));

	if (!hand) {
		if (ui->isReady == 1) {
			player->Play("menuHover");
			ui->isReady = 3;
		}
		else if (ui->isReady == 2) {
			MainController->SetEnabled(false);
			player->Close("CrazyDave");
			player->Play("riskPattern");
			hand = GameStatics::CreateObject<AZombieHand>(Vector(150, 275));
		}
	}
	else if(RiskTimeHandle.GetDelay()>5)GameStatics::OpenLevel<ABattleController>();
}
