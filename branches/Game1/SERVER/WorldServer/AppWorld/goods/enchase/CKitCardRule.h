#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

/*
*�׿�����
*/
class CKitCardRule:public BaseMemObj
{
public:
	CKitCardRule();
	CKitCardRule(DWORD dwId,BYTE btHoleNum,BYTE btCardNum);
	~CKitCardRule();
public:
	struct tagRuleValue
	{
		string strType;
		long  lVal;
	};
private:
	//�׿�����ID
	DWORD m_dwRuleId;
	//Ҫ�󿪿׵�����
	BYTE m_btRequireHoleNum;
	//�׿���������
	BYTE m_btRequireCardNum;
	//�׿���ID
	LONG* m_pCardIdArray;
	//��������
public:
	vector<CKitCardRule::tagRuleValue> m_vAttr;
	
public:	
	BOOL Serialize(vector<BYTE>* pStream, BOOL b = TRUE);
	BOOL Unserialize(BYTE* pStream, LONG& lOffset, BOOL b = TRUE);	

	DWORD GetRuleId(){return m_dwRuleId;}
	BYTE GetRequireHoleNum(){return m_btRequireHoleNum;}
	BYTE GetRequireCardNum(){return m_btRequireCardNum;}
	VOID SetCardIds(int* pIds);
};