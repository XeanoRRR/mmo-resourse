
#include "StdAfx.h"
#include "package.h"
#include "ClientResource.h"
#include "FilesInfo.h"
#include "../ziplib/lzo1x.h"
#include "../ziplib/zlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Lzo1x compress
void CompressData(const uchar* pInData,ulong lInDataSize,
				  uchar* pOutBuf,ulong* plOutDataSize,bool bFast){

	lzo_init();

	if(bFast){
		lzo_byte *lzowrkmem =(lzo_bytep) new uchar[LZO1X_1_MEM_COMPRESS]; //(lzo_bytep)_aligned_malloc(LZO1X_1_MEM_COMPRESS,16384);
		lzo1x_1_compress(pInData,lInDataSize,pOutBuf,(lzo_uint*)plOutDataSize,lzowrkmem);
		delete[] lzowrkmem;
		//_aligned_free(lzowrkmem);
	}
	else{
		lzo_byte *lzowrkmem =(lzo_bytep) new uchar[LZO1X_999_MEM_COMPRESS];//(lzo_bytep)_aligned_malloc(LZO1X_999_MEM_COMPRESS,16384);
		lzo1x_999_compress(pInData,lInDataSize,pOutBuf,(lzo_uint*)plOutDataSize,lzowrkmem);
		delete[] lzowrkmem;
		//_aligned_free(lzowrkmem);
	}
}

// Lzo1x uncompress
void DeCompressData(const uchar * pInData,ulong lInDataSize,
					uchar * pOutBuf,ulong* lOutDataSize)
{
	lzo_init();
	lzo1x_decompress(pInData,lInDataSize,pOutBuf,(lzo_uint*)lOutDataSize,NULL);
	//assert(LZO_E_OK==lzo1x_decompress_safe(pInData,lInDataSize,pOutBuf,(lzo_uint*)lOutDataSize,NULL));
}

// zip compress
int Compress(const uchar* pInData,ulong lInDataSize,
			 uchar* pOutBuf,ulong* plOutDataSize,int level)
{
	return compress2(pOutBuf,plOutDataSize, pInData,lInDataSize, level);
}

// zip uncompress
int  DeCompress(const uchar * pInData,ulong lInDataSize,
				uchar * pOutBuf,ulong* lOutDataSize)
{
	return uncompress(pOutBuf, lOutDataSize, pInData, lInDataSize);
}


CPackage::CPackage(CClientResource* pClientResource,string& strWorkPath,
				   ulong dwMaxIndexNum,ulong dwMaxEmptyIndexNum)
{ 
	m_pOriginResource = pClientResource;
	m_strWorkPath=strWorkPath;m_pFile = NULL ;

	m_dwMaxIndexNum = dwMaxIndexNum;
	m_dwMaxEmptyIndexNum = dwMaxEmptyIndexNum;
	//10W���ļ�
	m_dwIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxIndexNum;
	m_dwEmptyIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxEmptyIndexNum;

	//д�ļ����ݵ�ƫ����
	m_dwOffset = 4+4+4+m_dwIndexHeadSize+4+4+m_dwEmptyIndexHeadSize;
}
CPackage::~CPackage()
{
	CloseFileHandle();
}

void	CPackage::OpenFileHandle(const char* pszPackage)
{
	string strFullFileName = m_strWorkPath+"\\"+pszPackage+".dp";
	m_pFile = fopen(strFullFileName.c_str(), "r+b");

	if(!m_pFile)
	{
		Log4c::Warn(ROOT_MODULE, "���ļ� %s ����",pszPackage);
	}
	m_strPackName = pszPackage;
}

void	CPackage::CloseFileHandle()
{
	if (m_pFile)
	{
		fclose(m_pFile) ;
		m_pFile=NULL;
	}
}

bool	CPackage::OpenForUpdate( const char* pszPackage)
{
	string strFullFileName = m_strWorkPath+"\\"+pszPackage+".dp";
	m_pFile = fopen(strFullFileName.c_str(), "r+b");

	if(!m_pFile)
	{
		return Create(pszPackage);
	}
	if( Open(pszPackage,false) == false )
	{
		return Create(pszPackage);
	}
	return true;
}

