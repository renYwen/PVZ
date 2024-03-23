#include "FlagZombie.h"

AFlagZombie::AFlagZombie()
{
	name = FlagZombie;
	blood = 11;

	speed *= 2;

	walk.Load("flagwalk", 12, 1, 12, Vector(-15, -10));
	walk.SetInterval(0.08f);
	eat.Load("flageat", 11, 1, 11, Vector(-15, -10));
	eat.SetInterval(0.08f);
	die.Load("zombiedie", 10, 1, 10, Vector(-65, -15));
	die.SetInterval(0.1f);

	walk_1.Load("flagwalk_1", 12, 1, 12, Vector(-15, -10));
	walk_1.SetInterval(0.16f);
	eat_1.Load("flageat_1", 11, 1, 11, Vector(-15, -10));
	eat_1.SetInterval(0.16f);
	die_1.Load("zombiedie_1", 10, 1, 10, Vector(-65, -15));
	die_1.SetInterval(0.2f);

	ani->Insert("walk", walk);
	ani->Insert("eat", eat);
	ani->Insert("die", die);
	ani->Insert("walk_1", walk_1);
	ani->Insert("eat_1", eat_1);
	ani->Insert("die_1", die_1);
	ani->SetNode("walk");
}
