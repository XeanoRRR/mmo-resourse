#ifndef __SCRIPT__H__
#define __SCRIPT__H__
#pragma once


//	Script System
//	Create in 2002/7/22

/************************************************
*	-= �ű��� =-								*
*	���벢ִ�нű��ļ�							*
*	���������Կ���NPC����Ϊ						*
*	��������д�ض������						*
*	��������������								*
*	��������̬�ı���Ϸ�еĸ������ݣ���ʦ��		*
************************************************/

/********************************************************************************
*	�ű�ϵͳ�����л���:															*
*	��������������������������													*
*	������������������������������ָ����ʽ֪ͨ�ͻ�����							*
*	��������������������������������������������������							*
*	��������������������������������������										*
*	���ة������������������������٣��������������������������������٣�			*
*	��������������һ���ű�ʵ��������ִ��һ��ָ����ű��Ƿ���ɩ������ͷŽű�	*
*	����������������������������������������������������������������������		*
*	��ѭ�������������������Σ����������������������������������Σ���������		*
*	���������੤����������������������������������������������������������		*
*	����������������������������������������������������������������������		*
*	����������������������������������������������������������������������		*
*	���Щ�																		*
*	����																		*
*																				*
*	-= ˵�� =-																	*
*																				*
*	�����ڷ�������һ�ű�����ÿ����Ϸѭ������ϵͳ�����η���һ��������������	*
*	Ԫ�أ���ִ��һ������������ָ��������ָ���Ƿ���ɡ�����ýű�ʵ��������	*
*	����������Ϊÿ���µĽű�ִ��������һ���ű�ʵ�������������뵽�ű������С�	*
*																				*
********************************************************************************/

#include <string>
#include <map>
using namespace std;

#include "..\public\GUID.h"

#define ERROR_CODE		0x09fffff9	//���������
#define MAX_OPERATION	23			//����������
#define MAX_VAR			32			//һ������е����Ԫ������
#define MAX_VAR_LEN		3072		//����Ԫ�س���

#define MAX_STACK_CMD_LEN	256	//����Ԫ�س���

class CVariableList;
class CPlayer;
class CNpc;
class CRegion;
class CBaseModule;
class GameFaction;

enum SCRIPT_FUNC_DEF
{
	SR_FUNC_WAIT					= 6,
	SR_FUNC_GAMEMESSAGE				= 100,
	SR_FUNC_TALKBOX					= 2307,
	SR_FUNC_HELP					= 101,
	SR_FUNC_GETPLAYER				= 102,
	SR_FUNC_ISPLAYERONLINE			= 3003,
	SR_FUNC_GETREGIONID				= 103,
	SR_FUNC_LISTBANEDPLAYER			= 104,
	SR_FUNC_GETONLINEPLAYER			= 105,
	SR_FUNC_CREATEPRGN				= 106,
	SR_FUNC_CREATEPHRGN				= 107,
	SR_FUNC_CREATETRGN				= 108,
	SR_FUNC_GETPLAYERPDUPRGNID		= 109,
	SR_FUNC_GETPLAYERPHDUPRGNID		= 110,
	SR_FUNC_GETTEAMDUPRGNID			= 111,
	SR_FUNC_ADDRGNVAR				= 112,
	SR_FUNC_SETRGNVAR				= 113,
	SR_FUNC_DELRGNVAR				= 114,
	SR_FUNC_GETRGNVAR				= 115,
	SC_FUNC_DELETERGN				= 116,
	SC_FUNC_GETMPLATEIDBYGUID		= 117,
	RGN_FUNC_GETDUPTYPEBYGUID		= 118,
	SR_FUNC_GETDUPRGNONWERGUID		= 119,
	SC_FUNC_GETPHRGNGUIDBYONWERNAME	= 120,
	SR_FUNC_GETDUPRGNNUM			= 121,
	SR_FUNC_GETGUIDBYPLAYERNAME		= 123,
	SR_FUNC_GETPGUIDBYPLAYERNAME	= 124,
	SR_FUNC_GETPHGUIDBYPLAYERNAME	= 125,
	SR_FUNC_GETTGUIDBYPLAYERNAME	= 126,
	SC_FUNC_VIEWRGNVAR				= 127,
	SR_FUNC_VIEWDUPRGNDATA			= 128,
	SR_FUNC_HALTTHISSCRIPT			= 129,
	SR_FUNC_VIEWRGNVALIDBYGUID		= 130,
	SR_FUNC_UPDATEVARVALUE			= 131,
	SR_FUNC_VIEWGSPLAYERNUM			= 132,
	SR_FUNC_GETTEAMRGNIDNUM			= 133,
	SR_FUNC_GETTEAMRGNIDBYINDEX		= 134,
	SR_FUNC_GETTEAMRGNTEMPLATIDBYINDEX	= 135,
	SR_FUNC_REFRESHPLAYERDUPRGNLIST = 136,
	SR_FUNC_ADDFACRES				= 137,
	SR_FUNC_ADDFACEXP				= 138,
	SR_FUNC_ADDFACMEMBERCONTRIBUTE	= 139,
	SR_FUNC_USESKILLSYNC			= 140,
	SR_FUNC_USESKILLSYNCONCELL		= 141,
	SR_FUNC_INLVLDIFFERENCE			= 142,
	SR_FUNC_INLVLSPACE				= 143,
	SR_FUNC_GETPLAYERREGION			= 144,
	SR_FUNC_CHANGEPLAYERREGION		= 145,
	SR_FUNC_REGIONISEXIT			= 146,
	SR_FUNC_SILENCEPLAYER			= 147,
	SR_FUNC_UPDATENAME				= 148,
	SR_FUNC_RELIVEWINDOW			= 149,
	SR_FUNC_RELIVEGOODWINDOW		= 150,
	SR_FUNC_ADDFACTICKETS			= 151,
	SR_FUNC_ADDBASE					= 152,
	SR_FUNC_ADDBATTLE				= 153,
	SR_FUNC_ADDARMYINVEST			= 154,
	SR_FUNC_CHANGEJOINBATTLE		= 155,
	SR_FUNC_ADDBASEBATTLE			= 156,
	SR_FUNC_ADDTECHLV				= 157,
	SR_FUNC_ADDENEMY				= 158,
	SR_FUNC_GETRGNCAMPPLAYERNUM		= 159,
	SR_FUNC_ISSENTAISYS				= 160,
};

// �ű���
// ע��ÿ�������󲻵ó���16��Ԫ�أ�ÿ��Ԫ�����Ϊ128�ֽ�
class CScript : public BaseMemObj
{
public:
	//�ű�ִ�з���
	enum SCRIPTRETURN
	{
		SR_OK=0,		//����
		SR_HANG,		//�ű�����
		SR_END,			//����
		SR_RETURN,		//return��־
		SR_FORBREAK,   //for�ṹ������־
		SR_FORCONTINUE,//for�ṹ��continue

		SR_ERROR,		//����	
	};


#ifdef _SCRIPT_INFO_
public:

	DWORD			m_RunTime;
#endif

private:


	// �������Ͷ���
	enum VAR_CLASS
	{
		VC_CONST=0,		//����
		VC_VAR,			//�򵥱���
		VC_ARRAY,		//�������
		VC_STRING,		//�ַ�������
		VC_FUNCTION,	//����
		VC_BRACKET,		//С����
		VC_GUID,        //GUID����
	};

	// �������ṹ
	struct stOperation
	{
		char *Name;		//����
		int Level;		//���ȼ� ( 0 -> 4 )
	};

	//�ӽű�����
	typedef enum	eChildScriptType
	{
		CST_FOR=0,//For�ṹ����
		CST_CALL,//call���õ��ӽű�
		CST_CALLEX,//callEx���õ��ӽű�
	}eCST;
	//��¼�˽ű����õ�ջ��Ϣ
	struct tagChilScriptInfo : public BaseMemObj
	{
		eCST	eType;//�ӽű�����
		char pszPreOp[MAX_STACK_CMD_LEN];//�����ӽű�֮ǰ��Ԥ�����ű�����
		char pszCon[MAX_STACK_CMD_LEN];	//��������,�����ӽű��������ű�����
		CScript *pChildScript;//�ӽű�
	};


	typedef double (CScript::*CallFunc)(const char* CmdStr, char* retStr);

	typedef map<string, int>        STRING2INT;
	typedef map<string, CallFunc>   STRING2PFUNC;

	// �����ṹ
	static STRING2PFUNC     *s_FunctionList2;   // �����б�

	static double m_LoadFunctionNum;						//�����б�����������

	static double m_MaxFunction;							//��������
	static stOperation m_OperationList[MAX_OPERATION];	//��������б�


	// �ݹ鷵�ؽű��ı����б�, ֻ����CGame::Release()�ӿ��ͷ�
	static CVariableList* g_StackVarList;

	//��һ��MainLoop��������Ҫɾ���Ľű�
	static list<CGUID>	m_DelScripts;

public:



	static void LoadFunction(const char *, char* pData);			//���뺯���б�
	static void ReleaseFunction();							//�ͷź����б�

	static CVariableList* GetStackVarListInst(void);
	static void ReleaseStackVarListInst(void);

	bool m_bHang;									//�Ƿ����
	int	m_nHangFunc;							//����ĺ������
	DWORD m_nWaitTime;								//�ȴ�����ʱ��
	DWORD m_dwLastTime;								//���һ�μ�ʱ
	string	m_strRunScriptName;						//��ʱ������ִ�еĵĽű�����

	static void PushDelScript(const CGUID& guid);
	//ɾ����ǵĽű�
	static void DelScript();

	const char* GetClientUpdateVarName(void) { return m_szClientUpdateVarName; }
	void SetClientUpdateVarName(const char* szName) { if(szName)strcpy_s(m_szClientUpdateVarName, 64, szName); }
private:
	CGUID		m_guIndexID;				//��У��������ID
	CRegion*	m_pRegion;				//ָ�򳡾���ָ�루һ�����ڣ�
	CGUID		m_guRegionID;				//����ID
	// [070621 AHC]
	// �¼�Դ����
	CShape*     p_SrcShape;
	long		m_lSrcType;
	CGUID		m_guSrcID;
	// �¼�Ŀ�Ķ���
	CShape*     p_DesShape;
	long		m_lDesType;
	CGUID		m_guDesID;

	CGUID		m_PlayerGuid;			//ָ��Player��ID
	CGUID		m_guUsedItemID;			//ʹ����Ʒ�����ű�����¼����Ʒ��GUID
	
	POINT m_ptDropGoods;			//��Ʒ����λ�á�������


	tagChilScriptInfo* m_pChildScriptInfo;	//�ӽű�ջ��Ϣ(�ڽű��в���call,for�ȵ���ʱ,�����ջ��Ϣ)

	char		m_szClientUpdateVarName[64]; // �ͻ����޸ĵı�������

	char*		m_szClientOptInfo;
	long		m_lClientOptInfoLen; // m_szClientOptInfo�ĳ���
public:
	void	SetID(const CGUID& guid)			{m_guIndexID=guid;}
	const CGUID&	GetID()				{return m_guIndexID;}
	
	void	SetRegion(CRegion* r);
	CRegion	*GetRegion()				{return m_pRegion; }

	void	SetSkillEx(CBaseModule *pSkill);

	void    SetSrcShape(CShape* shape);
	void	SetSrcShapeEx(CShape* pShape);
	CShape* GetSrcShape(void)           { return p_SrcShape; }
	long	GetSrcType()	{return m_lSrcType;}
	const CGUID&	GetSrcID()			{ return m_guSrcID;}

	void    SetDesShape(CShape* shape);
	CShape* GetDesShape(void)           { return p_DesShape; }

	void ResetShape();//��������

