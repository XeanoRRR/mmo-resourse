#pragma once
#include "Monster.h"
#include "MasterInfo.h"

class  CSummonedCreature : public CMonster
{
protected:
	DWORD									m_dwSummonedTimeStamp;
	DWORD									m_dwLifeTime;

public:
	/**## �ٻ��޵��ٻ���������̣��ȴ���һ��������ʵ����Ȼ�� */
	/**## ����SetOriginalName��������Ҫ�ٻ����ֹ��Ȼ�����ú�  */
	/**## ����ʱ�䣬�Լ���Ϊģʽ�������ٻ��޵�ai�м�����Ҫע�⣬ */
	/**## һ�ǲ��ܹ������ˣ�����Ҫ��������˵��������Ҫ�ܸ��� */
	/**## ���˵���ͼ���й��� */
	CSummonedCreature						();
	virtual ~CSummonedCreature				();

	//##�̳���CMonster�ĺ���
	//virtual bool	AddToByteArray			( vector<BYTE>* pByteArray, bool bExData = true );		//## ��ӵ�CByteArray
	//virtual bool	DecordFromByteArray		( BYTE* pByte, long& pos, bool bExData = true );		//## ����
	virtual void	AI						();														//## ��Ҫ������ʱ����ж�
	//virtual void	OnDied					();														//## ������������������¼�

	//##Set����
	VOID			SetLifeTime				( DWORD dwLifeTime );									//## ����ʱ��

	//##Get����
	DWORD			GetRemainedTime			();
};
