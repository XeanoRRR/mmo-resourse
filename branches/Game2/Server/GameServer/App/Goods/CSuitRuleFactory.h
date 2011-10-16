#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#include "../Player.h"

#include "../../../../Public/Setup/SuitRuleSetup.h"

class CGoods;


class CSuitRuleFactory
{
public:		
	CSuitRuleFactory();
	~CSuitRuleFactory();

	//��װЧ������
	static BOOL SuitActive(CPlayer* pPlayer,int nNum,CGoods* pGoods);	
	//��װЧ��ȡ��
	static BOOL SuitInvalidate(CPlayer* pPlayer,int nNum,CGoods* pGoods);

private:
	static BOOL RuleActive(CPlayer* pPlayer,int suit_num, SuitRuleSetup::SuitRule* Suit);
	static BOOL RuleInvalidate(CPlayer* pPlayer,int suit_num, SuitRuleSetup::SuitRule* Suit); 
};