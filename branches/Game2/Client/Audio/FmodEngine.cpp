
#include "stdafx.h"
#include "FmodAudioDef.h"
#include "samplemanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float Audio::s_fMusicVolume=1.0f;
float Audio::s_fSoundVolume=1.0f;
float Audio::s_fSpeechVolume=1.0f;

bool Audio::m_bSoundVolumeChange = false;
bool Audio::m_bMusicVolumeChange = false;
bool Audio::m_bSpeechVolumeChange = false;

CFmodEngine *CFmodEngine::s_pInstance=NULL;

//CRITICAL_SECTION CFmodEngine::m_CriticalSectionAudioMSG;


//��ȡ/�������������ʵ��
//����˵��
//	hwnd���ھ��
//����˵��
//	�����ʼ���ɹ�����ȫ��Ψһ������ʵ��,���򷵻ؿ�
CFmodEngine *CFmodEngine::GetInstance(HWND hWnd)
{
	if(s_pInstance==NULL)
	{
		if( FSOUND_SetBufferSize(200) == FALSE)
		{
			Log4c::Warn(ROOT_MODULE,"FSOUND_SetBufferSize(int) failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
			return NULL;
		}
		if( FSOUND_SetHWND(hWnd) == FALSE )
		{
			Log4c::Warn(ROOT_MODULE,"FSOUND_SetHWND(void*) failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
			return NULL;
		}
		if( FSOUND_Init(44100,16,0) == FALSE )
		{
			Log4c::Warn(ROOT_MODULE,"FSOUND_Init() failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
			return NULL;
		}
		s_pInstance=new CFmodEngine();
	}
	return s_pInstance;
}

//������������
//����˵��
//	��
//����˵��
//	��
CFmodEngine::CFmodEngine()
{
	memset(m_pListenerPosition,0,sizeof(m_pListenerPosition));
	memset(m_pListenerVelocity,0,sizeof(m_pListenerVelocity));
	memset(m_pListenerFace,0,sizeof(m_pListenerFace));
	memset(m_pListenerUp,0,sizeof(m_pListenerUp));
	m_setAudioList.clear();
	m_bMusicEnabled=false;
	m_bSoundEnabled=false;
	m_bSpeechEnabled=false;

	m_nMaxSW2DChannels = 16;
	m_nMaxHW2DChannels = 0;
	m_nMaxHW3DChannels = 0;

	m_nSoundsSW2D = 0;
	m_nSoundsHW2D = 0;
	m_nSoundsHW3D = 0;

	m_nSpeechsSW2D = 0;
	m_nSpeechsHW2D = 0;
	m_nSpeechsHW3D = 0;
	m_dwLastGCTime= GetCurTickCount();

	m_Fades.clear();
	InitializeCriticalSection(&m_CSFade);

	int total = 0;
	//�õ����2D��3DӲ���ŵ�����
	if( FSOUND_GetNumHWChannels(&m_nMaxHW2DChannels,
								&m_nMaxHW3DChannels,&total) == FALSE)
	{
		Log4c::Warn(ROOT_MODULE,"FSOUND_GetNumHWChannels(int*,int*,int*) failed(%s).",FMOD_ErrorString(FSOUND_GetError()));
	}
	m_nMaxHW2DChannels = min(16,m_nMaxHW2DChannels);
	m_nMaxHW3DChannels = min(16,m_nMaxHW3DChannels);
	//�����Ϣ
	int nTotalChannels = FSOUND_GetMaxChannels();
	char strmsg[250]="";
	Log4c::Warn(ROOT_MODULE,"MaxSW2DChannels:%d,MaxHW2DChannels:%d,MaxHW3DChannels:%d,TotalHWChannel:%d,TotalChannel:%d.",
				m_nMaxSW2DChannels,m_nMaxHW2DChannels,m_nMaxHW3DChannels,total,nTotalChannels);
	//m_nMaxHW3DChannels = 0;
	//����Sample������
	m_pSmapleManager = new CSampleManager();
}

//�ͷ�ɾ����������
//����˵��
//	��
//����˵��
//	��
CFmodEngine::~CFmodEngine()
{
	RelaseAll();
	delete m_pSmapleManager;

	DeleteCriticalSection(&m_CSFade);

	FSOUND_Close();
}

//����3D������С������
//����˵��
//	min:��С���� max:������
//����˵��
//	��
void CFmodEngine::SetMinMaxDistance(float min,float max)
{
	CSampleManager::s_fMinDistance=min;
	CSampleManager::s_fMaxDistance=max;
}

//����rollofffactor
//����˵��
//	factor:rollofffactor,������ο�Fmod�����ĵ�
//����˵��
//	��
void CFmodEngine::SetRolloffFactor(float factor)
{
	FSOUND_3D_SetRolloffFactor(factor);
}

//����distancefactor
//����˵��
//	factor:distancefactor,������ο�Fmod�����ĵ�
//����˵��
//	��
void CFmodEngine::SetDistanceFactor(float factor)
{
	FSOUND_3D_SetDistanceFactor(factor);
}

//�����ͷŲ�������е�����
//����˵��
//	��
//����˵��
//	��
void CFmodEngine::RelaseAll()
{
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		delete (*it);
	}
	m_setAudioList.clear();
}

//�����ͷŲ�������е�����
//����˵��
//	��
//����˵��
//	��
//void CFmodEngine::ReleaseAllSounds()
//{
//	set<Audio *>::iterator it;
//	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
//	{
//		if((*it)->IsMusic())continue;
//		(*it)->Release();
//	}
//}

void CFmodEngine::StopAllSounds()
{
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if((*it)->IsSound())
		{
			(*it)->Stop();
		}
	}
}
void CFmodEngine::StopAllSpeechs()
{
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if((*it)->IsSpeech())
		{
			(*it)->Stop();
		}
	}
}

