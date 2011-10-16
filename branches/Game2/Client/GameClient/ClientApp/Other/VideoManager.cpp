#include "stdafx.h"
#include "VideoManager.h"
#include "../../Engine/Render.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVideoManager * CVideoManager::s_pInstance = NULL;

/*
 * ����: ���캯��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
CVideoManager::CVideoManager()
{
	m_iPlayThreadCount = 0;
	m_iCurrentPlaying  = -1;
	m_bIsFullScreen	   = false;
	m_bIsUniquelyPlaying = true;

	m_pClipList = NULL;

	ZeroMemory(m_pVideosArray, sizeof(CVideo *) * NUM_VIDEO);
	ZeroMemory(m_uiVideosLoop, sizeof(int) * NUM_VIDEO);
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
CVideoManager::~CVideoManager()
{
	m_iPlayThreadCount = 0;
	m_iCurrentPlaying  = -1;

	if (m_pClipList)
	{
		free(m_pClipList);
		m_pClipList = NULL;
	}
}



/*
 * ����: �õ���̬����
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
CVideoManager * CVideoManager::GetInstance()
{
	if (!s_pInstance)
		s_pInstance = new CVideoManager();

    return s_pInstance;
}



/*
 * ����: �õ���ǰ���ڲ��ŵ���Ƶָ��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
RGNDATA * CVideoManager::GetClipList()
{
	if (m_iCurrentPlaying == -1)
		return NULL;

	if (m_pClipList)
	{
		free(m_pClipList);
		m_pClipList = NULL;
	}

	static int NumRect = 4;
	m_pClipList = (RGNDATA *)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT) * NumRect);

	RECT * pRC = m_pVideosArray[m_iCurrentPlaying]->GetSize();

	static RECT rcClipRect[4];
	static int iWidth = render::Interface::GetInstance()->GetWndWidth();
	static int iHeight= render::Interface::GetInstance()->GetWndHeight();

	rcClipRect[0].left = 0;
	rcClipRect[0].top = 0;
	rcClipRect[0].right = pRC->left;
	rcClipRect[0].bottom = iHeight;

	rcClipRect[1].left = pRC->left + pRC->right;
	rcClipRect[1].top = 0;
	rcClipRect[1].right = iWidth;
	rcClipRect[1].bottom = iHeight;

	rcClipRect[2].left = pRC->left;
	rcClipRect[2].top = 0;
	rcClipRect[2].right = pRC->left + pRC->right;
	rcClipRect[2].bottom = pRC->top;

	rcClipRect[3].left = pRC->left;
	rcClipRect[3].top = pRC->top + pRC->bottom;
	rcClipRect[3].right = pRC->left + pRC->right;
	rcClipRect[3].bottom = iHeight;

	memcpy(m_pClipList->Buffer, &rcClipRect, sizeof(RECT) * NumRect);
	m_pClipList->rdh.dwSize = sizeof(RGNDATAHEADER);
	m_pClipList->rdh.iType = RDH_RECTANGLES;
	m_pClipList->rdh.nCount = NumRect;
	m_pClipList->rdh.nRgnSize = sizeof(RECT) * NumRect;
	m_pClipList->rdh.rcBound.left = 0;
	m_pClipList->rdh.rcBound.top = 0;
	m_pClipList->rdh.rcBound.right = iWidth;
	m_pClipList->rdh.rcBound.bottom = iHeight;

	return m_pClipList;
}



/*
 * ����: �趨�Ƿ�Ϊȫ��Ļ
 * ժҪ: -
 * ����: bIsFullScreen - �Ƿ�Ϊȫ��Ļ
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
void CVideoManager::SetFullScreen(bool bIsFullScreen)
{
	if (m_iCurrentPlaying == -1 ||
		!render::Interface::GetInstance()->GetSetting()->bIsWindow)
		return;

	m_bIsFullScreen = bIsFullScreen;

	if (m_pVideosArray[m_iCurrentPlaying])
	{
		if (m_pVideosArray[m_iCurrentPlaying]->IsFullScreen() != m_bIsFullScreen)
			m_pVideosArray[m_iCurrentPlaying]->SwitchScreenMode();
	}
}



/*
 * ����: ��ʼ��
 * ժҪ: -
 * ����: hWnd - ������
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
bool CVideoManager::Init(HWND hWnd)
{
	if (CoInitialize(NULL) != S_OK)
		return false;

	m_hWnd = hWnd;

	return true;
}



/*
 * ����: ���ٺ���
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
void CVideoManager::Destroy()
{
	for (DWORD dw = 0; dw < NUM_VIDEO; ++dw)
		SAFE_DELETE(m_pVideosArray[dw]);
}



/*
 * ����: ������Ƶ����
 * ժҪ: -
 * ����: pszFileName - ��Ƶ�ļ�·����
 *		 uiLoop		 - ѭ������,-1��ʾ����ѭ��
 *		 prcSize	 - ��Ƶ�ߴ�,NULL��ʾ��������ͻ����ߴ�
 * ����ֵ: ������óɹ�,�򷵻ص�ǰ���ڲ��ŵ���Ƶ����
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 */
int CVideoManager::PlayVideo(const char * pszFileName, unsigned int uiLoop, const RECT * prcSize)
{
	if (m_bIsUniquelyPlaying)
	{
		if (m_iCurrentPlaying != -1)
		{
			SAFE_DELETE(m_pVideosArray[m_iCurrentPlaying]);
			m_uiVideosLoop[m_iCurrentPlaying] = 0;
			m_iCurrentPlaying = -1;
			--m_iPlayThreadCount;
		}

		CVideo * pVideo = NULL;
		for (DWORD dw = 0; dw < NUM_VIDEO; ++dw)
		{
			pVideo = m_pVideosArray[dw];
			if (!pVideo)
			{
				m_iCurrentPlaying = dw;
				break;
			}
		}

		if (m_iCurrentPlaying != -1)
		{
			m_pVideosArray[m_iCurrentPlaying] = new CVideo();
			if (!m_pVideosArray[m_iCurrentPlaying]->Load(g_hWnd, pszFileName))
			{
				SAFE_DELETE(m_pVideosArray[m_iCurrentPlaying]);
				m_iCurrentPlaying = -1;
				return -1;
			}

			SetFullScreen(m_bIsFullScreen);
			m_pVideosArray[m_iCurrentPlaying]->AdjustSize(prcSize);
			m_pVideosArray[m_iCurrentPlaying]->Play();

			if (uiLoop != -1)
				m_uiVideosLoop[m_iCurrentPlaying] = uiLoop - 1;
			else
				m_uiVideosLoop[m_iCurrentPlaying] = -1;

			++m_iPlayThreadCount;

			return m_iCurrentPlaying;
		}else
			return -1;
	}else
	{
		return -1;
	}
}



