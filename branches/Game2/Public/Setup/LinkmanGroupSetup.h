

/*
*	file:		LinkmanGroupSetup.h
*	Brief:		������������	LinkmanGroupSetup
*				���ù�����		LGSManage
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-18
*/

#pragma once

#include "../DefPublic.h"
#include "../Common/LinkmanDef.h"

//! ������
///////////////////////////////////////////////////////////////////////////////////////////////////
class LinkmanGroupSetup
{
public:
	LinkmanGroupSetup(void);
	~LinkmanGroupSetup(void);

public:
	//!							���ö���
	void						Reset(void);
	//!							�Ӵ����XML�ڵ��л�ȡ������Ϣ
	bool						LoadSetup(TiXmlElement* pGroupElement);

	//! ����
	//! ----------------------------------------------------------------------------------
public:
	//!							���ö����ΨһID
	ULONG						m_uID;
	//!							����(eLinkmanGroupType)
	ULONG						m_uGroupType;
	//!							�����Ա���������ȼ���eLeaveWordPRI��
	ULONG						m_uLeaveWordPRI;
	//��						����Ƶ�����ͣ�eSecretTalkChannel��
	ULONG						m_uSecretTalkChannelType;
	//��						���û�������
	bool						m_bPartMutual;
	

	//! ��Ϊ
	//! ----------------------------------------------------------------------------------
public:
	//!							�����������
	ULONG						m_uMaxMemberNum;
	//!							�洢��ʽ��eKeepWay��
	ULONG						m_uKeepWay;
	//��						��ʱ��ʾ��Ϣ��eTimelyInfoType��
	ULONG						m_uTimelyInfo;
	//��						���ι��ܣ�eFunctionFiltrateType��
	ULONG						m_uFunctionFiltrate;

	

	//! ��Ա����
	//! ----------------------------------------------------------------------------------
public:
	//!							�ṩ�û���Ӳ���
	bool						m_bUserAdd;
	//!							�Զ���Ӱ취��eAutoAddType��
	ULONG						m_uAutoAddWay;
	//!							���ʱ�Ƿ�����Ŀ��ȷ��
	bool						m_bAddAimAffirm;

	//!							�ṩ�û�ɾ������
	bool						m_bUserDel;
	
	//!							������˭��ɣ�eGroupOperatorType��
	ULONG						m_uOperater;
	//��						��Ա�Ƿ����Ψһ��
	bool						m_bMemberIsOnly;
	//��						��Ա�ƶ����ȼ���eOnlyPRT��
	ULONG						m_uOnlyPRT;
	//��						ͬ��ɾ��
	bool						m_bSameDel;
	

	//! ��ʾ
	//! ----------------------------------------------------------------------------------
public:
	//! ��ʾ���ýṹ
	struct tagNotifySetup 
	{
//#ifndef USE_IN_SERVER
		//! 			������ʾ��Ϣ
		ULONG			uMessage_Login;
		//! 			������ʾ��Ϣ
		ULONG			uMessage_Logout;
		//! 			������ʾ��
		char			szMusic_Login[MAX_FILE_PATH_SIZE];
		//! 			������ʾ��
		char			szMusic_Logout[MAX_FILE_PATH_SIZE];
		//! 			�յ�������ʾ��
		char			szMusic_SecretTalk[MAX_FILE_PATH_SIZE];
//#endif //! USE_IN_SERVER
		//! 			����ӵ�Ŀ�귢����ʾ��Ϣ
		bool			bMessage_AddAim;
		//! 			��ɾ����Ŀ�귢����ʾ��Ϣ
		bool			bMessage_DelAim;
	};

	//!							��ʾ��Ϣ
	tagNotifySetup				m_NotifySetup;

	//! ���
	//! ----------------------------------------------------------------------------------
public:

//#ifndef USE_IN_SERVER
	//! ��ɫ���ýṹ
	struct tagColorSetup 
	{
		//!				����ɫ
		ULONG			uNameColor_Online;
		//!				����ɫ
		ULONG			uNameColor_Leave;
		//!				���ӹ�ϵ����ɫ
		ULONG			uNameColor_Online_Mutual;
		//!				��ɫ��ɾ��ɫ
		ULONG			uNameColor_Player_Deleted;
		//!				������ʾɫ
		ULONG			uSecretTalkColor;

	};

	//! ��ɫ����
	tagColorSetup				m_ColorSetup;
	//! �����ɫ�ƺ�
	char						m_szMemberAppellative[MAX_APPELLATIVE_SIZE];
	char						m_szFileName[MAX_APPELLATIVE_SIZE];

//#endif //! USE_IN_SERVER


};



//! ���ù�����
///////////////////////////////////////////////////////////////////////////////////////////////////
class LGSManage
{
public:
	LGSManage(void);
	~LGSManage(void);

public:
	//!							��ȡ����
	bool						LoadSetup						(const char *pFilename);
	//!							�õ����õĺ���������
	ulong						GetGroupNum						(void);
	//!							��ָ����˳��õ�һ���������
	const	LinkmanGroupSetup*	GetLinkmanGroupSetup_ByOrder	(ulong lOrder);
	//!							ͨ��ID�õ�������˳��
	ulong						GetIndexByID					(ulong uID);
	//!							ͨ�������͵õ�������˳��
	ulong						GetIndexByType					(eLinkmanGroupType eRelationType);


//#ifdef USE_IN_SERVER
	//!							����
	void						AddToByteArray					(vector<uchar> *pVecData);
	//!							����
	void						DecordFromByteArray				(uchar* pByte, long& pos);

//#endif //! USE_IN_SERVER

private:
	//!							��������
	vector<LinkmanGroupSetup>	m_LinkmanGroupSetup;
	//!							��ȡ�ɹ����
	bool						m_bLoadSucceed;
};