//�����ͷŲ�������е�����
//����˵��
//	��
//����˵��
//	��
//void CFmodEngine::ReleaseAllMusic()
//{
//	
//	set<Audio *>::iterator it;
//	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
//	{
//		if(!(*it)->IsMusic())continue;
//		(*it)->Release();
//	}
//}

void CFmodEngine::StopAllMusic()
{
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if((*it)->IsMusic())
			(*it)->Stop();
	}
}

//�ͷ�һ����������,�����������´�״̬���µ�ʱ��ɾ��
//����˵��
//	pAudio:Ҫ�ͷŲ�ɾ������������
//����˵��
//	��
void CFmodEngine::ReleaseAudio(Audio *pAudio)
{
	if( pAudio )
	{
		pAudio->Release();
	}
}

//��ѯ��ǰ�ܹ����ڵ��������������֣�
//����˵��
//	��
//����˵��
//	������
size_t CFmodEngine::GetSoundsCount()
{
	return (int)m_setAudioList.size();
}

//�����ļ����ֻ�ȡ(����)����
//����˵��
//	filename:�ļ���
//����˵��
//	����Ѿ���������ļ��������ֶ���,���ظö���;���򴴽��������µ����ֶ���
//ע������
//	�ͻ��������˱��������ֵ��������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
//CFmodMusic *CFmodEngine::GetMusic(const char *filename)
//{
//	if(!m_bMusicEnabled)return NULL;
//	if(filename==NULL)return NULL;
//
//	EnterCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//	set<Audio *>::iterator it;
//	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
//	{
//		Audio *pAudio=(*it);
//		if( pAudio->IsAlive() && pAudio->IsMusic() && stricmp(filename,pAudio->GetFilename())==0 )
//		{
//			LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//			return dynamic_cast<CFmodMusic *>(pAudio);
//		}
//	}
//	CFmodMusic *pMusic=new CFmodMusic(filename);
//	if(pMusic->IsAlive())
//	{
//		m_setAudioList.insert(pMusic);
//		LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//		return pMusic;
//	}
//	else
//	{
//		pMusic->Release(false);
//		delete pMusic;
//	}
//	LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//	return NULL;
//}

