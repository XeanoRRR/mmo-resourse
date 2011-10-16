//********************************************
//	���ຯ��
//********************************************

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mmsystem.h>
#include <math.h>
#include <shlobj.h>
#include <Commdlg.h>
#include <assert.h>
#include "tools.h"
#include <sys\timeb.h>
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*********************
//�����
//int random(int nMax)
//{ 
//	//srand((unsigned)time(NULL));
//
//	int ret = rand()*nMax/RAND_MAX;
//	if (ret == nMax && ret > 0)
//		ret--;
//
//	return ret;
//}

int random(int nMax)
{
	if (nMax > RAND_MAX)
	{
		float ret = (float)rand() * ( (float)nMax / (float)RAND_MAX );
		ret += (float)rand() * ((float)nMax/(float)RAND_MAX) / (float)RAND_MAX;

		while ((int)ret == nMax && ret > 0)
		{
			ret = (float)rand() * ( (float)nMax / (float)RAND_MAX );
			ret += (float)rand() * ((float)nMax/(float)RAND_MAX) / (float)RAND_MAX;
		}
		return (int)ret;
	}
	else
	{
		int ret = rand()*nMax/RAND_MAX;

		while (ret == nMax && ret > 0)
		{
			ret = rand()*nMax/RAND_MAX;
		}
		return ret;
	}
}

//*************************
//��ʱ
void Delay(long time)
{
	static long old_clock, new_clock; //��ʱ����
	new_clock=old_clock=GetCurTickCount();

	MSG msg;
	while( new_clock < old_clock + time )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
            if (!GetMessage(&msg, NULL, 0, 0)) return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
		else if(1)
		{
			new_clock=GetCurTickCount();
			if( GetAsyncKeyState(VK_F4) )
			{
				return;
			}
		}
		else WaitMessage();	//�ȴ���Ϣ
	}
}

//***************************
//��ȡ�ļ�����
int GetFileLength(const char *name)
{
	if(name == NULL)	return 0;
	int fh,nbytes = 0;
	fh = _open( name, _O_RDONLY );
	if( fh== -1 )
	{
		//char s[32];
		//sprintf(s, "file %s not found!", name);
		//MessageBox(hWnd, s,"GetFileLength",MB_OK);
		return -1;
	}
	
	nbytes=_filelength(fh);
	_close(fh);
	

	/*CRFile* prfile = rfOpen(name):
	if(prfile)
	{
		nbytes = prfile->GetDatalen();
		rfClose(prfile);
	}*/
	return nbytes;
}


//**************************
//�ж��ļ��Ƿ����
bool CheckFile(const char *name)
{
	WIN32_FIND_DATA fdt;
	if( FindFirstFile(name, &fdt)==INVALID_HANDLE_VALUE )
		return false;
	else 
		return true;
}

//**************************
//��ʾ������Ϣ
void ShowMessage(const char *msg,...)
{
	va_list va;
	char str[256];

	va_start(va,msg);
	vsprintf(str,msg,va);
	va_end(va);

	MessageBox(NULL, str, "Message",MB_OK);
}

static char szDebugFileName[_MAX_PATH] = "debug.txt";
void InitialDebugFileName()
{
	//��ʼ��debugfilename����
	SYSTEMTIME st;
	GetLocalTime(&st);

	_snprintf(szDebugFileName,256,"log\\debug%d_%d_%d[%2.2d_%2.2d_%2.2d].txt",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	struct _finddata_t c_file;
	intptr_t hFile;
	bool bExist = ((hFile = _findfirst( "log", &c_file )) != -1L );
	if (!bExist)	
		CreateDirectory("log",NULL);
}

const char* GetDebugFileName()
{
	return szDebugFileName;
}
//*************************
//���������Ϣ
void PutDebugString(const char *msg)
{
	//static string filenam
	static int num=0;
	char t[128];

	FILE *fp;
	if( (fp=fopen(szDebugFileName,"a+")) == NULL )
	{
		//ShowMessage("Can't open Debug.txt (%s)", msg);
		assert(0);
		return;
	}
	
	num++;
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d):", num, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);
}

//*************************
//����ļ���Ϣ
void PutStringToFile(const char* filename, const char *msg)
{
	char strFile[MAX_PATH];
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(strFile, "log/%s_%04d-%02d-%02d.txt", filename, stTime.wYear, stTime.wMonth, stTime.wDay);

	//����һ��log�ı���Ŀ¼
	CreateDirectory( "log",	NULL);

	FILE *fp;
	if( (fp=fopen(strFile,"a+")) == NULL )
	{
	//	assert(0);
		return;
	}

	static int num=0;
	num++;

	char t[128];
	sprintf(t, "\n%d(%02d-%02d %02d:%02d:%02d):", num, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	fseek(fp, 0, SEEK_END);
	fwrite(t, lstrlen(t), 1, fp);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);
}

