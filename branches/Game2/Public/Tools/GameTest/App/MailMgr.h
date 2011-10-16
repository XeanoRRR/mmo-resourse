



#ifndef _MailMgr_H_
#define _MailMgr_H_
#pragma once
#include "../Utility/PlayerDef.h"
#include "../Utility/gtDef.h"

class CMessage;

class CMailMgr
{
public:
	CMailMgr(void);
	~CMailMgr(void);

	void	Release();
	// ! ����ռ��䱣������	
	void    ClearMailData();																	 		   

	//	@ �Ƿ�õ�����
	bool	IsGetData(void)		{ return m_bIsGetData; }
	void	SetGetData(bool b)	{ m_bIsGetData = b; }

	//	@	ɾ���ʼ���Ʒ
	void	ReleaseMailGoods(stMailData& stMail);

	//	@	�ʼ�����
	void DecodeFromByteArray(stMailData& stMail,BYTE* pBytes,long& lpos);

	//	@ �����ʼ�����
	void RefreshData();
public:
	//**********************************�ռ�����ط���***************************************//
	// By MartySa
	// ���һ���ʼ���Ϣ
	void 	AddMail(stMailData &tagData);

	// ɾ��ָ��ID���ʼ�
	void	DelMailByGuid(const CGUID &gMailID);

	// ɾ��ѡ�е������ʼ�(��մ�ɾ�����ʼ��б�)
	//void	DelAllSelectedMail();

	// ȷ���ʼ���Ϣ����ʾ����������ɫ�ͱ���ĸ���������Ϣ��
	void	FormatMailInfo(stMailData &tagData);

	// ȷ���ʼ���ʱ��(���������Լ���ɫ)
	void	FormatMailTime(stMailData &tagData, string &str, long &color);

	// ���ָ��ID���ʼ�
	stMailData*	GetMailByGUID(const CGUID &gMailID);

	//**********************************��������ط���***************************************//
	// By MartySa
	// ɾ��ָ�������ĸ�����Ʒ
	void	DelGoodsEx(const int nIndex);
	//********************************** For Lua �ӿ�*******************************************
	
	void	AddSendToPlayerName(const string &name)   { m_vecSendToPlayerName.push_back(name); m_lSendMoney+=10; } //�������
	
	void   AddSendToNorGoods(const tagGoods *taggoods);												//��Ӹ�����Ʒ

	bool	SendPlayerName(CMessage *&msg);																//��������
	
	bool   SendNorGoods(CMessage *&msg, BYTE mailtype);												//������Ʒ

	void	ClearAllSendData();																			//������ݣ��´���

	long	GetSendMoney();																				//���㷢�͵�Ǯ��
	
	long	GetSendToPlayerSize()              { return (long)m_vecSendToPlayerName.size(); } //��ǰ��ϵ������

	long   GetSendGoodsSize()			       { return (long)m_listSendToGoods.size(); }	 //��ǰ������Ʒ����

	CGUID   &GetMailDataGuid(long index)       { return m_vecMailData[index].gMailID; }		 //��ȡ�ռ����ʼ��б�
	long    GetMailDataSize()				   { return (long)m_vecMailData.size(); }		 //��ȡ�ռ����ʼ���Ŀ

	//2009.1.6 �¼ӣ������ʼ����ݱ�����ʹ�� MartySa
	vector<stMailData>	&GetMailData()			   { return m_vecMailData; }



private:

	// �Ƿ��Ѿ��������������
	bool			   m_bIsGetData;

	// �ʼ���ʾ��Ϣ���ͣ�Ŀǰֻ�з��ͺ�ɾ���������ʾ��
	BYTE			   m_bTipType;

	// �ʼ��б�
	vector<stMailData> m_vecMailData;

	// ��ɾ�����ʼ��б�
	vector<stMailData> m_vecDelMailData;

	// �ռ��������б�
	vector<string>		m_vecSendToPlayerName;

	// �������͵ĸ�����Ʒ�б�
	list<stGoodsInfo>	m_listSendToGoods;

	// �ʼ�����
	eMAILTYPE			m_eMailType;

	// ��Ʒ����
	eMailGoodsType		m_eGoodsType;

	//	Ǯ
	long				m_lSendMoney;
};
#endif//_MailMgr_H_