CFmodMusic *CFmodEngine::GetMusicEx(const char *strFilename,DWORD dwMode)
{
	if(!m_bMusicEnabled)return NULL;
	if(strFilename==NULL)return NULL;
	if(strcmp(strFilename,"") == 0)	return NULL;

	if(dwMode == FSOUND_HW3D) dwMode = FSOUND_HW2D;
	if(dwMode == FSOUND_HW2D && m_nMaxHW2DChannels == 0) dwMode = FSOUND_2D;

	//�жϵ�ǰ���������Ƿ񳬹����ֵ
	if(dwMode == FSOUND_HW2D && m_nSoundsHW2D >= m_nMaxHW2DChannels)	return NULL;
	if(dwMode == FSOUND_2D   && m_nSoundsSW2D >= m_nMaxSW2DChannels)	return NULL;

	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		Audio *pAudio=(*it);
		//�������,���Ǽ���״̬�ģ�����NULL
		if(pAudio && stricmp(strFilename,pAudio->GetFilename())==0 )
		{
			if(pAudio->IsMusic() && pAudio->IsAlive())
				return NULL;
		}
	}

	if(m_pSmapleManager==NULL) return NULL;
	//�ӹ������еõ�sample
	FSOUND_STREAM *pStream = m_pSmapleManager->GetStream(strFilename,dwMode);
	if(pStream == NULL)	return NULL;

	CFmodMusic *pMusic=new CFmodMusic(strFilename,pStream,dwMode);
	m_setAudioList.insert(pMusic);
	if(dwMode == FSOUND_HW3D)
		m_nSoundsHW3D++;
	else if(dwMode == FSOUND_HW2D)
		m_nSoundsHW2D++;
	else if(dwMode == m_nSoundsSW2D)
		m_nSoundsSW2D++;
	return pMusic;
}

//�����ļ����ֲ����Ѿ����ڵ�����
//����˵��
//	filename:�ļ���
//����˵��
//	����Ѿ���������ļ��������ֶ���,���ظö���;���򴴽������ؿ�
//ע������
//	�ͻ��������˱��������ֵ��������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
CFmodMusic *CFmodEngine::FindMusic(const char *filename)
{
	if(filename==NULL)return NULL;
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		Audio *pAudio=(*it);
		if( pAudio->IsMusic() && stricmp(filename,pAudio->GetFilename())==0 )
		{
			return dynamic_cast<CFmodMusic *>(pAudio);
		}
	}	
	return NULL;
}

//�����ļ����ֻ�ȡ(����)��������
//����˵��
//	filename:�ļ���
//	b2d:ָʾ�Ƿ���2D����
//����˵��
//	����Ѿ���������ļ��������������Ҹ����������ڲ���״̬,���ظö���;
//	���򴴽��������µ���������,�������������ʹ�ô˴��ļ�����һ�δ�����,
//	����������ڲ�����Ϻ��Զ�������
//ע������
//	�ͻ��������˱������������������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
//	�����2D����,������ڸ��ļ���ʵ������,ֱ�ӷ��ظ���������
//CFmodSound *CFmodEngine::GetSound(const char *filename,bool b2d)
//{
//	static int num2d=0,num3d=0,total=0;
//	if(total==0)FSOUND_GetNumHWChannels(&num2d,&num3d,&total);
//	if(!m_bSoundEnabled)return NULL;
//	if(filename==NULL)return NULL;
//	if(b2d&&num2d<=m_nSounds2D)return NULL;
//	if(!b2d&&num3d<=m_nSounds3D)return NULL;
//	EnterCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//	CFmodSound *pSound=NULL;
//	FSOUND_SAMPLE *pSample=NULL;
//	set<Audio *>::iterator it;
//	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
//	{
//		CFmodSound *pSound=dynamic_cast<CFmodSound *>((*it));
//		if(pSound==NULL)continue;
//		if( !pSound->IsAlive() )continue;
//		if( stricmp(pSound->GetFilename(),filename)==0 )
//		{
//			if( !pSound->IsPlaying() && b2d==pSound->Is2D() )
//			{
//				LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//				return pSound;
//			}
//			pSample=pSound->m_pSample;
//		}
//	}
//	pSound=new CFmodSound(filename,pSample,b2d);
//	if(pSound->IsAlive())
//	{
//		m_setAudioList.insert(pSound);
//		LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//		return pSound;
//	}
//	else
//	{
//		pSound->Release(false);
//		delete pSound;
//	}
//	LeaveCriticalSection(&CFmodEngine::m_CriticalSectionAudioMSG);
//	return NULL;
//}

