
/*
*	file:		PlayerDepot.h
*	Brief:		�ֿ���
*	detail:		����ֿ�����		
*				
*	Authtor:	�ſ���
*	Datae:		2008-01-22
*/
#pragma once

#include "Container/AmountLimitGoodsContainer.h"
#include "Container/SubpackContainer.h"
#include "Container/VolumeLimitGoodsContainer.h"
#include "Container/Wallet.h"
#include "Goods/Goods.h"
#include "../../../Public/Common/DepotDef.h"

//!				�����б�pair<����ָ��, ��������>
typedef			list<pair<CGoodsContainer*, long>>	GoodsContainerList;

//! ------------------------------------������------------------------------------
class PlayerDepot
{
public:
	PlayerDepot(void);
	~PlayerDepot(void);

private:
	PlayerDepot(const PlayerDepot&){};
	PlayerDepot&operator=(const PlayerDepot& r){};


public:

	//!									����
	void								AddToByteArray_ForServer			(vector<BYTE> *pByteArray);
	//!									����
	void								GetFormByteArray_FromServer			(BYTE *pByteArray, long &lPos);
	//!									�������ݽ���
	void								GetSetupFormByteArray				(BYTE *pByteArray, long &lPos);


	//!									����
	void								CodeToDataBlock_ForServer			(DBWriteSet& setWriteDB);
	//!									����
	void								DecodeFormDataBlock_FromServer		(DBReadSet& setReadDB);
	//!									�������ݽ���
	void								DecodeSetupFromDataBlock			(DBReadSet& setReadDB);

	//!									���ö���
	void								Clear								(void);
	//!									�ͷŶ���
	void								Release								(void);
	//!									�󶨽�ɫ
	void								BindPlayer							(CPlayer *pPlayer);

private:
	//!									��ʼ������
	void								Init								(void);


public:
	//!									�õ������б�
	void								GetGoodsContainerList				(GoodsContainerList &ContainerList);
	//!									�ҵ���������
	CGoodsContainer*					FindContainer						(long eContainerId);

	//!									�õ�����
	const char*							GetPassword							(void);
	//!									��������
	void								SetPassword							(const char* pPassword);

	//!									�õ��ӱ��������ʶ
	long								GetSubpackFlag						(void);
	//!									�����ӱ����������
	void								SetSubpackFlag						(long lFlag);

	//!									�õ��ⶳʱ��
	long								GetThawdate						(void);
	//!									�õ��ⶳʱ��
	void								SetThawdate						(long lThawdate);


public:

	//!									��ȡ����
	static	bool						LoadSetup							(string filename);
	//!									�õ�����
	static	const	tagDepotSetup*		GetDepotSetup						(void);
private:

	static	tagDepotSetup	c_DepotSetup;


	//! 
	//!---------------------------------------------------------------------------------------------------------
private:
	//!											���ֿ�
	CVolumeLimitGoodsContainer					m_Primary;
	//!											���ֿ�2
	//CVolumeLimitGoodsContainer					m_Primary2;
	//!											���Ӳֿ���λ��5���ӱ�����λ��
	CSubpackContainer							m_Secondary;
	//!											���õĵ��ӱ�����λ
	bool										m_arrHlodSubpack[DEPOT_SUBPACK_NUM];

	//!											��Ҳֿ�
	CWallet										m_GoldDepot;
	//!											���Ҳֿ⣨�ɲ��õı�ʯ�ֿ����ģ��������Ʋ��䣩
	CWallet                 			        m_GemDepot;


private:
	//!											�󶨵����
	CPlayer										*m_pPlayer;

	//!											�ֿ�����
	char										m_szPassword[DEPOT_PASSWORD_ARRAY_LEN + 1];

	//!											��ȡ���
	bool										m_bIsFree;


	//!											��������������
	long										m_lInputPwdErrCount;
	//!											��һ�δ����ʱ�̣�һ��ʱ���ڣ�
	long										m_lFirstErrTime;
	//!											�ⶳʱ�̱���
	long										m_lThawDate;

	//!											�Ϸ��Ի��뾶
	ULONG										m_uSessionRadius;
	//!											�Ի�����X
	long										m_lSessionPosX;
	//!											�Ի�����Y
	long										m_lSessionPosY;

};