#pragma  once

const DWORD NOTIFY_COLOR_RED=0xffff0000;
const DWORD NOTIFY_COLOR_YELLOW=0xffffff00;
const DWORD NOTIFY_COLOR_WHITE=0xffffffff;

//�ַ�������ID
enum eStrTypeID
{	
	IDS_WS_SKILL		= 1,
	IDS_WS_UNISTRING	= 2,
	IDS_WGS_SYSMAIL		= 3,

	IDS_GS_GOODS		= 20,

	IDS_WS_RGNMANAGER		= 200,
	IDS_WS_LOGMESSAGE		= 203,
	IDS_WS_SERVERMESSAGE	= 204,
	IDS_WS_GAME				= 205,
	IDS_GS_RGNMANAGER		= 207,
	IDS_GS_FUNCTION			= 208,
	IDS_GS_SERVERMESSAGE	= 209,
	IDS_GS_RGNFUNC			= 210,
	IDS_GS_SCRIPT			= 211,
	IDS_DBS_SERVERMESSAGE	= 212,
	IDS_DBS_DBINCREMENTLOG	= 213,
	IDS_DBS_RSDUPRGN		= 214,
	IDS_DBS_RSGOODS			= 215,
	IDS_DBS_RSPLAYER		= 216,
	IDS_DBS_SAVETHREAD		= 217,
	IDS_GS_NOBILITYTITILE	= 218,
	IDS_GS_OTHERINFO		= 219,

	IDS_WS_DOCCUSYS_ERROR	= 600,
	IDS_WS_QUESTSYS_ERROR	= 610,
	IDS_WS_MONSTERFILE_ERROR= 620,
	IDS_GS_REGION_ERROR		= 630,

	IDS_WS_REPORT_INFO		= 810,
	IDS_CF_BUILDLIST_CHAR	= 1000,
	IDS_CF_MONSTERLIST_CHAR = 1001,
	IDS_CF_GOODSLIST_CHAR	= 1002,
	IDS_CF_SYSBOARDCASTF_CHAR = 1003,

};
//�ַ�����ԴID�Զ���
//���������ַ�����ԴID
enum
{	STR_WS_SKILL_FIRST				= 1,
	STR_WS_SKILL_SECOND,
	STR_WS_SKILL_THIRD,
	STR_WS_SKILL_FOURTH,
	STR_WS_SKILL_FIFTH,
	STR_WS_SKILL_SIXTH,
	STR_WS_SKILL_SEVENTH,
	STR_WS_SKILL_EIGHTH,
	STR_WS_SKILL_NINETH,
	STR_WS_SKILL_TENTH,
	STR_WS_SKILL_ELEVENTH,
	STR_WS_SKILL_TWELFTH,
	STR_WS_SKILL_THIRTEENTH,
	STR_WS_SKILL_FOURTEENTH,
	STR_WS_SKILL_FIFTEENTH,
};
//ϵͳ�ʼ�
enum
{
	STR_WS_SYSMAIL_FIRST			= 1,
	STR_WS_SYSMAIL_SECOND			= 2,
	STR_WS_SYSMAIL_THIRD			= 3,
	STR_WS_SYSMAIL_FOURTH			= 4,
	STR_WS_SYSMAIL_FIFTH			= 5,
	STR_WS_SYSMAIL_SIXTH			= 6,
	STR_WS_SYSMAIL_SEVENTH			= 7,
	STR_WS_SYSMAIL_EIGHTH			= 8,
	STR_WS_SYSMAIL_NINE				= 9,
	STR_WS_SYSMAIL_TEN				= 10,
	STR_WS_SYSMAIL_ELEVEN			= 11,
	STR_WS_SYSMAIL_TWELVE			= 12,
};

//ͳһ�ַ��������ַ�����ԴID
enum
{
	STR_WS_UNISTRING_FIRST		   =  1,
	STR_WS_UNISTRING_SECOND		   =  2,
};


// WS�����������ַ�����ԴID
enum
{
	STR_WS_RGNMANAGER_CREATERGN = 0, 
	STR_WS_RGNMANAGER_DELETERGN	= 1, 

};

// WS��¼��Ϣ�ַ�����ԴID
enum
{
	STR_WS_LOGMESSAGE_QEUSTPROPERTYCHARDELED = 0,
	STR_WS_LOGMESSAGE_QUESTPROPERTYNOCREATEORLOADBASE	= 1,
	STR_WS_LOGMESSAGE_CHARINGAME						= 2,
	STR_WS_LOGMESSAGE_LOGINGSINVALID					= 3,
	STR_WS_LOGMESSAGE_LOGINWSQUEST						= 4,
	STR_WS_LOGMESSAGE_CHARSTATEERR						= 5,
	STR_WS_LOGMESSAGE_LOADBASEFAIL						= 6,
	STR_WS_LOGMESSAGE_SENDLOADPROPERTYTOGS				= 7,
	STR_WS_LOGMESSAGE_LOGOUTCHARNOTINWS					= 8,
	STR_WS_LOGMESSAGE_LOGOUTCHARINWSONLY				= 9,
	STR_WS_LOGMESSAGE_LOGOUTERRNOCHAR					= 10,
	STR_WS_LOGMESSAGE_LOGOUTOK							= 11,
	STR_WS_LOGMESSAGE_REPEATLOGINTOGS					= 12,
	STR_WS_LOGMESSAGE_REPEATLOGINQUIT					= 13,
	STR_WS_LOGMESSAGE_DELCHARNOCHAROBJ					= 14,
	STR_WS_LOGMESSAGE_DELCHARINVALID					= 15,
	STR_WS_LOGMESSAGE_DELCHAROK							= 16,
	STR_WS_LOGMESSAGE_RESTORCHARNOCHAROBJ				= 17,
	STR_WS_LOGMESSAGE_RESTORECHARVALID					= 18,
	STR_WS_LOGMESSAGE_CREATECHARNOCOUNTRY				= 19,
	STR_WS_LOGMESSAGE_SENDCREATETODBS					= 20,
};
// WSϵͳ��Ϣ�ַ�����ԴID
enum
{
	STR_WS_SERVERMESSAGE_DBSTOWSSETUPDATA = 0,	
	STR_WS_SERVERMESSAGE_DBSRECONNETIONINFO				= 1,
	STR_WS_SERVERMESSAGE_GSRECONNSENDDATABEGIN			= 2,
	STR_WS_SERVERMESSAGE_GSRECONNLOGINPLAYERNUM			= 3,
	STR_WS_SERVERMESSAGE_GSRECONNSENTLOGINPLAYERNUM		= 4,
	STR_WS_SERVERMESSAGE_GSRECONNSENDRGNDATA			= 5,
	STR_WS_SERVERMESSAGE_GSRECONNSETUPDATA				= 6,
	STR_WS_SERVERMESSAGE_CHANGERGNNORGNONGS				= 7,
	STR_WS_SERVERMESSAGE_CHANGERGNGSNOTRUN				= 8,
	STR_WS_SERVERMESSAGE_CHANGERGNNOPLAYERONWS			= 9,
	STR_WS_SERVERMESSAGE_WSRECONNGSNOPLAYERONWS			= 10,
};														
// WS��Ϸģ���ַ�����ԴID								
enum													
{														
	STR_WS_GAME_LOADPACKOK = 0,							
	STR_WS_GAME_LOGSERVERINVALID							= 1,					
	STR_WS_GAME_CANTCONNWS									= 2,
	STR_WS_GAME_CANTCONNLS									= 3,
	STR_WS_GAME_CONNDBSOK									= 4,
	STR_WS_GAME_CONNLSOK									= 5,
	STR_WS_GAME_COSTSERVERINVALID							= 6,
	STR_WS_GAME_LOADCARDSERVERFAIL							= 7,
	STR_WS_GAME_RELOADSCRIPTOK								= 8,
	STR_WS_GAME_RELOADSCRIPTFAIL							= 9,
	STR_WS_GAME_LOADGOLBALSTRINGOK							= 10,
	STR_WS_GAME_LOADGOLBALSTRINGFAIL						= 11,
	STR_WS_GAME_LOADEXPLISTOK								= 12,
	STR_WS_GAME_LOADEXPLISTFAIL								= 13,
	STR_WS_GAME_LOADLEVELLISTOK								= 14,
	STR_WS_GAME_LOADLEVELLISTFAIL							= 15,
	STR_WS_GAME_LOADOCCUDEFLISTOK							= 16,
	STR_WS_GAME_LOADOCCUDEFLISTFAIL							= 17,
	STR_WS_GAME_LOADFACELISTOK								= 18,
	STR_WS_GAME_LOADFACELISTFAIL							= 19,
	STR_WS_GAME_LOADGOODSLISTOK								= 20,
	STR_WS_GAME_LOADGOODSLISTFAIL							= 21,	
	STR_WS_GAME_LOADCOMQUIPMENTLISTOK						= 22,
	STR_WS_GAME_LOADCOMQUIPMENTLISTFAIL						= 23,
	STR_WS_GAME_LOADHOLELISTOK								= 24,
	STR_WS_GAME_LOADHOLELISTFAIL							= 25,
	STR_WS_GAME_LOADGTREATEAREALISTOK						= 26,
	STR_WS_GAME_LOADTREATEAREALISTFAIL						= 27,
	STR_WS_GAME_LOADMONSTERLISTOK							= 28,
	STR_WS_GAME_LOADMONSTERLISTFAIL							= 29,
	STR_WS_GAME_LOADMGOODSLISTOK							= 30,
	STR_WS_GAME_LOADMGOODSLISTFAIL							= 31,
	STR_WS_GAME_LOADQUESTGOODSLISTOK						= 32,
	STR_WS_GAME_LOADQUESTGOODSLISTFAIL						= 33,
	STR_WS_GAME_LOADMSKILLLISTOK							= 34,
	STR_WS_GAME_LOADMSKILLLISTFAIL							= 35,
	STR_WS_GAME_LOADTREADELISTOK							= 36,
	STR_WS_GAME_LOADTREADELISTFAIL							= 37,
	STR_WS_GAME_LOADSKILLLISTOK								= 38,
	STR_WS_GAME_LOADSKILLLISTFAIL							= 39,
	STR_WS_GAME_LOADGLOBALPARAMLISTOK						= 40,
	STR_WS_GAME_LOADGLOBALPARAMLISTFAIL						= 41,
	STR_WS_GAME_LOADEXPGOODSLISTOK							= 42,
	STR_WS_GAME_LOADEXPGOODSLISTFAIL						= 43,
	STR_WS_GAME_LOADLOGLISTOK								= 44,
	STR_WS_GAME_LOADLOGLISTFAIL								= 45,
	STR_WS_GAME_LOADGMLISTOK								= 46,
	STR_WS_GAME_LOADGMLISTFAIL								= 47,
	STR_WS_GAME_LOADSCRIPTLISTOK							= 48,
	STR_WS_GAME_LOADSCRIPTLISTFAIL							= 49,
	STR_WS_GAME_LOADRGNLEVELLISTOK							= 50,
	STR_WS_GAME_LOADRGNLEVELLISTFAIL						= 51,
	STR_WS_GAME_LOADHITLEVELLISTOK							= 52,
	STR_WS_GAME_LOADHITLEVELLISTFAIL						= 53,
	STR_WS_GAME_LOADBROADCASTLISTOK							= 54,
	STR_WS_GAME_LOADCITYWARTIMEOK							= 55,
	STR_WS_GAME_LOADINVALIDWORDOK							= 56,
	STR_WS_GAME_LOADFACTIONPARAMOK							= 57,
	STR_WS_GAME_LOADVILWARPARAMTIMEOK						= 58,
	STR_WS_GAME_LOADCITYWARPARAMOK							= 59,
	STR_WS_GAME_LOADFACTIONWARTIMEOK						= 60,
	STR_WS_GAME_LOADQUESTPARAMOK							= 61,
	STR_WS_GAME_LOADQUESTPARAMFAIL							= 62,
	STR_WS_GAME_LOADCOUNTRYPARAMOK							= 63,
	STR_WS_GAME_LOADCOUNTRYPARAMFAIL						= 64,
	STR_WS_GAME_LOADINCGOODSOK								= 65,
	STR_WS_GAME_LOADINCGOODSFAIL							= 66,
	STR_WS_GAME_LOADCOUNTRYCONTRIBUTEGOODSOK				= 67,
	STR_WS_GAME_LOADCOUNTRYCONTRIBUTEGOODSFAIL				= 68,
	STR_WS_GAME_LOADDOCCUOK									= 69,
	STR_WS_GAME_LOADDOCCUFAIL								= 70,
	STR_WS_GAME_CONNECTLSOK									= 71,
	STR_WS_GAME_CONNECTDBSOK								= 72,
	STR_WS_GAME_CONNECTLOGSOK								= 73,
	STR_WS_GAME_CONNECTCOSTSOK								= 74,
	STR_WS_GAME_RECONNECTLSOK								= 75,
	STR_WS_GAME_RECONNECTDBSOK								= 76,
	STR_WS_GAME_RECONNECTLOGSOK								= 77,
	STR_WS_GAME_NETSERVERINITOK								= 78,
	STR_WS_GAME_WSFORGSINITOK								= 79,
	STR_WS_GAME_LOADWSSETUPOK								= 80,
	STR_WS_GAME_LOADRGNOK									= 81,
	STR_WS_GAME_LOADRGNFAIL									= 82,
	STR_WS_GAME_LOADRGNSETUPOK								= 83,
	STR_WS_GAME_MONSTERNUMINWS								= 84,
	STR_WS_GAME_GSLOST										= 85,
	STR_WS_GAME_UNKOWNGSLOST								= 86,
	STR_WS_GAME_QUESTPROPERTYBYRGNFAIL						= 87,
	STR_WS_GAME_QUESTPROPERTYBYGSFAIL						= 88,
	STR_WS_GAME_LOADOCCUATTRLISTOK							= 89,
	STR_WS_GAME_LOADOCCUATTRLISTFAIL						= 90,
	STR_WS_GAME_LOADSYSMAILOK								= 91,
	STR_WS_GAME_LOADSYSMAILFAIL								= 92,
	STR_WS_GAME_PM_OK										= 93,	// ������ҹ�������...ok!
	STR_WS_GAME_PM_FAILED									= 94,	// ������ҹ�������...failed!
	STR_WS_GAME_PET_OK										= 95,	// �����������...ok!
	STR_WS_GAME_PET_FAILED									= 96,	// �����������...failed!
	STR_WS_GAME_OCCUEXP_OK									= 97,	// �����������...ok!
	STR_WS_GAME_OCCUEXP_FAILED								= 98,	// �����������...failed!
    STR_WS_GAME_ANTI_CHEAT_OK                               = 99,   // ������������...ok!
	STR_WS_GAME_ANTY_CHEAT_FAILED                           = 100,  // ������������...failed!
	STR_WS_GAME_LOADCREDITSYSTEM_OK							= 101,
	STR_WS_GAME_LOADCREDITSYSTEM_FAILED						= 102,
	STR_WS_GAME_LOAD_COSMETOLOGY_OK							= 103,
	STR_WS_GAME_LOAD_COSMETOLOGY_FAILED						= 104,
	STR_WS_GAME_LOAD_QUESTINDEX_OK							= 105,
	STR_WS_GAME_LOAD_QUESTINDEX_FAILED						= 106,
	STR_WS_GAME_LOAD_CLOTTERYXML_OK							= 107,
	STR_WS_GAME_LOAD_CLOTTERYXML_FAILED						= 108,
};															
// GS�����������ַ�����ԴID									
enum														
{															
	STR_GS_RGNMANAGER_MAXCREATEPRGNNUM = 0,
	STR_GS_RGNMANAGER_MAXCREATETRGNNUM						= 1,
	STR_GS_RGNMANAGER_CREATEPRGN							= 2,
	STR_GS_RGNMANAGER_INITPRGN								= 3,
	STR_GS_RGNMANAGER_CREATEPHRGN							= 4,
	STR_GS_RGNMANAGER_INITPHRGN								= 5,
	STR_GS_RGNMANAGER_CREATETRGN							= 6,
	STR_GS_RGNMANAGER_INITTRGN								= 7,
	STR_GS_RGNMANAGER_ERRRGNOWNERNOTFOUND					= 8,
	STR_GS_RGNMANAGER_ERRRGNCREATEDFAILED					= 9,
	STR_GS_RGNMANAGER_DELPRGNOK								= 10,
	STR_GS_RGNMANAGER_DELPHRGNOK							= 11,
	STR_GS_RGNMANAGER_DELTRGNOK								= 12,
	STR_GS_RGNMANAGER_DELPHRGNGOODSOK						= 13,
};															 
// GS�ű������ַ�����ԴID									
enum														
{															
	STR_GS_FUNCTION_PARSEPARAMNOARRAYBLOCK = 0,				
	STR_GS_FUNCTION_ERRARRAYSUFFIXOUTBOUND					= 1,
	STR_GS_FUNCTION_ITERATEPARAMERR							= 2,
	STR_GS_FUNCTION_DEFINEVARNOTINT							= 3,
	STR_GS_FUNCTION_DEFINEVARNOTSTR							= 4,
	STR_GS_FUNCTION_SCRIPTFILENODEFLINE						= 5,
	STR_GS_FUNCTION_SETATTRFIRSTVARISSTRING					= 6,
	STR_GS_FUNCTION_CHANGEATTRFIRSTVARISSTRING				= 7,
	STR_GS_FUNCTION_SETMSATTRFIRSTVARISSTRING				= 8,
	STR_GS_FUNCTION_GETMSATTRFIRSTVARISSTRING				= 9,
	STR_GS_FUNCTION_CHANGEMSATTRFIRSTVARISSTRING			= 10,
	STR_GS_FUNCTION_SETGOODSATTREXFIRSTVARISSTRING			= 11,
	STR_GS_FUNCTION_GETGOODSATTREXFIRSTVARISSTRING			= 12,
	STR_GS_FUNCTION_CHANGEGOODSATTREXFIRSTVARISSTRING		= 13,
};
// GSϵͳ��Ϣ�ַ�����ԴID
enum
{
	STR_GS_SERVERMESSAGE_INITGLOBALSTRING = 0,
	STR_GS_SERVERMESSAGE_INITGOODSLIST						= 1,
	STR_GS_SERVERMESSAGE_INITSUITSETUP						= 2,
	STR_GS_SERVERMESSAGE_INITHOLESETUP						= 3,
	STR_GS_SERVERMESSAGE_INITCOMMERCESETUP					= 4,
	STR_GS_SERVERMESSAGE_INITMONSTERLIST					= 5,
	STR_GS_SERVERMESSAGE_INITGOODSDROPLIST					= 6,
	STR_GS_SERVERMESSAGE_INITPLAYERLIST						= 7,
	STR_GS_SERVERMESSAGE_INITPLAYEREXPLIST					= 8,
	STR_GS_SERVERMESSAGE_INITSKILLLIST						= 9,
	STR_GS_SERVERMESSAGE_INITSKILLSTUDYOCCULIST				= 10,
	STR_GS_SERVERMESSAGE_INITSKILLSTUDYLIST					= 11,
	STR_GS_SERVERMESSAGE_INITCOMMERCEGOODSLIST				= 12,
	STR_GS_SERVERMESSAGE_INITINCGOODSLIST					= 13,
	STR_GS_SERVERMESSAGE_INITCOUNTRYCONTRIBUTEGOODSDROP		= 14,
	STR_GS_SERVERMESSAGE_INITGLOBALPARAMLIST				= 15,
	STR_GS_SERVERMESSAGE_LOGSERVERPARAMLIST					= 16,
	STR_GS_SERVERMESSAGE_SCRIPTFUNCLIST						= 17,
	STR_GS_SERVERMESSAGE_SCRIPTVARLIST						= 18,
	STR_GS_SERVERMESSAGE_GLOBALSCRIPTVARLIST				= 19,
	STR_GS_SERVERMESSAGE_GETQUESTLIST						= 20,
	STR_GS_SERVERMESSAGE_GETDOCCULIST						= 21,
	STR_GS_SERVERMESSAGE_NOTFINDPHRGNTEMPLATEID				= 22,
	STR_GS_SERVERMESSAGE_INITPHRGNOK						= 23,
	STR_GS_SERVERMESSAGE_GETGMLISTOK						= 24,
	STR_GS_SERVERMESSAGE_STARTNORMALRGNOK					= 25,
	STR_GS_SERVERMESSAGE_STARTPRGNOK						= 26,
	STR_GS_SERVERMESSAGE_STARTPHRGNOK						= 27,
	STR_GS_SERVERMESSAGE_STARTTRGNOK						= 28,
	STR_GS_SERVERMESSAGE_ADDPROXYRGNOK						= 29,
	STR_GS_SERVERMESSAGE_RELOADRGNSETUPOK					= 30,
	STR_GS_SERVERMESSAGE_RELOADRGNSETUPEXOK					= 31,
	STR_GS_SERVERMESSAGE_INITRGNLEVELLISTOK					= 32,
	STR_GS_SERVERMESSAGE_INITHITLEVELLISTOK					= 33,
	STR_GS_SERVERMESSAGE_INITSERVERNETFAILED				= 34,
	STR_GS_SERVERMESSAGE_GAMEWORLDMONSTERNUM				= 35,
	STR_GS_SERVERMESSAGE_INITFACELIST						= 36,
	STR_GS_SERVERMESSAGE_LOADCOUNTRYPARAMOK					= 37,
	STR_GS_SERVERMESSAGE_LOADCOUNTRYINFOOK					= 38,
	STR_GS_SERVERMESSAGE_LOADDUPRGNOK						= 39,
	STR_GS_SERVERMESSAGE_LOADCITYWARINFOOK					= 40,
	STR_GS_SERVERMESSAGE_LOADVILWARINFOOK					= 41,
	STR_GS_SERVERMESSAGE_LOADGOODSEXSETUPOK					= 42,
	STR_GS_SERVERMESSAGE_LOADGOODSEXSETUPFAILED				= 43,
	STR_GS_SERVERMESSAGE_LOADDEPOTSETUPOK					= 44,
	STR_GS_SERVERMESSAGE_LOADRECVFRIENDSYSSETUPOK			= 45,
	STR_GS_SERVERMESSAGE_RECONNECTINFOFROMWSOK				= 46,
	STR_GS_SERVERMESSAGE_RECONNECTINFOFROMDBSOK				= 47,
	STR_GS_SERVERMESSAGE_SENDSERVERSAVEPLAYERDATASIZE		= 48,
	STR_GS_SERVERMESSAGE_CREATEPHRGNOK						= 49,
	STR_GS_SERVERMESSAGE_SYSMAIL							= 50,
	STR_GS_SERVERMESSAGE_CANTFINDRELEASEGOODSOWNER			= 51,
	STR_GS_SERVERMESSAGE_CANTFINDRELEASEGOODS				= 52,
	STR_GS_SERVERMESSAGE_LEVELCANTUSEAREASPEAK				= 53,
	STR_GS_SERVERMESSAGE_CANTSPEAKWITHOUTSOMETING			= 54,
	STR_GS_SERVERMESSAGE_CANTSPEAKWITHOUTMONEY				= 55,
	STR_GS_SERVERMESSAGE_CANTMMMWITHOUTPLAYERONLINE			= 56,
	STR_GS_SERVERMESSAGE_PWDERRJOIN							= 67,
	STR_GS_SERVERMESSAGE_COSMETOLOTY						= 73,
    STR_GS_SERVERMESSAGE_RELIVETIMER                        = 74,
    STR_GS_SERVERMESSAGE_RETURNCITYTIMER                    = 75,
};															
// GS�����ű��ַ�����ԴID									
enum														
{															
	STR_GS_RGNFUNC_ADDRGNVARERR = 0,
	STR_GS_RGNFUNC_SETRGNVARERR								= 1,
	STR_GS_RGNFUNC_DELRGNVARERR								= 2,
	STR_GS_RGNFUNC_GETRGNVARERR								= 3,
	STR_GS_RGNFUNC_VIEWRGNVARERR							= 4,
	STR_GS_RGNFUNC_VIEWRGNDATA								= 5,
	STR_GS_RGNFUNC_COUNTRYINFO								= 6,
};															
// GS�ű��ļ��ַ�����ԴID									
enum														
{															
	STR_GS_SCRIPT_STRVARBEENSETINTVALUE = 0,				
	STR_GS_SCRIPT_RETVALUEERR								= 1,
	STR_GS_SCRIPT_GUIDVARBEENSETINTVALUE					= 2,
	STR_GS_SCRIPT_CANTOPERWITHGUIDVAR						= 3,
};															
// DBSϵͳ��Ϣ�ַ�����ԴID									
enum														
{															
	STR_DBS_SERVERMESSAGE_WSCONNECTOK		= 0,			
	STR_DBS_SERVERMESSAGE_WSCONNECTFAIL		= 1,			
};

