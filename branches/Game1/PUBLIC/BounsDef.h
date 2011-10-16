#pragma once

#include "guid.h"
#include "DataBlockSetWriteRead.h"

// ��������
struct tagBounsDes
{
	DWORD	dwBounsID;		// ������ԴID
	string	strTitle;		// ����
	string	strDes;			// ����
	//BYTE	bType;			// �ӿ�����(1-���ߡ�2-��ֵ��3-�ű�)
	//string	strName;		//	bType 1-�������ơ�2-��������3-��
	//DWORD	dwValue;		//	bType 1-����������2-����ֵ��3-�ű�ID

	tagBounsDes()
	{
		dwBounsID	= 0;
		strTitle	= "";
		strDes		= "";
	}

	void EncodeToDataBlock(DBWriteSet& setWriteDB);
	void DecodeFromDataBlock(DBReadSet& setReadDB);
	
};

// ������Ϣ
struct tagBounsInfo
{
	CGUID	guid;				// �������Ա��
	string	strAccount;			// �콱����˺�
	BYTE	bStatus;			// ��ȡ״̬(1-δ��ȡ��2-���ᡢ3-����ȡ��4-����) 
	DWORD	dwBounsID;			// ������ԴID
    BYTE	bType;			    // �ӿ�����(1-���ߡ�2-��ֵ��3-�ű�)
    string	strName;		    //	bType 1-GoodsIndex��2-��������3-��
    DWORD	dwValue;		    //	bType 1-����������2-����ֵ��3-�ű�ID
	time_t	tValidTime;			// ��Ч��

	tagBounsInfo()
	{
		strAccount	= "";
		bStatus		= 0;
		dwBounsID	= 0;
        bType		= 0;
        strName		= "";
        dwValue		= 0;
	}

	void EncodeToDataBlock(DBWriteSet& setWriteDB);
	void DecodeFromDataBlock(DBReadSet& setReadDB);
};

// �ϴ���Ʒ��Ϣ
struct tagUploadGoods
{
    CGUID   playerGUID;                 // ��ɫGUID
    string  strAccount;                 // ��ɫAccount(ʢ������ID)
    string  strGoodsName;               // ��Ʒ����
    DWORD   dwGoodsIndex;               // ��ƷIndex
    DWORD   dwGoodsCounts;              // ��Ʒ����
    DWORD   dwWSNumber;                 // WorldServer���
    DWORD   dwGSNumber;                 // GameServer���    

    tagUploadGoods()
    {
        memset(this, 0, sizeof(tagUploadGoods));
    }
    void EncodeToDataBlock(DBWriteSet& setWriteDB)
    {
        setWriteDB.AddToByteArray(playerGUID);
        setWriteDB.AddToByteArray((char*)strAccount.c_str());
        setWriteDB.AddToByteArray((char*)strGoodsName.c_str());
        setWriteDB.AddToByteArray(dwGoodsIndex);
        setWriteDB.AddToByteArray(dwGoodsCounts);
        setWriteDB.AddToByteArray(dwWSNumber);
        setWriteDB.AddToByteArray(dwGSNumber);
    }
    void DecodeFromDataBlock(DBReadSet& setReadDB)
    {
        char strTmp[256];
        setReadDB.GetBufferFromByteArray(playerGUID);
        strAccount = setReadDB.GetStringFromByteArray(strTmp, 256);
        strGoodsName = setReadDB.GetStringFromByteArray(strTmp, 256);
        dwGoodsIndex = setReadDB.GetDwordFromByteArray();
        dwGoodsCounts = setReadDB.GetDwordFromByteArray();
        dwWSNumber = setReadDB.GetDwordFromByteArray();
        dwGSNumber = setReadDB.GetDwordFromByteArray();
    }
};

// ������Ϣ
struct tagNotice
{
    DWORD   dwID;
    time_t  tBegin;             // ���濪ʼʱ��
    time_t  tEnd;               // �������ʱ��
    string  strText;            // ����
    WORD    wType;              // �������ͣ�0ϵͳ��1���
    WORD    wPlay;              // ���ŷ�ʽ��0����һ�Σ�1ѭ������
 //   WORD    wGameID;            // 0ȫ��ȫ����101�ǳ���˵��102����
    WORD    wFlag;              // ����״̬��0δ���ţ�1���ڲ��ţ�2�Ѳ���

    tagNotice()
    {
        memset(this, 0, sizeof(tagNotice));
    }
};

typedef map<DWORD, tagNotice> MapNotice;
