#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include "../MessagePackage/CS2CContainerObjectMove.h"

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
	void Init(CPlayer* pPlayer);
	/// �ֶ���Ӳ��Ա���
	void AddContainer( DWORD dwContainerID, CVolumeLimitGoodsContainer* pVolumeContainer );
	void FillCell(int nIndex,CVolumeLimitGoodsContainer* pVolumeContainer);
	bool FillOneGoods(CGoods* pGoods,tagTestResult* pResult);
	bool ClearOneGoods(CGoods* pGoods);
	void Clear();
	void ClearCell(int nIndex,int nPos);
	bool DelTest(vector<CGoods*>& vecGoodsOut);
	bool DelTest(vector<tagTestArg_ID_Num>& vecArg,vector<tagDelTestResult>* vecResult);
	bool AddTest(vector<CGoods*>& vecGoodsIn,vector<tagTestResult>* vecResult=NULL);
	bool AddTest(CGoods* pGoods,vector<tagTestResult>* vecResult=NULL);
	void Release();
};