
/*
*	file:		SpriteSystem.h
*	Brief:		С���鸨��ϵͳ
*	detail:		ÿ�������һ����������ң����ܰ�����
*					�Զ�����
*					
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/


#pragma once

#include "GSAutoSupply.h"

class CPlayer;

//! С���鸨��ϵͳ��
////////////////////////////////////////////////////////////////////////////////
class SpriteSystem
{
public:
	SpriteSystem(void);
	~SpriteSystem(void);

public:
	//!							���ñ���
	static		void			SetupAddToByteArray			(vector<BYTE> &ByteArray);
	//!							���ý���
	static		void			SetupGetFormByteArray		(BYTE *pByteArray, long &lPos);

	//!							���ñ���
	static		void			CodeSetupToDataBlock		(DBWriteSet& setWriteDB);
	//!							���ý���
	static		void			DecodeSetupFromDataBlock	(DBReadSet& setReadDB);

	//!							����һ����Ʒ�ܷ������Զ�����
	static		BOOL			TestAutoSupplyGoods			(LONG lGoodsIdx);

public:
	//!							����������
	void						SetOwner					(CPlayer *pMainPlayer);
	//!							����
	void						AddToByteArray				(vector<BYTE> &ByteArray);
	//!							����
	void						GetFormByteArray			(BYTE *pByteArray, long &lPos);

	//!							����
	void						CodeToDataBlock				(DBWriteSet& setWriteDB);
	//!							����
	void						DecodeFromDataBlock			(DBReadSet& setReadDB);

	//!							��Ӧ�Զ����������޸�		
	void						OnChangeAutoSupply			(CMessage *pMsg);							


private:
	//!							�ܹ�ʹ�õ���Ʒ������
	static	map<LONG, DWORD>	c_mapCanUseGoodsIndex;
	
	//!							�Զ��������ݶ���
	GSAutoSupply				m_AutoSupply;



};