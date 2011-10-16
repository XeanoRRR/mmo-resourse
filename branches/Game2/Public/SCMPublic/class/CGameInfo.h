#include <time.h>


#pragma once

/*
	��Ϸ�����������в�������Ϣ����������Щ����й���
	
	tagGameInfoData��	��Ϸ��Ϣ�����ݡ���Ϣ�ľ������ݣ�������SM��SUC���壬��SCC����Ϊ���������ݽ���ͳһ����
	CGameInfo��			������Ϣ���ͺͼ�¼ʱ�����Ϣ����ΪGMT3�����߼���Ϣ���ݴ������С��λ
	CGameInfoGroup��	��������CGameInfo�����ͣ�������Ϣ����
*/

//!--------------------------------------------------------------------------------------------
//! ��Ϸ��Ϣ������
struct tagGameInfoData
{
	//!							��¼ʱ��
	time_t						_tTime;

	DWORD 						_dwData1;
	DWORD 						_dwData2;

	tagGameInfoData(VOID){memset(this, 0, sizeof(tagGameInfoData));}
	tagGameInfoData(DWORD dwData){time(&_tTime);_dwData1=dwData;_dwData2=0;}
};

//! ��SCC����ʹ��
struct tagDbGameInfo : public tagGameInfoData
{
	DWORD dwInfoType;
};

//!--------------------------------------------------------------------------------------------
//! һ����Ϸ��Ϣ
class CGameInfo
{
public:
	CGameInfo(DWORD dwType):m_dwType(dwType) {}

public:
	//!							�����Ϣ����
	DWORD						GetType(VOID)	{return m_dwType;}

	//!							���뵽���ݼ�
	BOOL						Code(DBWriteSet &setWriteDB) const;

    BOOL                        Code(Json::Value& var) const;
	//!							�����ݼ�����
	BOOL						DeCode(DBReadSet &setReadDB);

	//!							���һ����Ϣ
	VOID						AddInfo(tagGameInfoData &GameInfoData) {m_vGameInfoData.push_back(GameInfoData);}


protected:
	//!							��Ϣ����
	DWORD						m_dwType;
	//!							��Ϣ����
	vector<tagGameInfoData>		m_vGameInfoData;
};

