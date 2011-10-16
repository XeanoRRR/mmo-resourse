



#ifndef _Packet_Mgr_H_
#define _Packet_Mgr_H_
#pragma once
#include "../Utility/gtDef.h"


class CPacketMgr
{
public:
	CPacketMgr(class CGoodsMgr* pFather);

	~CPacketMgr(void);

	void Release();

	void DecodeFromDataBlock(DBReadSet& dbRead,bool bExData);

	void DecodeBusinessFromDataBlock(DBReadSet& dbRead,bool bExData);

	list<tagGoods>& GetMainGoodsList()			{ return m_listGoods; }

	CGoods* GetGoodsByGUID(const CGUID& guID,int *pPos,long* lExtendID);

	tagCote* GetCoteGoods(long i){						//�õ�������ϵ���Ʒ
		if( i>=0 && i <COTE_NUM)
			return m_Cote[i];
		return NULL;
	}

	int FindCote(){
		for(int i=0; i!=COTE_NUM; i++)
		{
			if( m_Cote[i]&&m_Cote[i]->pGoods)
				return i;
		}
		return -1;
	}
	int FindEmptyCote()
	{
		for(int i=0; i!=COTE_NUM; i++)
		{
			if( m_Cote[i] && m_Cote[i]->pGoods == NULL )
				return i;
		}
		return -1;
	}

	long GetCoteGoodsAmount()	{
		long l=0;
		for(int i=0; i!=COTE_NUM; i++)
		{
			if( GetCoteGoods(i)->pGoods )
				l++;
		}
		return l;
	}

	void ClearAllGoods(long lSocketID);
	list<tagGoods>* GetMainPacket()		{ return &m_listGoods;}
	list<tagGoods>* GetChildPackGoods(long lPackIndex)  {return &m_Cote[lPackIndex]->m_listCoteGoods;}

	CGoods* FindGoodsByOrgName(const char* name);
	//�����ӿ�
	list<tagGoods>* GetContainerGoodsList( int iContainerID);
	DWORD	GetContainerEmptyPos(int iContainerID);
	//�����ӿ�
	list<tagGoods>* GetGoodsList(WORD index=0);															//��ȡָ����������Ʒ��������ԭʼ������
	list<tagGoods>* GetSubGoodslist(WORD index);														//��ȡָ����ŵ��ӱ�������Ʒ����	

	tagGoods* GetGoods(WORD wPos);																		//��ȡԭʼ������ָ��λ�õ���Ʒ
	tagGoods* GetSubGoods(WORD ExtendID,WORD lPos);														//��ȡָ����ŵ��ӱ�����ĳ��λ�õ���Ʒ

	CGoods* GetSubPGoods(WORD index);																	//��ȡָ����ŵ��ӱ���������Ϊ��Ʒ������

	void SetSubPGoods(WORD index,CGoods* pGoods);														//����ָ����ŵ��ӱ�����pGoods
	//	void ClearCoteGoods(WORD index);																	//���ָ����ŵ��ӱ���

	WORD GetPackageXRowNum(WORD index) const		  {return (WORD)PACK_WIDE;}							//��ȡָ����ŵ��ӱ�����������
	WORD GetPackageDimen(WORD index);																	//��ȡָ����ŵ��ӱ������ݻ�
	WORD GetHostPackgeDimen() const 				  {return (WORD)HOST_GRIDITEM;}						//��ȡԭʼ�������ݻ�

	bool IsAllPackageFull();																			//�Ƿ����еı�����������
	WORD GetNotFullPackage();																			//��ȡָ����ŵı����ĵ�һ���ո��ӵ�λ��
	tagCote* GetCotePackage(WORD index);																//��ȡ����ָ����ŵ��ӱ������ݽṹ

	////////////////////////#�����ӵ���Ʒ����ĺ���/////////////////////////////////////
	tagGoods* GetGoodsByOrgName(const char* strOrgName);
	//
	long	GetPackGoodsPos(WORD wContainerID, const CGoods* pGoods);			//�õ�������Ʒ�ı���λ��

	bool	AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID);
	CGoods	*RemoveGoods(const CGUID& SourceGUID, const CGUID& DesGUID,DWORD dwPos,LONG lExtendID,DWORD dwAmount);

	WORD	GetGoodsContainerID(CGoods* pGoods);						//��ȡ�������ϵ���Ʒ���ڵ�����ID
	WORD	FindContainerIDOfGoods(CGoods* pGoods);						//�������������Ƿ���ĳ�����͵���Ʒ������з������ڵ�����ID
	//CGoods* GetGoodsByPosition(long lExtendID,WORD dwPos);				//ͨ��λ�ò�����Ʒ
	CGoods* FindFirstGoodsByIndex(DWORD dwIndex,int& lExtendID,int& pPos);						//���ҵ�һ�����ID���ڴ����ID����Ʒ

	//������Ʒ���������������������Ƿ��������Ʒ�����������Ʒ���ڵ�λ�ã������İﶨλ��
	bool	FindAppointGoods(long lGoodType,long lGoodId, long lGoodNum, long lContainerID);									
	//�������Ų��������������еĴ�����Ʒ������
	long	GetGoodsAmount(long nIdx);
	//������Ʒ���Ʋ��������������еĴ�����Ʒ������
	long	GetGoodsAmount(const char* cGoodsName);
	// ���ݴ������ƷID��Ҫ���ӵ���������һ���ѵ������ᳬ���ѵ����޲���δ����������Ʒ
	CGoods* FindOverLapGoodsByIndex(DWORD dwIndex,DWORD dwGoodsNum,int& lExtendID,int& pPos);

	///��ȡ���ʱ���ָ��λ�õ���Ʒ
	tagCote*	GetBusinessCote(void)		{ return m_CoteBussinesPack; }
	tagGoods* GetBusinessGoods(WORD wPos);
	void SetBussinesPack(CGoods*pPackGoods);
	bool ClearBussinesPackGoods();

	void AddGoodsPos(DWORD dwPos)	{ m_setPacketEmptyPos.insert(dwPos); }
	void DelGoodsPos(DWORD dwPos)	
	{
		set<DWORD>::iterator itPos = m_setPacketEmptyPos.find( dwPos );
		if( itPos != m_setPacketEmptyPos.end() )
			m_setPacketEmptyPos.erase(itPos);
	}

private:
	CGoodsMgr*		m_pFather;
	 //	������������ʱ����е���Ʒ
	tagCote*		m_CoteBussinesPack;
	//	��������ԭʼ������Ʒ������
	list<tagGoods>		m_listGoods;		
	//	���������
	tagCote*			m_Cote[COTE_NUM];	

	set<DWORD>			m_setPacketEmptyPos;
};
#endif//_Packet_Mgr_H_