#include "stdafx.h"
#include "GoodsTree.h"
#include "../GoodsModelManager.h"
#include "../../CEGUIEngine/CEGUIManager.h"

namespace UIClass
{
	const unsigned int GoodsTree::s_TreeID = 1;
	const unsigned int GoodsTree::s_ListRowNum = 15;
	UIData::GoodsOperate GoodsTree::s_GoodsOP;
#define IMAGES_FILE_NAME		"TaharezLook"
#define BRUSH_NAME				"TextSelectionBrush"
#define MULTILISTSEL_BRUSH_NAME "MultiListSelectionBrush"
#define MODEL_NAME				"\\model"
	GoodsTree::GoodsTree()
	{
		m_GoodsTree = NULL;
		m_RootItem = NULL;
		m_arrListItem = NULL;
	}

	GoodsTree::~GoodsTree()
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(m_RootWindow);
		//�������һ������������ݷ�ʽ
		if(m_arrListItem !=NULL)
			delete [] m_arrListItem;
		for(size_t size = 0 ;size < m_ChildRootItem.size(); ++size)
		{
			if(m_ChildRootItem[size] != NULL)
				delete m_ChildRootItem[size];
		}
		m_ChildRootItem.clear();
	}
	bool GoodsTree::Init()
	{
		WindowManager& WndMgr = WindowManager::getSingleton();
		m_RootWindow = WndMgr.loadWindowLayout("GoodsTree.layout");
		//����Ϊ�ֶ�����
		m_RootWindow->setDestroyedByParent(false);
		//���ص�CEGUImanager�����ĵװ棨�������ϣ�
		CEGUIManager::GetInstance()->GetRootWindow()->addChildWindow(m_RootWindow);

		//Tree�ĳ�ʼ��
		m_GoodsTree = (Tree*)m_RootWindow->getChild(s_TreeID);
		m_GoodsTree->initialise();
		//�Ƿ�����ݿ�Combox��Ŀ�ĳ�ʼ��
		InitIsDBCombobox();
		//��Ʒ������Ŀ�ĳ�ʼ��
		InitGoodTypeCombobox();
		//�������Ƿ����Combobox��Ŀ�ĳ�ʼ��
		InitIsSoundSwitchCombobox();
		//��������Combobox��Ŀ��ʼ��
		s_GoodsOP.LoadGoodsItemTypeData();//��Ҫ��ȡ�ļ�
		InitAddAttrCombobox();
		//��ʼ����������ֵ��Combobox(�Ƿ���Ч���Ƿ�����)
		InitAddAttrValueSelCombobox();
		//��ʼ����װ����Combobox����װ����)
		InitSuitAttrCombobox();
		//ע���¼���Ӧ
		//.............
		m_GoodsTree->subscribeEvent(Tree::EventSelectionChanged,Event::Subscriber(&GoodsTree::OnBranchSelectChanged,this));//����TreeItem�¼�

		MultiColumnList* mclbox = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/AddProperty/MultiColumnList"));
		mclbox->subscribeEvent(MultiColumnList::EventSelectionChanged,Event::Subscriber(&GoodsTree::OnAddAttrItemSelChanged,this));//����������Ŀѡ���¼�

		mclbox = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/GroupBoxSuitAtrr/MultiColumnList"));
		mclbox->subscribeEvent(MultiColumnList::EventSelectionChanged,Event::Subscriber(&GoodsTree::OnSuitAttrItemSelChanged,this));//��װ������Ŀѡ���¼�
		//���RootItem��Tree
		m_RootItem = AddItemToTree(String("AllGoods"));
		
		if(s_GoodsOP.LoadGoodsListData() == true) //��ȡGoods���ݣ�����䵽TreeItem
		{
			size_t GoodsTotalNum = s_GoodsOP.GetGoodsTotalNum();

			//���ʹ��һ������������ݣ����ڶ��Ϸ�������б��ĵ�Ԫ��
			//m_arrListItem = new MyListItem[s_ListRowNum * GoodsTotalNum];

			for(size_t size = 0; size < GoodsTotalNum ; ++size)
			{
				const UIData::tagGoodAttr *GoodAttr = s_GoodsOP.GetGoodAttrByIndex(size);
				if(GoodAttr != NULL)
				{
					TreeItem* Item = AddItemToRootItem(GoodAttr->strOrigName);
					if(Item != NULL)
						m_ChildRootItem.push_back(Item);
					//�������Item��MultiList��
					//SetItemContentInMulList(GoodAttr,(CEGUI::uint)size);
				}
			}
		}
		m_RootItem->toggleIsOpen();//Ĭ��չ��Tree
		return true;
	}

	void GoodsTree::InitIsDBCombobox()
	{
		//�Ƿ�����ݿ�Combox��Ŀ�ĳ�ʼ��
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombBoxIsDB");
		ListboxTextItem* itm;
		itm = new ListboxTextItem("True",0);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
		itm = new ListboxTextItem("False",1);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
	}
	void GoodsTree::InitGoodTypeCombobox()
	{
		//��Ʒ������Ŀ�ĳ�ʼ��
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombboxGoodType");
		ListboxTextItem* itm = NULL;
		for(int i = 0 ; i < UIData::gGoodsTypeNum ; ++i)
		{
			itm = new ListboxTextItem(s_GoodsOP.GetGoodsType(i),i);
			itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
			cbbo->addItem(itm);
		}
	}
	void GoodsTree::InitIsSoundSwitchCombobox()
	{
		//����ʱ�Ƿ����
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombBoxIsDB1");
		ListboxTextItem* itm;
		itm = new ListboxTextItem("True",0);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
		itm = new ListboxTextItem("False",1);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
	}

	void GoodsTree::InitAddAttrCombobox()
	{
		//��������
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox");
		ListboxTextItem* itm;
		for(size_t i = 0 ; i < s_GoodsOP.GetGoodItemTypeSize(); ++i)
		{
			itm = new ListboxTextItem(s_GoodsOP.GetGoodItemType(i),(CEGUI::uint)i);
			itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
			cbbo->addItem(itm);
		}
	}
	void GoodsTree::InitAddAttrValueSelCombobox()
	{
		//�Ƿ���Ч
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox1");
		ListboxTextItem* itm;
		itm = new ListboxTextItem("True",0);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
		itm = new ListboxTextItem("False",1);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
		//�Ƿ�����
		cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox11");
		itm = new ListboxTextItem("True",0);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
		itm = new ListboxTextItem("False",1);
		itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
		cbbo->addItem(itm);
	}
	void GoodsTree::InitSuitAttrCombobox()
	{
		//��װ����
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/GroupBoxSuitAtrr/CombBoxSuitAttr");
		ListboxTextItem* itm;
		for(size_t i = 0 ; i < s_GoodsOP.GetGoodItemTypeSize(); ++i)
		{
			itm = new ListboxTextItem(s_GoodsOP.GetGoodItemType(i),(CEGUI::uint)i);
			itm->setSelectionBrushImage(IMAGES_FILE_NAME,MULTILISTSEL_BRUSH_NAME);
			cbbo->addItem(itm);
		}
	}
	TreeItem* GoodsTree::AddItemToTree(String& ItemStr)
	{
		if(m_GoodsTree != NULL)
		{
			TreeItem *Item = new TreeItem(ItemStr);
			Item->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			m_GoodsTree->addItem(Item);
			return Item;
		}
		else
			return NULL;
	}

	TreeItem* GoodsTree::AddItemToRootItem(const String& ItemStr)
	{
		if(m_RootItem != NULL)
		{
			TreeItem *Item = new TreeItem(ItemStr);
			Item->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			m_RootItem->addItem(Item);
			return Item;
		}
		else
			return NULL;
	}
	void GoodsTree::SetItemContentInMulList(const UIData::tagGoodAttr* pGoodAttr,CEGUI::uint RowIdx)
	{
		if(pGoodAttr!=NULL)
		{
			MultiColumnList* mclbox = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/MultiColumnList"));
			//�����
			mclbox->resetList();
			//����ÿһ�е�ֵ
			mclbox->addRow();

			/*m_arrListItem[s_ListRowNum* RowIdx].setText(PropertyHelper::intToString(pGoodAttr->dwIndex));
			mclbox->setItem( &m_arrListItem[s_ListRowNum* RowIdx],0,RowIdx);*/
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwIndex)),0,RowIdx);
			mclbox->setItem(new MyListItem(CEGUI::String(pGoodAttr->strOrigName)),1,RowIdx);
			mclbox->setItem(new MyListItem(CEGUI::String(pGoodAttr->strName)),2,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::boolToString(pGoodAttr->bSave)),3,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwIconId)),4,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwGroundId)),5,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwEquipID)),6,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwValue)),7,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwSilverValue)),8,RowIdx);
			mclbox->setItem(new MyListItem(s_GoodsOP.GetGoodsType(pGoodAttr->dwType)),9,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwSound)),10,RowIdx);
			mclbox->setItem(new MyListItem(pGoodAttr->strContent),11,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwSoundID1)),12,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(pGoodAttr->dwSoundID2)),13,RowIdx);
			mclbox->setItem(new MyListItem(PropertyHelper::boolToString(pGoodAttr->bSoundSwitch)),14,RowIdx);

			//mclbox->setProperty("Font","Chinese");
		}
	}

	bool GoodsTree::OnBranchSelectChanged(const EventArgs& args)
	{
		const TreeEventArgs& treeArgs = static_cast<const TreeEventArgs&>(args);
		TreeItem *selItem = treeArgs.treeItem;
		if(selItem)
		{
			const String &str = selItem->getTextVisual();
			const UIData::tagGoodAttr *goodAttr = s_GoodsOP.GetGoodAttrByOgriName(str);
			if(goodAttr)
			{
				SetItemContentInMulList(goodAttr,0);
				//���»�������
				UpdateBaseAttrDisplay(goodAttr);
				//���¿ͻ�������
				UpdateClientAttrDisplay(goodAttr);
				//���¸�������
				UpdateAddAttrDisplay(goodAttr);
				//������װ����
				UpdateSuitAttrDisplay(goodAttr);
				String strModelGrapID = PropertyHelper::intToString(goodAttr->dwIconId);
				string strModelName = strModelGrapID.c_str();
				strModelName.insert(0,"\\");
				strModelName += MODEL_NAME;
				GoodsModelManager::GetInstance()->OpenModel(strModelName.c_str(),false);
			}
		}
		return true;
	}

	bool GoodsTree::OnAddAttrItemSelChanged(const EventArgs& args)
	{
		//������������Ŀѡ����Ϣ
		MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/AddProperty/MultiColumnList"));
		MyListItem* lti = static_cast<MyListItem*>(mcl->getFirstSelectedItem());
		if(!lti)
		{
			//���¶�Ӧ�Ŀؼ�
			//�Ƿ���Ч
			Combobox* cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox1");
			cbbo->setItemSelectState(1,false);
			cbbo->setItemSelectState((size_t)0,false);
			//�Ƿ�����
			cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox11");
			cbbo->setItemSelectState(1,false);
			cbbo->setItemSelectState((size_t)0,false);
			//ֵ1
			Editbox* edbox = GetEditbox("GoodsTreeFrame/AddProperty/EditBoxValue1");
			edbox->setText("");
			//ֵ2
			edbox = GetEditbox("GoodsTreeFrame/AddProperty/EditBoxValue11");
			edbox->setText("");
			return false;
		}
		//��ȡѡ�еĸ�����������
		const String &strAttrName = lti->getText();
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		//��ȡѡ�еĸ��������Ƿ���Ч
		const String &strEnable = lti->getText();
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		//�Ƿ�����
		const String &strHide = lti->getText();
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		//ֵ1
		const String &strValue1 = lti->getText();
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		//ֵ2
		const String &strValue2 = lti->getText();

		//���¶�Ӧ�Ŀؼ�
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox1");
		bool b = PropertyHelper::stringToBool(strEnable);
		//�Ƿ���Ч
		if( b == false)
			cbbo->setItemSelectState(1,true);
		else
			cbbo->setItemSelectState((size_t)0,true);
		b = PropertyHelper::stringToBool(strHide);
		cbbo = GetCombobox("GoodsTreeFrame/AddProperty/GroupBox11");
		//�Ƿ�����
		if(b == false)
			cbbo->setItemSelectState(1,true);
		else
			cbbo->setItemSelectState((size_t)0,true);
		//ֵ1
		Editbox* edbox = GetEditbox("GoodsTreeFrame/AddProperty/EditBoxValue1");
		edbox->setText(strValue1);
		//ֵ2
		edbox = GetEditbox("GoodsTreeFrame/AddProperty/EditBoxValue11");
		edbox->setText(strValue2);
		return true;
	}

	bool GoodsTree::OnSuitAttrItemSelChanged(const EventArgs& args)
	{
		//������װ������Ŀѡ����Ϣ
		MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/GroupBoxSuitAtrr/MultiColumnList"));
		MyListItem* lti = static_cast<MyListItem*>(mcl->getFirstSelectedItem());
		if(!lti)
		{
			//���¶�Ӧ�Ŀؼ�
			//ֵ1
			Editbox* edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox");
			edbox->setText("");
			//ֵ2
			edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox1");
			edbox->setText("");
			//��װ����
			edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox2");
			edbox->setText("");
			return false;
		}
		//��ȡѡ�е���װ��������
		const String &strAttrName = lti->getText();
		//ֵ1
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		const String &strValue1 = lti->getText();
		//ֵ2
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		const String &strValue2 = lti->getText();
		//��װ����
		lti = static_cast<MyListItem*>(mcl->getNextSelected(lti));
		const String &strSuitNum = lti->getText();

		//���¶�Ӧ�Ŀؼ�
		//ֵ1
		Editbox* edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox");
		edbox->setText(strValue1);
		//ֵ2
		edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox1");
		edbox->setText(strValue2);
		//��װ����
		edbox = GetEditbox("GoodsTreeFrame/GroupBoxSuitAtrr/EditBox2");
		edbox->setText(strSuitNum);
		return true;
	}

	void GoodsTree::UpdateBaseAttrDisplay(const UIData::tagGoodAttr* pGoodAttr)
	{
		if(!pGoodAttr)
			return;
		//��ƷID
		Editbox *pEditBox = GetEditbox("GoodsTreeFrame/BaseProperty/EditIndex");
		if(pEditBox)
		{
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwIndex));
		}
		//��Ʒ����
		pEditBox = GetEditbox("GoodsTreeFrame/BaseProperty/EditDisName");
		if(pEditBox)
		{
			//std::wstring wstr());
			pEditBox->setText("���");
		}
		//��Ʒ�۸�
		pEditBox = GetEditbox("GoodsTreeFrame/BaseProperty/EditPrice");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwValue));
		//��Ʒԭʼ����
		pEditBox = GetEditbox("GoodsTreeFrame/BaseProperty/EditOgriName");
		if(pEditBox)
			pEditBox->setText(pGoodAttr->strOrigName.c_str());
		//��Ʒ�Ƿ�����ݿ�
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombBoxIsDB");
		if(pGoodAttr->bSave == false)
			cbbo->setItemSelectState(1,true);
		else
			cbbo->setItemSelectState(size_t(0),true);
		//��Ʒ����
		cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombboxGoodType");
		cbbo->setItemSelectState(pGoodAttr->dwType,true);
	}
	void GoodsTree::UpdateClientAttrDisplay(const UIData::tagGoodAttr* pGoodAttr)
	{
		if(!pGoodAttr)
			return;
		//�ͻ��˽���ͼ��ID
		Editbox* pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwIconId));
		//���ͼ��ID
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID1");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwGroundId));
		//��װͼ��ID
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID2");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwEquipID));
		//ʰȡ����ID
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID3");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwSound));
		//�Ӷ������˺�����ID
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID4");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwSoundID1));
		//������б���������ID
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID5");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwSoundID2));
		//������������
		pEditBox = GetEditbox("GoodsTreeFrame/ClientProperty/EditGraID51");
		if(pEditBox)
			pEditBox->setText(PropertyHelper::intToString(pGoodAttr->dwWeaponActType));
		//����ʱ�Ƿ����
		Combobox* cbbo = GetCombobox("GoodsTreeFrame/BaseProperty/CombBoxIsDB1");
		if(pGoodAttr->bSoundSwitch == false)
			cbbo->setItemSelectState(1,true);
		else
			cbbo->setItemSelectState(size_t(0),true);
		//����
		MultiLineEditbox* muleditbox = static_cast<MultiLineEditbox*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/ClientProperty/Content"));
		muleditbox->setText(pGoodAttr->strContent);
	}

	void GoodsTree::UpdateAddAttrDisplay(const UIData::tagGoodAttr* pGoodAttr)
	{
		//������Ʒ����������ʾ
		if(!pGoodAttr)
			return;
		MultiColumnList* mclbox = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/AddProperty/MultiColumnList"));
		if(!mclbox)
			return;
		//�����
		mclbox->resetList();
		for(size_t size = 0 ; size < pGoodAttr->vecAddAttr.size(); ++size)
		{
			//����ÿһ�е�ֵ
			mclbox->addRow();
			const UIData::tagAddAttr &addAttr = pGoodAttr->vecAddAttr[size];
			mclbox->setItem(new MyListItem(s_GoodsOP.GetGoodItemType(addAttr.wType)),0,(CEGUI::uint)size);
			mclbox->setItem(new MyListItem(PropertyHelper::boolToString(addAttr.bEnable)),1,(CEGUI::uint)size);
			mclbox->setItem(new MyListItem(PropertyHelper::boolToString(addAttr.bHide)),2,(CEGUI::uint)size);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(addAttr.lValue1)),3,(CEGUI::uint)size);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(addAttr.lValue2)),4,(CEGUI::uint)size);
		}
	}

	void GoodsTree::UpdateSuitAttrDisplay(const UIData::tagGoodAttr* pGoodAttr)
	{
		//������װ����
		if(!pGoodAttr)
			return;
		MultiColumnList* mclbox = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("GoodsTreeFrame/GroupBoxSuitAtrr/MultiColumnList"));
		if(!mclbox)
			return;
		//�����
		mclbox->resetList();
		for(CEGUI::uint i = 0 ; i < pGoodAttr->vecSuitAttr.size() ; ++i)
		{
			//����ÿһ�е�ֵ
			mclbox->addRow();
			const UIData::tagSuitAttr& suitAttr = pGoodAttr->vecSuitAttr[i];
			mclbox->setItem(new MyListItem(s_GoodsOP.GetGoodItemType(suitAttr.wType)),0,i);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(suitAttr.lValue1)),1,i);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(suitAttr.lValue2)),2,i);
			mclbox->setItem(new MyListItem(PropertyHelper::intToString(suitAttr.wSuitNum)),3,i);
		}
	}
	Editbox* GoodsTree::GetEditbox(const char *WindowName)
	{
		return static_cast<Editbox*>(WindowManager::getSingleton().getWindow(WindowName));
	}
	Combobox* GoodsTree::GetCombobox(const char *WindowName)
	{
		return static_cast<Combobox*>(WindowManager::getSingleton().getWindow(WindowName));
	}
}