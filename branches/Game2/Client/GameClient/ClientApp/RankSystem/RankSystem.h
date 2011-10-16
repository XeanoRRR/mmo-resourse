/**
* @filename:RankSystem.h
* @date: 2010/3/22
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ���а�ͻ���ϵͳ
*/

#pragma  once
#include "../../../../Public/Common/RankDef.h"

/////////////////////////////////////////////////
// zhaohang  2010/3/23 
// ��Ϣ����
/////////////////////////////////////////////////
/// @brief
/// ���а�Ĳ�ѯ����Ϣ�Ľ���
class RankMsgProc
{
public:
	///���캯��
	RankMsgProc(){ m_eRankQuerySate = RT_OWNER;}
	///������Ϣ
	void ReceiveMsg(BYTE* pByte, long& lPos) { m_RankInfo.DecordFromByteArray(pByte,lPos); };
	///�������а�״̬
	void SetRankState(eRankType state){ m_eRankQuerySate = state ; }

	/// @brief
	/// ��ȡ��ǰ���������ص���Ϣ��ѯ״̬
	/// @return eRankQueryState
	/// ����״̬ö��ֵ
	eRankType GetRankState() { return m_eRankQuerySate; }

	/// ��ȡ��ǰ���а��ҳ��
	long GetCurPage()const { return m_RankInfo.lCurPage ; }

	/// ��ȡ��ǰ���а��������ҳ��
	long GetCurTotalPage() {return m_RankInfo.lTotalPage ;}

	/// ��ȡ���а���Ϣ
	tagRankInfo& GetRankInfo()  { return m_RankInfo;}

	/// ��ȡ���а���
	uint GetRankID(){return (uint)m_RankInfo.lRankID ; }

	/// @brief
	/// ��������������а�����
	/// @param type
	/// ���а��ѯ����
	/// @param RankID
	/// ��ѯID
	/// @return void
	/// 
	void Query(eRankType type,long RankID,const char* QueryPlayerName,const char* QueryContent,long PageOrRank = 0);
protected:
private:
	///���а���Ϣ�ṹ��
	tagRankInfo m_RankInfo;
	/// ��ǰ���а��ѯ����
	eRankType m_eRankQuerySate; 
};

/////////////////////////////////////////////////
// zhaohang  2010/3/25 
// ��ѯ�����ö�ȡ
/// @brief
/// ���а�˵���Ӧ�Ĳ�ѯ���ݶ�ȡ������(���ļ�"setup/RankSet.ini"�ж�ȡ
class RankQueryConfig
{
public:
	///���캯��
	RankQueryConfig(){ vecMQC.clear(); }
	///��������
	~RankQueryConfig(){ vecMQC.clear(); }
	/// @brief
	/// ��ȡ��ѯ�����ļ�
	bool ReadConfig();
	/// ��ȡ���˵����С
	size_t GetMainMenuSize() const{ return vecMQC.size() ; }
	///  ����������ȡ�˵�������
	const char* GetMainMenuName(uint mainMenuIdx)const { return vecMQC[mainMenuIdx].strMainMenu.c_str(); }
	/// �������˵�������ȡ��Ӧ���Ӳ˵���С
	size_t GetSubMenuSize(size_t index) const { return vecMQC[index].vecSubItem.size(); }

	/// ָ�����˵����Ӳ˵���������ȡ�˵�����
	const std::string& GetSubMenuName(size_t mainMenuIdx,size_t subMenuIdx) const { return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].strItem ; }

	/// ָ�����˵����Ӳ˵���������ȡ�Ӳ˵�ID
	uint GetSubMenuID(size_t mainMenuIdx,size_t subMenuIdx) const { return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].itemID ;}

	/// ָ�����˵����Ӳ˵���������ȡ�˵���ѯ����
	const std::string& GetSubMenuQueryContent(size_t mainMenuIdx,size_t subMenuIdx) const { return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].strQueryContent ;}

	/// ָ�����˵����Ӳ˵���������ȡ�Ӳ˵��д�С
	size_t GetSubMenuRowSize(size_t mainMenuIdx,size_t subMenuIdx) { return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].rowInfo.size(); }
	/// ָ�����˵����Ӳ˵��������Ӳ˵�����������ȡ�Ӳ˵��е�Header
	const std::string& GetSubMenuRowHeader(size_t mainMenuIdx,size_t subMenuIdx,size_t rowHeaderIdx) const
	{
		return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].rowInfo[rowHeaderIdx].strRowHeader;
	}
	/// 
	const std::string& GetSubMenuRowDataHeader(size_t mainMenuIdx,size_t subMenuIdx,size_t rowHeaderIdx) const
	{
		return vecMQC[mainMenuIdx].vecSubItem[subMenuIdx].rowInfo[rowHeaderIdx].strRowDataHeader;
	}
protected:
private:
	
	
	///�ͻ���һ���Ӳ˵���ʾ������Ϣ����
	struct tagRowInfo{
		std::string strRowHeader;///�б���
		std::string strRowDataHeader;///�����ݿ��ж�Ӧ���ַ�����
	};
	typedef std::vector<tagRowInfo> VECRI;
	///һ���Ӳ˵���ѯ��Ҫ�洢�Ľṹ
	struct tagSubItem{
		uint itemID;///�Ӳ˵�ID
		std::string strItem;///�Ӳ˵�lable
		std::string strQueryContent;///��ѯ����
		VECRI rowInfo;///����Ϣ����
	};
	typedef std::vector<tagSubItem> VECSI;
	/// һ�����˵���ѯ��Ҫ�洢�Ľṹ
	struct tagMenuQueryConfig
	{
		uint mainMenuID;///���˵�ID
		std::string strMainMenu;///���˵�lable
		VECSI vecSubItem;///�Ӳ˵��ṹ
	};
	///������ѯ��Ҫ�洢�Ľṹ
	std::vector<tagMenuQueryConfig>vecMQC;
};
/////////////////////////////////////////////////