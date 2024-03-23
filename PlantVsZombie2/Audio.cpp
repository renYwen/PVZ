#include "Audio.h"
#include "Camera.h"
#include "MultiThreadMediaPlayer.h"


void AudioPlayer::AdjustVolume()
{
	if (bSpacial) {
		int vol = volume;
		float dis = Vector::Distance(MainCamera->GetWorldPosition(), GetWorldPosition());
		if (dis <= innerRadius);
		else if (dis > outerRadius)vol = 0;
		else vol = (outerRadius - dis) * volume / (outerRadius - innerRadius);

		for (auto it = paths.begin(); it != paths.end(); ++it)
		{
			string file = string("setaudio ") + (*it) + string(" volume to ") + to_string(vol);
			MCI.SendStringA(file.c_str(), NULL, 0, NULL);
		}
	}
}

void AudioPlayer::SetVolume(int vol)
{
	if (vol) {
		volume = vol;

		for (auto it = paths.begin(); it != paths.end(); ++it)
		{
			string file = string("setaudio ") + (*it) + string(" volume to ") + to_string(vol);
			MCI.SendStringA(file.c_str(), NULL, 0, NULL);
		}
	}
}

void AudioPlayer::Play(string name, bool repeat)
{
	paths.insert(name);
	MCI.SendStringA((string("seek ") + name + string(" to start")).c_str(), 0, 0, 0);

	MCI.SendStringA((string("play ") + name + (repeat ? string(" repeat") : "")).c_str(), 0, 0, 0);

	string file = string("setaudio ") + name + string(" volume to ") + to_string(volume);
	MCI.SendStringA(file.c_str(), NULL, 0, NULL);
}

void AudioPlayer::Stop(string name)
{
	MCI.SendStringA((string("stop ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Close(string name)
{
	paths.erase(name);
	MCI.SendStringA((string("close ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Pause(string name)
{
	MCI.SendStringA((string("pause ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Resume(string name)
{
	MCI.SendStringA((string("resume ") + name).c_str(), 0, 0, 0);
}
