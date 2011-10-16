//=============================================================================
/**
 *  file: TreeGameClient.h
 *
 *  Brief:���ؼ���������ʾ��Ϸ�����е�GameClient�����Ӷ���
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================

#pragma once
class CGUID;
//monster
struct tagNodeInfo 
{
	long	lClientID;
	long	lType;
	char	strShowName[64];
	tagNodeInfo()
	{
		memset(this,0,sizeof(tagNodeInfo));
	}
};
// client
struct tagClientInfo
{
	long	lClientID;
	long	lType;
	char	strShowName[32];
	tagNodeInfo	*m_RegionInfo;
	tagNodeInfo	*m_MonsterInfo;
	tagNodeInfo	*m_GoodsInfo;
	tagNodeInfo	*m_MainPlayerInfo;
	tagNodeInfo	*m_SkillInfo;
	tagClientInfo()
	{
		lClientID = 0;
		lType = 0;
		m_RegionInfo = new tagNodeInfo;
		m_MonsterInfo = new tagNodeInfo;
		m_GoodsInfo = new tagNodeInfo;
		m_MainPlayerInfo = new tagNodeInfo;
		m_SkillInfo = new tagNodeInfo;
	}
};
//========================================================================================================================
class CGameClient;
class CShape;

class CTreeGameClient : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeGameClient)

public:
	CTreeGameClient();
	virtual ~CTreeGameClient();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

private:
	tagNodeInfo *m_pSelNode;									// ��ǰѡ��Ľ��
	list<tagClientInfo*>	m_pGameClientNode;					// client��	
	long m_lGameState;								//client״̬
public:
	list<tagClientInfo*>	GetGameClientNode()	{	return m_pGameClientNode; }
	void DelGameClient(long lClient);
	tagNodeInfo *GetSelectNode()	{	return m_pSelNode; }
	//����ڵ�
	HTREEITEM	InsertDataItem(HTREEITEM hParentItem, tagClientInfo *pNode);
	void InsertClientItem(CGameClient* pClient);
	void InsertShapeItem(CGameClient* pClient,CShape* pShape);

	HTREEITEM FindClientItem(long lClientID);
	HTREEITEM FindClientRegionItem(long lClientID,long lRegionID);
	HTREEITEM FindShapeItem(long lClientID,long lRegionID,long lType,long lShapeID);
	HTREEITEM FindChildItem(HTREEITEM hParent,long lType,long lShapeID=-1);

	//ɾ���ڵ�
	BOOL DelAllChildItem(HTREEITEM hParentItem);
	BOOL DelClientItem(long lClientID);
	BOOL DelShapeItem(long lClientID,long lRegionID,long lShapeType,long lShapeID);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	//����һ��client����
	HTREEITEM InsertClientDataItem(HTREEITEM hParent,tagClientInfo *pNode);
	HTREEITEM InsertRegionDataItem(HTREEITEM hParent,tagClientInfo *pNode);
	HTREEITEM InsertMonsterDataItem(HTREEITEM hParent,tagClientInfo *pNode);
	HTREEITEM InsertGoodsDataItem(HTREEITEM hParent,tagClientInfo *pNode);
	HTREEITEM InsertMainPlayerDataItem(HTREEITEM hParent,tagClientInfo *pNode);
	HTREEITEM InsertSkillDataItem(HTREEITEM hParent,tagClientInfo *pNode);

};


