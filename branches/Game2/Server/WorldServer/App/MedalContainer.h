
/*
*	file:		MedalContainer.h
*	Brief:		ѫ��������
*	detail:		
*	Authtor:	�ſ���
*	Datae:		2008-08-26
*/


#pragma once

class CGoods;

//! ѫ������
////////////////////////////////////////////////////////////////////////
class MedalContainer
{
public:
	MedalContainer(void);
	~MedalContainer(void);

public:
	//!						����
	void					AddToByteArray(vector<BYTE> &vData);
	//!						����
	void					Decode(BYTE *pData, LONG &lPos);

	//!						����
	void					CodeToDataBlock(DBWriteSet& setWriteDB);
	//!						����
	void					DecodeFromDataBlock(DBReadSet& setReadDB);
public:
	//!						������(ѫ��ID)���ѫ��
	BOOL					AddMedal(LONG lGoodsBaseIndex);
	//!						�õ�ӵ�е�ѫ��
	void					GetHoldMedal(set<CGoods*> &setGoods);

private:
	//!						ӵ�е�ѫ��(��������)
	set<LONG>				m_setHold;



};