#include "Audio_Manager.h"

Audio_Manager::Audio_Manager()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = make_unique<AudioEngine>(eflags);

	//title = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\タイトル音.wav");

	//BGM_play_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\プレイ中BGM.wav");
	//BGM_play = BGM_play_eff->CreateInstance();
}

Audio_Manager::~Audio_Manager()
{
}