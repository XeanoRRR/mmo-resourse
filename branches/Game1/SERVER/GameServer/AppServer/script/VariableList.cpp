#include "StdAfx.h"
#include "variablelist.h"
#include "script.h"

//#include "vld.h"


// stVariable
stVariable::stVariable()
: Array(0),Value(NULL)
{
}

stVariable::~stVariable(void)
{
	if( Array==0 )
	{
		M_FREE(Value, sizeof(double));
	}
	else if( Array>0 )
	{
		M_FREE(Value, sizeof(double)*Array);
	}
	else if( Array<0 )
	{
		M_FREE(strValue, sizeof(char)*abs(Array));
	}
}

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
	if( m_pVarList.size() == 0 && m_pGuidList.size() == 0 ) return;

    varlistitr itr = m_pVarList.begin();
	for(; itr != m_pVarList.end(); itr++)
	{
        MP_DELETE(itr->second);
	}
	m_pVarList.clear();
   
	ReleaseGuidList();
}

/////////////////////////////
// ��ȡ�����б�
void CVariableList::LoadVarList(char *filename, char *index, const char* pData)
{
	Release();

	CIni Ini(filename, pData);
    int varNum = 0;
	varNum=Ini.GetContinueDataNum(index);

	for(int i=0; i<varNum; i++)
	{
		//��ñ�������
		char *tmp=Ini.ReadCaption(index, i);

        stVariable* tv = MP_NEW stVariable;
              
        tv->Array = GetArrayNum( tmp, NULL, NULL );	//��ȡ�����±�

        if( tv->Array > 0 )	//�����ͱ���
		{
			//ȥ����������±�
			GetArrayName( tmp, tmp );

			tv->Value=(double*)M_ALLOC(sizeof(double)*tv->Array);

			int x=Ini.ReadInt(index, i);
			for(int j=0; j<tv->Array; j++)
			{
				tv->Value[j] = x;
			}
		}
		else if( tv->Array==0 )	//�򵥱���
		{
			char* pValue;
			pValue=Ini.ReadText(index,i);
			if (*pValue=='\"'){	//�ַ���
				int len=lstrlen(pValue)-2+1;//ȥ��ǰ������,���Ͻ�����0

				tv->Array=-len;	//������ʾ����Ϊ�ַ���

				tv->strValue=(char*)M_ALLOC(sizeof(char)*len);
				memcpy(tv->strValue,pValue+1,len-1);
				//�ַ���������
				tv->strValue[len-1]=0;
			}
			else				//����
			{
				if(tv->Value == NULL)
					tv->Value = (double*)M_ALLOC(sizeof(double));

				*tv->Value=atof(pValue);
			}
			SAFE_DELETE(pValue);

		}
		else	//����
		{
			assert(0);
			return;
		}

        m_pVarList[string(tmp)] = tv;
        SAFE_DELETE_ARRAY(tmp);
	}
}
bool CVariableList::AddVar(const char* name)
{
	if (*name=='$')
		AddVar(name,0.0f);
	else if(*name=='#')
		AddVar(name,"");
    else if(*name=='@') // GUID
        AddGuid(name, NULL_GUID);
	else
		return false;

	return true;
}

void CVariableList::AddVar(const char* name,double value)
{
	if (name == NULL)
	{
		return;
	}

	//�ȼ������ǲ����Ѿ�����
    if(m_pVarList.find( string(name) ) != m_pVarList.end())
	{
		//�������
        SetVarValue(name,0,value);	//ֱ�Ӹ�ֵ�˳�
        return;
	}

	stVariable* pVarList = MP_NEW stVariable;		// �����б�

	//��������
	pVarList->Array=0;	//0��ʾ����Ϊ����
	if(pVarList->Value == NULL)
		pVarList->Value = (double*)M_ALLOC(sizeof(double));

	*pVarList->Value=value;

	m_pVarList[string(name)] = pVarList;
}

