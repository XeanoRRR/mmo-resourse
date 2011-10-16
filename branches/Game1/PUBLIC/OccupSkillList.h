#pragma once

/**************************************************/
// ������Ҫ�����Ǳ���ÿ��ְҵ����ӵ�еļ������ñ�
// ���ṩһЩ�ӿڣ����ڲ���ĳ��ְҵ�����м��ܵ�
// 07.10.30 ����
/**************************************************/


#include "tinystr.h"
#include "tinyxml.h"
#include "tools.h"
#include <list>
using namespace std;


// ����ְҵ�ļ�����
class COccupSkill
{
public:
	// ְҵ���ܽṹ
	struct tagOccupSkill
	{
		string strName;		// ���ܵ�����
		long lSkillID;		// ����ID
		long lLeve;			// ����ܹ����ļ���
		tagOccupSkill()
		{
			strName = "";
			lSkillID = 0;
			lLeve = 0;
		}
	};
	COccupSkill();
	~COccupSkill();

	bool Init(TiXmlElement* pElem);		// ����XML�й��ڼ��ܵ�����

	void SetOccupId(long lId)	{m_lOccupId = lId;}
	long GetOccupId()	{return m_lOccupId;}

	void SetName(const char * strName)	{m_strName = strName;}			// ְҵ�������úͶ�ȡ
	const char * GetName()	{return m_strName.c_str();}

	void SetDesc(const char * strDesc)	{m_strDesc = strDesc;}			// ְҵ���������úͶ�ȡ
	const char * GetDesc()	{return m_strDesc.c_str();}

	long GetOccupSkillCount()	{return (long)(m_vecSkill.size()-1);}	// �õ���ְҵ��ӵ�еļ�����������0��ʼ���㣩

	vector<tagOccupSkill*> GetOccupSkill() {return m_vecSkill;}			// ��ø�ְҵ�ļ��ܱ�
	void SetSkill(tagOccupSkill *ptgOccupSkill) { m_vecSkill.push_back(ptgOccupSkill);}		// Ϊ��ְҵ�ļ��ܱ����һ��������Ϣ

	// ��ø�ְҵ�µ�ָ�����ܵ����ȼ�
	long GetOccupSkillMaxLev(DWORD dwSkillID);

private:
	string m_strName;		// ְҵ��
	string m_strDesc;		// ְҵ����
	long m_lOccupId;		// ְҵID(ͼƬID�����߶�Ӧenum eOccupation�е�ֵ)
	vector<tagOccupSkill*> m_vecSkill;	// ��ְҵ��Ӧ�ļ����б�
	void Release();			// �ͷ���Դ
};


// ����ְҵ�����б�
class COccupSkillList
{
private:
	static list<COccupSkill *> m_OccupSkillList;		// ���е�ְҵ�����б�
	

public:
	COccupSkillList();
	~COccupSkillList();
	static void Release();		// �ͷ���Դ
	static bool Init(const char *filename);			// ��ʼ������XML�����ļ�
	list<COccupSkill *> * GetOccupList() {return &m_OccupSkillList;}
	static COccupSkill * GetOccupSkill(long lOccupID);	// ����ְҵID��ö�Ӧְҵ�ļ����б���
	static bool OccuJudgeStudy(DWORD dwID,DWORD dwLv,long lOccupID );
	
	
	static bool AddToByteArray(vector<BYTE> &pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
 
};


/*******************************************************************
* תְϵͳ����
* ժҪ: �ͻ��˺ͷ������ɶ�ȡ���õ�תְ���������ж�
*		�ͻ��˻���ͨ��������תְ·������
* ����: ����
* ��������: 2009.02.25
********************************************************************/

class CChangeOccu
{
private:
	// ְҵ�ṹ
	struct tagChangeOccu
	{
		string strName;			// ְҵ����
		string strGoodsName;	// ��Ҫ��Ʒԭʼ��
		int nID;				// ְҵID
		int nParentID;			// ��ְҵְҵID
		int nValue;				// �ڵ�ID
		long lGoodsNum;			// ��Ʒ����
		tagChangeOccu()
		{
			strName = "";
			strGoodsName = "";
			nID = 0;
			nParentID = 0;
			nValue = 0;
			lGoodsNum = 0;
		}
	};
	static vector<tagChangeOccu>	m_vecSubOccu;								// ��ְҵ����ְҵ�б�
public:
	CChangeOccu();
	~CChangeOccu();
	static void Release();														// �ͷ���Դ
	static bool Init(const char *filename);										// ��ʼ������XML�����ļ�
	static void GetChangeOccuCondision(BYTE bOccu,string &str,int &num);		// ���ݴ����ְҵ�����תְ��������Ʒ����������
	static void GetChangeOccuPath(vector<BYTE> &vecOccu,BYTE bStart,BYTE bEnd);	// ���ݿ�ʼ�ͽ�����ְҵ������תְ·��
};