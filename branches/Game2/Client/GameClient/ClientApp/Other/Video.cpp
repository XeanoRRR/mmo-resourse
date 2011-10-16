#include "stdafx.h"
#include "Video.h"
#include <uuids.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * ����: ���캯��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
CVideo::CVideo()
{
	m_hWnd = NULL;
	ZeroMemory(&m_rcSize, sizeof(RECT));

	m_pGB = NULL;
	m_pMC = NULL;
	m_pVW = NULL;
	m_pMS = NULL;
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
CVideo::~CVideo()
{
	if (m_pMC)
		m_pMC->Stop();

	m_hWnd = NULL;
	ZeroMemory(&m_rcSize, sizeof(RECT));

	SAFE_RELEASE(m_pMS);
	SAFE_RELEASE(m_pVW);
	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pGB);
}



/*
 * ����: �Ƿ�Ϊȫ��Ļ����
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 */
bool CVideo::IsFullScreen()
{
	if (m_pVW != NULL)
	{
		long lMode = 0;
		m_pVW->get_FullScreenMode(&lMode);
		return lMode == OATRUE ? true : false;
	}else
		return false;
}



/*
 * ����: �Ƿ񲥷����
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 */
bool CVideo::IsComplete()
{
	if (m_pMS == NULL)
		return false;

	LONGLONG llCurrent = 0;
	LONGLONG llLength = 0;
	m_pMS->GetCurrentPosition(&llCurrent);
	m_pMS->GetDuration(&llLength);
	if (llCurrent >= llLength)
		return true;
	else
		return false;
}



/*
 * ����: �趨��ǰ����λ��
 * ժҪ: -
 * ����: llPos - ָ����ǰ����λ��
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::SetPos(LONGLONG llPos)
{
	if (m_pMS == NULL)
		return false;

	m_pMS->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	return true;
}



/*
 * ����: �趨��Ƶ�Ƿ�ɼ�
 * ժҪ: -
 * ����: bIsVisible - �Ƿ�ɼ�
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::SetVisible(bool bIsVisible)
{
	if (m_pMS == NULL)
		return false;

	m_pVW->put_Visible(bIsVisible ? OATRUE : OAFALSE);
	return true;
}



/*
 * ����: �õ���ǰý��״̬
 * ժҪ: -
 * ����: bIsVisible - �Ƿ�ɼ�
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 */
FILTER_STATE CVideo::GetState()
{
	if (m_pMC != NULL)
	{
		OAFilterState fs = 0;
		m_pMC->GetState(INFINITE, &fs);
		return (FILTER_STATE)fs;
	}

	return State_Stopped;
}



/*
 * ����: ������Ƶ�ļ�
 * ժҪ: -
 * ����: pszFileName - ��Ƶ�ļ���
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::Load(HWND hWnd, const char * pszFileName)
{
	WCHAR wFileName[MAX_PATH] = { 0 };

	if (FAILED(CoCreateInstance(CLSID_FilterGraph,
								NULL,
					 			CLSCTX_INPROC_SERVER, 
					 			IID_IGraphBuilder,
					 			(void **)&m_pGB)))
		return false;
       
	if (FAILED(m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC)))
		return false;

	if (FAILED(m_pGB->QueryInterface(IID_IVideoWindow, (void **)&m_pVW)))
		return false;

	if (FAILED(m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS)))
		return false;

	MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wFileName, MAX_PATH);
	if (FAILED(m_pGB->RenderFile(wFileName, NULL)))
		return false;

	m_hWnd = hWnd;

	m_pVW->put_Owner((OAHWND)hWnd);
	m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	return true;
}



/*
 * ����: ������Ƶ�ߴ�͸�����ͻ���һ��
 * ժҪ: -
 * ����: prcSize - �ߴ����
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::AdjustSize(const RECT * prcSize)
{
	if (m_pVW == NULL)
		return false;

	if (prcSize == NULL)
		GetClientRect(m_hWnd, &m_rcSize);
	else
		m_rcSize = *prcSize;

	m_pVW->SetWindowPosition(m_rcSize.left, m_rcSize.top, m_rcSize.right, m_rcSize.bottom);

	return true;
}



/*
 * ����: �л�����ȫ��ģʽ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::SwitchScreenMode()
{
	if (m_pVW == NULL)
		return false;

    static HWND hDrain = NULL;

	if (IsFullScreen())
	{
		m_pVW->put_FullScreenMode(OAFALSE);
		m_pVW->put_MessageDrain((OAHWND)hDrain);
	}else
	{
		m_pVW->get_MessageDrain((OAHWND *)&hDrain);
		m_pVW->put_MessageDrain((OAHWND)m_hWnd);
		m_pVW->put_FullScreenMode(OATRUE);
	}

	return true;
}



/*
 * ����: ��ʼ������Ƶ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::Play()
{
	if (m_pMC == NULL)
		return false;

	if (FAILED(m_pMC->Run()))
		return false;
	else
		return true;
}



/*
 * ����: ��ͣ������Ƶ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::Pasue()
{
	if (m_pMC == NULL)
		return false;

	if (FAILED(m_pMC->Pause()))
		return false;
	else
		return true;
}



/*
 * ����: ֹͣ������Ƶ
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.07
 * �޸���־:
 */
bool CVideo::Stop()
{
	if (m_pMC == NULL)
		return false;

	if (FAILED(m_pMC->Stop()))
		return false;
	else
		return true;
}