bool CPackage::Open(const char* pszPackage ,bool bUpdate)
{
	CloseFileHandle();
	OpenFileHandle(pszPackage);

	if (!m_pFile)
		return false;

	try
	{
		fseek(m_pFile, 0, SEEK_SET);
		//��д���ݵĵ�ַƫ����
		fread(&m_dwOffset, sizeof(ulong), 1, m_pFile);
		//���ļ�����ͷ��С
		fread(&m_dwIndexHeadSize, sizeof(ulong), 1, m_pFile);
		m_dwMaxIndexNum = m_dwIndexHeadSize/sizeof(tagFileIndex);
		//���ļ�������
		ulong dwIndexCount;
		fread(&dwIndexCount, sizeof(ulong), 1, m_pFile);
		//�ж��ļ������������
		if(dwIndexCount > m_dwMaxIndexNum) return false;

		m_Indexs.clear();
		for (uint i = 0; i < dwIndexCount; ++i)
		{
			tagFileIndex index;
			fread(&index, sizeof(tagFileIndex), 1, m_pFile);
			Crypto(&index, sizeof(index));

			_strlwr(index.szName );

			m_Indexs[index.szName] = index;
			if(bUpdate && m_pOriginResource)
			{
				string strName(index.szName);
				m_pOriginResource->GetFilesInfo()->UpdateFileInfo(	strName,index.dwCrc32,
																	index.dwSize,index.dwOrginSize,
																	index.dwValidSize,(ulong)1,
																	index.dwCompressType);
			}
		}

		fseek(m_pFile, 4+4+4+m_dwIndexHeadSize, SEEK_SET);
		//�����ļ�����ͷ��С
		fread(&m_dwEmptyIndexHeadSize,sizeof(ulong), 1, m_pFile);
		m_dwMaxEmptyIndexNum = m_dwEmptyIndexHeadSize/sizeof(tagFileIndex);
		//��ȡ����������
		fread(&dwIndexCount, sizeof(ulong), 1, m_pFile);
		if(dwIndexCount>m_dwMaxEmptyIndexNum)
			return false;
		m_Emptys.clear();
		for (uint i = 0; i < dwIndexCount; ++i)
		{
			tagFileIndex Empty;
			fread(&Empty, sizeof(tagFileIndex), 1, m_pFile);
			Crypto(&Empty, sizeof(Empty));
			m_Emptys.insert(Empty);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}




bool CPackage::Create(const char* pszPackage)
{
	string strFullFileName = m_strWorkPath+"\\"+pszPackage+".dp";
	m_pFile = fopen(strFullFileName.c_str(), "w+b");
	if (m_pFile)
	{
		m_Indexs.clear();
		m_Emptys.clear();

		//10W���ļ�
		m_dwIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxIndexNum;
		m_dwEmptyIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxEmptyIndexNum;

		//д�ļ����ݵ�ƫ����
		m_dwOffset = 4+4+4+m_dwIndexHeadSize+4+4+m_dwEmptyIndexHeadSize;

		WriteIndexEx();

		m_strPackName = pszPackage;
		return true;
	}
	else
	{
		Log4c::Warn(ROOT_MODULE, "�����ļ� %s ����",pszPackage);
	}
	return false;
}

//����������ļ��е�����
bool	CPackage::ClearData()
{
	m_Indexs.clear();
	m_Emptys.clear();
	
	//10W���ļ�
	m_dwIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxIndexNum;
	m_dwEmptyIndexHeadSize = sizeof(tagFileIndex)*m_dwMaxEmptyIndexNum;
	//д�ļ����ݵ�ƫ����
	m_dwOffset = 4+4+4+m_dwIndexHeadSize+4+4+m_dwEmptyIndexHeadSize;

	fseek(m_pFile,m_dwOffset,SEEK_SET);
	fflush(m_pFile);

	WriteIndexEx();
	return true;
}

ulong CPackage::AddFile(const char* pszFile,ulong &dwCrc32,
						ulong dwCompress, ulong &dwOrginSize)
{
	int nResult;
	tagFileIndex index;
	index.dwCrc32 = dwCrc32;
	strcpy_s(index.szName, 256, pszFile);
	string strFullFileName = m_strWorkPath+pszFile;

	FILE* fp = fopen(strFullFileName.c_str(), "rb");
	if(!fp)
	{
		Log4c::Warn(ROOT_MODULE,"���ļ� %s ����",pszFile);
		return 0;
	}
	assert(fp);
	if(!fp)
	{
		Log4c::Warn(ROOT_MODULE, "fopen�ļ� %s ����",pszFile);
		return 0;
	}

	nResult = fseek(fp, 0, SEEK_END);
	if(nResult)
	{
		Log4c::Warn(ROOT_MODULE, "fseek�ļ� %s ����",pszFile);
		return 0;
	}


	nResult = index.dwSize = ftell(fp);
	if(nResult == -1L)
	{
		Log4c::Warn(ROOT_MODULE, "ftell�ļ� %s ����",pszFile);
		return 0;
	}

	dwOrginSize = index.dwSize;

	nResult = fseek(fp, 0, SEEK_SET);
	if(nResult)
	{
		Log4c::Warn(ROOT_MODULE, "fseek�ļ� %s ����",pszFile);
		return 0;
	}
	
	if ( !ENABLEBIT(dwCompress,NOCOMPRESS) )
	{
		index.dwCompressType = dwCompress;
		if(index.dwSize > 0)
		{
			uchar *lpszIn = new uchar[index.dwSize];
			uchar *lpszOut = new uchar[index.dwSize*2];
			fread(lpszIn, index.dwSize, 1, fp);
			ulong nOutSize = index.dwSize*2;

			if( ENABLEBIT(dwCompress,0x4) )
				Compress(lpszIn, index.dwSize, lpszOut, &nOutSize, false);
			else
				CompressData(lpszIn, index.dwSize, lpszOut, &nOutSize, false);
			

			index.dwOrginSize = index.dwSize;
			dwOrginSize = index.dwOrginSize;
			index.dwSize = nOutSize;
			index.dwValidSize = nOutSize;
			//CCrc32Static::DataCrc32(lpszOut, nOutSize, index.dwCrc32);
			//dwCrc32 = index.dwCrc32;

			nResult = WriteData(index,lpszOut);

			delete []lpszIn;
			delete []lpszOut;
		}
		else
		{
			index.dwSize=0;
			uchar* pBuf = NULL;
			index.dwOrginSize = index.dwSize;
			index.dwValidSize = index.dwSize;
			nResult = WriteData(index,pBuf);
		}
	}
	else
	{
		index.dwCompressType = 1;
		uchar* pBuf = new uchar[index.dwSize];
		fread(pBuf, index.dwSize, 1, fp);
		index.dwOrginSize = index.dwSize;
		index.dwValidSize = index.dwSize;
		nResult = WriteData(index,pBuf);
		delete []pBuf;
	}

	fclose(fp);	
	if(nResult == false)
		return false;
	return index.dwSize;
}


ulong	CPackage::AddFile(FILE* pf ,const char* strFileName,ulong &dwCrc32,
						  ulong dwCompress, ulong &dwOrginSize)
{
	if(pf==NULL)	return 0;

	int nRet=0;
	tagFileIndex index;
	index.dwCrc32 = dwCrc32;
	strcpy_s(index.szName, 256, strFileName);

	nRet = fseek(pf, 0, SEEK_END);
	if(nRet)
	{
		Log4c::Warn(ROOT_MODULE, "fseek�ļ� %s ����",strFileName);
		return 0;
	}

	nRet = index.dwSize = ftell(pf);
	if(nRet == -1L)
	{
		Log4c::Warn(ROOT_MODULE, "ftell�ļ� %s ����",strFileName);
		return 0;
	}

	dwOrginSize = index.dwSize;

	nRet = fseek(pf, 0, SEEK_SET);
	if(nRet)
	{
		Log4c::Warn(ROOT_MODULE, "fseek�ļ� %s ����",strFileName);
		return 0;
	}

	if( !ENABLEBIT(dwCompress,NOCOMPRESS) )
	{
		index.dwCompressType = dwCompress;	

		if(index.dwSize>0)
		{
			uchar *lpszIn = new uchar[index.dwSize];
			uchar *lpszOut = new uchar[index.dwSize*2];
			fread(lpszIn, index.dwSize, 1, pf);
			ulong nOutSize = index.dwSize*2;
			
			if( ENABLEBIT(dwCompress,0x4) )
				Compress(lpszIn, index.dwSize, lpszOut, &nOutSize, 1);
			else
				CompressData(lpszIn, index.dwSize, lpszOut, &nOutSize, false);


			index.dwOrginSize=index.dwSize;
			dwOrginSize = index.dwOrginSize;
			index.dwSize = nOutSize;
			index.dwValidSize = nOutSize;
			//CCrc32Static::DataCrc32(lpszOut, nOutSize, index.dwCrc32);
			//dwCrc32 = index.dwCrc32;

			nRet = WriteData(index,lpszOut);

			delete []lpszIn;
			delete []lpszOut;
		}

		else
		{
			index.dwSize=0;
			uchar* pBuf = NULL;
			index.dwOrginSize = index.dwSize;
			index.dwValidSize = index.dwSize;
			nRet = WriteData(index,pBuf);
		}
	}
	else
	{
		index.dwCompressType = 1;
		uchar* pBuf = new uchar[index.dwSize];
		fread(pBuf, index.dwSize, 1, pf);

		index.dwOrginSize = index.dwSize;
		index.dwValidSize = index.dwSize;
		nRet = WriteData(index,pBuf);
		delete []pBuf;
	}

	if(nRet == false)
		return false;
	return index.dwSize;
}

//�����ѹ��������
ulong	CPackage::AddFile(const char* strFileName,int nSize,ulong dwCrc32,
						  ulong dwCompress, ulong dwOrginSize)
{
	tagFileIndex index;
	index.dwCrc32 = dwCrc32;
	strcpy_s(index.szName, 256, strFileName);
	index.dwCompressType=dwCompress;
	index.dwSize = nSize;
	index.dwOrginSize = dwOrginSize;
	index.dwValidSize = 0;

	uchar* pBuf = new uchar[index.dwSize];
	memset(pBuf,0,index.dwSize);

	index.dwValidSize = 0;
	int nRet =WriteData(index,pBuf);

	delete []pBuf;
	if(nRet == false)
		return false;
	return index.dwSize;
}
//����ѹ����������д����
ulong	CPackage::AddFileData(const char* strFileName,uchar* pBuf,ulong& nValidSize,
							  ulong nSize)
{
	size_t nRet;
	itIndex it = m_Indexs.find(strFileName);
	if(it == m_Indexs.end())	return 0;
	tagFileIndex& index = (*it).second;

	//�ļ�������С����
	//index.dwValidSize = nValidSize;
	if(index.dwValidSize+nSize>index.dwSize)	return 0;
	//index.dwCompressType = dwCompress;

	ulong dwOffset = index.dwOffset+index.dwValidSize;
	nRet = fseek(m_pFile,dwOffset,SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fseek�ļ� %s ����",strFileName);
		return 0;
	}

	nRet = fwrite(pBuf, nSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE, "fwrite�ļ� %s ����",strFileName);
		return 0;
	}

	nRet = fflush(m_pFile);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE,"fflush�ļ� %s ����",strFileName);
		return 0;
	}
	index.dwValidSize += nSize;
	nValidSize = index.dwValidSize;
	return index.dwValidSize;
}
//ɾ�������ļ�
ulong	CPackage::DelFile(const char* pszFile)														
{
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end())	return 0;

	//���ӿ�����
	tagFileIndex temptFileIndex = {"",(*it).second.dwOffset,(*it).second.dwSize,0,0,0};
	AddEmptyPart(temptFileIndex);
	m_Indexs.erase(it);
	return 0;
}
//�Ӱ�����ȡ�ļ�����ð� 
ulong CPackage::AddFileFromPackage(CPackage* pPackage,const char* pszFile ,ulong dwCompress)
{
	if(pPackage==NULL || pszFile==NULL)	return 0;
	return 0;
}