//*************************
//���log��Ϣ
void PutLogInfo(const char *msg)
{
	char strFile[MAX_PATH];
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	sprintf(strFile, "log/%04d-%02d-%02d.txt", stTime.wYear, stTime.wMonth, stTime.wDay);

	//����һ��log�ı���Ŀ¼
	CreateDirectory( "log",	NULL);

	FILE *fp;
	if( (fp=fopen(strFile, "a+")) == NULL )
	{
		return;
	}

	fseek(fp, 0, SEEK_END);
	fwrite(msg, lstrlen(msg), 1, fp);
	fclose(fp);
}

//*************************
//���ļ�����ȡ�ļ�·��
char *GetFilePath(const char *filename, char* out)
{
	int Len=lstrlen(filename);
	strcpy(out, filename);

	for(int i=Len-1; i>=0; i--)
	{
		if( out[i] == '\\' || out[i] == '/' )
			break;
		out[i]=0;
	}

	return out;
}

//���ļ�����ȡ�ļ���չ��
char *GetFileExtName(const char *filename, char* out)
{
	int Len=lstrlen(filename);
	out[0] = 0;

	for(int i=Len-1; i>=0; i--)
	{
		if( filename[i] == '.' )
		{
			strcpy(out, &filename[i]);
			return out;
		}
		
	}
	return out;
}

//*************************
//char to wchar
WCHAR* WChar(char *string)
{
	WCHAR *wchr;
	int wsize = MultiByteToWideChar(CP_ACP, NULL, string, lstrlen(string), NULL, 0);

	wchr=new WCHAR[wsize+1];
	wcsset(wchr, 0);
	MultiByteToWideChar(CP_ACP, NULL, string, lstrlen(string), wchr, wsize);
	wchr[wsize]=0;
	return wchr;
}

//*************************
//wchar to char
char* Char(WCHAR *string)
{
	char *chr;
	chr=new char[wcslen(string)+1];
	memset(chr, 0, wcslen(string)+1);
	for(unsigned int i=0; i<wcslen(string); i++)
	{
		chr[i]=(unsigned char)string[i];
	}
	return chr;
}

//ɾ��Ŀ¼
void DeleteDirectory(char* strDir)
{
	char strFile[MAX_PATH];
	sprintf(strFile, "%s\\*.*", strDir);
	
	// ɾ��Ŀ¼�е������ļ�
	BOOL result;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strFile, &FindFileData);
	if( hFind )
	{
		if( strcmp(FindFileData.cFileName, "..") != 0 && 
			strcmp(FindFileData.cFileName, ".") != 0 )
		{
			sprintf(strFile, "%s\\%s", strDir, FindFileData.cFileName);
			DeleteFile(strFile);
		}
		do
		{
			result = FindNextFile(hFind, &FindFileData);
			if( strcmp(FindFileData.cFileName, "..") != 0 && 
				strcmp(FindFileData.cFileName, ".") != 0 )
			{
				sprintf(strFile, "%s\\%s", strDir, FindFileData.cFileName);
				DeleteFile(strFile);
			}
		}while(result);
	}
    FindClose(hFind);

	// ɾ��Ŀ¼
	RemoveDirectory(strDir);
}

// ��ȡ·��
bool GetFileDialog(bool Load, const char* currentPath, char* strFile, const char* strFilter)
{
//	char tmp[256]="Script File (*.script)\0*.script\0All Files\0*.*\0";
	char tmp[256]="All Files\0*.*\0Script File (*.script)\0*.script\0";
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;	//AfxGetMainWnd()->GetSafeHwnd();
	ofn.hInstance = NULL;	//AfxGetInstanceHandle ();
	ofn.lpstrFilter = tmp;	// file types
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;		// index of first one
	ofn.nMaxCustFilter = 0;	
	ofn.lpstrFile = tmp;	// initial file suggested
	ofn.nMaxFile = 1024;
	ofn.lpstrTitle = NULL;		// dlg title
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = currentPath;   // initial path
	ofn.Flags = OFN_SHOWHELP | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_SHAREAWARE;
	ofn.lpstrDefExt = "*";		// default extension
	ofn.lpfnHook = NULL;

	if( Load )	//??
	{
		if( GetOpenFileName((LPOPENFILENAME)&ofn)) 
		{
			strcpy(strFile, ofn.lpstrFile);
			return true;
		}
	}
	else	//??
	{
		if( GetSaveFileName((LPOPENFILENAME)&ofn) ) 
		{
			strcpy(strFile, ofn.lpstrFile);
			return true;
		}
	}
	return false;
}

