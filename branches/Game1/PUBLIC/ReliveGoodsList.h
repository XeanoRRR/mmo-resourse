/*
*	file:		ReliveGoodsList.h
*	Brief:		������Ʒ�б���
*	detail:		
*	Authtor:	���Ƚ�
*	Datae:		2009-08-27
*/

#include "tinyxml.h"

// ������Ʒ�б���
class CReLiveGoodsList
{
public:	
	//������Ʒ�ṹ
	struct tagReLiveGoods
	{
		string strOrignalName;	//ԭʼ��
		DWORD  dwIndex ;		//����
		DWORD  dwNum;			//����
		DWORD  dwRecoverRatio;	//�ָ�����
	};
	//���վ�����Ʒ�Ľṹ
	struct tagExpGoods
	{
		/*string strOrignalName;	//ԭʼ��*/
		DWORD   dwExpRatio;		//��ȡ����
		string strScript;		//�����ű���
		string strHint;			//��ʾ����
	};


	CReLiveGoodsList();
	~CReLiveGoodsList();

	
	static bool Init(const char *filename);						// ����XML�й��ڸ�����Ʒ������
	static void Release();										// �ͷ���Դ	

	static bool AddToByteArray(vector<BYTE> &pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����	
	static list<tagReLiveGoods*> &GetReliveGoodsList() {return m_lReLiveGoods;}//��ȡ������Ʒ�б�
	static tagExpGoods* GetExpGoods(string strName);
	
private:
	static list<tagReLiveGoods*>	 m_lReLiveGoods;				// ���еĸ�����Ʒ�б�
	static map<string,tagExpGoods*>  m_mpExpGoods;					// ���վ������Ʒ�б�
	typedef map<string,tagExpGoods*>::iterator Eit;
};