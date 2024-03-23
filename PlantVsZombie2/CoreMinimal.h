/******************************************************
 * ���Ĵ���ͷ�ļ�
 * 
 * �����ⲿͷ�ļ�������API������ö�٣������࣬�����ṹ�壬����������
 *     ���� By Arty 2024.1.19
 * 
 ******************************************************/


#pragma once
/* �ⲿͷ�ļ� */


///ͼ����Ⱦ
#include<easyx.h>//ͼ�ο�API
#include<winuser.h>
#pragma comment(lib, "MSIMG32.LIB")//����͸������

///��ý��
#include<mmsystem.h>//������ý���豸�ӿ�
#pragma comment(lib,"winmm.lib")//���ؾ�̬��


///����
#include<iostream>//I/O
#include<string>//�ַ���
#include<algorithm>//�㷨
#include<math.h>//��ѧAPI
#include<fstream>//�ļ���д

#include "Overall.h"//ȫ�ֱ���

using namespace std;



/* enum */



/* class and struct */

#define PI 3.1415926


//��ά����
struct Vector
{
	float x = 0;
	float y = 0;
	Vector() {}
	Vector(float a, float b) :x(a), y(b) {}
	static float Distance(const Vector& a,const Vector& b){
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
	static Vector RotateVector(float angle, const Vector& another) {
		double radian = PI * angle / 180;
		float fSin = (float)sin(radian), fCos = (float)cos(radian);
		return Vector(another.x * fCos - another.y * fSin,
			another.x * fSin + another.y * fCos);
	}
	Vector operator+(const Vector& another)const{
		return Vector(x + another.x, y + another.y);
	}
	Vector operator-(const Vector& another)const{
		return Vector(x - another.x, y - another.y);
	}
	Vector& operator+=(const Vector& another){
		x += another.x, y += another.y;
		return *this;
	}
	Vector& operator-=(const Vector& another) {
		x -= another.x, y -= another.y;
		return *this;
	}
	bool operator==(const Vector& another)const {
		return (another.x==x) && (another.y == y);
	}
	bool operator!=(const Vector& another)const {
		return !(*this == another);
	}
	bool operator<(const Vector& another)const {
		if (x != another.x)return x < another.x;
		else return y < another.y;
	}

	
	Vector operator*(float another)const {
		return Vector(x * another, y * another);
	}
	Vector& operator*=(float another){
		x *= another, y *= another;
		return *this;
	}

	float operator*(const Vector& another)const //���
	{
		return x * another.x+ y * another.y;
	}
};

static ostream& operator<<(ostream& out, const Vector& vec)
{
	out << "(" << vec.x << "," << vec.y << ")";
	return out;
}

static Vector operator*(float one, const Vector& another)
{
	return another*one;
}


//��ά����
struct Pair
{
	int x=0;
	int y=0;
	Pair() {}
	Pair(int i, int j) :x(i), y(j) {}
	bool operator==(const Pair& another)const {
		return (another.x == x) && (another.y == y);
	}
	bool operator<(const Pair& another)const {
		if (x == another.x)
			return y < another.y;
		return x<another.x;
	}
};


//�����任���Գ�Ա
struct Transform
{
	Vector position = Vector(0, 0);
	float rotation = 0;
	Transform() {}
	Transform(Vector pos,float rot)
		:position(pos),rotation(rot){}
};


//������Ⱦ��Ϣ
struct SpriteInfo
{
	IMAGE* image = nullptr;
	string name ="";
	Vector delta = Vector(0, 0);
	//��Դλ��ƫ��(1.���ͬһ��Ⱦ����Ⱦ��ͬ������Դλ��ƫ������  2.�����spriteͼ��ת����������ƫ������)

	int flag = 0;//������ǣ�0��ʾ�޶�����1��ʾ���Ŷ�����Դ��2��ʾ�ֿ�������Դ��

