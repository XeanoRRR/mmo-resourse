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
void _AddToByteArray(vector<BYTE>* pBA, char* pStr);				// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, long l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, short l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, char l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, float l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, BYTE l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, WORD l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, DWORD l);					// ��CByteArray�������
void _AddToByteArray(vector<BYTE>* pBA, const CGUID& guid);			// ��CByteArray�������

bool  _GetBufferFromByteArray(BYTE* pSource, long &lPtr, CGUID& guid);				// ��CByteArray��ȡ���ݿ�
void* _GetBufferFromByteArray(BYTE* pSource, long &lPtr, void* pBuf, long lLen);	// ��CByteArray��ȡ���ݿ�
char* _GetStringFromByteArray(BYTE* pSource, long &lPtr, char* pStr);				// ��CByteArray��ȡ�ַ���

// ��CByteArray��ȡlong
inline long _GetLongFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(long);
	return *(long*)(&pSource[lPtr-sizeof(long)]);
}

// ��CByteArray��ȡshort
inline short _GetShortFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(short);
	return *(short*)(&pSource[lPtr-sizeof(short)]);
}

// ��CByteArray��ȡchar
inline char _GetCharFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr ++;
	return (char)pSource[lPtr-1];
}

inline float _GetFloatFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(float);
	return *(float*)(&pSource[lPtr-sizeof(float)]);
}

inline BYTE _GetByteFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(BYTE);
	return *(BYTE*)(&pSource[lPtr-sizeof(BYTE)]);
}

inline WORD _GetWordFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(WORD);
	return *(WORD*)(&pSource[lPtr-sizeof(WORD)]);
}

inline DWORD _GetDwordFromByteArray(BYTE* pSource, long &lPtr)
{
	lPtr += sizeof(DWORD);
	return *(DWORD*)(&pSource[lPtr-sizeof(DWORD)]);
}