// DBS��Ʒ������־�ַ�����ԴID
enum
{
	STR_DBS_GOODSINCLOG_LOADALLFAIL = 0,
	STR_DBS_GOODSINCLOG_LOADFAIL			= 1,
	STR_DBS_GOODSINCLOG_SAVEROLLBACK		= 2,
	STR_DBS_GOODSINCLOG_SAVEFAIL			= 3,
	STR_DBS_GOODSINCLOG_SAVEERROR			= 4,
};											
// DBS���˷����ַ�����ԴID					
enum
{
	STR_DBS_DUPRGN_CNPTRNULL = 0,
	STR_DBS_DUPRGN_FINDDUPRGNPROPERTYERR		= 1,
	STR_DBS_DUPRGN_LOADRGNOBJERR				= 2,
	STR_DBS_DUPRGN_LOADRGNOK					= 3,
	STR_DBS_DUPRGN_LOADRGNERR					= 4,
	STR_DBS_DUPRGN_LOADONERGNOK					= 5,
	STR_DBS_DUPRGN_LOADONERGNFAIL				= 6,
	STR_DBS_DUPRGN_DELDUPRGNPROPERTYERR			= 7,
	STR_DBS_DUPRGN_SAVERGNOBJOK					= 8,
	STR_DBS_DUPRGN_SAVERGNOBJFAIL				= 9,
	STR_DBS_DUPRGN_DELRGNOBJOK					= 10,
	STR_DBS_DUPRGN_DELRGNOBJFAIL				= 11,
	STR_DBS_DUPRGN_FINDRGNGOODSERR				= 12,
	STR_DBS_DUPRGN_LOADRGNGOODSOK				= 13,
	STR_DBS_DUPRGN_LOADRGNGOODSFAIL				= 14,
	STR_DBS_DUPRGN_SAVERGNGOODSOK				= 15,
	STR_DBS_DUPRGN_SAVERGNGOODSFAIL				= 16,
	STR_DBS_DUPRGN_QUESTRGNGUIDOK				= 17,
	STR_DBS_DUPRGN_QUESTRGNGUIDFAIL				= 18,
};												
// DBS��Ʒ�ַ�����ԴID							
enum
{
	STR_DBS_GOODS_CNPTRNULL = 0,
	STR_DBS_GOODS_SAVEGOODSERR					= 1,
	STR_DBS_GOODS_DELEQUIPMENTERR				= 2,
	STR_DBS_GOODS_DELORIPACKERR					= 3,
	STR_DBS_GOODS_DELDEPOTERR					= 4,
	STR_DBS_GOODS_DELSUBPACKERR					= 5,
	STR_DBS_GOODS_CLEARALLFAIL					= 6,
	STR_DBS_GOODS_LOADALLOCERR					= 7,
	STR_DBS_GOODS_LOADERR						= 8,
};												
// DBS����ַ�����ԴID							
enum											
{												
	STR_DBS_PLAYER_CNPTRNULL = 0,				
	STR_DBS_PLAYER_OPENBASEERR								= 1,	
	STR_DBS_PLAYER_UPDATELOGOUTTIMEERR						= 2,
	STR_DBS_PLAYER_DELPROPERTYERR							= 3,
	STR_DBS_PLAYER_CHECKNAMEERR								= 4,
	STR_DBS_PLAYER_NAMEIS									= 5,
	STR_DBS_PLAYER_X										= 6,
	STR_DBS_PLAYER_N										= 7,
	STR_DBS_PLAYER_GETNAMEBYIDERR							= 8,
	STR_DBS_PLAYER_GETIDERR									= 9,
	STR_DBS_PLAYER_GETCDKEYERR								= 10,
	STR_DBS_PLAYER_CREATEBASEROLLBACK						= 11,
	STR_DBS_PLAYER_CREATEBASESTARTTRANSIMITFAIL				= 12,
	STR_DBS_PLAYER_CREATEPROPERTYERR						= 13,
	STR_DBS_PLAYER_LOADPROPERTYERR							= 14,
	STR_DBS_PLAYER_SAVEBASEOK								= 15,
	STR_DBS_PLAYER_SAVEBASEERR								= 16,
	STR_DBS_PLAYER_SAVEBASEROLLBACK							= 17,
	STR_DBS_PLAYER_SAVEBASESTARTTRANSIMITFAIL				= 18,
	STR_DBS_PLAYER_SAVEPROPERTYOK							= 19,
	STR_DBS_PLAYER_SAVEPROPERTYERR							= 20,
	STR_DBS_PLAYER_SAVEPROPERTYROLLBACK						= 21,
	STR_DBS_PLAYER_SAVEPROPERTYSTARTTRANSIMITFAIL			= 22,
	STR_DBS_PLAYER_SAVEALLOK								= 23,
	STR_DBS_PLAYER_SAVEALLERR								= 24,
	STR_DBS_PLAYER_SAVEROLLBACK								= 25,
	STR_DBS_PLAYER_SAVEALLTARTTRANSIMITFAIL					= 26,
	STR_DBS_PLAYER_SAVESKILLHOTKEYERR						= 27,
	STR_DBS_PLAYER_SAVESKILLHOTKEYROLLBACK					= 28,
	STR_DBS_PLAYER_SAVESKILLHOTKEYSTARTTRANSIMITFAIL		= 29,
	STR_DBS_PLAYER_SAVESKILLHOTKEYINFOERR					= 30,
	STR_DBS_PLAYER_SAVESKILLHOTKEYINFOOK					= 31,
	STR_DBS_PLAYER_SAVESKILLHOTKEYINFOFAIL					= 32,
	STR_DBS_PLAYER_LOADSKILLOK								= 33,
	STR_DBS_PLAYER_LOADSKILLFAIL							= 34,
	STR_DBS_PLAYER_SAVESKILLOK								= 35,
	STR_DBS_PLAYER_SAVESKILLFAIL							= 36,
	STR_DBS_PLAYER_SAVESKILLROLLBACK						= 37,
	STR_DBS_PLAYER_SAVESKILLSTARTTRANSIMITFAIL				= 38,
	STR_DBS_PLAYER_SAVESKILLINFOFAIL						= 39,
	STR_DBS_PLAYER_THREADSAVESTATEFAIL						= 40,
	STR_DBS_PLAYER_THREADSAVESTATEROLLBACK					= 41,
	STR_DBS_PLAYER_THREADSAVESTATESTARTTRANSIMITFAIL		= 42,
	STR_DBS_PLAYER_THREADSAVESTATEOK						= 43,
	STR_DBS_PLAYER_SAVESTATEFAIL							= 44,
	STR_DBS_PLAYER_SAVESTATEOK								= 45,
	STR_DBS_PLAYER_LOADSTATEFAIL							= 46,
	STR_DBS_PLAYER_LOADSTATEOK								= 47,
	STR_DBS_PLAYER_THREADSAVESCRIPTFAIL						= 48,
	STR_DBS_PLAYER_THREADSAVESCRIPTROLLBACK					= 49,
	STR_DBS_PLAYER_THREADSAVESCRIPTSTARTTRANSIMITFAIL		= 50,
	STR_DBS_PLAYER_SAVESCRIPTFAIL							= 51,
	STR_DBS_PLAYER_LOADSCRIPTOK								= 52,
	STR_DBS_PLAYER_LOADSCRIPTFAIL							= 53,
	STR_DBS_PLAYER_LOADNULLPLAYER							= 54,
	STR_DBS_PLAYER_LOADPLAYERNUMFAIL						= 55,
	STR_DBS_PLAYER_GETDELTIMEFAIL							= 56,
	STR_DBS_PLAYER_GETLASTLOGOUTTIMEFAIL					= 57,
	STR_DBS_PLAYER_RESTORECHARROLLBACK						= 58,
	STR_DBS_PLAYER_RESTORECHARSTARTTRANSIMITFAIL			= 59,
	STR_DBS_PLAYER_RESTORECHARERR							= 60,
	STR_DBS_PLAYER_CHECKGUIDBYCDKEYFAIL						= 61,
	STR_DBS_PLAYER_LOADQUESTOK								= 62,
	STR_DBS_PLAYER_LOADQWUESTERR							= 63,
	STR_DBS_PLAYER_SAVEQUESTOK								= 64,
	STR_DBS_PLAYER_SAVEQUESTFAIL							= 65,
	STR_DBS_PLAYER_LOADQUESTINFOOK							= 66,
	STR_DBS_PLAYER_LOADQUESTINFOERR							= 67,
	STR_DBS_PLAYER_SAVEDOCCUOK								= 68,
	STR_DBS_PLAYER_SAVEDOCCUFAIL							= 69,
	STR_DBS_PLAYER_LOADLINKMANFAIL							= 70,
	STR_DBS_PLAYER_CLEARLINKMANERR							= 71,
	STR_DBS_PLAYER_SAVELINKMANERR							= 72,
	STR_DBS_PLAYER_UPDATELINKMANOK							= 73,
	STR_DBS_PLAYER_THREADSAVEINCLOGFAIL						= 74,
	STR_DBS_PLAYER_SAVEINCLOGROLLBACK						= 75,
	STR_DBS_PLAYER_SAVEINCLOGSTARTTRANSIMITFAIL				= 76,
	STR_DBS_PLAYER_SAVEINCLOGFAIL							= 77,
	STR_DBS_PLAYER_LOADINCLOGOK								= 78,
	STR_DBS_PLAYER_LOADINCLOGFAIL							= 79,
	STR_DBS_PLAYER_PRELOADBASEFAIL							= 80,
	STR_DBS_PLAYER_THREADSAVEMERQUESTFAIL					= 81,
	STR_DBS_PLAYER_THREADSAVEMERQUESTROLLBACK				= 82,
	STR_DBS_PLAYER_THREADSAVEMERQUESTSTARTTRANSIMITFAIL		= 83,
	STR_DBS_PLAYER_THREADSAVEMERQUESTOK						= 84,
	STR_DBS_PLAYER_LOADMERQUESTOK							= 85,
	STR_DBS_PLAYER_LOADMERQUESTFAIL							= 86,
};
// DBS�����߳��ַ�����ԴID
enum														
{															
	STR_DBS_SAVETHREAD_SAVE		= 0,
	STR_DBS_SAVETHREAD_LOAD									= 1,
	STR_DBS_SAVETHREAD_OK									= 2,
	STR_DBS_SAVETHREAD_FAIL									= 3,
	STR_DBS_SAVETHREAD_PLAYERNAME							= 4,
	STR_DBS_SAVETHREAD_BASEDATA								= 5,
	STR_DBS_SAVETHREAD_PROPERTY								= 6,
	STR_DBS_SAVETHREAD_SKILLHOTKEY							= 7,
	STR_DBS_SAVETHREAD_SKILL								= 8,
	STR_DBS_SAVETHREAD_STATE								= 9,
	STR_DBS_SAVETHREAD_SCRIPT								= 10,
	STR_DBS_SAVETHREAD_QUEST								= 11,
	STR_DBS_SAVETHREAD_DOCCU								= 12,
	STR_DBS_SAVETHREAD_LINKMAN								= 13,
	STR_DBS_SAVETHREAD_GOODSINCLOG							= 14,
	STR_DBS_SAVETHREAD_MERQUEST								= 15,
	STR_DBS_SAVETHREAD_GOODS								= 16,
};															
															
