/*------------------------------------------------------------------------------------------------

��������ģ��

����	:	���Ǿ�
����	:	2004-11-30

��װFMOD��Чϵͳ��API,ʹ֮�����ý�Ϊ�򵥷���ķ�ʽ����

��ģ��ṹ����:
				Audio<------------------------CFmodEngine
				/   \
			   /	 \
		CFmodSound  CFmodMusic
Audio�����ʾ��������,CFmodSound��CFmodMusic�ֱ��ʾ��Ч������
CFmodEngine���������湦��,���ڴ�����ά�����������Լ����������������棬�����豸�������Բ�����

����ʹ��
	�ͻ�����ͨ����ȡCFmodEngine��ʵ�����󴴽���ʹ��������������Դ,�ͻ��������˱���������������
������,����ͨ��FindSound��FindMusic������ڵ������������ֶ���,������ʹ��CFmodSound��CFmodMusic.
	���ֶ�����ʹ����Ϻ���Զ���������Դ��ɾ��;2D�������������ļ���һ�δ�����3D�����ᱻ����,���
����ʽ����CFmodSound��Relase����,����һֱ�������ڴ�;�ظ�������3D�������ڲ�������Զ����պ�ɾ��,
���,����ͻ�����Ҫ�������������������.
	���ڸ���ȫ�����ԵĲ���(��������,������λ��,�������Ϣ),��CFmodEngine����.

-------------------------------------------------------------------------------------------------*/
#ifndef __FMODAUDIODEF__
#define __FMODAUDIODEF__

#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include "fmod.h"
#include "fmod_errors.h"
using namespace std;

class CSampleManager;

//�����������ʾ��
class Audio{	
public:
	static float s_fMusicVolume;//ȫ����������
	static float s_fSoundVolume;//ȫ����������
	static float s_fSpeechVolume;//ȫ����������

	static bool m_bMusicVolumeChange;
	static bool m_bSoundVolumeChange;
	static bool m_bSpeechVolumeChange;

public:

	enum eAudioPlayingType{
		PLAY_NONE,//�޲������ͣ���ʾ����������
		PLAY_ONCE,//��������һ��
		PLAY_LOOP,//����ѭ������
		PLAY_FADEIN,//��������
		PLAY_FADEOUT,//�������� 
		PLAY_FADEDOWN,//�������䣨��С��
		PLAY_FADEUP,//�������䣨���
	};
private:
	int			m_nPlayingType;				//�����Ĳ�������
	string		m_strFilename;				//�������ļ�����
	int			m_nVolume;

protected:
	DWORD		m_dwMode;

public:
	//��ȡ�����Ĳ�������,ΪeAudioPlayingType֮�е�һ��
	int GetPlayingType(){return m_nPlayingType;}

	//���������Ĳ�������
	void SetPlayingType(eAudioPlayingType type){m_nPlayingType=type;}

	//��ȡ�������ļ�����
	const char *GetFilename(){return m_strFilename.c_str();}

	//�ж������Ƿ�������
	virtual bool IsMusic(){return false;}
	virtual bool IsSound(){return false;}
	virtual bool IsSpeech(){return false;}

	//�ж������Ƿ���2D
	virtual bool Is3D()	{return false;}
	DWORD	GetMode()	{return m_dwMode;}

	//�ж������Ƿ����ڲ���
	virtual bool IsPlaying()=0;

	//��ȡ������ǰ��ʹ�õ�channel
	virtual int	GetCurChannel()=0;
	
	//��������
	virtual int Play(int loops=1)=0;

	//��������
	virtual int FadeIn(unsigned int time,int loops)=0;

	//��������
	virtual int FadeOut(unsigned int time,int loops)=0;

	//ֹͣ����
	virtual void Stop()=0;

	//�ͷ�����,bDelayָ���Ƿ��ӳ��ͷ�
	virtual void Release(bool bDelay=true)=0;
	
	//��ȡ������ʱ�䳤��
	virtual int GetAudioLength()=0;

	//����������������С
	virtual void SetVolume(float volume){m_nVolume=max(0,min(255,(int)(volume*255)));};

	//��ȡ������������С
	virtual int GetVolume(){return m_nVolume;};