bool GetPathDialog(const char* strRoot, char* strPath)
{
	BROWSEINFO bi;
	char diskname[MAX_PATH];
	char path[MAX_PATH];

	ITEMIDLIST *pidl;
	bi.hwndOwner=NULL;	//AfxGetMainWnd()->GetSafeHwnd();
	bi.pidlRoot=0;
	bi.pszDisplayName=diskname;
	bi.lpszTitle="select a path";
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn=0;
	bi.lParam=0;
	bi.iImage=0;

	if( pidl = SHBrowseForFolder(&bi) )
	{
		SHGetPathFromIDList(pidl, path);

		strcpy(strPath, path);
		return true;
	}
	return false;
}

// �滻STR�е�'\\'Ϊ'/';
void ReplaceLine(char* str)
{
	long l = lstrlen(str);
	for(int i=0; i<l; i++)
	{
		if( str[i] == '\\' )
		{
			str[i] = '/';
		}
	}
}

// ��ȡһ�����ַ����еĵ�NUM�������ո�Ϊ���
bool GetString(const char* sour, char* dest, int num)
{
	long len = lstrlen(sour);
	long n = 0, j = 0;;
	for(int i=0; i<len+1; i++)
	{
		char chr = sour[i];

		if( chr == ' ' || chr == 0 )
		{
			n++;
			if( n > num )
			{
				dest[j] = 0;
				return true;
			}
			continue;
		}

		if( num == n )
		{
			dest[j] = sour[i];
			j++;
		}
	}

	return false;
}

// ��������
//
// ���������˼·�ǲ������ģ�������ͷβ��������ָ��Ȼ�����м��ƶ���
// ������ָ����ָ�����������������״���򽻻�������ָ�뵽���Խ���˶�
// ����ô�ͱ����Ѿ������ֳ������飬�ٵݹ�����Լ���������ֱ�ʵʩͬһ
// ���̼��ɡ�
//
// ������a����startΪ��ʼ�����λ�ã�endΪ��
// �����λ�ã�����a[10]��start=0��end=9��
template<class T>
void QuickSort(T* a, int begin, int end) 
{                                 
	int p=begin;
	int q=end;
	T mid=a[p]; //��׼��
	T temp;

	while(p<q)
	{
		// ��������ߵ������ڵ��ڱ�׼��
		while (a[p]<mid)
		{
			p++;
		}

		// �������ұߵ���С�ڵ��ڱ�׼��
		while (a[q]>mid)
		{
			q--;
		}

		if (p<=q) 
		{
			//����a[p]��a[q]��
			temp=a[p];
			a[p]=a[q];
			a[q]=temp;

			p++;
			q--;
		}  
	}

	if (q>begin) QuickSort(a, begin,q);	//������ǰ�벿������
	if (p<end) QuickSort(a, p,end); 	//�����Ժ�벿������
}

// ��ȡ��(sx,sy)��(dx,dy)�ķ���(0-7)
long GetLineDir(long sx, long sy, long dx, long dy)
{
	long dir = 0;
	float fx = (float)(dx - sx);
	float fy = (float)(dy - sy);
	if( fx || fy )
	{
		float tag = fabs((float)fy/(float)fx);
		if( tag < tan( 0.3926f ) )	// 22.5
		{
			dir = (fx>0)?(2):(6);
		}
		else if( tag < tan( 1.178f ) )	// 67.5
		{
			if( fx > 0 )
			{
				dir = (fy>0)?(3):(1);
			}
			else
			{
				dir = (fy>0)?(5):(7);
			}
		}
		else
		{
			dir = (fy>0)?(4):(0);
		}
	}
	return dir;
}

bool CreateDir(const char* dir)
{
	/*
	CFileFind f;
	if (f.FindFile(dir))
	{
		return true;
	}
	else
	{
		BOOL done = CreateDirectory(dir,NULL);
		if (done)
		{
			return true;
		}
	}

	int n = 0;
	do
	{
		n = dir.Find('\\',n);
		if (n == -1)
		{
			BOOL done = CreateDirectory(dir,NULL);
			if (!done)
			{
			//	AfxMessageBox("can not create dir " + dir) ;
				assert(0);
				return false;
			}
			return true;
		}

		char s[MAX_PATH];
		memset(s, 0, MAX_PATH);
		memcpy(s, dir, n);
		if (f.FindFile(s))
		{
		}
		else
		{
			BOOL done = CreateDirectory(s,NULL);
			if (!done)
			{
			//	AfxMessageBox("can not create dir " + dir) ;
				assert(0);
				return false;
			}
		}
		n++;
	}while(1);
*/
	return true;
}

