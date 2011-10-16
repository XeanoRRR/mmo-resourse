/*
*	file:		AutoSupplyData.h
*	Brief:		�Զ��������ݶ���
*	detail:		�ṩ����ı����ӿ�
*	Authtor:	�ſ���
*	Datae:		2008-05-21
*/

#pragma once

#include "SpriteDef.h"

class AutoSupplyData
{
public:
	AutoSupplyData(void);
	~AutoSupplyData(void);


public:
	//!								����
	void							AddToByteArray		(vector<uchar> &ByteArray);
	//!								����
	void							GetFormByteArray	(uchar *pByteArray, long &lPos);
	
	//!								����
	void							CodeToDataBlock		(DBWriteSet& setWriteDB);
	//!								����
	void							DecodeFromDataBlock	(DBReadSet& setReadDB);

	//!								���ö���
	void							Reset				(void);

	//!								���һ�����ã�uType��
	const	tagAotoSupplyNode*		GetOneSetup			(long eType);
	//!								�޸�һ������
	long							SetOneSetup			(long eType, const tagAotoSupplyNode &AotoSupplyNode);

private:
	//!								�Զ���ҩ����������
	tagAotoSupplyNode				m_AotoSupplySetup[AotoSupplyTypeNum];

};