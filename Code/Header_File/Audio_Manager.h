#pragma once
#include "Audio.h"
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>
#include <wrl.h>
#include <stdio.h>
using namespace std;

class Audio_Manager
{
public:
	unique_ptr <AudioEngine> m_audEngine;

	unique_ptr <SoundEffect> BGM_play_eff;
	unique_ptr <SoundEffect> BGM_play_boss_eff;
	unique_ptr <SoundEffect> BGM_title_eff;
	unique_ptr <SoundEffectInstance> BGM_play;
	unique_ptr <SoundEffectInstance> BGM_play_boss;
	unique_ptr <SoundEffectInstance> BGM_title;

	unique_ptr <SoundEffect> title;
	unique_ptr <SoundEffect> slash;
	unique_ptr <SoundEffect> attack;
	unique_ptr <SoundEffect> hit;
	unique_ptr <SoundEffect> repel[2];
	unique_ptr <SoundEffect> guard;
	unique_ptr <SoundEffect> guard_set;
	unique_ptr <SoundEffect> death;

	float volume = 0;

	Audio_Manager();
	~Audio_Manager();

private:

};