void CPackage::Crypto(void* pBuf, ulong dwSize)
{
	for (uint i = 0; i < dwSize; ++i)
	{
		((uchar*)pBuf)[i] ^= 0xFF;
	}
}

ulong CPackage::ExtractToFile(const char* pszFile,string strFolder)
{
	ulong dwSize = 0;
	if(m_pFile==NULL || pszFile == NULL)	return dwSize;
	
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end())	return dwSize;

	tagFileIndex *pIndex = &(*it).second;

	size_t nRet = fseek(m_pFile,pIndex->dwOffset,SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fseek���ļ�����");
		return 0;
	}

	uchar* pBuf = new uchar[pIndex->dwSize];
	nRet = fread(pBuf, pIndex->dwSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE, "�����ļ�����");
		delete []pBuf;
		return 0;
	}

	string strTemptFolder=strFolder;
	if(strTemptFolder=="")
	{
		strTemptFolder = m_strWorkPath;
		strTemptFolder += pIndex->szName;	
	}
	else
	{
		strTemptFolder = strFolder;
	}

	FILE* pf = fopen(strTemptFolder.c_str(),"wb");
	if(pf)
	{
		if( !ENABLEBIT(pIndex->dwCompressType,NOCOMPRESS) )
		{
			if(pIndex->dwSize > 0)
			{
				uchar* pOutBuf = new uchar[pIndex->dwOrginSize+1];
				ulong dwOutSize=pIndex->dwOrginSize+1;

				if( ENABLEBIT(pIndex->dwCompressType,0x4) )
					DeCompress(pBuf, pIndex->dwSize, pOutBuf, &dwOutSize);
				else
					DeCompressData(pBuf, pIndex->dwSize, pOutBuf, &dwOutSize);

				nRet = fwrite(pOutBuf, dwOutSize, 1, pf);
				if(nRet != 1)
				{
					Log4c::Warn(ROOT_MODULE, "fwrite�ļ� %s ����",strTemptFolder.c_str());
					delete []pBuf;
					return 0;
				}

				delete []pOutBuf;
				fflush(m_pFile);
				dwSize = dwOutSize;
			}
		}
		else
		{
			size_t nRet = fwrite(pBuf, pIndex->dwSize, 1, pf);
			if(nRet != 1)
			{
				Log4c::Warn(ROOT_MODULE, "fwrite���ļ�����");
				delete []pBuf;
				return 0;
			}
			nRet = fflush(m_pFile);
			if(nRet != 0)
			{
				Log4c::Warn(ROOT_MODULE, "fflush���ļ�����");
				delete []pBuf;
				return 0;
			}
			dwSize=pIndex->dwSize;
		}
		fclose(pf);
	}
	else
	{
        delete []pBuf;
		Log4c::Warn(ROOT_MODULE, "���ļ� %s ����",strTemptFolder.c_str());
		return 0;
	}

	delete []pBuf;
	return dwSize;
}

