#pragma once                //��//��
#ifndef __S2C_FILEINFO_H__  //��//��
#define __S2C_FILEINFO_H__  //��//��
#pragma pack(push, 1)       //��//��
//������������������������������//��
//�ļ����ƣ�S2C_FileInfo.h      //��
//�����������ļ���Ϣ              //��
//ƽ̨��أ�Windows             //��
//����������������������������������
// S2C�ļ���Ϣ
class S2C_FileInfo; // Ϊ��VA����������
BUILD_PACKET_BEGIN(S2C_FileInfo, S2C_PACKET_FILE_INFO) // �˴���ֹ�ֺ�
struct{ // �����Ч����
    DWORD dwFileSize;
    TCHAR szFileName[MAX_PATH];
}BUILD_PACKET_END(S2C_FileInfo)
//����������������������
#pragma pack(pop)   //��
#endif//#pragma once//��
//����������������������
