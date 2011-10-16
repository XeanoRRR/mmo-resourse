#include "StdAfx.h"
#include ".\region.h"


CRegion::CRegion(void)
{
	SetType(TYPE_REGION);
	m_lRegionType = 0;
	m_lResourceID = 0;					// ������ԴID
	m_lSvrResourceID = 0;
	m_fExpScale = 1.0;					// ��þ���ϵ��
	m_fOccuExpScale = 1.0f;
	m_fSpScale = 1.0f;
	m_fMeriScale = 1.0f;
	m_lWidth = 0;
	m_lHeight = 0;
	m_pCell = NULL;

	m_lID = 0;
	m_lLinedIdFlag = -1;
	m_byRgnHideFlag = 0;
}

CRegion::~CRegion(void)
{
	m_strFileName = "";
	M_FREE(m_pCell,sizeof(tagCell)*m_lWidth*m_lHeight);
	m_vectorSwitch.clear();
}

// ��ȡ
BOOL CRegion::Load()
{
	char strFileName[MAX_PATH];
	sprintf(strFileName, "regions/%d.rgn", GetID());
	
	CRFile *rfile = rfOpen(strFileName);
	if( rfile == NULL)
	{
		assert(0);
		return false;
	}

	// ���ļ�ͷ��Ϣ
	char strHead[MAX_PATH];
	memset(strHead, 0, MAX_PATH);
	long lVersion = 0;
	rfile->ReadData(strHead, lstrlen(g_strRegionHead));
	if( strcmp(strHead, g_strRegionHead) != 0 )
	{
		assert(0);
		return false;
	}

	rfile->ReadData(&lVersion, sizeof(long));
	if( lVersion != g_lRegionVersion )
	{
		assert(0);
		return false;
	}

	m_strFileName = strFileName;

	// ����������
	rfile->ReadData(&m_lRegionType, sizeof(long));
	rfile->ReadData(&m_lWidth, sizeof(long));
	rfile->ReadData(&m_lHeight, sizeof(long));

	New();

	rfile->ReadData(m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight);

	long lSwitchNum = 0;
	tagSwitch stSwitch;
	rfile->ReadData(&lSwitchNum, sizeof(long));
	for(int i=0; i<lSwitchNum; i++)
	{
		rfile->ReadData(&stSwitch, sizeof(tagSwitch));
		m_vectorSwitch.push_back(stSwitch);
	}

	rfClose(rfile);
	return true;
}

// �洢
BOOL CRegion::Save()
{
	char strFileName[MAX_PATH];
	sprintf(strFileName, "regions/%d.rgn", GetID());

	FILE* file;
	file = fopen(strFileName, "wb");
	if( file == NULL )
	{
		assert(0);
		return false;
	}

	m_strFileName = strFileName;

	fwrite(g_strRegionHead, strlen(g_strRegionHead), 1, file);
	fwrite(&g_lRegionVersion, sizeof(long), 1, file);

	fwrite(&m_lRegionType, sizeof(long), 1, file);
	fwrite(&m_lWidth, sizeof(long), 1, file);
	fwrite(&m_lHeight, sizeof(long), 1, file);

	fwrite(m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight, 1, file);

	long lSwitchNum = (long)m_vectorSwitch.size();
	fwrite(&lSwitchNum, sizeof(long), 1, file);
	for(int i=0; i<lSwitchNum; i++)
	{
		fwrite(&m_vectorSwitch[i], sizeof(tagSwitch), 1, file);
	}

	fclose(file);
	return true;
}

// �½�
BOOL CRegion::New()
{
	M_FREE(m_pCell,sizeof(tagCell)*m_lWidth*m_lHeight);
	m_vectorSwitch.clear();

	m_pCell = (tagCell*)M_ALLOC(sizeof(tagCell)*m_lWidth*m_lHeight);
	memset(m_pCell, 0, sizeof(tagCell)*m_lWidth*m_lHeight);

	return true;
}

// ��ӵ�CByteArray
bool CRegion::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	CBaseObject::AddToByteArray(pByteArray, bExData);

	_AddToByteArray(pByteArray, GetID());
	_AddToByteArray(pByteArray, m_lLinedIdFlag);
	_AddToByteArray(pByteArray, m_lRgnType);
	_AddToByteArray(pByteArray, m_lRegionType);
	_AddToByteArray(pByteArray, m_lResourceID);
	_AddToByteArray(pByteArray, m_fExpScale);
	_AddToByteArray(pByteArray, m_fOccuExpScale);
	_AddToByteArray(pByteArray, m_fSpScale);
	_AddToByteArray(pByteArray, m_fMeriScale);
	_AddToByteArray(pByteArray, m_lWidth);
	_AddToByteArray(pByteArray, m_lHeight);
	_AddToByteArray(pByteArray, m_btCountry);
	_AddToByteArray(pByteArray, m_lNotify);
	_AddToByteArray(pByteArray, m_lACRgnType);
	_AddToByteArray(pByteArray, m_byRgnHideFlag);

	int len=sizeof(tagCell);
	_AddToByteArray(pByteArray, (BYTE*)m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight);
	
	long n = (long)m_vectorSwitch.size();
	_AddToByteArray(pByteArray, n);
	for(int i=0; i<n; i++)
	{
		_AddToByteArray(pByteArray, (BYTE*)&m_vectorSwitch[i], sizeof(tagSwitch));
	}

	return true;
}

