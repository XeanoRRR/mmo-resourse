/*-------------------------------------------------------------------------------------------------------------------------------------------

��Ϸ�ͻ����������ź͹���Ĺ�����,����ͻ��˳����߼�����������ľ���ʵ��,����ʹ�ú�ά��

����	:	���Ǿ�
����	:	2004-12-02

������Ŀ����Ϊ�˼򻯿ͻ��˳����������ʹ�úͿ���,�����������ù�����������ľ���ϸ��
�ڲ��źͿ���������ʱ��,���ļ������򻯶Զ���Ĳ���,�����ֺ�2D����һ���ļ�����Ӧһ��������
��;�����3D����,����һ��channel��Ϣ����ͬһ���ļ������Ĳ�ͬ������.

---------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "../clientregion.h"
class CFmodEngine;
class CShape;
class CBaseObject;
class CFmodAudio;
class CFmodSound;
class CFmodMusic;
class CFmoEngine;

typedef D3DXVECTOR3 Vector3;

//�ͻ��˵��������������
class CAudioList
{
private:
	//����ʵ��
	CFmodEngine *m_pAudioEngine;
	//���������ļ���
	string m_strMusicFile;
	//�������ֵĽ���״̬
	bool  m_bFadeState;

private:
public:	// soft 2004.12.23
	//����������Ϣ
	void LoadSetup(const char *filename);

public:
	//����
	CAudioList(void);
	//�ͷ�
	virtual ~CAudioList(void);

public:
    
	/*����CShape�����Ͷ����Ͷ�����������*/
	int Play(CShape * pShape, long lType, WORD wAction, Vector3 vRole, bool loop=false, int nLoops = 1);

	/*���ų�������*/
	int Play(CClientRegion::REGION_SOUND_PTR pSound,long lRegionID,bool loop=false, int nLoops = 1);

	/*���������ļ�*/
	int Play(const char* filename, bool loop=false, bool bEnv = false, int nLoops = 1);

	/*���������ļ�,3D*/
	int Play(const char* filename, float fX, float fY, float fZ, bool loop=false, int nLoops = 1);

	/*��������*/
	void PlayMusic( const char *filename,int loops=0 );

	/*��������*/
	void FadeInMusic( const char *filename,unsigned int time,int loops=0 );

	/*��������*/
	void FadeOutMusic( const char *filename,unsigned int time,int loops=0 );

	/*��������*/
	void FadeMusic( const char *filename,unsigned int time,float Volume,int loops=0 );

	bool GetFadeState(){return m_bFadeState;};

	void SetFadeState(bool b);

	/*ֹͣ����*/
	void StopMusic(const char *filename);

	/*�ͷ�����*/
	void ReleaseMusic(const char *filename);

	/*����2D����*/
	int Play2DSound( const char *filename,int loops=1 );

	/*��������*/
	int PlaySpeech( const char *filename,int loops=1 );

	/*ֹͣ����*/
	void StopSpeech( const char *filename,int channel );

	/*����3D����*/
	int Play3DSpeech(const char *filename,int loops,float x,float y,float z);

	/*����2D����*/
	int FadeIn2DSound( const char *filename,unsigned int time,int loops=0 );

	/*����2D����*/
	int FadeOut2DSound( const char *filename,unsigned int time,int loops=0 );

	/*ֹͣ2D����*/
	//void Stop2DSound( const char *filename );

	/*�ͷ�2D����*/
	void Release2DSound( const char *filename );

	/*����3D����*/
	int Play3DSound( const char *filename,int loops,float x,float y,float z );

	///*����3D����*/
	//int FadeIn3DSound( const char *filename,float x,float y,float z,unsigned int time,int loops=0 );

	///*����3D����*/
	//int FadeOut3DSound( const char *filename,float x,float y,float z,unsigned int time,int loops=0 );

	/*ֹͣ3D����*/
	void Stop3DSound( const char *filename,int channel );

	/*ֹͣ3D����*/
	void Stop3DSound( int channel );

	/*�ͷ�3D����*/
	void Release3DSound( const char *filename,int channel );

	/*����3D������λ��*/
	//void Set3DSoundPosition(const char *filename,int &channel,float x,float y,float z );

	/*��������ϵͳ״̬*/
	void UpdateAudio();

	void Run();

	/*������������*/
	void SetMusicVolume(float fVolume);

	/*������������*/
	void SetSoundVolume(float fVolume);

	/*������������*/
	void SetSpeechVolume(float fVolume);

	/*���������Ƿ��*/
	void SetSoundEnabled(bool b);
	/*���������Ƿ��*/
	void SetSpeechEnabled(bool b);

	/*���������Ƿ��*/
	void SetMusicEnabled(bool b);

	/*�жϸ��ļ��������Ƿ��ڲ���,channelΪ-1��ʾ2D������������*/
	bool IsPlaying(const char *filename,int channel=-1);

	/*���ö����λ��*/
	void SetListenerPosition(float *pos);

	/*���ö���λ��*/
	void SetListenerPosition(float x,float y,float z);

	/*���ö����ٶ�*/
	void SetListenerVelocity(float *vel);

	/*���ö����ٶ�*/
	void SetListenerVelocity(float x,float y,float z);

	/*���ö��䷽��*/
	void SetListenerOritation(float *face,float *up);

	/*���ö��䷽��*/
	void SetListenerOritation(float facex,float facey,float facez,float upx,float upy,float upz);

	/*����װ����Ʒ��Ʒ������*/
	void PlayEquipmentSound(DWORD dwIndex,bool b3D=false,float x=0,float y=0,float z=0);

	/*���ñ��������ļ���*/
	void SetBackgroundMusic(const char *filename);

	/*��ȡ���������ļ���*/
	const char *GetBackgroundMusic();

	/*��������*/
	void GarbageColloction();
private:
	long m_lPlayerInterTime;     // ��ҵ��������Ϸ��ʱ��
	bool m_bPlayMonsterOppear;   // �Ƿ񲥷Ź�����ֵ���Ч
	long m_lPlayOppearSoundTime; // ���Ź��������Ч��ʱ��
	void CountMonsterOppear();     //���������ֵ�ʱ�䣬���ڿ�����ҽ�����Ϸʱ�����������Ź�����ֵ���Ч
public:
	void SetOppearSoundEnble(bool l);//�����Ƿ񲥷ų�����Ч
};
