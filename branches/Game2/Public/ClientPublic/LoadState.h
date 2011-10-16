//=============================================================================
/**
 *  file: LoadState.h
 *
 *  Brief:������ʾ��Դ��װ��״̬
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-11-12
 */
//=============================================================================

#ifndef __LOAD_STATE__
#define __LOAD_STATE__

//ö����Դװ�ص�״̬
enum eLoadState
{
	Load_Not,  //δװ��
	Load_Doing,//����װ��
	Load_Did,  //װ�����
};

class LoadState
{
protected:
	LoadState(void)
	:m_eLoad(Load_Not)
	{
	}
	virtual ~LoadState(void)
	{
	}
	//״ֵ̬
	eLoadState m_eLoad;	
public:
	void SetLoadState(eLoadState eState){m_eLoad = eState;}
	eLoadState GetLoadState()	{return m_eLoad;}
};

#endif
