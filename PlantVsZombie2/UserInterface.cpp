#include "UserInterface.h"
#include "Controller.h"
#include "GameStatics.h"
#include "Camera.h"


extern set<Widget*>GamePainters{};

extern set<UserInterface*>GameUIs{};

extern vector<UserInterface*>GameUIs_{};


/* Characters */

map<string, COLORREF>Characters::TextColorMap = 
{ {"$0",WHITE}/*白色*/,{"$1",RGB(245, 245, 245)}/*烟白色*/,
	{"$2",LIGHTGRAY}/*浅灰色*/,{"$3",DARKGRAY}/*深灰色*/, {"$4",BLACK}/*黑色*/,
	{"$5",GREEN}/*深绿色*/,{"$6",LIGHTGREEN}/*绿色*/, {"$7",YELLOW}/*黄色*/,
	{"$8",RGB(255, 165, 0)}/*橙色*/,{"$9",LIGHTRED}/*浅红色*/, {"$a",RED}/*深红色*/,
	{"$b",LIGHTMAGENTA}/*粉色*/,{"$c",MAGENTA}/*品红色*/, {"$d",RGB(148, 0, 211)}/*紫色*/,
	{"$e",BLUE}/*深蓝色*/,{"$f",LIGHTBLUE}/*海蓝色*/, {"$g",LIGHTCYAN}/*天蓝色*/,
	{"$h",CYAN}/*青色*/,{"$i",BROWN}/*褐色*/
};

void Characters::SetText(string te, int si, LPCTSTR ty)
{
	row = 1; column = 0;
	int temp = 0;
	for (int i = 0; i < te.size(); ++i) {
		if (te[i] == '\n') { row++, column = temp > column ? temp : column, temp = 0; }
		else if (te[i] == '$')
		{
			if (i + 1 < te.size()) {
				string buf = "$"; buf += te[i + 1];
				if (TextColorMap.find(buf) != TextColorMap.end())++i;
				else ++temp;
			}
			else ++temp;
		}
		else ++temp;
	}
	column = temp > column ? temp : column;
	texts = te;
	size = si;
	type = ty;
}

void Characters::PrintText(Vector pos, int pattern)
{
	settextstyle(6 * size, 3 * size, type);
	COLORREF color = BLACK;
	settextcolor(color);
	string temp;
	int r = 0;

	for (int i = 0; i < texts.size(); ++i) {
		if (texts[i] == '\n')
		{
			outtextxy((int)pos.x + (GetWidth() - (int)temp.size() * 3 * size) * pattern / 2,
				(int)pos.y + r * 6 * size, temp.c_str());
			temp.clear(), ++r;
		}
		else if (texts[i] == '$')
		{
			if (i + 1 < texts.size()) {
				string buf = "$"; buf += texts[i + 1];
				if (TextColorMap.find(buf) != TextColorMap.end())
				{
					color = (*TextColorMap.find(buf)).second;
					settextcolor(color);
					i++;
				}
				else temp.push_back(texts[i]);
			}
			else temp.push_back(texts[i]);
		}
		else {
			temp.push_back(texts[i]);
		}
	}
	outtextxy((int)pos.x + (GetWidth() - (int)temp.size() * 3 * size) * pattern / 2,
		(int)pos.y + r * 6 * size, temp.c_str());
	settextcolor(BLACK);
}







/* UIRenderer */

void UIRenderer::SetScale(Vector scale)
{
	if (copy)delete copy; copy = new IMAGE;
	loadimage(copy, sprite.name.c_str(), scale.x, scale.y);
	sprite.image = copy;
}

