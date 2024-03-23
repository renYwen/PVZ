#pragma once
#include"BaseBullet.h"
#include"Animation.h"


class AIcePea :public ABullet
{
	SpriteRenderer* shade;
	class Particle* flake;
	SpriteRenderer* puff;
	Animation snowPuff;
	Animator* ani;
public:
	AIcePea();
	void Update();
};