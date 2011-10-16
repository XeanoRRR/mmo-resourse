#ifndef		__packagefile__
#define		__packagefile__

////////////////////////////////////////////////////////
//Fun:ʵ�ִ������
//Create Time:2004.11.1
//Author:Joe
////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include <set>

using namespace std;

class CClientResource;


struct tagFileIndex
{
	char	szName[256];
	DWORD	dwOffset;
	DWORD	dwSize;							//��Ӳ���ϵĴ�С��δѹ������ԭʼ��С��ѹ���˾���ѹ�����С
	DWORD	dwOrginSize;					//�ļ�ԭʼ��С��δѹ��ǰsize
	DWORD	dwValidSize;					//��ǰ��Ч���ļ���С����Ҫ���ڶϵ�������¼�Ѿ����ص��ļ���С
	DWORD	dwCrc32;
	DWORD   dwCompressType;					/// 0 δѹ�� 1 ѹ��
};

struct SortEmpty
{
	bool operator()	(tagFileIndex left,tagFileIndex right)
	{
		if(left.dwSize>=right.dwSize)
			return true;
		else
			return false;
	}
};

typedef map<string,tagFileIndex>	INDEXOFFILES;
typedef map<string,tagFileIndex>::iterator itIndex;
typedef map<string,tagFileIndex>::const_iterator const_itIndex;

typedef multiset<tagFileIndex,SortEmpty>		IndexEmptys;
typedef multiset<tagFileIndex,SortEmpty>::iterator itEmpty;

class CPackage
{
public:
	CPackage(CClientResource* pClientResource,string& strWorkPath,
		DWORD dwMaxIndexNum = 100000,DWORD dwMaxEmptyIndexNum=1);
	~CPackage();

	DWORD	GetID()				{return m_dwTypeID;}
	void	SetID(DWORD dwId)	{m_dwTypeID = dwId;}

	bool	Open( const char* pszPackage,bool bUpdate=true );
	bool	OpenForUpdate( const char* pszPackage);

	bool	Create(const char* pszPackage);
	DWORD	AddFile(const char* pszFile ,DWORD &dwCrc32,
					DWORD dwCompress, DWORD &dwOrginSize);

	DWORD	AddFile(const char* strFileName,BYTE* pBuf,int iSize,
					DWORD &dwCrc32,DWORD dwCompress, DWORD &dwOrginSize);

	DWORD	AddFile(FILE* pf ,const char* strFileName,DWORD &dwCrc32,
					DWORD dwCompress, DWORD &dwOrginSize);

	DWORD	DelFile(const char* pszFile);																	//ɾ�������ļ�

	DWORD	AddFileFromPackage(	CPackage* pPackage,const char* pszFile,
								DWORD dwCompress);

	DWORD	AdjustFile(const char* pszFile,DWORD dwCompress);												//�ڲ������ļ�

	DWORD	ExtractToFile(const char* pszFile,string strFolder="");											//��ȡ���ļ�

	bool	ExtractToBuf(	const char* pszFile,BYTE *& pBuf,int iSize,
							DWORD& dwCrc32,DWORD& dwCompressType,
							DWORD &dwOrginSize,DWORD &dwValidSize);	//��ȡ������

	bool	ExtractToBuf(const char* pszFile,BYTE *& pBuf,int iSize,
							tagFileIndex& FileIndex);														//��ȡ������

	long	GetFileSize(const char* pszFile);																//�õ��ļ���С

	//bool	WriteIndex();			//����
	bool	WriteIndexEx();
	void	Crypto( void* pBuf, DWORD dwSize);

	void	Release();

	const INDEXOFFILES* GetIndexs()	{return & m_Indexs;	}

	void	OpenFileHandle(const char* pszPackage);
	void	CloseFileHandle();
	DWORD	WriteData(tagFileIndex& FileIndex,const void* pBuf);									//�����д������
	DWORD	WriteDataEx(tagFileIndex& FileIndex,const void* pBuf);									//�����д������

	//��Կͻ��˵�����д���ݲ���
public:
	bool	ClearData();
	DWORD	AddFile(const char* strFileName,int nSize,DWORD dwCrc32,
					DWORD dwCompress, DWORD dwOrginSize);

	DWORD	AddFileData(const char* strFileName,BYTE* pBuf,
						DWORD& nValidSize,DWORD nSize);
	FILE*	GetFileHandle()		{return m_pFile;}

	bool	GetFirstFileInfo(tagFileIndex& FileIndex);

private:
	void	AddEmptyPart(tagFileIndex& FileIndex);													//���һ��������
	bool	GetEmptyPart(DWORD dwSize,tagFileIndex& FileIndex);										//���ݴ�С�õ�һ��������

private:
	DWORD			m_dwMaxIndexNum;
	DWORD			m_dwMaxEmptyIndexNum;
	
	DWORD			m_dwIndexHeadSize;								//�ļ�����ͷ��С	
	DWORD			m_dwEmptyIndexHeadSize;							//���ļ�����ͷ�Ĵ�С

	DWORD			m_dwTypeID;										//��ID
	FILE*			m_pFile;
	string			m_strWorkPath;
	string			m_strPackName;

	INDEXOFFILES	m_Indexs;										//��ǰ�����ļ�����
	IndexEmptys		m_Emptys;										//��ǰ������Ŀ¼
	DWORD			m_dwOffset;										//��ǰ���������ļ����ݵĵ�ƫ����

	CClientResource*		m_pOriginResource;						//�ͻ�����Դ��
};

// Lzo1x compress
void CompressData(const unsigned char* pInData,unsigned long lInDataSize,
				  unsigned char* pOutBuf,unsigned long* plOutDataSize,bool bFast);

// Lzo1x uncompress
void DeCompressData(const unsigned char * pInData,unsigned long lInDataSize,
					unsigned char * pOutBuf,unsigned long* lOutDataSize);

// zip compress
int Compress(const unsigned char* pInData,unsigned long lInDataSize,
			 unsigned char* pOutBuf,unsigned long* plOutDataSize,int level);

// zip uncompress
int  DeCompress(const unsigned char * pInData,unsigned long lInDataSize,
				unsigned char * pOutBuf,unsigned long* lOutDataSize);


#endif		//__packagefile__