#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include "../message packaging/CS2CContainerObjectMove.h"

#define CONTAINER_NUM 6

class CGoods;
class CPlayer;
class CVolumeLimitGoodsContainer;

//����������CELL��Ϣ
struct tagCellInfo 
{
	DWORD dwGoodsId;
	DWORD dwGoodsMaxNum;
	DWORD dwGoodsNum;
	CGUID goodsGuid;
};

//����������Ϣ
struct tagContainerInfo
{
	DWORD dwContainerId;
	DWORD dwContainerVolume;
	tagCellInfo* pCellInfo;
};

//���Խ��
struct tagTestResult
{
	DWORD dwContainerId;
	DWORD dwPos;
	CS2CContainerObjectMove::OPERATION_TYPE eOperType;
};

//����ԭʼ������������ɾ�����ԵĽṹ��
struct tagTestArg_ID_Num
{
	DWORD _dwGoodsID;
	DWORD _dwNum;
};

//ɾ�����Խ��,�����ڼ�¼ɾ��ָ������λ����Ʒ������
struct tagDelTestResult
{
	DWORD dwContainerId;		
	DWORD dwPos;
	DWORD dwNum;//ɾ������
	CS2CContainerObjectMove::OPERATION_TYPE eOperType;
};


class CTestContainer
{
private:
	tagContainerInfo m_ContainerInfo[CONTAINER_NUM];
	int m_ContainerNum;
public:		
	CTestContainer();
	~CTestContainer();	
public:
	VOID Init(CPlayer* pPlayer);
	/// �ֶ���Ӳ��Ա���
	VOID AddContainer( DWORD dwContainerID, CVolumeLimitGoodsContainer* pVolumeContainer );
	VOID FillCell(int nIndex,CVolumeLimitGoodsContainer* pVolumeContainer);
	BOOL FillOneGoods(CGoods* pGoods,tagTestResult* pResult);
	BOOL ClearOneGoods(CGoods* pGoods);
	VOID Clear();
	VOID ClearCell(int nIndex,int nPos);
	BOOL DelTest(vector<CGoods*>& vecGoodsOut);
	BOOL DelTest(vector<tagTestArg_ID_Num>& vecArg,vector<tagDelTestResult>* vecResult);
	BOOL AddTest(vector<CGoods*>& vecGoodsIn,vector<tagTestResult>* vecResult=NULL);
	BOOL AddTest(CGoods* pGoods,vector<tagTestResult>* vecResult=NULL);
	VOID Release();
};