#pragma once
#include"CoreMinimal.h"



//文字
class Characters
{
	int row = 1;
	int column = 0;
	string texts="";
	int size = 3;
	LPCTSTR type= "楷体";

	static map<string, COLORREF>TextColorMap;

public:
	int GetWidth() { return column*3*size; }
	int GetHeight() { return row*6*size; }

	void SetCharacters(string ch) { texts = ch; }
	void SetText(string te, int si = 3, LPCTSTR ty = "楷体");

	void PrintText(Vector pos, int pattern);
};



//UI渲染组件
class UIRenderer :public VisualComponent
{
public:
	UIRenderer() { ; }
	virtual ~UIRenderer() { ; }

	virtual void Update() {
		VisualComponent::Update();
	}

	void Load(string path, int wid, int hei)//局部静态加载（耗性能,仅用于UI）
	{
		loadimage(sprite.image, LPCTSTR(path.c_str()), wid, hei);
		sprite.name = path;
	}

	void SetScale(Vector scale);
	void SetSprite(const SpriteInfo& sp) { sprite = sp; }
	void SetImage(IMAGE* buf) { sprite.image = buf; }
	void Render();
};


//基础小部件
class Widget
{
protected:
	Transform transform;
	Widget* parent = nullptr;
	set<Widget*>sons;

	Vector size = Vector(0, 0);
	bool bVisibiliy = false;
	int layer = 0;

	bool bInfoBox = false;
	Characters InfoText;

	bool IsUnderCursor();
	void ShowInfoBox();

public:
	Widget() { GamePainters.insert(this); }
	virtual ~Widget() { GamePainters.erase(this); }

	virtual void Update() { ; }

	virtual void Paint() { if(bVisibiliy)ShowInfoBox(); }

	inline void SetAttachment(Widget* par) {
		if (parent)parent->sons.erase(this);
		parent = par;
		par->sons.insert(this);
	}

	Widget* GetParent() { return parent; }
	set<Widget*> &GetSons() { return sons; }


	void SetSize(Vector si) { size = si; }
	Vector GetSize() { return size; }

	virtual void SetVisibility(bool visible) { 
		bVisibiliy = visible;
		if (visible)GamePainters.insert(this);
		else GamePainters.erase(this);
	}
	bool GetVisibility() { return bVisibiliy; }

	void SetLocation(Vector loc) { transform.position = loc; }
	Vector GetRelativeLocation() { return transform.position; }
	Vector GetAbsoluteLocation(){
		if (parent)return transform.position + parent->GetAbsoluteLocation();
		else return transform.position;
	}
	void AddLocation(Vector delta) { transform.position += delta; }

	
	void SetInfoBox(bool showInfo) { bInfoBox = showInfo; }
	void SetInfoText(string text) { bInfoBox = true; InfoText.SetText(text); }

	int GetLayer() { return layer; }

	bool operator<(Widget*another)
	{
		return this < another;
	}
};



//图像部件
class Image:public Widget
{
protected:
	UIRenderer* ima;
	IMAGE *ui;
public:
	Image();
	virtual ~Image();
	virtual void Update();
	virtual void SetVisibility(bool visible);

	virtual void LoadPicture(string path, Vector si = Vector(0, 0));
	void LoadPicture(string path, string path_en,Vector si = Vector(0, 0));//掩码图
	void SetLayer(int lay);
	BYTE GetTrans();
	void SetTrans(int tran);
	void SetPicSize(Vector si);
	void SetPicLoc(Vector loc);
	void SetScale(Vector scale);
	void SetRotation(float rot);
	void AddRotation(float rot);
	void RotateAround(Vector center,float angle);//围绕某一点旋转
	float GetRotation();

	bool IsCursorOn();
};



//按钮部件
class Button :public Image
{
	SpriteInfo normal;
	SpriteInfo hover;
	SpriteInfo click;
	int clickFlag = 0;
public:
	Button();
	virtual ~Button();

	virtual void Update();

	void LoadPicture(string path, Vector si = Vector(0, 0));
	void LoadHoverPicture(string path,Vector si = Vector(0, 0));
	void LoadClickPicture(string path, Vector si = Vector(0, 0));

	bool IsCursorClicked();
	bool IsCursorUp();
};



//文本部件
class Text :public Widget
{
protected:
	Characters text;
	int textPattern = 1;
	const string* bindedText = nullptr;
public:
	virtual void Update()
	{
		if(bindedText)
		text.SetCharacters(*bindedText);
	}
	
	virtual void Paint() { Widget::Paint();text.PrintText(GetAbsoluteLocation(), textPattern); }

	void SetPattern(int pat) { textPattern = pat; }
	void SetText(string te, int si = 3, LPCTSTR ty = "楷体") { bindedText = nullptr; text.SetText(te, si, ty); }
	void BindText(const string& origin) { bindedText = &origin; }
};





//UI类
class UserInterface
{
protected:
	set<Widget*> widgets;
	vector<Widget*> widgets_;

	void process_Remove(Widget* wig)
	{
		if (!(wig->GetSons().empty())) {
			for (auto it = wig->GetSons().begin(); it != wig->GetSons().end(); ++it)
				process_Remove((*it));
		}
		widgets_.push_back(wig);
	}
public:
	UserInterface() { ; }
	~UserInterface() { if(widgets.size())RemoveFromScreen(); }

	virtual void Update()
	{
		for (auto it = widgets.begin(); it != widgets.end(); ++it)
		{
			if ((*it)->GetVisibility())
				(*it)->Update();
		}
		if (!widgets_.empty()) {
			for (auto it = widgets_.begin(); it != widgets_.end(); ++it)
			{
				cout<< widgets.size() <<" ";
				widgets.erase(*it);
				delete* it;
				cout << widgets.size() << " ";
			}
		}
		widgets_.clear();
	}

	template<class T>
	T* AddWidget(Vector pos)
	{
		T* gameWid = new T;
		if (Widget* buf = static_cast<Widget*>(gameWid)) {
			buf->SetLocation(pos);
			widgets.insert(buf);
		}
		else {
			delete gameWid;
			gameWid = nullptr;
			cout << "Fail To Create" << endl;
		}
		return gameWid;
	}

	void RemoveWidget(Widget*wig)
	{
		if (wig->GetParent()) { wig->GetParent()->GetSons().erase(wig); }
		
		process_Remove(wig);
	}
	/* 有错！！！ */

	void ShowOnScreen()
	{
		for (auto it = widgets.begin(); it != widgets.end(); ++it)
		{
			(*it)->SetVisibility(true);
		}
	}

	void HideFromScreen()
	{
		for (auto it = widgets.begin();it != widgets.end(); ++it)
		{
			(*it)->SetVisibility(false);
		}
	}

	void RemoveFromScreen()
	{
		for (auto it = widgets.begin(); it != widgets.end(); ++it)
		{
			delete (*it);
		}
		widgets.clear();
		GameUIs_.push_back(this);
	}
};