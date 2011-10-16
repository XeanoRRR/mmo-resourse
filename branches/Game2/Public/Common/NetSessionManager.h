#pragma once
//Function:���𴴽������١�����Ựʵ��
//Author:Joe
//CreateTime: 2004.6.17
#include "../Module/MulTiThMap.h"
class CGUID;
class CNetSession;

class CNetSessionManager
{
private:
	CNetSessionManager(void);
public:
	~CNetSessionManager(void);

public:

	CNetSession*	CreateSession(const CGUID& UserGuid);						                    //����һ������Ựʵ��

	void OnDo(const CGUID& guid, const CGUID& UserGuid, long lPassWord,...);						//����ص�����
	void OnSyncCallbackResult(const CGUID& guid, const CGUID& UserGuid, long lPassWord,...);		//����ص�������������

	void Run();

	static CNetSessionManager* getInstance();

	void	Release();								    //�ͷ���Դ

private:

	static CNetSessionManager* instance;				//ȫ��Ψһ��ʵ��

	MulTiThMap<CGUID,CNetSession*>		m_Sessions;		//����ȫ�ֵĻỰʵ��
};

CNetSessionManager* GetNetSessionManager();
