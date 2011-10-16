
#include "stdafx.h"
#include "FmodAudioDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//����һ����������
//����˵��
//	filename:�����ļ�����
//	pSample:��ʼ����,һ�������ܱ������������ʹ��
//	b2d:�Ƿ���2D����
//����˵��
//	����ɹ�����,����������״̬,���������Ǵ��״̬,������
CFmodSpeech::CFmodSpeech(const char *filename,FSOUND_SAMPLE *pSample,DWORD dwMode)
:Audio(filename)
{
	m_dwMode = dwMode;
	//m_b3D=b3D;
	m_nLoops = 1;
	m_pSample=pSample;
	assert(m_pSample);
	if(m_pSample)
	{
		m_bAlive=m_pSample!=NULL;
		m_nChannel=FSOUND_FREE;
		SetPlayingType(Audio::PLAY_NONE);
		SetVolume(Audio::s_fSpeechVolume);
	}
}

//ɾ����������
//����˵��
//	��
//����˵��
//	��
CFmodSpeech::~CFmodSpeech()
{
	Release(false);
}

//�ͷ���������������Դ
//����˵��
//	bDelay �Ƿ��ӳ��ͷţ�����ӳ��ͷţ�ֻ��һ����Ч�ı�ǣ��Ժ���FmodEngine�ͷš�
//����˵��
//	��
void CFmodSpeech::Release(bool bDelay)
{
	Stop();
	m_bAlive=false;
}

