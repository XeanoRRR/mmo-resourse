#include "StdAfx.h"
#include "variablelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVariableList::CVariableList()
{

}

CVariableList::~CVariableList()
{
	Release();
}

void CVariableList::Release()
{
	if(m_pVarList.size() == 0) return;

	for(int i=0; i<m_pVarList.size(); i++)
	{
		SAFE_DELETE( m_pVarList[i]->Name );
		if( m_pVarList[i]->Array == 0 )	//����
		{
			SAFE_DELETE( m_pVarList[i]->Value );
		}
		else
		{	
			//�ַ���
			SAFE_DELETE_ARRAY(m_pVarList[i]->strValue);
		}
		OBJ_RELEASE(stVariable, m_pVarList[i]);
	}
	m_pVarList.clear();

	GuidList::iterator guidItr = m_pGuidList.begin(); // GUID�б�
	for(; guidItr != m_pGuidList.end(); guidItr++)
		delete guidItr->second;
	m_pGuidList.clear();
}

// ��ʼ����������
void CVariableList::IniVarNum(long lNum)
{
	Release();
	m_pVarList.resize(m_pVarList.size());
}

/////////////////////////////
//���¶�������б�
void CVariableList::ReLoadVarList(char *filename, char *index, const char* pData)
{
	if(!filename || !index) return;

	CIni Ini(filename, pData);
	long lVarNum=Ini.GetContinueDataNum(index);

	for(int i=0; i<lVarNum; i++)
	{
		stVariable* pTmpVar = OBJ_CREATE(stVariable);
		memset(pTmpVar,0,sizeof(stVariable)); 

		//��ñ�������
		char *tmp=Ini.ReadCaption(index, i);

		// [070919]
		if(tmp[0] == '#') // �ַ���
		{
			// ���±������ֵ
			char* ptr = tmp;

			int lArray = 0;
			char arrSize[128];
			memset(arrSize, 0, sizeof(arrSize));
			bool isX = false; // ��ǰ�ַ��Ƿ�'['
			int n = 0; // �±��ַ�������
			bool hasFindBlank = false; // �Ƿ��ҵ�'['
			int blankPos = 0; // '['��λ��
			while(*ptr != 0)
			{
				if(*ptr == '[')
				{
					isX = true;
					hasFindBlank = true;
				}
				if(*ptr == ']')
					isX = false;

				if( isX && *ptr != '[' && *ptr != ' ' && *ptr != '\t' )
				{
					arrSize[n] = *ptr;
					n++;
				}

				if(!hasFindBlank)
					blankPos++;
				ptr++;
			}

			// ��'[' ']'������Ϊֵ
			if(hasFindBlank) // �ַ�������
			{
				if(arrSize)
				{
					long sizet = blankPos + strlen(arrSize) + 1;
					pTmpVar->Name = new char[sizet];
					memset(pTmpVar->Name, 0, sizet);
					memcpy_s(pTmpVar->Name, blankPos, tmp, blankPos );
					strcat_s(pTmpVar->Name, sizet, arrSize);
				}
			}
			else
			{
				long lTmpSize = strlen(tmp);
				pTmpVar->Name = new char[lTmpSize+1];
				strcpy_s( pTmpVar->Name, lTmpSize+1, tmp);
			}
		}
		else 
		{
			long lTmpSize = strlen(tmp);
			pTmpVar->Name = new char[lTmpSize+1];
			strcpy_s( pTmpVar->Name, lTmpSize+1, tmp);
		}

		pTmpVar->Array=GetArrayNum( pTmpVar->Name, NULL, NULL );	//��ȡ�����±�
		if( pTmpVar->Array > 0 )	//�����ͱ���
		{
			//ȥ����������±�
			GetArrayName( pTmpVar->Name, pTmpVar->Name );

			pTmpVar->Value=new double[ pTmpVar->Array ];
			int x=Ini.ReadInt(index, i);
			for(int j=0; j<pTmpVar->Array; j++)
			{
				pTmpVar->Value[j]=x;
			}
		}
		else if( pTmpVar->Array==0 )	//�򵥱���
		{
			char* pValue;
			pValue=Ini.ReadText(index,i);
			if (*pValue=='\"'){	//�ַ���
				int len=lstrlen(pValue)-2+1;//ȥ��ǰ������,���Ͻ�����0

				pTmpVar->Array=-len;	//������ʾ����Ϊ�ַ���

				pTmpVar->strValue=new char[len];
				memcpy(pTmpVar->strValue,pValue+1,len-1);
				//�ַ���������
				pTmpVar->strValue[len-1]=0;
			}
			else				//����
			{
				if(pTmpVar->Value == NULL)
					pTmpVar->Value = new double;

				*pTmpVar->Value=atof(pValue);//Ini.ReadInt(index, i);
			}
			SAFE_DELETE_ARRAY(pValue);
		}
		else	//����
		{
			assert(0);
			return;
		}

		// ԭ�����ݲ���
		bool isFindVarName = false;
		for(int j=0; j<m_pVarList.size(); j++)
		{
			if( strcmp(m_pVarList[i]->Name, pTmpVar->Name) == 0) // �ҵ��и����ֱ���
			{
				isFindVarName = true;
				break;
			}
		}

		if(isFindVarName == false) // δ�ҵ����б�����
		{
			m_pVarList.push_back(pTmpVar);
		}
		else
			SAFE_DELETE(pTmpVar);

		SAFE_DELETE_ARRAY(tmp);
	}
}
// ��ȡ�����б�
void CVariableList::LoadVarList(char *filename, char *index, const char* pData)
{
	Release();

	if( !filename || !index ) return;

	CIni Ini(filename, pData);
	long lTotalVarNum = Ini.GetContinueDataNum(index);
	
	for(long i=0; i<lTotalVarNum; i++)
	{
		//��ñ�������
		char *tmp = Ini.ReadCaption(index, i);
		if( !tmp ) continue;

		// guid����,�������������Ͷ���
		if( tmp[0] == '@' ) 
		{
			CGUID* tmpGuid = new CGUID();
			m_pGuidList[std::string(tmp)] = tmpGuid;
			SAFE_DELETE_ARRAY(tmp);
			continue;
		}

		stVariable* pVar = OBJ_CREATE(stVariable);
		// ��֤ini��������Ҳ���������浽m_pVarList�����ڴ�й©
		if( !pVar )
		{
			SAFE_DELETE_ARRAY(tmp);
			continue;
		}
		m_pVarList.push_back(pVar);

		// [070919]
		if( tmp[0] == '#' ) // �ַ���
		{
			// ���±������ֵ
			char* ptr   = tmp;
			long lArray = 0;
			char arrSize[128] = {0};
			bool isX = false; // ��ǰ�ַ��Ƿ�'['
			long n   = 0; // �±��ַ�������
			bool hasFindBlank = false; // �Ƿ��ҵ�'['
			int blankPos      = 0; // '['��λ��
			while( *ptr != 0 )
			{
				if( *ptr == '[' )
				{
					isX          = true;
					hasFindBlank = true;
				}
				if( *ptr == ']' )
					isX          = false;

				if( isX && *ptr != '[' && *ptr != ' ' && *ptr != '\t' )
				{
					arrSize[n] = *ptr;
					n++;
				}

				if( !hasFindBlank )
					blankPos++;
				ptr++;
			}

			// ��'[' ']'������Ϊֵ
			if( hasFindBlank ) // �ַ�������
			{
				if( arrSize )
				{
					long sizet = blankPos + strlen(arrSize) + 1;
					pVar->Name = new char[sizet];
					memset(pVar->Name, 0, sizet);
					memcpy_s(pVar->Name, blankPos, tmp, blankPos);
					strcat_s(pVar->Name, sizet, arrSize);
				}
			}
			else
			{
				long lTmpSize = strlen(tmp);
				pVar->Name    = new char[lTmpSize+1];
				strcpy_s( pVar->Name, lTmpSize+1, tmp);
			}
		}
		else if( tmp[0] == '$' ) // ��ֵ��
		{
			long lTmpSize = strlen(tmp);
			pVar->Name    = new char[lTmpSize+1];
			strcpy_s(pVar->Name, lTmpSize+1, tmp);
		}
		SAFE_DELETE_ARRAY(tmp);
	
		pVar->Array = GetArrayNum(pVar->Name, NULL, NULL); //��ȡ�����±�
		if( pVar->Array > 0 )	//�����ͱ���
		{
			//ȥ����������±�
			GetArrayName(pVar->Name, pVar->Name);
			pVar->Value = new double[pVar->Array];
			long vlaue  = Ini.ReadInt(index, i);
			for( long j=0; j<pVar->Array; j++ )
			{
				pVar->Value[j] = vlaue;
			}
		}
		else if( pVar->Array==0 ) //�򵥱���
		{
			char* pValue = NULL;
			pValue = Ini.ReadText(index,i);
			if( pValue && pValue[0]=='\"' ) //�ַ���
			{	
				int len        = lstrlen(pValue)-2+1; //ȥ��ǰ������,���Ͻ�����0
				pVar->Array    = -len;	//������ʾ����Ϊ�ַ���
				pVar->strValue = new char[len];
				memcpy(pVar->strValue, pValue+1, len-1);
				pVar->strValue[len-1] = 0; //�ַ���������
			}
			else //����
			{
				if( NULL == pVar->Value )
					pVar->Value = new double;
				*(pVar->Value) = atof(pValue);
			}
			SAFE_DELETE_ARRAY(pValue);
		}
		else //����
		{
			assert(0);
			AddLogText("������� %s �±�С�ڣ���", pVar->Name);
			return;
		}
	}
}
bool CVariableList::AddVar(const char* name)
{
	if(!name) return false;

	if (*name=='$')
		AddVar(name,0.0f);
	else if(*name=='#')
		AddVar(name,"");
	else
		return false;

	return true;

}
void CVariableList::AddVar(const char* name, double value)
{
	if(!name) return;

	//�ȼ������ǲ����Ѿ�����
	for (int i=0;i<m_pVarList.size();i++)
	{
		if (strcmp(m_pVarList[i]->Name,name)==0)
		{	
			//�������
			SetVarValue(name,0,value);	
			//ֱ�Ӹ�ֵ�˳�
			return;
		}
	}

	stVariable* pVarList = OBJ_CREATE(stVariable);
	
	//����������
	long nameSize = strlen(name);
	pVarList->Name = new char[nameSize+1];
	strcpy( pVarList->Name, name);

	pVarList->Array=0;	//0��ʾ����Ϊ����


	if(pVarList->Value == NULL)
		pVarList->Value = new double;

	*pVarList->Value=value;

	m_pVarList.push_back(pVarList);
}
void CVariableList::AddVar(const char* name, const char* value)
{
	if(!name || !value) return;

	//�ȼ������ǲ����Ѿ�����
	for (int i=0;i<m_pVarList.size();i++)
	{
		if (strcmp(m_pVarList[i]->Name,name)==0)
		{	
			//�������
			SetVarValue(name,value);	
			//ֱ�Ӹ�ֵ�˳�
			return;
		}
	}

	// �����б�
	stVariable* pVarList = OBJ_CREATE(stVariable);
	
	//����������
	long nameSize = strlen(name);
	pVarList->Name = new char[nameSize+1];
	strcpy( pVarList->Name, name);
	int len=lstrlen(value)+1;
	pVarList->Array=-len;	//������ʾ����Ϊ�ַ���
	pVarList->strValue=new char[len];

	memcpy(pVarList->strValue,value,len);

		
	m_pVarList.push_back(pVarList);
}

