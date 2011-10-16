
#include "stdafx.h"
#include "FmodAudioDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//����һ�����ֶ���
//����˵��
//	filename:�����ļ�����
//����˵��
//	�������ɹ�,���ֽ�����״̬,���򲻿���,���Һܿ챻ɾ������
CFmodMusic::CFmodMusic(const char *filename,FSOUND_STREAM *pStream,DWORD dwMode)
:Audio(filename)
{
	assert(pStream);
	m_dwMode = dwMode;
	m_pStream=pStream;
	m_bAlive=m_pStream!=NULL;
	SetPlayingType(Audio::PLAY_NONE);
	m_nChannel=FSOUND_FREE;
	SetVolume(Audio::s_fMusicVolume);
}

//ɾ�����ֶ���
//����˵��
//	��
//����˵��
//	��
CFmodMusic::~CFmodMusic()
{
	Release(false);
}

//���Ų����ص�ǰ������ʹ�õ�channel
//����˵��
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodMusic::Play(int loops)
{	
	assert(m_pStream);
	if(m_bAlive == false)	return -1;
	m_nLoops=loops;

	m_nChannel=FSOUND_Stream_PlayEx(FSOUND_FREE,m_pStream,0,TRUE);
	if(m_nChannel == -1)
	{
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"FSOUND_Stream_PlayEx()(filename:%s) failed(ErrorInfo:%s).",
						GetFilename(),FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}

	//��������ѭ��ģʽ
	DWORD dwMode = FSOUND_Stream_GetMode(m_pStream);
	dwMode = dwMode&0xFFFFFFF0;
	if(loops==0)
		dwMode |= FSOUND_LOOP_NORMAL;
	else
		dwMode |= FSOUND_LOOP_OFF;
	if( FSOUND_Stream_SetMode(m_pStream,dwMode) == FALSE)
	{
		m_nChannel = FSOUND_FREE;
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"FSOUND_Stream_GetMode() failed(%s).",
					FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}

	if(FSOUND_SetVolume(m_nChannel,GetVolume()) == FALSE)
	{
		m_nChannel = FSOUND_FREE;
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"CFmodMusic::Play:FSOUND_SetVolume() failed(%s).",
				FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}

	SetPlayingType(loops>0?Audio::PLAY_ONCE:Audio::PLAY_LOOP);
	FSOUND_SetPaused(m_nChannel,FALSE);
	return m_nChannel;
}


//��������
//����˵��
//	time:���������ʱ���,�����ʱ����������������0����ǰ�������
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodMusic::FadeIn(unsigned int time,int loops)
{
	SetVolume(0);
	int channel=Play(loops);
	if(channel!=-1)
	{
		SetPlayingType(Audio::PLAY_FADEIN);
		m_nFadeTime=time;
		m_nFadeStartTime=GetCurTickCount();
	}
	return channel;
}

//��������
//����˵��
//	time:����������ʱ���,�����ʱ���������������ӵ�ǰ���0,֮��ֹͣ
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodMusic::FadeOut(unsigned int time,int loops)
{
	if(GetCurChannel() == -1 || IsAlive() == false )	return -1;
	//int channel=Play(loops);
	SetPlayingType(Audio::PLAY_FADEOUT);
	m_nFadeTime=time;
	m_nFadeStartTime=GetCurTickCount();
	return GetCurChannel();
}

//��������
//����˵��
//	time:����������ʱ���,�����ʱ���������������ӵ�ǰ���0,֮��ֹͣ
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodMusic::Fade(unsigned int time, float volume, int loops)
{
    if(GetCurChannel() == -1 || IsAlive() == false )	return -1;
	int VolumeTemp = GetVolume();
	m_fFadeCurVolume = (float)GetVolume()/255.0f;
	if(GetPlayingType()==Audio::PLAY_FADEOUT||GetPlayingType()==Audio::PLAY_FADEIN)
		return GetCurChannel(); 
	if(m_fFadeCurVolume>volume)
	{
		SetPlayingType(Audio::PLAY_FADEDOWN);
	}else if(m_fFadeCurVolume<volume){
		SetPlayingType(Audio::PLAY_FADEUP);
	}else{
		return GetCurChannel();
	}
	m_fFadeVolume = volume;
	m_nFadeTime=time;
	m_nFadeStartTime=GetCurTickCount();
	return GetCurChannel();
}


