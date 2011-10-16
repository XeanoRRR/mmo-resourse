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

	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos);


	tagRegionParam& GetRegionParam(void) { return m_RegionParam; }

	void SetRegionParam(tagRegionParam& value) { memcpy_s(&m_RegionParam, sizeof(tagRegionParam), &value, sizeof(tagRegionParam)); }

	// Other interfaces
	// ׼�����͵�����
	//vector<DB_CARD_ATTR_TYPE>& GetSendAttrEnumVector(void) { return m_SendAttrEnumVector; }
	// ��׼�����͵�������Vector�е���Ӧλ�õı�־
	void OpenAttrFlagInVector(DB_REGION_ATTR_TYPE attrType);
	// �ر�׼�����͵�������Vector�е���Ӧλ�õı�־
	void CloseAttrFlagInVector(DB_REGION_ATTR_TYPE attrType);
	// ��װ��Ϣ
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA);

private:
	// �ȴ������͸�DBS����������
	vector<long> m_SendAttrEnumVector;
	tagRegionParam m_RegionParam;
	long m_ID;
};

#endif//DB_REGION_H