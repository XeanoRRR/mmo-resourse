#include "StdAfx.h"
#include "GMList.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ��̬��Ա��ʼ��
map<string, tagGMInfo> GMList::m_mapGMInfo;
string GMList::s_strGodPassport = "@^$^#SDFSDslfld/$dsl2a";

/// ����
GMList::GMList()
{
}

/// ����
GMList::~GMList()
{
	Clear();
}

/// ����
void GMList::Clear()
{
	m_mapGMInfo.clear();
}

/// ���ļ�����GM�б�
bool GMList::Load(char* filename)
{
	std::ifstream ListFile;
	ListFile.open(filename, ios_base::in);
	if(ListFile.fail())
	{
		ListFile.close();
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SYS_31", filename));
		return false;
	}

	char bufLevel[256] = "";

	Clear();

	tagGMInfo st;
	while (!ListFile.eof())
	{
		ListFile >> st.strName >> bufLevel;

		if (0 == strcmp(bufLevel, "admin"))
			st.lLevel = GM_ADMIN;
		else if(0 == strcmp(bufLevel, "arch"))
			st.lLevel = GM_ARCH;
		else if(0 == strcmp(bufLevel, "wizard"))
			st.lLevel = GM_WIZARD;
		else
			st.lLevel = GM_PLAYER;

		if(st.lLevel > GM_PLAYER)
			m_mapGMInfo[st.strName] = st;
	}
	ListFile.close();

	CRFile* prfile = rfOpen("data/temp.ini");
	if(prfile != NULL)
	{
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);

		stream >> s_strGodPassport;
	}

	return true;
}

///��GM���ֶ�ȡ����Ϣ
tagGMInfo* GMList::GetInfoByName(const char* name)
{
	if (m_mapGMInfo.find(name) == m_mapGMInfo.end())
	{
		return NULL;
	}

	return &m_mapGMInfo[name];
}

/// ����
bool GMList::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray, (long)m_mapGMInfo.size());
	map<string, tagGMInfo>::iterator it=m_mapGMInfo.begin();
	for (; it!=m_mapGMInfo.end(); ++it)
	{
		tagGMInfo* pst = &it->second;
		_AddToByteArray(pByteArray, (char*)pst->strName.c_str());
		_AddToByteArray(pByteArray, (long)pst->lLevel);
	}
	_AddToByteArray(pByteArray, (char*)s_strGodPassport.c_str());

	return true;
}

/// ����
bool GMList::DecordFromByteArray(uchar* pByte, long& pos)
{
	Clear();

	char str[256];
	long lCount = _GetLongFromByteArray(pByte, pos);
	for (int i=0; i<lCount; i++)
	{
		tagGMInfo st;
		st.strName = _GetStringFromByteArray(pByte, pos, str);
		st.lLevel = (eGMLevel)_GetLongFromByteArray(pByte, pos);
		m_mapGMInfo[st.strName] = st;
	}

	_GetStringFromByteArray(pByte, pos, str);
	s_strGodPassport = str;
	return true;
}
