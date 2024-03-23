#pragma once
#include"BaseZombie.h"


class AFlagZombie :public AZombie
{
	Animation walk;
	Animation eat;
	Animation die;
	Animation walk_1;
	Animation eat_1;
	Animation die_1;

	bool pattern = true;
public:
	AFlagZombie();
	void Update() {
		AZombie::Update();
	}
};