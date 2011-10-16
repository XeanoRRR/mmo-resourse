
/*
*	file:		PlayerDepot.h
*	Brief:		�ֿ���
*	detail:		��װ�˲ֿ���ص���֤����Ʒ��������
*				
*
*				
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-01-22
*/

#pragma once

#include "Container/CAmountLimitGoodsContainer.h"
#include "Container/CAmountLimitGoodsShadowContainer.h"
#include "Container/CVolumeLimitGoodsContainer.h"
#include "Container/CSubpackContainer.h"
#include "Container/CWallet.h"
#include "Goods/CGoods.h"

#include "../../../Public/Common/DepotDef.h"

//! ------------------------------------������------------------------------------
class PlayerDepot
{
public:
	PlayerDepot(CPlayer *pPlayer);
	~PlayerDepot(void);

private:
	PlayerDepot(const PlayerDepot&){};
	PlayerDepot&operator=(const PlayerDepot& r){};


	//! ���򽻻�
	//!---------------------------------------------------------------------------------------------------------
public:
	//!							����
	void						CodeToDataBlock_ForClient			(DBWriteSet& setWriteDB);
	//!							����ֿ���Ϣͷ
	void						CodeHeadToDataBlock_ForClient		(DBWriteSet& setWriteDB);
	//!							����
	void						CodeToDataBlock_ForServer			(DBWriteSet& setWriteDB);
	//!							�ֿ����ñ���
	void						CodeSetupToDataBlock				(DBWriteSet& setWriteDB);
	//!							����
	void						DecodeFromDataBlock_FromServer		(DBReadSet& setReadDB);

	//!							�󶨽�ɫ
	void						SetOwner							(CPlayer *pPlayer);
	//!							���װ�ص���Ʒ
	void						Clear								(void);
	//!							�ͷŶ���
	void						Release								(void);


	//! �߼�����
	//!---------------------------------------------------------------------------------------------------------
public:
	//!							��Ӧ����뿪��Ϸ
	void						OnPlayerLeave						(void);
	//!							�ҵ���������
	CGoodsContainer*			FindContainer						(long eContainerId, bool bNotTest = false);
	//!							�жϲֿ������Ƿ�����
	BOOL						IsFull								(long eContainerId);
	//!							�����ֿ����Session
	bool						CreateSession						(ULONG uRadius, long lPosX, long lPosY);
	//!							���ԻỰ����ʧ���������ֿ�
	bool						TestSession							(void);

private:
	//!							����
	void						Lock								(void);
	//!							����
	bool						Unlock								(const char *pPassword);
	//!							�޸Ĳֿ�����
	bool						ChangePassword						(const char *pOldPwd, const char *pNewPwd);
	//!							����һ��������λ�Ŀ�����رգ����ؿ�����Ҫ�Ľ�Ǯ
	long						SetSecondary						(bool bOpen);

	//!							�����Ự
	void						EndSession							(void);
	//!							����ֿ�
	void						frost								(void);


	//! ��Ϣ��Ӧ
	//!---------------------------------------------------------------------------------------------------------
public:
	//!							��Ӧ��������
	void						OnInputPwdRequest					(CMessage *pMsg);
	//!							��Ӧ�ֿ���������
	void						OnLockRequest						(CMessage *pMsg);
	//!							��Ӧ�޸���������
	void						OnChangePwdRequest					(CMessage *pMsg);
	//!							��Ӧ�����ӱ�������
	void						OnBuySubpackRequest					(CMessage *pMsg);


	//! 
	//!---------------------------------------------------------------------------------------------------------
private:
	//!							��������
	bool						IsLock								(void);
	//!							���Զ���
	bool						IsFrost								(void);


	//! ��̬��Ա
	//!---------------------------------------------------------------------------------------------------------
public:
	//!							�õ�����
	static	tagDepotSetup*		GetDepotSetup						(void);

private:

	static	tagDepotSetup		c_DepotSetup;


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
	CWallet/*GemContainer*/				        m_GemDepot;


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