//! �����ַ���ID
static const LONG TEAM_STRING_TYPEID	= 16;
//! �����ַ���ö��
enum eTeamStringID
{															
	eTSID_Warning						= 0,	//! ����"/>	
	eTSID_AimHadTeam					= 10,	//! �Է����ж��顣"/>
	eTSID_TeamIsFull					= 20,	//! ��������������"/>
	eTSID_PleaseWait					= 30,	//! ���Ѿ����������룬�����ĵȴ��Է��𸴡�"/>
	eTSID_OutTime						= 40,	//! �����ѹ��ڡ�"/>
	eTSID_YouHadTeam					= 50,	//! ���Ѿ���������������"/>
	eTSID_CreateFailedAndTautology		= 60,	//! ���鴴��ʧ�ܣ����Ժ����ԡ�"/>
	eTSID_JoinFailedAndTautology		= 70,	//! �������ʧ�ܣ����Ժ����ԡ�"/>
	eTSID_AimRefuse						= 80,	//! �Է��ܾ�������顣"/>
	eTSID_AimNotOnlineOrBeing			= 90,	//! Ŀ�겻���߻򲻴���"/>
	eTSID_Busyness						= 100,	//! Ŀ�겻���߻򲻴���"/>
	eTSID_NotOpenTeam					= 110,	//! �Է�û�п���"/>
	eTSID_AimIsDead						= 120,	//! �Է�������״̬"/>
	eTSID_OperatorNotMaster				= 130,	//! �㲻�Ƕӳ���������������"/>
	eTSID_AimHideName					= 140,	//! �����Ŀǰ�����ڡ������ߡ�״̬���޷�ִ����Ӳ�����"/>
};

//! <FileTable  typeid="10" name="IDS_WS_GOODSEX"   file="textresource/GoodsEx.xml"/>
static const LONG GOODSEX_STRING_TYPEID = 10;
//! ��Ʒ��չ�ַ���ö��
//! �ⲿ�־���̫����������ļ�����ʽ��ʾ���ı�λ��
enum eGoodsExStringID
{
	WS_GoodsEx_10						= 10,//! ����"/>
	WS_GoodsEx_20						= 20,//! ��ʼ���ֽ��䷽����δ�ҵ�ԭʼ��Ϊ����"/>
	WS_GoodsEx_30						= 30,//! ������Ʒ"/>
	WS_GoodsEx_40						= 40,//! ��ʼ���ֽ��䷽����ԭʼ��Ϊ����"/>
	WS_GoodsEx_50						= 50,//! ������Ʒ�������ˡ���ƷƷ�ʡ����ԣ��������ˡ������������ԡ���"/>
	WS_GoodsEx_60						= 60,//! ��ȡ���ֽ���Ʒ���á��ɹ���"/>

	WS_GoodsEx_70						= 70,//! ��ʼ���ϳ��䷽����δ�ҵ�ԭʼ��Ϊ����"/>
	WS_GoodsEx_80						= 80,//! ��ʼ���ϳ��䷽����ԭʼ��Ϊ����"/>
	WS_GoodsEx_90						= 90,//! �ϳɡ��ֽ���Ʒ���á��ɹ���"/>

	WS_GoodsEx_100						= 100,//! ��ʼ����װ���������ó���û���ҵ�ԭʼ��Ϊ��"/>
	WS_GoodsEx_110						= 110,//! ���ĳ�Ȳ���"/>
	WS_GoodsEx_120						= 120,//! ��ȡ��װ���������á��ɹ���"/>

	WS_GoodsEx_130						= 130,//! ��ʼ���ֳ�ȸ����������ó���û���ҵ�ԭʼ��Ϊ��"/>
	WS_GoodsEx_140						= 140,//! ��ȡ��������Ʒ���á��ɹ���"/>

	WS_GoodsEx_150						= 150,//! ��ʼ��ħ�����Ϻϳ����ó���û���ҵ�ԭʼ��Ϊ��"/>
	WS_GoodsEx_160						= 160,//! ����ħ������"/>
	WS_GoodsEx_170						= 170,//! ��ʼ��ħ�����Ϻϳ����ó���ԭʼ��Ϊ����"/>
	WS_GoodsEx_180						= 180,//! ������Ʒ�������ˡ���ƷƷ�ʡ����ԣ��������ˡ������������ԡ���"/>
	WS_GoodsEx_190						= 190,//! ��ȡ��ħ�����Ϻϳɼ������á��ɹ���"/>
	WS_GoodsEx_200						= 200,//! ��ȡ��ħ�����á��ɹ���"/>

	WS_GoodsEx_210						= 210,//! ������Ʒ��չ����...�ɹ���"/>
	WS_GoodsEx_220						= 220,//! ������Ʒ��չ����...ʧ�ܣ�����"/>	
};


//! ���壺�����ַ���
enum ENUM_IDS_PUBLIC
{
	eIDS_ChineseZero					= 0,	//! 

	eIDS_ChineseFu						= 10,	//! 
	eIDS_ChineseGe,								//! 
	eIDS_ChineseShi,							//! 
	eIDS_ChineseBai,							//! 
	eIDS_ChineseQian,							//! 
	eIDS_ChineseWan,							//! 
	eIDS_ChineseYi,								//! 

	eIDS_GoodsHasLocked					= 101,	//! "���Ѿ�ѡ���˸���Ʒ�������Ʒ���������ط���������"
	eIDS_InputGoodsNum,							//! "��������Ʒ����"
	eIDS_GoodsDecordError,						//! "��Ʒ����ʧ�ܣ�"

	eIDS_JuBaoError                      = 104, //! ����ͬ�ľٱ���Ϣʱ�����,�ٱ���Ϣ���ڴ���,лл
	eIDS_JuBaoSucceed,                          //! лл���ṩ����Ϣ,���ǻ��ڵ�һʱ���������Ϣ���в�֤����
	eIDS_GameName                      = 120,  //! �ǳ���˵
	eIDS_SDOALogin                     = 121,  //! "[ʢ��ͨ��֤�û���½]"/>    
	eIDS_SDOALoginError                = 122,  //! "��½ȦȦ����:%d"
	eIDS_ErrorInfo                     = 123,  //! "������Ϣ(%d)""
	eIDS_OpenUrl                       = 124,  //! "����վ:%s\n"
	eIDS_SDOAInfoTOLogin               = 125,  //! "��½�ɹ���ſ���!"
	eIDS_ServerError                   = 126,  //! ""������δ��Ӧ,�Ƿ�����ȴ�?"
	eIDS_TimeOver                      = 127,  //! "����ʱ,���Ժ�����!"/>"
	eIDS_QueeInfo                      = 128,  //! ���ڽ�����Ϸ......\n���е�%dλ,������ʱ%d��."/>
	eIDS_LoginError                    = 129,  //! "���ӷ�����ʧ��,���Ժ�����!"
	eIDS_LoadLater                     = 130,  //! "���Ժ��½��"
	eIDS_InputAllInfo                  = 131,  //! "����д��������Ϣ��"

	// ͨ�ô���
	eIDS_CantFindAim					= 10001,//! "û�пɲ�������Ʒ��"
	eIDS_SrcIsLock,								//! "����Ʒ�������ط��������ˣ�"
	eIDS_SessionLapse,							//! "�Ի������Զ���Ѿ�ʧЧ��"
	eIDS_ErrOperation,							//! "����Ĳ���ֵ��"
	eIDS_ErrAssistant,							//! "�������ϴ���"
	eIDS_CantOperateAim,						//! "Ŀ�겻�����˲�����"
	eIDS_DelAssistantFailed,					//! "ɾ����������ʧ�ܣ�"
	eIDS_NoEnoughSpace,							//! "����Ҫ�ճ� %d �񱳰��ռ䣡"
	eIDS_NotEnoughMoney,						//! "û���㹻�Ľ�Ǯ��ɲ�����"
	eIDS_ErrStuff,								//! "���ϴ���"
	eIDS_NumError,								//! "������������"
	eIDS_NoEnoughSpace2,						//! "���ı����ռ䲻�㣡"
	eIDS_NotUpgrade,                            //! û��ǿ���޷����и�ħ����
	eIDS_UpgradeLow,                            //! ����Ʒ�Ѵﵽ��Ʒǿ���ȼ�������ĸ�ħ�ȼ����ޣ���һ����ħ��Ҫ������ǿ���ȼ�
};


//! ���壺ħ���ַ���ö��
enum ENUM_IDS_MAGIC_BOX
{
	eIDS_CantHandleTheGoodsOnWear		= 1,	//! "���������ϵ�װ������ִ�кϳɲ�����"
	eIDS_NormalComposeSucceed,					//! "��Ʒ�ϳɳɹ���"
	eIDS_ComposeConfigError,					//! "�ϳ��䷽����"
	eIDS_ComposeConfigNotExist,					//! "�ϳ��䷽�����ڣ�"
	eIDS_CantComposeTheGoodsWasFreezed,			//! "���ܺϳɶ������Ʒ��"
	eIDS_AreYouSureComposeTheseGoods,			//! "��ȷ��Ҫ�����䷽�ϳ���Ʒ��"
	eIDS_ComposeConfigErrorCantHandle,			//! "�䷽���󣬲��ܲ�����"
	eIDS_InputTheGoodsFirst,					//! "���ȷ�����Ҫ��������Ʒ��"

	eIDS_AreYouSureComposeMagicGoods	= 100,	//! "��ȷ��Ҫ���ƴ�ħ��������"
	eIDS_MagicComposeSucceed,					//! "ħ�����Ϻϳɳɹ���"
	eIDS_MagicComposeFailed,					//! "ħ�����Ϻϳ�ʧ�ܣ�"
	eIDS_MagicGoodsTopLevel,					//! "ħ�������Ѿ��ﵽ��ߺϳɵȼ���"

	eIDS_AreYouSureUnBind				= 200,	//! "��ȷ��Ҫ ������Ʒ��"
	eIDS_AreYouSureBind,						//! "��ȷ��Ҫ �󶨴���Ʒ��"
	eIDS_CantBind,								//! "����Ʒ�޷����а󶨣�"
	eIDS_CantUnBind,							//! "����Ʒ�޷����н��"
	eIDS_BindSucceed,							//! "��Ʒ�󶨳ɹ���"
	eIDS_UnBindSucceed,							//! "��Ʒ���ɹ���"
	eIDS_CantBindTheFreezeGoods,				//! "���ܶԶ�����Ʒ���� �󶨣���� ������"

	eIDS_AreYouSureUnFreeze				= 300,	//! "��ȷ��Ҫ �ⶳ����Ʒ��"
	eIDS_AreYouSureFreeze,						//! "��ȷ��Ҫ �������Ʒ��"
	eIDS_FreezeSucceed,							//! "��Ʒ����ɹ���"
	eIDS_UnFreezeSucceed,						//! "��Ʒ�ⶳ�ɹ���"
	eIDS_FreezeNeedBindFirst,					//! "���Ȱ���Ʒ��ſɽ��ж��������"
};


//! ���壺��Ʒ�ֽ� �ַ���
enum ENUM_IDS_DECOMPOSE
{
	eIDS_AreYouSureDecompose			= 1,	//! "��ȷ��Ҫִ�зֽ������"
	eIDS_WithoutDecomposeGoods,					//! "û�пɷֽ����Ʒ��"
	eIDS_GoodsCantDecompose,					//! "����Ʒ���ɷֽ⣡"
	eIDS_GoodsOnWearCantDecompose,				//! "���������ϵ�װ������ִ�зֽ������"
	eIDS_DecomposePreviwFailed,					//! "��ȡ�ֽ�Ԥ�����ʧ�ܣ�"
	eIDS_DecomposeSucceed,						//! "��Ʒ�ֽ�ɹ���"
	eIDS_AreYouSureDecomposeWithCard,			//! "����Ʒ��Ƕ�п�Ƭ����������ֽ⽫���¿�Ƭ��ʧ���Ƿ������"
	eIDS_CantDecomposeFreezeGoods,				//! "����Ʒ�ѱ����ᣬ�ⶳ��ſɷֽ⣡"
	eIDS_DecomposeDesc_1,						//! "��"
};


//! ���壺װ��ħ�� �ַ���
enum ENUM_IDS_EQUIP_MAGIC
{
	eIDS_AreYouSureMagicEquip			= 1,	//! "��ȷ����Ҫħ����"
	eIDS_NotEnoughMoneyMagicEquip,				//! "��û���㹻�Ľ�Ǯ�����ħ����"

	eIDS_InputMagicStuff,						//! "���ȷ���ħ��������ϣ�"
	eIDS_InputMagicEquip,						//! "���ȷ�����Ҫħ����װ����"

	eIDS_GetMagicStuffCfgError,					//! "��ȡ��Ӧ��ħ������ʧ�ܣ�"
	eIDS_GetMagicCfgError,						//! "��ȡħ������ʧ�ܣ�"
	eIDS_NotEnoughQualityForMagic,				//! "��װ����Ʒ��̫���޷�����ħ����"
	eIDS_EquipHasBeenTopMagicLevel,				//! "��װ���Ѿ�ħ������ߵȼ����޷��ټ���ħ���ˣ�"
	eIDS_MagicEquipNeed80Levle,					//! "����Ҫ 80 �����ϵ�װ�����ܽ���ħ����"
	eIDS_CantMagicTheEquipOnWear,				//! "���������ϵ�װ������ֱ��ִ��ħ��������"
	eIDS_EquipCantMagic,						//! "����Ʒ�޷�����ħ����"
	eIDS_NeedMoreMagicStuff,					//! "������Ҫ���� %d �� %s��"

	eIDS_NOT_PutDown         = 15,                      //! "�õ����޷�������"
	eIDS_Select_Comp         = 16,                       //! "ѡ����Ҫ��ħ����λ"
	eIDS_NeedMagicEquip      = 17,                      //! "�������Ҫ��ħ��װ��!"
	eIDS_NeedMagicStuff      = 18,                      //! "���ڸ���λ����%s!"
	eIDS_NeedXiangXingStuff  = 19,                    //! "����Ԫ�����Ծ�ʯ!"
	eDIS_SureEnchant         = 20,                      //! "ȷ�Ͻ��и���λԪ�����Ե�ǿ����"
	eDIS_SureChangeEnchant   = 21,                     //! "ȷ�Ͻ��и���λԪ�����Ե����ã�"
	eDIS_SureChangeXiangXing = 22,                     //! "ȷ�Ͻ��и���λԪ�����Եĸ�����"
	eDIS_Select_Enchant      = 30,                     //! "��ѡ��Ҫ��������λ"

