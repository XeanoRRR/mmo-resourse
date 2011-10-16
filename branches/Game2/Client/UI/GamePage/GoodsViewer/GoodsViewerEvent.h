#pragma  once
#include "../../PageEvent.h"
#include "GoodsOperate.h"
#include "../../UIDef.h"


class MyListItem:public CEGUI::ListboxTextItem
{
public:
	MyListItem(const CEGUI::String& text = ""):ListboxTextItem(text)
	{
		setSelectionBrushImage("TaharezLook","MultiListSelectionBrush");
	}
};

class GVEvent:public PageEvent
{
public:
	GVEvent();
	GVEvent(EVENTTYPE type){ SetEventType(type) ; }
	~GVEvent();

	void OnPageLoad(GamePage *pPage);
	void OnPageOpen(GamePage *pPage);
	void OnPageClose(GamePage *pPage);
	void OnPageUpdate(GamePage *pPage);
	void OnPageRelease(GamePage *pPage);

	/// @brief
	/// ����Item��RootItem
	/// @param *ItemStr
	/// ��Ŀ����
	/// @return TreeItem*
	/// ����ָ��ǰ�����TreeItemָ��
	CEGUI::TreeItem* AddItemToRootItem(const CEGUI::String& ItemStr);

	/////////////////////////////////////////////////
	// zhaohang  2010/3/15 
	// �ؼ����¼���Ӧ����

	/// @brief
	/// �������ο��ѡ����Ϣ
	/// @param args
	/// 
	/// @return
	/// 
	bool OnBranchSelectChanged(const CEGUI::EventArgs& args);

	/// @brief
	/// ������������Ŀѡ����Ϣ
	/// @param args
	/// 
	/// @return bool
	/// 
	bool OnAddAttrItemSelChanged(const CEGUI::EventArgs& args);

	/// @brief
	/// ������װ������Ŀѡ����Ϣ
	/// @param args
	/// 
	/// @return bool
	/// 
	bool OnSuitAttrItemSelChanged(const CEGUI::EventArgs& args);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// zhaohang  2010/3/17 
	// ��Ʒģ�Ϳ�����Ӧ
	/////////////////////////////////////////////////
	//bool OnScalAdd(const CEGUI::EventArgs& args);
	//bool OnScalMinus(const CEGUI::EventArgs& args);
	//bool OnXMinus(const CEGUI::EventArgs& args);
	//bool OnXAdd(const CEGUI::EventArgs& args);
	//bool OnYMinus(const CEGUI::EventArgs& args);
	//bool OnYAdd(const CEGUI::EventArgs& args);

	//bool OnLeftRot(const CEGUI::EventArgs& args);
	//bool OnRightRot(const CEGUI::EventArgs& args);
	//bool OnUpRot(const CEGUI::EventArgs& args);
	//bool OnDownRot(const CEGUI::EventArgs& args);

	/////////////////////////////////////////////////
	// zhaohang  2010/3/18 
	// �µĿ���ģ�ͺ���
	bool HandScal(const CEGUI::EventArgs& args);
	bool HandXMove(const CEGUI::EventArgs& args);
	bool HandYMove(const CEGUI::EventArgs& args);
	bool HandXRot(const CEGUI::EventArgs& args);
	bool HandYRot(const CEGUI::EventArgs& args);
	bool HandZRot(const CEGUI::EventArgs& args);
	/////////////////////////////////////////////////
protected:
	
private:
	/// @brief
	/// //�Ƿ�����ݿ�Combobox��Ŀ�ĳ�ʼ��
	/// @param
	/// 
	/// @return void
	/// 
	void InitIsDBCombobox();
	/// @brief
	/// //��Ʒ����Combobox��Ŀ�ĳ�ʼ��
	/// @param void
	/// 
	/// @return void
	/// 
	void InitGoodTypeCombobox();

	/// @brief
	/// ����ʱ�Ƿ����Combobox��Ŀ�ĳ�ʼ��
	/// @param
	/// 
	/// @return
	/// 
	void InitIsSoundSwitchCombobox();

