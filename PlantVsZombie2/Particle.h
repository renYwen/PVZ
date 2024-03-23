#pragma once
#include"CoreMinimal.h"
#include<map>


//单个粒子信息
struct ParticleInfo
{
	Vector offset = Vector(0,0);//相对粒子创建器中心位置
	int index = 0;//粒子资源使用序号
	Vector velocity = Vector(0, 0);
	DWORD lastTime = 0;//粒子创建时间
	float transparency = 255;
	ParticleInfo()
	{
		lastTime = timeGetTime();
	}
};



//粒子组件
class Particle :public VisualComponent
{
public:
	//粒子生产方式
	enum ParticlePattern
	{
		Center/*中心放射状*/, Line/*线端发射状*/
	};

private:
	int number = 0;//资源图帧数
	
	ParticlePattern pattern = Center;
	map<float,ParticleInfo>particles;
	short capacity = 1;//最大粒子容量
	
	float speed = 1.f;//粒子速度
	float gravity = 0;

	float lifeCycle = 1.f;//生命周期（针对每一个粒子）
	bool cyclic = false;//是否循环
	float interval = 0;//生产间隔（为零则所有粒子一起生产）
	DWORD lastTime = 0;//用于记录生产
	float fadingTime = 0;//逐渐消失时间

	Vector unitVector = Vector(1,0);//单位向量
	/* 中心放射状专属成员 */
	Vector radius = Vector(0,0);//放射半径（内径到外径）
	Vector scale = Vector(0,360);//放射范围
	/* 线段放射状专属成员 */
	float length = 0;//线段长
	double angle = 0;//线段法线朝向角度（默认为正右）
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

			if (!cyclic && particles.size() >= capacity)capacity = -1;//终止生产
		}//生产管理

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
		//销毁管理

		for (auto& each : particles) {
			if (gravity)each.second.velocity.y += gravity * DELTA_TIME;
			each.second.offset += each.second.velocity;
		}
		//粒子参数管理
	}

	void Load(string name,int num);//全局静态加载(资源必须是一张横图)

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

