/************************************************************************/
/* ���ﾫ�� ģ��  nikai 2009/1/10/                                    */
/************************************************************************/
#pragma once
#include "CPet.h"
class CFairy:public	CPet
{
private:
	PET_FOOD_SATIETY			m_emFoodsSatiefy;	//���ﱥʳ��

public:
	CFairy();
	virtual ~CFairy();
	virtual void AI();
	virtual bool Display();

	inline void SetSatiefyState(PET_FOOD_SATIETY emSatify)	{m_emFoodsSatiefy = emSatify;}
	
	PET_FOOD_SATIETY GetSatietyState(void);	//��þ��鱥ʳ�ȵȼ�

	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData = true);
};