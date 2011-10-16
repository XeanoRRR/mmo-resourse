/**
* @filename:	PlayerTrade.h
* @date:		2010/7/23
* @author:		wing (totti19851101@gmail.com) 

* @purpose: ��ҽ���
*/

#pragma once

#include "../../../../Public/Module/GUID.h"
#include "../Goods/Goods.h"
#include "../Player.h"

class PlayerTrade
{
public:
	enum tagPriceType
	{
		PT_Money,			// ����
		PT_YuanBao,			// Ԫ��
	};

	enum tagTradeObject
	{
		Trade_Other,					///< ����Ϊ�Է����
		Trade_Me,						///< ����Ϊ�Լ�
	};

	struct tagTradeGoods 
	{
		long lNum;						///< ��Ʒ������
		long lPos;						///< ��Ʒ�ڽ���ҳ���ϵ�λ��
		ulong dwIconId;					///< ��Ʒ��ͼ��ID
		CGUID guid;						///< ��Ʒ��GUID
	};

public:
	PlayerTrade(void);

	~PlayerTrade(void) {}

	/// ���ý��ײ���
	void SetTradePara(const CGUID& sessionID, const CGUID& myID,long myPlugID,
		const CGUID& otherID,long otherPlugID);

	/// ȡ�öԷ�
	CPlayer* GetOther(void);

	/// ȡ�ý��׸�����Ŀ
	long GetTradeGridNum(void) { return GRIDNUM; }

	/// ȡ�ý���Session ID
	const CGUID& GetSessionID(void) { return m_sessionID; }

	/// ȡ�ü�������Plug ID
	long GetMyPlugID(void) { return m_myPlugID; }

	/// ���ü������׽��
	void SetMyGold(ulong golds) { m_myGolds = golds; }
	/// ȡ�ü������׽��
	ulong GetMyGold(void) { return m_myGolds; }
	/// ���öԷ����׽��
	void SetOtherGold(ulong golds) { m_otherGolds = golds; }
	/// ȡ�öԷ����׽��
	ulong GetOtherGold(void) { return m_otherGolds; }

	/// ȡ�ü���������Ʒ�б�
	map<int,tagTradeGoods>& GetMyTradeGoodsList(void) { return m_myTradeGoodsList; }

	/// ȡ�öԷ�������Ʒ�б�
	map<int,CGoods>& GetOtherTradeGoodsList(void){ return m_otherTradeGoodesList; }

	/// ��Ӽ���������Ʒ
	void AddMyTradeGoods(CGoods* pGoods,int iPos);

	/// ��ӶԷ�������Ʒ
	void AddOtherTradeGoods(CGoods* pGoods,int iPos);

	bool OnInputGoods(CPlayer::tagGoods* pstGoods);							// �����׽����ʱ����������Ʒ���һ�����Ӧ����

	/// ȡ�ü���ָ��������λ��Ʒ����
	tagTradeGoods* GetMyGoodsData(int nIndex);
	/// ȡ�öԷ�ָ��������λ��Ʒ����
	CGoods* GetOtherGoodsData(int nIndex);

	void UpdataTradeInfo(WORD wPriceType,long lPrice,int iWhose);

	/// �رս���
	void CloseTrade(void);

	/// ��������״̬
	void SetLockedState(bool bState,int iWhose);

	/// ȡ��ĳ��������״̬
	bool GetLockedState(int iWhose);

	/// ���ý��׽��״̬�������Ƿ�Բ��������
	void SetResult(bool b)	{m_result = b;}

	/// ��һ�ν����Ƿ�Բ������
	bool GetResult() {return m_result;}

	/// �ҷ��Ƿ��������״̬��
	bool IsReady()		{return m_myIsLocked;}

	/// �ӽ��׽������Ƴ���Ʒ
	void RemoveTradeGoods(int nPos,int iWhose);

public:
	void SendClosePageMes(void);							///< ���͹ر���Ϣ
	void SendTradingGoodsMes(void);							///< ���ͽ�����Ϣ
	void SendChangeTradeGoodsMes(void);						///< ���͸��Ľ�����Ϣ
	void SendReadyTradeGoodsMes(void);						///< ����ȷ����Ϣ
	void SendRemoveTradeGoodsMes(int iPos);					///< �����Ƴ�������Ʒ��Ϣ
	void SendAddTradeGoodsMes(int nPos);					///< ���ͷ���Ʒ��ҳ���ϵ���Ϣ
	void SendCoinMes(void);									///< ���ͽ��׽�������Ϣ

private:
	static const long GRIDNUM;								///< ����ҳ��Ľ�����λ��

	CGUID m_sessionID;										///< ���׻ỰID
	CGUID m_myID;											///< �������ID
	CGUID m_otherID;										///< �Է����ID
	long m_myPlugID;										///< ����PlugID
	long m_otherPlugID;										///< �Է�PlugID
	ulong m_myGolds;										///< �������
	ulong m_otherGolds;										///< �Է����

	bool m_myIsLocked;										///< ��������״̬��־
	bool m_otherIsLocked;									///< �Է�����״̬��־

	bool m_result;											///< ���������Ƿ�Բ����������������;�жϵ�ʱ���ֵΪfalse��

	map<int,tagTradeGoods> m_myTradeGoodsList;				///< ����������Ʒ�б�
	map<int,CGoods> m_otherTradeGoodesList;					///< �Է�������Ʒ�б�
};