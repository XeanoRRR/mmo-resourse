#include "stdafx.h"
#include "ControlGoodsView.h"
#include "../GoodsModelManager.h"
#include "../../CEGUIEngine/CEGUIManager.h"
#include <CEGUIPropertyHelper.h>

namespace UIClass
{
	const float ControlGoodsView::m_CameraMaxZPos = -5.0f;
	const float ControlGoodsView::m_CameraMinZPos = 0.0f;
	const float ControlGoodsView::m_CameraStepZ = 0.4f;
	float ControlGoodsView::s_SliderInitialValue = 0.5f;
	float ControlGoodsView::s_SliderClickStepSize = 0.05f;
	ControlGoodsView::ControlGoodsView()
	{

	}

	ControlGoodsView::~ControlGoodsView()
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(m_RootWindow);
	}


	bool ControlGoodsView::Init()
	{
		WindowManager* WndMgr = &WindowManager::getSingleton();
		m_RootWindow = WndMgr->loadWindowLayout("GoodsViewerModelControl.layout");
		//����Ϊ�ֶ�����
		m_RootWindow->setDestroyedByParent(false);
		//���ص�CEGUImanager�����ĵװ棨�������ϣ�
		CEGUIManager::GetInstance()->GetRootWindow()->addChildWindow(m_RootWindow);
		//ע���¼�������
		WndMgr->getWindow("GoodModeCtr/RightRot")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleRightRot,this));
		WndMgr->getWindow("GoodModeCtr/LeftRot")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleLeftRot,this));
		WndMgr->getWindow("GoodModeCtr/FrontRot")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleTopRot,this));
		WndMgr->getWindow("GoodModeCtr/BackRot")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleBottomRot,this));
		//WndMgr->getWindow("GoodModeCtr/Scal+")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleZoomIn,this));
		//WndMgr->getWindow("GoodModeCtr/Scal-")->subscribeEvent(PushButton::EventClicked,Event::Subscriber(&ControlGoodsView::HandleZoomOut,this));
		WndMgr->getWindow("GoodModeCtr/SclaSlider")->subscribeEvent(Slider::EventValueChanged,Event::Subscriber(&ControlGoodsView::HandlSlider,this));

		//ָ�򻬿鴰��
		m_Slider = static_cast<Slider*>(WndMgr->getWindow("GoodModeCtr/SclaSlider"));
		return true;
	}
	bool ControlGoodsView::HandleRightRot(const CEGUI::EventArgs& e)
	{
		GoodsModelManager::GetInstance()->RotationY(m_CameraStepZ);
		return true;
	}

	bool ControlGoodsView::HandleLeftRot(const CEGUI::EventArgs& e)
	{
		GoodsModelManager::GetInstance()->RotationY(-m_CameraStepZ);
		return true;
	}

	bool ControlGoodsView::HandleTopRot(const CEGUI::EventArgs& e)
	{
		return true;
	}

	bool ControlGoodsView::HandleBottomRot(const CEGUI::EventArgs& e)
	{
		return true;
	}

	//����Ŵ�
	bool ControlGoodsView::HandleZoomIn(const CEGUI::EventArgs& e)
	{
		GoodsModelManager* GoodsMgr = GoodsModelManager::GetInstance();
		D3DXVECTOR3 CameraPos = GoodsMgr->GetCameraPosition();
		if(CameraPos.z < m_CameraMinZPos )
		{
			GoodsMgr->MoveCameraRel(0.0f,0.0f,m_CameraStepZ);
			Logger::getSingleton().logEvent("Current Camera Distance Z:"+PropertyHelper::floatToString(CameraPos.z));
			//������
			String CurVal = PropertyHelper::floatToString(m_Slider->getCurrentValue()+s_SliderClickStepSize);
			m_Slider->setProperty("CurrentValue",CurVal);
			Logger::getSingleton().logEvent("Current Slider Value:"+CurVal);
		}
		return true;
	}

	//������С
	bool ControlGoodsView::HandleZoomOut(const CEGUI::EventArgs& e)
	{
		GoodsModelManager* GoodsMgr = GoodsModelManager::GetInstance();
		D3DXVECTOR3 CameraPos = GoodsMgr->GetCameraPosition();
		if(CameraPos.z > m_CameraMaxZPos)
		{
			GoodsMgr->MoveCameraRel(0.0f,0.0,-m_CameraStepZ);
			Logger::getSingleton().logEvent("Current Camera Distance Z:"+PropertyHelper::floatToString(CameraPos.z));
			//������
			String CurVal = PropertyHelper::floatToString(m_Slider->getCurrentValue()-s_SliderClickStepSize);
			m_Slider->setProperty("CurrentValue",CurVal);
			Logger::getSingleton().logEvent("Current Slider Value:" + CurVal);
		}
		return true;
	}

	//����Slider
	bool ControlGoodsView::HandlSlider(const CEGUI::EventArgs& e)
	{
		Slider* slider = static_cast<Slider*>(static_cast<const WindowEventArgs&>(e).window);
		if(slider)
		{
			GoodsModelManager* GoodsMgr = GoodsModelManager::GetInstance();
			GoodsMgr->MoveCameraRel(0.0f,0.0f,(slider->getCurrentValue() - s_SliderInitialValue) * 5);
			s_SliderInitialValue = slider->getCurrentValue();
			//��־
			Logger::getSingleton().logEvent("Current Camera Distance Z:" + PropertyHelper::floatToString(GoodsMgr->GetCameraPosition().z));
			Logger::getSingleton().logEvent("Current Slider Value:" + PropertyHelper::floatToString(slider->getCurrentValue()));

			//����������ֵʱ�����������ģ��Zλ��
			if(s_SliderInitialValue == slider->getMaxValue())
			{
				Logger::getSingleton().logEvent("When Slider Value is max ,The Camera Distance Z:" + PropertyHelper::floatToString(GoodsMgr->GetCameraPosition().z));
			}
			if(s_SliderInitialValue == 0)
			{
				Logger::getSingleton().logEvent("When Slider Value is min,The Camera Distance Z:" + PropertyHelper::floatToString(GoodsMgr->GetCameraPosition().z));
			}
		}
		return true;
	}
}
