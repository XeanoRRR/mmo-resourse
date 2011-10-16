//********************************************
//	����
//********************************************
#pragma once
#include <windows.h>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

#define BYTE unsigned char

class CGUID;

enum
{
	cPlayerGoodsPageID	= 1,	// ���϶Ի���
	cPlayerEquipPageID,			// װ���Ի���
	cPlayerItemPageID,			// ��ݶԻ���
	cPlayerDepotPageID,			// �ֿ�Ի���
	cNpcTradePageID,			// NPC���׶Ի���
};

// const int enTeamMemberCount = 9;		// ����һ������

void _AddToByteArray(vector<BYTE>* pBA, void* pSource, long n);		// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, const char* pStr);			// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, long l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, short l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, char l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, float l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, double l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, BYTE l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, WORD l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, DWORD l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, const CGUID& guid);			// ��CByteArray�������

bool  _GetBufferFromByteArray(BYTE* pSource, long &lPtr, CGUID& guid, long bufSize=-1);				// ��CByteArray��ȡ���ݿ�
void* _GetBufferFromByteArray(BYTE* pSource, long &lPtr, void* pBuf, long lLen, long bufSize=-1);	// ��CByteArray��ȡ���ݿ�
char* _GetStringFromByteArray(BYTE* pSource, long &lPtr, char* pStr, long pSourceSize=-1);				// ��CByteArray��ȡ�ַ���

// ��CByteArray��ȡlong
inline long _GetLongFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(long))) )
		RaiseException(0x10001, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10001 long err
	lPtr += sizeof(long);
	return *(long*)(&pSource[lPtr-sizeof(long)]);
}

// ��CByteArray��ȡshort
inline short _GetShortFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(short))) )
		RaiseException(0x10002, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10002 short err
	lPtr += sizeof(short);
	return *(short*)(&pSource[lPtr-sizeof(short)]);
}

// ��CByteArray��ȡchar
inline char _GetCharFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(BYTE))) )
		RaiseException(0x10003, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10003 char err
	lPtr ++;
	return (char)pSource[lPtr-1];
}

inline float _GetFloatFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(float))) )
		RaiseException(0x10004, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10004 float err
	lPtr += sizeof(float);
	return *(float*)(&pSource[lPtr-sizeof(float)]);
}
inline double _GetDoubleFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(double))) )
		RaiseException(0x10005, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10005 double err
	lPtr += sizeof(double);
	return *(double*)(&pSource[lPtr-sizeof(double)]);
}
inline BYTE _GetByteFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(BYTE))) )
		RaiseException(0x10006, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10006 BYTE err
	lPtr += sizeof(BYTE);
	return *(BYTE*)(&pSource[lPtr-sizeof(BYTE)]);
}

inline WORD _GetWordFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(WORD))) )
		RaiseException(0x10007, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10007 WORD err
	lPtr += sizeof(WORD);
	return *(WORD*)(&pSource[lPtr-sizeof(WORD)]);
}

inline DWORD _GetDwordFromByteArray(BYTE* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(DWORD))) )
		RaiseException(0x10008, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10008 DWORD err
	lPtr += sizeof(DWORD);
	return *(DWORD*)(&pSource[lPtr-sizeof(DWORD)]);
}