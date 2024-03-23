#pragma once
#include"CoreMinimal.h"


//¸ÕÌå£¨ÎïÀí£©
class RigidBody :public Component
{
	Vector velocity = Vector(0,0);
	bool gravityEnabled = true;
	float gravity = 10.f;
	bool movable = true;

	float angularVelocity = 0;
	bool rotatable = true;
public:
	
	virtual void Update()
	{
		if (movable)
		{
			if(gravityEnabled)
			velocity.y += gravity * DELTA_TIME;

			if(owner)owner->AddPosition(velocity);
		}
		if (rotatable)
		{
			if (owner)owner->AddRotation(angularVelocity);
		}
	}

	void EnableGravity(bool en) { gravityEnabled = en; }
	
	void SetGravity(float gra) { gravity = gra; }

	Vector GetVelocity() { return velocity; }

	void SetMovable(bool mov) { movable = mov; }

	void SetVelocity(Vector vel) { velocity = vel; }

	void SetAngularVelocity(float vel) { angularVelocity = vel; }

	void SetRotatable(bool rot) { rotatable = rot; }
};