	//�ж������Ƿ���
	virtual bool IsAlive()=0;

	virtual void Run() = 0;
	//��������״̬
	virtual void Update()=0;

	//�ͷ�ɾ������
	virtual ~Audio(){};

protected:
	//��������
	Audio(const char *filename):m_strFilename(filename){}
};

//Fmod��������
class CFmodSound : public Audio
{
	friend class CFmodEngine;
private:
	//������������������ָ��
	FSOUND_SAMPLE	*m_pSample;
	
	//����������channel	
	int				m_nChannel;

	//����������߽�����ʱ��
	unsigned int	m_nFadeTime;

	//������ʼ������߽�����ʱ��
	unsigned int	m_nFadeStartTime;

	//�������һ�λ�����ţ��ĵ�ʱ��
	//unsigned int	m_nLastActiveTime;

	//������ѭ������
	int				m_nLoops;

	//�����Ƿ���
	bool			m_bAlive;

	//�����Ƿ���3D
	bool			m_b3D;

private:
	/*��������*/
	CFmodSound(const char *filename,FSOUND_SAMPLE *pStream,DWORD dwMode);	
	
	//������λ��
	float m_pPos[3];

public:

	/*��������,loops��ʾѭ������*/
	int Play(int loops=1);

	/*����������С�����Ĳ���,timeָ��������ʱ��,loops��ʾ����ѭ������*/
	int FadeIn(unsigned int time,int loops);

	/*�������������ֹͣ�Ĳ���,timeָ��������ʱ��,loops��ʾ����ѭ������,����������С���ֹͣ*/
	int FadeOut(unsigned int time,int loops);

	/*ֹͣ����*/
	void Stop();

	/*�ͷ�������������Դ*/
	void Release(bool bDelay=true);

	/*�����Ƿ���,����ʹ��*/
	bool IsAlive();

	/*�����Ƿ����ڲ���*/
	bool IsPlaying();

	/*�Ƿ���2D����*/
	bool Is3D();

	bool IsSound(){return true;}

	/*���õ�ǰ����������channel*/
	void SetCurChannel(int nCannel){m_nChannel = nCannel;}

	/*�õ���ǰ����������channel*/
	int GetCurChannel();

	/*�õ������Ĳ��ų���,����*/
	int GetAudioLength();

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void SetPosition(float x,float y,float z);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void SetPosition(float *pos);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void GetPosition(float &x,float &y,float &z);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void GetPosition(float *pos);

	FSOUND_SAMPLE *GetSample()	{return m_pSample;}

	void Run();
	/*ˢ������״̬*/
	void Update(){}

	/*ɾ������*/
	~CFmodSound();
};

//Fmod��������
class CFmodSpeech : public Audio
{
	friend class CFmodEngine;
private:
	//������������������ָ��
	FSOUND_SAMPLE	*m_pSample;
	
	//����������channel	
	int				m_nChannel;

	//����������߽�����ʱ��
	unsigned int	m_nFadeTime;

	//������ʼ������߽�����ʱ��
	unsigned int	m_nFadeStartTime;

	//�������һ�λ�����ţ��ĵ�ʱ��
	//unsigned int	m_nLastActiveTime;

	//������ѭ������
	int				m_nLoops;

	//�����Ƿ���
	bool			m_bAlive;

	//�����Ƿ���3D
	bool			m_b3D;

private:
	/*��������*/
	CFmodSpeech(const char *filename,FSOUND_SAMPLE *pStream,DWORD dwMode);	
	
	//������λ��
	float m_pPos[3];

public:

	/*��������,loops��ʾѭ������*/
	int Play(int loops=1);

	/*����������С�����Ĳ���,timeָ��������ʱ��,loops��ʾ����ѭ������*/
	int FadeIn(unsigned int time,int loops);

	/*�������������ֹͣ�Ĳ���,timeָ��������ʱ��,loops��ʾ����ѭ������,����������С���ֹͣ*/
	int FadeOut(unsigned int time,int loops);

	/*ֹͣ����*/
	void Stop();

	/*�ͷ�������������Դ*/
	void Release(bool bDelay=true);

	/*�����Ƿ���,����ʹ��*/
	bool IsAlive();