//�����ļ����ֻ�ȡ(����)��������
//����˵��
//	filename:�ļ���
//	bHW3D:ָʾ�Ƿ���3D������
CFmodSound *CFmodEngine::GetSoundEx(const char *strFilename,DWORD dwMode)
{	
	if(m_bSoundEnabled == false) return NULL;
	if(strFilename == NULL)	return NULL;
	if(strcmp(strFilename,"") == 0)	return NULL;
	//�ж��Ƿ�֧��HW
	if(dwMode == FSOUND_HW3D && m_nMaxHW3DChannels == 0) dwMode = FSOUND_HW2D;
	if(dwMode == FSOUND_HW2D && m_nMaxHW2DChannels == 0) dwMode = FSOUND_2D;

	//�жϵ�ǰ���������Ƿ񳬹����ֵ
	if(dwMode == FSOUND_HW3D && m_nSoundsHW3D >= m_nMaxHW3DChannels)	return NULL;
	if(dwMode == FSOUND_HW2D && m_nSoundsHW2D >= m_nMaxHW2DChannels)	return NULL;
	if(dwMode == FSOUND_2D   && m_nSoundsSW2D >= m_nMaxSW2DChannels)	return NULL;

	if(m_pSmapleManager==NULL) return NULL;
	//�ӹ������еõ�sample
	FSOUND_SAMPLE *pSample = m_pSmapleManager->GetSample(strFilename,dwMode);
	if(pSample == NULL)	return NULL;

	CFmodSound *pSound=new CFmodSound(strFilename,pSample,dwMode);
	m_setAudioList.insert(pSound);
	if(dwMode == FSOUND_HW3D)
		m_nSoundsHW3D++;
	else if(dwMode == FSOUND_HW2D)
		m_nSoundsHW2D++;
	else if(dwMode == m_nSoundsSW2D)
		m_nSoundsSW2D++;
	return pSound;
}

//�����ļ����ֺ�channel�����Ѿ����ڵ�����
//����˵��
//	filename:�ļ���
//	channel:����������ʹ�õ�channel,-1��ʾ����2D����
//����˵��
//	����ҵ��ø��ļ�����������ʹ�����channel������,�򷵻ظ�����,���򷵻ؿ�
//ע������
//	�ͻ��������˱������������������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
CFmodSound *CFmodEngine::FindSound(const char *filename,int channel)
{
	if(filename==NULL)	return NULL;
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if( (*it) && (*it)->IsSound() )
		{
			CFmodSound *pSound=(CFmodSound *)(*it);
			if(pSound==NULL)	continue;
			if(	stricmp(filename,pSound->GetFilename())	!=0	)	continue;

			return pSound;
			/*DWORD dwMode = pSound->GetMode();
			if( (dwMode ==FSOUND_HW2D || dwMode == FSOUND_2D) && channel == -1)
				return pSound;
			if(dwMode == FSOUND_HW3D &&  pSound->GetCurChannel() == channel )
				return pSound;*/
		}
	}
	return NULL;
}

//����channel�����Ѿ����ڵ�����(ֻ��3D��Ч��Ч)
//����˵��
//	channel:����������ʹ�õ�channel
//����˵��
//	����ҵ��ø��ļ�����������ʹ�����channel������,�򷵻ظ�����,���򷵻ؿ�
//ע������
//	�ͻ��������˱������������������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
CFmodSound *CFmodEngine::FindSound(int channel)
{
	if(channel==-1)	return NULL;
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if( (*it) && (*it)->IsSound() )
		{
			CFmodSound *pSound=(CFmodSound *)(*it);
			if(pSound==NULL)	continue;
			DWORD dwMode = pSound->GetMode();
			if(dwMode == FSOUND_HW3D &&  pSound->GetCurChannel() == channel )
				return pSound;
		}
	}
	return NULL;
}