void CVariableList::AddVar(const char* name, long bound, double value)
{
	if(!name) return;

	//�ȼ������ǲ����Ѿ�����
	if(isExist(name))
	{
		return;
	}
	//boundֻ�ܴ���0
	if (bound<=0)
		return;

	stVariable* pVarList = OBJ_CREATE(stVariable);
	long nameSize = strlen(name);
	pVarList->Name = new char[nameSize+1];
	strcpy( pVarList->Name, name);

	pVarList->Array=bound;

	pVarList->Value=new double[ bound ];


	for(int i=0; i<bound; i++)
	{
		pVarList->Value[i]=value;
	}

	m_pVarList.push_back(pVarList);
}

// [070528 AHC]
void CVariableList::AddGuid(const char* name, const CGUID& guid) // ����һ��GUID����
{
	if(!name) return;

	//�ȼ������ǲ����Ѿ�����
	string guidName = name;
	if(m_pGuidList.find( guidName ) != m_pGuidList.end())
	{
		//�������
		SetGuidValue(name,&guid);	//ֱ�Ӹ�ֵ�˳�
		return;
	}

	CGUID* pGuidList = new CGUID;		// �����б�

	//������ֵ��������
	*pGuidList = guid;
	m_pGuidList[guidName] = pGuidList;
}
bool CVariableList::isExist(const char* name)
{
	if(!name) return false;

	for (int i=0;i<m_pVarList.size();i++){
		if (strcmp(m_pVarList[i]->Name,name)==0){	//�������
			return true;
		}
	}
	return false;
}


