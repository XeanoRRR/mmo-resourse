
/*
*	file:		SpriteSystem.h
*	Brief:		С���鸨��ϵͳ
*	detail:		��WS�ϣ�����ֻ�������ݱ��֡��������ܣ�
*				��С����ϵͳ���������ݽ���ͳһ����
*					
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/
#pragma once

#include "../../../Public/ServerPublic/Server/Common/AutoSupplyData.h"

class CPlayer;

//! С���鸨��ϵͳ��
////////////////////////////////////////////////////////////////////////////////
class SpriteSystem
{
public:
	SpriteSystem(void);
	~SpriteSystem(void);
	

public:
	//!								��ȡ����
	static		BOOL				LoadSetup(void);
	//!								���ñ���
	static		void				SetupAddToByteArray(vector<BYTE> &ByteArray);
	

public:
	//!								����
	void							AddToByteArray		(vector<BYTE> &ByteArray);
	//!								����
	void							GetFormByteArray	(BYTE *pByteArray, long &lPos);

	//!	����
	void CodeToDataBlock(DBWriteSet& setWriteDB);
	//!	����
	void DecodeFromDataBlock(DBReadSet& setReadDB);


private:
	//!								�ܹ�ʹ�õ���Ʒ������ + ���÷�Χö�٣�
	static		map<LONG, DWORD>	c_mapCanUseGoodsIndex;

	//!								�Զ��������ݶ���
	AutoSupplyData					m_AutoSupplyData;
	


};