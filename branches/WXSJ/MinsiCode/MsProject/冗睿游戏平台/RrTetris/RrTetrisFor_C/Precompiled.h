//����������������������������������
#pragma once                    //��
#include "targetver.h"          //��
#include "Resource.h"           //��
#   define I_KNOW_THE_BASE_TYPE //��
#   define USE_COMPILER_MESSAGE //��
#   define USE_MS_ASSERTX_FULL  //��
#   define USE_MS_INI           //��
#   define USE_MS_LOG           //��
#   define USE_MS_GDI_PLUS_IMAGE//��
#   define USE_MS_WINDOW        //��
#   define USE_MS_SOUND         //��
#   define USE_MS_LUA           //��
#   define USE_MS_DX            //��
#   define USE_MS_2DENGINE      //��
#   define USE_MS_NET           //��
#   define DLL_EXPORTS          //��
#include "MsBaseDef.h"          //��
//����������������������������������
#include "MsRrGameDef.h"
#include "RrTetrisPacketDef.h"
#include "RrTetris_C.h"       // �����

#include "TetrisScene.h"    // ������
#include "TetrisShapeFile.h"// ������
#include "TetrisSquare.h"   // ������
#include "TetrisRender.h"   // ��Ⱦ��
#include "TetrisGame.h"     // ��Ϸ��