	eDIS_MainGoodsError      = 50,                     //! "�����ϴ���"
	eDIS_HelpGoodsError      = 51,                     //! "�������ϴ���"
	eDIS_Not_Enchant         = 52,                     //! "Ŀ�겻�����˲���"
	eDIS_NumError            = 53,                     //! "������������"
    eDIS_ChangeStuffCanPutDown = 54,                     //! "��ֻ�ܷ���Ԫ������ת����ʯ"
    eDIS_MagicStuffNotEnogh = 55,                     //!

	eIDS_MagicPreview,							//! "����Ԥ��"

	eIDS_InputMagicStuffFirst			= 100,	//! "���ȷ�����Ӧ�ȼ��� ħ�����ϣ�"
	eIDS_MagicEquipSucceed,						//! "װ���Ѿ�ħ���ɹ���"

	eIDS_InputMagicEquipFirst,					//! "�뽫��Ҫħ����װ������װ��������"
	eIDS_InputOddsStuff,						//! "�뽫 [COLOR FF0000]%d �� %s[/COLOR] �����Ϸ� ����������"

	eIDS_InputEnchantStuff   = 104,             //! "�����һ��Ԫ�ؾ�ʯ"   
	eIDS_EnchantNote_30      = 105,             //! "����������Ҫװ��ħ����30%"
	eIDS_EnchantNote_60      = 106,             //! "����������Ҫװ��ħ����60%"
	eIDS_EnchantNote_90      = 107,             //! "����������Ҫװ��ħ����90%"
	eIDS_NeedEnchantGoods    = 109,             //! "û�з���װ�������Ա������"

	eIDS_EnchantStuff        = 130,             //! "Ԫ�����Ե���ʼλ������"
};



//! ���壺װ��ǿ�� �ַ���
enum ENUM_IDS_EQUIP_UPGRADE
{
	eIDS_AreYouSureUpgradeEquip			= 1,	//! "��ȷ����Ҫǿ����"
	eIDS_NotEnoughMoneyUpgradeEquip,			//! "��û���㹻�Ľ�Ǯ�����ǿ����"
	eIDS_InputUpgradeStoneFirst,				//! "���ȷ���ǿ�����豦ʯ��"
	eIDS_InputUpgradeEquipFirst,				//! "���ȷ�����Ҫǿ����װ����"

	eIDS_EquipHasBeenMaxUpgradeLevel,			//! "��װ���޷��ٽ���ǿ���ˣ�"
	eIDS_CantUpgradeTheEquipOnWear,				//! "���������ϵ�װ������ֱ��ִ��ǿ��������"
	eIDS_EquipCantUpgrade,						//! "����Ʒ�޷�����ǿ����"
	eIDS_UpgradeStuffError,						//! "����Ʒ���� %s��"
	eIDS_UpgradeStuffAmountError,				//! "%s ���ֻ��ʹ�� %d ����"
	eIDS_NeedNotOddsStone,						//! "ǿ����ǰ�ȼ���װ������Ҫʹ�� ������ʯ��"
	eIDS_TheGoodsIsntOddsStone,					//! "����Ʒ���� ������ʯ��"
	eIDS_EquipUpgradePreviewError,				//! "װ��ǿ��Ԥ��ʧ�ܣ�"

	eIDS_UpgradePreview,						//! "����Ԥ��"
	eIDS_StoneLvlNotMatching,					//! "��ʯ�ȼ�����"

	eIDS_UpgradeEquipDesc_1				= 100,	//! "��"
	eIDS_UpgradeEquipDesc_2,					//! "��"
	eIDS_UpgradeEquipDesc_3,					//! "��"
	eIDS_UpgradeEquipDesc_4,					//! "��"
	eIDS_UpgradeEquipDesc_5,					//! "��"
	eIDS_UpgradeEquipDesc_6,					//! "��"
};



//! ���壺ħ����� �ַ���
enum ENUM_IDS_MAGIC_DECOMPOSE
{
	eIDS_AreYouSureMagicDecompose		= 1,	//! "��ȷ��Ҫ��������"
	eIDS_GetMagicDecomposeCfgError,				//! "��ȡ��װ���Ĳ����Ͻṹ ʧ�ܣ�"
	eIDS_GetMagicInfoError,						//! "��ȡ��װ����ħ���ṹ ʧ�ܣ�"
	eIDS_MagicLevelIsZero,						//! "����Ʒ��δ�õ�Ʒ���������޷������ϣ�"
	eIDS_CantMagicDecomposeTheEquipOnWear,		//! "���������ϵ�װ������ִ�в�������"
	eIDS_DecomposeMagicStuffNum,				//! "���ɻ�� %d �� %s��"
	eIDS_NotEnoughMoneyMagicDecompose,			//! "û���㹻�Ľ�Ǯ��ɲ��ϲ�⣡"
	eIDS_NoMagicEquip,							//! "û��ѡ������ϵ�װ����"
	eIDS_EquipHasMagicDecomposeOver,			//! "װ���Ѿ���⵽��͵ȼ���"
	eIDS_MagicDecomposeSucceed,					//! "װ�����ɹ���"
	eIDS_InputMagicDecomposEquipFirst,			//! "���ȷ��������� ħ��װ����"

	eIDS_PeelPreview,							//! "����Ԥ��"
};




//! ���壺�ֿ� �ַ���
enum ENUM_IDS_DEPOT
{
	eIDS_DepotHasFreezed				= 1,	//! "���ڶ������������룬�ֿ��ѱ����ᣬ��ȴ��ⶳ��"
	eIDS_PasswordError,							//! "������󣬴�ʧ�ܣ�"
	eIDS_SetPasswordSucceed,					//! "�������óɹ���"
	eIDS_SetPasswordFailed,						//! "��������ʧ�ܣ�"

	eIDS_InputSaveAmountOfMoney,				//! "���� ��Ǯ����"
	eIDS_InputDrawAmountOfMoney,				//! "��ȡ ��Ǯ����"
	eIDS_InputSaveAmountOfDiamond,				//! "���� ��ʯ����"
	eIDS_InputDrawAmountOfDiamond,				//! "��ȡ ��ʯ����"

	eIDS_NotEnoughMoneyInPacket,				//! "������û����ô���Ǯ��"
	eIDS_NotEnoughMoneyInDepot,					//! "�ֿ���û����ô���Ǯ��"
	eIDS_NotEnoughDiamondInPacket,				//! "������û����ô�౦ʯ��"
	eIDS_NotEnoughDiamondInDepot,				//! "�ֿ���û����ô�౦ʯ��"

	eIDS_AreYouSureBuyExtItem,					//! "��ȷ��Ҫ���� %d �Ľ�Ǯ��������λ��"
	eIDS_HavntEnoughMoneyBuyExtItem,			//! "��Ǯ���� %d���޷��������λ��"
	eIDS_ExtItemHasSoldOut,						//! "�Ӳֿ����չ��λ�����꣡"
	eIDS_CantMoveSubPacketWhenHasGoods,			//! "�ӱ�����װ����Ʒ�����ƶ���"
	eIDS_CantMoveSubPacketWhenOpening,			//! "�򿪵��ӱ��������ƶ������ȹرգ�"
	eIDS_ExtItemOnlyCanPutSubPacket,			//! "����λֻ�ܷ����ӱ�����"
	eIDS_ExtItemHavntBought,					//! "����λ��δ����"
	eIDS_ExtItemCantExchangeNotSubPacket,		//! "�Ǳ�����Ʒ ���ܺͱ�����λ�ı������н�����"
	eIDS_Depot_Reserved1,						//! "��δ"
	eIDS_Depot_Reserved2,						//! "����"

	eIDS_BuyExtItemSucceed,						//! "������λ�ɹ���"
	eIDS_BuyExtItemFailed,						//! "������λʧ�ܣ�"

	eIDS_InputOldPWError,						//! "�������������"
	eIDS_InputTwicePWDiffer,					//! "������������벻һ�£�"
	eIDS_InputOldPW,							//! "�����������"
	eIDS_InputNewPW,							//! "������������"
	eIDS_InputPWAgain,							//! "���ٴ�����������"
	eIDS_InputDepotPW,							//! "��������������"

	eIDS_MoneyInDepotHasFull,					//! "�ֿ���װ������ô��Ľ�Ǯ��"
	eIDS_MoneyInPacketHasFull,					//! "������װ������ô��Ľ�Ǯ��"
	eIDS_SilverInDepotHasFull,					//! "�ֿ���װ������ô������ң�"
	eIDS_SilverInPacketHasFull,					//! "������װ������ô������ң�"
	eIDS_GoodsCantSaveInDepot,					//! "����Ʒ��ֹ����ֿ⣡"
	eIDS_PleaseCloseBeforeTidyDepot,			//! "��رձ���������ֿ⣡"
	eIDS_PleaseCloseBeforeTidyPack,			    //! "��رղֿ�����������"
};



//! ���壺��Ʊϵͳ �ַ���
enum ENUM_IDS_LOTTERY
{
	eIDS_Lottery_MaxTodayoint				= 1,	//! "%d   ��[COLOR FFF600]%s[/COLOR]��"
	eIDS_Lottery_ForceReset,						//! ǿ������

	eIDS_Lottery_Prompt_ResetPrizes			= 10,	//! "%d   ��[COLOR FFF600]%s[/COLOR]��"
	eIDS_Lottery_Prompt_ResetConstellations,		//! "%d   ��[COLOR FFF600]%s[/COLOR]��"
	eIDS_Lottery_Prompt_AddLotteryCost,				//! "�����Ժ���г齱�����������۳� %d ö����֮��"
	eIDS_Lottery_Tip_LotteryCost,					//! "���γ齱���۳� %d ö����֮��"

	eIDS_Lottery_TIP_Roulette				= 20,	//! ����˵��

	eIDS_Lottery_Describe					= 100,	//! ����˵��

	eIDS_Lottery_Error						= 1000	//! ����
};




//! hlc:monster.ini ���ô���
enum
{
	WS_MONSTERLIST_DataSlopOver=1,				//! "momster��ini �ļ��ַ�������"
	WS_MONSTERLIST_IndexDataSlopOver=2,			//! "��������Խ��"
};

//!��Ʒ
enum
{
	GS_GOODS_GOLD			=35,						//���
	GS_GOODS_SILVER			=36,						//����
};


//! hlc:����ϵͳ���ô���
enum
{
	WS_QS_OTIOSEAIM=1,						//"���������Ϣ����������Ŀ�������Ŀ������."
	WS_QS_OTIOSESTEPGAIN,					//"���������Ϣ������������Ʒ��ø�������Ʒ�������"
	WS_QS_OTIOSESTEP,						//"���������Ϣ�������������ڲ�������޶�"
	WS_QS_OTIOSEENCOURAGEMENT,				//"���������Ϣ�������������ڽ�������޶�"
	WS_QS_SYSNAMEERROR,						//"���������Ϣ�����������ϵͳ�����ļ���"
	WS_QS_SYSFILEERROR,						//"���������Ϣ������ϵͳ�����ļ�����"
	WS_QS_VOIDONEFILE,						//"���������Ϣ���������������ļ�������"
	WS_QS_ONEFILEERROR,						//"���������Ϣ���������������ļ�����"
	WS_QS_QUESTADDERROR,					//"���������Ϣ��������������б�ʱ����"
};

//! hlc:��������
enum
{
	GS_RGN_BringMonster=1,					//���=%d�Ĺ���û�л��������
	GS_RGN_NoMonsterBringPlace,				//�޷�ΪMonster ��%s�� �ҵ����ʵ�λ�ã�
	GS_RGN_BringCollection,					//���=%d�Ĳɼ���û�л��������
	GS_RGN_NoCollectionBringPlace,			//�޷�ΪCollection ��%s�� �ҵ����ʵ�λ�ã�
	GS_RGN_MonsterAddRoad,					//�޷�ΪMonster ��%s�� ���·����
	GS_RGN_NoNpcBringPlace,					//�޷�Ϊnpc ��%s�� �ҵ����ʵ�λ�ã�
};

//! ���壺����ϵͳ �ַ���
enum ENUM_IDS_FRIENDS
{
	eIDS_AreYouSureAddLinkman			= 1,	//! "��ȷ��Ҫ�� %s ���� %s �б���"
	eIDS_AreYouSureDelLinkman,					//! "��ȷ��Ҫ�� %s �� %s ��ɾ����"
	eIDS_AreYouSureAddFriendToo,				//! "���Ƿ�Ҳ�� %s ��Ϊ���ѣ�"
	eIDS_RequestYouAddFriend,					//! "%s �������Ϊ���ѣ�"
	eIDS_RejectYourAddRequest,					//! "%s �ܾ��������������"
	eIDS_AreYouSureDelFriendLinkman,			//! "��ȷ��Ҫ�� %s �� %s ��ɾ����\
												//   ȷ�Ϻ󣬽��޷��������ҽ���˽�ģ�ֻ�����½�����Ҽ�������б����ܼ�������˽�ģ�" 

	eIDS_UnknownLinkmanMap				= 101,	//! " - ������"
	eIDS_UnknownLinkmanLevel,					//! "�ȼ� ������"
	eIDS_LinkmanLevel,							//! "�ȼ� %d"
	eIDS_UnknownLinkmanOccupation,				//! "ְҵ ������"
	eIDS_PlayerHasDel,							//! "%s - ��ɫ��ɾ��"
	eIDS_PlayerHavntOffline,					//! "%s - δ����"

	eIDS_NotifyPlayerOnline,					//! "%s��%s ������"
	eIDS_NotifyPlayerOffline,					//! "%s��%s ������"
	eIDS_NotifyLoverOnline,						//! "[����] %s ������"
	eIDS_NotifyLoverOffline,					//! "[����] %s ������"

	eIDS_NotifyPlayerAddYou				= 201,	//! "%s ������ӵ��� %s"
	eIDS_NotifyPlayerDelYou,					//! "%s ������ %s ��ɾ����"
	eIDS_NotifyYouAddPlayer,					//! "%s �Ѽ��뵽���� %s"
	eIDS_NotifyYouDelPlayer,					//! "%s �Ѵ����� %s �б�ɾ��"
	eIDS_NotifyYouMovePlayer,					//! "%s �Ѵ����� %s ���� %s"
	eIDS_NotifyPlayerHasAddInOtherGroup,		//! "%s �ѱ��������������"
	eIDS_NotifyPlayerHasAddInGroup,				//! "%s �Ѿ������� %s �б�����"
	eIDS_NotifyTheGroupHasMaxPlayers,			//! "���� %s ���������Ѵ�����"
	eIDS_NotifyTheOperatNeedPlayerLogin,		//! "��� %s �Ĳ�����Ҫ���ڱ����������ߺ�������"
	eIDS_NotifyYouHasSendTheRequest,			//! "���Ѿ����͹������ˣ������ĵȴ�������"
	eIDS_NotifyYouHasAutoReject,				//! "���Զ��ܾ��� %s ������Ӻ��ѵ�����"


	eIDS_NotifyPlayerHasHideInfo,				//! "�����Ŀǰ�����ڡ������ߡ�״̬���޷�ִ�к��Ѳ�����"

	eIDS_CantLeavewordForDelPlayer		= 301,	//! "���ܺ��Ѿ�ɾ���Ľ�ɫ ���������"
	eIDS_CantLeavewordForOfflinePlayer,			//! "���ܸ������ߵ� %s ������ԣ�"
	eIDS_AreYouSureLeaveword,					//! "%s �����ߣ����Ƿ�Ҫ���ԣ�"
	eIDS_NotifyYourLeavewordHasSaved,			//! "��� %s �������Ѵ洢"
	eIDS_NotifyYourLeavewordHasOverflow,		//! "��� %s �������ѳ�����������"
	eIDS_PlayerForYourLeavewordHasOverflow,		//! "%s ����������ѳ�����������"
	eIDS_LeavewordTooFast,						//! "���Թ���Ƶ�������Ժ����ԡ�����"
	eIDS_LeavewordFormat,						//! "%d��%d��%d�� %dʱ%d��%d�� => %s ���ԣ�%s"
	eIDS_BlackListFlag,							//! "@������ "

	eIDS_InputAddPlayerName				= 401,	//! "�������������"
	eIDS_NotifyFrendHasDelete           = 402   //! "�Է��Ѿ�����Ӻ����б���ɾ�����޷�����˽��"
};


//! ���壺����ϵͳ �ַ���
enum ENUM_IDS_FACTION
{
	eIDS_FAC_FactionPanel				= 1,	//! "����"
	eIDS_FAC_UnionPanel,						//! "����"

	eIDS_FAC_FacWarInfo					= 10,	//! "ս����Ϣ"
	eIDS_FAC_BeachheadWarInfo,					//! "�ݵ�ս��Ϣ"
	eIDS_FAC_WarList,							//! "��ս����"

