////////////////////////////////////////////////////////////////////////////
//CollectionGoods.h///////////////////////////////////////////////////////////////////
//�ɼ��࣬�������������ͷ���
//Author:Ф��
//Create Time:2008.2.3
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MoveShape.h"
#include "../../../Public/Common/DeputyOccu.h"
class CCollectionGoods :
	public CMoveShape
{
public:
	CCollectionGoods();
	~CCollectionGoods(){}

private:
	long	m_lCreatorType;		// ���������ߵ�����
	CGUID	m_CreatorID;		// ���������ߵ�GUID
	//DWORD m_dwCollectionGoodsID;		//�ɼ����ID
	//bool  m_bCanBeCollect;			//�ɼ����Ƿ�ɱ��ɼ��ı�־
	//string m_strCollectionGoodsShowName;//�ɼ������ʾ����
	//string m_strCollectionGoodsOriName;	//�ɼ����ԭʼ����
	//eDeputyOccu		m_eOccuID;			//��ְҵid
	//DWORD			m_dwPicID;			//ͼ��id
	//DWORD			m_dwRadarID;		//С��ͼ��־(�״�id)
	//DWORD			m_dwMixTimes;		//��С�ɼ�����
	//DWORD			m_dwMaxTimes;		//���ɼ�����
	//DWORD			m_dwCollectLevel;	//�ɼ��ȼ�
	//DWORD			m_dwRoleID;			//��Ҫ����ȼ�
	//DWORD			m_dwRoleStep;		//��Ҫ������

public:
	// ���òɼ������ʾģ�͵�����
	//void SetModelLevel();

	//��ʼ����ʾģ��
	void SetDisplayModelGroup(bool bAsyn = true);

	//���òɼ����״̬
	void SetAction(WORD l);

	//�ɼ������ʾ
	bool Display(void);

	//�ɼ����AI
	void AI();
	//long AI_Stand();			// վAI
	//long AI_BeAttack();			// ������AI
	//long AI_Died();				// ����AI

	//�л��ɼ����״̬
	void OnActionChange();

	//�ɼ������
	bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);

	//��ȡ��Ʒ����ʾ����
	string GetColletctionGoodsShowName();

	//��ȡ��Ʒ��ԭʼ��
	string GetCollectionGoodsOriName();

	//��ȡ��ְҵ��ID
	DWORD  GetOccuID();

	//��ȡ�ɼ�����ʾģ�͵�ID
	DWORD  GetPicID();

	//��ȡ�״��ͼ��ID
	DWORD  GetRadarID();

	//��ȡ��С�ɱ��ɼ��Ĵ���
	DWORD  GetMixTimes();

	//��ȡ���ɱ��ɼ��Ĵ���
	DWORD  GetMaxTimes();

	//��ȡ�ɼ���Ҫ��Ĳɼ��ȼ�
	DWORD  GetCollectLevel();

	//��ȡ����ID
	int  GetRoleID();

	//��ȡ����׶�
	DWORD  GetRoleStep();

	///��ȡ�ɼ��˲ɼ���ʱʹ�õĶ���ID
	DWORD  GetActionID();

	//��ȡ����ID�б�
	const vector<tagQuestGoods>* GetRoleInfo();

	//typedef vector<tagQuestGoods>::iterator QUESTGOODS_ITER;

	//��ȡ�ɼ���ı��ɼ��������б�
	const list<tgConditionGoods>* GetConditionList();

	// ���Monster�����ߵ����ͺ�ID
	long GetCreatorType()		{return m_lCreatorType;}
	CGUID GetCreatorID()		{return m_CreatorID;}
	// ����Monster�����ߵ����ͺ�ID
	void SetCreatorType(long type)		{m_lCreatorType = type;}
	void SetCreatorID(CGUID guid)		{m_CreatorID = guid;}
};