//��ȡ������
bool CPackage::ExtractToBuf(const char* pszFile,
                            uchar *& pBuf,
                            uint iSize,
							ulong& dwCrc32,
                            ulong& dwCompressType,
							ulong &dwOrginSize,
                            ulong &dwValidSize)
{
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end())	return false;
	if((*it).second.dwSize > iSize)	return false;

	dwCompressType = (*it).second.dwCompressType;
	dwCrc32 = (*it).second.dwCrc32;
	dwValidSize = (*it).second.dwValidSize;
	dwOrginSize = (*it).second.dwOrginSize;

	size_t nRet = fseek(m_pFile,(*it).second.dwOffset,SEEK_SET);
	if(nRet != 0)
	{
        Log4c::Warn(ROOT_MODULE,"fseek�ļ� %s ����",pszFile);
		return 0;
	}
	nRet = fread(pBuf, (*it).second.dwSize, 1, m_pFile);
	if(nRet != 1)
	{
        Log4c::Warn(ROOT_MODULE,"fread�ļ� %s ����",pszFile);
		return 0;
	}

	return true;
}

//��ȡ������
bool	CPackage::ExtractToBuf(const char* pszFile,uchar *& pBuf,
							   uint iSize,tagFileIndex& FileIndex)
{
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end())	return false;
	if((*it).second.dwSize > iSize)	return false;

	FileIndex = (*it).second;
	size_t nRet = fseek(m_pFile,(*it).second.dwOffset,SEEK_SET);
	if(nRet != 0)
	{
        Log4c::Warn(ROOT_MODULE,"fseek�ļ� %s ����",pszFile);
		return 0;
	}
	nRet = fread(pBuf, (*it).second.dwSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE,"fread�ļ� %s ����",pszFile);
		return 0;
	}
	return true;
}

