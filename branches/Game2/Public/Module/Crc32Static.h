#ifndef _CRC32STATIC_H_
#define _CRC32STATIC_H_

//����:CRC�����࣬�õ�һ���ļ���Ψһ��32λ������
class CCrc32Static
{
public:
	CCrc32Static();
	virtual ~CCrc32Static();
	static ulong FileCrc32Filemap(LPCTSTR szFilename, ulong &dwCrc32);
	static ulong DataCrc32(const void* pBuff,long lLen, ulong &dwCrc32);

protected:
	static bool GetFileSizeQW(const HANDLE hFile, ulonglong &qwSize);
	static inline void CalcCrc32(const uchar byte, ulong &dwCrc32);
	static ulong s_arrdwCrc32Table[256];
};

#endif