


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

#include "../../public/LinkmanGroupSetup.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////
class LinkmanGroup
{
public:
	explicit LinkmanGroup(const LinkmanGroupSetup* pSetup);
	~LinkmanGroup(void);

	void									Release							(void);
public:
	//!										���һ����ϵ��
	BOOL									AddMember						(tagLinkman_DB &Linkman_DB);
	//!										ɾ��һ����ϵ��
	BOOL									DelMember						(const CGUID &PlayerGuid);
	//!										�ҵ�һ����ϵ��
	CGUID									FindMember						(const char *pPlayerName);

	//!										��Ա��ͼ�ı�
	void									MemberMapChange					(const CGUID &PlayerGuid, const char *pMapName);
	//!										��Ա�ȼ��ı�
	void									MemberLevelChange				(const CGUID &PlayerGuid, LONG lLevel);
	//!										��Աְҵ�ı�
	void									MemberOccuChange				(const CGUID &PlayerGuid, LONG lOccu);

	//!										���һ�������Ϊ��ϵ�˵���
	BOOL									AddCompriseMePlayer				(tagLinkman_DB &Linkman_DB);
	//!										ɾ��һ�������Ϊ��ϵ�˵���
	BOOL									DelCompriseMePlayer				(const CGUID &PlayerGuid);
	//!										�ҵ�һ�������Ϊ��ϵ�˵���
	BOOL									FindCompriseMePlayer			(const CGUID &PlayerGuid);

	//!										һ����ϵ������
	void									LinkmanLogin					(const CGUID &PlayerGuid);
	//!										һ����ϵ������
	void									LinkmanLogout					(const CGUID &PlayerGuid);
	//!										��ϵ�˵�ɾ��״̬�ı�
	void									LinkmanDelStateChange			(const CGUID &PlayerGuid, BOOL bDeleted);

	//!										������ϵ������״̬
	void									UpdateOnlineState				(void);
	//!										���뵽��Ϣ
	void									Serialize_ForGs					(CMessage *pMsg);
	//!										��GS�������������н���
	void									Decode_FromGs					(BYTE *pData, LONG &lPos);
	//!										ת����Ա�б�����
	void									Serialize_ForDBS				(vector<tagLinkman_DB> &vLinkman);
	//��									ת������ҵ����ID�б�����
	void									AddMeGuidToVector				(vector<tagLinkman_DB> &vLinkman_DB);

	//!										ȡ������ӵ����
	const	map<CGUID, tagMeAddPlayer>&		GetMeAddMap						(void);
	//!										ȡ������ҵ����
	const	map<CGUID, tagLinkman_DB>&		GetAddMeMap						(void);

	//!										�õ�����ӵ�����
	LONG									GetMemberNum					(void);


private:
	//!										����ӵ����
	map<CGUID, tagMeAddPlayer>				m_mapMeAdd;
	//!										����ҵ����
	map<CGUID, tagLinkman_DB>				m_mapAddMe;

	//!										�������ָ��
	const LinkmanGroupSetup*				m_pSetup;


	//! ��̬��Ա
	//!-----------------------------------------------------------------------------------------------
public:
	//!										��������״̬
	static	void							UpdateLinkman_data				(tagLinkman_data* pLinkman_data);
private:
	//!										��Ҫ�󴴽�һ��tagLinkman_data����
	static	tagLinkman_data*				CreateLinkman_data				(const CGUID &PlayerGuid, const char *pPlayerName);
	//!										�ͷ�һ��tagLinkman_data����
	static	void							ReleaseLinkman_data				(tagLinkman_data **ppLinkman_data);
};



//!			��ҵĺ�������
typedef		vector<LinkmanGroup>	VecPlayerLinkMan;





///////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��������
//!		������ϵ�˵����ݵ�׼ȷ����Ҫȫ�ֵ�������ȷ������DB�ı�����һ������ʱ��������ڶ�ȡʱ��һЩ����ƫ�
//!		����ʵ�ֵ�Ŀ�ľ���Ϊ�ܹ�Ԥ�Ƶ�����Щ����ƫ����ṩЧ��Ļ�������
class LinkmanOptCache
{
public:
	LinkmanOptCache(void);
	~LinkmanOptCache(void);

public:
	//!					����һ����Ӳ���
	void				PushLinkmanAddOpt(tagLinkman_DB &Linkman_DB);
	//!					����һ��ɾ������
	void				PushLinkmanDelOpt(tagLinkman_DB &Linkman_DB);

	//!					ȷ��һ������û�б�ɾ��
	BOOL				AffirmLinkmanDataBeing(tagLinkman_DB &Linkman_DB);
	//!					����һ����ұ���Щ�������
	void				FindNewAddPlayerData(list<tagLinkman_DB> &listLinkman_DB, const CGUID& AimGuid);

	//!					���һ����ҵĻ���
	void				ClearOnePlayerData(const CGUID& OwnerGuid);

private:
	//! ���ݱȽϷº���
	class OptCacheData_compare
	{
	public:
		inline bool operator()( const tagLinkman_DB& Key1, const tagLinkman_DB& Key2 ) const
		{
			return Key1 < Key2;
		}
	};

	//!												��Ӳ�������
	set<tagLinkman_DB, OptCacheData_compare>		m_AddCache;
	//!												ɾ����������
	set<tagLinkman_DB, OptCacheData_compare>		m_DelCache;
};