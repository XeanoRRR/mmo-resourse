#pragma once

//Functin:��ɻỰ���ܣ������������ͻ��˵�һ���첽�ص�
//Athor:Joe
//Create Time:2004.6.17

class CNetSessionManager;

//�첽���ý��
enum eAsyncResult
{
	AR_DO,				//����
	AR_OK,				//���óɹ�
	AR_TimeOut,			//��ʱ
	AR_Fail,			//ʧ��
};

//�첽���ý�����ݽṹ
struct tagAsyncResult
{
	eAsyncResult	Result;		//���
	va_list			Arguments;	//����
};

//�ص��ӿ� 
class IAsyncCallback
{
public:
	virtual	void OnAsyncCallback(tagAsyncResult&)	= 0;			    //�첽�����еĻص�����,����Ϊ�ڻỰ�з�����Ϣʱ�����ĺ��������һ�ε���
	virtual	void Release() = 0;											//�ͷ���Դ
};

//���ýӿ�
class IAsyncCaller
{
public:
	virtual void DoAsyncCall(const CGUID& guid,long lPassWord,va_list) = 0;		//�첽�����еĵ��ýӿ�,�ýӿ�ͬ��ִ��
	virtual void Release() = 0;											        //�ͷ���Դ
};


class CNetSession
{
	//���������ֻ����CNetSessionManager�������
	//friend class CNetSessionManager;

public:
	CNetSession(const CGUID& guid, pair<CGUID,long> CookiePair);
	~CNetSession(void);

public:
	const CGUID&	GetGUID()	{return m_guid;}							//�õ�Ψһ��ʶ

	bool	CheckCookie(const CGUID& PlayerGuid,long lPassWord);		    //�õ�ȫ��Ψһ�����о�� 
	void	SetCallbackHandle(IAsyncCallback*);				                //���ûص��������

	void	Beging(ulong dwTimeOut,IAsyncCaller*,...);		                //��ʼ�첽����

	void	OnDo(va_list);									                //�ص�ʱ����
	void	OnResult(va_list);								                //�ص���������ʱ����
	void	OnTimeOut();									                //��ʱ����

	ulong	GetLeftTime()	{return m_dwTimeOut;}
	void	SetLeftTime(ulong dwTime)	{m_dwTimeOut=dwTime;}

private:
	CGUID	            m_guid;											    //Ψһ��ʶ
	pair<CGUID,long>	m_Cookie;							                //��֤��ʶ<PlayerID,randomID>
	ulong	            m_dwTimeOut;									    //��ʱʱ��
	IAsyncCallback*	    m_pCallback;							            //�ص��ӿ�
};
