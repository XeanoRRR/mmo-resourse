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

����: ����ģ��
�޸�: 2003/10/31
*/
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "fsingleton.h"

enum INPUT_EVENT_TYPE{
		IET_NONE,
	//����
		IET_KB_KEYDOWN,//DATA IS DIK CODE
		IET_KB_KEYUP,  //DATA IS DIK CODE
		IET_KB_CHAR,   //DATA IS VK CHAR
	//���
		IET_MS_BTNDOWN,//DATA IS DOWN BUTTON MASK
		IET_MS_BTNUP,  //DATA IS DOWN BUTTON MASK
		IET_MS_MOVE,   //DATA IS MOUSE BUTTON MASK
		IET_MS_WHEEL,  //DATA IS SCROLL VALUE
};

#define MAX_EVENT_QUEUE_SIZE 32

class input
{
public:
	class Interface
	{
		DECLARE_SINGLETON(Interface);

		int m_iQueueCount;	
		//CRITICAL_SECTION m_csCriticalSectionEvent;

		//KEYBOARD
		IDirectInput8	 	* m_pDInput;
		IDirectInputDevice8 * m_pDInputKeyboardDevice;

		

		POINT m_ptPreMousePosition;							//��һ�����λ��
		POINT m_ptCurMousePosition;							//��ǰ���λ��
		DWORD m_dwMouseButtonStatus;						//���״̬

		HWND m_hWnd;										//���ھ��

		int m_iEventQueueFront;
		int m_iEventQueueRear;

		BYTE m_pPreKeyStateBuffer[256];
		BYTE m_pCurKeyStateBuffer[256];

	public:
		struct _tagEVENT									//�¼�
		{
			INPUT_EVENT_TYPE eType;
			DWORD dwData;
			WPARAM wParam;
			LPARAM lParam;
		};
	private:
		_tagEVENT m_eEventQueue[MAX_EVENT_QUEUE_SIZE];

		BOOL CreateDevice();
		void DestroyDevice();

		Interface(void);
		~Interface(void);

	public:

		void UpdateInput();									//ˢ������

		//
		BOOL ParseMsg(UINT uiMessage,WPARAM wParam,LPARAM lParam);

		void PushEvent(INPUT_EVENT_TYPE eType,DWORD dwData,WPARAM wParam,LPARAM lParam);
		BOOL PopEvent(_tagEVENT *pstEvent);

		//�ӿڣ�
		//ȡ�ü���״̬
		BOOL GetKeyState(BYTE byKey) const { return m_pCurKeyStateBuffer[byKey];}
		//ȡ�ü���״̬������[256]
		const BYTE * GetKeyStateBuffer() const { return m_pCurKeyStateBuffer;}
		//ȡ�����״̬
		DWORD GetMouseBtnStatus() const { return m_dwMouseButtonStatus;}
		//ȡ���������
		void  GetMousePosition(POINT *ptCur,POINT *ptPre);
		
		//��ʼ��
		BOOL Create(HWND hWnd);
		//����
		void Destroy();
	};
};