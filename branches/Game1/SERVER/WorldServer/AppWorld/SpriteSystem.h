
/*
*	file:		SpriteSystem.h
*	Brief:		С���鸨��ϵͳ
*	detail:		��WS�ϣ�����ֻ�������ݱ��֡��������ܣ�
*				��С����ϵͳ���������ݽ���ͳһ����
*					
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/
#include "../public/AutoSupplyData.h"
#include "../public/AutoFightData.h"

#pragma once

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


    //! �����ȶ��һ�ʱ��
    void SetStableTime(DWORD dwTime);
    //�õ��ȶ��һ�ʱ��
    DWORD GetStableTime(void);
private:
    //!                             ����������ͬ����
    static      bool        FindExist(vector<LONG> &refVector, LONG lValue);
private:
	//!								�ܹ�ʹ�õ���Ʒ������ + ���÷�Χö�٣�
	static		map<LONG, DWORD>	c_mapCanUseGoodsIndex;
    //!                             �ܹ�ʹ�õ�ս������
    static      vector<LONG>        c_vectorCanUseFightSkill;
    //!                             �ܹ�ʹ�õĸ�������
    static      vector<LONG>        c_vectorCanUseAssistSkill;
    //!                             �ܹ�ʹ�õĸ���������Ʒ
    static      vector<LONG>        c_vectorCanUseAssistGoods;

	//!								�Զ��������ݶ���
	AutoSupplyData					m_AutoSupplyData;
    //!                             �Զ�ս�����ݶ���
    CAutoFightData                  m_AutoFightData;
	


};