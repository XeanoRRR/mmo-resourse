#include "StdAfx.h"
#include "Region.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRegion::CRegion(void)
{
	SetType(TYPE_REGION);
	m_lRegionType = 0;
	m_lResourceID = 0;					// ������ԴID
	m_fExpScale = 1.0f;					// ��þ���ϵ��
	m_fOccuExpScale = 1.0f;
	m_fSpScale = 1.0f;
	m_fMeriScale = 1.0f;
	m_lWidth = 0;
	m_lHeight = 0;
	m_pCell = NULL;
	m_pCellObj = NULL;
	m_dwLastNotifyHurtTime = 0;
	m_dwLastNotifyKillTime = 0;
}
CRegion::CRegion(const CGUID& guid)
{
	SetType(TYPE_REGION);
	m_lRegionType = 0;
	m_lResourceID = 0;					// ������ԴID
	m_fExpScale = 1.0;					// ��þ���ϵ��
	m_fOccuExpScale = 1.0f;
	m_fSpScale = 1.0;
	m_fMeriScale = 1.0f;
	m_lWidth = 0;
	m_lHeight = 0;
	m_pCell = NULL;
	m_pCellObj = NULL;
	m_dwLastNotifyHurtTime = 0;
	m_dwLastNotifyKillTime = 0;
	m_guid = guid;
	m_lLinedIdFlag =-1;
	m_byRgnHideFlag= 0;
    m_lRejoinTeam  = 0;
}
CRegion::~CRegion(void)
{
	m_strFileName = "";
	M_FREE(m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight);
	M_FREE(m_pCellObj, sizeof(tagCellObj)*m_lWidth*m_lHeight);
	m_vectorSwitch.clear();
}

// ��ȡ
BOOL CRegion::Load()
{
	char strFileName[MAX_PATH];
	_snprintf(strFileName, MAX_PATH, "regions/%d.rgn", GetID());
	FILE *file;
	file = fopen(strFileName, "rb");
	if( file == NULL )
	{
		assert(0);
		return false;
	}

	// ���ļ�ͷ��Ϣ
	char strHead[MAX_PATH];
	memset(strHead, 0, MAX_PATH);
	long lVersion = 0;
	fread(strHead, lstrlen(g_strRegionHead), 1, file);
	if( strcmp(strHead, g_strRegionHead) != 0 )
	{
		assert(0);
		return false;
	}

	fread(&lVersion, sizeof(long), 1, file);
	if( lVersion != g_lRegionVersion )
	{
		assert(0);
		return false;
	}

	m_strFileName = strFileName;

	// ����������
	fread(&m_lRegionType, sizeof(long), 1, file);
	fread(&m_lWidth, sizeof(long), 1, file);
	fread(&m_lHeight, sizeof(long), 1, file);

	New();

	fread(m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight, 1, file);

	long lSwitchNum = 0;
	tagSwitch tagSwitch;
	fread(&lSwitchNum, sizeof(long), 1, file);
	for(int i=0; i<lSwitchNum; i++)
	{
		fread(&tagSwitch, sizeof(tagSwitch), 1, file);
		m_vectorSwitch.push_back(tagSwitch);
	}

	fclose(file);
	return true;
}

// �洢
BOOL CRegion::Save()
{
	char strFileName[MAX_PATH];
	_snprintf(strFileName, MAX_PATH, "regions/%d.rgn", GetID());

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
	M_FREE(m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight);
	m_vectorSwitch.clear();
	M_FREE(m_pCellObj, sizeof(tagCellObj)*m_lWidth*m_lHeight);

	m_pCell = (tagCell*)M_ALLOC(sizeof(tagCell)*m_lWidth*m_lHeight);
	memset(m_pCell, 0, sizeof(tagCell)*m_lWidth*m_lHeight);

	m_pCellObj = (tagCellObj*)M_ALLOC(sizeof(tagCellObj)*m_lWidth*m_lHeight);
	memset(m_pCellObj, 0, sizeof(tagCellObj)*m_lWidth*m_lHeight);
	return true;
}
bool CRegion::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	CBaseObject::CodeToDataBlock(setWriteDB);
	setWriteDB.AddToByteArray(m_lID);
	setWriteDB.AddToByteArray(m_lLinedIdFlag);
	setWriteDB.AddToByteArray(m_lRgnType);
	setWriteDB.AddToByteArray(m_lRegionType);
	setWriteDB.AddToByteArray(m_lResourceID);
	setWriteDB.AddToByteArray(m_fExpScale);
	setWriteDB.AddToByteArray(m_fOccuExpScale);
	setWriteDB.AddToByteArray(m_fSpScale);
	setWriteDB.AddToByteArray(m_fMeriScale);
	setWriteDB.AddToByteArray(m_lWidth);
	setWriteDB.AddToByteArray(m_lHeight);
	setWriteDB.AddToByteArray(m_btCountry);
	setWriteDB.AddToByteArray(m_lNotify);
	return true;
}
// ��ӵ�CByteArray
bool CRegion::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	CBaseObject::AddToByteArray(pByteArray, bExData);	
	_AddToByteArray(pByteArray, m_lID);
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
	return true;
}