//�����ļ����ֻ�ȡ(����)��������
//����˵��
//	filename:�ļ���
//	bHW3D:ָʾ�Ƿ���3D������
CFmodSpeech *CFmodEngine::GetSpeechEx(const char *strFilename,DWORD dwMode)
{	
	if(m_bSpeechEnabled == false) return NULL;
	if(strFilename == NULL)	return NULL;
	if(strcmp(strFilename,"") == 0)	return NULL;
	//�ж��Ƿ�֧��HW
	if(dwMode == FSOUND_HW3D && m_nMaxHW3DChannels == 0) dwMode = FSOUND_HW2D;
	if(dwMode == FSOUND_HW2D && m_nMaxHW2DChannels == 0) dwMode = FSOUND_2D;

	//�жϵ�ǰ���������Ƿ񳬹����ֵ
	if(dwMode == FSOUND_HW3D && m_nSoundsHW3D >= m_nMaxHW3DChannels)	return NULL;
	if(dwMode == FSOUND_HW2D && m_nSoundsHW2D >= m_nMaxHW2DChannels)	return NULL;
	if(dwMode == FSOUND_2D   && m_nSoundsSW2D >= m_nMaxSW2DChannels)	return NULL;

	if(m_pSmapleManager==NULL) return NULL;
	//�ӹ������еõ�sample
	FSOUND_SAMPLE *pSample = m_pSmapleManager->GetSample(strFilename,dwMode);
	if(pSample == NULL)	return NULL;

	CFmodSpeech *pSpeech=new CFmodSpeech(strFilename,pSample,dwMode);
	m_setAudioList.insert(pSpeech);
	if(dwMode == FSOUND_HW3D)
		m_nSpeechsHW3D++;
	else if(dwMode == FSOUND_HW2D)
		m_nSpeechsHW2D++;
	else if(dwMode == m_nSoundsSW2D)
		m_nSpeechsSW2D++;
	return pSpeech;
}

//�����ļ����ֺ�channel�����Ѿ����ڵ�����
//����˵��
//	filename:�ļ���
//	channel:����������ʹ�õ�channel,-1��ʾ����2D����
//����˵��
//	����ҵ��ø��ļ�����������ʹ�����channel������,�򷵻ظ�����,���򷵻ؿ�
//ע������
//	�ͻ��������˱������������������Ժ�ʹ��!���ܻᱻ�Զ��ͷ�ɾ��
CFmodSpeech *CFmodEngine::FindSpeech(const char *filename,int channel)
{
	if(filename==NULL)	return NULL;
	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		if( (*it) && (*it)->IsSpeech() )
		{
			CFmodSpeech *pSpeech=(CFmodSpeech *)(*it);
			if(pSpeech==NULL)	continue;
			if(	stricmp(filename,pSpeech->GetFilename())	!=0	)	continue;

			return pSpeech;
			/*DWORD dwMode = pSound->GetMode();
			if( (dwMode ==FSOUND_HW2D || dwMode == FSOUND_2D) && channel == -1)
				return pSound;
			if(dwMode == FSOUND_HW3D &&  pSound->GetCurChannel() == channel )
				return pSound;*/
		}
	}
	return NULL;
}

//���¼���������
//����˵��
//	��
//����˵��
//	��
void CFmodEngine::SetListenerAttributes()
{
	FSOUND_3D_Listener_SetAttributes(m_pListenerPosition, m_pListenerVelocity,
		m_pListenerFace[0], m_pListenerFace[1], m_pListenerFace[2],
		m_pListenerUp[0], m_pListenerUp[1], m_pListenerUp[2]);
}

//���ü����ߵķ���
//����˵��
//	face:ǰ��ķ����������ʾ up:����ķ����������ʾ
//����˵��
//	��
//ע������
// face��up��������Ч�Ĳ��ҳ�������Ϊ3������
void CFmodEngine::SetListenerOritation(float *face,float *up)
{
	if(face==0 || up==0 )return;
	memcpy(m_pListenerFace,face,sizeof(m_pListenerFace));
	memcpy(m_pListenerUp,up,sizeof(m_pListenerUp));

	//����
	SetListenerAttributes();
}

//���ü����ߵķ���
//����˵��
//	facex,facey,facez:ǰ��ķ����������ʾ upx,upy,upz:����ķ����������ʾ
//����˵��
//	��
void CFmodEngine::SetListenerOritation(float facex,float facey,float facez,float upx,float upy,float upz)
{
	float face[3]={facex,facey,facez},up[3]={upx,upy,upz};
	SetListenerOritation(face,up);
}

