#include "StdAfx.h"
#include "Info.h"

CInfo::CInfo(void)
{
	m_bSwitch		= true;				// ����
	m_uNo			= 1;				// ���
	m_uQuantity		= 1;				// ����
	m_iDirection	= -1;				// ����
}

CInfo::CInfo(CInfo &rInfo)
{
	m_bSwitch		= rInfo.GetSwitch();			// ����
	m_uNo			= rInfo.GetNo();				// ���
	m_rcCoor		= rInfo.GetCoordinate();		// ����
	m_uQuantity		= rInfo.GetQuantity();			// ����
	m_iDirection	= rInfo.GetDirection();			// ����
}

CInfo::~CInfo(void)
{
}

bool CInfo::OffsetCoor(const CPoint point, long lWidth, long lHeight)
{
	m_rcCoor.left	+= point.x;
	m_rcCoor.right	+= point.x;
	m_rcCoor.top	+= point.y;
	m_rcCoor.bottom	+= point.y;

	if(m_rcCoor.right>lWidth)
	{
		m_rcCoor.left = lWidth - m_rcCoor.Width();
		m_rcCoor.right = lWidth;
	}

	if(m_rcCoor.left<1)
	{
		m_rcCoor.right = 1 + m_rcCoor.Width();
		m_rcCoor.left = 1;
	}

	if(m_rcCoor.bottom>lHeight)
	{
		m_rcCoor.top = lHeight - m_rcCoor.Height();
		m_rcCoor.bottom = lHeight;
	}

	if(m_rcCoor.top<1)
	{
		m_rcCoor.bottom = 1 + m_rcCoor.Height();
		m_rcCoor.top = 1;
	}

	return true;
}

bool CInfo::SetCoor(const CRect &rect, long lWidth, long lHeight)
{
	if(m_rcCoor != rect && RectIsValid(rect, lWidth, lHeight))
	{
		m_rcCoor = rect;
		return true;
	}
	return false;
}

// ���κϷ�
bool CInfo::RectIsValid(const CRect &rect, long lWidth, long lHeight)
{
	if(rect.Width()<0 || rect.Height()<0 || rect.left<1 || rect.right>lWidth || rect.top<1 || rect.bottom>lHeight)
	{
		return false;
	}
	
	return true;
}
