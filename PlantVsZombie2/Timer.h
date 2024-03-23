#pragma once
#include"CoreMinimal.h"




//计时器
template<class T>
class Timer
{
	DWORD currentTime = 0;
	DWORD lastTime = 0;
	DWORD delay = 10000;//ms
	bool bPersistant = false;//是否持续调用
	void(T::* function)() = nullptr;//绑定函数
	T* pOwner = nullptr;//绑定函数所属对象

	DWORD getDelay()
	{
		currentTime = timeGetTime();
		if (lastTime == 0) {
			lastTime = currentTime;
			return 0;
		}
		else {
			DWORD ret = currentTime - lastTime;
			return ret;
		}
	}

	bool Pulse()
	{
		if (pOwner && getDelay() >= delay) {
			if (bPersistant)lastTime = timeGetTime();
			else pOwner = nullptr;
			return true;
		}
		return false;
	}

public:
	Timer() { GameLastTimes.insert(&lastTime); }
	~Timer() { GameLastTimes.erase(&lastTime); };
	void Bind(double del, void(T::* func)(), T* owner, bool flag = false)
	{
		delay = DWORD(del * 1000);
		function = func;
		pOwner = owner;
		bPersistant = flag;
	}

	void Tick()
	{
		if (pOwner && getDelay() >= delay) {
			if (function)(pOwner->*function)();
			if (bPersistant)lastTime = timeGetTime();
			else pOwner = nullptr;
		}
	}

	double GetDelay() { return double(getDelay())/1000; }

	void SetDelay(double time) { delay = DWORD(time * 1000); }

	void ReSet() { lastTime = timeGetTime(); }
};