	eIDS_FAC_State_Open					= 101,	//! "���ž���"
	eIDS_FAC_State_Close,						//! "�չ�����"
	eIDS_FAC_Notify_CreateSucceed,				//! "�лᴴ���ɹ���"
	eIDS_FAC_Notify_SetChairmanSucceed,			//! "�᳤ְλת�óɹ���"
	eIDS_FAC_Notify_Disband,					//! "�л� %s �Ѿ���ɢ�ˣ�"
	eIDS_FAC_InviteYou,							//! "%s �����������л᣺%s��"
	eIDS_FAC_Notify_MemberHasKickedFromYourFac,	//! "%s �Ѿ����߳����лᣡ"
	eIDS_FAC_PlayerAcceptYourInvite,			//! "%s �Ѿ�����������������룡"
	eIDS_FAC_NotEnoughMoneyUpgradeEquip,		//! "%s �����������л᣺%s��"
	eIDS_FAC_ThePurviewNeedFacLevel,			//! "��Ȩ����Ҫ�л�ȼ��ﵽ %d�� ���ɿ��ţ�"
	eIDS_FAC_Notify_MemberHasKicked,			//! "%s �Ѿ����߳��лᣡ"
	eIDS_FAC_Notify_MemberHasQuit,				//! "%s �Ѿ��˳��лᣡ"
	eIDS_FAC_UploadFacIconSucceed,				//! "�л�ձ� �ϴ��ɹ���"
	eIDS_FAC_SetPlacardSucceed,					//! "�лṫ�� �Ѿ��޸ĳɹ���"
	eIDS_FAC_SetMemoSucceed,					//! "���屸��¼ �Ѿ��޸ĳɹ���"
	eIDS_FAC_SetJobPurviewSucceed,				//! "Ȩ���޸ĳɹ���"
	eIDS_FAC_FunHasTopLevel,					//! "�Ѵﵽ��ߵȼ�"

	eIDS_FAC_InputPlacard				= 201,	//! "�������лṫ�棺"
	eIDS_FAC_InputMemberTitle,					//! "������ %s �ĳƺţ�"
	eIDS_FAC_InputFacName,						//! "�������л����ƣ�"
	eIDS_FAC_InputFacIconFileName,				//! "������ձ�ͼƬ���ļ�����"

	eIDS_FAC_YouHavntJoinTheFac			= 301,	//! "����δ�����лᣡ"
	eIDS_FAC_YouHaveBeenChairman,				//! "���Ѿ�������Ϊ %s ���»᳤��"
	eIDS_FAC_OldMasterSetNewMaster,				//! "%s ������Ϊ %s �����峤��"
	eIDS_FAC_YouHaveBeenKicked,					//! "���Ѿ����߳��лᣡ"
	eIDS_FAC_YouHaveSetMemberJob,				//! "���Ѿ��� %s �Ĺٽ�����Ϊ %s ��"
	eIDS_FAC_YouHaveDeposalMemberJob,			//! "���Ѿ�ȡ���� %s �Ĺٽ�Ȩ�ޣ�"
	eIDS_FAC_YouHaveQuitFac,					//! "���Ѿ��˳��лᣡ"
	eIDS_FAC_YouHaveJoinTheFac,					//! "���Ѿ������л᣺ %s ��"
	eIDS_FAC_YourFacHasDisband,					//! "�����ڵ��л��Ѿ���ɢ��"
	eIDS_FAC_YouHaveSetMemberTitle,				//! "���Ѿ��� %s �ĳƺ�����Ϊ %s ��"
	eIDS_FAC_ChairmanHasSetYourJob,				//! "�᳤�Ѿ������� %s �Ĺٽ� ��"
	eIDS_FAC_ChairmanHasDeposalYourJob,			//! "�᳤�Ѿ�ȡ�������Ĺٽס�"
	eIDS_FAC_YouHasSendYourApply,				//! "�����л��������ύ����ȴ�����������"
	eIDS_FAC_YourFacLevelHasUpgrade,			//! "�������л�ĵȼ��Ѿ����� %d ����"
	eIDS_FAC_YourFacLevelHasDegrade,			//! "�������л�ĵȼ��Ѿ����� %d ����"
	eIDS_FAC_YourFacHasOpenRecruit,				//! "�����ڵ��л��Ѿ� ������ļ��"
	eIDS_FAC_YourFacHasCloseRecruit,			//! "�����ڵ��л��Ѿ� ֹͣ��ļ��"

	eIDS_FAC_AreYouSureDisbandFac		= 401,	//! "��ȷ��Ҫ��ɢ�л���"
	eIDS_FAC_AreYouSureQuitFac,					//! "��ȷ��Ҫ�˳��л���"
	eIDS_FAC_AreYouSureKickMember,				//! "��ȷ��Ҫ�� %s ����л���"
	eIDS_FAC_AreYouSureAbdication,				//! "��ȷ�����᳤λ��ת�ø� %s ��"
	eIDS_FAC_AreYouSureSetJobName,				//! "��ȷ���� %s �޸�Ϊ %s ��"
	eIDS_FAC_AreYouSureAuthorize,				//! "��ȷ����׼ %s ���뱾�л���"
	eIDS_FAC_AreYouSureInvite,					//! "��ȷ��Ҫ���� %s �����л���"
	eIDS_FAC_AreYouSureReject,					//! "��ȷ���ܾ� %s �����������"
	eIDS_FAC_AreYouSureApply,					//! "��ȷ��Ҫ������� %s ��"
	eIDS_FAC_AreYouSureCancelApply,				//! "��ȷ��Ҫȡ������ %s ��������"
	eIDS_FAC_AreYouSureEditPlacard,				//! "��ȷ��Ҫ�޸��лṫ����"
	eIDS_FAC_AreYouSureEditMemo,				//! "��ȷ��Ҫ�޸� ����¼��"

	eIDS_FAC_Chairman					= 501,	//! "�᳤"
	eIDS_FAC_NoJob,								//! "�޹ٽ�"
	eIDS_FAC_MemberNum,							//! "��Ա����%d/%d"
	eIDS_FAC_Recruit,							//! "��ļ��..."
	eIDS_FAC_UnRecruit,							//! "ֹͣ��ļ..."
	eIDS_FAC_WelfareExp,						//! "���帣������"

	eIDS_FAC_KickMember					= 601,	//! "������Ա"
	eIDS_FAC_SetJob,							//! "���ùٽ�"
	eIDS_FAC_SetTitle,							//! "���óƺ�"
	eIDS_FAC_Abdication,						//! "ת�û᳤λ��"
	eIDS_FAC_DeposalJob,						//! "ȡ���ٽ�"
	eIDS_FAC_SecretChat,						//! "����"
	eIDS_FAC_AddFriend,							//! "��Ϊ����"
	eIDS_FAC_InviteTeam,						//! "�������"

	eIDS_FAC_Purview					= 701,	//! "�༭���幫��"

	eIDS_FAC_PlacardText				= 801,	//! "���лṫ�桿��%s"
	eIDS_FAC_Prompt_LessMemNum,					//! "������֪ͨ����������Ա���٣������ %d �첻�� %d �ˣ������彫�ڣ�%d��%d��%d��24:00ʱ��������ɢ��"
	eIDS_FAC_Prompt_MasterLeaveTooLong,			//! "������֪ͨ�����峤�Ѿ� %d ��û�����ߴ���������������� %d �첻���ߣ��峤֮λ���ڣ�%d��%d��%d��24:00ʱ�����Զ����ڱ��幱����ߵ���Ա��"
	eIDS_FAC_Prompt_ResNotEnough,				//! "������֪ͨ����������Դ������ά�ּ��徭Ӫ���������Դ����1��Ϊ��ֵ������ȼ��������������м���Ƽ������͵��ü���ȼ���Ӧ�ȼ���"

	eIDS_FAC_Require_FacLvl				= 901,	//! "[COLOR 5B3200]����ȼ���[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_MasterLvl,					//! "[COLOR 5B3200]�峤�ȼ���[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_FacExp,					//! "[COLOR 5B3200]���徭�飺[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_FacRes,					//! "[COLOR 5B3200]������Դ��[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_Gold,						//! "[COLOR 5B3200]��ң�[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_Prop,						//! "[COLOR 5B3200]���ߣ�[/COLOR][COLOR %s] %d [/COLOR][BR]"
	eIDS_FAC_Require_Beachhead,					//! "[COLOR 5B3200]�ݵ㣺[/COLOR][COLOR %s] ռ�� %d ������ [/COLOR][BR]"

	eIDS_FAC_Upgrade_FunDesc			= 950,	//! "[COLOR 5B3200]����ȼ���[/COLOR][COLOR %s] %d [/COLOR]"

	eIDS_FAC_NumAndOnlineOfMembers		= 1001,	//! "[COLOR FFFFFF]%d[/COLOR][COLOR FFFF00] λ��Ա��[/COLOR][COLOR 00FF00]%d[/COLOR] [COLOR FFFF00]λ[/COLOR][COLOR 00FF00]����[/COLOR][COLOR FFFF00]��[/COLOR]"/>
	eIDS_FAC_NumAndMaxNumOfMembers,				//! "[COLOR FFFFFF]%d [/COLOR][COLOR FFFF00]λ��Ա��[/COLOR][COLOR FFFFFF]���������[/COLOR][COLOR FF0000] %d [/COLOR][COLOR FFFFFF]λ[/COLOR][COLOR FFFF00]��[/COLOR]"

	eIDS_FAC_WarInfoDesc				= 1010,	//! "[B][BR][IMG 1000][/IMG][BR]%s[BR][IMG 1001][/IMG][BR]%s[BR][IMG 1002][/IMG][BR]%s[/B]"
	eIDS_FAC_FacWarInfoDesc,					//! "[B][BR][IMG 1000][/IMG][BR]%s[BR][IMG 1001][/IMG][BR]%s[BR][IMG 1002][/IMG][BR]%s[/B]"
	eIDS_FAC_BeachheadDesc,						//! "[B][BR][IMG 1000][/IMG][BR]%s[BR][IMG 1001][/IMG][BR]%s[BR][IMG 1002][/IMG][BR]%s[/B]"
	eIDS_FAC_WarListDesc,						//! "[B][BR][IMG 1000][/IMG][BR]%s[BR][IMG 1001][/IMG][BR]%s[BR][IMG 1002][/IMG][BR]%s[/B]"

	eIDS_FAC_UnknownError				= 10000,//! "δ֪���󣬲���ʧ�ܣ�"
	eIDS_FAC_CountryError				= 10001,//! "���Ҳ���������ʧ�ܣ�"
	eIDS_FAC_CantCreateWhenYouHasFac,			//! "�����˳�֮ǰ���лᣬ���ܴ����µ��лᣡ"
	eIDS_FAC_FacNameRepeat,						//! "�л������ظ����뻻һ�����֣�"
	eIDS_FAC_YourLevelNotEnough,				//! "���ĵȼ����㣬����ʧ�ܣ�"
	eIDS_FAC_YourMoneyNotEnough,				//! "���Ľ�Ǯ���㣬����ʧ�ܣ�"
	eIDS_FAC_YourGemNotEnough,					//! "��ȱ�ٱ�ʯ������ʧ�ܣ�"
	eIDS_FAC_GoodsError,						//! "��Ʒ���󣬲���ʧ�ܣ�"
	eIDS_FAC_ConfigFileError,					//! "���û�ȡ��δ��ɣ�����ʧ�ܣ�"
	eIDS_FAC_ServerCoreError,					//! "���ķ��������󣬲���ʧ�ܣ�"
	eIDS_FAC_FacLevelNotEnough,					//! "���ȼ�̫�ͣ�����ʧ�ܣ�"
	eIDS_FAC_YouHavntThePurview,				//! "��û�д��л�Ȩ�ޣ�����ʧ�ܣ�"
	eIDS_FAC_CantHandleTheMemberHasSamePurview,	//! "���ܲ�����ͬ��Ȩ�޵Ļ�Ա������ʧ�ܣ�"
	eIDS_FAC_TheOperationNeedPlayerOnline,		//! "�˲�����Ҫ�Է����߲�����ɣ�"
	eIDS_FAC_OperateObjectNotExist,				//! "����Ŀ�겻���ڣ�"
	eIDS_FAC_PlayerHasJoinAntherFac,			//! "�Է��Ѿ����������л��ˣ�"
	eIDS_FAC_FacMaxNumError,					//! "�л���������������"
	eIDS_FAC_PlayerRejectYourInvite,			//! "�Է��ܾ����������룡"
	eIDS_FAC_ApplyRepeat,						//! "�����ظ�������룬����ȡ��֮ǰ��������ڽ��в�����"
	eIDS_FAC_Timeout,							//! "����ʱ���ѵ��������µ��NPC��"
	eIDS_FAC_SensitiveWord,						//! "�����к��������ַ���"
	eIDS_FAC_NotInFaction,						//! "û�м����лᣡ"
	eIDS_FAC_SameValue,							//! "ԭ��������������ͬ��"
};

//! ���壺��Ϸ���� �ַ���
enum ENUM_IDS_GAMECONFIG
{
	eIDS_GAMECFG_AreYouSureLoadDefault	= 1,	//! "��ȷ��Ҫ��ȡĬ�� ��Ϸ������"
	eIDS_GAMECFG_AreYouSureModify,				//! "��ȷ��Ҫ�޸� ��Ϸ������"
};


//! ���壺�ö����� �ַ���
enum ENUM_IDS_TOPMSG
{
	eIDS_TOPMSG_InputYourWords			= 1,	//! "��������Ҫ��������Ϣ"

	eIDS_TOPMSG_NumOfLeavingWords		= 201,	//! "ʣ������"

	eIDS_TOPMSG_TopChatInBox			= 301,	//! "����"

	eIDS_TOPMSG_Err1					= 901,	//! "������Ϣ"

	eIDS_TOPMSG_BEKILLED				= 902,  //! "����Ӫ�����%s�ģ�%d��%d����������ɱ��"

	eIDS_TOPMSG_KILLIED					= 903,  //! "����Ӫ��� %s ��%s��(%d,%d)�����ɹ���ɱ���ˣ���"

	eIDS_TOPMSG_BEATTACKED				= 904,  //! "����Ӫ�����%s�ģ�%d��%d����������ɱ��"

	eIDS_TOPMSG_ATTACKED				= 905,  //! "����Ӫ�������%s��ͼ�� (%d,%d)���������ж���Ӫ��������Ԯ!"

	eIDS_TOPMSG_YOUATTACKED				= 906,  //! "�㱻 %s ������!"
};



// ���ƣ����������Ϣ08.6.14
enum eIDS_CT_STR
{
	eIDS_BASEUI	=	1,						// ����������Ϣ
	eIDS_SKILL,								// ���ܣ�״̬�����
	eIDS_TEAM,								// �������
	eIDS_MAP,								// ��ͼ���
	eIDS_TSSHOP,							// ��ֵ�̵����
	eIDS_CONTROL,							// �������
	eIDS_QUEST,								// �������
	eIDS_POPMENU,							// ̽���˵�
	eIDS_RELIVE,                            // ��������
	eIDS_SPRITE,                            // С����������
	eIDS_SYSTEMSETUP,                       // ϵͳ���ý������
	eIDS_EQUIPWEAR,                         // װ���;öȽ���
	eIDS_TITLEMENU,                         // �ƺŲ˵�
	eIDS_MERCENARY = 15,                    // Ӷ������
	eIDS_INPUTBOX,                          // �ű��������ʾ
	eIDS_MAIL,		                        // �ʼ����
	eIDS_SELECTSECTION,                     // ѡ����Ϸ����
	eIDS_LUVINIA,                           // ���鶯������
	eIDS_GAMENOTE,                          // ��Ϸ����
	eIDS_TIMEBOX,                           // ʱ��Ի���
	eIDS_MEDAL,                             // ѫ�����
	eIDS_TOPMSG,							// �ö��������
	eIDS_PUBLIC = 300,                      // ��������
	eIDS_Enchant = 306,                     // ħ������

	eIDS_PRESENT = 400,						// ��Ϸ������ϵͳ���

	eIDS_MAPKEY = 210,							// ����ӳ���ȼ�ϵͳ

	eIDS_KEYNAME = 211,						// �����������
};

enum eIDS_CT_MAPKEY
{
	eIDS_MAPKEY_TYPE_MOVE = 100,
	eIDS_MAPKEY_FUNC_RUNORWALK, //�߻���
	eIDS_MAPKEY_FUNC_FORWARD,		// ��ǰ
	eIDS_MAPKEY_FUNC_GOBACK,		// ���
	eIDS_MAPKEY_FUNC_GOLEFT,		// ����
	eIDS_MAPKEY_FUNC_GORIGHT,     // ����

