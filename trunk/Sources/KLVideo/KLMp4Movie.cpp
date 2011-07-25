//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMp4Movie.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	MPEG-4 Movie Play Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KSystem.h"
#include "KKeyboard.h"
#include "KMp4Movie.h"
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��AVI�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KMp4Movie::Open(LPSTR FileName)
{
	// ����ƵAVI�ļ�
	m_Audio.Open(FileName);
	// ����ƵAVI�ļ�
	m_Video.Open(FileName);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ر��ļ����ͷ���Դ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::Close()
{
	// �ر���Ƶ��
	m_Audio.Close();
	// �ر���Ƶ��
	m_Video.Close();
}
//---------------------------------------------------------------------------
// ����:	Play
// ����:	��ʼ����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::Play(BOOL bZoom)
{
	m_Audio.Play();
	Sleep(750); //MSDN˵��Ƶ����Ƶ��0.75��
	m_Video.Play(bZoom);
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::Stop()
{
	m_Audio.Stop();
	m_Video.Stop();
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::Seek(int nPercent)
{
	m_Audio.Seek(nPercent);
	m_Video.Seek(nPercent);
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::Rewind()
{
	m_Audio.Rewind();
	m_Video.Rewind();
}
//---------------------------------------------------------------------------
// ����:	SetPos
// ����:	����λ��
// ����:	nX, nY		����
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::SetPos(int nX, int nY)
{
	m_Video.SetPos(nX, nY);
}
//---------------------------------------------------------------------------
// ����:	WaitForEnd
// ����:	�ȴ�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp4Movie::WaitForEnd()
{
	MSG	 Msg;

	while (TRUE)
	{
		// �ȴ���Ϣ���ͷ�CPU��Դ
		WaitMessage();

		// ��;�رճ���
		if (!GetMessage(&Msg, NULL, 0, 0))
		{
			PostQuitMessage(0);
			break;
		}

		// ESCAPE����������
		g_Keyboard.UpdateState();
		if (g_Keyboard.IsDown(DIK_ESCAPE))
		{
			Close(); // ��������߳�����Ѿ��ͷŵ��ڴ�
			break;
		}

		// �������ͨ����Ϣ
		if (Msg.message == WM_MP4MOVIE_END)
			break;
		
		// �������ͨ��û���յ�ʱ�Ĳ��ȴ���
		if (!m_Video.IsPlaying())
			break;
		
		DispatchMessage(&Msg);
	}
}
//---------------------------------------------------------------------------