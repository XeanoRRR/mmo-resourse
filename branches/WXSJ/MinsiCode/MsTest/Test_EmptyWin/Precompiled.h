#pragma once

//������������������������������������������������������
#define I_KNOW_THE_BASE_TYPE                        //��
#define CUSTOM_LOG_INFO                             //��
#define NO_MS_LUA                                   //��
#include "MsBaseDef.h"                              //��
//������������������������������������������������������
enum LOG_FILE                                       //��
{                                                   //��
    LOG_FILE_MAIN_LOG = 0,  // ����־�ļ�           //��
    LOG_FILE_MAX,           // �����־�ļ�         //��
};                                                  //��
extern LOG_FILE_INFO g_LogFileInfo[LOG_FILE_MAX];   //��
//������������������������������������������������������