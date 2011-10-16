#pragma once

#include "tinystr.h"
#include "tinyxml.h"
#include <map>
#include <vector>
const char MAXAREANAMESIZE=32;
const char MAXCREDITNAMESIZE=16;

struct tagInterface
{
	string strName;
	long PosX;
	long PosY;
	long Width;
	long Height;
	string Path1;
	string Path2;
	string Path3;
	tagInterface()
	{
		string strName="";
		long PosX=0;
		long PosY=0;
		long Width=0;
		long Height=0;
		string Path1="";
		string Path2="";
		string Path3="";
	}
};
struct tagLauncherBack
{
	string strName;
	long width;
	long height;
	long webX;
	long webY;
	long webW;
	long webH;
	string ImagePath;
	tagLauncherBack()
	{
		string strName="";
		long width=0;
		long height=0;
		long webX=0;
		long webY=0;
		long webW=0;
		long webH=0;
		string ImagePath="";
	}
};

//�����ȡ�����һ�������ļ������ļ���Ҫ����������������Դ·���������С
class CInterfaceXml
{
public:
	CInterfaceXml(void);
	~CInterfaceXml(void);

	bool LoadInterfaceXml();   //��ȡdata�¶����������ļ�
	bool LoadButtonNode(TiXmlElement* pElem);
	bool LoadLauncherBack(TiXmlElement* pElem);

	bool GetInterfaceTag(const char* Name,tagInterface &Interface);
    tagInterface GetInterfaceTag(const char* Name);

	tagLauncherBack GetLauncherBack(){return m_tagLauncherBack;};

private:
	vector<tagInterface> vecInterfacePage;
	tagLauncherBack m_tagLauncherBack;
};

//lanucher���ַ�����Դ��
class CStringRead
{
public:
	CStringRead();
	~CStringRead();

	static bool LoadTextresource();
	static bool LoadString(TiXmlElement* pElem);
	static string GetString(long id);
private:
	static map<long,string>  m_mapStringFile;
};
