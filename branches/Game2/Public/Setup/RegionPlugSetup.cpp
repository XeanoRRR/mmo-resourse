#include "StdAfx.h"
#include "RegionPlugSetup.h"
#include "../Common/Public.h"

map<long, CRegionPlugSetup::tagRegionPlugSetup>	CRegionPlugSetup::m_mapRegionPlugSetup;

/// ȡ��region plug ����
map<long, CRegionPlugSetup::tagRegionPlugSetup>& CRegionPlugSetup::GetRegionPlugSetupMap(void)
{
	return m_mapRegionPlugSetup;
}

/// ���ļ�����region plug����
bool CRegionPlugSetup::LoadRegionPlugSetup(const char* szPath)
{
	if(szPath == NULL) return FALSE;

	m_mapRegionPlugSetup.clear();
	CRFile* prfile = rfOpen(szPath);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,szPath);
		return false;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	tagRegionPlugSetup tmpRgnPlugSetup;
	while( ReadTo(stream, "#") )
	{
		stream >> tmpRgnPlugSetup.lID
			>> tmpRgnPlugSetup.bType
			>> tmpRgnPlugSetup.lRegionID
			>> tmpRgnPlugSetup.lX1
			>> tmpRgnPlugSetup.lY1
			>> tmpRgnPlugSetup.lX2
			>> tmpRgnPlugSetup.lY2;
		m_mapRegionPlugSetup[tmpRgnPlugSetup.lID] = tmpRgnPlugSetup;
	}

	return true;
}

/// ����
void CRegionPlugSetup::Release()
{
	m_mapRegionPlugSetup.clear();
}

/// ����
bool CRegionPlugSetup::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray, (long)m_mapRegionPlugSetup.size());// num
	map<long, tagRegionPlugSetup>::iterator itr = m_mapRegionPlugSetup.begin();
	for(; itr!=m_mapRegionPlugSetup.end(); ++itr)
	{
		_AddToByteArray(pByteArray, &(itr->second), sizeof(tagRegionPlugSetup));
	}
	return true;
}

/// ����
bool CRegionPlugSetup::DecordFromByteArray(uchar* pByte, long& pos)
{
	m_mapRegionPlugSetup.clear();
	long num = _GetLongFromByteArray(pByte, pos);
	for(int i=0; i<num; i++)
	{
		tagRegionPlugSetup tmpRgnPlugSetup;
		_GetBufferFromByteArray(pByte, pos, &tmpRgnPlugSetup, sizeof(tagRegionPlugSetup));
		m_mapRegionPlugSetup[tmpRgnPlugSetup.lID] = tmpRgnPlugSetup;
	}
	return true;
}