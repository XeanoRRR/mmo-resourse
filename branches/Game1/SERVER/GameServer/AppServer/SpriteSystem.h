
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
#include "GSAutoFight.h"

class CPlayer;
class CServerRegion;

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

    //!                         ����һ��ս�������ܷ�ʹ��
    static      BOOL            TestAutoFightSkill       (LONG lSkillID);

    //!                         ����һ��ս�����������ܷ�ʹ��
    static      BOOL            TestAutoAssistSkill       (LONG lSkillID);

    //!                         ����һ�����������ܷ�ʹ��
    static      BOOL            TestAutoAssistGoods       (LONG lGoodsID);

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

    //!                         �����ȶ��һ�ʱ��
    void                        SetStableTime(DWORD dwTime)
    {
        if (dwTime <= MAX_STABLETIME)
        {
            m_AutoFight.SetStableTime(dwTime);
        }
        else
        {
            m_AutoFight.SetStableTime(MAX_STABLETIME);
        }
    }
    //!                         �ж��Զ����������
    bool                        CanAutoRepair(void);
    //!                         �õ��ȶ��һ�ʱ��
    long                        GetStableTime(void)         {return m_AutoFight.GetStableTime();}

	//!							��Ӧ�Զ����������޸�		
	void						OnChangeAutoSupply			(CMessage *pMsg);							
    //!                         ��Ӧ�Զ��������
    void                        OnChangeAutoFight           (CMessage *pMsg);
    //!                         ��Ӧ���ܺ͸������ܵ��ߵļ�������
    //void                        OnCheckCanUse(CMessage *pMsg);


private:
	//!							�ܹ�ʹ�õ���Ʒ������
	static	map<LONG, DWORD>	c_mapCanUseGoodsIndex;
    //!                             �ܹ�ʹ�õ�ս������
    static  vector<LONG>        c_vectorCanUseFightSkill;
    //!                             �ܹ�ʹ�õĸ�������
    static  vector<LONG>        c_vectorCanUseAssistSkill;
    //!                             �ܹ�ʹ�õĸ���������Ʒ
    static  vector<LONG>        c_vectorCanUseAssistGoods;
	
	//!							�Զ��������ݶ���
	GSAutoSupply				m_AutoSupply;
    //!                         �Զ�������ݶ���
    GSAutoFight                 m_AutoFight;



};