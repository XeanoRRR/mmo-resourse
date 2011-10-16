#include "stdafx.h"
#include "ConfigParse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConfigParse::CConfigParse(const char* file)
:m_pRoot(NULL)
,m_pCurElem(NULL)
,m_pDoc(NULL)
{
	CRFile *prfile = rfOpen(file);
	if( prfile != NULL )
	{
		m_pDoc = new TiXmlDocument(file);
		m_pDoc->LoadData(prfile->GetData(),prfile->GetDatalen());
		m_pCurElem = m_pRoot = m_pDoc->RootElement();
	}
	rfClose(prfile);
}

CConfigParse::CConfigParse(void)
{
}

CConfigParse::~CConfigParse(void)
{
	DELETE_SAFE(m_pDoc);
}


// ��ָ��·�����õ�ǰ�ڵ�
bool CConfigParse::SetCursor(int count, const char* attr, ...)
{
	va_list argptr;
	va_start(argptr, attr);

	// �ø��ڵ�Ϊ��ǰ�ڵ�
	TiXmlElement* elem = m_pRoot;

	while( NULL != elem )
	{
		m_pCurElem = elem;
		if( --count < 0 )
			break;
		elem = elem->FirstChildElement(attr);
		attr = va_arg(argptr, const char*);
	}

	va_end(argptr);

	if( NULL == m_pCurElem )
	{
		PutoutLog("Config", LT_WARNING, attr);
		return false;
	}
	return true;
}

// �ø��ڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetRoot()
{
	if( NULL != m_pRoot )
	{
		m_pCurElem = m_pRoot;
		return true;
	}

	PutoutLog("Config", LT_WARNING, "");
	return false;
}

// �ø��ڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetParent()
{
	if( NULL == m_pCurElem )
	{
		PutoutLog("Config", LT_WARNING, "NULL == m_pCurElem");
		return false;
	}

	TiXmlNode* node = m_pCurElem->Parent();
	if( NULL != node )
	{
		m_pCurElem = node->ToElement();
		return true;
	}
	return false;
}

// ����һ�ֵܽڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetPrevSibling(const char* name)
{
	PutoutLog("Config", LT_WARNING, "");
	return false;
}

// ����һ�ֵܽڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetNextSibling(const char* name)
{
	if( NULL == m_pCurElem )
	{
		PutoutLog("Config", LT_WARNING, "NULL == m_pCurElem");
		return false;
	}

	TiXmlElement* temp = NULL;
	if( NULL == name )
		temp = m_pCurElem->NextSiblingElement();
	else
		temp = m_pCurElem->NextSiblingElement(name);

	if( NULL != temp )
	{
		m_pCurElem = temp;
		return true;
	}
	else
		return false;
}

// �����ӽڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetFirstChild(const char* name)
{
	if( NULL == m_pCurElem )
	{
		PutoutLog("Config", LT_WARNING, "NULL == m_pCurElem");
		return false;
	}

	TiXmlElement* temp = NULL;
	if( NULL == name )
		temp = m_pCurElem->FirstChildElement();
	else
		temp = m_pCurElem->FirstChildElement(name);

	if( NULL != temp )
	{
		m_pCurElem = temp;
		return true;
	}
	else
		return false;
}

// ��ĩ�ӽڵ�Ϊ��ǰ�ڵ�
bool CConfigParse::SetLastChild(const char* name)
{
	PutoutLog("Config", LT_WARNING, "");
	return false;
}

// ��ȡ��ǰ�ڵ�ָ������
const char* CConfigParse::GetAttr(const char* attr) const
{
	const char* res = NULL;
	if( NULL != m_pCurElem )
	{
		res = m_pCurElem->Attribute(attr);
	}

	if( NULL == res )
	{
		res = "";
		PutoutLog("Config", LT_WARNING, attr);
	}

	return res;
}

// ��ȡ��ǰ�ڵ�֮�ӽڵ�ָ������
const char* CConfigParse::GetChildAttr(const char* tag,
						const char* attr) const
{
	const char* res = NULL;
	TiXmlElement* pChildElem = NULL;
	if( NULL != m_pCurElem &&
		NULL != (pChildElem = m_pCurElem->FirstChildElement(tag) ) )
	{
		
		res = pChildElem->Attribute(attr);
	}

	if( NULL == res )
	{
		res = "";
		char str[128] = "";
		sprintf_s(str, sizeof(str), "%s/%s", tag, attr);
		PutoutLog("Config", LT_WARNING, str);
	}

	return res;
}