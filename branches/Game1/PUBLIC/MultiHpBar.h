////////////////////////////////////////////////////////////
//     �ļ���   MultiHpBar.h
//	   ����		heliangchen
//	   ʱ��		08/8/25
//	   ���ܽ��ܣ� ��������Ѫ�����ܣ�ʵ�ֿ���Ӷ��Ѫ����
//				  ÿ��Ѫ���ֱ���hpֵ�� ��ɫֵ�� �ʹ����ű����������ã�
///////////////////////////////////////////////////////////
#pragma once
class CMultiHpBar
{
	struct HpBar
	{
		//��¼Ѫ����Χ
		int HpRange;
		//����״̬
		BOOL  SpringState;
		//�ű����֣�������������Ѫ����ʧʱ������
		string ScriptName;
		HpBar();
	};
public:
	struct HpBarInfo
	{
		//Ѫ����ʾ��
		float Scale;
		//��ǰѪ������ѩ��
		int HpRange;
		//hp�ڶ�Ѫ�������ĵȼ�
		int Level;
		//�����ű���
		const char* ScriptName;
		HpBarInfo(float scale,int hprange,int lev,const char*csript)
			:Scale(scale),HpRange(hprange),Level(lev),ScriptName(csript)
		{}
	};
public:
	CMultiHpBar();
	~CMultiHpBar();
	//���һ��Ѫ��
	bool Clean();
	bool AddHpBar(int AddHp, const char* scriptname);
	bool AddDatatoByteArray(vector<BYTE>* pByteArray);
	bool DecordDataFromByteArray(BYTE* pByte, long& pos);

	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	int  GetMaxHp();
public:
	//��ȡ��ǰ��״̬�� ������� Hp ��ǰhp�� ��ֵ����level ��ǰ�ȼ��� ��ȡ����collor ��ǰ��ɫ�� state ����״̬
	//����ֵ ��ǰ�����Ľű���������û�з���NULL��
	HpBarInfo GetCurrentInfo(int Hp,int ChangeHp=0);
	void SetBaseMaxHp(int BaseMaxHp);
private:
	//Ѫ���б�
	long m_BaseMaxHp;
	list<HpBar> m_listHpBar;
};