void CVariableList::AddVar(const char* name,const char* value)
{
	if (name == NULL)
	{
		return;
	}
	//�ȼ������ǲ����Ѿ�����
	if(m_pVarList.find( string(name) ) != m_pVarList.end())
	{
		//�������
        SetVarValue(name,value);	//ֱ�Ӹ�ֵ�˳�
        return;
	}

	stVariable* pVarList = MP_NEW stVariable;		// �����б�

	int len=lstrlen(value)+1;

	pVarList->Array=-len;	//������ʾ����Ϊ�ַ���

	pVarList->strValue=(char*)M_ALLOC(sizeof(char)*len);
	memcpy(pVarList->strValue,value,len);
	
    //pVarList->strValue[len-1]=0;
    //pVarList->strSValue[len-1]=0;

	m_pVarList[string(name)] = pVarList;
}

// [070528 AHC]
void CVariableList::AddGuid(const char* name, const CGUID& guid) // ����һ��GUID����
{
	if (name == NULL)
	{
		return;
	}

    //�ȼ������ǲ����Ѿ�����
    string guidName = name;
    if(m_pGuidList.find( guidName ) != m_pGuidList.end())
    {
        //�������
        SetGuidValue(name,&guid);	//ֱ�Ӹ�ֵ�˳�
        return;
    }

    CGUID* pGuidList = OBJ_CREATE(CGUID);		// �����б�

    //������ֵ��������
    *pGuidList = guid;
    m_pGuidList[guidName] = pGuidList;
}

void CVariableList::AddVar(const char* name,long bound,double value) //����һ�������������
{ 
	if (name == NULL)
	{
		return;
	}

	//�ȼ������ǲ����Ѿ�����
	if(isExist(name))
    {
		return;
	}

	//boundֻ�ܴ���0
	if (bound<=0)
		return;


	stVariable* pVarList = MP_NEW stVariable;		// �����б�

	//��������

	pVarList->Array=bound;

	pVarList->Value=(double*)M_ALLOC(sizeof(double)*bound);

	for(int i=0; i<bound; i++)
	{
		pVarList->Value[i]=value;
	}

	m_pVarList[string(name)] = pVarList;

}

void CVariableList::SetVarList(const VarList* VarList)
{
	if( VarList==NULL )
		return;

	Release();
	
	VarList::const_iterator itr = VarList->begin();
    for(; itr != VarList->end(); itr++)
	{
		long lLen = lstrlen( itr->first.c_str() );

		if(NULL == m_pVarList[itr->first])
		{
			m_pVarList[itr->first] = MP_NEW stVariable;
		}

		m_pVarList[itr->first]->Array = itr->second->Array;
		lLen = itr->second->Array;
		if(lLen == 0)
		{
			m_pVarList[itr->first]->Value = (double*)M_ALLOC(sizeof(double));
			*(m_pVarList[itr->first]->Value) = *itr->second->Value;
		}
		else if( lLen > 0 )	//����
		{ 
			m_pVarList[itr->first]->Value = (double*)M_ALLOC(sizeof(double)*lLen);
			memcpy(m_pVarList[itr->first]->Value,  itr->second->Value,  sizeof(double)*lLen);
		}
		else if(lLen < 0)
		{	
			//�ַ���
			lLen = lstrlen( itr->second->strValue );
			m_pVarList[itr->first]->strValue = (char*)M_ALLOC(sizeof(char)*(lLen+1));
			strcpy_s( m_pVarList[itr->first]->strValue, lLen+1, itr->second->strValue );
			m_pVarList[itr->first]->strValue[lLen]='\0';
		}
	}
}

