/*

            �������
			 ��                                      ��
			����    ��� ��     ���    ����  ����
		   ��  ��      ���    ��    ��  ��        ��
		  ��          ��      ��    ��     ��     ��
	  ����      ����       ���    ����    ���

			"FROST" 3D GRAPHICS RENDERING ENGINE.

					 WRITTEN BY JIATAO.
--------------------------------------------------------------------
					COPYRIGHT 2002-2003. 

����: ��ģ��
�޸�: 2003/10/31
*/
#pragma once

#include "frender.h"
#include "fsingleton.h"


class engine
{
public:
	//�����ļ�
	class Manager
	{
		DECLARE_SINGLETON(Manager);

		BOOL m_bWndActive;									//���ڼ����־
		//���ھ������
		HWND m_hWnd;		

		//����
		unsigned __int64 m_qwUpdateFrame;
		unsigned __int64 m_qwRenderFrame;

		//frustum of current display frame 
		//render::Frustum * m_pFrustum;

		Manager(void);
		~Manager(void);

	public:
		//__��ʼ��__
		BOOL  Create(HWND hWnd);										//
		void  Destroy();

		//��Ϣ������
		BOOL  WindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

		//ˢ�£���Ⱦ
		void Update(void);
		void Render(void);
		//������Ⱦ������
		void SetWnd(void * pWnd);
		//���õ�ǰ��Ⱦ�����
		void SetActiveCamera(render::Camera * pCamera);
	//	render::Frustum * GetFrustum() { return m_pFrustum;}
	};

};