	eIDS_MAPKEY_TYPE_INTERFACE = 200,
	eIDS_MAPKEY_FUNC_ESC,			// ȡ���򿪽�����߿���ϵͳ�˵�����
	eIDS_MAPKEY_FUNC_HIDEINTERFACE,	// ���ز�������
	eIDS_MAPKEY_FUNC_TALKFACE,		// ����˽�Ľ���
	eIDS_MAPKEY_FUNC_QUST,			// �����������
	eIDS_MAPKEY_FUNC_SHOWGOODSNAME,	// ��ʾ���������Ʒ������ʾ
	eIDS_MAPKEY_FUNC_FASTREPLY,		// ���ٻظ�
	eIDS_MAPKEY_FUNC_AUTOSUPPLY,		// ����ҩƷ�Զ���������
	eIDS_MAPKEY_FUNC_SKILL,			// �������ܽ���
	eIDS_MAPKEY_FUNC_PRESTIGE,		// ����������������
	eIDS_MAPKEY_FUNC_PACKAGE,		// ��������
	eIDS_MAPKEY_FUNC_FRIEND,			// �������ѽ���
	eIDS_MAPKEY_FUNC_PRODUCE,		// ����רҵ��������
	eIDS_MAPKEY_FUNC_UNDERWORLD,		// ����������
	eIDS_MAPKEY_FUNC_PK,				// ����PK���ý���
	eIDS_MAPKEY_FUNC_HELP,			// ������������
	eIDS_MAPKEY_FUNC_INCREASESHOP,	// ������ֳ�̵�
	eIDS_MAPKEY_FUNC_NPCLIST,		// ����NPC�б�
	eIDS_MAPKEY_FUNC_PLAYERPROPERTY,// �����������Խ���
	eIDS_MAPKEY_FUNC_SERCHFRIEND,	// ����������������
	eIDS_MAPKEY_FUNC_PACKAGE2,		// ������������
	eIDS_MAPKEY_FUNC_BIGMAP,			// �������ͼ

	eIDS_MAPKEY_TYPE_HOTKEY = 300,
	eIDS_MAPKEY_FUNC_HOTKEY_301,	
	eIDS_MAPKEY_FUNC_HOTKEY_302,
	eIDS_MAPKEY_FUNC_HOTKEY_303,
	eIDS_MAPKEY_FUNC_HOTKEY_304,
	eIDS_MAPKEY_FUNC_HOTKEY_305,
	eIDS_MAPKEY_FUNC_HOTKEY_306,
	eIDS_MAPKEY_FUNC_HOTKEY_307,
	eIDS_MAPKEY_FUNC_HOTKEY_308,
	eIDS_MAPKEY_FUNC_HOTKEY_309,
	eIDS_MAPKEY_FUNC_HOTKEY_310,
	eIDS_MAPKEY_FUNC_HOTKEY_311,
	eIDS_MAPKEY_FUNC_HOTKEY_312,
	eIDS_MAPKEY_FUNC_HOTKEY_313,
	eIDS_MAPKEY_FUNC_HOTKEY_314,
	eIDS_MAPKEY_FUNC_HOTKEY_315,
	eIDS_MAPKEY_FUNC_HOTKEY_316,
	eIDS_MAPKEY_FUNC_HOTKEY_317,
	eIDS_MAPKEY_FUNC_HOTKEY_318,
	eIDS_MAPKEY_FUNC_HOTKEY_319,
	eIDS_MAPKEY_FUNC_HOTKEY_320,
	eIDS_MAPKEY_FUNC_HOTKEY_321,
	eIDS_MAPKEY_FUNC_HOTKEY_322,
	eIDS_MAPKEY_FUNC_HOTKEY_323,
	eIDS_MAPKEY_FUNC_HOTKEY_324,

	eIDS_MAPKEY_TYPE_OTHER = 400,
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_LOCKATTACK,		// ����Ŀ�������ͨ����
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_AUTOMOVE,		// �Զ���������ƶ�
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_FITTING,		// �Դ�����
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_AUTOPRINTSCREEN,// �Զ���ͼ
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_CHAT,			// ���������
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_CHANGETARGET,	// �л�����Ŀ��
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_PICKGOODS,		// ʰȡ��Ʒ
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_AFFIRMTARGET,	// ��ʾ��ǰѡ�е�Ŀ�����
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_FIRESPELLSELF,	// �뼼�ܿ�ݼ����ʹ�ö��Լ�ʩ�ŷ���
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_CLOSEDISTANCE,	// �����ӽǾ���
	eIDS_MAPKEY_FUNC_OTHERHOTKEY_FARDISTANCE,	// ��Զ�ӽǾ���

};

enum eIDS_CT_BASEUI
{
	eIDS_WAR_FOMAT = 1,						// ���棺����ID��%d�У�������ͬ���ַ���ID��%d
	eIDS_WAR_UPDATE,						// �Ҳ����������б��ļ���������update.exe�����޸�
	eIDS_WAR_EDITION,						// ��Ϸ�汾����
	eIDS_WAR_CDKEY,							// ��CDKEY�����ڣ�
	eIDS_WAR_ACCOUNT,						// ���˺ű���ֹʹ����%s
	eIDS_WAR_ACCOUNT_FORBIDUSE,				// �Բ��������˺��Ѿ�����ֹʹ�ã�
	eIDS_WAR_CANNOTLOADGAME,				// �Բ��������ڵ������޷���½����Ϸ����������Ϸ��Ӫ����ϵ��
	eIDS_WAR_CDKEY_ERROR,					// CDKEY���������(��ע��Ӣ����ĸ�Ĵ�Сд��)
	eIDS_WAR_ACCOUNT_ONLINE,				// ���ʺ�Ŀǰ���ڵ�½״̬���������ѽ����ʺ�ǿ�ƶ��ߣ����Ժ����ԡ�
	eIDS_WAR_SERV_DATAERROR,				// ���������ݿ�����쳣,���Ժ����ԣ�
	eIDS_WAR_SERV_BUSY,						// ��������æ�����Ժ����ԣ�
	eIDS_WAR_BASEINFO_UNSUCCESSFUL,			// ȡ�û�����Ϣʧ�ܣ�
	eIDS_WAR_DELROLE_SUCCESS,				// ��ɫɾ���ɹ���
	eIDS_WAR_DELROLE_UNSUCCESS,				// ��ɫɾ��ʧ�ܣ�
	eIDS_WAR_DELROLE_UNSUCCESS_CITY,		// �ý�ɫ�İ��ӵ�����ǣ�����ɾ����
	eIDS_WAR_DELROLE_UNSUCCESS_JOINUNION,	// �ý�ɫ�İ�������ͬ�ˡ�����ɾ����
	eIDS_WAR_DELROLE_UNSUCCESS_HAVEUNION,	// �ý�ɫ�İ�ᴴ����ͬ�ˡ�����ɾ����
	eIDS_WAR_RENEWROLE_SUCCESS,				// ��ɫ�ָ��ɹ�
	eIDS_WAR_RENEWROLE_UNSUCCESS,			// ��ɫ�ָ�ʧ��
	eIDS_WAR_ADDROLE_UNSUCCESS_DB,			// ��ӽ�ɫʧ��(���ݿ��������)��
	eIDS_WAR_ADDROLE_UNSUCCESS_BENAME,		// ��ӽ�ɫʧ��(������Ѿ�����)��
	eIDS_WAR_ADDROLE_UNSUCCESS_MAXNUM,		// ��ӽ�ɫʧ��(�ﵽ������������)��
	eIDS_WAR_ADDROLE_UNSUCCESS_NAMERROR,	// ��ӽ�ɫʧ��(��������зǷ��ַ�)��
	eIDS_WAR_ADDROLE_UNSUCCESS_UNKNOWN,		// ��ӽ�ɫʧ��(δ֪�Ĵ�������)��
	eIDS_WAR_CONNECT_UNSUCCESS,				// ������Ϸ������ʧ�ܣ�������...
	eIDS_WAR_AMPLYINFO_UNSUCCESS,			// ȡ����ϸ��Ϣʧ�ܣ�
	eIDS_WAR_ROLE_UNLOAD,					// �ý�ɫû�е�½��
	eIDS_WAR_RECONNECT,						// ��Ŷ~~~~���������Ӳ����ˣ�����һ�°�:)
	eIDS_WAR_QUITGAME_ONFIGHT,				// ս��״̬�������ߣ��Ƿ�ǿ���˳���
	eIDS_WAR_QUITGAME_TOSELECT,				// �˳���Ϸ������ѡ��ҳ�棿
	eIDS_WAR_QUITGAME_TOWINDOWS,			// �˳���Ϸ��Windows���棿
	eIDS_WAR_CREATEROLE,					// ���ȴ���һ����ɫ��
	eIDS_WAR_ISRENEWROLE,					// ��ɫ�Ѿ���ɾ����Ҫ�ָ��ý�ɫ��?
	eIDS_WAR_CANNTCREATEROLE,				// Ŀǰ���ܴ�������Ľ�ɫ�ˣ�
	eIDS_WAR_READYDEL,						// �ý�ɫ�Ѿ���ɾ����
	eIDS_WAR_DEL_DISFAC,					// ��ɫ����7�պ󳹵�ɾ����������ǰ������лὫ������ɢ��
	eIDS_WAR_DEL_AFTERDAY,					// ����%d�챻ɾ��
	eIDS_WAR_PLAYER_FULL,                   // ��������������
	eIDS_WAR_NOT_ACTIVE,                    // �ʺ�δ����
	eIDS_WAR_IS_LOCKED,                     // �û�������
	eIDS_WAR_REJECTED,                      // �˺ű���
	eIDS_WAR_INIT_FINISH,                   // ��֤����û��׼����
    eIDS_WAR_DB_COM_ERROR,                  // ���ݿ�����������
	eIDS_WAR_CONN_DB,                       // �������ӵ�DB
	eIDS_WAR_OPT_FAILED,                    // DB����ʧ��
	eIDS_WAR_DB_FAILED,                     // ���ݿ�ά���У�����ͨ���˺���֤
	eIDS_WAR_QUEUE_SUCCESS,                 // ������Ϸ�ɹ������ڶ�ȡ��ɫ��Ϣ������
	eIDS_WAR_FROZEN_CDKEY,                  // �������Թ����˺Ŷ���
	
 
	eIDS_WAR_DOUBCLICKCOMP = 50,			// ���˫���ؼ� %s\n
	eIDS_WAR_ENTERCOMP,						// ������ؼ� %s\n
	eIDS_WAR_LEAVECOMP,						// ����뿪�ؼ� %s\n
	eIDS_WAR_PRESSCOMP,						// ��갴�¿ؼ� %s\n
	eIDS_WAR_CLICKCOMP,						// ������ؼ� %s\n
	eIDS_WAR_RELEASEDCOMP,					// ����ɿ��ؼ� %s\n
	eIDS_WAR_SETCOMPNULL,					// �����µ����ؼ�ΪNULL\n
	eIDS_WAR_SETCOMP,						// �����µ����ؼ�\n
	eIDS_WAR_SETACTCOMPNULL,				// �����µĻ�ؼ�ΪNULL\n
	eIDS_WAR_SETACTCOMP,					// �����µĻ�ؼ� %s\n

	eIDS_TAB_LOCK,							// ��������ѡȡ�󣬽��Զ��ر�����������ѡȡ
	eIDS_TAB_UNLOCK,						// ��������TAB��ѡȡ��Χ��ֻ�б���ѡ�Ķ���ɱ�TAB��ѡȡ����ȡ�����й�ѡ���ر�TAB����

	eIDS_STR_NEUTRAL	=	100,			// ��������
	eIDS_STR_HONOR_JAZZ_INFO,				// ������ѫ�º;�λ��Ϣ
	eIDS_STR_SPEAK_FASE,					// ��˵��̫����!
	eIDS_STR_COUNCHAT_GOODS_GOLD,			// �ڹ���Ƶ��������Ϣ������%d��[%s]���Ľ�Ǯ:%d
	eIDS_STR_COUNCHAT_UNSUCCESS_GOLD,		// ���н�Ǯ���㣬�޷��������Һ�����
	eIDS_STR_COUNCHAT_UNSUCCESS_GOODS,		// �������[%s]���㣬�޷��������Һ�����
	eIDS_STR_COUNCHAT_GOLD,					// �ڹ���Ƶ��������Ϣ�����Ľ�Ǯ:%d
	eIDS_STR_COUNCHAT_GOODS,				// �ڹ���Ƶ��������Ϣ������%d��[%s]

	eIDS_STR_WORLDCHAT_GOODS_GOLD,			// ������Ƶ��������Ϣ������%d��[%s]���Ľ�Ǯ:%d
	eIDS_STR_WORLDCHAT_UNSUCCESS_GOLD,		// ���н�Ǯ���㣬�޷��������纰����
	eIDS_STR_WORLDCHAT_UNSUCCESS_GOODS,		// �������[%s]���㣬�޷��������纰����
	eIDS_STR_WORLDCHAT_GOLD,				// ������Ƶ��������Ϣ�����Ľ�Ǯ:%d
	eIDS_STR_WORLDCHAT_GOODS,				// ������Ƶ��������Ϣ������%d��[%s]

	eIDS_STR_PRIVATECHAT_TIP,				// /������� ˵������
	eIDS_STR_START_FACEACT,					// %s��ʼ���鶯��%d
	eIDS_STR_INPUT_FINDCODE,				// ��������������


	eIDS_HELP_PRIVATECHAT	=	120,		// [/�Է����� ����...] ָͬ�����˽�ġ�
	eIDS_HELP_REGIONCHAT,					// [!����...] ͬ�������������������
	eIDS_HELP_TEAMCHAT,						// [#����...] ͬһ������������졣
	eIDS_HELP_FACCHAT,						// [%����...] ͬһ�����������졣

	eIDS_STR_WORLDCHAT	=	130,			// [���纰��]%s:%s
	eIDS_STR_WORLDCHAT_UNSUCCESS,			// [���纰��] ʧ�ܣ�
	eIDS_STR_COUNCHAT,						// [���Һ���]%s:%s"
	eIDS_STR_COUNCHAT_UNSUCCESS,			// [���Һ���] ʧ�ܣ�
	eIDS_STR_UNIONCOUNCHAT,					// [�˹�����]%s:%s
	eIDS_STR_UNIONCOUNCHAT_UNSUCCESS,		// [�˹�����] ʧ�ܣ�

	eIDS_STR_SECURTIY_SAFE	=	150,		// [��ȫ��]
	eIDS_STR_ENTER_SAFEAREA,				// ���ѽ���[��ȫ��]
	eIDS_STR_SECURTIY_FIGHT,				// [������]
	eIDS_STR_ENTER_FIGHTAREA,				// ���ѽ���[������]
	eIDS_STR_YOU_LEAVEAREA,					// ���뿪��[��ȫ��]
	eIDS_STR_AUTOMOVETO,					// �Զ��ƶ���:%s(%d,%d)
	eIDS_STR_AUTOMOVETO_FAILED,				// �����ƶ�����λ�ã���������ȷ�����꣡

	eIDS_STR_WHO_PET = 200,					// �ĳ���
	eIDS_STR_WHO_CARRIAGE,					// �Ļ�����

	eIDS_STR_PAGE_ROLE	=	300,			// ��  ɫ
	eIDS_STR_PAGE_REPUTE,					// ��  ��
	eIDS_STR_ENCOURAGINGEXP,				// ;��������%d
	eIDS_STR_LINENUM = 304,                 // %��

	/* *****************************
	����:   ������ַ�����Դ����
	����:zhaohang(kaba)
	��������: 2009/5/27
	�޸���־:
		�޸�����:
		�޸�����:
		�޸���  :
	* *****************************/
	eIDS_CHECKCHEAT_QUESTION = 409,			//�װ�������뾡����⣬�о�ϲ������Ӵ!
	eIDS_CHECKCHEAT_REMAINTIME = 410,		//����ʣ��ʱ�䣺%d�룡
	eIDS_CHECKCHEAT_LOCKACCOUNT = 411,		//���������Ϸʱ���Ѿ����ˣ���ú���Ϣ!
	eIDS_CHECKCHEAT_CUE			= 412,       //����ʣ��ʱ������ɴ��⣡
											//413:����IP��½�˽�ɫ�������ﵽ��󣬽��ܵ�һ��ʱ�䱣��
											//414:����IP��½���˺�����̫�࣬���ܵ�һ��ʱ������
											//415:����IP��������ɫ�Ĵ���̫�࣬���ܵ�һ��ʱ������
	eIDS_CHECKCHEAT_ANSRIGHT    =416,		//���δ�����ȷ
	eIDS_CHECKCHEAT_ANSWRONG    =417,        //���δ������
	eIDS_CHECKCHEAT_OVERTIME    =418,		//���δ��ⳬʱ

	eIDS_IME_HOR_STYLE			= 423,		//�л�������
	eIDS_IME_VER_STYLE			= 424,		//�л������
	eIDS_LIVEPOINT				= 425,		//��������Դ
	eIDS_LIVEPOINTDESP			= 426,
	eIDS_CPPOINT                = 427,      //CPֵ��ʾ��Ϣ
											//428:��ǰ������������ݹ���.
											//429:��[%s]�ĶԻ�
											//430:��[%s]�ĶԻ���¼
	eIDS_PRICHATMSGBOX			= 431,      //��Ϣ����������β鿴
	eIDS_PRICHATNEWMSG          = 432,       //����Ϣ(%d)

