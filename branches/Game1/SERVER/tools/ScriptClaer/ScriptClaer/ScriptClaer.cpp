// ScriptClaer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UpdateFolder.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(argv[1] == NULL || argv[2] == NULL)
	{
		printf("�밴���·�ʽ����ת��Ŀ��λ�úͽ�����λ�ã�\r\nScriptClaer C:/script C:/script_new \r\n������ֹ��\r\n");
		return 0;
	}

	printf("ȷ������Ĳ���������ʼ��Y/N��");
	char UserEnter = getchar();
	if ('N' == UserEnter || 'n' == UserEnter)
	{
		printf("�޷���%s���û���ֹ��\r\n", argv[1]);
		return 0;
	}

	UpdateFolder folder;
	if(!folder.LoadDataFromDir(argv[1]))
	{
		printf("�޷��򿪡�%s����������ֹ��\r\n", argv[1]);
		return 0;
	}
	
	if (!folder.WriteDataToDir(argv[2]))
	{
		printf("д�롾%s��λ���ļ�����Ϊ�㣬������ɣ�\r\n", argv[1]);
	}

	printf("������ϣ�\r\n");
	return 0;
}

