#pragma once
#include"CoreMinimal.h"


//媒体播放器
class AudioPlayer :public Component
{
	set<string>paths;
	float innerRadius = 0;
	float outerRadius = 0;
	bool bSpacial = false;//支持空间感的播放器时间开销较大，慎用
	int volume = 1000;//0~1000

	void AdjustVolume();

public:
	AudioPlayer(bool spacial = false):bSpacial(spacial) {}
	AudioPlayer(int vol, bool spacial = false):volume(vol),bSpacial(spacial) {}
	virtual ~AudioPlayer() {}

	virtual void Update() {AdjustVolume();}

	void SetRadius(float inner,float outer){
		inner = inner < 0 ? 0 : inner;
		outer = outer < 0 ? 0 : outer;
	}

	void SetSpacial(bool spacial) { bSpacial = spacial; }

	void SetVolume(int vol);

	void Play(string name, bool repeat = false);

	void Stop(string name);

	void Close(string name);

	void Pause(string name);

	void Resume(string name);
};