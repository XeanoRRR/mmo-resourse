
//�����Player��صĽṹ��ö��
#pragma once

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

class CGoods;
/*
* ����: �ʼ�ϵͳ 's Def
* ժҪ: ��ҿ���ͨ�������գ��ʼ�����ñ˴�֮����ϵ�����ཻ����Ʒ�ȵ��ߡ�
*		����Ϣ���洢�ڷ�������һ��ʱ�䣬ֱ������Ķ�������ɾ����Ϣ�����ż�����ʱ�䵽�
* ����: ����
* ��������: 08.9.17
* �޸���־: 2008.12.1 tanhaowen ��ӵ�test��
*/
// �������ʾ���ʼ���������

enum eMailTipType
{
	DELMAIL_TIP = 0,			// ɾ���ʼ�����ʾ����
	SENDMAIL_TIP,				// �����ʼ�����ʾ����
	UNKNOWN_TIP				// ��֪����
};

enum eMailGoodsType
{
	GOODS_NORMAL_TYPE = 0,			// ��ͨ��Ʒ
	GOODS_TSHOP_ASKFOR_TYPE,		// ��ֵ�̵���Ҫ��Ʒ
	GOODS_TSHOP_LARG_TYPE,			// ��ֵ�̵�������Ʒ
	GOODS_UNKNOW_TYPE				// ������Ʒ
};

struct stGoodsInfo 
{
	CGoods *pGoods;					//  ��Ʒ
	DWORD	dwPos;					//	�ڱ������е�����
	DWORD	dwType;					//	��Ʒ���ڵ���������
	DWORD	dwYuanBao;				//	Ԫ���۸�
	stGoodsInfo() : pGoods(NULL), dwPos(0), dwType(0), dwYuanBao(0)
	{}
};

// �ͻ��˶�����ʼ���Ϣ�ṹ
// struct	stMailData
// {
// 	CGUID			gMailID; 			// �ʼ���ID
// 	DWORD			dwTimeStart;		// �����ʼ��Ŀ�ʼʱ��(��Ϊ��λ)
// 	DWORD			dwTime;				// �ʼ�����Чʱ��(��Ϊ��λ)
// 	DWORD			dwGoldNum;			// �����Ľ�Ǯ��
// 	bool			bIsReaded;			// �Ƿ��Ķ���
// 	bool			bIsInDelList;		// �Ƿ��ѱ���ӵ��˱�ɾ���б���ȥ
// 	bool			bIsCanReBack;		// �Ƿ��������
// 	string			strMailName;		// �ʼ�����
// 	//char			strMailName[128];
// 	string			strMailDesc;		// �ʼ�����
// 	//char			strMailDesc[2048];
// 	string			strMailDescEx;		// �ʼ���������
// 	//char			strMailDescEx[2048];
// 	string			strSenderName;		// �ʼ�����������
// 	//char			strSenderName[128];
// 	long			colorTitle;			// �ż�����������ɫ
// 	long			colorSender;		// �ż�������������ɫ
// 	long			colorGoodsEx;		// ������Ʒ���ֵ���ɫ
// 	list<CGoods*>	GoodsList;			// ������Ʒ�б�
// 	eMAILTYPE		eMailType;			// �ʼ������ͣ�5�֣���ͨ����ͨ��Ʒ���������ţ���Ҫ�����ͣ�
// 	stMailData()
// 	{
// 		gMailID = CGUID::GUID_INVALID;
// 		dwTimeStart   = 0;
// 		dwTime		  = 0;
// 		dwGoldNum	  = 0;
// 		bIsReaded	  = false;
// 		bIsInDelList  = false;
// 		bIsCanReBack  = true;
// 		ZeroMemory(strMailName,strlen(strMailName));
// 		ZeroMemory(strMailDesc,strlen(strMailDesc));
// 		ZeroMemory(strMailDescEx,strlen(strMailDescEx));
// 		ZeroMemory(strSenderName,strlen(strSenderName));
// 		colorTitle	  = 0xffffffff;
// 		colorSender	  = 0xffffffff;
// 		colorGoodsEx  = 0xffffffff;
// 		eMailType	  = COMMON_MAIL;
// 	}
// };
struct	stMailData
{
	CGUID			gMailID; 			// �ʼ���ID
	DWORD			dwTimeStart;		// �����ʼ��Ŀ�ʼʱ��(��Ϊ��λ)
	DWORD			dwTime;				// �ʼ�����Чʱ��(��Ϊ��λ)
	DWORD			dwGoldNum;			// �����Ľ�Ǯ��
	bool			bIsReaded;			// �Ƿ��Ķ���
	bool			bIsInDelList;		// �Ƿ��ѱ���ӵ��˱�ɾ���б���ȥ
	bool			bIsCanReBack;		// �Ƿ��������
	string			strMailName;		// �ʼ�����
	string			strMailDesc;		// �ʼ�����
	string			strMailDescEx;		// �ʼ���������
	string			strSenderName;		// �ʼ�����������
	long			colorTitle;			// �ż�����������ɫ
	long			colorSender;		// �ż�������������ɫ
	long			colorGoodsEx;		// ������Ʒ���ֵ���ɫ
	list<CGoods*>	GoodsList;			// ������Ʒ�б�
	eMAILTYPE		eMailType;			// �ʼ������ͣ�3�֣���ͨ����ͨ��Ʒ���������ţ�
	stMailData()
	{
		gMailID = CGUID::GUID_INVALID;
		dwTimeStart = 0;
		dwTime	= 0;
		dwGoldNum = 0;
		bIsReaded = false;
		bIsInDelList = false;
		bIsCanReBack = true;
		strMailName = "";
		strMailDesc = "";
		strMailDescEx = "";
		strSenderName = "";
		colorTitle = 0xffffffff;
		colorSender = 0xffffffff;
		colorGoodsEx = 0xffffffff;
		eMailType = COMMON_MAIL;
	}
}; 
#define MAIL_PREPAGE 8			// �ʼ��б�һҳ�ĸ���
// ״̬�б�
