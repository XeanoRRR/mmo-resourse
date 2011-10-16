#pragma once
#include "shape.h"
#include "..\server\setup\GoodsList.h"

class CGoods :
	public CShape
{
public:
	CGoods(void);
	virtual ~CGoods(void);
	DWORD GetIndex(){return m_dwIndex;}
	void  SetIndex(DWORD dwVal){m_dwIndex=dwVal;}

private:
	DWORD	m_dwIndex;		// ���
	long	lNum;			//����
	vector<CGoodsList::tagEffect>	m_vectorEffect;		// ��չ�����б�
	vector<CGoodsList::tagFixValues>		m_vecFixValues; //���������б�


public:
	CGoodsList::tagGoods* GetProperty()	{return CGoodsList::GetProperty(m_dwIndex);}
	vector<CGoodsList::tagEffect>*		GetEffect()	{return &m_vectorEffect;}
	vector<CGoodsList::tagFixValues>* GetFixs(){return &m_vecFixValues;}

	//�õ����ΪwType����չ����ֵ,nWhichָ��ֵ1����ֵ2
	DWORD GetEffectVal(WORD wType,int nWhich);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	bool AddToByteArray_ForClient(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	void SetNum(long lVal){lNum=lVal;}
	long GetNum(){return lNum;}
};