bool CVariableList::RemoveVar(const char* name)
{
	if(!name) return false;

	//TODO:ʵ��
	//�ȼ������ǲ����Ѿ�����
	vector<stVariable*>::iterator itr = m_pVarList.begin();
	for (; itr != m_pVarList.end(); itr++)
	{
		//�������
		if(strcmp((*itr)->Name,name)==0)
		{
			if( (*itr)->Array == 0 )
			{
				SAFE_DELETE( (*itr)->Value );
			}
			else
			{
				SAFE_DELETE_ARRAY( (*itr)->strValue );
			}

			SAFE_DELETE_ARRAY( (*itr)->Name );
			OBJ_RELEASE(stVariable,*itr);
			itr = m_pVarList.erase(itr);
			return true;
		}
	}
	return false;
}

bool CVariableList::RemoveGuid(const char* name)//ɾ��Guid����
{
	if(!name) return false;

	//TODO:ʵ��
	//�ȼ������ǲ����Ѿ�����
	string guidName = name;
	GuidList::iterator itr = m_pGuidList.find( guidName );
	if(itr != m_pGuidList.end())
	{
		//�������,ɾ���ɱ���     
		SAFE_DELETE( itr->second );
		m_pGuidList.erase(itr);
		return true;
	}

	return false;
}
//////////////////////////

