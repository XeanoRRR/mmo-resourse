#pragma once
// ���οؼ���ͷ�ļ�
#include "afxcmn.h"
#include "Region.h"
#include "RegionList.h"
#include "ListLog.h"

#include <fstream>
#include <string>
#include <list>
#include <map>
#include <vector>

#define		ITEM_ROOT				0
#define		ITEM_REGION_UNEXISTED	1

using namespace std;

typedef map<unsigned, CRegion *>				MAPTREE;
typedef map<unsigned, CRegion *>::iterator		ITRTREE;
typedef list<CRegionList *>						LISTREGION;
typedef list<CRegionList *>::iterator			ITRREGION;
struct tagMonsterName
{
	string strOrigName;
	string strName;
};
struct tagCollectionName
{
	string strOrigName;
	string strName;
};
typedef vector<tagMonsterName *>	VECMONSTERNAME;
typedef VECMONSTERNAME::iterator	VECMONSTERNAMEITR;
typedef vector<tagCollectionName *>	VECCOLLECTIONNAME;
typedef VECCOLLECTIONNAME::iterator	VECCLOOECTIONNAMEITR;

const string strFileRegion	= _T("Setup\\regionlist.ini");	// �����б��ļ�

enum { ITEM_LAYER_ROOT = 0, ITEM_LAYER_REGION, ITEM_LAYER_TYPE, ITEM_LAYER_EDIT,
ITEM_LAYER_MONSTER,ITEM_LAYER_COLLECTION,ITEM_LAYER_SIGNPOS};
enum { ITEM_OFFSET_MONSTER = 0, ITEM_OFFSET_NPC,ITEM_OFFSET_SIGNPOS,ITEM_OFFSET_ROADS};
enum { ERROR_NONE = 0, ERROR_RECT};

class CRegionTree :
	public CTreeCtrl
{
public:
	CRegionTree(void);
public:
	~CRegionTree(void);

// ��ʼ��
public:
	BOOL InitialRegionTree();

// ��Ϣ��Ӧ����
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);

// ��������
public:
	inline MAPTREE &GetMapTree()					{return m_mapTree;}
	inline const DWORD_PTR GetRegionNo()const		{return m_uNo;}
	inline const HTREEITEM GetItem()const			{return m_hItem;}
	inline LISTREGION &GetRegionList()				{return m_listRegionList;}
	inline void SetErrorType(unsigned uErrorType)	{m_uErrorType = uErrorType;}

	const UINT GetLayer(HTREEITEM hItem);
	const UINT GetOffset(HTREEITEM hItem);
	BOOL ReadData(HTREEITEM hItem);															// ��ȡ����
	BOOL AddItemMonster(HTREEITEM hItem, CRegion *pRegion, CMonster *pMonster = NULL);		// ����MONSTER���
	BOOL AddItemMonInfo(HTREEITEM hItem, CRegion *pRegion, CMonsterInfo *pMonInfo = NULL);	// ����MONSTERINFO���
	

	BOOL AddItemCollection(HTREEITEM hItem, CRegion *pRegion, CCollection *pCollection= NULL);
	BOOL AddItemCollInfo(HTREEITEM hItem, CRegion *pRegion, CCollectionInfo *pCollInfo = NULL);	// ����CollInfo���


	BOOL AddItemNPC(HTREEITEM hItem, CRegion *pRegion, CNPC *pNPC = NULL);					// ����NPC���
	BOOL DelItemMonster(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);				// ɾ��MONSTER���
	BOOL DelItemMonInfo(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);				// ɾ��MONSTERINFO���
	
	BOOL DelItemRoadsPoint(HTREEITEM hItem,int m,int n, CRegion *pRegion);
	BOOL DelItemRoads(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);

	BOOL DelItemCollInfo(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);				//ɾ�� Collectioninfo�ڵ�
	BOOL DelItemCollection(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);					// ɾ��Collection���
	BOOL AddNode(HTREEITEM hItem, unsigned uLayer, unsigned uOffset, CRegion *pRegion);		// ���ӽ��
	BOOL DelNode(HTREEITEM hItem, unsigned uLayer, unsigned uOffset, CRegion *pRegion);		// ɾ�����
	BOOL AddItemMonster(HTREEITEM hItem, CRegion *pRegion, CRect *rect,CMonster *pMonster=NULL);		//������ӽڵ�
	BOOL AddItemCollection(HTREEITEM hItem, CRegion *pRegion, CRect *rect,CCollection *pCollection=NULL);		//�������collention�ڵ�
	BOOL AddItemSignPos(HTREEITEM hItem, CRegion *pRegion, CRect *rect,CSignPos *pSignPos=NULL);		//�������·���ڵ�
	BOOL AddItemSignPos(HTREEITEM hItem, CRegion *pRegion,CSignPos *pSignPos=NULL);		//�������·���ڵ�
	BOOL DelItemSignPos(HTREEITEM hItem, unsigned uOffset, CRegion *pRegion);			//ɾ��·���
	BOOL AddItemRoadsPoint(HTREEITEM hItem, CRegion *pRegion, int n,CRect *rect,CRoads *pRoads=NULL);		//�������·����
	BOOL AddItemRoads(HTREEITEM hItem, CRegion *pRegion,CRoads *pRoads = NULL);	//�������·��
	void DlgADD(CRect *rect);

//////////////////////////////////////////////////////////////////////
	void			SetItemFont(HTREEITEM hItem, LOGFONT&);
	void			SetItemBold(HTREEITEM hItem, BOOL);
	void			SetItemColor(HTREEITEM hItem, COLORREF);
	BOOL			GetItemFont(HTREEITEM hItem, LOGFONT *);
	BOOL			GetItemBold(HTREEITEM hItem);
	COLORREF		GetItemColor(HTREEITEM hItem);
	int				m_ncurrRoads;		//��ǰѡ�е�·��

// ��Ա����
protected:
	struct Color_Font {
		COLORREF color;
		LOGFONT  logfont;
	};

	CMap <void*, void*, Color_Font, Color_Font&> m_mapColorFont;

	BOOL		m_bExist;			// Exist or not
	UINT		m_uLayer;			// ��ǰѡ��Ľ�����ڲ��
	UINT		m_uOffset;			// ��ǰѡ��Ľ��ƫ��
	UINT		m_uRCLayer;			// �Ҽ�ѡ�������ڲ��
	UINT		m_uRCOffset;		// �Ҽ�ѡ����ƫ��
	HTREEITEM	m_hItem;			// ��ǰѡ��Ľ��
	HTREEITEM	m_hRCItem;			// �Ҽ�ѡ��Ľ��
	DWORD_PTR	m_uNo;				// ��ǰѡ��ĳ������
	DWORD_PTR	m_uRCNo;			// �Ҽ�ѡ��ĳ������
	MAPTREE		m_mapTree;			// ���ν���б�ӳ��
	CRegion		*m_pRCRegion;		// �Ҽ�ѡ��ĳ���
	unsigned	m_uErrorType;		// �����Ϣ����
	LISTREGION	m_listRegionList;	// �ɱ༭�����б�
	VECMONSTERNAME	m_vecMonsterName;	
	VECCOLLECTIONNAME	m_vecCollectionName;
public:
	afx_msg void OnPopMenuAdd();
public:
	afx_msg void OnPopMenuDel();
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnPaint();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
