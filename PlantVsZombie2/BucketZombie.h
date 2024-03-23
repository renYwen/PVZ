#pragma once
#include"BaseZombie.h"
#include"Bucket.h"

class ABucketZombie :public AZombie
{
	Animation walk;
	Animation eat;
	Animation die;
	Animation walk_1;
	Animation eat_1;
	Animation die_1;

	bool pattern = true;
public:
	ABucketZombie();
	void Update() { 
		AZombie::Update(); 
		if (blood > 0 && blood <= 10 && pattern) {
			walk.Load("normalwalk",5, 10, 46);
			eat.Load("normaleat", 4, 10, 39, Vector(-3, 0));
			walk_1.Load("normalwalk_1", 5, 10, 46);
			eat_1.Load("normaleat_1", 4, 10, 39, Vector(-3, 0));
			

			ABucket* pBucket = GameStatics::CreateObject<ABucket>(GetWorldPosition()+Vector(15,0));
			pBucket->GetRenderer()->SetLayer(row + 2);
			pattern = false;
		}
	}
};
