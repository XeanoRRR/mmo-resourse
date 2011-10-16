
#include "StdAfx.h"
#include "public.h"
#include "AutoSupplyData.h"


AutoSupplyData::AutoSupplyData(void)
{
	Reset();
}
AutoSupplyData::~AutoSupplyData(void)
{

}

#define DATA_SIZE sizeof(tagAotoSupplyNode) * AotoSupplyTypeNum

//! ����
void AutoSupplyData::AddToByteArray(vector<BYTE> &ByteArray)
{
	_AddToByteArray(&ByteArray, m_AotoSupplySetup, DATA_SIZE);
}

//! ����
void AutoSupplyData::GetFormByteArray(BYTE *pByteArray, long &lPos)
{
	_GetBufferFromByteArray(pByteArray, lPos, m_AotoSupplySetup, DATA_SIZE);
}

//!							����
void AutoSupplyData::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_AotoSupplySetup, DATA_SIZE);
}
	//!							����
void AutoSupplyData::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	setReadDB.GetBufferFromByteArray(m_AotoSupplySetup, DATA_SIZE);
}

//! ���ö���
void AutoSupplyData::Reset(void)
{
	memset(m_AotoSupplySetup, 0, DATA_SIZE);
}

//! ���һ�����ã�uType��
const tagAotoSupplyNode* AutoSupplyData::GetOneSetup(LONG eType)
{
	if (AotoSupplyTypeNum > eType)
	{
		return &(m_AotoSupplySetup[eType]);
	}
	return NULL;
}

//! �޸�һ������
LONG AutoSupplyData::SetOneSetup(LONG eType, const tagAotoSupplyNode &AotoSupplyNode)
{
	if (AotoSupplyTypeNum <= eType)
		return eASET_TypeWithout;

	//! ��ͬ�Ķ���ֵ����������
	if(0 == memcmp(&(m_AotoSupplySetup[eType]), &AotoSupplyNode, sizeof(tagAotoSupplyNode)))
		return eASET_NotChange;

	//! �ж�ֵ���߼��Ϸ���
	if(	0 >= AotoSupplyNode.lGoodsIndex && 
		0 >= AotoSupplyNode.lSpringScale && 
		A_Hundred_Percent <= AotoSupplyNode.lSpringScale
	)
		return eASET_ValueErr;
	
	memmove(&(m_AotoSupplySetup[eType]), &AotoSupplyNode, sizeof(tagAotoSupplyNode));
	return eASET_NotErr;
}