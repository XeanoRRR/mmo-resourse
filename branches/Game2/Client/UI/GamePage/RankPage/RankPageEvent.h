/**
* @filename:RankPageEvent.h
* @date: 2010/3/23
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ���а�UI�߼���Ӧ
*/
#pragma  once
#include "../../PageEvent.h"

///���а�UI�¼���
class RankPageEvent:public PageEvent
{
public:
	///
	RankPageEvent();
	///
	RankPageEvent(EVENTTYPE type);
	///
	~RankPageEvent();
	///
	void OnPageLoad(GamePage *pPage);
	///
	void OnPageOpen(GamePage *pPage);
	///
	void OnPageClose(GamePage *pPage);
	///
	void OnPageUpdate(GamePage *pPage);
	///
	void OnPageRelease(GamePage *pPage);

private:
	/// @brief
	/// ��ʼ�����˵�Item
	bool InitMenu();

	/// @brief
	/// ���ò˵����Ӳ˵�Item��Ĭ��ѡ����
	void SetDefaultCurRankID(uint curSelRankID = 11 )
	{
		m_curRankID = curSelRankID;
	}

	/// @brief
	/// ע�����ؼ����¼���Ӧ����
	void SubscribeEvent();

	/// @brief
	/// �����ѯ����
	bool HandlQueryRank(const CEGUI::EventArgs& e);

	/// @brief
	/// �����ѯ����
	bool HandlQueryName(const CEGUI::EventArgs& e);

	/// @brief
	/// �������ҳ
	bool HandlQueryNextPage(const CEGUI::EventArgs& e);

	/// @brief
	/// ������ǰ��ҳ
	bool HandlQueryPrePage(const CEGUI::EventArgs& e);

	/// @brief
	/// �����ѯ�ҵ�����
	bool HandlMyRank(const CEGUI::EventArgs& e);

	/// @brief
	/// �������˵����Ӳ˵�ѡ��������¼�
	bool HandlListBoxSelChange(const CEGUI::EventArgs& e);

	/// @brief
	/// ����رս��水ť����¼�
	bool HandlClose(const CEGUI::EventArgs& e);

	/// @brief
	/// ����Tab�ؼ�ѡ�б仯����
	bool HandlTabSelChanged(const CEGUI::EventArgs& e);

	/// @brief
	/// ����ҳ����Ϣ
	void UpdatePageNumInfo();
	/// @brief
	/// ����rankID���Ӳ˵�ID������MultiColumnListҪ��ʾ���б�ͷ
	void UpdateMulitListHeader(uint rankID);

	/// @brief
	/// ���¶����б����ʾ����
	/// @param rankID
	/// ��rankID������������һ���Ӳ˵���ѯ����Ϣ
	void UpdateMulitListData(uint rankID);

	/////////////////////////////////////////////////
	// zhaohang  2010/3/29 
	// ��������
	/// @brief
	/// �������ƻ�ȡָ����ListBoxָ��
	/// @param lbName
	/// listbox������
	/// @return Listbox*
	/// ����ָ����ListBox��ָ��
	CEGUI::Listbox* GetListBox(const char* lbName);

	/// @brief
	/// �������ƻ�ȡMultiColumnList��ָ��
	/// @param Name
	/// ָ����MultiColumnList������
	/// @return MultiColumnList*
	/// ����ָ�����Ƶ�MultiColumnList*
	CEGUI::MultiColumnList* GetMultiColumnList(const char* Name);

	/// @brief
	/// ��ȡָ�����Ƶ�Editbox
	CEGUI::Editbox* GetEditBox(const char* name);

	/// @brief
	/// �ɲ�ѯID��ȡ���˵�ID
	/// @param rankID
	/// ��ѯ��ID��Ҳ���Խ��Ӳ˵�ID
	/// @return uint
	/// �������˵���ID
	uint GetMainMenuIDByRankID(uint rankID);

	/// @brief
	/// ��ȡ���˵�������
	/// @param rankID
	/// ��ѯ��ID��Ҳ���Խ��Ӳ˵�ID
	/// @return uint
	/// �������˵�������
	uint GetMainMenuIndexByRankID(uint rankID);

	/// @brief
	/// �ɲ�ѯID�����Ӳ˵�����ListBox�е�����
	/// @param rankID
	/// ��ѯ��ID��Ҳ���Խ��Ӳ˵�ID
	/// @return uint
	/// �Ӳ˵�����ListBox�е�����
	uint GetIndexInListBoxByRankID(uint rankID);

	/// @brief
	/// ��ʽ���ַ�����ʾ����ɫ
	/// @param str
	/// ������ַ�������
	/// @param const char* colour
	/// ��ʽ������ɫ�ַ���
	/// @return CEGUI::String&
	/// ���ظ�ʽ���ú��ַ���������
	CEGUI::String& FormatText(CEGUI::String& str,const char* colour = "[colour='FFFFFFFF']");
	/////////////////////////////////////////////////

	///���а�UI������
	CEGUI::Window* m_wnd;
	//std::map<uint,std::vector<CEGUI::ListboxTextItem*>>m_mapLbti;//�洢���е��Ӳ˵���

	uint m_curRankID;///��ǰѡ�е��Ӳ˵�ID������˶����rankID

	CEGUI::String m_strPageText;///�洢ҳ����Ϣ���ַ���  "��ǰ�� %d/%d ҳ"
};