//�����б��еı���
void CVariableList::UpdateVarList(const VarList* VarList)
{
	if( VarList==NULL )
		return;

    VarList::const_iterator itr = VarList->begin();
    for(; itr != VarList->end(); itr++)
	{
		long lLen = itr->second->Array;

		if(lLen == 0)	//��������
		{
			AddVar((char*)itr->first.c_str(), *itr->second->Value);
		}
		else if( lLen > 0 )	//����
		{ 
			//ȱʡ
			AddVar((char*)itr->first.c_str(), (long)lLen, *itr->second->Value);
		}
		else if(lLen < 0) //�ַ���
        {	
			AddVar((char*)itr->first.c_str(), itr->second->strValue);
		}
	}
}
void CVariableList::ReleaseGuidList(void)
{
	GuidList::iterator gitr = m_pGuidList.begin();
	for(; gitr != m_pGuidList.end(); gitr++)
	{
		OBJ_RELEASE(CGUID, gitr->second );
	}
	m_pGuidList.clear();
}
void CVariableList::SetGuidList(const GuidList* guidList)
{
	if( guidList==NULL )
		return;

	ReleaseGuidList();

	GuidList::const_iterator itr = guidList->begin();
	for(; itr != guidList->end(); itr++)
	{
		CGUID* pGuid = OBJ_CREATE(CGUID);		// �����б�
		if(pGuid)
		{
			//������ֵ��������
			*pGuid = *(itr->second);
			m_pGuidList[itr->first] = pGuid;
		}
	}
}
bool CVariableList::isExist(const char* name)
{
	if (name == NULL)
	{
		return false;
	}

	if(m_pVarList.find( string(name) ) != m_pVarList.end())
    {
		//�������
        return true;
	}
	return false;
}
bool CVariableList::isGuidExist(char* name)
{
	if (name == NULL)
	{
		return false;
	}

    if(m_pGuidList.find( string(name) ) != m_pGuidList.end())
    {	
        //�������
        return true;
    }

    return false;
}

bool CVariableList::RemoveVar(const char* name)
{
	if (name == NULL)
	{
		return false;
	}

	varlistitr it = m_pVarList.find( name );
	if( it != m_pVarList.end() )
	{
		stVariable *var = it->second;
		if( var->Array == 0 )
		{
			M_FREE( var->Value, sizeof(double) );
		}
		else if(var->Array < 0)
		{
			M_FREE( var->strValue, sizeof(char)*abs(var->Array) );
		}
		else if(var->Array > 0)
		{
			M_FREE( var->Value, sizeof(double)*var->Array );
		}

		MP_DELETE(var);
		m_pVarList.erase( it );
		return true;
	}
	
	return false;
}

bool CVariableList::RemoveGuid(const char* name)//ɾ��Guid����
{
	if (name == NULL)
	{
		return false;
	}

	GuidList::iterator it = m_pGuidList.find( name );
	if( it != m_pGuidList.end() )
	{
		OBJ_RELEASE(CGUID, it->second );
		m_pGuidList.erase( it );
		return true;
	}

    return false;
}
//////////////////////////

/////////////////////////////
// ��ȡ������ϸ����
void CVariableList::LoadVarData(char* file, char* index)
{
	CIni Ini(file);
	int num=Ini.GetContinueDataNum(index);

	for(int i=0; i<num; i++)
	{
		char* strCaption = Ini.ReadCaption(index, i);
		int array = GetArrayNum(strCaption, NULL, NULL);	// ȡ�����±�
		char name[NAME_STR_NUM];
		GetArrayName(strCaption, name);		// ȡ��������
		SetVarValue(name, array, Ini.ReadInt(index, i));
		SAFE_DELETE_ARRAY( strCaption );
	}
}

/////////////////////////////
// д�������ϸ����
//TODO:�����ַ�������֧��(m_pVarList[i].Array<0)
void CVariableList::SaveVarData(char* file, char* index)
{
	CIni Ini(file);
    hash_map<string, stVariable*>::iterator itr = m_pVarList.begin();
    for(; itr != m_pVarList.end(); itr++)
	{
		// �򵥱���
		if( itr->second->Array == 0 )
		{
			Ini.Write(index, (char*)itr->first.c_str(), (int)(itr->second->Value));
		}
		else if( itr->second->Array > 0)	// �������
		{
			for(int j=0; j<itr->second->Array; j++)
			{
				char str[NAME_STR_NUM];
				_snprintf(str, 256, "%s[%d]", itr->first.c_str(), j);
				Ini.Write(index, str, itr->second->Value[j]);
			}
		}
		else{	//�ַ�������
		
		}

	}
	Ini.Save();
}

