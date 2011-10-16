//********************************************
//	���ຯ��
//********************************************
#pragma once
#include <list>
#include <map>
using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DECREF
#define SAFE_DECREF(x) if(x) { (x)->DecRef(); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if(x) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x) if(x) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x)=NULL; }
#endif

#define WCHAR unsigned short

//���ͷ�����
//exp: Do( m=MessageBox( NULL, _pTemp, "test", MB_OK ), Ini.ReadText("index", "test") );
//extern char *g_pTemp;
//#define Do(line, param) {g_pTemp=param; line; SAFE_FREE(g_pTemp);}

int random(int nMax);						//�����
void Delay(long time);						//��ʱ
int GetFileLength(const char *name);		//ȡ�ļ�����
bool CheckFile(const char *name);			//�ж��ļ��Ƿ����
void ShowMessage(const char *msg,...);		//��ʾ��Ϣ
void InitialDebugFileName();
void PutDebugString(const char *msg);		//���������Ϣ
const char* GetDebugFileName();
void PutStringToFile(const char* filename, const char *msg);	//����ļ���Ϣ
void PutLogInfo(const char *msg);			//���log��Ϣ
char *GetFilePath(const char *filename, char* out);	//���ļ�����ȡ�ļ�·��
char *GetFileExtName(const char *filename, char* out);	//���ļ�����ȡ�ļ���չ��
WCHAR* WChar(char *str);					//char to wchar
char* Char(WCHAR* str);						//wchar to char
void DeleteDirectory(const char* strDir);	//ɾ��Ŀ¼
bool GetFileDialog(bool Load, const char* currentPath, char*, const char* strFilter="All Files\0*.*\0");	// ��ȡ�ļ�
bool GetPathDialog(const char* strRoot, char*);					// ��ȡ·��
void ReplaceLine(char* str);		// �滻STR�е�'\\'Ϊ'/';
bool GetString(const char* sour, char* dest, int num);	// ��ȡһ�����ַ����еĵ�NUM�������ո�Ϊ���

bool CreateDir(const char* dir); 
void FindFile(const char* strFile, const char* strExt, list<string>* plistFileName);	// ��ȡ���е�ָ����չ�����ļ�

inline bool IsInRect(long x, long y, RECT& rect)
{
	if( x > rect.left && x < rect.right && y > rect.top && y < rect.bottom )
	{
		return true;
	}
	return false;
}

bool FormatMoneyNumber(char *str,const int size);//��ʽ����Ǯ��ʾ,ÿ����֮���һ��","

template<class T>
void QuickSort(T* a, int begin, int end);	// ��������

long GetLineDir(long sx, long sy, long dx, long dy);		// ��ȡ��(sx,sy)��(dx,dy)�ķ���(0-7)


// �߾���ʱ�亯��,����ϵͳ�Կ��������ĺ�����,��RDTSCָ���ȡCPU�Կ���������ָ�����ڣ�����CPUƵ�ʵõ�
DWORD GetCurTickCount();
DWORD GetCurTime();

void IniEncoder(const char* pstrIn, char* pstrOut);		// ����
void IniDecoder(const char* pstrIn, char* pstrOut, int len);		// ����