#pragma once
#include"Renderer.h"
#include"RigidBody.h"
#include"GameStatics.h"


class ABucket :public StaticMesh
{
	RigidBody* rigid;
	BYTE tran = 255;
public:
	ABucket()
	{
		rigid = GameStatics::ConstructComponent<RigidBody>(Vector(0, 0));
		rigid->SetAttachment(root);
		rigid->SetVelocity(Vector(0.5, -1.f));
		rigid->SetAngularVelocity(-1);

		renderer->Load("bucket");
	}

	virtual void Update() {
		if (rigid->GetVelocity().y > 4.25 && tran == 255)
		{
			rigid->SetRotatable(false); rigid->SetMovable(false); tran -= 5;
		}

		if (tran < 255)
		{
			renderer->SetTransparency(tran-=5);
			if (tran <= 0)Destroy();
		}
	}
};