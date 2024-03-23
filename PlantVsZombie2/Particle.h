#pragma once
#include"CoreMinimal.h"
#include<map>


//����������Ϣ
struct ParticleInfo
{
	Vector offset = Vector(0,0);//������Ӵ���������λ��
	int index = 0;//������Դʹ�����
	Vector velocity = Vector(0, 0);
	DWORD lastTime = 0;//���Ӵ���ʱ��
	float transparency = 255;
	ParticleInfo()
	{
		lastTime = timeGetTime();
	}
};



//�������
class Particle :public VisualComponent
{
public:
	//����������ʽ
	enum ParticlePattern
	{
		Center/*���ķ���״*/, Line/*�߶˷���״*/
	};

private:
	int number = 0;//��Դͼ֡��
	
	ParticlePattern pattern = Center;
	map<float,ParticleInfo>particles;
	short capacity = 1;//�����������
	
	float speed = 1.f;//�����ٶ�
	float gravity = 0;

	float lifeCycle = 1.f;//�������ڣ����ÿһ�����ӣ�
	bool cyclic = false;//�Ƿ�ѭ��
	float interval = 0;//���������Ϊ������������һ��������
	DWORD lastTime = 0;//���ڼ�¼����
	float fadingTime = 0;//����ʧʱ��

	Vector unitVector = Vector(1,0);//��λ����
	/* ���ķ���״ר����Ա */
	Vector radius = Vector(0,0);//����뾶���ھ����⾶��
	Vector scale = Vector(0,360);//���䷶Χ
	/* �߶η���״ר����Ա */
	float length = 0;//�߶γ�
	double angle = 0;//�߶η��߳���Ƕȣ�Ĭ��Ϊ���ң�
	void Produce()
	{
		ParticleInfo temp;
		temp.transparency = transparency;
		temp.index = RandEx(0,number-1);
		if (pattern == Center)
		{
			double radian = -(double)RandEx(scale.x, scale.y) * PI / 180;
			unitVector = Vector(cos(radian), sin(radian));
			if (radius!=Vector(0,0))temp.offset = (radius.x + (radius.y - radius.x)* RandRatio()) * unitVector;
			temp.velocity = speed * unitVector;
		}
		else
		{
			if (length)temp.offset = Vector(unitVector.y, unitVector.x) * length * RandRatio();
			temp.velocity = unitVector * speed;
		}
		particles.insert({ float(temp.lastTime + temp.index) + temp.offset * temp.velocity,temp});
	}
public:
	Particle() { lastTime = timeGetTime(); }
	virtual ~Particle() { ; }

	virtual void Update() {
		VisualComponent::Update();
		if (particles.size() < capacity) {
			if (interval == 0)for (int i = 0; i < capacity; i++)Produce();
			else if (timeGetTime() - lastTime >= DWORD(1000 * interval)) { Produce(); lastTime = timeGetTime(); }

			if (!cyclic && particles.size() >= capacity)capacity = -1;//��ֹ����
		}//��������

		if (particles.size()) {
			if (interval == 0) {
				if (timeGetTime() - (*particles.begin()).second.lastTime >= DWORD(1000 * lifeCycle))particles.clear();
				else if (fadingTime && float(timeGetTime() - (*particles.begin()).second.lastTime)
					>= 1000 * (lifeCycle - fadingTime)){
					for (auto& each : particles)each.second.transparency -= transparency * DELTA_TIME / fadingTime;
				}
			}
			else {
				for (auto& each : particles) {
					if (timeGetTime() - each.second.lastTime >= DWORD(1000 * lifeCycle)) {
						particles.erase(each.first); break;
					}
					else if (fadingTime && float(timeGetTime() - each.second.lastTime) >= 1000 * (lifeCycle-fadingTime))
					each.second.transparency -= transparency * DELTA_TIME /fadingTime;
				}
			}
		}
		//���ٹ���

		for (auto& each : particles) {
			if (gravity)each.second.velocity.y += gravity * DELTA_TIME;
			each.second.offset += each.second.velocity;
		}
		//���Ӳ�������
	}

	void Load(string name,int num);//ȫ�־�̬����(��Դ������һ�ź�ͼ)

	void SetCapacity(short cap) { capacity = cap;}
	void SetSpeed(float spe) { speed = spe; }
	void SetGravity(float gra) { gravity = gra; }
	void SetLife(float life) { lifeCycle = life; }
	void SetCyclic(bool cyc) { cyclic = cyc; }
	void SetInterval(float inter) { interval = inter; }
	void SetFading(float fade) { fadingTime = fade; }

	void SetPattern(ParticlePattern pat) { pattern = pat; }
	void SetCenter(Vector rad, Vector scal = Vector(0,360)) { radius = rad; scale = scal; }
	void SetLine(float len, double ang) { 
		length = len; angle = ang; 
	    unitVector = Vector(cos(-angle * PI / 180), sin(-angle * PI / 180));
	}

	void Render();
};