void UIRenderer::Render()
{
	if (sprite.image) {
		Vector pos =  GetLocalPosition() + sprite.delta;


		if (bEn) {
			putimage(pos.x, pos.y, en, SRCPAINT);
			putimage(pos.x, pos.y, sprite.image, SRCAND);
			return;
		}//三元光栅

		HDC dstDC = GetImageHDC(NULL);
		HDC srcDC; int w, h;
		if ((transform.rotation == 0 && !filter) || !copy) {
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

		if (sprite_.isUsed)
			AlphaBlend(dstDC, int(pos.x), int(pos.y), sprite_.aimSize.x, sprite_.aimSize.y,
				srcDC, sprite_.aimLoc.x, sprite_.aimLoc.y, sprite_.aimSize.x, sprite_.aimSize.y, bf);
		else AlphaBlend(dstDC, int(pos.x), int(pos.y), w, h, srcDC, 0, 0, w, h, bf);
	}
}



/* Widget */

void Widget::ShowInfoBox()
{
	if (IsUnderCursor() && bInfoBox) {
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
		setfillcolor(RGB(255, 247, 213));

		Vector pos = MainController->GetCursorScreenPosition() + Vector(-15, 15);
		fillrectangle(pos.x, pos.y, pos.x + InfoText.GetWidth(), pos.y + InfoText.GetHeight());
		InfoText.PrintText(pos, 1);
	}
}

bool Widget::IsUnderCursor()
{
	float x = MainController->GetCursorScreenPosition().x;
	float y = MainController->GetCursorScreenPosition().y;
	Vector loc = GetAbsoluteLocation();
	if (x<size.x + loc.x && x> loc.x && y<size.y + loc.y && y> loc.y)
	{
		return true;
	}
	return false;
}




/* Image */

Image::Image()
{
	ima = GameStatics::ConstructComponent<UIRenderer>(Vector(0,0));
	ui = new IMAGE;
	ima->SetImage(ui);
}

Image::~Image()
{
	GameComponents_.insert(static_cast<Component*>(ima));
	delete ui;
}

void Image::Update()
{
	Widget::Update(); ima->SetPosition(GetAbsoluteLocation());
}

void Image::SetVisibility(bool visible)
{
	bVisibiliy = visible;
	if (visible) { GameRenders.insert(ima); ima->SetPosition(GetAbsoluteLocation());}
	else GameRenders.erase(ima);
}

void Image::LoadPicture(string path, Vector si)
{
	Vector temp = (si == Vector(0, 0) ? GetSize() : si);
	ima->Load(path, temp.x, temp.y); ima->SetLayer(layer);
}

void Image::LoadPicture(string path, string path_en, Vector si)
{
	Vector temp = (si == Vector(0, 0) ? GetSize() : si);
	ima->Load_(path, path_en,temp.x, temp.y); ima->SetLayer(layer);
}

void Image::SetLayer(int lay)
{
	layer = lay;  if(ima)ima->SetLayer(layer);
}

BYTE Image::GetTrans()
{
	return ima->GetTransparency();
}

void Image::SetTrans(int tran)
{
	if (ima)ima->SetTransparency(tran);
}

void Image::SetPicSize(Vector si)
{
	ima->SetFixSize(si);
}

void Image::SetPicLoc(Vector loc)
{
	ima->SetFixLoc(loc);
}

void Image::SetScale(Vector scale)
{
	ima->SetScale(scale);
}

void Image::SetRotation(float rot)
{
	ima->SetRotation(rot);
}

void Image::AddRotation(float rot)
{
	ima->AddRotation(rot);
}

void Image::RotateAround(Vector center, float angle)
{
	double radian = PI * angle / 180;
	float fSin = (float)sin(-radian), fCos = (float)cos(-radian);			

	Vector relLoc = GetAbsoluteLocation() - center;

	SetLocation(Vector(relLoc.x * fCos - relLoc.y * fSin,
		relLoc.x * fSin + relLoc.y * fCos) + center - parent->GetAbsoluteLocation());
	AddRotation(angle);
}

float Image::GetRotation()
{
	return ima->GetLocalRotation();
}

bool Image::IsCursorOn()
{
	return bVisibiliy && IsUnderCursor();
}





/* Button */

Button::Button()
{
	normal.image = new IMAGE;
	hover.image = new IMAGE;
	click.image = new IMAGE;
}

Button::~Button()
{
	delete normal.image;
	delete hover.image;
	delete click.image;
}

void Button::Update()
{
	Image::Update();
	if (IsCursorClicked())ima->SetSprite(click);
	else if (IsCursorOn())ima->SetSprite(hover);
	else ima->SetSprite(normal);

	if (IsCursorClicked())clickFlag = 1;
	else if (clickFlag == 1) clickFlag = 2 - MainController->GetCursorMessage().lbutton;
	else clickFlag = 0;
}

void Button::LoadPicture(string path, Vector si)
{
	Vector temp = (si == Vector(0, 0) ? GetSize() : si);
	ima->SetLayer(layer);
	loadimage(normal.image, LPCTSTR(path.c_str()), temp.x, temp.y);
}

void Button::LoadHoverPicture(string path, Vector si)
{
	Vector temp = (si == Vector(0, 0) ? GetSize() : si); 
	ima->SetLayer(layer);
	loadimage(hover.image, LPCTSTR(path.c_str()), temp.x, temp.y);
}

void Button::LoadClickPicture(string path, Vector si)
{
	Vector temp = (si == Vector(0, 0) ? GetSize() : si); 
	ima->SetLayer(layer);
	loadimage(click.image, LPCTSTR(path.c_str()), temp.x, temp.y);
}

bool Button::IsCursorClicked()
{
	return bVisibiliy && IsUnderCursor() && MainController->GetCursorMessage().lbutton;
}

bool Button::IsCursorUp()
{
	return bVisibiliy && clickFlag == 2;
}



//#define PI 3.1415926
//
//struct Vector2D
//{
//	double x, y;
//public:
//	Vector2D(double x = 0, double y = 0) :x(x), y(y) {}
//
//
//	Vector2D operator+(const Vector2D& v) const {
//		return Vector2D(x + v.x, y + v.y);
//	}
//
//	Vector2D operator-(const Vector2D& v) const {
//		return Vector2D(x - v.x, y - v.y);
//	}
//};
//
//class Object2D
//{
//	Vector2D Location;
//	Vector2D Delta;
//	IMAGE Img;
//public:
//	void LoadPicture(string path, int w, int h) {
//		loadimage(&Img, LPCTSTR(path.c_str()), w, h);
//	}
//	void SetLocation(Vector2D loc) {
//		Location = loc;
//	}
//	void RotateImage(double angle);
//	void RotateAround(Vector2D center, double angle);
//	void Render()
//	{
//		Vector2D pos = Location + Delta;
//		HDC dstDC = GetImageHDC(NULL);
//		HDC srcDC = GetImageHDC(&Img);
//		int w = Img.getwidth();
//		int h = Img.getheight();
//		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
//		AlphaBlend(dstDC, pos.x, pos.y, w, h, srcDC, 0, 0, w, h, bf);
//	}
//};
//
//
//void Object2D::RotateImage(double angle)
//{
//	double radian = PI * angle / 180;
//	radian = -radian;														
//	float fSin = (float)sin(radian), fCos = (float)cos(radian);				
//	float fNSin = (float)sin(-radian), fNCos = (float)cos(-radian);
//	int left = 0, top = 0, right = 0, bottom = 0;							
//	int w = Img.getwidth(), h = Img.getheight();
//	DWORD* pBuf = GetImageBuffer(&Img);
//	POINT points[4] = { {0, 0}, {w, 0}, {0, h}, {w, h} };					
//	for (int j = 0; j < 4; j++)												
//	{
//		points[j] = {
//			(int)(points[j].x * fCos - points[j].y * fSin),
//			(int)(points[j].x * fSin + points[j].y * fCos)
//		};
//		if (points[j].x < points[left].x)	left = j;
//		if (points[j].y > points[top].y)	top = j;
//		if (points[j].x > points[right].x)	right = j;
//		if (points[j].y < points[bottom].y)	bottom = j;
//	}
//
//	int nw = points[right].x - points[left].x;								
//	int nh = points[top].y - points[bottom].y;
//	int nSize = nw * nh;
//	int offset_x = points[left].x < 0 ? points[left].x : 0;					
//	int offset_y = points[bottom].y < 0 ? points[bottom].y : 0;
//
//	IMAGE img(nw, nh);
//	DWORD* pNewBuf = GetImageBuffer(&img);
//
//
//	for (int i = offset_x, ni = 0; ni < nw; ++i, ++ni)						
//	{
//		for (int j = offset_y, nj = 0; nj < nh; ++j, ++nj)
//		{
//			int nx = (int)(i * fNCos - j * fNSin);							
//			int ny = (int)(i * fNSin + j * fNCos);
//			if (nx >= 0 && nx < w && ny >= 0 && ny < h) {				
//				pNewBuf[nj * nw + ni] = pBuf[ny * w + nx];
//			}
//		}
//	}
//	Img = img;
//	Delta = Vector2D((w - nw) / 2, (h - nh) / 2);
//}
//
//void Object2D::RotateAround(Vector2D center, double angle)
//{
//	double radian = PI * angle / 180;
//	double fSin = sin(-radian), fCos = cos(-radian);
//
//	Vector2D relativeLoc = Location - center;
//
//	Location = Vector2D(relativeLoc.x * fCos - relativeLoc.y * fSin,
//		relativeLoc.x * fSin + relativeLoc.y * fCos) + center;
//	RotateImage(angle);
//}