/////////////////////////////
// ��ȡ������ϸ����
void CVariableList::LoadVarData(char* file, char* index)
{
	
}

int CVariableList::GetArrayNum(char *name, CPlayer* pPlayer, CRegion* pRegion)
{
	if(!name) return -1;

	char* ptr = name;
	int lArray = 0;
	char arrSize[32];
	memset(arrSize, 0, sizeof(arrSize));
	bool isX = false; // ��ǰ�ַ��Ƿ�'['
	int n = 0; // �±��ַ�������
	bool hasFindBlank = false; // �Ƿ��ҵ�'['
	int blankPos = 0; // '['��λ��
	while(*ptr != 0)
	{
		if(*ptr == '[')
		{
			isX = true;
			hasFindBlank = true;
		}
		if(*ptr == ']')
			isX = false;

		if( isX && *ptr != '[' && *ptr != ' ' && *ptr != '\t' )
		{
			arrSize[n] = *ptr;
			n++;
		}

		if(!hasFindBlank)
			blankPos++;
		ptr++;
	}
	return atoi(arrSize);
}

////////////////////////////
// ��ȡ�������������
void CVariableList::GetArrayName(char *name, char *ret)
{
	if(!name || !ret) return;

	strcpy(ret, name);

	//ȥ����������±�
	int len = (int)strlen(ret)-1;
	for(int k=len; k>=0; k--)
	{
		if( ret[k] == '[' )
		{
			ret[k]=0;
			break;
		}
	//	ret[k]=0;
	}
}

