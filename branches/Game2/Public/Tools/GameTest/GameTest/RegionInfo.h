//=============================================================================
/**
 *  file: RegionInfo.h
 *
 *  Brief:��ʾ������Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================


#pragma once
#include "afxwin.h"
#include "resource.h"
//#include "../App/MoveShape.h"
//#include "../App/GameClient.h"

class CRegion ;
class CClientRegion;
class CPlayer;
class CMoveShape;
class CGameClient;
// CRegionInfo �Ի���

class CRegionInfo : public CDialog
{
	DECLARE_DYNAMIC(CRegionInfo)

public:
	CRegionInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegionInfo();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_REGION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	//��ǰ��ʾ�ĳ���
	CClientRegion*	m_pDisplayRegion;
	long m_lDrawingRegionID;					//���ڻ��Ƶĳ���id
	long m_lClientID;							//��ǰ��ʾ�ͻ��˵�ID
	CRect			m_rcClient;
	CRect			m_rcRegion;
	//���ڴ������ĵĳ���λ��
	CPoint			m_ptCenter;
	//�ڹ��캯���н��� ����DC�����bitmap����
	CDC	*m_DisplayMemDC;		//��ʾ����
	// ����λͼ
	CBitmap	*m_Bitmap;

public:
	BOOL ReadRgnData();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void DrawCell(CDC &dcMem);
	void DrawShape(CDC &dcMemory);
	void SetDisplayRegion(CGameClient* pClient);
	void SetRegioBitmapData();
	// ��ǰ����
	float m_PlayerX;			//mainPlayer ����
	float m_PlayerY;

	float m_RegionID;			//����ID

	void SetDisplayCoordinate(long x,long y);
	void SetPlayerRegion(CPlayer* pPlayer);
	CPlayer* mMainPlayer;
};
