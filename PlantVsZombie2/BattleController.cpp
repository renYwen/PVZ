#include "BattleController.h"
#include "BasePlant.h"
#include "BattleUI.h"
#include "GameStatics.h"
#include "Audio.h"
#include "RigidBody.h"
#include "Camera.h"

#include "PeaShooter.h"
#include "SunFlower.h"
#include "CherryBomb.h"
#include "WallNut.h"
#include "Mine.h"
#include "Chomper.h"
#include "IceShooter.h"
#include "DoubleShooter.h"

#include "Sun.h"
#include "Car.h"
#include "Split.h"
#include "Roll.h"

#include "NormalStand.h"
#include "FlagZombie.h"
#include "HatStand.h"
#include "NormalZombie.h"
#include "HatZombie.h"
#include "BucketZombie.h"
#include "ArmorZombie.h"
#include "JumpZombie.h"
#include "BossZombie.h"

extern APlant* plants[9][5]{};
extern int zombieNumPerRow[5] = { 0 };

float generateTime[80] = { 32,52,72,90,110,125,140,152,160,170,180,195,/* 第1波 */
	200,210,215,218,220,222,224,228,/* 精英僵尸 */
	240,250,258,265,275,282,290,295,300,305,310,318,322,330,335,339,345,350,355,360,365,370,375,380,/* 第2波 */
	385,395,399,401,402,403,405,408,410,415,419,423,427,430,435,440,/* 第3波 */
	445,450,455,459,460,461,463,465,466,469,470,472,475,480,487,495,500,502,510,520
};


ABattleController::ABattleController()
{
	ui = GameStatics::CreateUI<UBattleUI>();
	ui->ShowOnScreen();
	ui->StartTitle0->SetVisibility(false);
	ui->StartTitle1->SetVisibility(false);
	ui->StartTitle2->SetVisibility(false);
	ui->Level->SetVisibility(false);
	ui->EmptyBar->SetVisibility(false);
	ui->FullBar->SetVisibility(false);
	ui->Head->SetVisibility(false);
	ui->Scoop->SetVisibility(false);
	ui->Blank->SetVisibility(false);
	ui->Warning->SetVisibility(false);
	ui->FinalWarning->SetVisibility(false);
	ui->Blood->SetVisibility(false);
	ui->Blood_1->SetVisibility(false);
	
	for(int i=0;i<3;i++)ui->Flag[i]->SetVisibility(false);
		
	player = GameStatics::ConstructComponent<AudioPlayer>(Vector(0,0));
	player->SetAttachment(root);
	
	
	MainCamera->SetPosition(Vector(-225,0));

	BossGenerateTimeHandle.Bind(3.5f, &ABattleController::Generate_, this, true);
	SunTimeHandle.Bind(8.f, &ABattleController::Produce, this, true);
}