// ����
bool CRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CBaseObject::DecordFromByteArray(pByte, pos, bExData);
	SetID(_GetLongFromByteArray(pByte, pos));
	m_lLinedIdFlag = _GetLongFromByteArray(pByte, pos);
	m_lRgnType = _GetLongFromByteArray(pByte, pos);
	m_lRegionType = _GetLongFromByteArray(pByte, pos);
	m_lResourceID = _GetLongFromByteArray(pByte, pos);
	m_fExpScale = _GetFloatFromByteArray(pByte, pos);
	m_fOccuExpScale = _GetFloatFromByteArray(pByte, pos);
	m_fSpScale = _GetFloatFromByteArray(pByte, pos);
	m_fMeriScale = _GetFloatFromByteArray(pByte, pos);
	m_lWidth = _GetLongFromByteArray(pByte, pos);
	m_lHeight = _GetLongFromByteArray(pByte, pos);
	m_btCountry = _GetByteFromByteArray(pByte, pos);
	m_lNotify	= _GetLongFromByteArray(pByte, pos);	
	m_lACRgnType = _GetLongFromByteArray(pByte, pos);
	m_byRgnHideFlag = _GetByteFromByteArray(pByte, pos);
	return true;
}

// ��ȡһ����������赲�ĵ�
bool CRegion::GetRandomPos( long &lX , long &lY )
{
	return GetRandomPosInRange(lX, lY, 0, 0, GetWidth(), GetHeight());
}

// ���ܣ���һ����Χ�ڻ�ȡһ����������赲�ĵ�
// �����Χ���ó�����ô������������ȡ�����
// ���1000�ζ�û���ҵ����ʵĵ㣬��ô���α���������ΧѰ�Ҳ����赲�ĵ�
// ������α��������У���ô�ѷ�Χ����һȦ���в���
// �������������û���ҵ����ʵĵ㣬��ô�������һ���㣨�����赲���
bool CRegion::GetRandomPosInRange(long &lX , long &lY, long sx, long sy, long w, long h)
{

_Begin:

	// ����߽��жϣ���������Ͼ�����Ϊ��������
	if (sx>=GetWidth() || sy>=GetHeight() || w<0 || h<0 || sx+w<0 || sy+h<0)
	{
		sx = 0;
		sy = 0;
		w = GetWidth();
		h = GetHeight();
	}

	if (sx<0)
	{
		w += sx;
		sx = 0;
	}

	if (sy<0)
	{
		h += sy;
		sy = 0;
	}

	if (sx+w>GetWidth())
		w = GetWidth() - sx;

	if (sy+h>GetHeight())
		h = GetHeight() - sy;


	// ����1000�λ�ȡ�����
	for(int i=0; i<1000; i++)
	{
		lX	= sx + random( w );
		lY	= sy + random( h );

		tagCell* pCell = GetCell( lX, lY );
		if (pCell)
		{
			if (pCell->bBlock == BLOCK_NO)
			{
				if (pCell->lSwitch == 0)
				{
					return true;
				}
			}
		}
	}

	// ���1000���Ҳ�������ô���α���һ��
	for (lX=sx; lX<sx+w; lX++)
	{
		for (lY=sy; lY<sy+h; lY++)
		{
			tagCell* pCell = GetCell(lX, lY);
			if (pCell)
			{
				if (pCell->bBlock == BLOCK_NO)
				{
					if (pCell->lSwitch == 0)
					{
						return true;
					}
				}
			}
		}
	}


	// ����Ѿ������ŵ�ͼ�������ˣ���ôֻ�����ȡһ������-_-||
	if (sx<=0 && sy<=0 && w>=GetWidth() && h>=GetHeight())
	{
		lX = random(GetWidth());
		lY = random(GetHeight());
		return false;
	}


	// ������α��������У���ô�ѷ�Χ����10Ȧ���в���
	sx -= 10;
	sy -= 10;
	w += 20;
	h += 20;
	goto _Begin;

	return false;
}
