#include "stdafx.h"
#include "MapScriptSystem.h"

#include "../GameTest/resource.h"		// ������
#include "..\GameTest\GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// 
// int  baseobject_open(lua_State* L);
// int  shape_open(lua_State* L);
// int  moveshape_open(lua_State* L);
// int  player_open(lua_State* L);
// int  message_open(lua_State* L);
// int  tools_open(lua_State* L);
// int  game_client_open(lua_State* L);
// 
// //int quest_open(lua_State*L); 
// //int quest_attribute_open(lua_State*L);
// //int quest_step_open(lua_State* L);
// //int quest_system_open(lua_State* L);
// int questcontent_open(lua_State* L);
// 
// int  cdkey_open(lua_State* L);
// int  client_region_open(lua_State* L);
// int  script_map_sys_open(lua_State* L);
// 
// int  dialog_open(lua_State* L);
// int  display_info_open(lua_State* L);
// int  game_client_tree_open(lua_State* L);
// int  test_script_dlg_open(lua_State* L);
// 
// int guID_open(lua_State* L);
// int goods_open(lua_State* L);
// int goodslist_open(lua_State* L);
// int monster_open(lua_State* L);
// 
// //��ʼ����̬��ʵ��
// CMapScriptSystem *CMapScriptSystem::m_MapSYSInstance = NULL;		
// 
// CMapScriptSystem::CMapScriptSystem()
// {
// 	m_lMapLuaState = NULL;
// 	m_pDB = NULL;
// 	m_nDBSize = 0;
// 	m_pTestDialog= NULL;
// }
// 
// CMapScriptSystem::~CMapScriptSystem()
// {
// 
// }
// 
// ////��������ȡMapFile�ű�״̬��ʵ��
// CMapScriptSystem *CMapScriptSystem::getInstance()
// {
// 	if(NULL == m_MapSYSInstance)
// 		m_MapSYSInstance= new CMapScriptSystem;
// 	return m_MapSYSInstance;
// }
// 
// //�ͷ���Դ
// bool CMapScriptSystem::Release()
// {
// 	getInstance()->UnInitial();
// 	SAFE_DELETE(m_MapSYSInstance);
// 	return true;
// }
// 
// //��ʼ��
// bool CMapScriptSystem::Init(CGameTestDlg *pTestDialog, const char *strScriptFile/* =NULL */)
// {
// 	m_pTestDialog  = pTestDialog;
// 	m_lMapLuaState = lua_open();
// 	
// 	if(NULL == m_lMapLuaState)
// 	{
// 		PutDebugString("�ں���CMapScriptSystem::Init()��,lua_open()����ʧ�ܣ�");
// 		return false;
// 	}
// 	//���������Ѽ�����
// 	int nRet = lua_gc(m_lMapLuaState, LUA_GCSETPAUSE, 000);
// 	nRet	 = lua_gc(m_lMapLuaState, LUA_GCSETSTEPMUL, 400);
// 
// 	luaL_openlibs(m_lMapLuaState);
// 
// 	baseobject_open(m_lMapLuaState);
// 	shape_open(m_lMapLuaState);
// 	moveshape_open(m_lMapLuaState);
// 	player_open(m_lMapLuaState);
// 	message_open(m_lMapLuaState);
// 	tools_open(m_lMapLuaState);
// 	game_client_open(m_lMapLuaState);
// 	client_region_open(m_lMapLuaState);
// 	script_map_sys_open(m_lMapLuaState);
// 	guID_open(m_lMapLuaState);
// 	goods_open(m_lMapLuaState);
// 	goodslist_open(m_lMapLuaState);
// 	monster_open(m_lMapLuaState);
// 	cdkey_open(m_lMapLuaState);
// 
// 	//quest_open(m_lMapLuaState);
// 	//quest_attribute_open(m_lMapLuaState);
// 	//quest_step_open(m_lMapLuaState);
// 	//quest_system_open(m_lMapLuaState);
// 	questcontent_open(m_lMapLuaState);
// 
// 	//�ؼ���
// 	dialog_open(m_lMapLuaState);
// 	//display_info_open(m_lMapLuaState);
// 	//game_client_tree_open(m_lMapLuaState);
// 	//test_script_dlg_open(m_lMapLuaState);
// 
// 	if(strScriptFile)
// 	{		
// 		int nRet = luaL_dofile(m_lMapLuaState, strScriptFile);
// 		if( nRet != 0 )
// 		{
// 			char strInfo[102400]="";
// 			sprintf(strInfo,"error in function <CScriptSys::Init>(ErroNum:%d,info:%s)",
// 				nRet,tolua_tostring(m_lMapLuaState, -1, 0));
// 			PutDebugString(strInfo);
// 		}
// 	}
// 	return true;
// }
// 
// //����Lua_state
// bool CMapScriptSystem::UnInitial()
// {
// 	lua_close(m_lMapLuaState);
// 	if(m_nDBSize>0)
// 	{
// 		SAFE_DELETE_ARRAY(m_pDB);
// 		m_nDBSize = 0;
// 	}
// 	return true;
// }
// 
// //�ռ��ڴ�����
// int CMapScriptSystem::LuaGC(int nGcType)
// {
// 	int nCount = lua_gc(m_lMapLuaState, LUA_GCCOUNT, 0);
// 	lua_gc(m_lMapLuaState, LUA_GCCOLLECT, 0);
// 	return 1;
// }
// 
// //��������ȡ�ű���������
// BYTE *CMapScriptSystem::GetDB(int minsize)
// {
// 	if(minsize > m_nDBSize)
// 	{
// 		SAFE_DELETE_ARRAY(m_pDB);
// 		m_nDBSize = minsize;
// 		m_pDB	  = new BYTE[m_nDBSize];
// 	}
// 	return m_pDB;
// }
// 
// //
// bool CMapScriptSystem::LoadLuaRequireFile(const char* fileName)
// {
// 	int nRet = luaL_dofile(m_lMapLuaState, fileName);
// 	if( nRet != 0 )
// 	{
// 		char strInfo[102400]="";
// 		sprintf(strInfo,"error in function <CScriptSys::LoadLuaRequireFile>(ErroNum:%d,info:%s)",
// 			nRet,tolua_tostring(m_lMapLuaState, -1, 0));
// 		PutDebugString(strInfo);
// 		m_pTestDialog->OutputScriptInfo(strInfo);
// 		return false;
// 	}
// 	return true;
// }
// 
// //
// void CMapScriptSystem::StartTest(const char *strFielName)
// {
// 	if(strFielName)
// 	{		
// 		int nRet = luaL_dofile(m_lMapLuaState, strFielName);
// 		if( nRet != 0 )
// 		{
// 			char strInfo[102400]="";
// 			sprintf(strInfo,"error in function <CScriptSys::StartTest>(ErroNum:%d,info:%s)",
// 				nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 			PutDebugString(strInfo);
// 			m_pTestDialog->OutputScriptInfo(strInfo);
// 		}
// 	}
// 	return;
// }
// 
// //
// //ִ��ĳ���ַ��� -->����ָLua�ű��ļ��е�ĳ�����庯���� �������
// void CMapScriptSystem::PerformString(const char *strContent)
// {
// 	if(NULL == strContent) 
// 		return;
// 	int nRet = 0;
// 	
// 	if ( strContent )
// 	{
// 		nRet = luaL_dostring(m_lMapLuaState, strContent);
// 	}
// 	
// 	if( nRet != 0 )
// 	{
// 		char strInfo[102400]="";
// 		sprintf(strInfo,"error in function <CMapScriptSystem::PerformString>(ErroNum:%d,info:%s)",
// 			nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 		PutDebugString(strInfo);
// 		m_pTestDialog->OutputScriptInfo(strInfo);
// 	}
// }
// 
// //
// //����test
// void CMapScriptSystem::EndTest(const char *strFielName)
// {
// 	if(strFielName)
// 	{		
// 		int nRet = luaL_dofile(m_lMapLuaState, strFielName);
// 		if( nRet != 0 )
// 		{
// 			char strInfo[102400]="";
// 			sprintf(strInfo,"error in function <CMapScriptSystem::EndTest>(ErroNum:%d,info:%s)",
// 				nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 			PutDebugString(strInfo);
// 			m_pTestDialog->OutputScriptInfo(strInfo);
// 		}
// 	}
// 	return;
// }
// 
// //
// //��ʱ��Ӧ
// long CMapScriptSystem::OnTimeOut(DWORD timerid, DWORD curtime, const void *var)
// {
// 	tagMapScriptTimer* pScrTimer = (tagMapScriptTimer*)var;
// 	lua_pushvalue(m_lMapLuaState,  LUA_GLOBALSINDEX);
// 	lua_pushstring(m_lMapLuaState, "OnTimeOut");
// 	lua_rawget(m_lMapLuaState,	   -2);
// 	lua_pushnumber(m_lMapLuaState, timerid);
// 	lua_pushnumber(m_lMapLuaState, pScrTimer->arg);
// 	int nRet = lua_pcall(m_lMapLuaState, 2, 1, 0);
// 	if(nRet != 0)
// 	{
// 		char strInfo[512]="";
// 		sprintf(strInfo,"error in function <CMapScriptSystem::OnTimeOut>(ErroNum:%d,info:%s)",
// 			nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 		PutDebugString(strInfo);
// 		m_pTestDialog->OutputScriptInfo(strInfo);
// 		lua_pop(m_lMapLuaState, 1);
// 		return 0;
// 	}
// 	nRet = (int)tolua_tonumber(m_lMapLuaState, -1, 0);
// 	lua_pop(m_lMapLuaState,1);
// 	return nRet;
// }
// //
// //����ʱ����ȡ���ĵ���
// void CMapScriptSystem::OnTimerCancel(DWORD timerid, const void *var)
// {
// 	tagMapScriptTimer* pScrTimer = (tagMapScriptTimer*)var;
// 	SAFE_DELETE(pScrTimer);
// }
// 
// //
// //����һ����ʱ��
// long CMapScriptSystem::Schedule(unsigned long arg, unsigned long futuretime, unsigned long interval)
// {
// 	futuretime += timeGetTime();
// 	tagMapScriptTimer* pScrTimer = new tagMapScriptTimer;
// 	pScrTimer->arg = arg;
// 	long lTiemrID = m_pTestDialog->GetTimer()->Schedule(this,(void*)pScrTimer, futuretime, interval);
// 	//����д���
// 	if(-1 == lTiemrID)
// 		delete pScrTimer;
// 	return lTiemrID;
// }
// 
// //
// //ȡ��һ����ʱ��
// int CMapScriptSystem::Cancel(long timerid)
// {
// 	int nRet = m_pTestDialog->GetTimer()->Cancel(timerid);	
// 	return nRet;
// }
// 
// //
// //ִ�нű�
// void CMapScriptSystem::RunScript(string ScriptName)
// {}
// 
// //
// long CMapScriptSystem::SendInfo(string strCDkey, CMessage *msg)
// {
// 	lua_pushvalue(m_lMapLuaState,LUA_GLOBALSINDEX);
// 	lua_pushstring(m_lMapLuaState, "SendInfo");
// 	lua_rawget(m_lMapLuaState,-2);
// 	lua_pushstring(m_lMapLuaState, strCDkey.c_str());
// 	tolua_pushusertype(m_lMapLuaState,(void*)msg,"CMessage");
// 	int nRet = lua_pcall(m_lMapLuaState,2,1,0);
// 	if(nRet != 0)
// 	{
// 		char strInfo[512]="";
// 		sprintf(strInfo,"error in function <SendInfo>(ErroNum:%d,info:%s)",nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 		PutDebugString(strInfo);
// 		lua_pop(m_lMapLuaState,2);
// 	}
// 	lua_pop(m_lMapLuaState, 1);
// 	return nRet;
// }
// 
// //
// long CMapScriptSystem::AcceptInfo(string strCDkey, CMessage *msg)
// {
// 	lua_pushvalue(m_lMapLuaState,LUA_GLOBALSINDEX);
// 	lua_pushstring(m_lMapLuaState, "AcceptInfo");
// 	lua_rawget(m_lMapLuaState,-2);
// 	lua_pushstring(m_lMapLuaState, strCDkey.c_str());
// 	tolua_pushusertype(m_lMapLuaState,(void*)msg,"CMessage");
// 	int nRet = lua_pcall(m_lMapLuaState,2,1,0);
// 	if(nRet != 0)
// 	{
// 		char strInfo[512]="";
// 		sprintf(strInfo,"error in function <AcceptInfo>(ErroNum:%d,info:%s)",nRet,tolua_tostring(m_lMapLuaState,-1,0));
// 		PutDebugString(strInfo);
// 		lua_pop(m_lMapLuaState, 2);
// 	}
// 	lua_pop(m_lMapLuaState,1);
// 	return nRet;
// }
// 
// //
// //����ָ����lua�ļ�
// bool CMapScriptSystem::LoadOneScriptFile(const char* strFileName)
// {
// 	int nRet = luaL_dofile(m_lMapLuaState, strFileName);
// 	if( nRet != 0 )
// 	{
// 		char strInfo[102400]="";
// 		sprintf(strInfo,"error in function <CScriptSys::LoadOneScriptFile>(ErroNum:%d,info:%s)",
// 			nRet,tolua_tostring(m_lMapLuaState, -1, 0));
// 		PutDebugString(strInfo);
// 		m_pTestDialog->OutputScriptInfo(strInfo);
// 		return false;
// 	}
// 	
// 	return true;
// }
// 
// //��̬����һ�����ļ�
// bool CMapScriptSystem::AddScriptFile(const char *strScriptFileName)
// {
// 	int nRet = luaL_dofile(m_lMapLuaState, strScriptFileName);
// 	char strInfo[102400]="";
// 	if( nRet != 0 )
// 	{
// 		sprintf(strInfo,"<�﷨������>(ErroNum:%d,info:%s)",
// 			nRet,lua_tostring(m_lMapLuaState, -1));
// 		MessageBox(NULL,strInfo,"��̬���ؽű��ļ�ʧ��",MB_OK);
// 		return false;
// 	}
// 	return true;
// }
// 
// //��̬����һ����ʱ�ű�
// bool CMapScriptSystem::AddScriptString(const char *strString)
// {
// 	int nRet = luaL_dofile(m_lMapLuaState,strString);
// 	char strInfo[102400]="";
// 	if( nRet != 0 )
// 	{
// 		sprintf(strInfo,"<�﷨������>(ErroNum:%d,info:%s)",
// 			nRet,lua_tostring(m_lMapLuaState,-1));
// 		MessageBox(NULL,strInfo,"��̬������ʱ�ű��ļ�ʧ��",MB_OK);
// 		return false;
// 	}
// 	return true;
// }
// 
// 
// //Bind for LuaScript
// //�ű�����
// int MapFileScheduleTimer(lua_State* L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(!tolua_isstring(L,1,0,&err) ||				//CDkey
// 		!tolua_isnumber(L,2,0,&err) ||				//
// 		!tolua_isnumber(L,3,0,&err) ||				//ʱ����
// 		!tolua_isnoobj(L,4,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileScheduleTimer'.",&err);
// 		return 0;
// 	}
// #endif
// 	long arg		 = tolua_tonumber(L,1,0);
// 	long furturetime = tolua_tonumber(L,2,0);
// 	long interval	 = tolua_tonumber(L,3,0);
// 	long lTimerID	 = CMapScriptSystem::getInstance()->Schedule(arg, furturetime, interval);
// 	tolua_pushnumber(L, (lua_Number)lTimerID);
// 	return 1;
// }
// 
// //
// int MapFileCancelTimer(lua_State* L)
// {	
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(!tolua_isnumber(L,1,0,&err) ||
// 		!tolua_isnoobj(L,2,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileCancelTimer'.",&err);
// 		return 0;
// 	}
// #endif
// 	long lTimerID = tolua_tonumber(L,1,0);
// 	long nRet     = CMapScriptSystem::getInstance()->Cancel(lTimerID);
// 	tolua_pushnumber(L, (lua_Number)nRet);
// 	return 1;
// }
// 
// //
// int MapFileGetDB(lua_State* L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(	!tolua_isnumber(L,1,0,&err) ||
// 		!tolua_isnoobj(L,2,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileGetDB'.",&err);
// 		return 0;
// 	}
// #endif
// 	long num  = (long)tolua_tonumber(L, 1, 0);
// 	BYTE* pDB = CMapScriptSystem::getInstance()->GetDB(num);
// 	tolua_pushuserdata(L, (void*)pDB);
// 	return 1;
// }
// 
// //
// int MapFileGet_Read_DB(lua_State*L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(	!tolua_isnoobj(L, 1, &err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileGet_Read_DB'.",&err);
// 		return 0;
// 	}
// #endif
// 	DBReadSet readDB;
// 	return 0;
// }
// 
// //
// int MapFileGetDialog(lua_State* L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(	!tolua_isnoobj(L,1,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileGetDialog'.",&err);
// 		return 0;
// 	}
// #endif
// 	CGameTestDlg *dialog = (CGameTestDlg*)CMapScriptSystem::getInstance()->GetTestDialog();
// 	tolua_pushusertype(L, (void*)dialog, "CGameTestDlg");
// 	return 1;
// }
// 
// //
// int MapFileSendMessage(lua_State* L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(	!tolua_isnumber(L,1,0,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileGetDialog'.",&err);
// 		return 0;
// 	}
// #endif
// 	UINT msg = (UINT)tolua_tonumber(L,1,0);
// 	DWORD wParam = (DWORD)tolua_touserdata(L,2,0);
// 	DWORD lParam = (DWORD)tolua_touserdata(L,3,0);
// 	::SendMessage(CMapScriptSystem::getInstance()->GetTestDialog()->m_hWnd,msg,
// 		wParam,lParam);
// 	return 0;
// }
// 
// //
// int MapFilePostMessage(lua_State* L)
// {
// #ifdef _DEBUG
// 	tolua_Error err;
// 	if(	!tolua_isnumber(L,1,0,&err) )
// 	{
// 		tolua_error(L,"#ferror in function 'MapFileGetDialog'.",&err);
// 		return 0;
// 	}
// #endif
// 	UINT msg = (UINT)tolua_tonumber(L,1,0);
// 	DWORD wParam = (DWORD)tolua_tonumber(L,2,0);
// 	DWORD lParam = (DWORD)tolua_tonumber(L,3,0);
// 	::PostMessage(CMapScriptSystem::getInstance()->GetTestDialog()->m_hWnd,msg,
// 		wParam,lParam);
// 	return 0;
// }
// void script_map_sys_types (lua_State* L)
// {
// 	tolua_usertype(L,"CGameTestDlg");
// 	tolua_usertype(L,"CMapScriptSystem");
// }
// //
// int  script_map_sys_open(lua_State* L)
// {
// 	tolua_open(L);
// 	script_map_sys_types(L);
// 	tolua_module(L,NULL,0);
// 	tolua_beginmodule(L,NULL);
// 	{
// 		tolua_module(L,"Game",0);
// 		tolua_beginmodule(L,"Game");
// 		{
// 			//ע�ắ��
// 			//					Lua������		C++������
// 			tolua_function(L,"mapfile_schedule_timer",	MapFileScheduleTimer);
// 			tolua_function(L,"mapfile_cancel_timer",	MapFileCancelTimer);
// 			tolua_function(L,"mapfile_get_db",			MapFileGetDB);
// 			tolua_function(L,"mapfile_get_db_read",		MapFileGet_Read_DB);		//get_db_read
// 			tolua_function(L,"mapfile_get_dialog",		MapFileGetDialog);
// 			tolua_function(L,"mapfile_send_message",	MapFileSendMessage);
// 			tolua_function(L,"mapfile_post_message",	MapFilePostMessage);
// 
// 			tolua_constant(L,"TYPE_PLAYER",		TYPE_PLAYER);
// 			tolua_constant(L,"TYPE_GOODS",		TYPE_GOODS);
// 			tolua_constant(L,"TYPE_REGION",		TYPE_REGION);
// 			tolua_constant(L,"TYPE_MONSTER",	TYPE_MONSTER);
// 			tolua_constant(L,"TYPE_SESSION",	TYPE_SESSION);
// 			tolua_constant(L,"TYPE_PLUG",		TYPE_PLUG);
// 
// 
// 			//for DriveClient ���浱ǰ״̬
// 			//player��Ľ���enmu
// 			tolua_constant(L,"UI_PAGE_CHARACTER",		CPlayer::UI_PAGE_CHARACTER);
// 			tolua_constant(L,"UI_PAGE_SKILL",			CPlayer::UI_PAGE_SKILL);
// 			tolua_constant(L,"UI_PAGE_DEPOT",			CPlayer::UI_PAGE_DEPOT);
// 			tolua_constant(L,"UI_PAGE_MAIL",			CPlayer::UI_PAGE_MAIL);
// 			tolua_constant(L,"UI_PAGE_FRIEND",			CPlayer::UI_PAGE_FRIEND);
// 			tolua_constant(L,"UI_PAGE_TEAM",			CPlayer::UI_PAGE_TEAM);
// 			tolua_constant(L,"UI_PAGE_ITEM",			CPlayer::UI_PAGE_ITEM);
// 			tolua_constant(L,"UI_PAGE_SUBBACK",			CPlayer::UI_PAGE_SUBBACK);
// 			tolua_constant(L,"UI_PAGE_MAX_NUM",			CPlayer::UI_PAGE_MAX_NUM);
// 			
// 			//���̲���
// 			tolua_constant(L,"VK_SPACE",			    VK_SPACE);
// 		}
// 		tolua_endmodule(L);
// 	}
// 	tolua_endmodule(L);
// 	return 1;
// }