//�õ��ļ���С
long CPackage::GetFileSize(const char* pszFile)
{
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end())
		return 0;
	else
		return (*it).second.dwSize;
}

bool	CPackage::GetFirstFileInfo(tagFileIndex& FileIndex)
{
	if(m_Indexs.size() == 0)	return false;
	FileIndex = m_Indexs.begin()->second;
	return true;
}

void	CPackage::Release()
{
	fclose(m_pFile);
}

//���һ��������
void	CPackage::AddEmptyPart(tagFileIndex& FileIndex)
{
	if(m_Emptys.size() < m_dwMaxEmptyIndexNum)
		m_Emptys.insert(FileIndex);
}

//���ݴ�С�õ�һ��������
bool	CPackage::GetEmptyPart(ulong dwSize,tagFileIndex& FileIndex)
{
	itEmpty it =m_Emptys.begin();
	for(;it != m_Emptys.end();it++)
	{
		if((*it).dwSize>=dwSize)
		{
			FileIndex = (*it);
			m_Emptys.erase(it);
			return true;
		}
	}
	return false;
}


bool	CPackage::WriteIndexEx()
{
	if(!m_pFile)
		return false;

	//����20M�ռ�
	uchar *TemptBuff = new uchar[4+4+4+m_dwIndexHeadSize+4+4+m_dwEmptyIndexHeadSize];
	int nPos = 0;
	//д���ݵĵ�ַƫ����
	memcpy(&TemptBuff[nPos],&m_dwOffset,sizeof(ulong));
	nPos += sizeof(ulong);
	//д�ļ�����ͷ��С
	memcpy(&TemptBuff[nPos],&m_dwIndexHeadSize,sizeof(ulong));
	nPos += sizeof(ulong);
	//д�ļ���������
	size_t dwCount = m_Indexs.size();
	memcpy(&TemptBuff[nPos],&dwCount,sizeof(ulong));
	nPos += sizeof(ulong);
	for(itIndex it = m_Indexs.begin(); it != m_Indexs.end(); it++)
	{
		tagFileIndex index = (*it).second;
		Crypto(&index, sizeof(index));
		memcpy(&TemptBuff[nPos],&index, sizeof(tagFileIndex));
		nPos += sizeof(tagFileIndex);
	}

	nPos = 4+4+4+m_dwIndexHeadSize;
	//д������ͷ��С
	memcpy(&TemptBuff[nPos],&m_dwEmptyIndexHeadSize,sizeof(ulong));
	nPos += sizeof(ulong);
	//д����������
	dwCount = m_Emptys.size();
	memcpy(&TemptBuff[nPos],&dwCount,sizeof(ulong));
	nPos += sizeof(ulong);
	for (itEmpty it = m_Emptys.begin(); it != m_Emptys.end(); it++)
	{
		tagFileIndex index = (*it);
		Crypto(&index, sizeof(index));
		memcpy(&TemptBuff[nPos],&index, sizeof(tagFileIndex));
		nPos += sizeof(tagFileIndex);
	}
	
	size_t nRet = fseek(m_pFile, 0, SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fseek���ļ�����");
		return 0;
	}
	//д�ļ�����
	nRet = fwrite(TemptBuff, nPos, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE, "fwrite���ļ�����");
		delete[] TemptBuff;
		return 0;
	}

	nRet = fflush(m_pFile);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fflush���ļ�����");
		delete[] TemptBuff;
		return 0;
	}
	delete[] TemptBuff;
	return true;
}