	void	SetFireShapeEx(CShape* shape);

	void SetIsHang(bool b)	{m_bHang=b;}
	bool IsHang()			{return m_bHang;}
	void SetHangFunc(int n)	{m_nHangFunc=n;}
	int GetHangFunc()		{return m_nHangFunc;}

	void	SetPlayerID(const CGUID& PlayerGuid)	{ m_PlayerGuid = PlayerGuid; }
	const CGUID&	GetPlayerID()				{return m_PlayerGuid;}

	void	SetUsedItemID(CGUID& guid)	{m_guUsedItemID = guid;}
	CGUID& GetUsedItemID()		{return m_guUsedItemID;}

	bool IsGM();					// �ж���ǰ����Ƿ���GM��
	void SetDropGoodsPoint(POINT& pt) {m_ptDropGoods = pt;};
	POINT& GetDropGoodsPoint()	{return m_ptDropGoods;}

	double GetTrueNpcID(double id);		// ����ʵ�ʵ�NPC��ID

	void ResetDataPos(void)
	{
		m_Point = 0;//��ǰָ��
		m_Line=m_Word=0;		//��ǰ����
	}

	const char* GetClientOptInfo(void) { return m_szClientOptInfo; }
	void SetClientOptInfo(const char* szInfo);
private:
	// -�ļ�-
	char m_FileName[MAX_PATH];		//�ű��ļ���
	char *m_Data;					//�ű�����
	int m_DataLen;					//�ű�����

	int m_Point;					//��ǰָ��
	int m_Line, m_Word;				//��ǰָ�������
	int m_Block;					//����� '{' ++   '}' --

	// -ָ��-
	int m_VarNum;							//��������
	enum VAR_CLASS m_VarClass[MAX_VAR];		//��������
	string m_Var[MAX_VAR];					//������

	//union{

	double m_Value[MAX_VAR];				//������ֵ
	//GUID����ָ��ֵ(��RunFunction()���ص�longָ����и�ֵ����),����RunScript����ֵ

	// ��¼����ֵ��ָ��ͳ���
	struct tagStrValueInfo
	{
		tagStrValueInfo(void);
		~tagStrValueInfo(void);
		char* pStrValue;
		long lStrValueLen;
	};

	tagStrValueInfo m_strValueInfo[MAX_VAR];			//������ֵ(�ַ���ָ��ͳ���)
	
	CGUID m_guidValue[MAX_VAR];			//GUID������ֵ(GUIDָ��)

	//};
	int m_Operation[MAX_VAR];				//������

	CVariableList* m_pSelfVariableList;		//�ű�ʵ��ӵ�еĽű�����
	CVariableList* m_pVariableList;			//�ű�������ʹ�õĽű������б�,������m_pSelfVariableList
private:
	// -����-
	int GetValueType(const char* pValue);			//��ȡһ����������(����,�ַ���)
	int GetValueClass();		//��ȡһ������������(����,�ַ���,����)
	static int DumpString(char* &pDest,long lDesLen, const char* pSrc);	//��pSrc�ַ������Ƶ�pDest,�����ڴ����·���ȸ�������,�����ַ�������
	double Modulus(long ModulusType,long OccuTypeA,long OccuTypeB, long LevelA, long LevelB);

	int GotoNextLine();						//ת����һ��
	int JumpTo(const char *);						//����ָ����λ��
	int JumpToNextBlock();					//������һ�� { }

	// -�ű�ָ��-
	void Check(const char *);			//�ֽ�
	bool ComputeVar(int);		//����һ��������ֵ
	int Count();				//����

	bool IsOperation(char);		//�Ƿ��ǲ�����
	bool IsChar(char);			//�Ƿ��Ǳ���
	int OperationNum(const char *);	//�õ����������

	int GetOperation(int num) { return m_Operation[num]; };	//��ȡ���������
	const char *GetVar(int num) { return m_Var[num].c_str(); };	//��ȡ������
	int GetValue(int num) {	return m_Value[num]; };			//��ȡ������ֵ

	friend CVariableList;

	int Prew(int num);			//ǰһ�����õ���ֵ����
	int PrewString(int num);	//ǰһ�����õ��ַ�������
	int FindHighest();			//�ҳ�������ȼ�

	// -����-
	char *GetFunctionName(const char *, long& lLen);		//��ȡ��������
	double GetIntParam(const char *, int);			//��ȡ���β���
	char *GetStringParam(const char *, int);	//��ȡ�ַ�������
	char *GetOperation(const char *, int);		//ȡ��ĳ���ָ���
	bool IsCompart(char);						//�Ƿ��Ƿָ���

	//��¼�����ӽű�
	void EnterChildScript(eCST eType,CScript* pChildScript,const char* pszPreOp,const char* pszCon);
	void LeaveChildScript(CScript* pChildScript);

public:

	// -����ӿ�-
	CScript(CShape* srcShape=NULL, CRegion *Region=NULL, CShape *desShape=NULL, CGUID& guUsedItemID = CGUID::GUID_INVALID, DWORD dwDiedMonsterIndex=0);	//����
	virtual ~CScript();									//�ͷ�
	void ReleaseChildScript();//�ͷ��ӽű�
	//��ʼ���ӽű�����
	void InitChildScriptData(CScript *pChildScript);

	char *GetData() { return m_Data; }		//�����ļ�����
	bool LoadScript(const char *, char* = NULL);	//����һ���ű��ļ�
	bool ReadCmd(char *strRet, int MaxLen=MAX_VAR_LEN);	//����һ��ָ��
	bool ReadForCmd(long& beginPos, long& endPos);	//����forѭ�����ڵ�ȫ���ű�
	void RunLine(const char *);					//����һ�нű�
	double RunFunction(const char *, char* retStr=NULL/*�����ַ���ָ��*/);			//����һ���ű�����
	enum SCRIPTRETURN RunStep();					//ִ��һ���ű�
	enum SCRIPTRETURN ContinueRun(CVariableList* stackVarList);		//�첽�������غ����ִ��
	enum SCRIPTRETURN ContinueRunCall(CVariableList* stackVarList);	//�첽�������غ����ִ��call�Ӿ�
	enum SCRIPTRETURN ContinueRunCallEx(CVariableList* stackVarList);	//�첽�������غ����ִ��callEx�Ӿ�
	enum SCRIPTRETURN ContinueRunFor(CVariableList* stackVarList);	//�첽�������غ����ִ��for�Ӿ�
	enum SCRIPTRETURN RunStr(CMoveShape* pDest,const char* pszContent);	//ִ��һ���ַ���
	
	CScript* BeginForScript(char* pScriptData = NULL);
	void EndForScript(CScript* forScript);//�ⲿ���ýű�
	void ResetChildScriptInfo();
	//long RunScript(CMoveShape* pDest,const char* pszContent);

	char* GetFileName()	{return m_FileName;}
	CVariableList* GetVariableList()	{ return m_pVariableList; }
	CVariableList* GetSelfVariableList()	{ return m_pSelfVariableList; }
	void SetVariableList(CVariableList* pVariableList) {m_pVariableList=pVariableList;}
	void ResetVariableList()	{m_pVariableList = m_pSelfVariableList;}

	double CheckParamStringForArray(const char* CmdStr, long cmdStrSize, char** MainVarName, long MainVarNameSize, char** InnVarName, long InnVarNameSize, long& countPos);

	// ���ñ���ֵ
	int SetScriptVarValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName, const char* value);
	int SetScriptVarValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName, long lArray=0, double value=0);
	const char* GetScriptVarValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName);
	double GetScriptVarValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName, long lArray);

	int SetScriptGuidValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName, const CGUID& guid);
	const CGUID& GetScriptGuidValue(CMoveShape* moveShapePtr/*ָ�������ָ��*/, const char* varName);

	// [070608 AHC]
	bool  SetAttr(OBJECT_TYPE type, const CGUID& entityID, std::string& attrName, double value);
	double  GetAttr(OBJECT_TYPE type, const CGUID& entityID, std::string& attrName);
	bool  ChangeAttr(OBJECT_TYPE type, const CGUID& entityID, std::string& attrName, double value);

	// ʹ��ָ������MoveShape����
	bool  SetMoveShapeAttrEx(DWORD entityPtr/*ָ���ַDWORDֵ*/, std::string& attrName, double value);
	double  GetMoveShapeAttrEx(DWORD entityPtr/*ָ���ַDWORDֵ*/, std::string& attrName);
	bool  ChangeMoveShapeAttrEx(DWORD entityPtr/*ָ���ַDWORDֵ*/, std::string& attrName, double value);

	int GetVarValue(const char* str,int num);

	//////////////////////////////////////////////////////////////////////////
	//	ȫ�ֱ���
	//////////////////////////////////////////////////////////////////////////
public:
	static CVariableList* GetGeneralVariableList()	{ return m_GeneralVariableList; }

	static void LoadGeneralVariable(BYTE* pByte, long lPos);	// ����ȫ�ֱ����б�
	static void ReleaseGeneralVariable();					// �ͷ�ȫ�ֱ����б�

	static bool UpdateToWorldServer(const char* strVarName, int pos, double nValue); // ����"$"��WorldServer
	static bool UpdateToWorldServer(const char* strVarName, const char* szValue);	 // ����"#"��WorldServer
	static bool UpdateToWorldServer(const char* strVarName, const CGUID& guid);		 // ����"@"��WorldServer

	static void IniBuffSkillVariableList();

private:
	static CVariableList* m_GeneralVariableList;
	//����ϵͳ�õ��ĵı���������ս������
	static CVariableList* m_BuffSkillVariableList;

	//[070515 AHC] Function
