#include "Resources.h"
#include "MultiThreadMediaPlayer.h"

extern Resources ResourcePool{};




Resources::Resources()
{
	;
}

Resources::~Resources()
{
	for (auto i = imageRes.begin(); i != imageRes.end(); ++i)
		delete (*i).second;
}

void Resources::Init()
{
	/* 渲染 */

	//子弹
	Load("pea", "res/animations/bullets/pea.png", 28, 28);
	Load("icepea", "res/animations/bullets/icepea.png", 28, 28);
	Load("sun", "res/animations/bullets/sun.png", 483, 160);

	//特效
	Load("boom", "res/animations/bullets/boom.png", 131, 92);
	Load("explosion", "res/animations/bullets/explosion.png", 240, 227);
	Load("soil", "res/animations/effects/soil.png", 76, 170);
	Load("rockParticles", "res/animations/effects/RockSmall.png", 320, 40);
	Load("ash", "res/animations/zombies/ash.png", 100, 2880);
	
	Load("snowFlakes", "res/animations/effects/SnowFlakes.png", 45, 15);
	Load("snowParticles", "res/animations/effects/SnowPea_particles.png", 30, 10);
	Load("snowSplats", "res/animations/effects/SnowPea_splats.png", 88, 21);
	Load("snowPuff", "res/animations/effects/SnowPea_puff.png", 270, 45);
	Load("peaSplats", "res/animations/effects/Pea_splats.png", 88, 21);
	Load("peaParticles", "res/animations/effects/Pea_particles.png", 30, 10);
	Load("mineParticles", "res/animations/effects/PotatoMine_particles.png", 150, 30);
	Load("nutParticles", "res/animations/effects/WallnutParticlesSmall.png", 40, 8);



	//植物
	Load("cherrybomb", "res/animations/plants/cherrybomb.png", 484, 512);
	Load("doubleshooter", "res/animations/plants/doubleshooter.png", 483, 320);
	Load("iceshooter", "res/animations/plants/iceshooter.png", 71, 1065);
	Load("mine", "res/animations/plants/mine.png", 75, 440);
	Load("mine_1", "res/animations/plants/mine_1.png", 75, 55);
	Load("peashooter", "res/animations/plants/peashooter.png", 483, 320);
	Load("sunflower", "res/animations/plants/sunflower.png", 483, 320);
	Load("sunflower_1", "res/animations/plants/sunflower_1.png", 483, 320);
	Load("wallnut", "res/animations/plants/wallnut.png", 483, 480);
	Load("wallnut_1", "res/animations/plants/wallnut_1.png", 65, 803);
	Load("wallnut_2", "res/animations/plants/wallnut_2.png", 483, 480);
	Load("chomper", "res/animations/plants/chomper.png", 89, 1105);
	Load("chomper_1", "res/animations/plants/chomper_1.png", 125, 972);
	Load("chomper_2", "res/animations/plants/chomper_2.png", 72, 468);

	//僵尸
	Load("zombiedie", "res/animations/zombies/die.png", 166, 1440);
	Load("zombiedie_1", "res/animations/zombies/die_1.png", 166, 1440);

	Load("bossidle", "res/animations/zombies/boss/idle.png", 154, 2580);
	Load("bossattack", "res/animations/zombies/boss/attack.png", 154, 1760);

	Load("armorwalk", "res/animations/zombies/armor/walk.png", 139, 1584);
	Load("armoreat", "res/animations/zombies/armor/eat.png", 118, 1290);
	Load("armorwalk_1", "res/animations/zombies/armor/walk_1.png", 139, 1584);
	Load("armoreat_1", "res/animations/zombies/armor/eat_1.png", 118, 1290);
	Load("armordie", "res/animations/zombies/armor/die.png", 180, 2142);
	Load("armordie_1", "res/animations/zombies/armor/die_1.png", 180, 2142);

	Load("bucketwalk", "res/animations/zombies/bucket/walk.png", 1000, 695);
	Load("bucketeat", "res/animations/zombies/bucket/eat.png", 1000, 556);
	Load("bucketwalk_1", "res/animations/zombies/bucket/walk_1.png", 1000, 695);
	Load("bucketeat_1", "res/animations/zombies/bucket/eat_1.png", 1000, 556);
	Load("bucket", "res/animations/zombies/bucket/Zombie_bucket3.png", 50, 54);
	
	Load("normalstand", "res/animations/zombies/normal/stand.png", 166, 1584);
	Load("normalwalk", "res/animations/zombies/normal/walk.png", 1000, 695);
	Load("normaleat", "res/animations/zombies/normal/eat.png", 1000, 556);
	Load("normalwalk_1", "res/animations/zombies/normal/walk_1.png", 1000, 695);
	Load("normaleat_1", "res/animations/zombies/normal/eat_1.png", 1000, 556);

	Load("flagwalk", "res/animations/zombies/flag/walk.png", 114, 1728);
	Load("flageat", "res/animations/zombies/flag/eat.png", 109, 1584);
	Load("flagwalk_1", "res/animations/zombies/flag/walk_1.png", 114, 1728);
	Load("flageat_1", "res/animations/zombies/flag/eat_1.png", 109, 1584);
	
	Load("hatstand", "res/animations/zombies/hat/stand.png", 166, 1152);
	Load("hatwalk", "res/animations/zombies/hat/walk.png", 97, 3024);
	Load("hateat", "res/animations/zombies/hat/eat.png", 95, 1584);
	Load("hatwalk_1", "res/animations/zombies/hat/walk_1.png", 97, 3024);
	Load("hateat_1", "res/animations/zombies/hat/eat_1.png", 95, 1584);
	Load("hat", "res/animations/zombies/hat/Zombie_cone3.png", 47, 46);

	Load("jumprun", "res/animations/zombies/jump/run.png", 1616, 960);
	Load("jumprun_1", "res/animations/zombies/jump/run_1.png", 1616, 960);
	Load("jumpjump", "res/animations/zombies/jump/jump.png", 1800, 1782);
	Load("jumpjump_1", "res/animations/zombies/jump/jump_1.png", 1800, 1782);
	Load("jumpwalk", "res/animations/zombies/jump/walk.png", 900, 810);
	Load("jumpwalk_1", "res/animations/zombies/jump/walk_1.png", 900, 810);
	Load("jumpeat", "res/animations/zombies/jump/eat.png", 900, 486);
	Load("jumpeat_1", "res/animations/zombies/jump/eat_1.png", 900, 486);
	Load("jumpdie", "res/animations/zombies/jump/die.png", 162, 1764);
	Load("jumpdie_1", "res/animations/zombies/jump/die_1.png", 162, 1764);

	//其他
	Load("roll", "res/images/SodRoll.png", 68, 141);
	Load("rollCap", "res/images/SodRollCap.png", 71, 73);
	Load("bg0", "res/images/bg0.jpg", 1400, 600);
	Load("bg1", "res/images/bg1.jpg", 1400, 600);
	Load("bg3", "res/images/bg3.jpg", 1400, 600);
	Load("bg5", "res/images/bg5.jpg", 1400, 600);
	Load("car", "res/images/car.png", 70, 57);
	Load("smallShadow", "res/images/shadow.png", 25, 13);
	Load("smallShadow_1", "res/images/shadow.png", 21, 10);
	Load("bigShadow", "res/images/shadow.png", 67, 32);
	Load("zombieHand", "res/animations/effects/ZombieHand.png", 2310, 330);
	Load("head", "res/animations/zombies/head.png", 135, 2004);
	Load("head_1", "res/animations/zombies/head_1.png", 135, 2004);




	/* 媒体 */

	//背景音乐
	Load("CrazyDave", "res/sounds/CrazyDave.mp3");
	Load("Start", "res/sounds/start.mp3");
	Load("Pioneer", "res/sounds/pioneer.mp3");
	Load("ChooseSeeds", "res/sounds/chooseyourseeds.mp3");
	Load("Elite", "res/sounds/elite.mp3");
	Load("Boss", "res/sounds/boss.mp3");

	//音效
	Load("eat_finish", "res/sounds/zombieEat/eat_finish.mp3");
	Load("zombie_die", "res/sounds/zombieDie.mp3");
	Load("zombie_eat", "res/sounds/zombieEat/eat.mp3");
	Load("zombie_eat_1", "res/sounds/zombieEat/eat_1.mp3");
	Load("bucket", "res/sounds/zombie/bucket.mp3");
	Load("bucket_1", "res/sounds/zombie/bucket_1.mp3");
	Load("hat", "res/sounds/zombie/hat.mp3");
	Load("hat_1", "res/sounds/zombie/hat_1.mp3");

	Load("pea", "res/sounds/bulletAttack/pea.mp3");
	Load("freeze", "res/sounds/bulletAttack/freeze.mp3");
	Load("roll", "res/sounds/roll.mp3");

	Load("menuHover", "res/sounds/cursor/menuhover.mp3");
	Load("failCardHit", "res/sounds/cursor/failcardhit.mp3");
	Load("cardHit", "res/sounds/cursor/cardhit.mp3");
	Load("sunClick", "res/sounds/bulletAttack/sunclick.mp3");
	Load("plant", "res/sounds/cursor/plant.mp3");
	Load("scoop", "res/sounds/cursor/scoopsound.mp3");

	Load("zombieCome", "res/sounds/zombieCome.mp3");
	Load("warning", "res/sounds/warnings/warning.mp3");
	Load("bigComing", "res/sounds/warnings/bigcoming.mp3");
	Load("finalWarning", "res/sounds/warnings/finalwarning.mp3");

	Load("car", "res/sounds/car.mp3");
	Load("carCrash", "res/sounds/bulletAttack/carcrash.mp3");
	Load("bomb", "res/sounds/bulletAttack/bomb.mp3");
	Load("flower_eat", "res/sounds/bulletAttack/flowereat.mp3");

	Load("doubleShoot", "res/sounds/bulletProduce/doubleshoot.mp3");
	Load("iceShoot", "res/sounds/bulletProduce/iceshoot.mp3");


	Load("jump", "res/sounds/zombie/jump.mp3");
	Load("riskPattern", "res/sounds/riskpattern.mp3");
	Load("mineBomb", "res/sounds/bulletAttack/mineboom.mp3");
	Load("grow", "res/sounds/bulletProduce/grow.mp3");

	Load("shoot", "res/sounds/bulletProduce/shoot.mp3");
	Load("iceShoot", "res/sounds/bulletProduce/iceshoot.mp3");
	Load("produce", "res/sounds/bulletProduce/produce.mp3");

	Load("hello", "res/sounds/zombieGenerate/hello.mp3");
	Load("hello_1", "res/sounds/zombieGenerate/hello_1.mp3");
	Load("hello_2", "res/sounds/zombieGenerate/hello_2.mp3");
	Load("hello_3", "res/sounds/zombieGenerate/hello_3.mp3");
}

void Resources::Load(std::string name, std::string path, int wid, int hei)
{
	IMAGE* buf = new IMAGE;
	loadimage(buf, LPCTSTR(path.c_str()), wid, hei);
	imageRes.insert({name,buf});
	imagePath.insert({ name,path });
}

void Resources::Load(std::string name, std::string path)
{
	std::string file = std::string("open ") + path + std::string(" alias ") + name;
	MCI.SendStringA(file.c_str(), NULL, 0, NULL);
}

IMAGE* Resources::Fetch(std::string name)
{
   return (*imageRes.find(name)).second;
}

std::string Resources::FindPath(std::string name)
{
	return (*imagePath.find(name)).second;
}


