

/*
*	file:		LinkmanSystem.h
*	Brief:		��ϵ�˹�����
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-27
*/

#include "LinkmanGroup.h"


class LinkmanSystem
{
public:
	LinkmanSystem(void);
	~LinkmanSystem(void);

public:
	//!								�õ�Ψһʵ��
	static	LinkmanSystem&			GetInstance(void);

public:
	//!								�ж�һ����ҡ�OwnerGuid���Ƿ�����һ����ҡ�pOtherOneName��������ָ���Ĺ�ϵ
	BOOL							CheckRelation(const CGUID &OwnerGuid, const char *pOtherOneName, eLinkmanGroupType eRelationType);
	//!								�ж�һ����ҡ�OwnerGuid���Ƿ�����һ����ҡ�OtherOneGuid��������ָ���Ĺ�ϵ
	BOOL							CheckRelation(const CGUID &OwnerGuid, const CGUID &OtherOneGuid, eLinkmanGroupType eRelationType);
	

	//! ��ӦWS��Ϣ
public:
	//!								��ʼ������
	void							InitSetup(BYTE* pByte, LONG& pos);
	//!								��Ӧ��ʼ����Ϣ
	void							OnInitMessage(CMessage *pMsg);
	//!								����������֪ͨ
	void							OnReleaseMessage(CMessage *pMsg);

	//!								��Ӧ��ϵ�˵�½��Ϣ
	void							OnLinkmanLoginMessage(CMessage *pMsg);
	//!								��Ӧ��ϵ��������Ϣ
	void							OnLinkmanLogoutMessage(CMessage *pMsg);
	//!								��Ӧ�����Ϣ�ı���Ϣ
	void							OnPlayerInfoChange(CMessage *pMsg);
	//!								��Ӧ��ϵ��ɾ��״̬�ı�
	void							OnPlayerDelStateChange(CMessage *pMsg);

	//!								��Ӧ�����ϵ�˲���
	void							OnAddLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lGroupID, tagLinkman_data &Linkman_data);
	//!								��Ӧɾ����ϵ�˲���
	void							OnDelLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lGroupID, const CGUID &AimGuid, const char *pAimName);
	//!								��Ӧ�ƶ���ϵ�˲���
	void							OnMoveLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lSrcGroupID, LONG lDesGroupID, tagLinkman_data &Linkman_data);


	//!	��Ӧ�ͻ�����Ϣ
public:						
	//!								��Ӧ���������ϵ��
	void							OnRequestAdd(CMessage *pMsg);
	//!								��Ӧ����ɾ����ϵ��
	void							OnRequestDel(CMessage *pMsg);
	//!								��Ӧ�����ƶ���ϵ��
	void							OnRequestMove(CMessage *pMsg);
	//!								��Ӧ��һظ������
	void							OnAnswerAdd(CMessage *pMsg);
	
public:
	//!								������ϵ�����ݵ�WS
	void							SendDataToWs(void);
	//!								��Ӧ����˳�������
	void							OnPlayerExit(const CGUID &PlayerGuid);
	//!								��¼PK��������
	void							NotePkPlayer(CPlayer *pDecedent, CPlayer *pMurderer);
	//!								��¼������
	void							AddSpouse(CPlayer *pSelf, CPlayer *pSide);
	//!								ɾ��������
	void							DelSpouse(CPlayer *pSelf);
	//!								�ҵ���������Ϣ
	const	tagLinkman*				FindSpouseInfo(CPlayer *pSelf);




private:
	//!											����ϵͳ�������
	LGSManage									m_LGSManage;
	//!											������ҵ���ϵ������
	map<CGUID, VecLinkmanGroup>					m_mapPlayerLinkman;


	
	
};