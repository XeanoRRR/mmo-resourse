#ifndef DB_REGION_H
#define DB_REGION_H
#include "BaseDBEntity.h"
#include "..\..\..\public\Date.h"
#include "../public/DBDefine.h"

#include <list>

using namespace std;



class CDBRegion : public CBaseDBEntity
{
public:
	//˰
	struct tagRegionParam
	{
		long  lID;							//������ID
		long  lMaxTaxRate;					//���˰��
		long  lCurrentTaxRate;				//��ǰ˰��
		long  lYuanbaoTaxRate;				//Ԫ��˰��
		long  lBusinessTaxRate;				//����˰��
		DWORD dwTotalTax;					//˰���ܺ�
		DWORD dwBusinessTotalTax;			//����˰���ܺ�
		DWORD dwTodayTotalTax;				//����˰���ۼ�ֵ
		long  lSupRegionID;					//����˰��ĳ���ID(0��ʾ���Ͻ�˰��)
		long  lTurnInTaxRate;				//�Ͻɵ�����������˰��
		CGUID  OwnedFactionID;				//ӵ����������İ��
		CGUID  OwnedUnionID;				//ӵ�����������ͬ��ID 
	};

	CDBRegion();
	CDBRegion(const CGUID& id);
	~CDBRegion();

	long GetID(void) { return m_ID; }
	void SetID(long id) { m_ID = id; }

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos);
	
	void SaveForThread(long msgid, BYTE* msgBuf, long& msgBufPos, SaveObj* saveData);

	void SetLoadFromDBFlag(bool bVal){m_bLoadedFromDB=bVal;}
	bool IsLoadFromDB(){return m_bLoadedFromDB;}

	tagRegionParam& GetRegionParam(void) { return m_RegionParam; }

	void SetRegionParam(tagRegionParam& value) { memcpy(&m_RegionParam, &value, sizeof(value)); }

	virtual CDBRegion& operator=(CDBRegion& right)
	{
		if(this!=&right)
		{
			m_ID=right.m_ID;
			m_GUID=right.m_GUID;
			m_bLoadedFromDB=right.m_bLoadedFromDB;
			SetRegionParam(right.GetRegionParam());
		}
		return *this;
	}
private:
	//////////////////////////////////////////////////////////////////////////
	bool m_bLoadedFromDB;
	tagRegionParam m_RegionParam;
	long m_ID;
};

#endif//DB_REGION_H