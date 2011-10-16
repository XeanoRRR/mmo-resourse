/**************************************************
*		�ļ���  ��Mail.h
*		��Ҫ�������ʼ������߼�

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�10/10/18
*		�޶�˵����
****************************************************/
#ifndef _MAIL_H_
#define _MAIL_H_

#pragma once

#include <list>
using namespace std;
#include "../Goods/Goods.h"
#include "../../../../Public/Common/BaseDef.h"
#include "../Player.h"



namespace mail{
	// �ͻ��˶�����ʼ���Ϣ�ṹ
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

	class CMail
	{
	public:
		CMail(void);
		~CMail(void);

	public:
		//static CMail *Instance();
		void	AddGoods( CPlayer::tagGoods* pstGoods );
		void	DecordDataFromByteArray( stMailData &stData,BYTE* pByte, long& pos );			// �ʼ����ݽ���	
		void 	AddMail(stMailData &tagData);													// ���һ���ʼ���Ϣ	
		void	FormatMailInfo(stMailData &tagData);											// ȷ���ʼ���Ϣ����ʾ����������ɫ�ͱ���ĸ���������Ϣ��		
		void	ReleaseMailGoods(stMailData &stData);											// ɾ���ʼ�����Ʒ	
		void	DelMailByGuid(CGUID gMailID);													// ɾ��ָ��ID���ʼ�
		void	AddMailToDel(const unsigned int id);
		void	UpdataMail(stMailData& tagData);												// ����ָ�����ʼ���Ϣ(������ʼ����ڱ��鿴����Ҫ�����Ķ��������ʾ
		bool	CheckSendCondition( eMAILTYPE eMailType );										// ����ʼ����������Ƿ��ɣ���ɷ��ţ����򲻷���
		void	ClearData();																	// �����ѷ��͵��ʼ�����
		void    ClearAllMailData();
		void	DelMail();
		void	SendMail();
		
		
		//do...��PersonalMail.cpp����
		void	DoSendMail();
		void	DoRecvMail();
		void	DoReadMail( const unsigned int index );
		void	DoDelMail();
		void	DoAgreePay(const unsigned int index);	
		void	DoGetGold();
		void	DoGetGoods(uint nMailIndex,uint nGoodsIndex);	
		void	DoRemoveGoods(const unsigned int index);														// ��ȡ�ʼ���Ʒʱ�Ƴ���Ʒ
		void	DoRejectMail(const unsigned int index);											//����
		
		//set
		void	SetRecver( string& strName );
		void	SetTitle( string& strTitle ) 			{ m_strTitle = strTitle; }
		void	SetContent( string& strContent ) 		{ m_strContent = strContent; }
		void	SetGoldNum( const long lGoldNum ) 		{ m_lGoldNum = lGoldNum; }						// ���ͽ����߸���ȡ��
		void	SetGoodsIndex( int nIndex )				{ m_nGoodsIndex = nIndex; }						// indexͨ�����������ع�������Ϣ���
		void	SetMailType( bool bMailType )			{ m_bIsPayMail = bMailType; }
		void	SetCurReadMailGUID(CGUID& guid)			{ m_guidCurReadMail = guid; }
		void	SetMailBoxGUID(CGUID& guid)				{m_guidCurMailBox = guid;}							
			
		//get
		const int					GetGoodsIndex()	const		{ return m_nGoodsIndex; }	
		const CGUID&				GetCurReadMailGUID() const	{ return m_guidCurReadMail; }			// ���õ�ǰ����������guid		
		const CGUID&				GetMailBoxGUID() const 		{ return m_guidCurMailBox; }
		vector<stMailData>&			GetMailData() 				{ return m_vecMailData; }	
		bool						GetCurReadMailData ( stMailData& mailData);					// �õ���ǰ���ڶ����ʼ�����	
		const int					GetPostage(const eMAILTYPE eMailType);
		eMAILTYPE					GetEMailType() const;
		stMailData*					GetMailByGUID(const CGUID& gMailID);	
		list<CPlayer::tagGoods>&	GetGoodsList()				{ return m_listGoods; }	
		vector<stMailData>&			GetDelMail() 				{ return m_vecDelMail; }

	private:	
		//static CMail*			m_pInstance;
		list<CPlayer::tagGoods>	m_listGoods;			// �����б�	
		vector<stMailData>		m_vecMailData;			// �ʼ��б�	
		vector<stMailData>		m_vecDelMail;			// ��ɾ�����ʼ��б�	
		CGUID					m_guidCurReadMail;		// ��ǰ�����Ķ����ʼ�ID	
		CGUID					m_guidCurMailBox;		// ��ǰ���ڲ���������GUID
		vector<string>			m_vecRecver;
		string					m_strTitle;				// �ʼ�����
		string					m_strContent;			// �ʼ�����
		unsigned long			m_lGoldNum;				// ���ͽ����߸���ȡ��
		int						m_nGoodsIndex;			// ��Ʒ����(�ڼ�����Ʒ)
		bool					m_bIsPayMail;
	};
	}

#endif