// ����
bool CRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CBaseObject::DecordFromByteArray(pByte, pos, bExData);
	m_lID = _GetLongFromByteArray(pByte, pos);
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
    m_lRejoinTeam   = _GetLongFromByteArray(pByte, pos);
	New();	
	int n = sizeof(tagCell);
	_GetBufferFromByteArray(pByte, pos, (BYTE*)m_pCell, sizeof(tagCell)*m_lWidth*m_lHeight);

	tagSwitch tagSwitch;
	long lSwitchNum = _GetLongFromByteArray(pByte, pos);
	for(int i=0; i<lSwitchNum; i++)
	{
		_GetBufferFromByteArray(pByte, pos, &tagSwitch, sizeof(tagSwitch));
		m_vectorSwitch.push_back(tagSwitch);
	}
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

//�õ�һ���ռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
void CRegion::GetLeastShapesCellFromSpace(int nObjCenterX,int nObjCenterY,
		int nObjFigureX,int nObjFigureY,int nMaxDistance,int nDiretion,
		long& lX,long& lY)
{
	//�����������������
	static int facedir[8] = { 4, 5, 6, 7, 0, 1, 2, 3 };
	//���������ƫ������ֵ
	static int DirOff[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	//��ͬ����ǰ����ֵ����������
	static int forward[8][2] = {6,2, 2,0, 0,4, 2,4, 2,6, 4,6, 4,0, 6,0};

	//�����
	int nStep = 0;
	if(nDiretion==0 || nDiretion==4)
		nStep = nMaxDistance*2+1+nObjFigureX;
	else if(nDiretion==2 || nDiretion==6)
		nStep = nMaxDistance*2+1+nObjFigureY;
	else
		nStep = min(nObjFigureX,nObjFigureY)+nMaxDistance*2+1;
	
	int nCount0 = nMaxDistance;
	int nCount1 = nMaxDistance;

	int nLenX = nMaxDistance+nObjFigureX;
	int nLenY = nMaxDistance+nObjFigureY;
	//ѭ����ֵ
	int nFirstX = nObjCenterX+DirOff[facedir[nDiretion]][0]*nLenX;
	int nFirstY = nObjCenterY+DirOff[facedir[nDiretion]][1]*nLenY;

	int nLastX = nObjCenterX+DirOff[nDiretion][0]*nLenX;
	int nLastY = nObjCenterY+DirOff[nDiretion][1]*nLenY;

	long lLeastShapeNum = 0xFFFFFF;
	long lLestX = 0;
	long lLeasY = 0;
	for(int i=0;i<nStep;i++)
	{
		int nBeginX = nFirstX+DirOff[nDiretion][0]*i;
		int nBeginY = nFirstY+DirOff[nDiretion][1]*i;
		//�쿴�Ƿ����赲
		if( GetBlock(nBeginX,nBeginY)== BLOCK_NO )
		{
			//�쿴�Ƿ���MoveShape
			long lShapeNum = GetCellObjNum(nBeginX,nBeginY);
			if( lShapeNum<= 0)
			{
				lX = nBeginX;lY=nBeginY;
				return;
			}
			else if(lShapeNum < lLeastShapeNum)
			{
				lLeastShapeNum = lShapeNum;
				lLestX = nBeginX;lLeasY = nBeginY;
			}
		}

		//ѭ������
		if(nDiretion%2 != 0)
		{
			nCount0 = abs(nLastX-nBeginX);
			nCount1 = abs(nLastY-nBeginY);
		}
		for(int i0=1,i1=1;i0<=nCount0 || i1<=nCount1;i0++,i1++)
		{
			if(i0<=nCount0)
			{
				int nTemptX0 = nBeginX+DirOff[forward[nDiretion][1]][0]*i0;
				int nTemptY0 = nBeginY+DirOff[forward[nDiretion][1]][1]*i0;
				//�쿴�Ƿ����赲
				if( GetBlock(nTemptX0,nTemptY0)== BLOCK_NO )
				{
					//�쿴�Ƿ���MoveShape
					long lShapeNum = GetCellObjNum(nTemptX0,nTemptY0);
					if( lShapeNum<= 0)
					{
						lX = nTemptX0;lY=nTemptY0;
						return;
					}
					else if(lShapeNum < lLeastShapeNum)
					{
						lLeastShapeNum = lShapeNum;
						lLestX = nTemptX0;lLeasY = nTemptY0;
					}
				}
			}
			if(i1<=nCount1)
			{
				int nTemptX1 = nBeginX+DirOff[forward[nDiretion][0]][0]*i1;
				int nTemptY1 = nBeginY+DirOff[forward[nDiretion][0]][1]*i1;
				//�쿴�Ƿ����赲
				if( GetBlock(nTemptX1,nTemptY1)== BLOCK_NO )
				{
					//�쿴�Ƿ���MoveShape
					long lShapeNum = GetCellObjNum(nTemptX1,nTemptY1);
					if( lShapeNum<= 0)
					{
						lX = nTemptX1;lY=nTemptY1;
						return;
					}
					else if(lShapeNum < lLeastShapeNum)
					{
						lLeastShapeNum = lShapeNum;
						lLestX = nTemptX1;lLeasY = nTemptY1;
					}
				}
			}
		}
	}

	lX = lLestX;lY = lLeasY;
}

// �л���
long CRegion::AddSwitch(tagSwitch* pSwitch)
{
	m_vectorSwitch.push_back(*pSwitch);
	return (long)m_vectorSwitch.size();
}

void CRegion::DelSwitch(long lPos)
{
	long l = 1;
	for(vector<tagSwitch>::iterator it=m_vectorSwitch.begin(); it!=m_vectorSwitch.end(); it++)
	{
		if( l++ == lPos )
		{
			m_vectorSwitch.erase(it);

			// ������ͼ������л�����
			for(int i=0; i<GetWidth()*GetHeight(); i++)
			{
				if( m_pCell[i].lSwitch == lPos )
				{
					m_pCell[i].lSwitch = 0;
				}
				else
					if( m_pCell[i].lSwitch > lPos )
					{
						m_pCell[i].lSwitch--;
					}
			}
			return;
		}
	}
}

CRegion::tagSwitch* CRegion::GetSwitch(long l)
{
	if( l > 0 && l <= (long)m_vectorSwitch.size() )
	{
		return &m_vectorSwitch[l-1];
	}
	return NULL;
}

CRegion::tagSwitch* CRegion::GetSwitch(long x, long y)
{
	tagCell* pCell = GetCell(x, y);
	if( pCell )
	{
		int lSwitch = GetCell(x, y)->lSwitch;
		if( lSwitch > 0 )
		{
			return GetSwitch(lSwitch);
		}
	}
	return NULL;
}

// �Ƿ����ָ���ĵ��л���
long CRegion::HasSwitch(tagSwitch* pSwitch)
{
	int num = (int)m_vectorSwitch.size();
	for(int i=0; i<num; i++)
	{
		if(	m_vectorSwitch[i].lState == pSwitch->lState
			&& m_vectorSwitch[i].lRegionID == pSwitch->lRegionID
			&& m_vectorSwitch[i].lCoordX == pSwitch->lCoordX
			&& m_vectorSwitch[i].lCoordY == pSwitch->lCoordY
			&& m_vectorSwitch[i].lDir == pSwitch->lDir )
		{
			return i;
		}
	}
	return -1;
}

void CRegion::GetReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d)
{
	lRegion = l = t = r = b = d = 0;
}
