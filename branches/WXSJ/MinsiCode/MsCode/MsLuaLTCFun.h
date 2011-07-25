#pragma once              //��  //��
#ifndef __LUALTCFUN_H__   //��  //��
#define __LUALTCFUN_H__   //��  //��
//����������������������������  //��
//�ļ����ƣ�LuaLTCFun.h         //��
//����������Lua��C��������      //��
//ƽ̨��أ�Windows             //��
//����������������������������������

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// LUA����ֵ����
#define LUA_RET_ERROR   -1

// LTC����ע������������
#define LTC_FUN_REG_ARRAY_MAX_COUTN 4096

// LTC����
// LUA�������÷�������ʱ�Ĵ���������
#define LUA_ERROR_DISPOSE_FUN_NAME "LTCErrorMsgDispose"

// ��ʼ��LUA�е�LTC����
BOOL InitLuaLTCFun(VOID* Parameter);

// LUA�����е�LTC����������

// Lua��������
INT LUACALL LuaErrorMsgDispose(lua_State *L);

// ȫ��LTC����ע������
extern luaL_Reg g_RegLTCArray[LTC_FUN_REG_ARRAY_MAX_COUTN]; 

//����������������������
#endif//#pragma once//��
//����������������������