#ifndef		__packagefile__
#define		__packagefile__
//SM ����ʱȥ������ĺ궨��
//#ifndef ZLIB_WINAPI
//#define ZLIB_WINAPI
//#endif

////////////////////////////////////////////////////////
//Fun:ʵ�ִ������
//Create Time:2004.11.1
//Author:Joe
////////////////////////////////////////////////////////

class CClientResource;

struct tagFileIndex
{
	char	szName[256];
	ulong	dwOffset;
	ulong	dwSize;							//��Ӳ���ϵĴ�С��δѹ������ԭʼ��С��ѹ���˾���ѹ�����С
	ulong	dwOrginSize;					//�ļ�ԭʼ��С��δѹ��ǰsize
	ulong	dwValidSize;					//��ǰ��Ч���ļ���С����Ҫ���ڶϵ�������¼�Ѿ����ص��ļ���С
	ulong	dwCrc32;
	ulong   dwCompressType;					/// 0 δѹ�� 1 ѹ��
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
		ulong dwMaxIndexNum = 100000,ulong dwMaxEmptyIndexNum=1);
	~CPackage();

	ulong	GetID()				{return m_dwTypeID;}
	void	SetID(ulong dwId)	{m_dwTypeID = dwId;}

	bool	Open( const char* pszPackage,bool bUpdate=true );
	bool	OpenForUpdate( const char* pszPackage);

	bool	Create(const char* pszPackage);
	ulong	AddFile(const char* pszFile ,ulong &dwCrc32,
					ulong dwCompress, ulong &dwOrginSize);

	ulong	AddFile(const char* strFileName,uchar* pBuf,int iSize,
					ulong &dwCrc32,ulong dwCompress, ulong &dwOrginSize);

	ulong	AddFile(FILE* pf ,const char* strFileName,ulong &dwCrc32,
					ulong dwCompress, ulong &dwOrginSize);

	ulong	DelFile(const char* pszFile);																	//ɾ�������ļ�

	ulong	AddFileFromPackage(	CPackage* pPackage,const char* pszFile,
								ulong dwCompress);

	ulong	AdjustFile(const char* pszFile,ulong dwCompress);												//�ڲ������ļ�

	ulong	ExtractToFile(const char* pszFile,string strFolder="");											//��ȡ���ļ�

	bool ExtractToBuf(const char* pszFile,
        uchar *& pBuf,uint iSize,
        ulong& dwCrc32,
        ulong& dwCompressType,
        ulong &dwOrginSize,
        ulong &dwValidSize);	// ��ȡ������

	bool ExtractToBuf(const char* pszFile,
        uchar *& pBuf,
        uint iSize,
        tagFileIndex& FileIndex);														//��ȡ������

	long	GetFileSize(const char* pszFile);																//�õ��ļ���С

	//bool	WriteIndex();			//����
	bool	WriteIndexEx();
	void	Crypto( void* pBuf, ulong dwSize);

	void	Release();

	const INDEXOFFILES* GetIndexs()	{return & m_Indexs;	}

	void	OpenFileHandle(const char* pszPackage);
	void	CloseFileHandle();
	ulong	WriteData(tagFileIndex& FileIndex,const void* pBuf);									//�����д������
	ulong	WriteDataEx(tagFileIndex& FileIndex,const void* pBuf);									//�����д������

	//��Կͻ��˵�����д���ݲ���
public:
	bool	ClearData();
	ulong	AddFile(const char* strFileName,int nSize,ulong dwCrc32,
					ulong dwCompress, ulong dwOrginSize);

	ulong	AddFileData(const char* strFileName,uchar* pBuf,
						ulong& nValidSize,ulong nSize);
	FILE*	GetFileHandle()		{return m_pFile;}

	bool	GetFirstFileInfo(tagFileIndex& FileIndex);

private:
	void	AddEmptyPart(tagFileIndex& FileIndex);													//���һ��������
	bool	GetEmptyPart(ulong dwSize,tagFileIndex& FileIndex);										//���ݴ�С�õ�һ��������

private:
	ulong			m_dwMaxIndexNum;
	ulong			m_dwMaxEmptyIndexNum;
	
	ulong			m_dwIndexHeadSize;								//�ļ�����ͷ��С	
	ulong			m_dwEmptyIndexHeadSize;							//���ļ�����ͷ�Ĵ�С

	ulong			m_dwTypeID;										//��ID
	FILE*			m_pFile;
	string			m_strWorkPath;
	string			m_strPackName;

	INDEXOFFILES	m_Indexs;										//��ǰ�����ļ�����
	IndexEmptys		m_Emptys;										//��ǰ������Ŀ¼
	ulong			m_dwOffset;										//��ǰ���������ļ����ݵĵ�ƫ����

	CClientResource*		m_pOriginResource;						//�ͻ�����Դ��
};

// Lzo1x compress
void CompressData(const uchar* pInData,ulong lInDataSize,
				  uchar* pOutBuf,ulong* plOutDataSize,bool bFast);

// Lzo1x uncompress
void DeCompressData(const uchar * pInData,ulong lInDataSize,
					uchar * pOutBuf,ulong* lOutDataSize);

// zip compress
int Compress(const uchar* pInData,ulong lInDataSize,
			 uchar* pOutBuf,ulong* plOutDataSize,int level);

// zip uncompress
int  DeCompress(const uchar * pInData,ulong lInDataSize,
				uchar * pOutBuf,ulong* lOutDataSize);


#endif		//__packagefile__