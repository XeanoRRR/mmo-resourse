#ifndef _SAMPLE_MANAGER_H_
#define _SAMPLE_MANAGER_H_
//Sample������

#include "fmod.h"
#include "fmod_errors.h"

class CSampleManager
{
public:
	static float s_fMinDistance;//3D��������С����
	static float s_fMaxDistance;//3D������������

	struct tagSampleRef
	{
		FSOUND_SAMPLE *pSample;	//��Դ���
		int nRefCount;			//���ô���
		DWORD dwStartRefTime;	//���һ������ʱ��
		DWORD dwUseNum;			//ʹ�ù����ܴ���
		void *pData;
		int datalen;
	};

	struct tagKey
	{
		string strName;			//��Դ���ļ���
		DWORD  dwMode;			//װ��ģʽ
	};

	class key_compare
	{
	public:
		bool operator( )( const tagKey& Key1, const tagKey& Key2 ) const
		{
			int nRet = Key1.strName.compare(Key2.strName);
			if(nRet < 0)
				return true;
			else if(nRet == 0)
				if(Key1.dwMode < Key2.dwMode)
					return true;
			return false;
		}

	};

	struct tagStreamRef
	{
		FSOUND_STREAM *pStream;	//��Դ���
		int nRefCount;			//���ô���
		DWORD dwStartRefTime;	//���һ������ʱ��
		DWORD dwUseNum;			//ʹ�ù����ܴ���
		void *pData;
		int datalen;
	};

public:
	CSampleManager(void);
	virtual ~CSampleManager(void);

	//�õ�һ��sample��Դ
	//����б�����,��������1
	//���û�У���װ����Դ����ʼֵ��0
	FSOUND_SAMPLE *GetSample(const char* strFileName,DWORD dwMode);
	//��CFmodSound���ٵ�ʱ�򣬵��ô˽ṹ��ʹ��������1
	void Release(FSOUND_SAMPLE *pSample);

	FSOUND_STREAM *GetStream(const char* strFileName,DWORD dwMode);

	void Release(FSOUND_STREAM *pStream);
	/*��������*/
	void GrabageCollection();

private:
	void Clear(void);
	FSOUND_SAMPLE *LoadSample(const char* strFileName,DWORD dwMode,void*& pBuf,int& len);
	void UnLoadSample(FSOUND_SAMPLE *pSample);

	FSOUND_STREAM *LoadStream(const char* strFileName,DWORD dwMode,void*& pBuf,int& len);
	void UnLoadStream(FSOUND_STREAM *pStream);

private:
	DWORD	m_dwSampleStartTime;
	DWORD	m_dwSampleTotalNum;

	typedef map<tagKey,tagSampleRef,key_compare>	mapSample;
	typedef map<tagKey,tagSampleRef,key_compare>::iterator itSample;
	mapSample	m_Samples;

	DWORD	m_dwStreamStartTime;
	DWORD	m_dwStreamTotalNum;

	typedef map<tagKey,tagStreamRef,key_compare>	mapStream;
	typedef map<tagKey,tagStreamRef,key_compare>::iterator itStream;
	mapStream	m_Streams;
};

#endif
