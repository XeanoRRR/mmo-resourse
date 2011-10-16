// Fox@20080917----------------------------------------------
// ConfigParse.h
// Author :		Fox(yulefox@gmial.com)
// Function :	���ڽ��������ļ���ÿ�������ļ���Ӧһ������
// Create :		2008-09-05
// Fox@20080917----------------------------------------------

#ifndef _PUBLIC_CONFIG_PARSE_H_
#define	_PUBLIC_CONFIG_PARSE_H_

#include "TinyXml.h"
#include "Tools.h"

class CConfigParse
{
public:
	CConfigParse(const char* file);
	~CConfigParse(void);

	bool IsLoaded(void)			// �Ƿ���سɹ�
	{
		return NULL != m_pRoot;
	}

	// ���ݽڵ��ι�ϵ��ȡָ���ڵ�ֵ
	const char* GetVal(const char* attr) const;

	// ��ȡ��ǰ�ڵ�ָ������
	template<typename T>
	const char* GetAttr(const char* attr,
		T val) const;			// ��Ĭ��ֵ��ȱʡ��Ӧ�ó�ʼ��ʱָ��
	template<typename T, typename CT>
	const char* GetAttr(const char* attr,
		T val,
		CT def) const;			// ��Ĭ��ֵ��ȱʡ��Ӧ�õ���ʱָ��
	const char* GetAttr(const char* attr) const;

	// ��ȡ��ǰ�ڵ�֮�ӽڵ�ָ������
	template <typename T>
	const char* GetChildAttr(const char* tag,
		const char* attr,
		T val) const;			// ��Ĭ��ֵ��ȱʡ��Ӧ�ó�ʼ��ʱָ��
	template<typename T, typename CT>
	const char* GetChildAttr(const char* tag,
		const char* attr,
		T val,
		CT def) const;			// ��Ĭ��ֵ��ȱʡ��Ӧ�õ���ʱָ��
	const char* GetChildAttr(const char* tag,
		const char* attr) const;

	// ��ָ��·�����õ�ǰ�ڵ�
	bool SetCursor(int count, const char* attr, ...);

	// �ø��ڵ�Ϊ��ǰ�ڵ�
	bool SetRoot();

	// �ø��ڵ�Ϊ��ǰ�ڵ�
	bool SetParent();

	// ����һ�ֵܽڵ�Ϊ��ǰ�ڵ�
	bool SetPrevSibling(const char* name = NULL);

	// ����һ�ֵܽڵ�Ϊ��ǰ�ڵ�
	bool SetNextSibling(const char* name = NULL);

	// �����ӽڵ�Ϊ��ǰ�ڵ�
	bool SetFirstChild(const char* name = NULL);

	// ��ĩ�ӽڵ�Ϊ��ǰ�ڵ�
	bool SetLastChild(const char* name = NULL);

protected:	// �ⲿ������
	CConfigParse(void);

private:
	enum CONFIG_ERROR
	{
		CE_NO_ERROR	= 0,		// ��ȷ
		CE_ROOT_NULL,			// ���ڵ�Ϊ��
		CE_PARENT_NULL,			// ���ڵ�Ϊ��
		CE_CURRENT_NULL,		// ��ǰ�ڵ�Ϊ��
		CE_PRVIOUS_NULL,		// ǰһ�ڵ�Ϊ��
		CE_NEXT_NULL,			// ��һ�ڵ�Ϊ��
		CE_FIRST_CHILD_NULL,	// ���ӽڵ�Ϊ��
		CE_LAST_CHILD_NULL,		// ĩ�ӽڵ�Ϊ��
	};

	TiXmlDocument* m_pDoc;
	TiXmlElement* m_pRoot;
	TiXmlElement* m_pCurElem;
};

template <typename T>
const char* CConfigParse::GetAttr(const char* attr,
								  T val) const
{
	const char* res = NULL;
	if( NULL != m_pCurElem )
	{
		res = m_pCurElem->Attribute(attr, val);
	}

	if( NULL == res )
	{
		PutoutLog("Config", LT_WARNING, attr);
	}

	return res;
}

template <typename T, typename CT>
const char* CConfigParse::GetAttr(const char* attr,
								  T val,
								  CT def) const
{
	const char* res = NULL;
	if( NULL != m_pCurElem )
	{
		res = m_pCurElem->Attribute(attr, val);
	}

	if( NULL == res )
	{
		*val = def;
		PutoutLog("Config", LT_WARNING, attr);
	}

	return res;
}

template<typename T>
const char* CConfigParse::GetChildAttr(const char* tag,
			const char* attr,
			T val) const
{
	const char* res = NULL;
	TiXmlElement* pChildElem = NULL;
	if( NULL != m_pCurElem &&
		NULL != (pChildElem = m_pCurElem->FirstChildElement(tag) ) )
	{
		
		res = pChildElem->Attribute(attr, val);
	}

	if( NULL == res )
	{
		char str[128] = "";
		sprintf_s(str, sizeof(str), "%s/%s", tag, attr);
		PutoutLog("Config", LT_WARNING, str);
	}

	return res;
}

template<typename T, typename CT>
const char* CConfigParse::GetChildAttr(const char* tag,
			const char* attr,
			T val,
			CT def) const
{
	const char* res = NULL;
	TiXmlElement* pChildElem = NULL;
	if( NULL != m_pCurElem &&
		NULL != (pChildElem = m_pCurElem->FirstChildElement(tag) ) )
	{
		
		res = pChildElem->Attribute(attr, val);
	}

	if( NULL == res )
	{
		*val = def;
		char str[128] = "";
		sprintf_s(str, sizeof(str), "%s/%s", tag, attr);
		PutoutLog("Config", LT_WARNING, str);
	}

	return res;
}

#endif