	/*�����Ƿ����ڲ���*/
	bool IsPlaying();

	/*�Ƿ���2D����*/
	bool Is3D();

	bool IsSpeech(){return true;}

	/*�õ���ǰ����������channel*/
	int GetCurChannel();

	/*�õ������Ĳ��ų���,����*/
	int GetAudioLength();

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void SetPosition(float x,float y,float z);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void SetPosition(float *pos);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void GetPosition(float &x,float &y,float &z);

	/*���������Ŀռ�λ��,��2D����û��Ч��*/
	void GetPosition(float *pos);

	FSOUND_SAMPLE *GetSample()	{return m_pSample;}

	void Run();
	/*ˢ������״̬*/
	void Update(){}

	/*ɾ������*/
	~CFmodSpeech();
};




//Fmod���ֶ���
class CFmodMusic : public Audio
{
	friend class CFmodEngine;
private:
	//�Ƿ���
	bool				m_bAlive;

	//���ֹ��������Ķ���ָ��
	FSOUND_STREAM		*m_pStream;

	//���ֲ�����ʹ�õ�channel
	int					m_nChannel;

	//���ֽ���򽥳���ʱ�䷶Χ
	unsigned int		m_nFadeTime;

	//���ֿ�ʼ���루����ʱ��ʱ��
	unsigned int		m_nFadeStartTime;

	//���ֲ��ŵĴ���
	int					m_nLoops;

	//���ֽ���ʱ��Ŀ������
	float                 m_fFadeVolume;

	//������ִ�н���ʱ�̵ĵ�ǰ����
	float                 m_fFadeCurVolume;

private:
	//�������ֶ���
	CFmodMusic(const char *filename,FSOUND_STREAM *pStream,DWORD dwMode);

public:
	//�ж��Ƿ�������,������
	bool IsMusic(){return true;}

	/*��������*/
	int Play(int loops=-1);

	/*���ֽ���*/
	int FadeIn(unsigned int time,int loops);

	/*���ֽ���*/
	int FadeOut(unsigned int time,int loops);

	/*���ֽ���*/
	int Fade(unsigned int time,float volume,int loops);

	/*ֹͣ����*/
	void Stop();

	/*�ͷ�����*/
	void Release(bool bDelay=true);

	/*�Ƿ���*/
	bool IsAlive();

	/*�Ƿ��ڲ���*/
	bool IsPlaying();

	/*��ȡ���ֵ�ʱ�䳤��*/
	int GetAudioLength();

	/*��ȡ��ǰʹ�õ�channel*/
	int GetCurChannel();

	FSOUND_STREAM *GetStream()	{return m_pStream;}
	void Run();
	/*����״̬*/
	void Update(){}

	/*ɾ���ͷ�*/
	~CFmodMusic();
};

//Fmod��������
class CFmodEngine
{
public:
	/*��ȡһ������ʵ��*/
	static CFmodEngine* GetInstance(HWND hwnd);
	/*�ṩ�ٽ�������*/
	//static CRITICAL_SECTION m_CriticalSectionAudioMSG;
private:
	//ȫ��Ψһ������ʵ��
	static CFmodEngine* s_pInstance;
private:

	struct tagFade
	{
		int  nChannel;
		bool bFadeIn;
		int nMaxVolume;
		DWORD dwStartTime;
		DWORD dwFadeTime;
	};
	//sample������
	CSampleManager *m_pSmapleManager;
	//��������ļ���
	set<Audio *> m_setAudioList;
	//�����ߵ�λ��
	float m_pListenerPosition[3];
	//�����ߵ��ٶ�
	float m_pListenerVelocity[3];
	//�����ߵ��淽��
	float m_pListenerFace[3];
	//�����ߵ��Ϸ���
	float m_pListenerUp[3];
	//�Ƿ������
	bool m_bSoundEnabled;
	//�Ƿ������
	bool m_bMusicEnabled;
	//�Ƿ������
	bool m_bSpeechEnabled;

	//SW2D����������
	int m_nSoundsSW2D;
	//HW2D����������
	int m_nSoundsHW2D;
	//HW3D����������
	int m_nSoundsHW3D;