// ��ȡ���е�ָ���ļ�
void FindFile(const char* strFile, const char* strExt, list<string>* plistFileName)
{
	char strPath[MAX_PATH];
	GetFilePath(strFile, strPath);

	WIN32_FIND_DATA stFindData;
	HANDLE hFind = FindFirstFile(strFile, &stFindData);
	if (hFind==INVALID_HANDLE_VALUE)
	{
		return;
	}

	BOOL bNext = true;
	while(1)
	{
		bNext = FindNextFile(hFind, &stFindData);
		if (bNext==0)
		{
			break;
		}

		// ��Ŀ¼
		if (stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(stFindData.cFileName, "..")!=0)
			{
				char str[MAX_PATH];
				sprintf(str, "%s%s/*.*", strPath, stFindData.cFileName);
				FindFile(str, strExt, plistFileName);
			}
		}
		else	// �ļ�
		{
			char strExtName[MAX_PATH];
			GetFileExtName(stFindData.cFileName, strExtName);
			if (stricmp(strExtName, strExt)==0)
			{
				char str[MAX_PATH];
				sprintf(str, "%s%s", strPath, stFindData.cFileName);
				strlwr(str);	//  ת����Сд
				plistFileName->push_back(str);
			}
		}
	}

	FindClose(hFind);
}

bool FormatMoneyNumber(char *str,const int size)
{
	int len=lstrlen(str);

	if(len<=3 || len/3 + len > size)
		return false;

	int end=len;
	for(int i=len-3; i>0; i-=3)
	{
		int l=lstrlen(str);
		for(int j=l;j>i;--j)
		{
			str[j]=str[j-1];
		}
		str[i]=','; 
		str[++end]='\0';
	}
	//�Ӹ�0�����ͷ
	if(end<size)
		str[end]='\0';
	return true;
}

// ��ȡCPU�Կ���������ʱ��������
inline unsigned __int64 GetCPUCycleCount()
{
	__asm _emit 0x0F
	__asm _emit 0x31
	//__asm
	//{
	//	RDTSC
	//}
	//if(IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE))
	//{
	//	return RDTSC();
	//}
	//else
	//{
	//	return 0;
	//}
}

// ��ȡCPU��Ƶ��
inline DWORD GetCPUFrequency()
{
	static BOOL bRDTSCSupported=IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE);
	if(!bRDTSCSupported)
	{
		return timeGetTime();
	}
	static DWORD dwFrequency=0;
	if(dwFrequency>0)return dwFrequency;
	HKEY hKey=0;
	DWORD dwSize;
	if(RegOpenKey(HKEY_LOCAL_MACHINE,"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",&hKey)==ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey,"~MHz",NULL,NULL,(LPBYTE)&dwFrequency,&dwSize);
		RegCloseKey(hKey);
		dwFrequency*=1000;
	}
	return dwFrequency;
}


// �߾���ʱ�亯��,����ϵͳ�Կ��������ĺ�����,��RDTSCָ���ȡCPU�Կ���������ָ�����ڣ�����CPUƵ�ʵõ�
DWORD GetCurTickCount()
{
	return timeGetTime();

	static BOOL bSupported=IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE);

	if(!bSupported)
	{
		return timeGetTime();
	}

	unsigned __int64 cpuCount=GetCPUCycleCount();
	DWORD cpuFrequency=GetCPUFrequency();
	return (DWORD)(cpuCount/cpuFrequency);
}

DWORD GetCurTime()
{
	struct _timeb stCostTime;
	_ftime(&stCostTime);
	return stCostTime.time-stCostTime.timezone*60;
}

// ����
void IniEncoder(const char* pstrIn, char* pstrOut)
{
	size_t len = strlen(pstrIn);
	for(int i=0; i<len; i++)
	{
		pstrOut[i] = ~pstrIn[i];
		pstrOut[i] += 12;
	}
}

// ����
void IniDecoder(const char* pstrIn, char* pstrOut, int len)
{
	for(int i=0; i<len; i++)
	{
		pstrOut[i] = pstrIn[i];
		pstrOut[i] -= 12;
		pstrOut[i] = ~pstrOut[i];
	}
}