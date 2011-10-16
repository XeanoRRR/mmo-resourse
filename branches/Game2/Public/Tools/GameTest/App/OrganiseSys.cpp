


#include "stdafx.h"
#include "OrganiseSys.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COrganiseSys::COrganiseSys()
{

}

COrganiseSys::~COrganiseSys()
{
	Release();
}

void COrganiseSys::Release()
{
}

void COrganiseSys::DecodeFromDataBlock(DBReadSet& dbRead,bool bSelf)
{
	// �л������ GUID
	dbRead.GetBufferFromByteArray(m_FamilyGuid);
	dbRead.GetBufferFromByteArray(m_UnionGuid);

	if((BOOL)dbRead.GetByteFromByteArray())
	{
		LONG lFacDemiseWarning = 0;
		LONG lFacDisbandWarning = 0;

		if( bSelf )
		{
			lFacDemiseWarning = dbRead.GetLongFromByteArray(); //! ��ᴫλ���ѵ���ʱ
			lFacDisbandWarning = dbRead.GetLongFromByteArray(); //! ����ɢ���ѵ���ʱ
		}

		// �ƺ�
		TCHAR szFactionTitle[ MAX_MEMBER_NAME_SIZE ];
		dbRead.GetStringFromByteArray( szFactionTitle, MAX_MEMBER_NAME_SIZE );

		// �л�ͼ��CRC
		m_dwFacIconCRC = dbRead.GetDwordFromByteArray();

		// PUBLIC��Ϣ��Ȩ�ޡ��ٽ�
		tagFacPublicData stFacPublicData;
		ZeroMemory( &stFacPublicData, sizeof(stFacPublicData) );

		TCHAR	szPronounce[ MAX_PronounceCharNum ];
		ZeroMemory( szPronounce, sizeof(szPronounce) );

		long lMyJobPurview = 0;

		if( bSelf )
		{
			// ��ȡPUBLIC��Ϣ
			dbRead.GetBufferFromByteArray( &stFacPublicData, sizeof(tagFacPublicData) );
			// ��ȡ�ٽ�
			m_lFamilyJobLevel = dbRead.GetLongFromByteArray();

			// ��ȡȨ������
			lMyJobPurview = dbRead.GetLongFromByteArray();

			// ����
			dbRead.GetStringFromByteArray( szPronounce, MAX_PronounceCharNum);
		}
		else
		{
			dbRead.GetStringFromByteArray( stFacPublicData.szName, MAX_MEMBER_NAME_SIZE );
		}

		// ������ƺͳƺ�
		m_strFamilyName = stFacPublicData.szName;
		m_strFamilyTitle = szFactionTitle;

		// ����һ���л�
		//CFactionManager*	pFactionManager = GetGame()->GetFactionManager();

		//FamilyInfo	stFacInfo;
		//ZeroMemory( &stFacInfo, sizeof(stFacInfo) );
		//stFacInfo.FamilyGuid				 = m_FamilyGuid;
		//strcpy(stFacInfo.strName,stFacPublicData.szName);
		//strcpy(stFacInfo.strPlacard,szPronounce);
		//stFacInfo.arrFunLevel[ FUN_FACTION ] = stFacPublicData.lLevel;
		//stFacInfo.tMasterLastOnlineTime		 = (time_t)lFacDemiseWarning;
		//stFacInfo.tLessMemNumTime			 = (time_t)lFacDisbandWarning;
	}
}