	/* ������Ϣ��Ϊһ����sprite����Դ���� */
	int row = 1;
	int column = 1;
	int index = 0;
};
//������Ⱦ��Ϣ
struct SpriteInfo_
{
	bool isUsed = false;
	Vector aimSize = Vector(0, 0);
	Vector aimLoc = Vector(0, 0);
};


//��������ࣨ���ࣩ
class Component
{
	friend class Object;
	void SetOwner(class Object* own) {
		if (!owner)owner = own; else cout << "please use SetAttachment instead" << endl;
	}
protected:
	class Object* owner = nullptr;
	Transform transform;
	Component* parent = nullptr;
	set<Component*>sons;
	void process_Destruct()
	{
		if (!sons.empty()) {
			for (auto it = sons.begin(); it != sons.end(); ++it) {
				(*it)->process_Destruct();
			}
		}
		GameComponents_.insert(this);
	}

public:
	Component() { ; }
	virtual ~Component() { ; }

	virtual void Update() { ; }

	class Object* GetOwner() { return owner; }


	Transform GetTransform() const { return transform; }

	void SetAttachment(Component* par) {
		if (parent)parent->sons.erase(this);
		parent = par;
		par->sons.insert(this);

		while (par->parent)par = par->parent;
		owner = par->owner;
	}

	void Destruct()
	{
		if (parent)parent->sons.erase(this);

		process_Destruct();
	}

	Vector GetWorldPosition();

	float GetWorldRotation();

	Vector GetLocalPosition(){return transform.position;}

	float GetLocalRotation() {return transform.rotation; }

	void SetPosition(Vector pos){ transform.position = pos;}

	void SetRotation(float rot) { transform.rotation = rot; }

	void AddPosition(Vector delta) { transform.position += delta; }
	
	void AddRotation(float delta) { transform.rotation += delta; }
};

/*֧�ֳ����任������̳�Component, ʹ��ʱ��Ҫָ��
��֧�ֳ����任��������̳�Component,ʹ��ʱ����Ҫָ��
�κζ���һ���г����任�����԰���Component���*/


//���ӻ������
class VisualComponent:public Component
{
	struct TempInfo//��ʱ�Ĵ����
	{
		float angle = 0;//��ʱ�Ĵ�Ƕ�
		COLORREF color = BLACK;//��ʱ�Ĵ���ɫ
	}temp;
	IMAGE RotateImage(IMAGE* pImg, double radian);
	//�������Ľϴ����ã�������ͼ���ⶼ֧��
	void FilterImage();
	//�������Ľϴ����ã�������ͼ���ⶼ֧��
protected:
	SpriteInfo sprite;//��Ⱦ��Ϣ
	SpriteInfo_ sprite_;//��Ⱦ����

	int layer = 0;//��Ⱦ�㼶
	BYTE transparency = 255;//͸����


	IMAGE* en = nullptr;//����ͼ�������ھ�̬ͼ��
	bool bEn = false;//ʹ������ͼ

	IMAGE* copy = nullptr;//ͼ�񱸷ݣ����ڵ���ͼ���С����ת����ɫ�˾���
	
	bool filter = false;//�Ƿ����˾�
public:
	VisualComponent() { GameRenders.insert(this); }
	virtual ~VisualComponent() { 
		GameRenders.erase(this); 
	    if (en)delete en; if (copy)delete copy;
	}

	virtual void Update()
	{
		float rotation = GetWorldRotation();
		if (rotation != temp.angle) {
			if (copy)delete copy;
			copy = new IMAGE(RotateImage(sprite.image, PI * rotation / 180));
			temp.angle = rotation;
		}
	}

	void Load_(string path, string path_en, int wid, int hei)//�ֲ���̬���أ���������ͼ��
	{
		IMAGE buf;if(en)delete en; en = new IMAGE;
		loadimage(&buf, LPCTSTR(path.c_str()), wid, hei);
		loadimage(en, LPCTSTR(path_en.c_str()), wid, hei);
		bEn = true;

		sprite.image->Resize(wid, hei);
		SetWorkingImage(sprite.image);
		putimage(0, 0, en, NOTSRCCOPY);
		putimage(0, 0, &buf, SRCPAINT);
		//��ʱ���ƣ�����ʾ����Ļ��
		SetWorkingImage(); 
	}