//�ڲ������ļ�
ulong	CPackage::AdjustFile(const char* pszFile,ulong dwCompress)
{
	itIndex it = m_Indexs.find(pszFile);
	if(it == m_Indexs.end()) return false;

	if((*it).second.dwCompressType == dwCompress)	return false;

	//���ڴ�С���ļ�ʵ�ʴ�С����ʱ�򣬲��ܵ���
	if((*it).second.dwSize != (*it).second.dwValidSize)	return false;

	tagFileIndex index = (*it).second;
	index.dwCompressType = dwCompress;

	uchar* pBuf = new uchar[(*it).second.dwSize];

	size_t nRet = fseek(m_pFile,(*it).second.dwOffset,SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fseek���ļ�����");
		return 0;
	}
	nRet = fread(pBuf, (*it).second.dwSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE, "fread���ļ�����");
		return 0;
	}

	DelFile(pszFile);

	if( !ENABLEBIT(dwCompress,NOCOMPRESS) && ENABLEBIT(index.dwCompressType,NOCOMPRESS) )
	{
		if(index.dwSize > 0)
		{
			uchar *lpszOut = new uchar[index.dwSize*2];
			ulong nOutSize = index.dwSize*2;

			if( ENABLEBIT(dwCompress,0x4) )//zipѹ��
				Compress(pBuf, index.dwSize, lpszOut, &nOutSize, 1);
			else
				CompressData(pBuf, index.dwSize, lpszOut, &nOutSize, false);

			index.dwOrginSize = index.dwSize;
			index.dwValidSize = nOutSize;
			index.dwSize = nOutSize;
			nRet = WriteData(index,lpszOut);

			delete []lpszOut;
		}
		else
		{
			index.dwSize=0;
			uchar* pBuf = NULL;
			index.dwValidSize=index.dwSize;
			nRet = WriteData(index,pBuf);
		}
	}
	else if((dwCompress&0x1) && !(index.dwCompressType&0x1))
	{
		uchar* pOutBuf = new uchar[index.dwOrginSize];
		ulong dwOutSize=index.dwOrginSize;

		if( ENABLEBIT(index.dwCompressType,0x4) )
			DeCompress(pBuf, index.dwSize, pOutBuf, &dwOutSize);
		else
			DeCompressData(pBuf, index.dwSize, pOutBuf, &dwOutSize);
		index.dwSize = dwOutSize;
		index.dwValidSize = dwOutSize;

		nRet = WriteData(index,pOutBuf);

		delete []pOutBuf;
	}
	//ѹ��֮����л���ʱ������
	else
	{
	}

	delete []pBuf;
	if(nRet == false)
		return false;
	return index.dwSize;
}

