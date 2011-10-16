



#pragma once


class CPlayerPwd
{
public:
	CPlayerPwd(void);
	virtual ~CPlayerPwd(void);

public:
	//!
	enum ePwdState
	{
		//! ����
		ePS_Lock,
		//! ��
		ePS_Free,
		//! ����
		ePS_Frost,
	};

public:
	//!
	void						Init(long MaxPwdErrCount, long FrostSec, const char *pPwd = NULL);
	//!							����
	void						Lock(void);
	//!							����
	ePwdState					Unlock(const char *pPassword);
	//!							��������
	ePwdState					GetState(void);
	//!							�޸�����
	ePwdState					ChangePassword(const char *pOldPwd, const char *pNewPwd);
	

	//!							����ֿ�
	void						frost(void);

public:
	//!							�õ����룬����ӿ�ֻӦ���ڱ����
	const char*					GetPwd(void){return m_szPassword;}
	//!							�õ��ⶳʱ�䣬����ӿ�ֻӦ���ڱ����
	DWORD						GetThawTime(void){return m_ThawDate;}
	//!							���ýⶳʱ�䣬����ӿ�ֻӦ���ڱ����
	void						SetThawTime(DWORD ThawDate){m_ThawDate = ThawDate; if(0 != ThawDate)m_State = ePS_Frost;};

private:
	//!							�������������
	long						m_MaxPwdErrCount;
	//!							��������
	long						m_FrostSec;

	//!							����
	char						m_szPassword[PASSWORD_SIZE];

	//!							״̬
	ePwdState					m_State;


	
	//!							��������������
	long						m_lInputPwdErrCount;
	//!							��һ�δ����ʱ�̣�һ��ʱ���ڣ�
	long						m_lFirstErrTime;
	//!							�ⶳʱ�̱���
	DWORD						m_ThawDate;
};