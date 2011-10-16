/*
 * Video.h
 * ����: ��Ƶ���Ŷ����ࡣ
 * ժҪ: ����DShowģ��
 *		 Ŀǰ֧���ļ���ʽ : *.WMV, *.ASF, *.MPEG, *.AVI, QickTime
 * ����: lpf
 * ��������: 2008-08-07
 */

#pragma once

#include <dshow.h>

class CVideo
{
	HWND m_hWnd;
	RECT m_rcSize;

	IGraphBuilder * m_pGB;
	IMediaControl * m_pMC;
	IVideoWindow  * m_pVW;
	IMediaSeeking * m_pMS;

public:
	CVideo();			//���캯��
	~CVideo();			//��������

	// �趨���Ա����
	bool IsFullScreen();
	bool IsComplete();
	bool SetPos(LONGLONG llPos);
	bool SetVisible(bool bIsVisible);

	RECT *		 GetSize()			{ return &m_rcSize; }
	FILTER_STATE GetState();					//�õ���ǰý��״̬

	// ��������
	bool Load(HWND hWnd, const char * pszFileName);
	bool AdjustSize(const RECT * prcSize = NULL);
	bool SwitchScreenMode();
	bool Play();
	bool Pasue();
	bool Stop();
};