	eIDS_WAR_DELROLE_LEVELLESS = 800,				// δ�ﵽɾ����ɫ��Ҫ�ĵȼ�
	eIDS_WAR_DELROLE_DELETING,				// ��ɫɾ����...
	eIDS_WAR_LEVEL,							// ��ɫ�ȼ�
	eIDS_WAR_REGION						// ��ɫ������
};

enum eIDS_CT_SKILL
{
	eIDS_USE_UNSUCCESS_OBJERROR = 1,			// �����ͷ�ʧ�ܣ���ѡ��һ�����ʵ�Ŀ�꣡
	eIDS_USE_UNSUCCESS_OBJNULL,					// ��Ч��Ŀ����󣬸ü����޷��ͷţ�
	eIDS_USE_UNSUCCESS_ARM,						// Ҫ�ͷŸü��ܱ���װ����Ӧ��������
	eIDS_USE_UNSUCCESS_GOODS,					// û�б�Ҫ�ĵ��ߣ��޷�ʹ�ü��ܣ�
	eIDS_USE_UNSUCCESS_HP,						// û���㹻��HP��ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_MP,						// û���㹻��MP��ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_NP,						// û���㹻��ŭֵ��ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_INT_MINDEST,				// ��������Сʩ�����벻�㣬ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_INT_MAXDEST,				// ���������ʩ�����벻�㣬ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_MINDEST,					// ��Сʩ�����벻�㣬ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_MAXDEST,					// ���ʩ�����벻�㣬ʹ�ü���ʧ�ܣ�

	eIDS_USE_UNSUCCESS_STATE,					// ��ǰ״̬���޷�ʹ�øü��ܣ�
	eIDS_USE_UNSUCCESS_YVALUE,					// û���㹻����ֵ��ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_OUCC,					// ��ǰְҵ����ʹ�øü��ܣ�
	eIDS_USE_UNSUCCESS_OUTAREA,					// �������÷�Χ��
	eIDS_USE_UNSUCCESS_NOSKILL,					// û�иü��ܣ�
	eIDS_USE_UNSUCCESS_COOLTIME,				// ������ȴʱ��δ����
	eIDS_USE_UNSUCCESS_BLOCK,					// ���赲�ˣ�
	eIDS_USE_UNSUCCESS_DELSKILL,				// ���ܱ�ȡ���ˣ�

	eIDS_USE_UNSUCCESS_NOOBJ,					// û������Ŀ�꣡
	eIDS_USE_UNSUCCESS_INTTIME,					// ����ʱ��û�꣬ʹ�ü���ʧ�ܣ�
	eIDS_USE_UNSUCCESS_MADE,					// ����ʧ�ܣ�
	eIDS_USE_UNSUCCESS_COLLECTION,				// �ɼ�ʧ�ܣ�
	eIDS_USE_UNSUCCESS_ACTING,					// ����û����ɣ�
	eIDS_USE_UNSUCCESS_MOVEING,					// �ƶ��в���ʹ�ü��ܣ�
	eIDS_USE_UNSUCCESS_CANNOTPHY,				// Ŀǰ����ʹ������ϵ���ܣ�
	eIDS_USE_UNSUCCESS_CANNOTMAGIC,				// Ŀǰ����ʹ��ħ��ϵ���ܣ�
	eIDS_USE_UNSUCCESS_OBJDIED,					// �ü��ܲ��ܶ��������Ķ���ʹ�ã�
	eIDS_ERROR_ATTACK_QUESTOBJ,					// ���ڹ���һ�����������������ɱ����������ʹ���������
	eIDS_ERROR_ATTACK_INGOD,					// Ŀ������������л������ı���״̬��
	eIDS_ERROR_USEGOODS_FORSKILL,				// ʹ����Ʒʩ�ż���ʱ��ʩ��Ŀ��̫Զ�Ĵ�����ʾ
	eIDS_ERROR_BYREGION,						// ��ǰ�������޷�ʹ�øü��ܣ�
	eIDS_ERROR_BYEQHORSE,						// δװ�������޷��������״̬��

	eIDS_STUDY_SKILLTOP	= 48,					// �ڵ�ǰְҵ�£������Ѵﵽ��ߵȼ����޷�����!
	eIDS_STUDY_NEEDLEV,							// �ȼ�
	eIDS_STUDY_OUCC,							// ְҵ������ѧϰ������
	eIDS_STUDY_LEV,								// �ȼ�������ѧϰ������
	eIDS_STUDY_SP,								// SP������ѧϰ������
	eIDS_STUDY_PRESKILL,						// ǰ�ü��ܲ�����ѧϰ������
	eIDS_STUDY_LIMITSKILL,						// ���Ƽ��ܲ�����������
	eIDS_STUDY_GOODS,							// ȱ��ѧϰ�������������Ʒ��
	eIDS_STUDY_UNSUCCESS,						// ���������㣬ѧϰ����ʧ�ܣ�
	eIDS_STUDY_MADESKILL_UNSUCCESS,				// ��������ѧϰʧ�ܣ�
	eIDS_STUDYED,								//   ����ѧ��
	eIDS_STUDY_DIS_CONDITION,					//   ���������㣩
	eIDS_STUDY_NOGOLDS,							// ��Ҳ��㣬ѧϰ����ʧ�ܣ�
	eIDS_STUDY_NOSILVER,						// ���Ҳ��㣬ѧϰ����ʧ�ܣ�
	eIDS_STUDY_FAILED_OCCULV,					// ְҵ�ȼ�������������ѧϰ����ʧ�ܣ�

	eIDS_PAGE_NORMALATTC = 98,					// ��ͨ����
	eIDS_PAGE_SKILL,							// ��  ��
	eIDS_PAGE_STUDYED,							// ϰ  ��
	eIDS_PAGE_CHANGEOCCU,						// ת  ְ


	eIDS_ERROR_CONDITION_NOOBJ = 201,			// �ɼ����󲻴��ڣ�
	eIDS_ERROR_CONDITION_STATE,					// ��ǰ״̬�²��ܲɼ���
	eIDS_ERROR_CONDITION_NOINFO,				// δ֪�Ĳɼ��
	eIDS_ERROR_CONDITION_OCCUEX,				// ���ĸ�ְҵ���ܲɼ����

	eIDS_ERROR_CONDITION_LEV,					// ���Ĳɼ��ȼ�������
	eIDS_ERROR_CONDITION_GOODS,					// ��Ʒ���������㣡
	eIDS_ERROR_CONDITION_PACK,					// ���ı�������~��
	eIDS_ERROR_CONDITION_NOTQUESTOBJ,			// �ⲻ������ɼ��
	eIDS_ERROR_CONDITION_SPECQUEST,				// Ҫ�ɼ��������Ҫִ�����������
	eIDS_ERROR_CONDITION_TYPE,					// δ֪�Ĳɼ����ͣ�

	eIDS_ERROR_MADE_NOOBJ,						// û�����������
	eIDS_ERROR_MADE_OCCUEX,						// ���ĸ�ְҵ�޷���������Ʒ��
	eIDS_ERROR_MADE_NOSKILL,					// ����ûѧ����������Ʒ��
	eIDS_ERROR_MADE_TOOL,						// ��������Ʒ��Ҫ����Ĺ��ߣ�
	eIDS_ERROR_MADE_GOODS,						// ��������Ʒ��Ҫ����Ĳ��ϣ�
	eIDS_ERROR_MADE_NPCDEST,					// ����NPC̫Զ����
	eIDS_ERROR_MADE_PACK,						// ��û���㹻�ı����ռ���������Ʒ����
};

enum eIDS_CT_TEAM
{
	eIDS_TEAM_CAPED	= 1,						// �ö����Ѿ��Ƕӳ���
	eIDS_TEAM_CAPSELF,							// ����Ϊ�˱��Ӷӳ���
	eIDS_TEAM_CAPMEMBER,						// %s��Ϊ�˱��Ӷӳ���
	eIDS_TEAM_OCCULEV,							// ְҵ���ܲ���Ҫ�󣬲�����ӣ�
	eIDS_TEAM_EXPALLOT,							// ���鵱ǰ�ķ��䷽ʽΪ��%d
	eIDS_TEAM_MEMBERLOADEX,						// ����%s����������
	eIDS_TEAM_MEMBERRELOAD,						// ����%s��������

	eIDS_TEAM_SELFLEAV,							// ���뿪�˶���
	eIDS_TEAM_MEMBERLEAV,						// %s�뿪�˶���
	eIDS_TEAM_ON_OFF,							// ��û�д���ӿ��أ��ܾ��˶Է���ҵ��������

	eIDS_TEAM_SELFJOINED,						// �����������飬�Ƿ�ͬ�⣿
	eIDS_TEAM_BEGJOIN,							// %s �����������顣
	eIDS_TEAM_JOIN,								// ������飡
	eIDS_TEAM_OBJNOTJOIN,						// �Է���ʱ��Ը��ӣ�
	eIDS_TEAM_NOJOIN,							// ��Ը�������Ķ��飡
	eIDS_TEAM_WHOSETEAM,						// �Ķ���
	eIDS_TEAM_OPENSHOP,							// ����״̬���޷�����ļ
	eIDS_TEAM_HASTEAM,							// ���Ѿ��ж����ˣ����ܼ�����������
	eIDS_TEAM_MEMINOTHERMAP,					// ���Ѳ��ڵ�ǰ��ͼ��
	eIDS_TEAM_MEMLOST,							// ������������
	eIDS_TEAM_INTHISTEAM,						// ���Ѿ��ڱ�������
	eIDS_TEAM_ISJOINTEAM,						// ��ȷ��Ҫ����ö��飿
	eIDS_TEAM_NOMINATE,							// �����Ա%s�Ƽ�%s������飬���Ƿ�ͬ�⣿
	eIDS_TEAM_INSPOINT,							// �������£��ò�����Ч



	eIDS_TEAMALLOT_GA_FREE = 30,				// ����ʰȡ
	eIDS_TEAMALLOT_GA_QUEUE,					// ˳��ʰȡ
	eIDS_TEAMALLOT_GA_RANDOM,					// ���ʰȡ

	eIDS_TEAM_TooFar = 40,						// Ŀ�����̫Զ���ܸ����ƶ�
};

enum eIDS_CT_MAP
{
	eIDS_MAP_POS1 = 1,							// ��ǰ����   X��%d  Y��%d
	eIDS_MAP_REIONG,							// ��ǰ������%s
	eIDS_MAP_POS2,								// [COLOR 00FF00]%s[br]����: (%d, %d)[/COLOR]
	eIDS_MAP_POS3,								// [COLOR FF0000]����: (%d, %d)[/COLOR]
	eIDS_MAP_POS4,								// ����: (%d, %d)

	eIDS_MAP_NOOCCUPY = 10,						// δռ��
	eIDS_MAP_OUROCCUPY,							// �ҷ�ռ��
	eIDS_MAP_OURWILLOCCUPY,						// �ҷ�Ԥռ��
	eIDS_MAP_ENEMYOCCUPY,						// ���з�ռ��
	eIDS_MAP_ENEMYWILLOCCUPY,					// ���з�Ԥռ��
};

enum eIDS_CT_TSSHOP
{
	eIDS_TSSHOP_YUANBAO = 1,					// ����
	eIDS_TSSHOP_DIANJUAN,						// ���
	eIDS_TSSHOP_BUYINFO,						// ����ȷ������%d��%s����%d��������Ʒ��
	eIDS_TSSHOP_BUYSHOP,						// �̵깺��
	eIDS_TSSHOP_CHONGZHI,						// ��ֵ

	eIDS_TSSHOP_TRADETO,						// ���׸�
	eIDS_TSSHOP_TRADEGIVE,						// ���׵�
	eIDS_TSSHOP_TRADEBU,						// ������
	eIDS_TSSHOP_TRADEYSALE,						// ������

	eIDS_TSSHOP_PAY,							// ����
	eIDS_TSSHOP_OMIT,							// ...
	eIDS_TSSHOP_TRADEYING,						// �ڽ����У����ܴ���ֵ�̵ꡣ
	eIDS_TSSHOP_DIED,							// ����״̬�£��޷�����ֵ�̵�
	eIDS_TSSHOP_WEIMIAN,						// λ�澫��
	eIDS_TSSHOP_NOWEIMIAN,						// ����λ�澫�����㣡
	eIDS_TSSHOP_NODIANJUAN,						// ���ĵ���㣡
	eIDS_TSSHOP_NOYUANBAO,						// �������겻�㣡
	eIDS_TSSHOP_ASKCOMPFULL,					// ��Ҫ��������
	eIDS_TSSHOP_LARCOMPFULL,					// ������������
	eIDS_TSSHOP_XZSHOP,							// �����̵�
	eIDS_TSSHOP_DQSHOP,							// ����̵�
	eIDS_TSSHOP_WMSHOP,							// λ�澫���̵�
	eIDS_TSSHOP_SEARCHTXT,						// �������
	eIDS_TSSHOP_OTHER,							// ����
	eIDS_TSSHOP_SEARCHFAILED,					// �Բ���û���ѵ�������������Ʒ��
	eIDS_TSSHOP_RETURN = 28,					//������ȯ
};

enum eIDS_CT_CONTROL
{
	eIDS_CONTROL_AUTOMOVE	=	1,				// �����Զ��ܲ�״̬,��������ȡ��...
	eIDS_CONTROL_FOLLOWPLAYER,					// �Զ�������� %s ,��������ȡ��...
	eIDS_CONTROL_PASSSKILL,						// �ü���Ϊ�������ܣ��޷�ֱ��ʹ�ã�
	eIDS_CONTROL_NOTUSESKILLEX,					// ��ǰ״̬����ʹ�ø�ְҵ���ܣ�
	eIDS_CONTROL_OBJNULL,						// Ŀ��գ�
	eIDS_CONTROL_MSGSEND,						// ��Ϣ���ͳɹ�����������
	eIDS_CONTROL_NOGOODS,						// ��Ʒ����û���㣡������
	eIDS_CONTROL_NOQUEST,						// ��������û���㣡������
	eIDS_CONTROL_NOQUESTSTEP,					// ����������û���㣡������
	eIDS_CONTROL_NOOCCU,						// ְҵ���������㣡������
	eIDS_CONTROL_BEINGCOLLECTED,				// �ɼ������ڱ��ɼ���
	eIDS_CONTROL_CANTBECOLLECTED,				// �ɼ��ﲻ�ܱ��ɼ���
	eIDS_CONTROL_CHANGETOWALKMODE=13,			// �л�������״̬��
	eIDS_CONTROL_CHANGETORUNMODE=14,			// �л����ܲ�״̬��
	eIDS_CONTROL_CANTTOUCHNPC=15,				// ���ܵ��NPC��
	eIDS_CONTROL_CANTFOLLOWPLAYER=18,			// ��ֵ�̵꿪��ʱ�����ܸ�������ƶ���

	eIDS_MOVETEST_0		=	20,					// (Time:%d)�ƶ�ģʽ����ҵ���x:%d,y:%d.
	eIDS_MOVETEST_1,							// (Time:%d)����˳��ƶ�ģʽx:%d,y:%d.
	eIDS_MOVETEST_2,							// (Time:%d)����ģʽ����ҵ���x:%d,y:%d.
	eIDS_MOVETEST_3,							// (Time:%d)����ģʽ����ҹ���x:%d,y:%d.
	eIDS_MOVETEST_4,							// (Time:%d)����˳�����ģʽx:%d,y:%d.
	eIDS_MOVETEST_5,							// (Time:%d)��ҽ����ƶ�ģʽx:%d,y:%d.
	eIDS_MOVETEST_6,							// (Time:%d)��ҽ��빥��ģʽx:%d,y:%d.
	eIDS_MOVETEST_7,							// (Time:%d)�����ƶ�,MPX:%d,MPY:%d,DestX:%d,DestY:%d
	eIDS_MOVETEST_8,							// (Time:%d)�ƶ�һ��,MPrX:%d,MPY:%d,action:%d,dir:%d,nDis:%d,newx1:%d,newy1:%d,newx2:%d,newy2:%d
	eIDS_MOVETEST_9,							// (Time:%d)�����ͷż�����Ϣ,skillid:%d,MPX:%d,MPY:%d
	eIDS_MOVETEST_10,							// (Time:%d)�����ƶ�,oldx:%d,oldy:%d,newx:%d,newy:%d
	eIDS_MOVETEST_11,							// ��ʼ����(Time:%d)%s����,Px:%d,Py:%d,Mx:%d,My:%d
	eIDS_MOVETEST_12,							// (Time:%d)����ֹͣ�Զ��ƶ�ģʽx:%d,y:%d
	eIDS_MOVETEST_13,							// (Time:%d)����ֹͣ�Զ�����ģʽx:%d,y:%d.
	eIDS_MOVETEST_14,							// (Time:%d)�յ����ܻظ�ret:%d,sendmsgcount:%d,Mx:%d,My:%d,MAction:%d
	eIDS_MOVETEST_15,							// (Time:%d)���ö���%d,x:%d,y:%d.
	eIDS_CONTROL_NOTCOLLECT,					// ����ʱ���ܲɼ���������
	eIDS_CONTROL_MOVEHIGHFREQ=37,				// �ƶ�̫Ƶ��
	eIDS_CONTROL_AUTOMOVETIP=38,				// [COLOR FFFFFF]����Զ��ƶ�����[/COLOR][BR][COLOR FFF600]%s��%d,%d��[BR]%s[/COLOR][BR][COLOR 00FF00]���Ҽ������ťȡ���Զ��ƶ���[/COLOR]