///////////////////////
// ��ȡ������ֵ
double CVariableList::GetVarValue(const char *name, int num)
{
	if(!name) return VAR_NOT_FOUND;

	for(int i=0; i<m_pVarList.size(); i++)
	{
		if( stricmp(name, m_pVarList[i]->Name) == 0 )
		{
			if( m_pVarList[i]->Array == 0 && num==0 )
			{
				if(m_pVarList[i]->Value)
					return *m_pVarList[i]->Value;
			}
			else if( m_pVarList[i]->Array <= num )	//�±�Խ��������Ͳ�ƥ��(��������,ʵ��Ϊ�ַ���)
			{
				assert(0);
				return VAR_NOT_FOUND;
			}
			else	//����
			{
				return m_pVarList[i]->Value[num];
			}
		}
	}

//	assert(0);
	return VAR_NOT_FOUND;
}
///////////////////////
// ��ȡ������ֵ(�ַ���)
const char* CVariableList::GetVarValue(const char *name)
{
	if(!name) return NULL;

	for(int i=0; i<m_pVarList.size(); i++)
	{
		if( stricmp(name, m_pVarList[i]->Name) == 0 )
		{
			if( m_pVarList[i]->Array < 0)
				return m_pVarList[i]->strValue;
			else
				return NULL;
		}
	}

//	assert(0);
	return NULL;
}
CGUID CVariableList::GetGuidValue(const char *name)				// ��ȡGUID��ֵ
{
	if(!name) return CGUID::GUID_INVALID;

	string guidName = name;
	map<string, CGUID*>::iterator itr = m_pGuidList.find( guidName );
	if( itr != m_pGuidList.end() )
	{
		return *(itr->second);
	}

	return CGUID::GUID_INVALID;
}
//////////////////////////
// ���ñ�����ֵ���������������±ֵ꣬��
int CVariableList::SetVarValue(const char *name, int num, double value)
{
	if(!name) return VAR_NOT_FOUND;

	for(int i=0; i<m_pVarList.size(); i++)
	{
		if( stricmp(name, m_pVarList[i]->Name) == 0 )
		{
			if( m_pVarList[i]->Array > num )	//����
			{
				m_pVarList[i]->Value[num]=value;
				return true;
			}
			else if(m_pVarList[i]->Array == 0)
			{
				*m_pVarList[i]->Value=value;
				return true;
			}
		}
	}

	//	assert(0);
	return VAR_NOT_FOUND;
}
//////////////////////////
// ���ñ�����ֵ����������ֵ��
int CVariableList::SetVarValue(const char *name, const char* value)
{
	if(!name) return VAR_NOT_FOUND;

	if (0==value)	//�����NULL,��Ĭ��Ϊ�մ�.
		value="";
	for(int i=0; i<m_pVarList.size(); i++)
	{
		if( stricmp(name, m_pVarList[i]->Name) == 0 )
		{
			int len=lstrlen(value)+1;	//���ַ�������,Ҫ����ĩβ�Ľ�����0��ռһ���ֽ�
			//׼�����¹滮����

			if (m_pVarList[i]->Array<0){	//ԭ���ñ������ַ���
				if(abs(m_pVarList[i]->Array) < len){	//���ԭ������ռ���������
					//�¿�����
					SAFE_DELETE_ARRAY(m_pVarList[i]->strValue);
					m_pVarList[i]->strValue=new char[len];
				}
			}
			else if(m_pVarList[i]->Array>0)
			{	
				//ԭ������������
				if(m_pVarList[i]->Array*(double)sizeof(double)<len)
				{
					SAFE_DELETE_ARRAY(m_pVarList[i]->Value);
					m_pVarList[i]->strValue=new char[len];
				}
			}
			else{	//ԭ������������
				SAFE_DELETE(m_pVarList[i]->strValue);
				m_pVarList[i]->strValue=new char[len];
			}

			//�����ַ���
			strcpy(m_pVarList[i]->strValue,value);
			m_pVarList[i]->Array=-len;
			return true;

		}
	}

//	assert(0);
	return VAR_NOT_FOUND;
}

int CVariableList::SetGuidValue(const char *name, const CGUID* value)				// ����GUID��ֵ
{
	if(!name) return VAR_NOT_FOUND;

	string guidName = name;
	map<string, CGUID*>::iterator itr = m_pGuidList.find( guidName );
	if( itr != m_pGuidList.end() )
	{
		*itr->second = *value; // ������ֵ
		return true;
	}

	return VAR_NOT_FOUND;
}

// ��ӵ�vector<BYTE>
//TODO:�����ַ�������֧��(m_pVarList[i]->Array<0)
bool CVariableList::AddToByteArray( vector<BYTE>* pByteArray )
{
	if(!pByteArray) return false;

	_AddToByteArray(pByteArray, (long)m_pVarList.size());				// ��������	

	vector<BYTE> vectorTemp;
	for(int i=0; i<m_pVarList.size(); i++)
	{
		_AddToByteArray(&vectorTemp,m_pVarList[i]->Name);	// ����
		_AddToByteArray(&vectorTemp,m_pVarList[i]->Array);
		if( m_pVarList[i]->Array == 0 )		// �������
		{
			if(m_pVarList[i]->Value)
				_AddToByteArray(&vectorTemp, *(m_pVarList[i]->Value));
		}
		else if( m_pVarList[i]->Array > 0)	// ����
		{
			for(int j=0; j<m_pVarList[i]->Array; j++)
			{
				_AddToByteArray(&vectorTemp, m_pVarList[i]->Value[j]);
			}
		}
		else{		//�ַ���
			_AddToByteArray(&vectorTemp, m_pVarList[i]->strValue);
		}
	}

		// GUID
	_AddToByteArray( pByteArray, (long)m_pGuidList.size() );		// ��������
	GuidList::iterator gitr = m_pGuidList.begin();
	for(; gitr != m_pGuidList.end(); gitr++)
	{
		if( gitr->second != NULL )		// GUID����
		{
			_AddToByteArray( &vectorTemp, gitr->first.c_str() );	// ����	
			_AddToByteArray(&vectorTemp, *(gitr->second));			// GUID����
		}
	}

	_AddToByteArray(pByteArray, (long)vectorTemp.size());
	if(vectorTemp.size())
		_AddToByteArray(pByteArray, &vectorTemp[0], (long)vectorTemp.size());

	return true;
}

