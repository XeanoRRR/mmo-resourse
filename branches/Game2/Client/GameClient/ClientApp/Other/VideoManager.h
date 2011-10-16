/*
 * VideoManager.h
 * ����: ��Ƶ���Ŷ����ࡣ
 * ժҪ: ����DShowģ��
 *		 Ŀǰ֧���ļ���ʽ : *.WMV, *.ASF, *.MPEG, *.AVI, QickTime
 * ����: lpf
 * ��������: 2008-08-14
 */

#pragma once
#include "Video.h"

#define NUM_VIDEO 5

class CVideoManager
{
	HWND m_hWnd;
	int m_iPlayThreadCount;					//�����̼߳�����
	int m_iCurrentPlaying;					//��ǰ���ŵ���Ƶ����(-1��ʾ��ǰû����Ƶ����)
	bool m_bIsFullScreen;					//��ǰ��Ƶ�Ƿ�ȫ������
	bool m_bIsUniquelyPlaying;

	RGNDATA * m_pClipList;

	CVideo * m_pVideosArray[NUM_VIDEO];		//��Ƶ�б�
	unsigned int m_uiVideosLoop[NUM_VIDEO];	//��Ƶѭ������

	static CVideoManager * s_pInstance;
	CVideoManager();		//���캯��

public:
	~CVideoManager();		//��������

	static CVideoManager * GetInstance();
	static void			   ReleaseInstance()		{ SAFE_DELETE(s_pInstance); }

	int GetPlayThreadCount()						{ return m_iPlayThreadCount; }
	bool IsFullScreen()								{ return m_bIsFullScreen; }
	RGNDATA * GetClipList();

	bool Init(HWND hWnd);
	void Destroy();
	void GarbageColloction();

	int	 PlayVideo(const char * pszFileName, unsigned int iLoop = 1, const RECT * rcSize = NULL);		//������Ƶ
	void StopVideo(int iVideoIndex = -1);																//ֹͣ��Ƶ
	void SetFullScreen(bool bIsFullScreen);																//�趨����Ƶ�Ƿ�ȫ��
};