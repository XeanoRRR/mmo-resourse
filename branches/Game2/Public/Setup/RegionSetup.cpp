#include "StdAfx.h"
#include "RegionSetup.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<long,CRegionSetup::tagRegionSetup> CRegionSetup::s_mapRegionSetup;

/// ����
CRegionSetup::CRegionSetup()
{
}

/// ����
CRegionSetup::~CRegionSetup()
{
	s_mapRegionSetup.clear();
}

/// ���ļ����س�����������
bool CRegionSetup::LoadRegionSetup(const char* filename)
{
	s_mapRegionSetup.clear();

	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE, "%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	tagRegionSetup stRegionSetup;
	while(ReadTo(stream, "#"))
	{
		stream  >> stRegionSetup.lID
			>> stRegionSetup.lCanEnterLevel
			>> stRegionSetup.pszEnterScript
			>> stRegionSetup.pszExitScript;

		s_mapRegionSetup[stRegionSetup.lID] = stRegionSetup;
	}

	return true;
}

/// ����
bool CRegionSetup::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray, long(s_mapRegionSetup.size()));
	map<long,tagRegionSetup>::iterator iter = s_mapRegionSetup.begin();
	for(; iter!=s_mapRegionSetup.end(); ++iter)
	{
		tagRegionSetup *pRS = &iter->second;
		_AddToByteArray(pByteArray, pRS, sizeof(tagRegionSetup));
	}
	return true;
}

/// ����
bool CRegionSetup::DecordFromByteArray(uchar* pByte, long& pos)
{
	s_mapRegionSetup.clear();	

	tagRegionSetup stRegionSetup;
	long lSize = _GetLongFromByteArray(pByte, pos);
	for(long i=0; i<lSize; i++)
	{
		_GetBufferFromByteArray(pByte, pos, &stRegionSetup, sizeof(tagRegionSetup));
		s_mapRegionSetup[stRegionSetup.lID] = stRegionSetup;
	}
	return true;
}

/// ȡ�ý��볡���ĵȼ�����
long CRegionSetup::GetEnterRegionLvl(long lRegionID)
{
	map<long,tagRegionSetup>::iterator it = s_mapRegionSetup.find(lRegionID);
	if( it != s_mapRegionSetup.end() )
	{
		return (*it).second.lCanEnterLevel;
	}
	return 0;
}

/// ȡ�ý��볡��ִ�еĽű�
const char* CRegionSetup::GetEnterRegionScript(long lRegionID)
{
	map<long,tagRegionSetup>::iterator it = s_mapRegionSetup.find(lRegionID);
	if( it != s_mapRegionSetup.end() )
	{
		return (*it).second.pszEnterScript;
	}
	return NULL;
}


/// ȡ���˳�����ִ�еĽű�
const char* CRegionSetup::GetExitRegionScript(long lRegionID)
{
	map<long,tagRegionSetup>::iterator it = s_mapRegionSetup.find(lRegionID);
	if( it != s_mapRegionSetup.end() )
	{
		return (*it).second.pszExitScript;
	}
	return NULL;
}