/////////////////////////////
// ��ȡ����������±�
int CVariableList::GetArrayNum(const char *name, CPlayer* pPlayer, CRegion* pRegion, CMonster* pNpc, CGUID& guUsedItemID, DWORD dwDiedMonsterIndex)
{
	int n=0;
	char temp[NAME_STR_NUM];

	unsigned int len = (unsigned int)strlen(name);
	for(unsigned int i=0; i<len; i++)
	{
		if( name[i]=='[' )
		{
			i++;
			while( name[i] != ']' )
			{
				temp[n]=name[i];
				n++;
				i++;
			}

			temp[n]=';';
			temp[n+1] = '\0';
			CScript SubScript((CShape*)pPlayer, pRegion, (CShape*)pNpc, guUsedItemID, dwDiedMonsterIndex);
			SubScript.RunLine(temp);
			return SubScript.GetValue(0);
		}
	}

	return 0;
}

////////////////////////////
// ��ȡ�������������
void CVariableList::GetArrayName(const char *name, char *ret)
{
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
	}
}

///////////////////////
// ��ȡ������ֵ
double CVariableList::GetVarValue(const char *name, int num)
{
	if (name == NULL)
	{
		return 0.0f;
	}

    string varName = name;
    varlistitr itr = m_pVarList.find( varName );
    if( itr != m_pVarList.end() )
	{
        if( itr->second->Array == 0 && num==0 ) // ��ͨ����,���ͱ���
        {
            return *itr->second->Value;
        }
        else if( itr->second->Array <= num || num < 0)	//�±�Խ��������Ͳ�ƥ��(��������,ʵ��Ϊ�ַ���)
        {
			//ShowMessage("'%s' array out of range!", name);
			assert(0);
			return (double)VAR_NOT_FOUND;
		}
		else	//����
		{
			return (double)itr->second->Value[num];
		}
	}

	return (double)VAR_NOT_FOUND;
}

const CGUID &CVariableList::GetGuidValue(const char *name)				// ��ȡGUID��ֵ
{
	if(name == NULL) return NULL_GUID;

    string guidName = name;
    hash_map<string, CGUID*>::iterator itr = m_pGuidList.find( guidName );
    if( itr != m_pGuidList.end() )
    {
        return *(itr->second);
    }

    return CGUID::GUID_INVALID;
}
///////////////////////
// ��ȡ������ֵ(�ַ���)
const char* CVariableList::GetVarValue(const char *name)
{
	if (name == NULL)
	{
		return NULL;
	}

    string varName = name;
    hash_map<string, stVariable*>::iterator itr = m_pVarList.find( varName );
    if( itr != m_pVarList.end() )
	{
		if( itr->second->Array < 0) // ���ַ���
				return itr->second->strValue;
	}

	return NULL;
}
//////////////////////////
// ���ñ�����ֵ���������������±ֵ꣬��
int CVariableList::SetVarValue(const char *name, int num, double value)
{
	if (name == NULL)
	{
		return 0;
	}

    string varName = name;
    hash_map<string, stVariable*>::iterator itr = m_pVarList.find( varName );
    if( itr != m_pVarList.end() )
    {
        if( itr->second->Array > num )	//����
        {
            itr->second->Value[num]=value;
            return 1;
        }
        else if(itr->second->Array == 0)
        {
			if(itr->second->Value == NULL)
			{
				itr->second->Value= (double*)M_ALLOC(sizeof(double));
			}

            *itr->second->Value=value;
            return 1;
        }
    }

	return VAR_NOT_FOUND;
}
//////////////////////////
// ���ñ�����ֵ����������ֵ��
int CVariableList::SetVarValue(const char *name,const char* value)
{
	if (name == NULL)	//�����NULL,��Ĭ��Ϊ�մ�.
	{
		return -1;
	}
	if(NULL==value) 
		value="";

    string varName = name;
    hash_map<string, stVariable*>::iterator itr = m_pVarList.find( varName );
    if( itr != m_pVarList.end() )
    {
        int len=lstrlen(value)+1;	//���ַ�������,Ҫ����ĩβ�Ľ�����0��ռһ���ֽ�
        //����ǿ�Ʒ���,����Ч�����ڴ�ر�֤
        if ( itr->second->Array < 0 ) //ԭ���ñ������ַ���
        {	
			long lOldSize = abs( itr->second->Array );
            //�¿�����
			M_FREE(itr->second->strValue, sizeof(char)*lOldSize);
			itr->second->strValue = (char*)M_ALLOC(sizeof(char)*len);
        }
        else if( itr->second->Array > 0 )  //ԭ������������
        {	
			long lOldSize = (itr->second->Array * (double)sizeof(double));
            M_FREE(itr->second->Value, lOldSize);
			itr->second->strValue=(char*)M_ALLOC(sizeof(char)*len);
        }
        else //ԭ������������
        {	
            M_FREE(itr->second->strValue, sizeof(double));
            itr->second->strValue=(char*)M_ALLOC(sizeof(char)*len);
        }

        //�����ַ���
        strcpy(itr->second->strValue,value);
        itr->second->Array=-len;
        return 1;
	}
	
	return VAR_NOT_FOUND;
}

