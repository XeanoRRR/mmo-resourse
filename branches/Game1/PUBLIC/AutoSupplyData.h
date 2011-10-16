
/*
*	file:		AutoSupplyData.h
*	Brief:		�Զ��������ݶ���
*	detail:		�ṩ����ı����ӿ�
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/

#include "SpriteDef.h"

#pragma once

class AutoSupplyData
{
public:
	AutoSupplyData(void);
	~AutoSupplyData(void);


public:
	//!								����
	void							AddToByteArray		(vector<BYTE> &ByteArray);
	//!								����
	void							GetFormByteArray	(BYTE *pByteArray, long &lPos);
	
	//!								����
	void							CodeToDataBlock		(DBWriteSet& setWriteDB);
	//!								����
	void							DecodeFromDataBlock	(DBReadSet& setReadDB);


	//!								���ö���
	void							Reset				(void);


	//!								���һ�����ã�uType��
	const	tagAotoSupplyNode*		GetOneSetup			(LONG eType);
	//!								�޸�һ������
	LONG							SetOneSetup			(LONG eType, const tagAotoSupplyNode &AotoSupplyNode);



private:
	//!								�Զ���ҩ����������
	tagAotoSupplyNode				m_AotoSupplySetup[AotoSupplyTypeNum];

};