//���ü����ߵ�λ��
//����˵��
//	pos:λ�õ�������ʾ
//����˵��
//	��
//ע������
// pos����Ч�Ĳ��ҳ�������Ϊ3������
void CFmodEngine::SetListenerPosition(float *pos)
{
	if(pos==0)return;
	memcpy(m_pListenerPosition,pos,sizeof(m_pListenerPosition));

	//����
	SetListenerAttributes();
}

//���ü����ߵ�λ��
//����˵��
//	x,y,z:λ�õ�������ʾ
//����˵��
//	��
void CFmodEngine::SetListenerPosition(float x,float y,float z)
{
	float pos[3]={x,y,z};
	SetListenerPosition(pos);

	//����
	SetListenerAttributes();
}

//���ü����ߵ��ٶ�
//����˵��
//	vel:�ٶȵ�������ʾ
//����˵��
//	��
//ע������
// vel����Ч�Ĳ��ҳ�������Ϊ3������
void CFmodEngine::SetListenerVelocity(float *vel)
{
	if(vel==0)return;
	memcpy(m_pListenerVelocity,vel,sizeof(m_pListenerVelocity));

	//����
	SetListenerAttributes();
}

//���ü����ߵ��ٶ�
//����˵��
//	x,y,z:�ٶȵ�������ʾ
//����˵��
//	��
void CFmodEngine::SetListenerVelocity(float x,float y,float z)
{
	float vel[3]={x,y,z};
	SetListenerVelocity(vel);
}

//�����������ֵ�ͳһ���������
//����˵��
//	fVolume:����,��0��1
//����˵��
//	��
void CFmodEngine::SetMusicVolume(float fVolume)
{
	Audio::s_fMusicVolume=fVolume;
	Audio::m_bMusicVolumeChange = true;
}

//��������������ͳһ���������
//����˵��
//	fVolume:����,��0��1
//����˵��
//	��
void CFmodEngine::SetSoundVolume(float fVolume)
{
	Audio::s_fSoundVolume=fVolume;
	Audio::m_bSoundVolumeChange = true;
}

//��������������ͳһ���������
//����˵��
//	fVolume:����,��0��1
//����˵��
//	��
void CFmodEngine::SetSpeechVolume(float fVolume)
{
	Audio::s_fSpeechVolume=fVolume;
	Audio::m_bSpeechVolumeChange = true;
}

//���������Ƿ���Ч��
//����˵��
//	b: �Ƿ������
//����˵��
//	��
void CFmodEngine::SetMusicEnabled(bool b)
{
	if(!b&&m_bMusicEnabled)
	{
		//ReleaseAllMusic();
		StopAllMusic();
	}
	m_bMusicEnabled=b;
}

//���������Ƿ���Ч��
//����˵��
//	b: �Ƿ������
//����˵��
//	��
void CFmodEngine::SetSoundEnabled(bool b)
{
	if(!b&&m_bSoundEnabled)
	{
		//ReleaseAllSounds();
		StopAllSounds();
	}
	m_bSoundEnabled=b;
}

//���������Ƿ���Ч��
//����˵��
//	b: �Ƿ������
//����˵��
//	��
void CFmodEngine::SetSpeechEnabled(bool b)
{
	if(!b&&m_bSpeechEnabled)
	{
		//ReleaseAllSounds();
		StopAllSpeechs();
	}
	m_bSpeechEnabled=b;
}