/*
 * ����: ֹͣ��Ƶ����
 * ժҪ: -
 * ����: iVideoIndex - ��Ҫֹͣ����Ƶ����(��0��ʼ,���Ϊ-1���ʾ,��ǰ���ŵ���Ƶ)
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.18
 * �޸���־:
 */
void CVideoManager::StopVideo(int iVideoIndex)
{
	if (iVideoIndex < -1 || iVideoIndex >= NUM_VIDEO)
		return;

	if (iVideoIndex == -1)
	{
		if (m_iCurrentPlaying != -1)
		{
			SAFE_DELETE(m_pVideosArray[m_iCurrentPlaying]);
			m_uiVideosLoop[m_iCurrentPlaying] = 0;
			m_iCurrentPlaying = -1;
			--m_iPlayThreadCount;
		}else
			return;
	}else
	{
		if (iVideoIndex == m_iCurrentPlaying)
		{
			SAFE_DELETE(m_pVideosArray[m_iCurrentPlaying]);
			m_uiVideosLoop[m_iCurrentPlaying] = 0;
			m_iCurrentPlaying = -1;
			--m_iPlayThreadCount;
		}else
		{
			SAFE_DELETE(m_pVideosArray[iVideoIndex]);
			m_uiVideosLoop[iVideoIndex] = 0;
		}
	}
}



/*
 * ����: ��������������
 * ժҪ: ������ϵ���Ƶ����һ���ͷſռ�,����ѭ�����д���
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.08.14
 * �޸���־:
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 *	2009.08.25 - lpf
 *		�޸�������ʱ��,һ������stop��,������Ч��,����ȥͨ��GetState()�����ж����Ƿ�ֹͣ,��Ϊ��Щ�����޷��õ���ȷ״̬;
 */
void CVideoManager::GarbageColloction()
{
	CVideo * pVideo = NULL;
	for (DWORD dw = 0; dw < NUM_VIDEO; ++dw)
	{
		pVideo = m_pVideosArray[dw];

		if (pVideo)
		{
			if (pVideo->IsComplete())
			{
				if (m_uiVideosLoop[dw] == -1)
				{
					pVideo->SetPos(0);
				}else if (m_uiVideosLoop[dw])
				{
					pVideo->SetPos(0);
					--m_uiVideosLoop[dw];
				}else
				{
					pVideo->Stop();
					SAFE_DELETE(m_pVideosArray[dw]);
					m_uiVideosLoop[dw] = 0;
					m_iCurrentPlaying = -1;
					--m_iPlayThreadCount;
				}
			}
		}
	}
}