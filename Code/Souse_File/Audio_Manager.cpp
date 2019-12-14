#include "Audio_Manager.h"

Audio_Manager::Audio_Manager()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = make_unique<AudioEngine>(eflags);

	title = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�^�C�g����.wav");
	slash = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\����.wav");
	hit = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�a��.wav");
	attack = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�U��.wav");
	death = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\���S.wav");
	guard = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�K�[�h��_01.wav");
	repel[0] = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�e����_01.wav");
	repel[1] = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�e����_02.wav");
	guard_set = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�`���L��.wav");

	BGM_play_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�v���C��BGM.wav");
	BGM_play = BGM_play_eff->CreateInstance();
	BGM_play_boss_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�{�X��BGM.wav");
	BGM_play_boss = BGM_play_boss_eff->CreateInstance();
	BGM_title_eff = make_unique<SoundEffect>(m_audEngine.get(), L"Audio\\�^�C�g��BGM.wav");
	BGM_title = BGM_title_eff->CreateInstance();
}

Audio_Manager::~Audio_Manager()
{
}