//======================================================================================
/**
*	@file		ReliveGoodsList.h
*	@data		2009-08-27
*	@author		���Ƚ�
*/
//======================================================================================
#pragma  once


/**
*	@class ReLiveGoodsList
*
*	@brief ������Ʒ�б���
*/
class ReLiveGoodsList
{
public:	
	/// ������Ʒ�ṹ
	struct tagReLiveGoods
	{
		string strOrignalName;	//ԭʼ��
		ulong  dwIndex ;		//����
		ulong  dwNum;			//����
		ulong  dwRecoverRatio;	//�ָ�����
	};
	
	/// ����
	ReLiveGoodsList();

	/// ����
	~ReLiveGoodsList();

	/// ���ļ����ظ�����Ʒ�б�
	static bool Init(const char* filename);

	/// ���
	static void Release();

	/// ����
	static bool AddToByteArray(vector<uchar>& pByteArray);

	/// ����	
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ��ȡ������Ʒ�б�
	static list<tagReLiveGoods*>& GetReliveGoodsList() {return m_lReLiveGoods;}
	
private:
	/// ���еĸ�����Ʒ�б�
	static list<tagReLiveGoods*> m_lReLiveGoods;
};