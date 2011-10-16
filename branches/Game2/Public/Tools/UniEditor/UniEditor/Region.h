#pragma once

#include "Monster.h"
#include "NPC.h"
#include "Collection.h"
#include "SignPos.h"
#include "Roads.h"
#include <fstream>
#include <string>
#include <map>
using namespace std;

typedef vector<CMonster *>			VECMONSTER;
typedef VECMONSTER::iterator		VECMONSTERITR;
typedef vector<CMonsterInfo *>		VECMONSTERINFO;
typedef VECMONSTERINFO::iterator	VECMONSTERINFOITR;
typedef vector<CNPC *>				VECNPC;
typedef VECNPC::iterator			VECNPCITR;
typedef vector<CCollection *>		VECCOLLECTION;
typedef VECCOLLECTION::iterator		VECCOLLECTIONITR;
typedef vector<CCollectionInfo *>	VECCOLLINFO;
typedef	VECCOLLINFO::iterator		VECCOLLINFOITR;
typedef vector<CSignPos *>			VECSIGNPOS;
typedef VECSIGNPOS::iterator		VECSIGNPOSITR;
typedef map<int,CRoads *>			MAPROADS;
typedef MAPROADS::iterator			MAPROADSITR;
typedef MAPROADS::reverse_iterator	MAPROADSREVERSEITR;
typedef vector<CRoads::tagRoadsPoint>	VECROADPOINT;
typedef VECROADPOINT::iterator		VECROADPOINTITR;

// �������ӽṹ
typedef struct tagCell
{
	friend ifstream &operator>>(ifstream &rstream, tagCell *prCell);

	unsigned short	bBlock		:3;		// �赲��0��������  1���ɷ���  2�����ɷ��У�
	unsigned short	bSecurity	:3;		// ��ȫ��
	unsigned short	lWar		:2;		// ��ս��
	unsigned short	lMaterial	:8;		// ����
	WORD			lSwitch;			// �л���(��Ŵ�1��ʼ)
}Cell;

// �л���
typedef struct tagSwitch
{
	friend ifstream &operator>>(ifstream &rstream, tagSwitch *prSwtich);

	long			lState;				// �л�״̬
	long			lRegionID;			// ���ڵ�ID��
	long			lCoordX;			// ���ڵ�����
	long			lCoordY;
	long			lDir;				// ���ڵ㷽��
}Switch;

class CRegion
{
public:
	CRegion(void);
public:
	~CRegion(void);
	BOOL Release();

public:
	// ����ͼ�����ļ� "*.rgn"
	friend ifstream &operator>>(ifstream &rstream, CRegion *pregion);

// ���ݶ�д
public:
	const inline unsigned GetRegionNo()const				{return m_uRegionNo;}
	inline void SetRegionNo(const unsigned uNo)				{m_uRegionNo = uNo;}
	const inline CString GetRegionName()const				{return m_szRegionName;}
	inline void SetRegionName(const CString szName)			{m_szRegionName = szName;}
	const inline string &GetRgn()const						{return m_strCls_Rgn;}
	const inline long Width()const							{return m_lWidth;}
	const inline long Height()const							{return m_lHeight;}
	const inline long CellCount()const						{return m_lWidth * m_lHeight;}
	inline Cell* GetCell()const								{return m_pCell;}
	const inline long SwitchCount()const					{return m_lSwitchCount;}
	inline CPoint* GetSwitch()const							{return m_ptSwitch;}
	const inline long SwitchPoint()const					{return m_lSwitchPoint;}
	VECMONSTER &GetMonster();
	VECNPC &GetNPC();
	VECCOLLECTION &GetCollection();
	VECSIGNPOS &GetSignPos();
	MAPROADS &GetRoads();
	void WriteToVecDisPlay(long lType,long lValue);	
	bool CheckMonsterNo(const unsigned uNo, unsigned &uInd);			// У��MonsterNo��Ϣ
	bool CheckCollectionNo(const unsigned uNo, unsigned &uInd);			// У��CollectionNo��Ϣ
protected:
	unsigned		m_uRegionNo;		// �������
	CString			m_szRegionName;		// ��������
	string			m_strCls_Rgn;		// 7 Characters
	long			m_lVersion;			// Version
	long			m_lType;			// Type
	long			m_lWidth;			// Region width
	long			m_lHeight;			// Region Height
	Cell*			m_pCell;			// Region cell pointer
	long			m_lSwitchCount;		// Switch count
	CPoint*			m_ptSwitch;			// Switch coordinate
	long			m_lSwitchPoint;		// Switch point
	Switch*			m_pSwitch;			// Switch pointer

	VECMONSTER		m_vecMonster;		// ��ǰ���������б�
	VECNPC			m_vecNPC;			// ��ǰ����NPC�б�
	VECCOLLECTION	m_vecCollection;	// ��ǰ�����ɼ����б�
	VECSIGNPOS		m_vecSignPos;		//��ǰ����·���
	MAPROADS		m_mapRoads;			//��ǰ����·����ʶ

	vector<long>	m_vecMDisplay;		//�����б����������Ŀ�Ĺ���������Ϣ
	vector<long>	m_vecCDisplay;		//�����б����������Ŀ�Ĳɼ���������Ϣ

};

// ����ͼ�����ļ� "*.rgn"
ifstream &operator>>(ifstream &rstream, tagCell *prCell);
ifstream &operator>>(ifstream &rstream, tagSwitch *prSwitch);
ifstream &operator>>(ifstream &rstream, CRegion *pregion); 