public:
	//-----------------------------------------------------
	//�����﷨����
	enum SCRIPTRETURN call(const char* cmd, char* retStr=NULL);
	enum SCRIPTRETURN callEx(const char* cmd, char* retStr=NULL);
	enum SCRIPTRETURN  For(const char* cmd, char* retStr=NULL);	
	double Goto(const char* cmd, char* retStr=NULL);
	double If(const char* cmd, char* retStr=NULL);
	double pause(const char* cmd, char* retStr=NULL);
	double wait(const char* cmd, char* retStr=NULL);
	double random(const char* cmd, char* retStr=NULL);
	double Rgb(const char* cmd, char* retStr=NULL);
	double LocalVar(const char* cmd, char* retStr=NULL);	
	double define(const char* cmd, char* retStr=NULL);
	double undefine(const char* cmd, char* retStr=NULL);
	double Ifdef(const char* cmd, char* retStr=NULL);
	double DestDefine(const char* cmd, char* retStr=NULL);
	double DestUndefine(const char* cmd, char* retStr=NULL);
	double IfDestDef(const char* cmd, char* retStr=NULL);
	double Time(const char* cmd, char* retStr=NULL);
	double Year(const char* cmd, char* retStr=NULL);
	double Month(const char* cmd, char* retStr=NULL);
	double Day(const char* cmd, char* retStr=NULL);
	double Hour(const char* cmd, char* retStr=NULL);
	double Minute(const char* cmd, char* retStr=NULL);
	double DayOfWeek(const char* cmd, char* retStr=NULL);
	double HourDiff(const char* cmd, char* retStr=NULL);
	double MinuteDiff(const char* cmd, char* retStr=NULL);
	double Second(const char* cmd, char* retStr=NULL);
	double CheckGuidIsNull(const char* cmd, char* retStr=NULL);
	double GetCurTick(const char* cmd, char *retStr=NULL);
	double TestLog(const char* cmd, char* retStr=NULL);
	double Int(const char* cmd, char* retStr=NULL);
	double RoundNum(const char* cmd, char* retStr=NULL);
	double GetPow(const char* cmd, char* retStr=NULL);
	//! ��ȡһ���ַ������Ӵ�
	double GetSubString(const char* cmd, char* retStr=NULL);
	
	//-----------------------------------------------------
	//ϵͳȫ�ֺ���
	double FormatGUID(const char* cmd, char* retStr=NULL);
	double ListOnlineGM(const char* cmd, char* retStr=NULL);
	double ListSilencePlayer(const char* cmd, char* retStr=NULL);
	double ListBanedPlayer(const char* cmd, char* retStr=NULL);
	double GetOnlinePlayers(const char* cmd, char* retStr=NULL);
	double PostRegionInfo(const char* cmd, char* retStr=NULL);
	double PostRegionInfoByIndex(const char* cmd, char* retStr=NULL);
	double PostWorldInfo(const char* cmd, char* retStr=NULL);
	double PostWorldInfoByIndex(const char* cmd, char* retStr=NULL);
	double PostCountryInfo(const char* cmd, char* retStr=NULL);
	double KickAllPlayer(const char* cmd, char* retStr=NULL);
	double KickRegionPlayer(const char* cmd, char* retStr=NULL);
	double RegisterGlobalTimer(const char* cmd, char* retStr=NULL);
	double UnRegisterGlobalTimer(const char* cmd, char* retStr=NULL);
	double KickPlayer(const char* cmd, char* retStr=NULL);
	double KickAround(const char* cmd, char* retStr=NULL);
	double BanPlayer(const char* cmd, char* retStr=NULL);
	double SilencePlayer(const char* cmd, char* retStr=NULL);
	double GetTextRes		(const char* cmd, char* retStr=NULL);
	double SetPlayerRegion		(const char* cmd, char* retStr=NULL);
	double SetPlayerRegionEx	(const char* cmd, char* retStr=NULL);
	double RollRegionInfo	(const char* cmd, char* retStr=NULL);
	double RollWorldInfo	(const char* cmd, char* retStr=NULL);
	double GetPlayerRegion(const char* cmd, char* retStr=NULL);
	double ChangePlayerRegion(const char* cmd, char* retStr=NULL);
	double ChangePlayerName(const char* cmd, char* retStr=NULL);

	//-----------------------------------------------------
	//���ж���(����ɼ����ɫ)����ʹ�ú���
	double SetAttrInt(const char* cmd, char* retStr=NULL);
	double SetAttrStr(const char* cmd, char* retStr=NULL);
	double SetAttrGuid(const char* cmd, char* retStr=NULL);
	double GetAttr(const char* cmd, char* retStr=NULL);
	double GetScriptAttr(const char* cmd, char* retStr=NULL);
	double ChangeAttr(const char* cmd, char* retStr=NULL);
	double RegisterTimer(const char* cmd, char* retStr=NULL);
	double UnRegisterTimer(const char* cmd, char* retStr=NULL);
	double PlayMotion(const char* cmd, char* retStr=NULL);
	//��Ŀ��ִ��һ���ű�
	double DesRunScript(const char* cmd, char* retStr=NULL);
	//------//����ִ������Χһ����Χ�ڵ��������
	double	GetPlayerNumByAround(const char* cmd, char* retStr=NULL);
	//------//����ִ������Χһ����Χ�ڵĹ�������
	double	GetMonsterNumByAround(const char* cmd, char* retStr=NULL);
	//------//����ִ������Χһ����Χ�ڵ�ͬ���������
	double	GetSameMonsterNumByAround(const char* cmd, char* retStr=NULL);
	double SetDelTime(const char* cmd, char* retStr=NULL);
	double SetDelTimeByGuid(const char* cmd, char* retStr=NULL);
	//ȡ��Դ��guid
	double GetSrcShapeGUID(const char* cmd, char* retStr=NULL);
	//ȡ��Ŀ��guid
	double GetDesShapeGUID(const char* cmd, char* retStr=NULL);
	double ChangeMoveSpeed(const char* cmd, char* retStr=NULL);	
	double ChangeBaseSpeed(const char* cmd, char* retStr=NULL);
	double SetMoveSpeed(const char* cmd, char* retStr=NULL);
	double GetCurrentSpeed(const char* cmd, char* retStr=NULL);
	double GetDestIntVar(const char* cmd, char* retStr=NULL);			//�õ�Ŀ�����ͱ�����ֵ,$boundĬ��Ϊ0
	double SetDestIntVar(const char* cmd, char* retStr=NULL);		//����Ŀ�����α�����ֵ,$boundĬ��Ϊ0
	double GetDestStrVar(const char* cmd, char* retStr=NULL);		//�õ�Ŀ���ַ���������ֵ
	double SetDestStrVar(const char* cmd, char* retStr=NULL);			//����Ŀ���ַ���������ֵ
	double GetDestGuidVar(const char* cmd, char* retStr=NULL);		//�õ�Ŀ��Guid������ֵ
	double SetDestGuidVar(const char* cmd, char* retStr=NULL);			//����Ŀ�����GUID��ֵ
	double IsShapeValid( const char *cmd, char *retStr = NULL ); // �ж�ָ��GUID�����Ƿ���Ч

	//-----------------------------------------------------
	//������غ���
	//1����������
	double SetMonsterAttr(const char* cmd, char* retStr=NULL);
	double GetMonsterAttr(const char* cmd, char* retStr=NULL);
	double ChangeMonsterAttr(const char* cmd, char* retStr=NULL);
	double AddHpBar(const char* cmd, char* retStr=NULL);
	double GetHpLevel(const char* cmd, char* retStr=NULL);
	double GetKillerID(const char* cmd, char* retStr=NULL);
	double GetKillerType(const char* cmd, char* retStr=NULL);
	double SetBeneficiary(const char* cmd, char* retStr=NULL);
	double GetBeneficiaryID(const char* cmd, char* retStr=NULL);
	double GetBeneficiaryType(const char* cmd, char* retStr=NULL);
	double GetBeneTeamGuid	(const char* cmd, char* retStr=NULL);
	double DelRegionMonsterByName(const char* cmd, char* retStr=NULL);
	//2������AI��غ���
	double MonsterTalk(const char* cmd, char* retStr=NULL);
	double MonsterTalkByIndex(const char* cmd, char* retStr=NULL);
	double MonsterTalkByGUID(const char* cmd, char* retStr=NULL);
	double MonsterTalkByGuidByIndex(const char* cmd, char* retStr=NULL);
	double UseSkillByID		(const char* cmd, char* retStr=NULL);
	double UseSkillByIDOnCell( const char *cmd, char *retStr=NULL);
	double UseSkillSync( const char *cmd, char *retStr = NULL );
	double UseSkillSyncWithTrace( const char *cmd, char *retStr = NULL );
	double UseSkillOnCellSync( const char *cmd, char *retStr = NULL );
	double MoveToCellOnPeace	(const char* cmd, char* retStr=NULL);
	double MoveToCellOnFight	(const char* cmd, char* retStr=NULL);
	double MoveToCellAround	(const char* cmd, char* retStr=NULL);
	double MoveByStepOnPeace	(const char* cmd, char* retStr=NULL);
	double MoveByStepOnFight	(const char* cmd, char* retStr=NULL);
	double SendExpActOnPeace	(const char* cmd, char* retStr=NULL);
	double SendExpActOnFight	(const char* cmd, char* retStr=NULL);
	double MoveByTimeOnPeace	(const char* cmd, char* retStr=NULL);
	double MovebyTimeOnFight	(const char* cmd, char* retStr=NULL);
	double CallFriendAround	(const char* cmd, char* retStr=NULL);
	double CallMonsterAround	(const char* cmd, char* retStr=NULL);
	double Stand				(const char* cmd, char* retStr=NULL);
	double StandOnce( const char *cmd, char *retStr=NULL );
	double RandomMoveByTime	(const char* cmd, char* retStr=NULL);
	double ChangeAI			(const char* cmd, char* retStr=NULL);
	double StartCycle		(const char* cmd, char* retStr=NULL);
	double EndCycle			(const char* cmd, char* retStr=NULL);
	double EscapeByTimeOnFight(const char* cmd, char* retStr=NULL);
	double DeleteSpring(const char* cmd, char* retStr=NULL);
	double EndEscape(const char* cmd, char* retStr=NULL);
	double EndEscapeByGUID(const char* cmd, char* retStr=NULL);
	double SetBefoolID(const char* cmd, char* retStr=NULL);
	double CleanBefoolID(const char* cmd, char* retStr=NULL);
	double SetForceTarget(const char* cmd, char* retStr=NULL);
	double CleanForceTatget(const char* cmd, char* retStr=NULL);	//
	//�ű��ӿ�
	//�ı�����״̬
	double ChangeWeaknessState(const char* cmd, char* retStr=NULL);
	//ִ������Χ�Ĺ������Լ���Ŀ��һ��ʱ��
	double EscapeByAround(const char* cmd, char* retStr=NULL);
	//����ִ��һ���ű�					(ben)
	double	MonsterRunScriptsAround(const char* cmd, char* retStr=NULL);
	//�����Ƿ�����ս��״̬				(ben)
	double	MonsterIsFight(const char* cmd, char* retStr=NULL);
	//���ù����״̬		//0= ��ƽ�� 1= ս��			(ben)
	double    SetMonsterFight(const char* cmd, char* retStr=NULL);		
	//������Χ��ͬ����﹥����ǰĿ��		����״̬ 0=�����ƣ� 1= ��ƽ�� 2= ս��			(ben)
	double	CallFriendAttack(const char* cmd, char* retStr=NULL);		
	//������Χ�����й��﹥����ǰĿ��		(ben)
	double	CallMonsterAttack(const char* cmd, char* retStr=NULL);			
	//--------//ִ����һ����Χ�ڵ�ָ��ԭʼ�����﹥��ָ��GUIDĿ��	//@Ŀ��GUID���������Ҳ�����ǹ��������Ĭ��Ϊִ�е�ǰ�ű�����һ���(ben)
	double	MonsterAttackObjectByAround(const char* cmd, char* retStr=NULL);

	//ָ��GUID���﹥��ָ��GUIDĿ��	//@������GUID��������Ĭ�ϵ�ǰִ���ߡ�
	double	MonsterAttackObject(const char* cmd, char* retStr=NULL);
	//�ı�ָ��GUID���������
	double	SetMonsterDir(const char* cmd, char* retStr=NULL);
	//ָ��GUID������й�λ����
	double	ReLiveMonster(const char* cmd, char* retStr=NULL);
	//����ָ��GUID����Ŀ���GUID
	double	GetMonsterTarget(const char* cmd, char* retStr=NULL);
	//����ָ��GUID�����Ŀ��
	double	SetMonsterTarget(const char* cmd, char* retStr=NULL);
	// ��ȡָ�������(x,y)����
	double GetMonsterPos( const char *cmd, char *retStr = NULL );
	double CreateMonster(const char* cmd, char* retStr=NULL);
	double CreateMonsterEx(const char* cmd, char* retStr=NULL);	
	double DelMonster(const char* cmd, char* retStr=NULL);
	double DelMonsterEx(const char* cmd, char* retStr=NULL);
	double DelMonsterRect(const char* cmd, char* retStr=NULL);
	double KillMonster(const char* cmd, char* retStr=NULL);
	double GetMonsterRefeashTime(const char* cmd, char* retStr=NULL);
	// �������Լ������ָ����������GUID
	double GetPlayerByDistance( const char *cmd, char *retStr = NULL );
	// ���غ�ĳGUID���֮��ľ������
	double GetDistanceFromPlayer( const char *cmd, char *retStr = NULL );
	// ���ѡ��һ����Χ�ڿɹ��������
	double GetAttackablePlayer( const char *cmd, char *retStr = NULL );
	// ���ѡ��һ����Χ��ָ��ְҵ�Ŀɹ������
	double ChooseTargetByOccu( const char *cmd, char *retStr = NULL );

	//-----------------------------------------------------
	//�ɼ�����غ���
	double CreateCollectionEx(const char* cmd, char* retStr=NULL);
	double CreateCollection(const char* cmd, char* retStr=NULL);
	double DelCollection(const char* cmd, char* retStr=NULL);
	
	//-----------------------------------------------------
	//������غ���
	//���ŵĲ���
	double SetDoor(const char* cmd, char* retStr=NULL);
	double GetDoor(const char* cmd, char* retStr=NULL);
	//-----------------------------------------------------
	//��ɫ��غ���
	//1.��������
	double ChangeMe(const char* cmd, char* retStr=NULL);
	double GetMe(const char* cmd, char* retStr=NULL);
	double SetMe(const char* cmd, char* retStr=NULL);
	double ChangeDest(const char* cmd, char* retStr=NULL);
	double GetDest(const char* cmd, char* retStr=NULL);
	double SetDest(const char* cmd, char* retStr=NULL);
	double SetPlayerPos(const char* cmd, char* retStr=NULL);
	double SetPlayerDir(const char* cmd, char* retStr=NULL); 
	double AddAreaCreditByAreaId(const char* cmd, char* retStr=NULL);
	double AddAreaCredit(const char* cmd, char* retStr=NULL);
	double SetAreaCredit(const char* cmd, char* retStr=NULL);
	double SetAreaCreditByAreaId(const char* cmd, char* retStr=NULL);
	double GetAreaCredit(const char* cmd, char* retStr=NULL);
	double GetAreaCreditLevel(const char* cmd,char* retStr=NULL);
	double GetOrganCreditLevel(const char* cmd,char* retstr=NULL);
	double AddOrganCredit(const char* cmd,char* retstr=NULL);
	double SetOrganCredit(const char* cmd,char * retsre=NULL);
	double GetCountry(const char* cmd, char* retStr=NULL);
	double ChangeCountry(const char* cmd, char* retStr=NULL);
	double GetContribute(const char* cmd, char* retStr=NULL);
	double SetContribute(const char* cmd, char* retStr=NULL);
	double IsCharged(const char* cmd, char* retStr=NULL);
	double SetCharged(const char* cmd, char* retStr=NULL);
	double GetName(const char* cmd, char* retStr=NULL);
	double ChangePlayer(const char* cmd, char* retStr=NULL);
	double SetPlayer(const char* cmd, char* retStr=NULL);
	double GetPlayer(const char* cmd, char* retStr=NULL);
	double SetPlayerLevel(const char* cmd, char* retStr=NULL);
	double IsPlayerOnline(const char* cmd, char* retStr=NULL);
	double ChangeMoneyByName(const char* cmd, char* retStr=NULL);
	double GetMoneyByName(const char* cmd, char* retStr=NULL);
	double SetMoneyByName(const char* cmd, char* retStr=NULL);
	double ChangeSilverByName(const char* cmd,char* retStr=NULL);
	double GetSilverByName(const char* cmd,char* retStr=NULL);
	double SetSilverByName(const char* cmd,char* retStr=NULL);
	double AddTicketByName(const char* cmd, char* retStr=NULL);
	double GetTicketByName(const char* cmd, char* retStr=NULL);
	double SetPlayerExp(const char* cmd, char* retStr=NULL);
	double ChangePlayerExp(const char* cmd, char* retStr=NULL);
	double SetPlayerOccuExp(const char* cmd, char* retStr=NULL);
	double ChangePlayerOccuExp(const char* cmd, char* retStr=NULL);
	double GetPlayerOccuExp(const char* cmd, char* retStr=NULL);
	double SetPlayerOccuLvl(const char* cmd, char* retStr=NULL);
	double GetPlayerOccuLvl(const char* cmd, char* retStr=NULL);
	double SetPlayerOccuSp(const char* cmd, char* retStr=NULL);
	double ChangePlayerOccuSp(const char* cmd, char* retStr=NULL);
	double GetPlayerOccuSp(const char* cmd, char* retStr=NULL);
	double ChangePlayerOccuExpSp(const char* cmd, char* retStr=NULL);
	double IsGM(const char* cmd, char* retStr=NULL);
	double ShowPlayerInfo(const char* cmd, char* retStr=NULL);
	double ShowPlayerGoods(const char* cmd, char* retStr=NULL);
	double ShowGeneralVariable(const char* cmd, char* retStr=NULL);
	double DurancePlayer(const char* cmd, char* retStr=NULL);
	double Onstate(const char* cmd, char* retStr=NULL);
	double GetActive(const char* cmd, char* retStr=NULL);
	double AddActive(const char* cmd, char* retStr=NULL);
	double TakeActive(const char* cmd, char* retStr=NULL);
	double ResetActive(const char* cmd, char* retStr=NULL);
	double GetServer(const char* cmd, char* retStr=NULL);
	double CheckReliveByOrigin(const char* cmd, char* retStr=NULL);
	double CheckReliveGoods(const char* cmd, char* retStr=NULL);
	//! �õ������˶����жԷ���GUID
	double GetOtherOneInMarriageTeam(const char* cmd, char* retStr=NULL);
	//! �õ���ҽ�鲽��
	double GetPlayerMarriageStep(const char* cmd, char* retStr=NULL);
	//! ������ҽ�鲽��
	double SetPlayerMarriageStep(const char* cmd, char* retStr=NULL);
	//! ���ý�����
	double SetPlayerSpouse(const char* cmd, char* retStr=NULL);
	//! ɾ��������
	double DelPlayerSpouse(const char* cmd, char* retStr=NULL);
	//! �õ�����ID
	double GetPlayerSpouse(const char* cmd, char* retStr=NULL);
	//! �õ���������
	double GetPlayerSpouseName(const char* cmd, char* retStr=NULL);

	//! ��ȡԪ������
	double GetElementProperty(const char* cmd, char* retStr=NULL);
	//! ����Ԫ������
	double SetElementProperty(const char* cmd, char* retStr=NULL);
	//������һ���ֵ����
	double SetActiveRatio(const char* cmd, char* retStr=NULL);
	double GetActiveRatio(const char* cmd, char* retStr=NULL);
    //! ��������ȶ��һ�ʱ��
    double SetStableHookTime(const char* cmd, char* retStr=NULL);
    double AddStableHookTime(const char* cmd, char* retStr=NULL);
    double GetStableHookTime(const char* cmd, char* retStr=NULL);
	//��ȡ��һ���ֵ����
	//2.-------��չ����-------
	double GetOccuValueRatio(const char* cmd, char* retStr=NULL);
	double GetOccuLvlCoe(const char* cmd, char* retStr=NULL);
	double ChangeOccu(const char* cmd, char* retStr=NULL);
	double CheckLevel(const char* cmd, char* retStr=NULL);
	double CheckOccuLvl(const char* cmd, char* retStr=NULL);
	double ReLive(const char* cmd, char* retStr=NULL);
	double ScriptIsRun(const char* cmd, char* retStr=NULL);
	double RemoveScript(const char* cmd, char* retStr=NULL);
	double AutoMove(const char* cmd, char* retStr=NULL);
	double MovePlayer(const char* cmd, char* retStr=NULL);
	double GodMode(const char* cmd, char* retStr=NULL);
	double ResidentMode(const char* cmd, char* retStr=NULL);
	
	//-----------------------------------------------------
	//��Ʒ��غ���
	double AddGoods(const char* cmd, char* retStr=NULL);
	double AddArmament(const char* cmd, char* retStr=NULL);
	double AddArmamentForGM(const char* cmd, char* retStr=NULL);
	double DelGoods(const char* cmd, char* retStr=NULL);
	double AddGoodsToRgn(const char* cmd, char* retStr=NULL);
	double CheckGoods(const char* cmd, char* retStr=NULL);
	double CheckGoodsInBank(const char* cmd, char* retStr=NULL);
	double OpenGoodsExWindow(const char* cmd, char* retStr=NULL);	//! ����Ʒ��չ�Ի�����
	double BindGoods(const char* cmd, char* retStr=NULL);			//! �󶨣������Ʒ
	double FrostGoods(const char* cmd, char* retStr=NULL);			//! ���ᣨ�ⶳ����Ʒ
	double CheckSpace(const char* cmd, char* retStr=NULL);
	double GetGoodsNum(const char* cmd, char* retStr=NULL);
	double GetGoodsFree(const char* cmd, char* retStr=NULL);
	double GetEquipIDByPos(const char* cmd, char* retStr=NULL);
	double GetEquipNameByPos(const char* cmd, char* retStr=NULL);	//! ���ָ��װ����λ��װ��ԭʼ��
	double GetPlayerGoodsAddonProperty(const char* cmd, char* retStr=NULL);
	double SetPlayerGoodsAddonProperty(const char* cmd, char* retStr=NULL);
	double GetGoodsBaseType(const char* cmd, char* retStr=NULL);
	double OpenGoodsEnchase(const char* cmd, char* retStr=NULL);
	double DelPlayerGoods(const char* cmd, char* retStr=NULL);
	double DelUsedGoods(const char* cmd, char* retStr=NULL);
	double CheckUsedGoods(const char* cmd, char* retStr=NULL);
	double GetUsedGoodsPropertiy1(const char* cmd, char* retStr=NULL);
	double GetUsedGoodsPropertiy2(const char* cmd, char* retStr=NULL);
	double SetUsedGoodsPropertiy1(const char* cmd, char* retStr=NULL);
	double SetUsedGoodsPropertiy2(const char* cmd, char* retStr=NULL);
	double BuyGoods(const char* cmd, char* retStr=NULL);
	double GetGoodsAddonProperty(const char* cmd,char* retStr=NULL);
	double FullRefine(const char* cmd, char* retStr=NULL);
	double AddMedal			(const char* cmd, char* retStr=NULL);
	double DelMedal			(const char* cmd, char* retStr=NULL);
	double GetMedal			(const char* cmd, char* retStr=NULL);
	double GetMedalNum		(const char* cmd, char* retStr=NULL);
	double GetShowMedalName	(const char* cmd, char* retStr=NULL);
	double IsEquipEnchased(const char* cmd, char* retStr=NULL);
	double GetGoodsDisplayName(const char* cmd, char* retStr=NULL);
	double GetCostYuanBao(const char* cmd, char* retStr=NULL);
	
	double BeginMake(const char* cmd, char* retStr=NULL);			//! ��ʼһ���Ǹ�ְҵ���������䷽���͸���ң�
	double FinishMake(const char* cmd, char* retStr=NULL);			//! ���һ���Ǹ�ְҵ���������䷽�ϳ���Ʒ��
	double BackToCity(const char* cmd, char* retStr=NULL);			//�س�

	double BeginContainerGoodsOpt(const char* cmd, char* retStr=NULL);		//! ��ʼһ��ͨ����Ʒ��������
	double GetOptGoodsInContainer(const char* cmd, char* retStr=NULL);		//! �õ�������Ʒ
	double PutOptGoodsInContainer(const char* cmd, char* retStr=NULL);		//! ���������Ʒ��������
	double RemoveOptGoodsInContainer(const char* cmd, char* retStr=NULL);	//! ��������еĲ�����Ʒ
	double RefreshContainerTxt(const char* cmd, char* retStr=NULL);			//! ˢ�������ı�
	double RefreshContainerGold(const char* cmd, char* retStr=NULL);		//! ˢ��������ʾ��Ǯ
	double EndContainerGoodsOpt(const char* cmd, char* retStr=NULL);		//! �ر�һ��ͨ����Ʒ��������

	double GetGoodsOriginalName(const char* cmd, char* retStr=NULL);		//! ����GUID �õ�ָ����Ʒ��������ϣ���ԭʼ��
	double GetGoodsAddonPropertyValues(const char* cmd, char* retStr=NULL);	//! ����GUID �õ�ָ����Ʒ��������ϣ��ĸ�������ֵ
	double SetGoodsAddonPropertyValues(const char* cmd, char* retStr=NULL);	//! ����GUID ����ָ����Ʒ��������ϣ��ĸ�������ֵ
	double ResetGoodsStochastic(const char* cmd, char* retStr=NULL);		//! ����GUID ����ָ����Ʒ��������ϣ����������ֵ
	double DelGoodsByGuid(const char* cmd, char* retStr=NULL);				//! ����GUID ɾ��ָ����Ʒ��������ϣ�
	double AddGoodsReturnGuid(const char* cmd, char* retStr=NULL);			//! ������Ʒ�������ص�һ����Ʒ��GUID
	double IsFrostGoods(const char* cmd, char* retStr=NULL);				//! ����GUID �õ�ָ����Ʒ��������ϣ��Ƿ�Ϊ����
	
	double GetUsedGoodsOriginalName(const char* cmd,char* retStr=NULL);		//! �������ʹ�õ���Ʒ��ԭʼ��
	
	double GetUsedGoodsGuid(const char* cmd,char* retStr=NULL);    //! �������ʹ�õ���Ʒ��GUID
    double UploadGoods(const char* cmd, char* retStr=NULL);        // �ϴ���Ʒ
	
	double SetAddonDesc( const char *cmd, char *retStr = NULL );	/// ������Ʒ��������
	double GetAddonDesc( const char *cmd, char *retStr = NULL );	/// ��ȡ��Ʒ��������
	
	double AddGoodsToServer(const char *cmd, char *retStr = NULL);                        // ���һ����Ʒ��������
	double AddGoodsToClient(const char *cmd, char *retStr = NULL);
	

	//-----------------------------------------------------
	//������غ���
	double AddQuest(const char* cmd, char* retStr=NULL);
	double GetCurrentMasterRoleID(const char* cmd, char* retStr=NULL);
	double CompleteQuest(const char* cmd, char* retStr=NULL);
	double DisbandQuest(const char* cmd, char* retStr=NULL);
	double GetQuestState(const char* cmd, char* retStr=NULL);
	double GetValidQuestNum(const char* cmd, char* retStr=NULL);
	double UpdateQuestStep(const char* cmd, char* retStr=NULL);
	double UpdateQuestData(const char* cmd, char* retStr=NULL);
	double UpdateQuestParamData(const char* cmd, char* retStr=NULL);
	//ȡ��Ӷ������id
	double GetMercQuest(const char* cmd, char* retStr=NULL);
	//���õ�Ӷ��������
	double SetMercQuest(const char* cmd, char* retStr=NULL);
	double GetRoleStepNum(const char* cmd, char* retStr=NULL);
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ��ֵ
	double GetParam(const char* cmd, char* retStr=NULL);
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĸ�������ֵ
	double GetRoleAccParam(const char* cmd, char* retStr=NULL);
	//���� ĳһ����ĳһ�����Index��Ӧ�ĸ�������ֵ
	double SetRoleAccParam(const char* cmd, char* retStr=NULL);
	//���� ĳһ����ĳһ�����Index��Ӧ��ֵ
	double AddRoleAccParam(const char* cmd, char* retStr=NULL);
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	double GetRoleRegionID(const char* cmd, char* retStr=NULL);
	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
	double SetRoleRegionID(const char* cmd, char* retStr=NULL);
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	double GetRoleRegionX(const char* cmd, char* retStr=NULL);
	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
	double SetRoleRegionX(const char* cmd, char* retStr=NULL);
	//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	double GetRoleRegionY(const char* cmd, char* retStr=NULL);
	//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
	double SetRoleRegionY(const char* cmd, char* retStr=NULL);
	//ȡ�������б����������
	double GetRoleSum(const char* cmd, char* retStr=NULL);
	//ȡ���������������½�
	double GetChapter(const char* cmd, char* retStr=NULL);
	//�������������½���
	double SetChapter(const char* cmd, char* retStr=NULL);
	//��������¼
	double AddFinishedRoleRecord(const char* cmd, char* retStr=NULL);
	double DelFinishedRoleRecord(const char* cmd, char* retStr=NULL);	
	//ȡ��һ�������������״̬���Ƿ���ɣ�
	double GetRoleFinishedRecord(const char* cmd, char* retStr=NULL);
	//ȡ��һ������ģ���ǰ����ģ���Ӧ�ı�id���ı�����
	double GetStepText(const char* cmd, char* retStr=NULL);
	double GetText(const char* cmd, char* retStr=NULL);
	//ȡ���ı�����
	double GetTextType(const char* cmd, char* retStr=NULL);
	//��ȡ���������
	double GetAttributeRoleName(const char* cmd, char* retStr=NULL);
	//�����Ƿ�ᱻ��¼
	double GetAttributeBeRecord(const char* cmd, char* retStr=NULL);
	//��������
	double GetAttributeRoleType(const char* cmd, char* retStr=NULL);
	//�������npc��id
	double GetAttributeEmbracerNPCID(const char* cmd, char* retStr=NULL);
	//�Ƿ���ʾ
	double GetAttributeShowAble(const char* cmd, char* retStr=NULL);
	//�Ƿ��ܷ�����
	double GetAttributeRepeated(const char* cmd, char* retStr=NULL);
	//�Ƿ��ܾ��鱶��Ӱ��
	double GetAttributeExpRatio(const char* cmd, char* retStr=NULL);
	//�����Ƿ�ʧ��
	double GetAttributeDeadLost(const char* cmd, char* retStr=NULL);
	//����ʧ�ܺ��ܷ����½�
	double GetAttributeLostRepeated(const char* cmd, char* retStr=NULL);
	//��С�ȼ�����
	double GetAttributeMinGrade(const char* cmd, char* retStr=NULL);
	//���ȼ�����
	double GetAttributeMaxGrade(const char* cmd, char* retStr=NULL);
	//ְҵ����
	double GetAttributeOccupationRestrict(const char* cmd, char* retStr=NULL);
	//��ҪӶ������
	double GetAttributeEngageArmsRepute(const char* cmd, char* retStr=NULL);
	//�½���
	double GetAttributeChapterNum(const char* cmd, char* retStr=NULL);
	//��ȡ��һ������ID
	double GetAttributeNextRoleID(const char* cmd, char* retStr =NULL);
	//ȡ��index��Ӧ��ǰ������
	double GetLabelPrefix(const char* cmd, char* retStr=NULL);
	//ȡ��ǰ����������
	double GetLabelSum(const char* cmd, char* retStr=NULL);
	//Ŀ��ģ������
	double GetAimAMTType(const char* cmd, char* retStr=NULL);
	//Ŀ������
	double GetAimType(const char* cmd, char* retStr=NULL);
	//ȡ��Ŀ��id
	double GetAimId(const char* cmd, char* retStr=NULL);
	//ȡ��Ŀ������
	double GetAimNum(const char* cmd, char* retStr=NULL);
	//ȡ�� ��������
	double GetAimParameIndex(const char* cmd, char* retStr=NULL);
	//����������Ʒ���������ͣ�����Ʒ��
	double GetDropFrom(const char* cmd, char* retStr=NULL);
	//����
	double GetDropOdds(const char* cmd, char* retStr=NULL);
	//ȡ��Ŀ������
	double GetAimName(const char* cmd, char* retStr=NULL);
	//ȡ�ñ�������
	double GetPrepareParam(const char* cmd, char* retStr=NULL);
	//CQusetEncouragement
	//CQusetEncouragement
	double GetStepEncouragementSum(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementSum(const char* cmd, char* retStr=NULL);
	//ȡ�ý���������
	double GetSetpEncouragementName(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementName(const char* cmd, char* retStr=NULL);
	//ȡ�ý�������
	double GetSetpEncouragementType(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementType(const char* cmd, char* retStr=NULL);
	//�����ľ���id
	double GetSetpEncouragementID(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementID(const char* cmd, char* retStr=NULL);
	//��������
	double GetSetpEncouragementNum(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementNum(const char* cmd, char* retStr=NULL);
	//������ռ�ĸ�����
	double GetStepEncouragementCell(const char* cmd, char* retStr=NULL);
	double GetQuestEncouragementCell(const char* cmd, char* retStr=NULL);
	//ִ�н���
	double GetStepSum(const char* cmd, char* retStr=NULL);
	double GetStepAimNum(const char* cmd, char* retStr=NULL);
	double FindRoleIsInSystem(const char* cmd, char* retStr=NULL);
	double FindRoleStepIsInSystem(const char*cmd, char* retStr=NULL);
	//ٸ������
	//����ٸ������ˢ��ʱ��
	double ResetMercQuestUpdateTime(const char*cmd, char* retStr=NULL);
	//���ٸ���б�ID��������
	double AcquireMercQuest(const char*cmd, char* retStr=NULL);
	//����ٸ���������ݵ��ͻ��ˣ�$ٸ���б�ID==0��ʾֻ����ˢ��ʱ��
	double SendMercQuestToClient(const char*cmd, char* retStr=NULL);
	//��ٸ���������
	double OpenMercQuestInterface(const char*cmd, char* retStr=NULL);
	//�õ�ٸ����������
	double GetMercQuestData(const char*cmd, char* retStr=NULL);
	//����ٸ����������
	double SetMercQuestData(const char*cmd, char* retStr=NULL);
	//�������񼯽��
	double GetQuestMassPoint(const char*cmd, char* retStr=NULL);
	//��������ٸ����������
	double ResetAllMercQuestData(const char*cmd, char* retStr=NULL);
	//�鿴ĳһ�����Ƿ���Ӷ������ˢ�±�����
	double IsInMercQuestList(const char*cmd, char* retStr=NULL);
	double SetRoleProtect(const char* cmd, char* retStr=NULL);
	double SetMasterQuest(const char* cmd, char* retStr=NULL);
	//�ͽ�����
	double OpenShareQuestInterface(const char* cmd, char* retStr=NULL);
	double GetTeamCurrQuest(const char* cmd, char* retStr=NULL);
	double SetTeamCurrQuest(const char* cmd, char* retStr=NULL);
	double DelTeamShareQuest(const char* cmd,char* retStr=NULL);
	double IsCompleteQuest(const char* cmd,char* retStr=NULL);
	double GetQuestOdds(const char* cmd,char* retStr=NULL);
	double CanAcceptMercQuest(const char* cmd,char* retStr=NULL);
	//��������
	double OpenIndexQuestInterface(const char* cmd, char* retStr=NULL);
	double SetIndexQuestParamNum(const char* cmd, char* retStr=NULL);
	double SetIndexQuestState(const char* cmd, char* retStr=NULL);
	double ResetIndexQuest(const char* cmd, char* retStr=NULL);
	//-----------------------------------------------------
	//��ְҵ��غ���
	double CanCollect(const char* cmd, char* retStr=NULL);
	double CanCheckCollect(const char* cmd, char* retStr=NULL);
	double CheckCollect(const char* cmd, char* retStr=NULL);
	double LockCollection(const char* cmd,char* retStr=NULL);
	double OutlockCollection(const char* cmd, char* retStr=NULL);
	double CanFacture(const char* cmd, char* retStr=NULL);
	double Facture(const char* cmd, char* retStr=NULL);
	double FactureSkillStudyBegin(const char* cmd, char* retStr=NULL);
	double ClearFactureList(const char* cmd, char* retStr=NULL);
	double AddFacture(const char* cmd, char* retStr=NULL);
	double DelFacture(const char* cmd, char* retStr=NULL);
	double FindFacture(const char* cmd, char* retStr=NULL);
	double GetFactureNeedsLevel(const char* cmd, char* retStr=NULL);
	double GetFactureNeedsDOccu(const char* cmd, char* retStr=NULL);
	double UpdateEffect(const char* cmd, char* retStr=NULL);
	double AddFactureExp(const char* cmd,char* retstr=NULL);
	double AddCollectionExp(const char* cmd,char* retstr=NULL);
	//-----------------------------------------------------
	//����״̬��غ���
	//1������������صĺ���
	double AddState(const char* cmd, char* retStr=NULL);		
	double AddSkill(const char* cmd, char* retStr=NULL);
	double DelSkill(const char* cmd, char* retStr=NULL);
	double SetSkillLevel(const char* cmd, char* retStr=NULL);
	double GetSkillLevel(const char* cmd, char* retStr=NULL);
	double CheckStudySkill  (const char* cmd, char* retStr=NULL);
	double GetAllStateNum	(const char* cmd, char* retStr=NULL);
	double DelNewState		(const char* cmd, char* retStr=NULL);
	double DelDeBuff		(const char* cmd, char* retStr=NULL);
	double DelAllBuff		(const char* cmd, char* retStr=NULL);
	double Modulus			(const char* cmd, char* retStr=NULL);

	double AddStateToShape	(const char* cmd, char* retStr=NULL);
	//2�����ܽű����õ��ĺ���
	double AddNewState(const char* cmd, char* retStr=NULL);	
	double RemoverNewState(const char* cmd, char* retStr=NULL);	
	double SetMoveShapeAttrEx(const char* cmd, char* retStr=NULL);
	double GetMoveShapeAttrEx(const char* cmd, char* retStr=NULL);
	double ChangeMoveShapeAttrEx(const char* cmd, char* retStr=NULL);
	double AddDeBuffToObj	(const char* cmd, char* retStr=NULL);
	double SetEventDestObj	(const char* cmd, char* retStr=NULL);
	double GetBaseModuleLv  (const char* cmd, char* retStr=NULL);
	double ReLiveSkill		(const char* cmd, char* retStr=NULL);
	double GetPkResult(const char* cmd, char* retStr=NULL);
	double GetObjectType	(const char* cmd, char* retStr=NULL);
	double GetShieldNO		(const char* cmd, char* retStr=NULL);
	double SendSkillList	(const char* cmd, char* retStr=NULL);
	double GetDistance		(const char* cmd, char* retStr=NULL);
	double GetWeaponStr		(const char* cmd, char* retStr=NULL);	
	double GetDesEnergyRatio(const char* cmd, char* retStr=NULL);
	double GetEnergyRatio	(const char* cmd, char* retStr=NULL);
	double DeductEnergy		(const char* cmd, char* retStr=NULL);
	double SetTarget		(const char* cmd, char* retStr=NULL);	
	double GetSkillParam	(const char* cmd, char* retStr=NULL);
	double GetSkillElement  (const char* cmd, char* retStr=NULL);
	double GetSkillUseType	(const char* cmd, char* retStr=NULL);
	double GetStateParamByID(const char* cmd, char* retStr=NULL);				
	double GetStateParamByIndex(const char* cmd, char* retStr=NULL);
	double CheckStateFirer	(const char* cmd, char* retStr=NULL);
	double BeedAttackEvent	(const char* cmd, char* retStr=NULL);
	double GetSkillLv		(const char* cmd, char* retStr=NULL);
	double ReLiveInScript	(const char* cmd, char* retStr=NULL);
	double GetGuidByParam	(const char* cmd, char* retStr=NULL);
	double RunScriptByDest	(const char* cmd, char* retStr=NULL);
	double BeginUseSkill	(const char* cmd, char* retStr=NULL);
	double GetGoodsAddProperty(const char* cmd, char* retStr=NULL);
	double GetGoodsPlacePos	(const char* cmd, char* retStr=NULL);
	double DelGoodsByPlacePos	(const char* cmd, char* retStr=NULL);
	double SucceedUseItem	(const char* cmd, char* retStr=NULL);
	double GetActModify(const char* cmd, char* retStr=NULL);//ȡ�ü��ܶ�������ʱ��
	double SendUnLockGoodsMsg(const char* cmd, char* retStr=NULL);
	double GetBeneficiaryInfo(const char* cmd, char* retStr=NULL);
	double GetKillerInfo	(const char* cmd, char* retStr=NULL);
	double GetModType		(const char* cmd, char* retStr=NULL);
	double GetAttackerType	(const char* cmd, char* retStr=NULL);
	double GetFunMoHua		(const char* cmd, char* retStr=NULL);
	double GetBModifier		(const char* cmd, char* retStr=NULL);
	double GetPrevent		(const char* cmd, char* retStr=NULL);	
	double GetPartDamageOut	(const char* cmd, char* retStr=NULL);
	double OnCostYuanBao	(const char* cmd, char* retStr=NULL);

	//-----------------------------------------------------
	//�����˺Ϳͻ��˽������ͻ��˿�����صĽű�����
	double AddInfo(const char* cmd, char* retStr=NULL); 
	double AddDestInfo(const char* cmd, char* retStr=NULL); 
	double AddInfoByIndex(const char* cmd, char* retStr=NULL);
	double GameMessage(const char* cmd, char* retStr=NULL);
	double TalkBox(const char* cmd, char* retStr=NULL);
	//!	��ͼƬ��TalkBox
	double PicTalkBox(const char* cmd, char* retStr=NULL);
	double EndTalkBox(const char* cmd, char* retStr=NULL);
	double TalkBoxByIndex(const char* cmd, char* retStr=NULL);
	double StoryTalkBox(const char* cmd, char* retStr=NULL);
	double StoryTalkBoxByIndex(const char* cmd, char* retStr=NULL);
	double PixyTalkBox(const char* cmd, char* retStr=NULL);
	double EndPixyTalkBox(const char* cmd, char* retStr=NULL);
	double PixyTalkBoxByIndex(const char* cmd, char* retStr=NULL);
	double PlayEffectByIndex(const char* cmd, char* retStr=NULL);
	double PlayerTalk(const char* cmd, char* retStr=NULL);
	double PlayerTalkByIndex(const char* cmd, char* retStr=NULL);
	double PackGlint(const char* cmd, char* retStr=NULL);
	double PostPlayerInfo(const char* cmd, char* retStr=NULL);
	double PostDestPlayerInfo(const char* cmd, char* retStr=NULL);
	double PostPlayerInfoByIndex(const char* cmd, char* retStr=NULL);
	double PostDestPlayerInfoByIndex(const char* cmd, char* retStr=NULL);
	double ShowString(const char* cmd, char* retStr=NULL);
	double Weather(const char* cmd, char* retStr=NULL);
	double PlayEffect(const char* cmd, char* retStr=NULL);
	double PlayAction(const char* cmd, char* retStr=NULL);
	double PlaySound(const char* cmd, char* retStr=NULL);
	double Play3DSound(const char* cmd, char* retStr=NULL);
	double PlayBkMusic(const char* cmd, char* retStr=NULL);
	double StopBkMusic(const char* cmd, char* retStr=NULL);
	double AddGoodToShortCutBar(const char* cmd, char* retStr=NULL);
	double AddSkillToShortCutBar(const char* cmd, char* retStr=NULL);
	double TransforCamera(const char* cmd, char* retStr=NULL);
	double OperaInterface(const char* cmd, char* retStr=NULL);
	double HelpHint(const char* cmd, char* retStr=NULL);
	double InterfaceHint(const char* cmd, char* retStr=NULL);
	double OpenReliveWindow(const char* cmd, char* retStr=NULL);
	double OpenReLiveByGoodsWindow(const char* cmd, char* retStr=NULL);
	// �޸Ľű�����
	double UpdateScriptVarValue(const char* cmd, char* retStr=NULL);
	double PlayVideo		(const char* cmd, char* retStr=NULL);
	double StopVideo		(const char* cmd, char* retStr=NULL);
	double SendExpAction	(const char* cmd, char* retStr=NULL);
	double GetPlayerAllProperties(const char* cmd, char* retStr=NULL);
	double GetPlayerAllVariables(const char* cmd, char* retStr=NULL);
	// ��ʾ��ǰ���ȫ���ű�����
	double ViewPlayerAllVariableList(const char* cmd, char* retStr=NULL);
	

	//-----------------------------------------------------
	//��������֯��غ���


	//1�����麯��
	double GetTeamNum(const char* cmd, char* retStr=NULL);
	double GetTeamNumInRgn(const char* cmd, char* retStr=NULL);
	double GetTeamerName(const char* cmd, char* retStr=NULL);
	double GetTeamGUID(const char* cmd, char* retStr=NULL);
	double GetPlayerTeamGUID(const char* cmd, char* retStr=NULL);
	double IsTeamCaptain(const char* cmd, char* retStr=NULL);
	//! ������е�ͼ��ֻ�е���С��ָ�������Ķ���
	double CheckRgnOnlyTeam(const char* cmd, char* retStr=NULL);
	double GetTeamCaptainPos(const char* cmd, char* retStr=NULL);
	double SetRegionForTeam(const char* cmd, char* retStr=NULL);
	double SetTeamRegion(const char* cmd, char* retStr=NULL);
	double IsTeammatesAroundMe(const char* cmd, char* retStr=NULL);
	double InLvlDifference(const char* cmd, char* retStr=NULL);
	double InLvlSpace(const char* cmd, char* retStr=NULL);
	double RunScriptForTeam(const char* cmd, char* retStr=NULL);


	//2.�����֯
	double CreateFaction(const char* cmd, char* retStr=NULL);
	double CreateUnion(const char* cmd, char* retStr=NULL);
	double OpenApplyBoard(const char* cmd, char* retStr=NULL);
	double OpenFacIconBoard(const char* cmd, char* retStr=NULL);
	double SetFactionRecruitState(const char* cmd, char* retStr=NULL);
	double GetFactionLevel(const char* cmd, char* retStr=NULL);	
	double GetFactionIDByPlayerName(const char* cmd, char* retStr=NULL);
	double GetUinionIDByPlayerName(const char* cmd, char* retStr=NULL);
	//! Ϊ������ڹ��������Դ�����ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacRes("@PlayerGuid", #value, $Ret);
	double AddFacRes(const char* cmd, char* retStr=NULL);
	//! Ϊ������ڹ�����Ӿ��飨���ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacExp("@PlayerGuid", #value, $Ret);
	double AddFacExp(const char* cmd, char* retStr=NULL);
	//! Ϊ�����ӹ��ṱ�ף����ٴ��븺����@PlayerGuid��=""��$Ret=1��ʾִ�гɹ���AddFacMemberContribute("@PlayerGuid", #value, $Ret);
	double AddFacMemberContribute(const char* cmd, char* retStr=NULL);
	//! ��ȡ��Ա����ֵ
	double GetMemContribute(const char* cmd,char* retStr=NULL);
	//! ��ȡ������Դ
	double GetFacRes(const char* cmd,char* retStr=NULL);
	//! ��ȡ���徭��
	double GetFacExp(const char* cmd,char* retStr=NULL);
	//! ��ѯ����Ƿ��ǻ᳤
	double IsFacMaster(const char* cmd, char* retStr=NULL);
	//��ӹ����ĳ�ݵ����Ʊ,���ٴ�����
	double AddFacTickets(const char* cmd,char* retstr=NULL);
	//���ĳ�ݵ��ս��ֵ �����ٴ�����
	double AddFacBaseBattle(const char* cmd,char* retstr=NULL);
	//�޸�һ���ݵ����Ϣ
	double ChangeBaseInfo(const char* cmd,char* retstr=NULL);
	//���ӹ�����Ա��ս����ֵ
	double AddBattleVal(const char* cmd,char* retstr=NULL);
	//���ӹ�����Ա�ľ���Ͷ����ֵ
	double AddArmyInvest(const char* cmd,char* retstr=NULL);
	//��ȡĳ�ݵ����Ʊ��
	double GetFacTickets(const char* cmd,char* retstr=NULL);
	//��ȡ������Ա��ս��ֵ
	double GetBattleVal(const char* cmd,char* retstr=NULL);
	//��ȡ������Ա��Ͷ��ֵ
	double GetArmyInvest(const char* cmd,char* retstr=NULL);
	//��ȡ��Ա����Ȩ
	double GetJoinBattle(const char* cmd,char* retstr=NULL);
	//��ӿƼ�
	double AddTech(const char* cmd,char* retstr=NULL);
	//�õ�һ���Ƽ��ĵȼ�
	double GetTechLv(const char* cmd,char* retstr=NULL);
	//���ݼ���GUID��ȡ��������
	double GetFactionName(const char* cmd,char* retstr=NULL);
	//���ӻ���ٵжԼ�����Ϣ
	double ChangeEnemy(const char* cmd,char* retstr=NULL);
	//��ȡ�ݵ��ս��ֵ
	double GetFacBaseBattle(const char* cmd,char* retstr=NULL);
	//�����Ѹ��¾ݵ���Ϣ���ݵ��ͻ���
	double SendBaseInfo(const char* cmd,char* retstr=NULL);
	//��ȡ����ȼ���Ƽ��ȼ�
	double GetFacLevel(const char* cmd,char* retstr=NULL);
	//�򿪹������
	double FacDisplay(const char* cmd,char* retstr=NULL);

	//��������֯��غ���
	//1��������غ���
	double GetRgnAttr(const char* cmd, char* retStr=NULL);
	double SetRgnAttr(const char* cmd, char* retStr=NULL);
	double GetCellType(const char* cmd, char* retStr=NULL);
	double RegionIsExit(const char* cmd, char* retStr=NULL);
	double GetRegionID(const char* cmd, char* retStr=NULL);
	double GetMapInfo(const char* cmd, char* retStr=NULL);
	double GetCountryOwningRegion(const char* cmd, char* retStr=NULL);
	double SetBlock(const char* cmd, char* retStr=NULL);
	double GetRegionRandomPos(const char* cmd, char* retStr=NULL);
	double GetRandomPosInRange(const char* cmd, char* retStr=NULL);
	double GetNextPosByDir( const char *cmd, char *retStr=NULL);
	double GetLineDir( const char *cmd, char *retStr = NULL );
	double GetRegionPlayerNum(const char* cmd, char* retStr=NULL);
	double RunScriptForAllPlayerInRgn(const char* cmd,char* retStr=NULL);
	//1��������غ���
	double CreatePRgn			(const char* cmd, char* retStr=NULL);
	double CreatePHRgn			(const char* cmd, char* retStr=NULL);
	double CreateTRgn			(const char* cmd, char* retStr=NULL);
	// ȡ�õ�ǰ�ű���Ҹ��˸���GUID	GetCurPlayerPDupRgnID(@Ret);
	double GetCurPlayerPDupRgnID	(const char* cmd, char* retStr=NULL);
	//ȡ�õ�ǰ�ű���Ҹ��˷��ݸ���GUID	GetCurPlayerPHDupRgnID(@Ret);
	double GetCurPlayerPHDupRgnID	(const char* cmd, char* retStr=NULL);
	//ȡ�õ�ǰ�ű����鸱��GUID		GetCurTeamDupRgnID(@Ret);
	//double GetCurTeamDupRgnID		(const char* cmd, char* retStr=NULL);
	//ȡ����Ҹ��˸���GUID		GetPlayerPDupRgnID(@playerID, @Ret);
	double GetPlayerPDupRgnID	(const char* cmd, char* retStr=NULL);
	//ȡ����Ҹ��˷��ݸ���GUID	GetPlayerPHDupRgnID(@playerID, @Ret);
	double GetPlayerPHDupRgnID	(const char* cmd, char* retStr=NULL);
	//ȡ�ö��鸱��GUID			GetTeamDupRgnID(@playerID, @Ret);
	double GetTeamDupRgnID		(const char* cmd, char* retStr=NULL);
	//ȡ����ͨ����GUID				@Ret=GetNormalRgnID($lTemplateRgnID);
	double GetNormalDupRgnID	(const char* cmd, char* retStr=NULL);
	// �л���ͨ����byģ��id
	double ChangeRgnByTemplateID (const char* cmd, char* retStr=NULL);
	// ��ӳ�������
	double AddRgnVar(const char* cmd, char* retStr=NULL);
	// ���ó�������
	double SetRgnVar(const char* cmd, char* retStr=NULL);
	// ɾ����������
	double DelRgnVar(const char* cmd, char* retStr=NULL);
	// ȡ��ָ����������
	double GetRgnVar(const char* cmd, char* retStr=NULL);
	//ɾ��ָ�������ɣĵĸ�����
	double DeleteRgn			(const char* cmd, char* retStr=NULL);
	//����ָ�������ɣĵĸ�������ͼ�ɣģ���ø����ɣĵĸ����������򷵻�0��
	double GetTemplateIDByGUID	(const char* cmd, char* retStr=NULL);
	//���ص�ǰ���ڳ����ģɣġ�
	double GetCurRgnGUID		(const char* cmd, char* retStr=NULL);
	//����ָ�������ɣĵĸ����е�ǰ������������ԭGetRegionPlayerNum��
	//����ָ�������ɣĵĸ��������͡�
	double GetDupTypeByGUID		(const char* cmd, char* retStr=NULL);
	//����ָ�������ɣĵĸ����������ߣɣġ�
	double GetDupRgnOnwerGUID	(const char* cmd, char* retStr=NULL);
	//����ָ�������ɣĵĸ������������ޡ�
	double GetDupRgnPlayerMaxNum		(const char* cmd, char* retStr=NULL);
	//����ָ����ɫ�ɣ���ҽ�ɫ�ĸ��˷��ݸ����ɣġ�����Ҫ����Ҳ�����ʱҲ�ɲ�ѯ��
	double GetPHRgnGUIDByOnwerName	(const char* cmd, char* retStr=NULL);
	//������ص�ǰ�����е�һ����Ϊ�赲�����꣬��������ֵ���浽ָ����2����ұ����С���ԭGetRegionRandomPos��
	//���͵�ָ��������ָ�����ꡣ��ԭChangeRegion��
	//�����д�������ĳ�Աȫ�����͵�ָ��������ָ�����ꡣ��ԭSetTeamRegion��
	//��ָ��������ָ�����괴��ָ��ԭʼ�����������ű��������ԭCreateMonster��
	//ɾ��ָ������������Ϊָ��ԭʼ�������
	double DeleteMonster		(const char* cmd, char* retStr=NULL);
	//��ָ��������ָ�����괴��ָ���ɣĵĵ�ͼ������ݶ������ܹ���󿪷ţ�
	double CreateDupRgnEntity	(const char* cmd, char* retStr=NULL);
	//�Ը���Ϊִ����ִ��һ���ű�������ʹ�ø�����û����ҽű�Ҳ����ִ�У�
	double RunDupRgnScript		(const char* cmd, char* retStr=NULL);
	//����Ŀǰ��Ϸ�д��ڵĸ���������
	double GetDupRgnNum			(const char* cmd, char* retStr=NULL);
	//����Ŀǰ��Ϸ�д��ڵĸ����������ޡ�
	double GetDupRgnMaxNum		(const char* cmd, char* retStr=NULL);
	//����ָ�������ɣĵĸ����Ĺ���Ȩ���ţ�1��0����
	double SetDupRgnOperFlag	(const char* cmd, char* retStr=NULL);
	//�õ�ָ�������ɣĵĸ����Ĺ���Ȩ���ţ�1��0����
	double GetDupRgnOperFlag	(const char* cmd, char* retStr=NULL);
	//����ִ���ߵ�ǰ���ڶ����е�ָ������˳��ŵĽ�ɫ�ɣġ�
	double GetTeamMaterIDByPos		(const char* cmd, char* retStr=NULL);
	//�޸�ָ�������ɣĵ����ø�������ʾ��������һ����������޸ġ���������ͬ��ҽ�ɫ��������
	double EditDupRgnName		(const char* cmd, char* retStr=NULL);
	// ȡ�ñ���GSID
	double GetLocalGSID			(const char* cmd, char* retStr=NULL);
	// ͨ��������ֲ���GUID, ����2����GUID
	double GetGUIDByPlayerName  (const char* cmd, char* retStr=NULL);
	// ͨ��������ֲ�����P GUID, ����2����GUID
	double GetPGUIDByPlayerName  (const char* cmd, char* retStr=NULL);
	// ͨ��������ֲ�����PH GUID, ����2����GUID
	double GetPHGUIDByPlayerName  (const char* cmd, char* retStr=NULL);
	// ͨ��������ֲ�����T GUID, ����2����GUID
	double GetTGUIDByPlayerName  (const char* cmd, char* retStr=NULL);
	// ���õ�ǰ�ű���Ҹ��˸���GUID       SetPlayerPDupRgnID("@OwnerID","@RgnID");
	double SetPlayerPDupRgnID (const char* cmd, char* retStr=NULL);					
	// ���õ�ǰ�ű���Ҹ��˷��ݸ���GUID		SetPlayerPHDupRgnID("@OwnerID","@RgnID");	
	double SetPlayerPHDupRgnID (const char* cmd, char* retStr=NULL);		
	// ���õ�ǰ�ű����鸱��GUID			AddTeamRgnID("@OwnerID","@RgnID");
	double AddTeamRgnID(const char* cmd, char* retStr=NULL);
	//! Ϊ��ǰ�ű�����ɾ������GUID		DelTeamRgnID("@OwnerID", $templateID);
	double DelTeamRgnID(const char* cmd, char* retStr=NULL);
	//! Ϊ��ǰ�ű�����������и���GUID	ClearTeamRgnID("@OwnerID");
	double ClearTeamRgnID(const char* cmd, char* retStr=NULL);
	//! ������ҽ������ж��鸱��GUID	SwapTeamRgnID("@OwnerID", "@AimID");
	double SwapTeamRgnID(const char* cmd, char* retStr=NULL);
	//! �õ�������ڶ���Ķ��鸱����			GetTeamRgnIDNum("@OwnerID", $Ret);
	double GetTeamRgnIDNum(const char* cmd, char* retStr=NULL);
	//! �õ�������ڶ���ĵ�N�����鸱��ID		GetTeamRgnIDByIndex("@OwnerID", $N, "@ReRgnGuid");
	double GetTeamRgnIDByIndex(const char* cmd, char* retStr=NULL);
	//! �õ�������ڶ���ĵ�N�����鸱������ͼID	GetTeamRgnTemplatIDByIndex("@OwnerID", $N, $Ret);
	double GetTeamRgnTemplatIDByIndex(const char* cmd, char* retStr=NULL);

	// �鿴������������
	double ViewRgnVar(const char* cmd, char* retStr=NULL);
	// �л�����
	double ChangeDupRgn(const char* cmd, char* retStr=NULL);
	// �л����˸���
	double ChangeDupRgnByRgnID(const char* cmd, char* retStr=NULL);	
	// �������˷��ݸ�����Ʒ(Monster)
	double CreatePHGoods(const char* cmd, char* retStr=NULL);//(const CGUID& ownerID, const char* name, long nCounts, long posX, long posY,
	//	long width, long height, long dir, const char* strScript, const char* strTalk);
	// ɾ�����˷��ݸ�����Ʒ(Monster)
	double DeletePHGoods(const char* cmd, char* retStr=NULL);//(const CGUID& ownerID, const CGUID& goodsID);
	//ȡ�ø���GUID
	double GetPlayerGUID(const char* cmd, char* retStr=NULL);
	//ȡ��Ŀ���GUID
	double GetDestGUID(const char* cmd, char* retStr=NULL);
	// ͨ��GUID���ø�player��monster�Ľű�
	double CallScriptByGUID(const char* cmd, char* retStr=NULL);
	// �鿴ָ��GUID��Rgn�Ƿ����
	double RgnIsAvlidByGUID(const char* cmd, char* retStr=NULL);
	// �鿴ָ��pos�ĸ�����ϸ����
	double ViewDupRgnData(const char* cmd, char* retStr=NULL);
	// @������ֵ�ӿ�,Param1:@Src   Param2:@Des
	double EvaluateGUID(const char* cmd, char* retStr=NULL);
	// �鿴Rgn�Ƿ����Param1:@RgnID, Param2:#RetVar
	double ViewRgnValidByGUID(const char* cmd, char* retStr=NULL);
	// ȡ�õ�ǰ�ű������ߵ�GUID
	double GetCallerGUID(const char* cmd, char* retStr=NULL);
	// �����ǰGS�����и����͸���
	double ClearCurGSDupRgn(const char* cmd, char* retStr=NULL);
	// ˢ����ҵĸ�����¼�б�
	double ReFreshPlayerDupRgnList(const char* cmd, char* retStr=NULL);
	// �ı䵱ǰ������ĳ���ݵ�״̬
	double SetRgnStrongPointState (const char* cmd, char* retStr=NULL);
	// ȡ�õ�ǰ������ĳ���ݵ�״̬
	double GetRgnStrongPointState (const char* cmd, char* retStr=NULL);
	// ȡ��ָ����ͼָ����Ӫ��ȷ����
	double GetRegionCampPlayerNum(const char* cmd, char* retStr=NULL);

	//�����Ľű���غ���
	//1��������غ���
	//........................................................
	//2. ս��ϵͳ
	
	//! ս���ŶӵĽ�����˳�
	double SentaiQueue(const char* cmd, char* retStr=NULL);
	//! �����ҵ�ս�ӻ���
	double GetPlayerSentaiPoints(const char* cmd, char* retStr=NULL);
	//! �޸���ҵ�ս�ӻ���
	double ChangePlayerSentaiPoints(const char* cmd, char* retStr=NULL);
	//! ��ȡ�����ս�ӻ���
	double GetTeamSentaiPoints(const char* cmd, char* retStr=NULL);
	//! ��������
	double EndMatch(const char* cmd, char* retStr=NULL);
	//! ����һ������ı���
	double EndTeamMatch(const char* cmd, char* retStr=NULL);
	//! �Ƿ���ս��ϵͳ��
	double IsSentaiSys(const char* cmd, char* retStr=NULL);

	//......................................................
	//3.����ϵͳ��غ���
	// ���������̵���棺��/��
	double OpenBusiness			(const char* cmd,char* retStr=NULL);
	double IsBusinessState( const char *cmd, char *retStr = NULL );
	double UseHiggleSkill( const char *cmd, char *retStr = NULL );
	double UseTradeSkill( const char *cmd, char *retStr = NULL );
	double UseCheckingSkill( const char *cmd, char *retStr = NULL );
	double UseChafferSkill		(const char* cmd,char* retStr=NULL);
	double OpenInvest			(const char* cmd,char* retStr=NULL);
	double RemoveBusinessState	(const char* cmd,char* retStr=NULL);
	// �����������̽��棺��0��/����1��
	double OpenFBusiness( const char *cmd, char *retStr = NULL );

	//4.�ʼ�ϵͳ����
	double SendSysMail			(const char* cmd, char* retStr=NULL);
	double ReceiveSysMail		(const char* cmd, char* retStr=NULL);
	double ReceiveMail			(const char* cmd, char* retStr=NULL);
	//5.�������
	double OpenBouns			(const char* cmd, char* retStr=NULL);

	// Fox@20081230 ����
	double CreatePet(const char* pCmd, char* pRetStr = NULL);
	double DelPet(const char* pCmd, char* pRetStr = NULL);
	double RelivePet(const char* pCmd, char* pRetStr = NULL);
	double AddPetSkill(const char* pCmd, char* pRetStr = NULL);
	double GetFocusPet(const char* pCmd, char* pRetStr = NULL);
	double GetPet(const char* pCmd, char* pRetStr = NULL);

	// �������غ��� BAIYUN@20090603
	// �ı���һ�Խ��
	double ChangeActivity(const char *cmd, char *retStr = NULL);
	// ��������߲�������ڶ�����ܵ�¼
	double ACKickPlayer(const char *cmd, char *retStr = NULL);
	// ֹͣ��Ҽ��Ľű�����
	double StopCheckAC(const char *cmd, char *retStr = NULL);
	// ��ȡ��ҷ�����������
	double GetACAttr( const char *cmd, char *retStr = NULL );
	// ��������Ҷ�ʱ��
	double StartCheackAC(const char *cmd , char *retStr = NULL);

	// ���书����ؽӿ�
	double GetRegionHideFlagById( const char *cmd, char *retStr = NULL );
	double GetPlayerCanHideByGuid(const char *cmd, char *retStr = NULL);
	double SetPlayerCanHideByGuid(const char *cmd, char *retStr = NULL);
	double GetPlayerHideFlagByGuid(const char *cmd, char *retStr = NULL);
	double SetPlayerHideFlagByGuid(const char *cmd, char *retStr = NULL);
	double GetHideName(const char *cmd, char *retStr = NULL);
	double GMViewHideNameAround(const char *cmd, char *retStr = NULL);
	double GMAddSkill(const char *cmd, char *retStr = NULL);
	double GMDelSkill(const char *cmd, char *retStr = NULL);
	// ��ʱ��������ؽӿ�
	// ��ʱ�������¼��ʱ����Ϣ�����ؿͻ��˽����Ϣ
	double SetTimer(const char* cmd, char* retStr = NULL);
	// ��ʱ���رգ����践�ؿͻ�����Ϣ
	double CloseTimer(const char* cmd, char* retStr = NULL);
	// ȡ����Ҷ�ʱ��״̬
	double GetPlayerTimer(const char* cmd, char* retStr = NULL);
	// ȡ�ö�ʱ��ʣ��ʱ��
	double GetTimer(const char* cmd, char* retStr = NULL);
    //�򿪲�Ʊ����
	double OpenLotteryPage(const char* cmd, char* retStr = NULL);
	//��ȡ��Ʒ
	double DoLottery(const char* cmd, char* retStr = NULL);
	//�����������
	double SetWorldCredit(const char* cmd, char* retStr = NULL);
	//��ø��˻���
	double GetPersonalCredit(const char* cmd, char* retStr = NULL);
	//����ۼƻ���
	double GetPersonalCreditTotal(const char* cmd, char* retStr = NULL);
	//���õ�ǰ��߻��ֺ������
	double SetCurHighestCredit(const char* cmd, char* retStr = NULL);
	//������ҵ����̵ȼ�
	double SetWorldCreditVelue(const char* cmd, char* retStr = NULL);
	//��������������
	double GetWorldCreditMax(const char* cmd, char* retStr = NULL);
	//��ͻ��˷���������ֺ͸�����߻�����Ϣ
	double SendWorldCredit(const char* cmd, char* retStr = NULL);
	//���Ͳ�Ʊϵͳ�����纰����Ϣ
	double SendLotteryChat(const char* cmd, char* retStr = NULL);
	//���ص��γ齱�Ľ�Ʒ��
	double GetLotteryRe(const char* cmd, char* retStr = NULL);
	//��õ�ǰ��ҵ���������
	double GetNextTotalExp(const char* cmd, char* retStr = NULL);

    // �����־
    double MarriageLog(const char* cmd, char* retStr = NULL);

};

/************************************************
*	//���нű��Ľṹ							*
*	struct stRunScript							*
*	{											*
*	CRegion* pRegion;	// ����					*
*	CPlayer* pPlayer;	// ����					*
*	CNpc* pNpc;			// NPC					*
*	char strFile[64];	// �ű��ļ���			*
*	};											*
************************************************/

//���нű��Ľṹ
struct stRunScript
{
	CRegion* pRegion;		// ����
	CShape* srcShape;		// ����
	CShape* desShape;		// NPC
	CGUID guUsedItemID;		// ʹ�õĵ���ID
	DWORD dwDiedMonsterIndex;	// �����Ĺ���Index
	char* pszFileName;			// �ű��ļ���

	stRunScript()
	{
		pRegion = NULL;
		srcShape = NULL;
		desShape = NULL;
		pszFileName = NULL;
		dwDiedMonsterIndex = 0;
	}
};

//�����ṹ�����ڸ��ű���������б�
struct stVarPair
{
	std::string Name; // ������
	double NumValue;
	std::string StrValue; // cannot be used in union, so ..
	CGUID GuidValue;
	enum
	{
		VT_NUMBER, VT_STRING, VT_GUID, VT_INVALID
	} ValueType;

	stVarPair( const std::string &name, double value ) : Name( name ), NumValue( value ), ValueType( VT_NUMBER ) { }
	stVarPair( const std::string &name, std::string &value ) : Name( name ), StrValue( value ), NumValue( 0 ), ValueType( VT_STRING ) { }
	stVarPair( const std::string &name, const CGUID &value ) : Name( name ), GuidValue( value ), NumValue( 0 ), ValueType( VT_GUID ) { }
	stVarPair() : NumValue( 0 ), ValueType( VT_INVALID ) { }
};

typedef std::list<stVarPair> VarPairListType;

// �ű�����
typedef map<CGUID,CScript*,hash_guid_compare>	listScript;
typedef map<CGUID,CScript*,hash_guid_compare>::iterator	itScript;

extern listScript		g_Scripts;			//�ű�����
typedef deque<CScript*> FreeScript;
extern FreeScript g_FreeScriptList;     // Ԥ����Ŀ��нű��ڴ�
extern map<CScript*,string> g_UsingScript;     // ����ʹ�õ�Script

// [070511 AHC]
CScript* PopFreeScript(const char* pszName);
void	 PushFreeScript(CScript* freeScript);
double RunScript(stRunScript *, char* = NULL, int paramA=0, int paramB=0);//�ⲿ���ýű�

// ���ýű�ʱ��������������
double RunScript( stRunScript *rs, char *content, const VarPairListType &var_list );

long DelectAllScripByShape(const CGUID& PlayerGuid);
//ɾ����ҵ�Talkbox�ű�
long DelPlayerTalkBoxScript(const CGUID& PlayerGuid, SCRIPT_FUNC_DEF funcDef);

long DelectPlayerScript(const CGUID &ScriptID,const CGUID& PlayerGuid);	//ɾ��ĳ��ID��Ӧ�Ľű�
bool ScriptIfExit(CPlayer* pPlayer,char* strFile);			//�ж�ĳ����ҵĽű��Ƿ�����ִ��
long DelectPlayerScript(CPlayer* pPlayer,char* strFile);	//�ڽű����������PLAYER��Ӧ�Ľű�
long ContinueRunScript(const CGUID& ScriptID, CVariableList* stackVarList);//����ִ��һ���ű�
long GetScriptIndex();						//�õ�һ���ű���ȫ��ID


//����ű���Ϣ
void OutputScriptInfo();
// �ͻ���ѡ����Ч���ж�
bool ClientOptIsValid(const CGUID& scriptid, const char* szClientOpt);
// ����ű��Ŀͻ���ѡ��������
void ClearClientOptData(const CGUID& scriptid);
#endif