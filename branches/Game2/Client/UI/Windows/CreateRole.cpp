/**
* @filename:CreateRole.cpp
* @date: 2010/7/12
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ��ɫ��������
*/

/////////////////////////////////////////////////
// zhaohang  2010/7/12 
// �ָ���ɫ���Ա����ͣ����ͣ���ɫѡ���ܣ�����������UI/CreateRoleĿ¼����
////////////////////////////////////////////////
#include "stdafx.h"
#include "CreateRoleDef.h"
#include "../UIDef.h"
#include "../GamePage/CreateRole/CreateRoleEvent.h"
#include "../../GameClient/ClientApp/Other/CountryList.h"
///<��ֵ�����ʼ��״̬�����Ժ��¼�ע��
void SetCreateRoleInitProperty(CEGUI::Window* pgWnd);

///< ���ұ��
bool OnCountryChanged(const CEGUI::EventArgs& e);
///< ��ɫ�Ա���
bool OnSexChanged(const CEGUI::EventArgs& e);
///< ���ͱ��
bool OnFacChanged(const CEGUI::EventArgs& e);
///< ���ͱ��
bool OnHairChanged(const CEGUI::EventArgs& e);
///< ��ɫ���
bool OnHairColorChanged(const CEGUI::EventArgs& e);
///< ����ѡ��Ľ�ɫ������������ͣ����ͣ���ѡUI����
void ResetDataBySexSelChanged();

///< ����ѡ��ķ��ͣ�������䷢ɫ��ѡUI����
void ResetHairColorDateOnHairChanged();

void SetCreateRoleInitProperty(CEGUI::Window* pgWnd)
{
	if(!pgWnd)
		return;
	CEGUI::Combobox* SelCountry = WComboBox(pgWnd->getChildRecursive(CREATEROLE_SEL_COUNTRY_CCB));
	if(SelCountry)
	{
		SelCountry->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnCountryChanged));
		SelCountry->setReadOnly(true);
		CCountryList::MapCountryList* mapCL = CCountryList::GetCountryList();
		for(uint cnt = 0; cnt < mapCL->size(); ++cnt)
		{
			const char* ctName = CCountryList::GetCountryName((BYTE)cnt+1);
			if(ctName)
			{
				CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(ToCEGUIString(ctName));
				lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
				lti->setID(cnt+1);//ID�͹���ID����
				if(cnt == 0)
					lti->setSelected(true);//���ù���Ĭ��ֵ
				SelCountry->addItem(lti);
			}
		}
		//���ݹ���Ĭ��ֵ��������ʾ�ı�
		SelCountry->getEditbox()->setText(ToCEGUIString(CCountryList::GetCountryName(0+1)));//��һ����Ϊdata/CountryList.xml�������
		/***********************************************************************/
		/* zhaohang fix 2010-9-3
		/***********************************************************************/
		CREvent::SetSelectCountry(1);//�߼��ϵĹ���Ĭ��ֵ
	}
	CEGUI::Combobox* selHair = WComboBox(pgWnd->getChildRecursive(CREATEROLE_SEL_HAIR_CCB));
	if(selHair)
	{
		selHair->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnHairChanged));
		selHair->setReadOnly(true);
	}
	CEGUI::Combobox* selHairColor = WComboBox(pgWnd->getChildRecursive(CREATEROLE_SEL_HAIRCOLOR_CCB));
	if(selHairColor)
	{
		selHairColor->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnHairColorChanged));
		selHairColor->setReadOnly(true);
	}
	CEGUI::Combobox* selFac = WComboBox(pgWnd->getChildRecursive(CREATEROLE_SEL_FAC_CCB));
	if(selFac)
	{
		selFac->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnFacChanged));
		selFac->setReadOnly(true);
	}
	CEGUI::Combobox* selSex = WComboBox(pgWnd->getChildRecursive(CREATEROLE_SEL_SEX_CCB));
	if(selSex)
	{
		CEGUI::ListboxTextItem* itm1  = new CEGUI::ListboxTextItem(ToCEGUIString(CREATEROLE_SEX_MALE));
		itm1->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		itm1->setID(0);//��ID���Ա����
		selSex->addItem(itm1);
		CEGUI::ListboxTextItem* itm2 = new CEGUI::ListboxTextItem(ToCEGUIString(CREATEROLE_SEX_FAMALE));
		itm2->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
		itm2->setID(1);//��ID���Ա����
		selSex->addItem(itm2);
		//ע���¼�
		selSex->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::SubscriberSlot(OnSexChanged));
		selSex->setReadOnly(true);
	}
}

