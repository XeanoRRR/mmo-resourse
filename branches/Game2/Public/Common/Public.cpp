//********************************************
//	����
//********************************************

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, void* pSource, long n)
{
	if(n <= 0)	return;
	size_t cursize = pBA->size();
	size_t totalsize = cursize+ n;
	pBA->resize(totalsize);
	memcpy(&(pBA->operator [](cursize)),pSource,n);
}
	
// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, const char* pStr)
{
	long size = lstrlen(pStr);
	_AddToByteArray(pBA,(void*)&size,sizeof(long));
	_AddToByteArray(pBA,(void*)pStr,size);
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, long l)
{
	_AddToByteArray(pBA,(void*)&l,sizeof(long));
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, short l)
{
	_AddToByteArray(pBA,(void*)&l,sizeof(short));
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, char l)
{
	pBA->push_back(l);
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, float l)
{
	_AddToByteArray(pBA,(void*)&l,sizeof(float));
}
// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, double l)	
{
	_AddToByteArray(pBA,(void*)&l,sizeof(double));
}
// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, uchar l)
{
	pBA->push_back(l);
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, ushort l)
{
	_AddToByteArray(pBA,(void*)&l,sizeof(ushort));
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, ulong l)
{
	_AddToByteArray(pBA,(void*)&l,sizeof(ulong));
}

// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, const CGUID& guid)
{
	long size = sizeof(CGUID);
	if(guid == CGUID::GUID_INVALID)
	{
		size = 0;
	}
	_AddToByteArray(pBA,(void*)&size,1);
	_AddToByteArray(pBA,(void*)&guid,size);
}

// ��CByteArray��ȡ���ݿ�
bool  _GetBufferFromByteArray(uchar* pSource, long &lPtr, CGUID& guid, long bufSize)
{
	long size = 0;
	_GetBufferFromByteArray(pSource,lPtr,&size,1, bufSize);
	if(size == 0)
	{
		guid = CGUID::GUID_INVALID;
		return false;
	}
	else
	{
		_GetBufferFromByteArray(pSource,lPtr,&guid,sizeof(CGUID), bufSize);
		return true;
	}
	return true;
}

// ��CByteArray��ȡ���ݿ�
void* _GetBufferFromByteArray(uchar* pSource, long &lPtr, void* pBuf, long lLen, long bufSize)
{
	if( bufSize != -1 && bufSize < lPtr+lLen )
		RaiseException(0x1000A, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x1000A buffer err

	//copy
	memcpy(pBuf,pSource+lPtr,lLen);
	lPtr += lLen;
	return pBuf;
}

// ��CByteArray��ȡ�ַ���
char* _GetStringFromByteArray(uchar* pSource, long& lPtr, char* pStr, long pSourceSize)
{
	long lSize = 0;
	_GetBufferFromByteArray(pSource,lPtr,(void*)&lSize,sizeof(long), pSourceSize);
	_GetBufferFromByteArray(pSource,lPtr,(void*)pStr,lSize, pSourceSize);
	pStr[lSize]='\0';
	return pStr;
}