	virtual void SetScale(Vector scale);//�����ھ�̬ͼ��
	void SetLayer(int num) {
		GameRenders.erase(this);
		layer = num;
		GameRenders.insert(this);
	}
	int GetLayer()const { return layer; }

	void SetTransparency(BYTE tra) { transparency = tra; }
	BYTE GetTransparency() const { return transparency; }

	void SetFilter(bool open, COLORREF col = BLACK) 
	{
		filter = open; if (!filter)return;
		if (copy)delete copy; copy = new IMAGE(*sprite.image);
		temp.color = col; FilterImage(); 
	}

	void SetFixEnable(bool en) { sprite_.isUsed = en; }
	void SetFixSize(Vector size) { sprite_.isUsed = true; sprite_.aimSize = size; }
	void SetFixLoc(Vector size) { sprite_.isUsed = true; sprite_.aimLoc = size; }


	virtual void Render() = 0;
};




//���������ࣨ���ࣩ
class Object
{
protected:
	Component* root;
	Object* parent = nullptr;
	set<Object*>sons;

	SpriteInfo aniSource;//����Դ��Ϣ�����ڼ�¼�����ݶ�����Ϣ��

	void process_Destroy()
	{
		if (!sons.empty()) {
			for (auto it = sons.begin(); it != sons.end(); ++it)
				(*it)->process_Destroy();
		}
		GameObjects_.insert(this);
	}

public:
	Object() { root = new Component; root->SetOwner(this);}
	virtual ~Object() { root->Destruct(); }
	
	virtual void Update() = 0;

	const SpriteInfo& GetAniSource() { return aniSource; }
	void SetAniSource(const SpriteInfo& info) { aniSource = info; }//���ڴ��ݶ�����Ϣ

	void SetAttachment(Object* par) {
		parent = par;
		par->sons.insert(this);
	}

	void UndoAttachment(Object* par) {
		SetPosition(GetWorldPosition());
		SetRotation(GetWorldRotation());
		parent = nullptr;
		par->sons.erase(this);
	}

	void Destroy()
	{
		if (parent)parent->sons.erase(this);

		process_Destroy();
	}
	

	Vector GetWorldPosition()
	{
		if (parent)
			return parent->GetWorldPosition() + GetLocalPosition();
		else
			return GetLocalPosition();
	}
	Vector GetLocalPosition() { return root->GetLocalPosition(); }
	void SetPosition(Vector pos) { root->SetPosition(pos); }
	void AddPosition(Vector delta) { root->AddPosition(delta); }
	float GetWorldRotation()
	{
		if (parent)
			return parent->GetWorldRotation() + GetLocalRotation();
		else
			return GetLocalRotation();
	}
	float GetLocalRotation() { return root->GetLocalRotation(); }
	void SetRotation(float rot) { root->SetRotation(rot); }
	void AddRotation(float delta) { root->AddRotation(delta); }
};



/* functions */

//��ȡָ��������Χ�����
extern int RandEx(int up, int down);
//���0~1��Χ�ڵ�С����ͨ�����ڱ������㣩
extern float RandRatio();

//����ת��
template<class T>
static T* Cast(Object*base)
{
	return dynamic_cast<T*>(base);
}
template<class T>
static T* Cast(Component* base)
{
	return dynamic_cast<T*>(base);
}


//��Ϸ��ͣ
static void Pause()
{
	pauseTime = timeGetTime();
	pause = true;
	getimage(&pauseImage, 0, 0, WIN_WIDTH, WIN_HIGHT);
}

//ȡ����ͣ
static void Continue()
{
	DWORD deltaTime = timeGetTime() - pauseTime;
	for (auto it = GameLastTimes.begin(); it != GameLastTimes.end(); ++it)
	{
		*(*it) += deltaTime;
	}
	pause = false;
}