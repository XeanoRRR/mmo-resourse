// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once
#pragma   warning(disable:4996)   //ȫ���ص�
#pragma   warning(disable:4244)   //ȫ���ص�
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include<sstream>
#include<vector>
#include <wchar.h>
#include <afxdisp.h>        // MFC �Զ�����
#include<windef.h>
#include <shlwapi.h>
#include <stdlib.h>
#include<WinNT.h> 
#include<float.h>
using namespace std;



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

struct tagSkill
{
	string strSkillType;	//��������,�������͡��������ܱ�ʶ��:basic  ��ͨ���ܱ�ʶ��:common  ���ڼ��ܱ�ʶ��:cycle
	DWORD dwSkillID;		//����ID
	WORD wSkillLevel;		//���ܵȼ�
	WORD wInterTime;		//���ʱ��
};

struct tagMonster
{
	int nNO;				 //�Զ����ɵ����
	DWORD dwIndex;			//����ֵ
	string strOrigName;		//ԭʼ��
	string strName;			//������
	string strBlock;		//�Ƿ����赲
	string strSuper;		//�Ƿ��޵�
	WORD   wDropFlag;		//������־
	string strCollection;	//��Ӧ�ɼ�����
	string strTitle;		//��ν
	DWORD dwTitleColor;		//��ν��ɫ
	DWORD dwTitlePicture;	//��νͼƬ
	string strCountryID;	//ͼ��ID
	string strNpcSort;		//NPC���
	WORD   wBossLevel;		//BOSS���
	DWORD dwPicID;			//ͼƬID
	DWORD dwPicLevel;		//ͼƬ�ȼ�
	DWORD dwNameColorID;	//������ɫ���
	DWORD dwLevelColorID;	//�ȼ���ɫ���
	DWORD dwHpColor;		//Ѫ����ɫ
	DWORD dwSoundID;		//����ID
	string strTame;			//�Ƿ��ѱ��
	DWORD dwTameTimes;		//�����ѱ������
	string strShape;		//����
	int nLevel;				//�ȼ�
	DWORD dwExp;			//����ֵ
	DWORD dwWorkPoint;		//ְҵ����
	DWORD dwMaxHp;			//���HP
	WORD wMinAttack;		//��С������
	WORD wMaxAttack;		//��󹥻���
	WORD wMinMagicAttack;	//��С��������
	WORD wMaxMagicAttack;	//���������
	WORD wHitRating;		//��������
	WORD wDefense;			//������
	WORD wMagicDefense;		//ħ��������
	WORD wBlock;			//��
	WORD wParry;			//�м�
	WORD wImmunity;			//����
	WORD wPierce;			//��͸
	WORD wDodge;			//��������
	float fMoveSpd;			//�ƶ��ٶ�
	float fBatMoveSpd;		//ս���ƶ��ٶ�
	float fTurningSpeed;	//ת���ٶ�
	WORD wWillAnti;			//��־����
	WORD wGroundAnti;		//������Ԫ�ؿ���
	WORD wWaterAnti;		//����ˮԪ�ؿ���
	WORD wFireAnti;			//������Ԫ�ؿ���
	WORD wWindAnti;			//������Ԫ�ؿ���
	WORD wSacreAnti;		//������ʥԪ�ؿ���
	WORD wDarkAnti;			//�����ڰ�Ԫ�ؿ���
	WORD wNotElemAnti;		//������Ԫ�ؿ���
	float wHpRecoverSpd;		//�����ָ��ٶ�
	float wMpRecoverSpd;		//ħ���ָ��ٶ�
	WORD wBodyAnti;			//���ʿ���
	WORD wBloodAnti;		//������Ѫ����
	WORD wDuskAnti;			//�������ο���
	WORD wNumbAnti;			//������Կ���
	WORD wSleepAnti;		//����˯�߿���
	WORD wDrugAnti;			//�����ж�����
	WORD wNotMoveAnti;		//����������
	WORD wSilenceAnti;		//������Ĭ����
	WORD wMinStayTimes;		//��Сͣ������
	WORD wMaxStayTimes;		//���ͣ������
	WORD wMinWalkTimes;		//��С���ߴ���
	WORD wMaxWalkTimes;		//������ߴ���
	WORD wAlertRange;		//���䷶Χ
	WORD wPurRange;			//׷����Χ
	WORD wPeaMoveRange;		//��ƽ�ƶ���Χ
	string strAIModel;		//AIģʽ
	int bTalk;				//�ɷ�Ի�
	string strMonRace;		//��������
	string strElem;			//Ԫ������
	WORD wStayTime;			//ͣ����ʱ��
	string strAIWay;		//AI����ʽ
	WORD wHurtHuman;		//���������������˺�
	WORD wHurtElem;			//��Ԫ�ع�������˺�
	WORD wHurtDiff;			//�������������˺�
	WORD wHurtAnimal;		//�Զ����������˺�
	WORD wHurtDead;			//�������������˺�
	WORD wHurtDevil;		//�Զ�ħ���������˺�
	WORD wHurtInsect;		//���������������˺�
	WORD wHurtFire;			//�Ի�ϵ��������˺�
	WORD wHurtWater;		//��ˮϵ��������˺�
	WORD wHurtGround;		//����ϵ��������˺�
	WORD wHurtWind;			//�Է�ϵ��������˺�
	WORD wHurtSacre;		//����ʥϵ��������˺�
	WORD wHurtDark;			//�Ժڰ�ϵ��������˺�
	WORD wHurtNotElem;		//����Ԫ��ϵ�й�������˺�
	WORD wVitality;			//����
	WORD wStormHit;			//��������
	WORD wStormHurt;		//�������˺�
	WORD wStormAnti;		//����������
	WORD wMagicStorm;		//ħ��������
	WORD wMagicStormHurt;	//ħ�������˺�
	WORD wMagicStormAnti;	//ħ����������
	float fInjurRate;		//���˱���
	WORD wAlertTime;		//����ʱ��
	float fEscape;			//���ܵ���							
	WORD wSkillRate;		//���ܸ���
	DWORD wSkillInter;		//���ܼ��
	string strReturn;		//�Ƿ�ɻ�
	string strAllRecover;	//�Ƿ���ȫ�ָ�
	WORD wReDistance;		//��λ����
	WORD wReTime;			//��λʱ��
	string strRefe1;		//ai�ο�1
	string strRefe2;		//ai�ο�2
	string strRefe3;		//ai�ο�3
	string strRefe4;		//ai�ο�4
	string strRefe5;		//ai�ο�5
	string strBirthSct;		//�����ű�
	string strBattleSct;	//ս���ű�
	string strInjurSct;		//���˽ű�
	string strDeadSct;		//�����ű�
	string strCycleSct;		//���ڽű�
	string strChoseSkillSct;//ѡ���ܽű�
	string strFindTgtSct;	//Ѱ��Ŀ��ű�
	string strKillTgtSct;	//ɱ��Ŀ��ű�
	vector<tagSkill>vecSkill;
};

