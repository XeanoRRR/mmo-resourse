/**
 * @file StringMan.h
 * һ��ʹ�ö��ֽ��ַ����Ͷ��߳� (MT(d))
 * ��ʹ��Public��Ŀʱ, �Ƽ��������ģ��. ��� StringMan.cpp
 * @author Fox (xiangyunfeng@svn.com)
 * @date Mar.22, 2010
 */
   
#pragma once

namespace AppFrame
{

/**
 * ��ʼ���ļ�����
 */

void FileOpInit(const char* filter);
uint FileOpenDlg(char* files);
}; // end AppFrame