int CVariableList::SetGuidValue(const char *name, const CGUID* value)				// ����GUID��ֵ
{
	if (name == NULL)
	{
		return 0;
	}
    string guidName = name;
    hash_map<string, CGUID*>::iterator itr = m_pGuidList.find( guidName );
    if( itr != m_pGuidList.end() )
    {
        *itr->second = *value; // ������ֵ
        return 1;
    }

    return VAR_NOT_FOUND;
}

// ��ӵ�vector<BYTE>
//TODO:�����ַ�������֧��(m_pVarList[i].Array<0)
bool CVariableList::AddToByteArray( vector<BYTE>* pByteArray )
{
	_AddToByteArray( pByteArray, (long)m_pVarList.size() );				// ��������	

	vector<BYTE> vectorTemp;
    hash_map<string, stVariable*>::iterator itr = m_pVarList.begin();
    for(; itr != m_pVarList.end(); itr++)
	{
		_AddToByteArray( &vectorTemp, itr->first.c_str() );	// ����
		_AddToByteArray( &vectorTemp, itr->second->Array );
		if( itr->second->Array == 0 )		// �������
		{
			_AddToByteArray(&vectorTemp, *itr->second->Value);
		}
		else if( itr->second->Array > 0)	// ����
		{
			for(int j=0; j<itr->second->Array; j++)
			{
				_AddToByteArray(&vectorTemp, itr->second->Value[j]);
			}
		}
		else{		//�ַ���
			_AddToByteArray(&vectorTemp, itr->second->strValue);
		}
	}

	// GUID
	_AddToByteArray( pByteArray, (long)m_pGuidList.size() );		// ��������
	GuidList::iterator gitr = m_pGuidList.begin();
	for(; gitr != m_pGuidList.end(); gitr++)
	{		
		_AddToByteArray( &vectorTemp, gitr->first.c_str() );	// ����	
		_AddToByteArray(&vectorTemp, *(gitr->second));				// GUID����
	}

	_AddToByteArray(pByteArray, (long)vectorTemp.size());
	if(vectorTemp.size())
		_AddToByteArray(pByteArray, &vectorTemp[0], (long)vectorTemp.size());

	return true;
}

