#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#include "..\Player.h"
class CSuitRule;
class CGoods;


using namespace std;

class CSuitRuleFactory
{
public:		
	CSuitRuleFactory();
	~CSuitRuleFactory();

	static BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
	static BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);
	static BOOL Load(const CHAR* StrPath);
	static VOID Release();

	//��װЧ������
	static BOOL SuitActive(CPlayer* pPlayer,int nNum,CGoods* pGoods);	
	//��װЧ��ȡ��
	static BOOL SuitInvalidate(CPlayer* pPlayer,int nNum,CGoods* pGoods);

private:
	static map<int,CSuitRule*> m_mRules;
};