	eIDS_CONTROL_CANTFACEACTION_ON_RIDE	   = 41,		// ����״̬����ʹ����̬������
	eIDS_CONTROL_CANTFACEACTION_IN_WATER   = 42,		// ˮ��״̬����ʹ����̬������
	eIDS_CONTROL_CANTFACEACTION_AT_CURRENT = 43,		// ��ǰ״̬����ʹ����̬������
	eIDS_CONTROL_CANTFACEACTION_AT_FIGHT   = 44,		// ս��״̬����ʹ����̬������
};

enum eIDS_CT_QUEST
{
	eIDS_QUEST_NOTMOVETO = 1,						// �Բ����ڱ��������޷��Զ��ƶ���Ŀ�꣡
	eIDS_QUEST_XMLERROR,							// ���󣺵�ǰ���ò������½�Ϊ%d�������������飡
	eIDS_QUEST_DOON,								// ����ɣ�
	eIDS_QUEST_ADDQUEST,							// ���һ������\n
	eIDS_QUEST_ADDQUESTSHOW,						// ������ʾ�����һ������
	eIDS_QUEST_SHOWQUESTNUM,						// ������ʾ������
	eIDS_QUEST_ADDNEWQUEST,							// ������ʾ������������
	eIDS_QUEST_DELNPCTIP,							// ɾ�����������NPC����ʾ
	eIDS_QUEST_RETESTNPCINFO,						// ���¼�ⳡ��������������NPC����ʾ��Ϣ
	eIDS_QUEST_RIGHTTARGETMAX,						// ������ʾ���ͬʱ��ʾ5����
	eIDS_QUEST_NOTRACE,								// ������û����Ҫ��ʾ������Ŀ�꣡

	eIDS_QUEST_PAGE_QUEST	=	99,					// ��  ��
	eIDS_QUEST_PAGE_SAGA,							// ��  ��
};

enum eIDS_CT_POPMENU
{
	eIDS_POPMENU_JOINTEAM	= 1,					// �������
	eIDS_POPMENU_JOINCAP,							// ��Ϊ�ӳ�
	eIDS_POPMENU_JOINKICK,							// �߳�����
	EIDS_POPMENU_EXCHANGE,							// ��������
	EIDS_POPMENU_FRIEND,							// ��Ϊ����
	EIDS_POPMENU_FOLLOW,							// �����ƶ�
	EIDS_POPMENU_CHAT,								// ˽������
	eIDS_POPMENU_JOINFAC, 							// �������
	EIDS_POPMENU_PLAYERINFO,						// �����Ϣ
	EIDS_POPMENU_COPYNAME,							// ��������
	EIDS_POPMENU_BLACKLIST,							// ���������
	EIDS_POPMENU_ALARM,								// �ٱ����
	EIDS_POPMENU_NOMINATE,							// �Ƽ����
	EIDS_POPMENU_ALARM_USEWG,							// ʹ����Ϸ���
	EIDS_POPMENU_ALARM_WGINFO,							// ���������Ϣ
	EIDS_POPMENU_ALARM_GAMECRACK,						// ������Ϸ©��

	EIDS_POPMENU_FREECOLLECT = 100,					// ����ʰȡ
	EIDS_POPMENU_TACTICCOLLECT,						// ˳��ʰȡ
	EIDS_POPMENU_RANDOMCOLLECT,						// ���ʰȡ
};

enum eIDS_CT_MAIL
{
	eIDS_MAIL_OPENFAILD = 1,						// ����������޷������䣡
	eIDS_MAIL_TYPEERROR = 2,						// Ч���ʼ����ͳ�����������߼�!
	eIDS_MAIL_ADDGOODSFAILD = 3,					// ����Ʒ��֮ǰ�ĸ�����Ʒ���Ͳ�������Ʒ���ʧ�ܣ�
	eIDS_MAIL_READFAILD = 4,						// �Ķ��ʼ�ʧ�ܣ�
	eIDS_MAIL_GOODS_CLICKPAYMENT = 5,				// ���ܻ�ȡ��Ʒ�������������Ű�ť��
	eIDS_MAIL_GOLDS_CLICKPAYMENT = 6,				// ���ܻ�ȡ��ң������������Ű�ť��
	eIDS_MAIL_PAYMENT = 7,							// ����
	eIDS_MAIL_PRESENT = 8,							// ����
	eIDS_MAIL_REQUEST = 9,							// ��ȡ
	eIDS_MAIL_PRESENT_NOGOLD = 10,					// ������Щ��Ʒ��������겻��������ʧ�ܣ�
	eIDS_MAIL_PRESENT_ASSURE = 11,					// ��ȷʵҪ������Щ��Ʒ��
	eIDS_MAIL_REQUEST_ASSURE = 12,					// ��ȷʵҪҪ��Է���Ҫ��Щ��Ʒ��
	eIDS_MAIL_NO_GOLDS = 13,						// ���Ľ�Ҳ��㣬����ʧ�ܣ�
	eIDS_MAIL_ISPAYGOLD = 14,						// �Ƿ�ȷ���öԷ��������ţ�
	eIDS_MAIL_NOTDEL_BYPAYGOLD = 15,				// �������ż��޷�ɾ����
	eIDS_MAIL_ISDELMAIL_HAVEGOODS = 16,				// ��ѡ��Ҫɾ�����ż����к�����Ʒ��������ȷ���Ƿ�ɾ����
	eIDS_MAIL_ISDELMAIL = 17,						// �Ƿ�ȷ��ɾ����ѡ�ż���
	eIDS_MAIL_ISPAYTOACCEPT = 18,					// �Ƿ�ͬ����%d��Ҽ۸���ȡ�ż��еĸ�����
	eIDS_MAIL_NOTPAYTOACCEPT = 19,					// ���Ľ�Ҳ��㣬���ܸ��
	eIDS_MAIL_ISAGREEDPRESENT = 20,					// �Ƿ�ͬ����%d����ļ۸��ʼ��еĸ������͸�%s��
	eIDS_MAIL_UPDATAGOODSLIST = 21,					// ����������Ʒ�Ѿ������޷������ż�������ɾ����
	eIDS_MAIL_CANNOTADDGOODS = 22,					// ����Ʒ������Ϊ�������ͣ�

	eIDS_MAIL_SYSTEM = 23,							// ϵͳ
	eIDS_MAIL_ADDGOODSFAILD_BYMULIT = 24,			// ���и�����Ʒ���ҵ��ż�����Ⱥ����
	eIDS_MAIL_GETNEWMAIL = 25,						// �����µ��ż����ǹ���%s��
	eIDS_MAIL_OPENFAILD_BYSHOPORTRADE = 26,			// ����������������״̬�����ܴ����䣡
	eIDS_MAIL_SENDTOSELF = 27,						// �Բ��𣬲��ܸ��Լ������ʼ���
	eIDS_MAIL_GETGOODSFAILED = 28,					// ȡ������Ʒʧ�ܣ����鱳���Ƿ��п�λ��
	eIDS_MAIL_GROUPSEND = 29,						// ��ȷ��Ҫ����%d����Ⱥ�����ʼ���
	eIDS_MAIL_NEWMAIL = 30,							// �����µ��ʼ�����ע����գ�
	eIDS_MAIL_HAVEGOODSORGOLD = 31,					// ���Ƿ�ȷ�����ʼ��и�����һ���Ʒ��
	eIDS_MAIL_PAYNOGOLDS = 32,						// ����ȷ���븶�����ŵĽ�

	eIDS_MAIL_SEND_ERROR = 50,							// ����ʧ����ʾ��Ϣ
	eIDS_MAIL_SENDING_FAILED_INVALID_POSTAGE = 51,		// ���ʲ���������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_SUBJECT = 52,		// ��������Ƿ��ַ�������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_TEXT = 53,			// ���ݰ����Ƿ��ַ�������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_EXTEXT = 54,		// �������ݰ����Ƿ��ַ�������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_GOLD = 55,			// ��ҳ������ƣ�����ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_GOODS = 56,		// ��Ʒ�������ƣ�����ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_MAXNAME = 57,		// ���ֹ���������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_MAXSUBJECT = 58,	// �������������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_MAXTEXT = 59,		// �������ݹ���������ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_INVALID_NOPLAYER = 60,		// û���ҵ������ˣ�����ʧ�ܣ�
	eIDS_MAIL_SENDING_FAILED_NOPLAYERORTITLE = 61,		// û�������˻����⣬����ʧ�ܣ�


	eIDS_MAIL_CURPAGE = 90,								// �� %d ҳ
	eIDS_MAIL_TIME_DAYS = 91,							// %d ��
	eIDS_MAIL_TIME_HOURS = 92,							// %d Сʱ
	eIDS_MAIL_TIME_MINS = 93,							// %d ����

	eIDS_MAIL_ACCEPTPAGE = 99,							// �ռ���
	eIDS_MAIL_SENDPAGE = 100,							// ������
};

enum eIDS_CT_RELIVE
{
	eIDS_RELIVE_FIRSTPAGE     = 1,                        //���Ѿ���ƣ������
	eIDS_RELIVE_ASKBUYITEM    = 2,                        //ѯ�ʹ��򸴻����
	eIDS_RELIVE_RELIVESKILL   = 3,                        //���ܸ�����
	eIDS_RELIVE_RELIVESKILLEX = 4,                        //���ܸ�����
	eIDS_RELIVE_BUYITEMFAIL   = 5,                        //�������겻��
    eIDS_RELIVE_BUYGOODSNOTE  = 9,                        //���������ʾ
};

enum eIDS_CT_SPRITE
{
	eIDS_SPRITE_PUTDOWNFAIL = 1,                      //������Ʒʧ�� 
    eIDS_SPRITE_WEARING     = 2,                      //��ʣҩ����                               
    eIDS_SPRITE_FAILED_OPEN = 3,                      //���Զ���������ʧ��
	eIDS_SPRITE_FAILED_FIGHTAREA = 4,                 //�Զ�Ѱ�ְ뾶��Χ��Ч
	eIDS_SPRITE_SAVE = 5,                             //�Ѿ��������ݵ�����
	eIDS_SPRITE_FAILED_ASSISTPROP = 6,                //�������Ч������Ʒ
	eIDS_SPRITE_FAILED_NOTMAPSTABLE = 7,              //�˳������ܽ����ȶ�������ֹ���
	eIDS_SPRITE_FAILED_NOTMAPCOMMON = 8,              //�˳������ܽ�����ͨ������ֹ���
	eIDS_SPRITE_FAILED_ASSISTSKILL = 9,               //�������Ч��������
	eIDS_SPRITE_PROTECT_STABLE = 10,                  //��ʾ�Ѿ��������ȶ���������״̬
	eIDS_SPRITE_FAILED_NOSTABLETIME = 11,             //û���㹻���ȶ�����ʱ��
	eIDS_SPRITE_FAILED_ENOUGHMONEY = 12,              //û���㹻������
	eIDS_SPRITE_FAILED_NOCONTROL = 13,                //�������������ܽ����޸�
	eIDS_SPRITE_FAILED_FIGHTSKILL = 14,               //�������Чս������.
	eIDS_SPRITE_FAILED_REPEATFIGHTSKILL = 15,         //���и�ս������
	eIDS_SPRITE_FAILED_REPEATASSISTSKILL = 16,        //���иø�������
	eIDS_SPRITE_FAILED_REPEATASSISTPROP = 17          //���и����͵���
};

enum eIDS_CT_TIMEBOX
{
	eIDS_TIMEBOX_NOTE = 1,                           //ʱ���ı���˵��
};

enum eIDD_CT_EQUIPWEAR
{
	eIDS_EQUIPWEAR_VERYMANGLE    = 1,
	eIDS_EQUIPWEAR_VERYMANGLEEX  = 2,
	eIDS_EQUIPWEAR_MANGLE        = 3,
	eIDS_EQUIP_SUCCESS		     = 4,				// [%s]��װ����
};

enum eIDS_CT_SYSTEMSETUP
{
	eIDS_SYSTEMSETUP_SETDEFAULT = 1,                      //����Ĭ��
    eIDS_SYSTEMSETUP_SCREEN     = 2,                      //ȫ����ʾ 
    eIDS_SYSTEMSETUP_APPLICATION = 3,                     //�Ƿ�Ӧ��
};

enum eIDS_CT_TITLEMENU
{
	eIDS_TITLEMENU_CURTITLE  = 1,                          //��ǰ�ƺ�
	eIDS_TITLEMENU_HIDETITLE = 2,                          //����ʾ�ƺ�
	eIDS_TITLEMENU_KNIGHT    = 3,                          //��λ�ƺ�
    eIDS_TITLEMENU_FACTION   = 4,                          //���ƺ�
    eIDS_TITLEMENU_MEDAL1    = 5,                          //ѫ�³ƺ�1
	eIDS_TITLEMENU_MEDAL2    = 6,                          //ѫ�³ƺ�2
	eIDS_TITLEMENU_MEDAL3    = 7,                          //ѫ�³ƺ�3
};

enum eIDS_CT_SELECTSECTION
{
	eIDS_SELECTSECTION_UNSELECT = 1,                        //û��ѡ����Ϸ����
	eIDS_SELECTSECTION_WIRE     = 2,                        //����
	eIDS_SELECTSECTION_TOIL     = 3,                        //��ͨ 
};

enum eIDS_CT_GAMENOTE
{
	eIDS_GAMENOTE_TEXT = 1,                                //��Ϸ����
};

enum eIDS_CT_MERCENARY
{
	eIDS_MERCENARY_COMPLATE          = 1,                     //�µ�ˢ���Ѿ�����������ˢ������ť
	eIDS_MERCENARY_UNCOMPLATE             = 2,                     //�����´����񷢲���ʱ�仹��
	eIDS_MERCENARY_HAVEITEM   = 3,                           //ӵ��ˢ�µ���
	eIDS_MERCENARY_UNHAVEITEM = 4,                           //û��ˢ�µ���
	eIDS_MERCENARY_Min               = 5,                     //����
	eIDS_MERCENARY_Sec               = 6,                     //��
	eIDS_MERCENARY_ItemNum           = 7,                     //ӵ������
	eIDS_MERCENARY_QuestHave         = 8,                     //ί��ʧЧ
	eIDS_MERCENARY_AcceptQuest       = 9,                     //��������
	eIDS_MERCENARY_DelQuest          = 10,                    //��������
	eIDS_MERCENARY_AcceptTeamQuest   = 11,                    //����Ϊȫ������
	eIDS_MERCENARY_Indiv             = 12,                    //����
	eIDS_MERCENARY_Conti             = 13,                    //����
	eIDS_MERCENARY_Accept            = 14,                    //ί�н���
	eIDS_MERCENARY_Complate          = 15,                    //�������
    eIDS_MERCENARY_UnComplated       = 16,                    //δ���
	eIDS_MERCENARY_Complated         = 17,                    //�����
	eIDS_MERCENARY_Shared            = 18,                    //���Ƽ�
};

enum eIDS_CT_INPUTBOX 
{
	eIDS_INPUTBOX_NUMBER = 1,                                 //������һ������
	eIDS_INPUTBOX_NULL   = 2,                                 //���벻��Ϊ��
};

enum eNOTIFY_POS
{
	eNOTIFYPOS_LEFT_BOTTOM=1,//����
	eNOTIFYPOS_RIGHT_BOTTOM,//����
	eNOTIFYPOS_CENTER,//�м�
	eNOTIFYPOS_SYSTEM,//ϵͳ
};

// ��Ϸ������ϵͳ
enum ePRESENT
{
	ePRESENT_DATE = 1,					// ����ʱ�䣺%04d��%02d��%02d��
	ePRESENT_HAVEPRESENT,				// ����δ��ȡ�Ľ�������ע����ȡ!
	ePRESENT_UNCHECKED,					// ��û��ѡ���κν���!
	ePRESENT_NOSPACE,					// ���ı����ռ䲻�㣬�����ٿճ� 1 �񱳰��ռ�!
	ePRESENT_FROZE,						// �ý����ѱ����ᣬ���ڴ����У����Ժ�����!
	ePRESENT_DREW,						// �ý����ѱ���ȡ����������!
	ePRESENT_OVERDUE,					// �ý����ѹ��ڣ���������ȡ!
	ePRESENT_UNSUCCESS,					// ��ȡʧ��!
	ePRESENT_SUCCESS,					// ��ȡ�ɹ�!
};

enum eReportInfo
{
	eTipInfo1=1,//��ʾ��Ϣ1
	eTipInfo11=11,

	eTipOther=31,
};