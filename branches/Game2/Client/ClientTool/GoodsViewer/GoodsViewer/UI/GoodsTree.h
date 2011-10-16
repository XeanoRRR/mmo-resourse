/**
* @filename:GoodsTree.h
* @date: 2010/2/4
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ����GoodsTree���ڶ�Ӧ��صĲ���,�¶���Ľ����඼Ҫ��BaseUIClass�̳ж���
*/
#include "../../CEGUIEngine/BaseUIClass.h"
#include "../GoodsData/GoodsOperate.h"

namespace UIClass
{
	class MyListItem:public CEGUI::ListboxTextItem
	{
	public:
		MyListItem(const CEGUI::String& text = ""):ListboxTextItem(text)
		{
			setSelectionBrushImage("TaharezLook","MultiListSelectionBrush");
		}
	};
	class GoodsTree:public BaseUIClass
	{
	public:
		/// @brief
		/// ���캯��
		/// @param
		/// 
		/// @return
		/// 
		GoodsTree() ;

		/// @brief
		/// ��������
		/// @param
		/// 
		/// @return
		/// 
		~GoodsTree() ;

		/// @brief
		/// ��д�ĳ�ʼ������, �����Լ���UI����,������ļ��ж�ȡ��Goods������
		/// @param
		/// 
		/// @return
		/// 
		bool Init();

		
		/// @brief
		/// ����Item��RootItem
		/// @param *ItemStr
		/// ��Ŀ����
		/// @return TreeItem*
		/// ����ָ��ǰ�����TreeItemָ��
		TreeItem* AddItemToRootItem(const String& ItemStr);

		/// @brief
		/// ����ItemContent��MultiList
		/// @param const UIData::tagGoodAttr*
		/// @param RowIdx
		/// ������к�
		/// @return
		/// 
		void SetItemContentInMulList(const UIData::tagGoodAttr* pGoodAttr,CEGUI::uint RowIdx);

		/// @brief
		/// ����BranchOpened
		/// @param args
		/// 
		/// @return
		/// 
		bool HandleEventBranchOpened(const EventArgs& args);

		/// @brief
		/// �������ο��ѡ����Ϣ
		/// @param args
		/// 
		/// @return
		/// 
		bool OnBranchSelectChanged(const EventArgs& args);

		/// @brief
		/// ������������Ŀѡ����Ϣ
		/// @param args
		/// 
		/// @return bool
		/// 
		bool OnAddAttrItemSelChanged(const EventArgs& args);
		
		/// @brief
		/// ������װ������Ŀѡ����Ϣ
		/// @param args
		/// 
		/// @return bool
		/// 
		bool OnSuitAttrItemSelChanged(const EventArgs& args);
	protected:
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
	private:
		/// @brief
		/// �������ο��Item
		/// @param &ItemStr
		/// ��Ŀ����
		/// @return TreeItem*
		/// ����ָ��ǰ����TreeItem��ָ��
		TreeItem* AddItemToTree(String& ItemStr);

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
		/// ������������ȡָ�����Ƶ�Editbox�ؼ�
		/// @param WindowName
		/// ��������
		/// @return Editbox*
		/// ����ָ�����Ƶ�Editbox��ָ��
		Editbox* GetEditbox(const char *WindowName);

		/// @brief
		/// ������������ȡָ�����Ƶ�Combobox�ؼ�
		/// @param WindowName
		/// ��������
		/// @return Combobox*
		/// ����ָ�����Ƶ�Combobox��ָ��
		Combobox* GetCombobox(const char *WindowName);

		Tree* m_GoodsTree;//ָ�����Tree��ָ��
		TreeItem* m_RootItem;//ָ�����Tree�ĸ�Item
		std::vector<TreeItem*>m_ChildRootItem;//����RootItem�ĺ��ӣ��������У������ڴ��ͷ�
		MyListItem* m_arrListItem;//ָ������,ָ������б��ĵ�Ԫ��
		static const unsigned int s_TreeID;
		static const unsigned int s_ListRowNum;//����Ŀ
		static UIData::GoodsOperate s_GoodsOP;///<�����߼�������
	};
}