void ABattleController::Update()
{
	Controller::Update();

	/* 动画部分 */
	static StaticMesh* bg1 = nullptr;
	static StaticMesh* bg3 = nullptr;
	static AudioPlayer* player_ = nullptr;
	//开场
	if (bAniFlag)
	{
		static StaticMesh* bg0 = nullptr;
		double time = AnimationTimeHandle.GetDelay();
		if (time == 0) {
			player_ = GameStatics::ConstructComponent<AudioPlayer>(Vector(0, 0));
			player_->SetAttachment(root);
			player_->Play("ChooseSeeds");
			bg0 = GameStatics::CreateObject<StaticMesh>(Vector(-225, 0));
			bg0->GetRenderer()->Load("bg0");
			bg0->GetRenderer()->SetLayer(-8);

			
			GameStatics::CreateObject<ANormalStand>(Vector(825,50));
			GameStatics::CreateObject<ANormalStand>(Vector(750, 180));
			GameStatics::CreateObject<AHatStand>(Vector(815, 225));
			GameStatics::CreateObject<AHatStand>(Vector(975, 425));
			GameStatics::CreateObject<ANormalStand>(Vector(925, 300));
			GameStatics::CreateObject<ANormalStand>(Vector(950, 200));
			GameStatics::CreateObject<ANormalStand>(Vector(850, 400));
		}

		if (MainCamera->GetLocalPosition().x > 150)
			ui->text->SetVisibility(false);
		
		if(time >= 2.f && time < 2.1f)
			MainCamera->MoveTo(Vector(375, 0), 1.75f);
		else if (time >= 6.f && time < 6.1f)
			MainCamera->MoveTo(Vector(0, 0), 1.f);
		else if (time >= 8.f) {
			static int num = 0;
			if (rollPattern == 0) {
				
				player->Play("roll",true);
				ARoll* buf = GameStatics::CreateObject<ARoll>(Vector(3, 240));
				buf->SetLayers(-4,-3);
				bg1 = GameStatics::CreateObject<StaticMesh>(Vector(-225, 0));
				bg1->GetRenderer()->Load("bg1");
				bg1->GetRenderer()->SetLayer(-7);
				bg1->GetRenderer()->SetFixSize(Vector(235, WIN_HIGHT));
				rollPattern = 1;
			}
			else if (rollPattern == 1)
			{
				player_->SetVolume(1000-num);
				num += 4;
				bg1->GetRenderer()->SetFixSize(Vector(num + 235, WIN_HIGHT));

				if (num >= 800) { 
					bAniFlag = false; player->Close("roll");bg0->Destroy();
				}
			}
		}
		return;
	}

	double time = MainControlTimeHandle.GetDelay();
	if (time >= 75 && time<100)//三行
	{
		static int num = 0;
		if (rollPattern == 1) {
			player->Play("roll", true);
			rollPattern = 2;
			ARoll* buf = GameStatics::CreateObject<ARoll>(Vector(3, 142));
			buf->SetLayers(1, 2);
			buf = GameStatics::CreateObject<ARoll>(Vector(3, 342));
			buf->SetLayers(5, 6);
			
			bg3 = GameStatics::CreateObject<StaticMesh>(Vector(-225, 0));
			bg3->GetRenderer()->Load("bg3");
			bg3->GetRenderer()->SetLayer(-6);
			bg3->GetRenderer()->SetFixSize(Vector(235, WIN_HIGHT));
		}
		else if (rollPattern == 2)
		{
			num += 4;
			bg3->GetRenderer()->SetFixSize(Vector(num + 235, WIN_HIGHT));

			if (num >= 800) {
				player->Close("roll");bg1->Destroy(); rollPattern = 3;
			}
		}
	}
	else if (time >= 150)//五行
	{
		static StaticMesh* bg5 = nullptr;
		static int num = 0;
		if (rollPattern == 3) {
			
			player->Play("roll", true);
			rollPattern = 4;
			ARoll* buf = GameStatics::CreateObject<ARoll>(Vector(3, 42));
			buf->SetLayers(0, 1);
			buf = GameStatics::CreateObject<ARoll>(Vector(3, 442));
			buf->SetLayers(6, 7);
			
			bg5 = GameStatics::CreateObject<StaticMesh>(Vector(-225, 0));
			bg5->GetRenderer()->Load("bg5");
			bg5->GetRenderer()->SetLayer(-5);
			bg5->GetRenderer()->SetFixSize(Vector(235, WIN_HIGHT));
		}
		else if (rollPattern == 4)
		{
			num += 4;
			bg5->GetRenderer()->SetFixSize(Vector(num + 235, WIN_HIGHT));

			if (num >= 800) {
				player->Close("roll");bg3->Destroy(); rollPattern = 5;
			}
		}
	}


	/* 参数及生成控制 */

	//开场UI及音乐

	static bool flag = true;
	if (time < 1)
	{
		static int num = 200;
		player_->SetVolume(num-=2);

		
		if(ui->SeedBank->GetAbsoluteLocation().y<0)ui->SeedBank->AddLocation(Vector(0,4));

		static DWORD lasForCar = timeGetTime();
		static int numForCar = 4;
		if (timeGetTime() - lasForCar >= 75 && numForCar>=0) {
			ACar* buf = GameStatics::CreateObject<ACar>(Vector(-75, 125+100*numForCar--));
			buf->GetRenderer()->SetLayer(numForCar + 1);
			lasForCar = timeGetTime();
		}
	}
	else if (time >= 1 && time < 1.75) {
		ui->StartTitle0->SetVisibility(true); 
		if (flag) {
			player_->Close("ChooseSeeds");
			player_->Destruct();

			player->Play("Start");
			player->Play("Pioneer", true);
			flag = false;
		}
	}
	else if (time >= 1.75 && time < 2.5){
		ui->StartTitle1->SetVisibility(true);	ui->StartTitle0->SetVisibility(false);
	}
	else if(time >= 2.5 && time < 3.25){
		ui->StartTitle2->SetVisibility(true); ui->StartTitle1->SetVisibility(false);
	}
	else {
		ui->StartTitle2->SetVisibility(false);
		ui->Scoop->SetVisibility(true);
		ui->Blank->SetVisibility(true);
		ui->bStart = true;
	}
	


	//银行数字闪烁
	if (BlinkTimeHandle.GetDelay() < 0.75f && 
		(int(BlinkTimeHandle.GetDelay()*1000))%100<50)
		ui->sunNum = string("$9") + to_string(sunShine);
	else ui->sunNum = to_string(sunShine);

	if (ui->failFlag) {
		player->Play("failCardHit");
		if (ui->failFlag == 2)
		BlinkTimeHandle.ReSet();
		ui->failFlag = 0;
	}


	//天空阳光生成
	SunTimeHandle.Tick();

	
	/* 植物 */
	if (msg.lbutton)
	{
		if (ui->index >= 0 && curIndex != ui->index)
		{
			if (plant)plant->Destroy();
			curIndex = ui->index;

			switch (curIndex)
			{
			case 0:plant = GameStatics::CreateObject<APeaShooter>(GetCursorWorldPosition());
				break;
			case 1:plant = GameStatics::CreateObject<ASunFlower>(GetCursorWorldPosition());
				break;
			case 2:plant = GameStatics::CreateObject<ACherryBomb>(GetCursorWorldPosition());
				break;
			case 3:plant = GameStatics::CreateObject<AWallNut>(GetCursorWorldPosition());
				break;
			case 4:plant = GameStatics::CreateObject<AMine>(GetCursorWorldPosition());
				break;
			case 5:plant = GameStatics::CreateObject<AIceShooter>(GetCursorWorldPosition());
				break;
			case 6:plant = GameStatics::CreateObject<AChomper>(GetCursorWorldPosition());
				break;
			case 7:plant = GameStatics::CreateObject<ADoubleShooter>(GetCursorWorldPosition());
				break;
			}
			if(plant)
			plant->GetRenderer()->SetLayer(10);

			player->Play("cardHit");
		}//创建植物

		if (plant && ui->index < 0) {
			Pair coor = FindCoordinate(GetCursorWorldPosition());
			int j = 0;
			if (rollPattern >= 3 && rollPattern < 5)j = 1;
			else if (rollPattern >= 5)j = 2;
			if (coor.x >= 0 && coor.x < 9 && coor.y >= 2-j && coor.y <= 2+j)
			{
				if (!plants[coor.x][coor.y]) {
					plant->SetPosition(Vector(coor.x * 80 + 30, coor.y * 100 + 90));
					plant->Enabled(true);
					plant->column = coor.x, plant->row = coor.y;
					sunShine -= plant->GetPrice();
					plants[coor.x][coor.y] = plant;
					ui->timers[curIndex].ReSet();
					GameStatics::CreateObject<ASplit>(plant->GetWorldPosition() + Vector(25, 50));
					plant = nullptr, curIndex = -1;
					player->Play("plant");
				}
			}
			else{
				plant->Destroy(); plant = nullptr,curIndex = -1;
			}
			

			if (prePlant)
			{
				prePlant->Destroy(); prePlant = nullptr;
			}
		}//固定植物
	}

	if (ui->index < 0 && curIndex>=0) {
		Pair coor = FindCoordinate(GetCursorWorldPosition());
		int j = 0;
		if (rollPattern >= 3 && rollPattern < 5)j = 1;
		else if (rollPattern >= 5)j = 2;
		if (coor.x >= 0 && coor.x < 9 && coor.y >= 2-j && coor.y <= 2+j && !plants[coor.x][coor.y])
		{
			if (!prePlant) {
				switch (curIndex)
				{
				case 0:prePlant = GameStatics::CreateObject<APeaShooter>(GetCursorWorldPosition());
					break;
				case 1:prePlant = GameStatics::CreateObject<ASunFlower>(GetCursorWorldPosition());
					break;
				case 2:prePlant = GameStatics::CreateObject<ACherryBomb>(GetCursorWorldPosition());
					break;
				case 3:prePlant = GameStatics::CreateObject<AWallNut>(GetCursorWorldPosition());
					break;
				case 4:prePlant = GameStatics::CreateObject<AMine>(GetCursorWorldPosition());
					break;
				case 5:prePlant = GameStatics::CreateObject<AIceShooter>(GetCursorWorldPosition());
					break;
				case 6:prePlant = GameStatics::CreateObject<AChomper>(GetCursorWorldPosition());
					break;
				case 7:prePlant = GameStatics::CreateObject<ADoubleShooter>(GetCursorWorldPosition());
					break;
				}
				if (prePlant) {
					prePlant->GetRenderer()->SetLayer(0);
					prePlant->GetRenderer()->SetTransparency(100);
				}
			}
		}
		else {
			if(prePlant)prePlant->Destroy();
			prePlant = nullptr;
		}
	}//管理虚拟植物


	if (plant)
	{
		plant->SetPosition(GetCursorWorldPosition()-Vector(40,40));
	}
	if (prePlant)
	{
		Pair coor = FindCoordinate(GetCursorWorldPosition());
		int j = 0;
		if (rollPattern >= 3 && rollPattern < 5)j = 1;
		else if (rollPattern >= 5)j = 2;
		if (coor.x >= 0 && coor.x < 9 && coor.y >= 2-j && coor.y <= 2+j) 
		prePlant->SetPosition(Vector(coor.x * 80 + 30, coor.y * 100 + 90));
	}

	/* 铲子 */

	if (plantUnderScoop && *plantUnderScoop){
		(*plantUnderScoop)->GetRenderer()->SetTransparency(255);
		plantUnderScoop = nullptr;
	}//铲子所在植物高亮恢复

	if (ui->scoopFlag)
	{
		ui->Scoop->SetLocation(GetCursorWorldPosition() - Vector(25, 25));

		Pair coor = FindCoordinate(GetCursorWorldPosition());
		if (coor.x >= 0 && coor.x < 9 && coor.y >= 0 && coor.y < 5) {
			if (plants[coor.x][coor.y])
			{
				plantUnderScoop = &plants[coor.x][coor.y];
				plants[coor.x][coor.y]->GetRenderer()->SetTransparency(180);
			}
		}

		if (msg.lbutton) {
			if (coor.x >= 0 && coor.x < 9 && coor.y >= 0 && coor.y < 5) {
				if (plants[coor.x][coor.y])
				{
					plants[coor.x][coor.y]->Destroy();
					plants[coor.x][coor.y] = nullptr;
				}
				player->Play("plant");
			}
			ui->scoopFlag = false;
			ui->Scoop->SetLayer(-1);
			ui->Scoop->SetLocation(Vector(536, 0));
		}
	}
	if (ui->Blank->IsCursorOn() && msg.lbutton) {
		ui->scoopFlag = true; ui->Scoop->SetLayer(20);
		player->Play("scoop");
	}



	/* 僵尸 */

	//僵尸生成
	if (MainControlTimeHandle.GetDelay() > generateTime[zombieNum] && zombieNum<80) Generate(); 
	else if (zombieNum >= 80)BossGenerateTimeHandle.Tick();


	if (zombieNum == 80) {
		boss = GameStatics::CreateObject<ABossZombie>(Vector(650, 245));
		boss->row = 2;
		boss->GetRenderer()->SetLayer(4);
		zombieNumPerRow[2]++;

		player->Close("Elite");
		player->Play("Boss", true);
		zombieNum++;


		ui->Blood->SetVisibility(true);
		ui->Blood_1->SetVisibility(true);
	}
	if (boss && boss->GetBlood()>0) { ui->Blood_1->SetPicSize(Vector(412.f * boss->GetBlood()/1000, 24)); }
	//Boss



}