void CFmodEngine::Run()
{
	set<Audio *>	NotAliveAudio;NotAliveAudio.clear();

	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();)
	{
		Audio *pAudio=(*it);
		if(pAudio)
		{		
			pAudio->Run();
			if(pAudio->IsAlive() == false)
			{
				if(pAudio->IsSound())
				{
					FSOUND_SAMPLE *pSample = ((CFmodSound*)pAudio)->GetSample();
					m_pSmapleManager->Release(pSample);
				}
				if(pAudio->IsMusic())
				{
					FSOUND_STREAM *pStream = ((CFmodMusic*)pAudio)->GetStream();
					m_pSmapleManager->Release(pStream);
				}
				if(pAudio->IsSpeech())
				{
					FSOUND_SAMPLE *pSample = ((CFmodSpeech*)pAudio)->GetSample();
					m_pSmapleManager->Release(pSample);
				}
				it = m_setAudioList.erase(it);
				DWORD dwMode = pAudio->GetMode();
				if(dwMode == FSOUND_HW3D)
					m_nSoundsHW3D--;
				else if(dwMode == FSOUND_HW2D)
					m_nSoundsHW2D--;
				else if(dwMode == m_nSoundsSW2D)
					m_nSoundsSW2D--;
				delete pAudio;
				continue;
			}
		}
		it++;
	}

	//ÿ��2�����ռ�һ��û��ʹ�õ���Դ
	DWORD dwCurTime = GetCurTickCount();
	if( dwCurTime-m_dwLastGCTime > 120000)
	{
		m_pSmapleManager->GrabageCollection();
	}
}

void CFmodEngine::Fade(int nChannel,bool bFadeIn,int nMaxVolume,DWORD dwFadeTime)
{
	DWORD dwCurTime = GetCurTickCount();
	tagFade fade = {nChannel,bFadeIn,nMaxVolume,dwCurTime,dwFadeTime};
	EnterCriticalSection(&m_CSFade);
	m_Fades[nChannel] = fade;
	LeaveCriticalSection(&m_CSFade);
}

//���������Լ�����ά�������������ֵ�״̬
//����˵��
//	��
//����˵��
//	��
//ע������
//	����ϵͳ��Ҫ���ڸ���״̬,��˿ͻ�������붨�ڵ���,������ר�ŵ��߳�������,ʱ����������50MS
//�����Ľ��뽥��
void CFmodEngine::Update()
{
	//SetListenerAttributes();
	FSOUND_Update();
	DWORD dwCurTime=GetCurTickCount();
	EnterCriticalSection(&m_CSFade);
	itFade it = m_Fades.begin();
	for(;it != m_Fades.end();)
	{
		int nVolume = 0;
		bool bErase = false;
		tagFade &fade = (*it).second;
		DWORD time=dwCurTime-fade.dwStartTime;
		if(fade.bFadeIn)
		{
			nVolume=time*fade.nMaxVolume/fade.dwFadeTime;
			if(time>fade.dwFadeTime)
			{
				bErase = true;
			}
		}
		else
		{
			nVolume=fade.nMaxVolume-time*fade.nMaxVolume/fade.dwFadeTime;
			if(time>fade.dwFadeTime)
			{
				bErase = true;
				if( FSOUND_StopSound(fade.nChannel) == FALSE)
				{
					Log4c::Warn(ROOT_MODULE,"CFmodEngine::Update:FSOUND_StopSound() failed(%s).",
							FMOD_ErrorString(FSOUND_GetError()));
				}
			}
		}
		if(FSOUND_SetVolume(fade.nChannel,nVolume) == FALSE)
		{
			bErase = true;
			Log4c::Warn(ROOT_MODULE,"CFmodEngine::Update:FSOUND_SetVolume() failed(%s).",
					FMOD_ErrorString(FSOUND_GetError()));
		}
		if(bErase)
			it = m_Fades.erase(it);
		else
			it++;
	}
	LeaveCriticalSection(&m_CSFade);

	/*set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		Audio *pAudio=(*it);
		if(pAudio==NULL)
		{
			continue;
		}
		if(pAudio->IsAlive())
		{
			pAudio->Update();
		}
	}*/
}

/*��������*/
void CFmodEngine::GrabageCollection()
{
	static set<Audio *> setDeleted;
	setDeleted.clear();

	set<Audio *>::iterator it;
	for(it=m_setAudioList.begin();it!=m_setAudioList.end();it++)
	{
		Audio *pAudio=(*it);
		if(pAudio==NULL)
		{
			continue;
		}
		if(pAudio->IsAlive() == false)
		{
			setDeleted.insert(pAudio);
		}
	}

	for(it=setDeleted.begin();it!=setDeleted.end();it++)
	{
		(*it)->Release(false);
		delete (*it);
		m_setAudioList.erase((*it));
	}
}

