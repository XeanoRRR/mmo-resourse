//=============================================================================
/**
 *  file: RegionInfo.cpp
 *
 *  Brief:��ʾ������Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================

#include "stdafx.h"
#include "GameTest.h"
#include "GameTestDlg.h"
#include "RegionInfo.h"
#include "WndMsgDef.h"

#include "../Script/ScriptSys.h"

#include "../App/ClientRegion.h"
#include "../App/Shape.h"
#include ".\regioninfo.h"
#include "../App/MoveShape.h"
#include "../App/Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRegionInfo, CDialog)
CRegionInfo::CRegionInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CRegionInfo::IDD, pParent)
	, m_RegionID(0)
{
	m_DisplayMemDC = new CDC;//��ʼ��������ʾ��DC
	m_Bitmap = new CBitmap;//��ʼ��CBITMAP,���Ҫ���ص�ͼƬ��Ϣ
	m_pDisplayRegion = NULL;
	mMainPlayer = NULL;
}

CRegionInfo::~CRegionInfo()
{
	SAFE_DELETE(m_DisplayMemDC);
	SAFE_DELETE(m_Bitmap);
}

void CRegionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_STATIC_PLAYER_X, m_PlayerX);
//	DDX_Text(pDX, IDC_STATIC_PLAYER_Y, m_PlayerY);
//	DDX_Text(pDX, IDC_STATIC_RegionID, m_RegionID);
}

BEGIN_MESSAGE_MAP(CRegionInfo, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CRegionInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rs;
	GetClientRect(rs);
	m_rcClient = rs;
	// ����������ʾDC
	if(!m_DisplayMemDC->CreateCompatibleDC(NULL))
	{
		AfxMessageBox("����������ʾDC����!");
		return FALSE;
	}
	CDC *dc = GetDC();
	// ��������Bitmap
	if(!m_Bitmap->CreateCompatibleBitmap(dc,512,512))
	{
		AfxMessageBox("����Bitmap����!");
		return FALSE;
	}

	if (!m_DisplayMemDC->SelectObject(m_Bitmap))
	{
		return FALSE;
	}
	ReleaseDC(dc);
	return TRUE;  
}
void CRegionInfo::SetDisplayRegion(CGameClient* pClient)
{
	if ( pClient == NULL) return;
	if(m_pDisplayRegion != pClient->GetClientRegion() )
	{
		m_pDisplayRegion = pClient->GetClientRegion();
		m_lClientID = pClient->GetSocketID();
		m_lDrawingRegionID = m_pDisplayRegion->GetResourceID();
		SetRegioBitmapData();
	}
}

//��Rgn���Ӷ���
void CRegionInfo::SetRegioBitmapData()
{
	if ( m_DisplayMemDC )
	{
		int lRegionWidth = m_pDisplayRegion->GetWidth();
		int lRegionHeight = m_pDisplayRegion->GetHeight();
		//������rgn�����ݶ����ڴ���
		m_rcRegion = CRect(0,0,lRegionWidth,lRegionHeight);
		m_DisplayMemDC->SelectStockObject(BLACK_BRUSH);
		m_DisplayMemDC->SelectStockObject(BLACK_PEN);
		m_DisplayMemDC->Rectangle(&m_rcRegion);  
		CRegion::tagCell *pCell = m_pDisplayRegion->GetCellData();
		for(long row_y =0; row_y < lRegionHeight; ++row_y)				//��
		{
			for(long row_x =0; row_x < lRegionWidth; ++row_x)			//��
			{
				// �����
				if(pCell[row_x].lSwitch > 0)
				{
					m_DisplayMemDC->SetPixel(row_x,row_y, RGB(250, 10, 200));
				}

				else 
				{
					//				pCell[row_x+1].bBlock == pCell[row_x].bBlock)
					BYTE block = m_pDisplayRegion->GetBlock(row_x,row_y);
					CPen pen;
					switch(block)
					{
					case CRegion::BLOCK_NO:
						m_DisplayMemDC->SetPixel(row_x,row_y, RGB(230, 170, 150));
						break;
						break;
					case CRegion::BLOCK_UNFLY:
						m_DisplayMemDC->SetPixel(row_x,row_y, RGB(40,125, 10));
						break;
					case CRegion::BLOCK_SHAPE:
						m_DisplayMemDC->SetPixel(row_x,row_y, RGB(255, 0, 0));
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

//������---��ң����NPC ��
void CRegionInfo::DrawShape(CDC &dcMemory)
{
	CRect rect;
	GetClientRect(rect);
	//�����
	CPlayer* pMainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(m_lClientID)->GetMainPlayer();
	//���λ��
	long lPX = pMainPlayer->GetPosX();
	long lPY = pMainPlayer->GetPosY();
	//�����Ϊ����,��Χ128*128������
	//��Χ: 
	//x����� 
	long lStartX = lPX - 128;
	//�յ�
	long lEndX = lPX + 128;
	//Y����� 
	long lStartY = lPY - 128;
	//�յ�
	long lEndY = lPY + 128;

	list<CBaseObject*>* pObjectList = m_pDisplayRegion->GetObjectList();
	list<CBaseObject*>::iterator it = pObjectList->begin();
	long lX(0),lY(0);
	long lType(0);
	for(;it != pObjectList->end();it++)
	{
		CShape* pShape = (CShape*)(*it);
		if(pShape)
		{
			lX = pShape->GetPosX();
			lY = pShape->GetPosY();
			lType = pShape->GetType();
			CBrush brush;
			switch(lType)
			{
			case TYPE_PLAYER:
			    brush.CreateSolidBrush(RGB(10,10,250));
				dcMemory.SelectObject(&brush);
				dcMemory.Ellipse(lX-4,lY-4,lX+4,lY+4);
//				dcMemory.SetPixel(lPX,lPY, RGB(255,10,130));
				//rect.SetRect(lX-3,lY-3,lX+3,lY+3);
				//dcMemory.FillRect(rect,&brush);
				break;
			case TYPE_MONSTER:
				brush.CreateSolidBrush(RGB(255,0,0));
				dcMemory.SelectObject(&brush);
				dcMemory.Ellipse(lX-2,lY-2,lX+2,lY+2);
//				rect.SetRect(lX-2,lY-2,lX+2,lY+2);
//				dcMemory.FillRect(rect,&brush);
				break;
			case TYPE_GOODS:
				brush.CreateSolidBrush(RGB(235,235,0));
				dcMemory.SelectObject(&brush);
				dcMemory.Ellipse(lX-3,lY-3,lX+3,lY+3);
				//rect.SetRect(lX-1,lY-1,lX+1,lY+1);
				//dcMemory.FillRect(rect,&brush);
			    break;
			default:
			    break;
			}
//				SetDisplayCoordinate( lX, lY);						///      ������
//				dcMemory.Rectangle(lX-2,lY-2,lX+2,lY+2);
		}
	}
		
}

//����
void CRegionInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rClient;
	GetClientRect(rClient);

	CGameClient *pClient = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(m_lClientID);
	if ( !pClient )
	{
		SetDisplayRegion(NULL);
		return;
	}
	if ( pClient->GetClientRegion()->GetResourceID() != m_lDrawingRegionID )
	{
		m_pDisplayRegion = pClient->GetClientRegion();
		m_lDrawingRegionID = m_pDisplayRegion->GetResourceID();
	}

	OutputDebugString("����!\n");
	SetRegioBitmapData();
	DrawShape(*m_DisplayMemDC);

	dc.SetBkColor(RGB(255, 255,255));
	dc.StretchBlt(0,0,rClient.Width(),rClient.Height(),m_DisplayMemDC,0,0,m_rcRegion.Width(),m_rcRegion.Height(),SRCCOPY);

	ReleaseDC(&dc);
}

//��ʾ����
void CRegionInfo::SetDisplayCoordinate(long x,long y)
{
	UpdateData(TRUE);
	SetDlgItemInt(IDC_STATIC_PLAYER_X,x,1);
	SetDlgItemInt(IDC_STATIC_PLAYER_Y,y,1);
	
}
//��ʾrgn ID
void CRegionInfo::SetPlayerRegion(CPlayer* pPlayer)
{
	if(mMainPlayer != pPlayer)
	{
		mMainPlayer = pPlayer;
		SetDlgItemInt(IDC_STATIC_RegionID, mMainPlayer->GetRegionID(),1);
	}
}