//ֹͣ����,ͬʱ�ͷ����ֵ���Դ,�����ֲ��ûᱻɾ��
//����˵��
//	��
//����˵��
//	��
void CFmodMusic::Stop()
{
	if(m_nChannel!=FSOUND_FREE)
	{
		if( FSOUND_StopSound(m_nChannel) == FALSE)
		{
			Log4c::Warn(ROOT_MODULE,"FSOUND_StopSound() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
		}
		m_nChannel=FSOUND_FREE;
	}
	SetPlayingType(Audio::PLAY_NONE);
	m_bAlive = false;
}

//�ͷ���������������Դ
//����˵��
//	bDelay �Ƿ��ӳ��ͷţ�����ӳ��ͷţ�ֻ��һ����Ч�ı�ǣ��Ժ���FmodEngine�ͷš�
//����˵��
//	��
void CFmodMusic::Release(bool bDelay)
{
	Stop();
	m_bAlive=false;
}

//��ȡ������ǰ��channel
//����˵��
//	��
//����˵��
//	���ص�ǰchannel,-1����Ч��channel,����������Ч��δ����
int CFmodMusic::GetCurChannel()
{
	return m_nChannel;
}

//�ж������Ƿ��ڴ��״̬,�Ǵ��״̬��ܿ�ᱻɾ���ͷ�
//����˵��
//	��
//����˵��
//	true��ʾ���������Ա�����,false���෴
bool CFmodMusic::IsAlive()
{
	return m_bAlive;
}


//�ж������Ƿ����ڲ���
//����˵��
//	��
//����˵��
//	true��ʾ�����ڲ���,false���෴
bool CFmodMusic::IsPlaying()
{
	if(m_nChannel==FSOUND_FREE)return false;
	return FSOUND_IsPlaying(m_nChannel)!=0;
}

//�������ֵĲ��ų���,��λ�Ǻ���
//����˵��
//	��
//����˵��
//	���ȵĺ�����,-1��ʾ��Ч
int CFmodMusic::GetAudioLength()
{
	if(!IsAlive())return -1;
	return FSOUND_Stream_GetLengthMs(m_pStream);
}

void CFmodMusic::Run()
{
	if( m_bAlive == false)	return;
	bool bPlaying = IsPlaying();
	bool bVolumeChange = Audio::m_bMusicVolumeChange;
	if(bVolumeChange)
	{
		SetVolume(Audio::s_fMusicVolume);
		Audio::m_bMusicVolumeChange = false;
	}

	switch(GetPlayingType())
	{
	case Audio::PLAY_ONCE:
		break;
	case Audio::PLAY_LOOP:
		break;
	case Audio::PLAY_FADEOUT:
		{
			if(bPlaying == false)	break;
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fVolume=Audio::s_fMusicVolume-time*Audio::s_fMusicVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>m_nFadeTime)
			{
				m_bAlive = false;
				Stop();
			}
		}
		break;
	case Audio::PLAY_FADEIN:
		{
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fVolume=time*Audio::s_fMusicVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>m_nFadeTime)
			{
				SetVolume(Audio::s_fMusicVolume);
				SetPlayingType(Audio::PLAY_LOOP);
				m_nLoops = 0;
			}
		}
		break;
	case Audio::PLAY_FADEDOWN:
		{
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fFadeVolume =  m_fFadeCurVolume - m_fFadeVolume;
			float fVolume=m_fFadeCurVolume-time*fFadeVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>m_nFadeTime)
			{
				SetVolume(m_fFadeVolume);
				SetPlayingType(Audio::PLAY_LOOP);
				m_nLoops = 0;
			}
		}
		break;
	case Audio::PLAY_FADEUP:
		{
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fFadeVolume = m_fFadeVolume - m_fFadeCurVolume;
			float fVolume = m_fFadeCurVolume + time*fFadeVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>m_nFadeTime)
			{
				SetVolume(m_fFadeVolume);
				SetPlayingType(Audio::PLAY_LOOP);
				m_nLoops = 0;
			}
		}
		break;
	default:
		break;
	}
	if(bPlaying == false)
		m_bAlive = false;
	if(bVolumeChange)
	{
		if(FSOUND_SetVolume(m_nChannel,GetVolume()) == FALSE)
		{
			m_nChannel = FSOUND_FREE;
			m_bAlive = false;
			Log4c::Warn(ROOT_MODULE,"CFmodMusic::Run:FSOUND_SetVolume() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
		}
	}
}

//�������ֵ�״̬
//����˵��
//	��
//����˵��
//	��
//void CFmodMusic::Update()
//{
//	switch(GetPlayingType())
//	{
//	case Audio::PLAY_ONCE:
//		SetVolume(Audio::s_fMusicVolume);
//		break;
//	case Audio::PLAY_LOOP:
//		SetVolume(Audio::s_fMusicVolume);
//		break;
//	case Audio::PLAY_FADEOUT:
//		{
//			DWORD time=GetCurTickCount()-m_nFadeStartTime;
//			if(time>m_nFadeTime)
//			{
//				SetPlayingType(Audio::PLAY_NONE);
//				SetVolume(0);
//				Stop();
//			}
//			else
//			{
//				float fVolume=Audio::s_fMusicVolume-time*Audio::s_fMusicVolume/m_nFadeTime;
//				SetVolume(fVolume);
//			}
//		}
//		break;
//	case Audio::PLAY_FADEIN:
//		{
//			DWORD time=GetCurTickCount()-m_nFadeStartTime;
//			if(time>m_nFadeTime)
//			{
//				SetPlayingType(m_nLoops<=0?Audio::PLAY_LOOP:Audio::PLAY_ONCE);
//				SetVolume(Audio::s_fMusicVolume);
//			}
//			else
//			{
//				float fVolume=time*Audio::s_fMusicVolume/m_nFadeTime;
//				SetVolume(fVolume);
//			}
//		}
//		break;
//	default:
//		break;
//	}
//	if(m_nChannel!=-1)
//	{
//		FSOUND_SetVolume(m_nChannel,GetVolume());
//	}
//}