#include "Audio_Manager.h"

Audio_Manager::Audio_Manager()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = make_unique<AudioEngine>(eflags);

	title = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\タイトル音.wav");
	slash = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\居合.wav");
	hit = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\斬撃.wav");
	attack = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\攻撃.wav");
	death = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\死亡.wav");
	guard = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\ガード音_01.wav");
	repel[0] = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\弾き音_01.wav");
	repel[1] = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\弾き音_02.wav");
	guard_set = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\チャキ音.wav");

	BGM_play_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\プレイ中BGM.wav");
	BGM_play = BGM_play_eff->CreateInstance();
	BGM_play_boss_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\ボス戦BGM.wav");
	BGM_play_boss = BGM_play_boss_eff->CreateInstance();
	BGM_title_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\タイトルBGM.wav");
	BGM_title = BGM_title_eff->CreateInstance();
}

Audio_Manager::~Audio_Manager()
{
}