//�����д������
ulong	CPackage::WriteData(tagFileIndex& FileIndex,const void* pBuf)
{
	if(pBuf==NULL || FileIndex.dwSize <= 0)	return false;

	ulong dwOffset = 0;
	bool bFlag = false;
	//������д��ڸ��ļ������
	itIndex it = m_Indexs.find(FileIndex.szName);
	if(it != m_Indexs.end())
	{
		if(FileIndex.dwSize <= (*it).second.dwSize)
		{
			dwOffset = (*it).second.dwOffset;
			bFlag = true;
		
			if((*it).second.dwSize > FileIndex.dwSize)
			{
				//����µĿ�����
				tagFileIndex temptFileIndex1 = {"",dwOffset+FileIndex.dwSize,
					((*it).second.dwSize-FileIndex.dwSize),0,0,0};
				AddEmptyPart(temptFileIndex1);
			}
		}
		else
		{
			//���Ѿ��е��ļ�������ӵ�������
			tagFileIndex temptFileIndex1 = {"",(*it).second.dwOffset,(*it).second.dwSize,0,0,0};
			AddEmptyPart(temptFileIndex1);
		}
	}
	else
	{
		//�ж��������
		if(m_Indexs.size() >= m_dwMaxIndexNum)
		{
			Log4c::Warn(ROOT_MODULE, "���<%s>��д���ļ�<%s>������������ļ�����%d.",
				m_strPackName.c_str(),FileIndex.szName,m_dwMaxIndexNum);
			return false;
		}

		//Ѱ�Ұ����Ƿ��п�����
		tagFileIndex temptFileIndex;
		if(GetEmptyPart(FileIndex.dwSize,temptFileIndex))
		{
			dwOffset = temptFileIndex.dwOffset;
			bFlag = true;
		
			if(temptFileIndex.dwSize > FileIndex.dwSize)
			{
				//����µĿ�����
				tagFileIndex temptFileIndex2 = {"",dwOffset+FileIndex.dwSize,
					temptFileIndex.dwSize-FileIndex.dwSize,0,0,0};
				AddEmptyPart(temptFileIndex2);
			}
		}
	}

	if(bFlag == false)
	{
		dwOffset = m_dwOffset;
		m_dwOffset += FileIndex.dwSize;
	}

	FileIndex.dwOffset = dwOffset;
	_strlwr(FileIndex.szName);
	m_Indexs[FileIndex.szName] = FileIndex;

	size_t nRet = fseek(m_pFile,dwOffset,SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fread���ļ�����");
		return false;
	}
	nRet = fwrite(pBuf, FileIndex.dwSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE, "fwrite���ļ�����");
		return false;
	}
	nRet = fflush(m_pFile);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fflush���ļ�����");
		return false;
	}

	return FileIndex.dwSize;
}