void ABattleController::Produce()
{
	int x = RandEx(100,700);
	ASun* buf = GameStatics::CreateObject<ASun>(Vector(x, 0));
	buf->line = RandEx(200, 500);
	buf->rigid->EnableGravity(false);
	buf->rigid->SetVelocity(Vector(0,0.6f));
}

void ABattleController::Generate()
{
	if (comeFlag) {
		player->Play("zombieCome");
		comeFlag = false;

		ui->Level->SetVisibility(true);
		ui->EmptyBar->SetVisibility(true);
		ui->FullBar->SetVisibility(true);
		ui->Head->SetVisibility(true);

		for (int i = 0; i < 3; i++)ui->Flag[i]->SetVisibility(true);
	}
	//僵尸首次出现

	zombieNum++;
	{
		if (zombieNum == 13) {
			player->Play("warning");
			ui->Warning->SetVisibility(true); return;
		}
		else if (zombieNum == 14)
		{
			player->Play("bigComing");
			ui->Warning->SetVisibility(false);
			ui->Flag[0]->SetLocation(ui->Flag[0]->GetAbsoluteLocation() + Vector(0, -10));
			AZombie*buf = GameStatics::CreateObject<AFlagZombie>(Vector(800, 245));
			buf->row = 2;buf->GetRenderer()->SetLayer(4);zombieNumPerRow[2]++;
		}
		/*else if (zombieNum == 21) {
			player->Close("Pioneer");
			player->Play("Elite", true);
		}*/
		else if (zombieNum == 45) {
			player->Play("warning");
			ui->Warning->SetVisibility(true); return;
		}
		else if (zombieNum == 46) {
			player->Play("bigComing");
			ui->Warning->SetVisibility(false);
			ui->Flag[1]->SetLocation(ui->Flag[1]->GetAbsoluteLocation() + Vector(0, -10));
			AZombie* buf = GameStatics::CreateObject<AFlagZombie>(Vector(800, 245));
			buf->row = 2; buf->GetRenderer()->SetLayer(4); zombieNumPerRow[2]++;
		}
		else if (zombieNum == 61) {
			player->Play("warning");
			ui->Warning->SetVisibility(true); return;
		}
		else if (zombieNum == 62) { ui->Warning->SetVisibility(false); return; }
		else if (zombieNum == 63) {
			player->Play("bigComing");
			player->Play("finalWarning");
			ui->FinalWarning->SetVisibility(true);
			ui->Flag[2]->SetLocation(ui->Flag[2]->GetAbsoluteLocation() + Vector(0, -10));
			AZombie* buf = GameStatics::CreateObject<AFlagZombie>(Vector(800, 245));
			buf->row = 2; buf->GetRenderer()->SetLayer(4); zombieNumPerRow[2]++;
		}
		else if (zombieNum == 64)ui->FinalWarning->SetVisibility(false);
	}
	//主要事件


	int length = zombieNum * 2+10;
	if (length > 157)length = 157;
	ui->FullBar->SetPicSize(Vector(length,21));
    ui->FullBar->SetPicLoc(Vector(ui->FullBar->GetSize().x - length, 0));
	ui->FullBar->SetLocation(Vector(732-length,575));
	ui->Head->SetLocation(ui->FullBar->GetAbsoluteLocation());
	//僵尸进度条更新记录

	
	int i = 2;
	if (rollPattern >= 3 && rollPattern < 5)
		i = RandEx(1, 3);
	else if (rollPattern >= 5)
		i = RandEx(0, 4);
	AZombie* buf;
	if (zombieNum < 10)
		buf = GameStatics::CreateObject<ANormalZombie>(Vector(800, i * 100 + 45));
	else if (zombieNum < 25)
	{
		int number = RandEx(0, 4);
		if (number < 3)buf = GameStatics::CreateObject<ANormalZombie>(Vector(800, i * 100 + 45));
		else buf = GameStatics::CreateObject<AHatZombie>(Vector(800, i * 100 + 45));
	}
	else if(zombieNum < 50) {
		int number = RandEx(0, 11);
	    if(number < 3)buf = GameStatics::CreateObject<ANormalZombie>(Vector(800, i * 100 + 45));
		else if (number < 6)buf = GameStatics::CreateObject<AHatZombie>(Vector(800, i * 100 + 45));
		else if(number < 9)buf = GameStatics::CreateObject<ABucketZombie>(Vector(800, i * 100 + 45));
		else buf = GameStatics::CreateObject<AJumpZombie>(Vector(800, i * 100 + 45));
	}
	else
	{
		int number = RandEx(0, 14);
		if (number < 3)buf = GameStatics::CreateObject<ANormalZombie>(Vector(800, i * 100 + 45));
		else if (number < 6)buf = GameStatics::CreateObject<AHatZombie>(Vector(800, i * 100 + 45));
		else if (number < 9)buf = GameStatics::CreateObject<ABucketZombie>(Vector(800, i * 100 + 45));
		else if (number < 12)buf = GameStatics::CreateObject<AJumpZombie>(Vector(800, i * 100 + 45));
		else buf = GameStatics::CreateObject<AArmorZombie>(Vector(800, i * 100 + 45));
	}
	buf->row = i;
	buf->GetRenderer()->SetLayer(i+2);
	zombieNumPerRow[i]++;
	//僵尸生成


	int num = RandEx(0, 10);
	switch (num)
	{
	case 0: 	player->Play("hello"); break;
	case 1: 	player->Play("hello_1"); break;
	case 2:	    player->Play("hello_2"); break;
	case 3: 	player->Play("hello_3"); break;
	}
	//随机播放僵尸出现音效
}

void ABattleController::Generate_()
{
	zombieNum++;	
	int i = RandEx(0, 4);
	AZombie* buf;
	int number = RandEx(0, 14);
	if (number < 3)buf = GameStatics::CreateObject<ANormalZombie>(Vector(800, i * 100 + 45));
	else if (number < 6)buf = GameStatics::CreateObject<AHatZombie>(Vector(800, i * 100 + 45));
	else if (number < 9)buf = GameStatics::CreateObject<ABucketZombie>(Vector(800, i * 100 + 45));
	else if (number < 12)buf = GameStatics::CreateObject<AJumpZombie>(Vector(800, i * 100 + 45));
	else buf = GameStatics::CreateObject<AArmorZombie>(Vector(800, i * 100 + 45));
	buf->row = i;
	buf->GetRenderer()->SetLayer(i + 2);
	zombieNumPerRow[i]++;
	//僵尸生成


	int num = RandEx(0, 10);
	switch (num)
	{
	case 0: 	player->Play("hello"); break;
	case 1: 	player->Play("hello_1"); break;
	case 2:	    player->Play("hello_2"); break;
	case 3: 	player->Play("hello_3"); break;
	}
	//随机播放僵尸出现音效
}
 


