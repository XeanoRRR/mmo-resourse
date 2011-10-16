#pragma once
#include "..\myadobase.h"
#include "../public/GUID.h"
#include <map>
//#include "..\..\server\dbserver\appworld\DBCityWar.h"
class CBaseDBEntity;
class CDBDupRgn;
class CDBRgnGoods;
class CDBRgnGoodsGroup;

class CRsDupRgn :
	public CMyAdoBase
{
public:
	CRsDupRgn(void);
	~CRsDupRgn(void);

public:
	bool	LoadAll(_ConnectionPtr& cn);							//װ�����еĵж԰���ϵ
	bool	SaveAll(void);//�������еĵж԰���ϵ

	// GS��ѯָ����ҵĸ��˷��ݸ���GUID
	void	QuestPHGuidByPlayerID(const char* playerName, CGUID& retPHGUID);

	//! ��ȡ���˷�����Ϣ
	void    LoadDupRgn(CDBDupRgn* cw, _ConnectionPtr& cn);
	//! ������˷�����Ϣ
	void    SaveDupRgn(CDBDupRgn* cw, _ConnectionPtr& cn);
	//! ɾ�����˷�����Ϣ
	void    DeleteDupRgn(CDBDupRgn* cw);

	//! ��ȡ���˷����ڵ���Ʒ��Ϣ
	bool    LoadDupRgnObj(const CGUID& rgnGUID, CDBRgnGoodsGroup* goodsGroup, _ConnectionPtr& cn);
	//! ������˷����ڵ���Ʒ��Ϣ
	static bool    SaveDupRgnObj(const CGUID& rgnGUID, map<CGUID,CDBRgnGoods*>& goodsGroup, _ConnectionPtr& cn);

	//! ɾ�����˷����ڵ���Ʒ��Ϣ
	void    DeleteDupRgnObj(const CGUID& rgnGUID);

	//��Buffer
	static bool SaveBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &pRs);
	//��Buffer
	bool LoadBufferField(CBaseDBEntity *pGoods, const char* szFiledName, _RecordsetPtr &pRs);
};
