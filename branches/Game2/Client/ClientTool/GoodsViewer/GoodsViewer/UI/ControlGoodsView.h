/**
* @filename:ControlGoodsView.h
* @date: 2010/1/27
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ����ģ�Ͳ鿴��UI��
*/
#pragma  once
#include "../../CEGUIEngine/BaseUIClass.h"

namespace UIClass
{
	class ControlGoodsView:public BaseUIClass
	{
	public:
		ControlGoodsView();
		virtual ~ControlGoodsView();

		/// @brief
		/// ��д��ʼ�����麯���������Լ���CEGUI����
		/// @param
		/// 
		/// @return
		/// 
		bool Init();
	protected:
		/// @brief
		/// ����������ת
		/// @param e
		bool HandleRightRot(const CEGUI::EventArgs& e);
		/// @brief
		/// ����������ת
		bool HandleLeftRot(const CEGUI::EventArgs& e);

		/// @brief
		/// ��������ת
		/// @param
		/// 
		/// @return
		/// 
		bool HandleTopRot(const CEGUI::EventArgs& e);

		/// @brief
		/// ��������ת
		/// @param
		/// 
		/// @return
		/// 
		bool HandleBottomRot(const CEGUI::EventArgs& e);

		/// @brief
		/// ����Ŵ�
		/// @param
		/// 
		/// @return
		/// 
		bool HandleZoomIn(const CEGUI::EventArgs& e);

		/// @brief
		/// ������С
		/// @param
		/// 
		/// @return
		/// 
		bool HandleZoomOut(const CEGUI::EventArgs& e);

		/// @brief
		/// ����Slider
		/// @param
		/// 
		/// @return
		/// 
		bool HandlSlider(const CEGUI::EventArgs& e);
	private:
		CEGUI::Slider* m_Slider;///<ָ�����Ļ��鴰��ָ��
		static float s_SliderInitialValue;///layout�ļ���Slider���õĳ�ʼֵ
		static float s_SliderClickStepSize;///<layout�ļ���Slider��ClickStepSizeֵ
		static const float m_CameraMaxZPos;///<�������������ģ�͵����Zֵ����Ϊʹ�õ�����������ϵ�������Ǹ�ֵ
		static const float m_CameraMinZPos;///<�������������ģ�͵���СZֵ
		static const float m_CameraStepZ;///<����������ƶ��ĵ�λZֵ
	};
}