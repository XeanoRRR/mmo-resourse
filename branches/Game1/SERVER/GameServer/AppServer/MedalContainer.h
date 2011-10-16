
/*
*	file:		MedalContainer.h
*	Brief:		ѫ��������
*	detail:		ѫ��������������������ݣ�һ������ѫ����ص�ȫ�����ݺͲ�������һ������ΪCPlayer�ṩ�Ĺ��ܽӿ�
*				��˵����������Ʒʵ���ɾ�̬���ֽ���ά��������ʵ����������Ʒ���й���ֻ������Ʒ����(ѫ��ID
*	Authtor:	�ſ���
*	Datae:		2008-08-26
*/


#include "../public/GoodsExDef.h"
#pragma once

//! ��������
////////////////////////////////////////////////////////////////////////
//! ѫ�����ʹ����
const LONG MEDAL_MAX_USE_NUM = 3;

//! ѫ��������������ֵ
enum eMCOptReValue
{
	//! �����ɹ�
	eMCRV_Succeed = 1,
	//! û�и�ѫ��
	eMCRV_MedalNotBeing,
	//! �ظ����
	eMCRV_RepeatAdd,
	//! Ŀ�겻����
	eMCRV_AimCantFind,
};


//! ѫ������
////////////////////////////////////////////////////////////////////////
class MedalContainer
{
public:
	MedalContainer(CPlayer *pOwner);
	~MedalContainer(void);


public:
	//!						������(ѫ��ID)���ѫ��
	eMCOptReValue			AddMedal(LONG lGoodsBaseIndex);
	//!						������(ѫ��ID)ɾ��ѫ��
	eMCOptReValue			DelMedal(LONG lGoodsBaseIndex);

	//!						�ж��Ƿ���ָ������(ѫ��ID)�ĵ�ѫ��
	BOOL					HasMedal(LONG lGoodsBaseIndex);
	//!						�õ���ʹ�õ�ѫ������
	LONG					GetMedalNum(void);

	//!						ʹ��һ��ѫ�£�ͬʱֻ��ʹ�����MEDAL_MAX_USE_NUM��ѫ�£�
	CGoods*					UseMedal(LONG lGoodsBaseIndex);
	//!						�黹һ��ʹ�õ�ѫ��
	eMCOptReValue			ReturnMedal(LONG lGoodsBaseIndex);
	//!						��һ������ʹ�õ�ѫ�½�����һ��δ��ʹ�õ�ѫ��
	CGoods*					SwapMedal(LONG lUsingIndex, LONG lNewIndex);


public:
	//!						����
	void					AddToByteArray(vector<BYTE> &vData);
	//!						����
	void					Decode(BYTE *pData, LONG &lPos);

	//!						����
	void					CodeToDataBlock(DBWriteSet& setWriteDB);
	//!						����
	void					DecodeFromDataBlock(DBReadSet& setReadDB);


private:
	//!						ӵ����
	CPlayer					*m_pOwner;
	//!						ӵ�е�ѫ��(��������)
	set<LONG>				m_setHold;
	//!						����ʹ�õ�ѫ��(��������)
	set<LONG>				m_setUsing;


#pragma region ��̬ȫ�ֳ�Ա

public:
	//!										��ʼ��ѫ��ϵͳ
	static			BOOL					InitializeMedalSystem(void);
	//!										�ͷ���Դ
	static			void					Release(void);

	//!										�ҵ�һ��ѫ��
	static			CGoods*					FindMedia(LONG lGoodsBaseIndex);
private:
	//!										������Ϊkey��ѫ�¶����б�
	static			map<LONG, CGoods*>		s_mapMedalByIndex;

#pragma endregion


};