//�������������ص�ǰ������ʹ�õ�channel
//����˵��
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodSpeech::Play(int loops)
{
	assert(m_pSample);
	m_nLoops=loops;
	m_nChannel=FSOUND_PlaySoundEx(FSOUND_FREE,m_pSample,0,TRUE);	
	//����
	if(m_nChannel==-1)
	{
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"FSOUND_PlaySoundEx() failed(filename:%s,ModeType:%d,info:%s).",
						GetFilename(),GetMode(),FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}

	//����ʧ��
	if( FSOUND_SetLoopMode(m_nChannel,loops==0?
								FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF)== FALSE )
	{
		m_nChannel = FSOUND_FREE;
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"FSOUND_SetLoopMode() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}
	//�����3D,����λ������
	if( GetMode() == FSOUND_HW3D )
	{
		if( FSOUND_3D_SetAttributes(m_nChannel,m_pPos,0) == FALSE)
		{
			m_nChannel = FSOUND_FREE;
			m_bAlive = false;
			Log4c::Warn(ROOT_MODULE,"FSOUND_3D_SetAttributes() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
			return -1;
		}
	}
	if( FSOUND_SetVolume(m_nChannel,GetVolume()) == FALSE)
	{
		m_nChannel = FSOUND_FREE;
		m_bAlive = false;
		Log4c::Warn(ROOT_MODULE,"FSOUND_SetVolume() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
		return -1;
	}
	SetPlayingType(m_nLoops<1?Audio::PLAY_LOOP:Audio::PLAY_ONCE);
	FSOUND_SetPaused(m_nChannel,FALSE);
	return m_nChannel;
}

//��������
//����˵��
//	time:���������ʱ���,�����ʱ����������������0����ǰ�������
//	loops:����������ѭ������,���С��1,��ʾһֱѭ������;��֮�򲥷�loops�κ�ֹͣ
//����˵��
//	����ɹ�����,������ʹ�õ�channel;ʧ���򷵻�-1
int CFmodSpeech::FadeIn(unsigned int time,int loops)
{
	SetVolume(0);
	int channel=Play(loops);
	if( channel != -1)
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
int CFmodSpeech::FadeOut(unsigned int time,int loops)
{
	if(GetCurChannel() == -1 || IsAlive() == false )	return -1;
	//int channel=Play(loops);
	SetPlayingType(Audio::PLAY_FADEOUT);
	m_nFadeTime=time;
	m_nFadeStartTime=GetCurTickCount();
	return GetCurChannel();
}

//��ȡ������ǰ��channel
//����˵��
//	��
//����˵��
//	���ص�ǰchannel,-1����Ч��channel,����������Ч��δ����
int CFmodSpeech::GetCurChannel()
{
	return m_nChannel;
}

//���������Ĳ��ų���,��λ�Ǻ���
//����˵��
//	��
//����˵��
//	���ȵĺ�����,-1��ʾ��Ч
int CFmodSpeech::GetAudioLength()
{
	//if(!IsAlive())return -1; 
	//return -1;
	return FSOUND_Sample_GetLength(m_pSample);
}

//ֹͣ����
//����˵��
//	��
//����˵��
//	��
void CFmodSpeech::Stop()
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

//�ж������Ƿ��ڴ��״̬,�Ǵ��״̬��ܿ�ᱻɾ���ͷ�
//����˵��
//	��
//����˵��
//	true��ʾ���������Ա�����,false���෴
bool CFmodSpeech::IsAlive()
{
	return m_bAlive;
}

//�ж������Ƿ����ڲ���
//����˵��
//	��
//����˵��
//	true��ʾ�����ڲ���,false���෴
bool CFmodSpeech::IsPlaying()
{
	if(m_nChannel==FSOUND_FREE)	return false;
	return FSOUND_IsPlaying(m_nChannel)!=0;
}

//�ж������Ƿ���2D����
//����˵��
//	��
//����˵��
//	true��ʾ������2D����,false���෴
bool CFmodSpeech::Is3D()
{
	if( GetMode() == FSOUND_HW3D)	return true;
	return false;
}

//����������λ��
//����˵��
//	pos:x,y,z������,��������Ϊ3������
//����˵��
//	��
//ע������
//	pos�ĳ�������Ϊ3,������������Խ��Ĵ���;pos��������Чָ��
void CFmodSpeech::SetPosition(float *pos)
{
	if(pos==0)return;
	SetPosition(pos[0],pos[1],pos[2]);
}

//����������λ��
//����˵��
//	x:x����	y:y����	z:����
//����˵��
//	��
void CFmodSpeech::SetPosition(float x,float y,float z)
{
	m_pPos[0]=x;m_pPos[1]=y;m_pPos[2]=z;
}

//��ȡ����������,�洢��x,y,z�����ñ�����
//����˵��
//	x:�洢x��������ñ��� y:�洢y��������ñ���	z:�洢z��������ñ���
//����˵��
//	��
void CFmodSpeech::GetPosition(float &x,float &y,float &z)
{
	x=m_pPos[0];y=m_pPos[1];z=m_pPos[2];
}

//��ȡ����������,�洢��pos������
//����˵��
//	pos:��������Ϊ3������
//����˵��
//	��
//ע������
//	pos��������Чָ���ҳ�������Ϊ3
void CFmodSpeech::GetPosition(float *pos)
{
	memcpy(pos,m_pPos,sizeof(m_pPos));
}

void CFmodSpeech::Run()
{
	//���Ƿ���ڻ
	if( m_bAlive == false)	return;
	bool bPlaying = IsPlaying();
	bool bVolumeChange = Audio::m_bSpeechVolumeChange; 
	if(bVolumeChange)
	{
		SetVolume(Audio::s_fSpeechVolume);
		Audio::m_bSpeechVolumeChange = true;
	}

	switch(GetPlayingType())
	{
	case Audio::PLAY_ONCE:
		{
			if(!bPlaying)
			{
				if(--m_nLoops>0)
				{
					int nChannel = FSOUND_PlaySound(m_nChannel,m_pSample);
					if(nChannel == -1 || m_nChannel != nChannel)
					{
						Log4c::Warn(ROOT_MODULE,"FSOUND_PlaySoundEx() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
					}
					else
					{
						bPlaying = true;
					}
				}
			}
		}
		break;
	case Audio::PLAY_LOOP:
		{
		}
		break;
		//����
	/*case Audio::PLAY_FADEIN:
		{
			if(bPlaying == false)	break;
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fVolume=time*Audio::s_fSoundVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>=m_nFadeTime)
			{
				SetPlayingType(Audio::PLAY_ONCE);
				m_nLoops = 1;
			}
		}
		break;*/
		//����
	/*case Audio::PLAY_FADEOUT:
		{
			if(bPlaying == false)	break;
			DWORD time=GetCurTickCount()-m_nFadeStartTime;
			float fVolume=Audio::s_fSoundVolume-time*Audio::s_fSoundVolume/m_nFadeTime;
			SetVolume(fVolume);
			bVolumeChange = true;
			if(time>=m_nFadeTime)
			{
				m_bAlive = false;
			}
		}
		break;*/
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
			Log4c::Warn(ROOT_MODULE,"CFmodSound::Run:FSOUND_SetVolume() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
		}
	}
}

//����������״̬
//����˵��
//	��
//����˵��
//	��
//void CFmodSound::Update()
//{
//	switch(GetPlayingType())
//	{
//	case Audio::PLAY_ONCE:
//		{
//			if(!IsPlaying())
//			{
//				if(--m_nLoops>0)
//				{
//					m_nChannel = FSOUND_PlaySound(m_nChannel,m_pSample);
//				}
//				else
//				{
//					Stop();
//				}
//			}
//		}
//		break;
//	case Audio::PLAY_LOOP:
//		{
//			SetVolume(Audio::s_fSoundVolume);
//		}
//		break;
//	case Audio::PLAY_FADEIN:
//		{
//			DWORD time=GetCurTickCount()-m_nFadeStartTime;
//			if(time>=m_nFadeTime)
//			{
//				SetPlayingType(Audio::PLAY_LOOP);
//				SetVolume(Audio::s_fSoundVolume);
//			}
//			else
//			{
//				float fVolume=time*Audio::s_fSoundVolume/m_nFadeTime;
//				SetVolume(fVolume);
//			}
//		}
//		break;
//	case Audio::PLAY_FADEOUT:
//		{
//			DWORD time=GetCurTickCount()-m_nFadeStartTime;
//			if(time>m_nFadeTime)
//			{
//				Stop();
//			}
//			else
//			{
//				float fVolume=Audio::s_fSoundVolume-time*Audio::s_fSoundVolume/m_nFadeTime;
//				SetVolume(fVolume);
//			}
//		}
//		break;
//	default:
//		break;
//	}	
//	if( IsPlaying() )
//	{
//		if(m_b3D)FSOUND_3D_SetAttributes(m_nChannel,m_pPos,0);
//		FSOUND_SetVolume(m_nChannel,GetVolume());
//	}
//	else
//	{
//		/*if(m_bCloned)
//		{
//			static DWORD dwTimerStart=0;
//			if(dwTimerStart=0)dwTimerStart=GetCurTickCount();
//			else if(GetCurTickCount()-dwTimerStart>3000)
//			{
//				m_bAlive=false;
//				dwTimerStart=0;
//			}
//		}
//		else if(GetCurTickCount()-m_nLastActiveTime>30000)
//		{
//			m_bAlive=false;
//
//		}*/
//	}
//}
