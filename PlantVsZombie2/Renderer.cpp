#include "Renderer.h"
#include "Camera.h"
#include "GameStatics.h"
#include "Resources.h"


void SpriteRenderer::Load(string name)
{
	sprite.image = ResourcePool.Fetch(name);
	sprite.name = name;
}



void SpriteRenderer::Render()
{
	if (sprite.image) {
		Vector pos = GetWorldPosition() - MainCamera->GetWorldPosition() + sprite.delta;

		if (bEn) {
			putimage(pos.x, pos.y, en, SRCPAINT);
			putimage(pos.x, pos.y, sprite.image, SRCAND);
			return;
		}//三元光栅

		HDC dstDC = GetImageHDC(NULL);
		HDC srcDC; int w, h;
		if ((GetWorldRotation() == 0 && !filter)|| !copy) {
			srcDC = GetImageHDC(sprite.image);
			w = sprite.image->getwidth();
			h = sprite.image->getheight();
		}
		else {
			srcDC = GetImageHDC(copy);
			w = copy->getwidth();
			h = copy->getheight();
		}//旋转处理

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, transparency, AC_SRC_ALPHA };
		if (sprite.flag == 1) {
			w /= sprite.column, h /= sprite.row;
			int i = sprite.index / sprite.column;
			int j = sprite.index % sprite.column;
			AlphaBlend(dstDC, int(pos.x), int(pos.y), w, h, srcDC, j * w, i * h, w, h, bf);
		}
		else {
			if (sprite_.isUsed)
				AlphaBlend(dstDC, int(pos.x), int(pos.y), sprite_.aimSize.x, sprite_.aimSize.y,
					srcDC, sprite_.aimLoc.x, sprite_.aimLoc.y, sprite_.aimSize.x, sprite_.aimSize.y, bf);
			else AlphaBlend(dstDC, int(pos.x), int(pos.y), w, h, srcDC, 0, 0, w, h, bf);
		}
	}
}


StaticMesh::StaticMesh()
{
	renderer = GameStatics::ConstructComponent<SpriteRenderer>(Vector(0,0));
	renderer->SetAttachment(root);
}
