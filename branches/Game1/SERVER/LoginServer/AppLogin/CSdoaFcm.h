





#include "../../sdoa4fcm/FCMsdkBase.h"


#pragma once


class CSdoaFcm
{
public:
	CSdoaFcm(VOID);
	~CSdoaFcm(VOID);

public:
	//!					
	bool								Init(VOID);
	//!
	VOID								Release(VOID);

	//!									��Ӧ�û�����
	int									UserOnline(const char* szRoleID, long lIP);
	//!									��Ӧ�û�����
	int									UserOffline(const char* szRoleID);

//! 3���ص�����
public:
	//!									��Ӧ������֪ͨ
	static	int							OnNotifyCall(const char* szUserID, const char* szRoleID,int iOnlineMinutes,int iOfflineMinutes);
	//!									��Ӧ�����Խ������ needFCM = 1��Ҫ������ ��needFCM = 0����Ҫ������
	static	void						OnSwitch(int needFCM);
	//!									��Ӧ��ѯ���󷵻�
	static	void						OnQueryReturn(const char* szUserID, const char* szRoleID,int iState ,int iOnlineMinutes,int iOfflineMinutes);

private:
	//!									ʢ��SDK�еķ�������֤����
	CFCMSDKBase							*m_pCFCMSDKBase;
};