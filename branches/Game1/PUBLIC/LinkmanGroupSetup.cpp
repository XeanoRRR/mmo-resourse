
#include "StdAfx.h"

#include "LinkmanGroupSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//! ������
///////////////////////////////////////////////////////////////////////////////////////////////////
LinkmanGroupSetup::LinkmanGroupSetup(void)
{
	Reset();
}

LinkmanGroupSetup::~LinkmanGroupSetup(void)
{
}

//! ���ö���
void LinkmanGroupSetup::Reset(void)
{
	memset(this, 0, sizeof(LinkmanGroupSetup));
}

//! �Ӵ����XML�ڵ��л�ȡ������Ϣ
BOOL LinkmanGroupSetup::LoadSetup(TiXmlElement* pGroupElement)
{
	if (NULL == pGroupElement) return FALSE;

	BOOL bRe = TRUE;
	const char *pSetupStr = NULL;
	//! ----------------------------����----------------------------
	TiXmlElement* pBase = pGroupElement->FirstChildElement("Base");
	if (NULL == pBase) return FALSE;

	//! ���ö����ΨһID
	m_uID = atoi(pBase->Attribute("ID"));

	//! ����(eLinkmanGroupType)
	if (NULL != (pSetupStr = pBase->Attribute("GroupType")))
	{
		if		(0 == stricmp(pSetupStr, "eLGT_Friend"))	m_uGroupType = eLGT_Friend;
		else if (0 == stricmp(pSetupStr, "eLGT_Spouse"))	m_uGroupType = eLGT_Spouse;
		else if (0 == stricmp(pSetupStr, "eLGT_Stranger"))	m_uGroupType = eLGT_Stranger;
		else if (0 == stricmp(pSetupStr, "eLGT_Hellion"))	m_uGroupType = eLGT_Hellion;
		else if (0 == stricmp(pSetupStr, "eLGT_Blacklist"))	m_uGroupType = eLGT_Blacklist;
		else												m_uGroupType = eLGT_Invalid;
	}
	else
	{
		m_uGroupType = eLGT_Invalid;
		bRe = FALSE;
	}

	//! �����Ա���������ȼ���eLeaveWordPRI��
	m_uLeaveWordPRI = atoi(pBase->Attribute("LeaveWordPRI"));
	if (eLWPRI_UnSteadyest < m_uLeaveWordPRI) bRe = FALSE;


	//! ����Ƶ�����ͣ�eSecretTalkChannel��
	m_uSecretTalkChannelType = atoi(pBase->Attribute("SecretTalkChannelType"));
	if (eSTC_BaseValue >= m_uSecretTalkChannelType || eSTC_MaxValue <= m_uSecretTalkChannelType) bRe = FALSE;

	//! ���û�������
	m_bPartMutual = FALSE;
	if (NULL != (pSetupStr = pBase->Attribute("PartMutual")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bPartMutual = TRUE;
	}
	else
		bRe = FALSE;
	


	//! ----------------------------��Ϊ----------------------------
	TiXmlElement* pActive = pGroupElement->FirstChildElement("Active");
	if (NULL == pActive) return FALSE;

	//! �����������
	m_uMaxMemberNum = atoi(pActive->Attribute("MaxMemberNum"));
	//! �洢��ʽ��eKeepWay��
	if (NULL != (pSetupStr = pActive->Attribute("KeepWay")))
	{
		if		(0 == stricmp(pSetupStr, "eKW_ServerKeep"))	m_uKeepWay = eKW_ServerKeep;
		else if (0 == stricmp(pSetupStr, "eKW_ClientKeep"))	m_uKeepWay = eKW_ClientKeep;
		else												m_uKeepWay = eKW_NotKeep;
	}
	else
	{
		m_uKeepWay = eKW_NotKeep;
		bRe = FALSE;
	}

	//! ��ʱ��ʾ��Ϣ��eTimelyInfoType��
	m_uTimelyInfo = eTIT_Hide_All;
	if (NULL != (pSetupStr = pActive->Attribute("TimelyInfo")))
	{	
		if (0 == stricmp(pSetupStr, "eTIT_Show_All"))	m_uTimelyInfo = eTIT_Show_All;
	}
	else
		bRe = FALSE;

	//! ���ι��ܣ�eFunctionFiltrateType��
	if (NULL != (pSetupStr = pActive->Attribute("FunctionFiltrate")))
	{	
		if		(0 == stricmp(pSetupStr, "eFFT_UuFiltrate_All"))	m_uFunctionFiltrate = eFFT_UuFiltrate_All;
		else if (0 == stricmp(pSetupStr, "eFFT_Filtrate_Chat"))		m_uFunctionFiltrate = eFFT_Filtrate_Chat;
		else if (0 == stricmp(pSetupStr, "eFFT_Filtrate_Team"))		m_uFunctionFiltrate = eFFT_Filtrate_Team;
		else														m_uFunctionFiltrate = eFFT_Filtrate_All;
	}
	else
	{
		m_uFunctionFiltrate = eFFT_Filtrate_All;
		bRe = FALSE;
	}


	//! ----------------------------��Ա����----------------------------
	TiXmlElement* pMemberOpt = pGroupElement->FirstChildElement("MemberOpt");
	if (NULL == pMemberOpt) return FALSE;

	//! �ṩ�û���Ӳ���
	m_bUserAdd = FALSE;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("UserAdd")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bUserAdd = TRUE;
	}
	else
		bRe = FALSE;
	
	//! �Զ���Ӱ취��eAutoAddType������
	if (NULL != (pSetupStr = pMemberOpt->Attribute("AutoAddWay")))
	{
		if	(0 == stricmp(pSetupStr, "eAAT_SecretTalk"))	m_uAutoAddWay = eAAT_SecretTalk;
		else if (0 == stricmp(pSetupStr, "eAAT_Attacker"))	m_uAutoAddWay = eAAT_Attacker;
		else if (0 == stricmp(pSetupStr, "eAAT_Spouse"))	m_uAutoAddWay = eAAT_Spouse;
		else												m_uAutoAddWay = eAAT_NotAdd;
	}
	else
	{
		m_uAutoAddWay = eAAT_NotAdd;
		bRe = FALSE;
	}

	//! ���ʱ�Ƿ�����Ŀ��ȷ��
	m_bAddAimAffirm = FALSE;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("AddAimAffirm")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bAddAimAffirm = TRUE;
	}
	else
		bRe = FALSE;

	//! �ṩ�û�ɾ������
	m_bUserDel = FALSE;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("UserDel")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bUserDel = TRUE;
	}
	else
		bRe = FALSE;

	//! ������˭��ɣ�eGroupOperatorType��
	m_uOperater = eGOT_Client;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("Operater")))
	{	
		if (0 == stricmp(pSetupStr, "eGOT_Server"))	m_uOperater = eGOT_Server;
	}
	else
		bRe = FALSE;

	//! ��Ա�Ƿ����Ψһ��
	m_bMemberIsOnly = FALSE;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("MemberIsOnly")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bMemberIsOnly = TRUE;
	}
	else
		bRe = FALSE;

	//! ��Ա�ƶ����ȼ���eOnlyPRT��
	m_uOnlyPRT = atoi(pMemberOpt->Attribute("OnlyPRT"));
	if (eOPRI_Steadyest > m_uOnlyPRT || eOPRI_UnSteadyest < m_uOnlyPRT) bRe = FALSE;

	//! ͬ��ɾ��
	m_bSameDel = FALSE;
	if (NULL != (pSetupStr = pMemberOpt->Attribute("SameDel")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_bSameDel = TRUE;
	}
	else
		bRe = FALSE;



	//! ----------------------------��ʾ----------------------------
	TiXmlElement* pNotify = pGroupElement->FirstChildElement("Notify");
	if (NULL == pNotify) return FALSE;

#ifndef USE_IN_SERVER
	//! ������ʾ��Ϣ
	m_NotifySetup.uMessage_Login = FALSE;
	if (NULL != (pSetupStr = pNotify->Attribute("Message_Login")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_NotifySetup.uMessage_Login = TRUE;
	}
	else
		bRe = FALSE;

	//! ������ʾ��Ϣ
	m_NotifySetup.uMessage_Logout = FALSE;
	if (NULL != (pSetupStr = pNotify->Attribute("Message_Logout")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_NotifySetup.uMessage_Logout = TRUE;
	}
	else
		bRe = FALSE;

	//! ������ʾ��
	if (NULL != (pSetupStr = pNotify->Attribute("Music_Login")))
	{
		LONG lCpySize = (MAX_FILE_PATH_SIZE > strlen(pSetupStr)) ? strlen(pSetupStr) : MAX_FILE_PATH_SIZE;
		memmove(m_NotifySetup.szMusic_Login, pSetupStr, lCpySize);
	}
	else
		bRe = FALSE;

	//! ������ʾ��
	if (NULL != (pSetupStr = pNotify->Attribute("Music_Logout")))
	{
		LONG lCpySize = (MAX_FILE_PATH_SIZE > strlen(pSetupStr)) ? strlen(pSetupStr) : MAX_FILE_PATH_SIZE;
		memmove(m_NotifySetup.szMusic_Logout, pSetupStr, lCpySize);
	}
	else
		bRe = FALSE;

	//! �յ�������ʾ��
	if (NULL != (pSetupStr = pNotify->Attribute("Music_SecretTalk")))
	{
		LONG lCpySize = (MAX_FILE_PATH_SIZE > strlen(pSetupStr)) ? strlen(pSetupStr) : MAX_FILE_PATH_SIZE;
		memmove(m_NotifySetup.szMusic_SecretTalk, pSetupStr, lCpySize);
	}
	else
		bRe = FALSE;

#endif //! USE_IN_SERVER
	
	//! ����ӵ�Ŀ�귢����ʾ��Ϣ
	m_NotifySetup.bMessage_AddAim = FALSE;
	if (NULL != (pSetupStr = pNotify->Attribute("Message_AddAim")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_NotifySetup.bMessage_AddAim = TRUE;
	}
	else
		bRe = FALSE;

	//! ��ɾ����Ŀ�귢����ʾ��Ϣ
	m_NotifySetup.bMessage_DelAim = FALSE;
	if (NULL != (pSetupStr = pNotify->Attribute("Message_DelAim")))
	{	
		if (0 == stricmp(pSetupStr, "YES"))	m_NotifySetup.bMessage_DelAim = TRUE;
	}
	else
		bRe = FALSE;

	//! ----------------------------���----------------------------
#ifndef USE_IN_SERVER
	TiXmlElement* pAppearance = pGroupElement->FirstChildElement("Appearance");
	if (NULL == pAppearance) return FALSE;

	//! ����ɫ
	m_ColorSetup.uNameColor_Online			= strtoul(pAppearance->Attribute("NameColor_Online"), NULL, 0);

	//! ����ɫ
	m_ColorSetup.uNameColor_Leave			= strtoul(pAppearance->Attribute("NameColor_Leave"), NULL, 0);

	//! ���ӹ�ϵ����ɫ
	m_ColorSetup.uNameColor_Online_Mutual	= strtoul(pAppearance->Attribute("NameColor_Online_Mutual"), NULL, 0);

	//! ��ɫ��ɾ��ɫ
	m_ColorSetup.uNameColor_Player_Deleted	= strtoul(pAppearance->Attribute("NameColor_Leave_Mutual"), NULL, 0);

	//! ������ʾɫ
	m_ColorSetup.uSecretTalkColor			= strtoul(pAppearance->Attribute("SecretTalkColor"), NULL, 0);

	//! �����ɫ�ƺ�
	if (NULL != (pSetupStr = pAppearance->Attribute("MemberAppellative")))
	{
		LONG lCpySize = (MAX_APPELLATIVE_SIZE > strlen(pSetupStr)) ? strlen(pSetupStr) : MAX_APPELLATIVE_SIZE;
		memmove(m_szMemberAppellative, pSetupStr, lCpySize);
	}
	else
		bRe = FALSE;

	//! �����ļ���
	if (NULL != (pSetupStr = pAppearance->Attribute("FileName")))
	{
		LONG lCpySize = (MAX_APPELLATIVE_SIZE > strlen(pSetupStr)) ? strlen(pSetupStr) : MAX_APPELLATIVE_SIZE;
		memmove(m_szFileName, pSetupStr, lCpySize);
	}
	else
		bRe = FALSE;
#endif //! USE_IN_SERVER

	return TRUE;
}
















//! ���ù�����
///////////////////////////////////////////////////////////////////////////////////////////////////
LGSManage::LGSManage(void)
{
	m_bLoadSucceed = FALSE;
}
LGSManage::~LGSManage(void)
{
}

//! ��ȡ����
BOOL LGSManage::LoadSetup (const char *pFilename)
{
	//! �����������ɹ����������ٶ����޸�
	if (m_bLoadSucceed) return FALSE;

	BOOL bRe = TRUE;
	//! ----------------------------���ڵ� �������ڵĴ���Ӧ����1~10----------------------------
	TiXmlNode *pNode = NULL; 		

	CRFile *prfile = rfOpen(pFilename);
	if(NULL == prfile)
	{
		//���������Ϣ
		return FALSE;
	}

	TiXmlDocument m_Tdoc(pFilename);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);

	do 
	{
		pNode=m_Tdoc.FirstChild("LinkmanSetup");
		if (pNode==NULL) break;

		TiXmlElement* pLinkmanSetup = pNode->ToElement();//��ȡnode ��ָ��
		if (pLinkmanSetup==NULL) break;

		//! ��ʼ��ȡ
		m_LinkmanGroupSetup.clear();
		LinkmanGroupSetup tmpLinkmanGroupSetup;

		//! ID�ظ���֤�õļ���
		set<LONG> idSet;

		TiXmlElement* pGroup = pLinkmanSetup->FirstChildElement("Group");
		for (;pGroup != NULL; pGroup = pGroup->NextSiblingElement("Group"))
		{
			tmpLinkmanGroupSetup.Reset();
			if(!tmpLinkmanGroupSetup.LoadSetup(pGroup)) bRe = FALSE;
			m_LinkmanGroupSetup.push_back(tmpLinkmanGroupSetup);
			idSet.insert(tmpLinkmanGroupSetup.m_uID);
		}

		if(m_LinkmanGroupSetup.size() > idSet.size()) bRe = FALSE;
		
	} while(FALSE);

	m_bLoadSucceed = (bRe) ? TRUE : FALSE;
	return bRe;
}


//! �õ����õĺ���������
LONG LGSManage::GetGroupNum(void)
{
	if (m_bLoadSucceed) 
		return (LONG)m_LinkmanGroupSetup.size();
	return 0;
}

//! ��ָ����˳��õ�һ���������
const LinkmanGroupSetup* LGSManage::GetLinkmanGroupSetup_ByOrder(LONG lOrder)
{
	if (-1 < lOrder && lOrder < m_LinkmanGroupSetup.size() && m_bLoadSucceed)
			return &(m_LinkmanGroupSetup[lOrder]);
	return NULL;
}

//! ͨ��ID�õ�������˳��
LONG LGSManage::GetIndexByID(ULONG uID)
{
	for (LONG i = 0; i < m_LinkmanGroupSetup.size(); ++i)
	{
		if (m_LinkmanGroupSetup[i].m_uID == uID)
		{
			return i;
		}
	}
	return -1;
}

//! ͨ�������͵õ�������˳��
LONG LGSManage::GetIndexByType(eLinkmanGroupType eRelationType)
{
	for (LONG i = 0; i < m_LinkmanGroupSetup.size(); ++i)
	{
		if (m_LinkmanGroupSetup[i].m_uGroupType == (LONG)eRelationType)
		{
			return i;
		}
	}
	return -1;
}



//!----------------------------------------------------------------------------------------------------------
#ifdef USE_IN_SERVER

//! ����
void LGSManage::AddToByteArray(vector<BYTE> *pVecData)
{
	_AddToByteArray(pVecData, (LONG)m_bLoadSucceed);

	LONG lSize = m_LinkmanGroupSetup.size();
	_AddToByteArray(pVecData, lSize);
	for (LONG i = 0; i < lSize; ++i)
	{
		_AddToByteArray(pVecData, &(m_LinkmanGroupSetup[i]), sizeof(LinkmanGroupSetup));
	}
}

//! ����
void LGSManage::DecordFromByteArray(BYTE* pByte, LONG& pos)
{
	m_bLoadSucceed = _GetLongFromByteArray(pByte, pos);
	if (m_bLoadSucceed)
	{
		m_LinkmanGroupSetup.clear();
		LONG lSize = _GetLongFromByteArray(pByte, pos);
		for (LONG i = 0; i < lSize; ++i)
		{
			LinkmanGroupSetup tmpLinkmanGroupSetup;
			_GetBufferFromByteArray(pByte, pos, &tmpLinkmanGroupSetup, sizeof(LinkmanGroupSetup));
			m_LinkmanGroupSetup.push_back(tmpLinkmanGroupSetup);
		}
	}
}

#endif //! USE_IN_SERVER
//!----------------------------------------------------------------------------------------------------------