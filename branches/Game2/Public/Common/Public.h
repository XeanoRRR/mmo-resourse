//********************************************
//	����
//********************************************
#pragma once

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

void _AddToByteArray(vector<uchar>* pBA, void* pSource, long n);	// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, const char* pStr);			// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, long l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, short l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, char l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, float l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, double l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, uchar l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, ushort l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, ulong l);					// ��CByteArray�������
void _AddToByteArray(vector<uchar>* pBA, const CGUID& guid);		// ��CByteArray�������

bool  _GetBufferFromByteArray(uchar* pSource, long &lPtr, CGUID& guid, long bufSize=-1);				// ��CByteArray��ȡ���ݿ�
void* _GetBufferFromByteArray(uchar* pSource, long &lPtr, void* pBuf, long lLen, long bufSize=-1);	    // ��CByteArray��ȡ���ݿ�
char* _GetStringFromByteArray(uchar* pSource, long &lPtr, char* pStr, long pSourceSize=-1);				// ��CByteArray��ȡ�ַ���

// ��CByteArray��ȡlong
inline long _GetLongFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(long))) )
		RaiseException(0x10001, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10001 long err
	lPtr += sizeof(long);
	return *(long*)(&pSource[lPtr-sizeof(long)]);
}

// ��CByteArray��ȡshort
inline short _GetShortFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(short))) )
		RaiseException(0x10002, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10002 short err
	lPtr += sizeof(short);
	return *(short*)(&pSource[lPtr-sizeof(short)]);
}

// ��CByteArray��ȡchar
inline char _GetCharFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(uchar))) )
		RaiseException(0x10003, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10003 char err
	lPtr ++;
	return (char)pSource[lPtr-1];
}

inline float _GetFloatFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(float))) )
		RaiseException(0x10004, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10004 float err
	lPtr += sizeof(float);
	return *(float*)(&pSource[lPtr-sizeof(float)]);
}
inline double _GetDoubleFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(double))) )
		RaiseException(0x10005, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10005 double err
	lPtr += sizeof(double);
	return *(double*)(&pSource[lPtr-sizeof(double)]);
}
inline uchar _GetByteFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(uchar))) )
		RaiseException(0x10006, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10006 uchar err
	lPtr += sizeof(uchar);
	return *(uchar*)(&pSource[lPtr-sizeof(uchar)]);
}

inline ushort _GetWordFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(ushort))) )
		RaiseException(0x10007, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10007 ushort err
	lPtr += sizeof(ushort);
	return *(ushort*)(&pSource[lPtr-sizeof(ushort)]);
}

inline ulong _GetDwordFromByteArray(uchar* pSource, long &lPtr, long pSourceSize=-1)
{
	if( (pSourceSize != -1 && pSourceSize < long(lPtr+sizeof(ulong))) )
		RaiseException(0x10008, EXCEPTION_NONCONTINUABLE, 0, 0);// 0x10008 ulong err
	lPtr += sizeof(ulong);
	return *(ulong*)(&pSource[lPtr-sizeof(ulong)]);
}