ulong	CPackage::WriteDataEx(tagFileIndex& FileIndex,const void* pBuf)
{
	if(pBuf==NULL || FileIndex.dwSize <= 0)	return false;

	ulong dwOffset = 0;
	bool bFlag = false;
	//������д��ڸ��ļ������
	itIndex it = m_Indexs.find(FileIndex.szName);
	if(it != m_Indexs.end())
	{
		if(FileIndex.dwSize <= (*it).second.dwSize)
		{
			dwOffset = (*it).second.dwOffset;
			bFlag = true;
		
			if((*it).second.dwSize > FileIndex.dwSize)
			{
				//����µĿ�����
				tagFileIndex temptFileIndex1 = {"",dwOffset+FileIndex.dwSize,
					((*it).second.dwSize-FileIndex.dwSize),0,0,0};
				AddEmptyPart(temptFileIndex1);
			}
		}
		else
		{
			//���Ѿ��е��ļ�������ӵ�������
			tagFileIndex temptFileIndex1 = {"",(*it).second.dwOffset,(*it).second.dwSize,0,0,0};
			AddEmptyPart(temptFileIndex1);
		}
	}
	else
	{
		//�ж��������
		if(m_Indexs.size() >= m_dwMaxIndexNum)
		{
			Log4c::Warn(ROOT_MODULE,"���<%s>��д���ļ�<%s>������������ļ�����%d.",
				m_strPackName.c_str(),FileIndex.szName,m_dwMaxIndexNum);
			return false;
		}
		//Ѱ�Ұ����Ƿ��п�����
		tagFileIndex temptFileIndex;
		if(GetEmptyPart(FileIndex.dwSize,temptFileIndex))
		{
			dwOffset = temptFileIndex.dwOffset;
			bFlag = true;
		
			if(temptFileIndex.dwSize > FileIndex.dwSize)
			{
				//����µĿ�����
				tagFileIndex temptFileIndex2 = {"",dwOffset+FileIndex.dwSize,
					temptFileIndex.dwSize-FileIndex.dwSize,0,0,0};
				AddEmptyPart(temptFileIndex2);
			}
		}
	}

	if(bFlag == false)
	{
		dwOffset = m_dwOffset;
		m_dwOffset += FileIndex.dwSize;
	}

	FileIndex.dwOffset = dwOffset;
	_strlwr(FileIndex.szName);
	m_Indexs[FileIndex.szName] = FileIndex;

	WriteIndexEx();

	size_t nRet = fseek(m_pFile,dwOffset,SEEK_SET);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fread���ļ�����");
		return false;
	}
	nRet = fwrite(pBuf, FileIndex.dwSize, 1, m_pFile);
	if(nRet != 1)
	{
		Log4c::Warn(ROOT_MODULE,"fwrite���ļ�����");
		return false;
	}
	nRet = fflush(m_pFile);
	if(nRet != 0)
	{
		Log4c::Warn(ROOT_MODULE, "fflush���ļ�����");
		return false;
	}

	return FileIndex.dwSize;
}