	//SW2D����������
	int m_nSpeechsSW2D;
	//HW2D����������
	int m_nSpeechsHW2D;
	//HW3D����������
	int m_nSpeechsHW3D;

	//�����õ�software mixed 2d channels.
	int m_nMaxSW2DChannels;
	//�����õ�hardware mixed 2d channels.
	int m_nMaxHW2DChannels;
	//�����õ�hardware mixed 3d channels.
	int m_nMaxHW3DChannels;

	DWORD m_dwLastGCTime;

	typedef map<int,tagFade>	mapFade;
	typedef map<int,tagFade>::iterator itFade;
	mapFade	m_Fades;
	CRITICAL_SECTION m_CSFade;

private:
	//���¼����ߵ���Ϣ
	void SetListenerAttributes();
	//��������
	CFmodEngine();
public:
	//�ͷ�����
	~CFmodEngine();
	
	/*����rolloff factor*/
	void SetRolloffFactor(float factor);

	/*����mindistance��maxdistance*/
	void SetMinMaxDistance(float min,float max);

	/*����distancefactor*/
	void SetDistanceFactor(float factor);

	/*�������ֵ�����*/
	void SetMusicVolume(float fVolume);

	/*��������������*/
	void SetSoundVolume(float fVolume);

	/*��������������*/
	void SetSpeechVolume(float fVolume);

	/*���������Ƿ���Ч*/
	void SetMusicEnabled(bool b);

	/*���������Ƿ���Ч*/
	void SetSoundEnabled(bool b);

	/*���������Ƿ���Ч*/
	void SetSpeechEnabled(bool b);

	/*���ü����ߵ�λ��*/
	void SetListenerPosition(float *pos);
	
	/*���ü����ߵ�λ��*/
	void SetListenerPosition(float x,float y,float z);
	
	/*���ü����ߵ��ٶ�*/
	void SetListenerVelocity(float *vel);

	/*���ü����ߵ��ٶ�*/
	void SetListenerVelocity(float x,float y,float z);
	
	/*���ü����ߵķ���*/
	void SetListenerOritation(float *face,float *up);
	
	/*���ü����ߵķ���*/
	void SetListenerOritation(float facex,float facey,float facez,float upx,float upy,float upz);

	void Run();
	/*��������״̬*/
	void Update();

	/*��������*/
	void GrabageCollection();

	/*��ѯ�ܵ���������*/
	size_t GetSoundsCount();

	/*��ȡ����*/
	//CFmodSound *GetSound(const char *strFilename,bool b2d);

	//��ȡ����
	CFmodSound *GetSoundEx(const char *strFilename,DWORD dwMode);

	/*��������*/
	CFmodSound *FindSound(const char *strFilename,int channel=-1);

	/*ͨ��channel��������*/
	CFmodSound *FindSound(int channel=-1);

	//��ȡ����
	CFmodSpeech *GetSpeechEx(const char *strFilename,DWORD dwMode);

	/*��������*/
	CFmodSpeech *FindSpeech(const char *strFilename,int channel=-1);
	/*��ȡ����*/
	//CFmodMusic *GetMusic(const char *strFilename);

	CFmodMusic *GetMusicEx(const char *strFilename,DWORD dwMode);

	/*��ȡ����*/
	CFmodMusic *FindMusic(const char *strFilename);

	/*�ͷ�Audio*/
	void ReleaseAudio(Audio *audio);

	/*�ͷ�����*/
	//void ReleaseAllSounds();
	void StopAllSounds();

	/*�ͷ�����*/
	//void ReleaseAllSounds();
	void StopAllSpeechs();

	/*�ͷ�����*/
	//void ReleaseAllMusic();
	void StopAllMusic();

	/*�ͷ�ȫ����Դ*/
	void RelaseAll();

	void SetHW3DMaxChnnels(int nMax)	{m_nMaxHW3DChannels = nMax;}
	void SetHW2DMaxChnnels(int nMax)	{m_nMaxHW2DChannels = nMax;}
	void Fade(int nChannel,bool bFadeIn,int nMaxVolume,DWORD dwFadeTime);

	int GetHW3DMaxChnnels()             {return m_nMaxHW3DChannels;}
	int GetHW2DMaxChnnels()             {return m_nMaxHW2DChannels;}
};

#endif