// ����
//TODO:�����ַ�������֧��(m_pVarList[i].Array<0)
bool CVariableList::DecordFromByteArray( BYTE* pByte, long& pos, bool isPlayerVar, long bufSize )
{
	char strName[NAME_STR_NUM];
	char strValue[VALUE_STR_NUM]="";
	long lVarNum;
	long lDataSize;
	long lArray;
	
	if(isPlayerVar)
		LoadVarList("data/variable.ini", "VariableList", GetGame()->GetVariableList_FileData());

	//��ͨ��������
	lVarNum = _GetLongFromByteArray(pByte,pos,bufSize);  
	//GUID����
	int num = _GetLongFromByteArray( pByte, pos,bufSize );		// ��������

	lDataSize=_GetLongFromByteArray(pByte,pos,bufSize);

	double value = 0;

	for(int i=0; i<lVarNum; i++)
	{
		_GetStringFromByteArray(pByte, pos, strName,bufSize);
        lArray = _GetLongFromByteArray(pByte,pos,bufSize);
		if( lArray == 0 )	// �������
		{
			value = _GetDoubleFromByteArray(pByte,pos,bufSize);
			AddVar(strName, value);
		}
		else if(lArray > 0)	// ����
		{
			AddVar(strName, lArray, 0.0f);

			for(int j=0; j<lArray; j++)
			{
				value = _GetDoubleFromByteArray(pByte,pos,bufSize);
				SetVarValue(strName, j, value);
			}
		}
		else
        {		//�ַ���
			_GetStringFromByteArray(pByte, pos, strValue,bufSize);
			AddVar(strName,strValue);			
		}
	}

	// GUID
	char GuidName[NAME_STR_NUM];
	CGUID tGuid;
	for(int i=0; i<num; i++)
	{
		_GetStringFromByteArray( pByte, pos, GuidName,bufSize );	// ����	
		_GetBufferFromByteArray(pByte, pos, tGuid,bufSize);	// GUID����
		AddGuid(GuidName, tGuid);
	}


	return true;
}

bool CVariableList::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray( (long)m_pVarList.size() );				// ��������	

	vector<BYTE> vectorTemp;
    hash_map<string, stVariable*>::iterator itr = m_pVarList.begin();
    for(; itr != m_pVarList.end(); itr++)
	{
		_AddToByteArray( &vectorTemp, itr->first.c_str() );	// ����
		_AddToByteArray( &vectorTemp, itr->second->Array );
		if( itr->second->Array == 0 )		// �������
		{
			_AddToByteArray(&vectorTemp, *itr->second->Value);
		}
		else if( itr->second->Array > 0)	// ����
		{
			for(int j=0; j<itr->second->Array; j++)
			{
				_AddToByteArray(&vectorTemp, itr->second->Value[j]);
			}
		}
		else{		//�ַ���
			_AddToByteArray(&vectorTemp, itr->second->strValue);
		}
	}

	// GUID
	setWriteDB.AddToByteArray( (long)m_pGuidList.size() );		// ��������
	GuidList::iterator gitr = m_pGuidList.begin();
	for(; gitr != m_pGuidList.end(); gitr++)
	{		
		_AddToByteArray( &vectorTemp, gitr->first.c_str() );	// ����	
		_AddToByteArray(&vectorTemp, *(gitr->second));				// GUID����
	}

	setWriteDB.AddToByteArray((long)vectorTemp.size());
	if(vectorTemp.size())
		setWriteDB.AddToByteArray(&vectorTemp[0], (long)vectorTemp.size());

	return true;
}
bool CVariableList::DecodeFromDataBlock(DBReadSet& setReadDB,bool isPlayerVar)
{
	char strName[NAME_STR_NUM];
	char strValue[VALUE_STR_NUM]="";
	long lVarNum;
	long lDataSize;
	long lArray;
	
	if(isPlayerVar)
		LoadVarList("data/variable.ini", "VariableList", GetGame()->GetVariableList_FileData());

	//��ͨ��������
	lVarNum = setReadDB.GetLongFromByteArray();  
	//GUID����
	int num = setReadDB.GetLongFromByteArray( );		// ��������

	lDataSize=setReadDB.GetLongFromByteArray();

	double value = 0;

	for(int i=0; i<lVarNum; i++)
	{
		setReadDB.GetStringFromByteArray(strName,NAME_STR_NUM);
        lArray = setReadDB.GetLongFromByteArray();
		if( lArray == 0 )	// �������
		{
			value = setReadDB.GetDoubleFromByteArray();
			AddVar(strName, value);
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
			setReadDB.GetStringFromByteArray(strValue,VALUE_STR_NUM);
			AddVar(strName,strValue);			
		}
	}

	// GUID
	char GuidName[NAME_STR_NUM];
	CGUID tGuid;
	for(int i=0; i<num; i++)
	{
		setReadDB.GetStringFromByteArray( GuidName,NAME_STR_NUM );	// ����	
		setReadDB.GetBufferFromByteArray( tGuid);	// GUID����
		AddGuid(GuidName, tGuid);
	}
	return true;
}