void ResetDataBySexSelChanged()
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Combobox* SelFac = WComboBox(mgr.getWindow(CREATEROLE_SEL_FAC_CCB));
	uint SelSex = (uint)CREvent::GetSelectSex();
	if(SelFac)
	{
		SelFac->resetList();
		//�����Ա���޸�Ĭ��Face��HairStyleʱ,��Ӧ�޸��ı���ʾ
		SelFac->getEditbox()->setText(CEGUI::PropertyHelper::intToString(0));
		for(short i = 0 ; i < CREvent::GetFaceNum(SelSex) ; ++i)
		{
			CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(i));
			lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			lti->setID(i);//ID��FacIndex����
			if(0==i)		//�����Ա���޸�Ĭ��Face��HairStyleʱ,��Ӧ�޸�ItemListѡ��״̬
				lti->setSelected(true);
			SelFac->addItem(lti);
		}
	}
	CEGUI::Combobox* SelHair = WComboBox(mgr.getWindow(CREATEROLE_SEL_HAIR_CCB));
	if(SelHair)
	{
		SelHair->resetList();
		//�����Ա���޸�Ĭ��Face��HairStyleʱ,��Ӧ�޸��ı���ʾ
		SelHair->getEditbox()->setText(CEGUI::PropertyHelper::intToString(0));
		for(short i = 0 ; i < CREvent::GetHairNum(SelSex) ; ++i)
		{
			CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(i));
			lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			lti->setID(i);// ID��HairIndex����
			if(0 == i)//�����Ա���޸�Ĭ��Face��HairStyleʱ,��Ӧ�޸�ItemListѡ��״̬
				lti->setSelected(true);
			SelHair->addItem(lti);
		}
	}
}

void ResetHairColorDateOnHairChanged()
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Combobox* hairColor = WComboBox(mgr.getWindow(CREATEROLE_SEL_HAIRCOLOR_CCB));
	if(hairColor)
	{
		hairColor->resetList();
		//��շ�ɫ�����ı�
		hairColor->getEditbox()->setText("");
		for(short i = 0 ; i < CREvent::GetHairColorNum(CREvent::GetSelectSex(),(WORD)CREvent::GetHair()) ; ++i)
		{
			CEGUI::ListboxTextItem* lti = new CEGUI::ListboxTextItem(CEGUI::PropertyHelper::intToString(i));
			lti->setSelectionBrushImage(IMAGES_FILE_NAME,BRUSH_NAME);
			lti->setID(i);//ID��HairColorIndex����
			hairColor->addItem(lti);
		}
	}
}

bool OnSexChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbb = WComboBox(WEArgs(e).window);
	CEGUI::ListboxItem* lti = cbb->getSelectedItem();
	if(lti)
	{
		CREvent::SetSelectSex(lti->getID());
		//�����Ա���޸�Ĭ��Face��HairStyle,ʹ��ģ���ܹ�������ʾ
		CREvent::SetFace(0);
		CREvent::SetHairStyle(0);
	}
	else
	{
		CREvent::SetSelectSex(0);
		//�����Ա���޸�Ĭ��Face��HairStyle,ʹ��ģ���ܹ�������ʾ
		CREvent::SetFace(0);
		CREvent::SetHairStyle(0);
	}
	ResetDataBySexSelChanged();
	return true;
}

bool OnFacChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbb = WComboBox(WEArgs(e).window);
	CEGUI::ListboxItem* lti = cbb->getSelectedItem();
	if(lti)
		CREvent::SetFace(lti->getID());
	else
		CREvent::SetFace(0);
	return true;
}

bool OnHairChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbb = WComboBox(WEArgs(e).window);
	CEGUI::ListboxItem* lti = cbb->getSelectedItem();
	if(lti)
		CREvent::SetHairStyle(lti->getID());
	else
		CREvent::SetHairStyle(0);
	ResetHairColorDateOnHairChanged();
	return true;
}
bool OnHairColorChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbb = WComboBox(WEArgs(e).window);
	CEGUI::ListboxItem* lti = cbb->getSelectedItem();
	if(lti)
		CREvent::SetHairColor(lti->getID());
	else
		CREvent::SetHairColor(0);
	return true;
}

bool OnCountryChanged(const CEGUI::EventArgs& e)
{
	CEGUI::Combobox* cbb = WComboBox(WEArgs(e).window);
	CEGUI::ListboxItem* lti = cbb->getSelectedItem();
	if(lti)
		CREvent::SetSelectCountry(lti->getID());
	else
		CREvent::SetSelectCountry(1);//range��Data/CountryList.xml ���þ���,���������������Ĭ�Ϲ���IDΪ1
	return true;
}