	/// @brief
	/// ��ʼ����������Combobox��Ŀ
	/// @param
	/// 
	/// @return
	/// 
	void InitAddAttrCombobox();

	/// @brief
	/// ��ʼ����������ֵ��Combobox(�Ƿ���Ч���Ƿ�����)
	/// @param void
	/// 
	/// @return void
	/// 
	void InitAddAttrValueSelCombobox();

	/// @brief
	/// ��ʼ����װ����Combobox����װ����)
	/// @param
	/// 
	/// @return
	/// 
	void InitSuitAttrCombobox();

	/// @brief
	/// 
	/// @param
	/// 
	/// @return
	/// 
	void ReSetScrollPos();
	/// @brief
	/// ������������ȡָ�����Ƶ�Editbox�ؼ�
	/// @param WindowName
	/// ��������
	/// @return Editbox*
	/// ����ָ�����Ƶ�Editbox��ָ��
	CEGUI::Editbox* GetEditbox(const char *WindowName);

	/// @brief
	/// ������������ȡָ�����Ƶ�Combobox�ؼ�
	/// @param WindowName
	/// ��������
	/// @return Combobox*
	/// ����ָ�����Ƶ�Combobox��ָ��
	CEGUI::Combobox* GetCombobox(const char *WindowName);

	/// @brief
	/// �������ο��Item
	/// @param &ItemStr
	/// ��Ŀ����
	/// @return TreeItem*
	/// ����ָ��ǰ����TreeItem��ָ��
	CEGUI::TreeItem* AddItemToTree(const CEGUI::String& ItemStr);

	/////////////////////////////////////////////////
	// zhaohang  2010/3/16 
	// �ؼ�������ʾ����function

	/// @brief
	/// ����ItemContent��MultiList
	/// @param const UIData::tagGoodAttr*
	/// @param RowIdx
	/// ������к�
	/// @return
	/// 
	void SetItemContentInMulList(const UIData::tagGoodAttr* pGoodAttr,CEGUI::uint RowIdx);

	/// @brief
	/// ���»����������ݵ���ʾ
	/// @param pGoodAttr
	/// ������Ʒ������ָ��
	/// @return
	/// void
	void UpdateBaseAttrDisplay(const UIData::tagGoodAttr* pGoodAttr);

	/// @brief
	/// ������Ʒ�ͻ��������������ݵ���ʾ
	/// @param pGoodAttr
	/// ������Ʒ���Ե�ָ��
	/// @return
	/// void 
	void UpdateClientAttrDisplay(const UIData::tagGoodAttr* pGoodAttr);

	/// @brief
	/// ������Ʒ�����������ݵ���ʾ
	/// @param pGoodAttr
	/// ������Ʒ���Ե�ָ��
	/// @return
	/// void
	void UpdateAddAttrDisplay(const UIData::tagGoodAttr* pGoodAttr);

	/// @brief
	/// ������Ʒ��װ�������ݵ���ʾ
	/// @param pGoodAttr
	/// ������Ʒ���Ե�ָ��
	/// @return
	/// void
	void UpdateSuitAttrDisplay(const UIData::tagGoodAttr* pGoodAttr);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// zhaohang  2010/3/15 
	CEGUI::Window* m_wnd;//GoodsViewer����ĸ�����
	CEGUI::Tree* m_GoodsTree;  //ָ�����Tree��ָ��
	CEGUI::TreeItem* m_RootItem;//ָ�����Tree�ĸ�Item
	std::vector<CEGUI::TreeItem*>m_ChildRootItem;//����RootItem�ĺ��ӣ��������У������ڴ��ͷ�

	UIData::GoodsOperate m_GoodsOP;//��Ʒ���ݲ�����,ʹ�ö�̬�ڴ���䷽ʽ���ͷŷ���OnPageRelease() function��

	int m_xMove;
	int m_yMove;
	int m_zMove;
	int m_xRot;
	int m_yRot;
	int m_zRot;
	/////////////////////////////////////////////////
	
};