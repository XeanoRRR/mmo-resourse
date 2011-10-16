#pragma once

// ��������
struct tagBounsDes
{
	ulong	dwBounsID;		// ������ԴID
	string	strTitle;		// ����
	string	strDes;			// ����
	uchar	bType;			// �ӿ�����(1-���ߡ�2-��ֵ��3-�ű�)
	string	strName;		//	bType 1-�������ơ�2-��������3-��
	ulong	dwValue;		//	bType 1-����������2-����ֵ��3-�ű�ID

	tagBounsDes()
	{
		dwBounsID	= 0;
		strTitle	= "";
		strDes		= "";
		bType		= 0;
		strName		= "";
		dwValue		= 0;
	}

	void EncodeToDataBlock(DBWriteSet& setWriteDB);
	void DecodeFromDataBlock(DBReadSet& setReadDB);
	
};

// ������Ϣ
struct tagBounsInfo
{
	CGUID	guid;				// �������Ա��
	string	strAccount;			// �콱����˺�
	uchar	bStatus;			// ��ȡ״̬(1-δ��ȡ��2-���ᡢ3-����ȡ��4-����) 
	ulong	dwBounsID;			// ������ԴID
	time_t	tValidTime;			// ��Ч��

	tagBounsInfo()
	{
		strAccount	= "";
		bStatus		= 0;
		dwBounsID	= 0;
	}

	void EncodeToDataBlock(DBWriteSet& setWriteDB);
	void DecodeFromDataBlock(DBReadSet& setReadDB);
};

