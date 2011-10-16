
/*
*	file:		LinkmanGroup.h
*	Brief:		��������
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-20
*/

#pragma once

#include "../../../../Public/Common/LinkmanDef.h"
#include "../../../../Public/Setup/LinkmanGroupSetup.h"

class LinkmanGroup
{
public:
	explicit LinkmanGroup(const LinkmanGroupSetup* pSetup, const CGUID &OwnerGuid);
	~LinkmanGroup(void);

public:
	//!										���һ����ϵ��
	void									AddMember						(tagLinkman &Linkman, BOOL bSend2Client);
	//!										ɾ��һ����ϵ��
	void									DelMember						(const CGUID &PlayerGuid, BOOL bSend2Client, const char *pAimName);
	//!										�ҵ�һ����ϵ��
	BOOL									FindMember						(const CGUID &PlayerGuid);
	//!										�ҵ�һ����ϵ�˵�GUID
	CGUID									FindMemberID					(const char *pPlayerName);
	//!										�õ���ϵ������
	LONG									GetMemberNum					(void);
	//!										�õ���һ����ϵ�˵�GUID
	const	tagLinkman*						GetFirstMember					(void);

	//!										���һ�������Ϊ��ϵ�˵���
	void									AddCompriseMePlayer				(tagLinkman_DB &Linkman, const char *pOptName);
	//!										ɾ��һ�������Ϊ��ϵ�˵���
	void									DelCompriseMePlayer				(const CGUID &PlayerGuid, const char *pOptName);

	//!										һ����ϵ������
	void									LinkmanLogin					(tagLinkman_data &Linkman_data);
	//!										һ����ϵ������
	void									LinkmanLogout					(const CGUID &PlayerGuid);
	//!										һ����ϵ�˵�ͼ�ı�
	void									LinkmanMapChange				(const CGUID &PlayerGuid, const char *pMapName);
	//!										һ����ϵ�˵ȼ��ı�
	void									LinkmanLevelChange				(const CGUID &PlayerGuid, LONG lLevel);
	//!										һ����ϵ��ְҵ�ı�
	void									LinkmanOccuChange				(const CGUID &PlayerGuid, LONG lOccu);
	//!										һ����ϵ��ɾ��״̬�ı�
	void									LinkmanDelStateChange			(const CGUID &PlayerGuid, BOOL bDeleted);

	//!										�����ʼ������
	void									Decode_FromWs					(CMessage *pMsg);
	//!										���뵽�ͻ���
	void									Serialize_ForClient				(CMessage *pMsg);
	//!										���뵽WS
	void									Serialize_ForWS					(vector<BYTE> *pVecData);
	//!
	void									AddMemberToVec					(vector<tagLinkman> &vLinkman);
	void									AddCompriseMePlayerToVec		(vector<tagLinkman_DB> &vCompriseMePlayer);

private:
	//!										����ӵ����
	map<CGUID, tagLinkman>					m_mapMeAdd;
	//!										����ҵ����
	map<CGUID, tagLinkman_DB>				m_mapAddMe;
	//!										�������ָ��
	const LinkmanGroupSetup*				m_pSetup;
	//!										������ID
	CGUID									m_OwnerGuid;

};


//!									��ҵ���ϵ������
typedef		vector<LinkmanGroup>	VecLinkmanGroup;