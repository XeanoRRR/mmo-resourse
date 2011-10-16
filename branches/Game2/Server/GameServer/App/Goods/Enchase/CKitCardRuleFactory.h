#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include "../../Player.h"
#include "../../../../../Public/Setup/KitCardRuleSetup.h"

class CGoods;

class CKitCardRuleFactory 
{
public:
	CKitCardRuleFactory();
	~CKitCardRuleFactory();	

public:	
	//�׿�Ч������
	static bool KitCardActive(CPlayer* pPlayer,CGoods* pGoods);

	//�׿�Ч��ȡ��
	static bool KitCardInvalidate(CPlayer* pPlayer,CGoods* pGoods,ulong dwKitCardId);

private:
	static bool RuleActive(CPlayer* pPlayer,CGoods* pGoods,ulong dwHoleNum,vector<ulong>& vecIds,
		KitCardRuleSetup::KitCardRule* kitCard);

	static bool RuleInvalidate(CPlayer* pPlayer,CGoods* pGoods, KitCardRuleSetup::KitCardRule* kitCard);
};