// ����
//TODO:�����ַ�������֧��(m_pVarList[i]->Array<0)
bool CVariableList::DecordFromByteArray( BYTE* pByte, long& pos, long bufSize )
{
	if(!pByte) return false;

	char strName[256];
	long lVarNum;
	long lArray;

	// ��ͨ��������
	lVarNum = _GetLongFromByteArray( pByte, pos, bufSize );
  
	//GUID����
	int num = _GetLongFromByteArray( pByte, pos, bufSize );

	//buf size
	int tBufSize = _GetLongFromByteArray( pByte, pos, bufSize );

	double value = 0;

	for(int i=0; i<lVarNum; i++)
	{

		_GetStringFromByteArray(pByte, pos, strName, bufSize);
		lArray = _GetLongFromByteArray(pByte,pos, bufSize);
		if( lArray == 0 )	// �������
		{
			value = _GetDoubleFromByteArray(pByte,pos, bufSize);
			AddVar(strName, value);
		}
		else if(lArray > 0)	// ����
		{
			AddVar(strName, lArray, 0.0f);

			for(int j=0; j<lArray; j++)
			{
				value = _GetDoubleFromByteArray(pByte, pos, bufSize);
				SetVarValue(strName, j, value);
			}
		}
		else
		{		//�ַ���
			char szValue[1024];
			_GetStringFromByteArray(pByte, pos, szValue, bufSize);
			AddVar( strName, szValue );
		}
	}

	// GUID
	char GuidName[256];
	CGUID tGuid;
	for(int i=0; i<num; i++)
	{
		_GetStringFromByteArray( pByte, pos, GuidName, bufSize );	// ����	
		_GetBufferFromByteArray(pByte, pos, tGuid, bufSize);	// GUID����
		AddGuid(GuidName, tGuid);
	}

	return true;
}

bool CVariableList::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	char strName[256];
	char strValue[256]="";
	long lVarNum;
	long lDataSize;
	long lArray;

	//��ͨ��������
	lVarNum = setReadDB.GetLongFromByteArray();  
	//GUID����
	int num = setReadDB.GetLongFromByteArray( );		// ��������

	lDataSize=setReadDB.GetLongFromByteArray();

	double value = 0;

	for(int i=0; i<lVarNum; i++)
	{
		setReadDB.GetStringFromByteArray(strName, 256);
		lArray = setReadDB.GetLongFromByteArray();
		if( lArray == 0 )	// �������
		{
			//SetVarValue(strName, 0, *(long*)(&pByte[pos]) );
			value = setReadDB.GetDoubleFromByteArray();
			AddVar(strName, value);
			//pos+=4;
		}
		else if(lArray > 0)	// ����
		{
			AddVar(strName, lArray, 0.0f);

			for(int j=0; j<lArray; j++)
			{
				value = setReadDB.GetDoubleFromByteArray();
				SetVarValue(strName, j, value);
			}
		}
		else
		{		//�ַ���
			setReadDB.GetStringFromByteArray(strValue,256);
			AddVar(strName,strValue);			
		}
	}

	// GUID
	char GuidName[256];
	CGUID tGuid;
	for(int i=0; i<num; i++)
	{
		setReadDB.GetStringFromByteArray( GuidName,256 );	// ����	
		setReadDB.GetBufferFromByteArray( tGuid);	// GUID����
		AddGuid(GuidName, tGuid);
	}
	return true;
}
