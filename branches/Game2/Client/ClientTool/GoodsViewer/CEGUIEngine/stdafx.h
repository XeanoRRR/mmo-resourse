// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "../../../Engine/Singleton.h"
#include <CEGUI.h>
#include <RendererModules\Direct3D9\CEGUIDirect3D9Renderer.h>
#include <ScriptingModules\LuaScriptModule\CEGUILua.h>
using namespace CEGUI;


