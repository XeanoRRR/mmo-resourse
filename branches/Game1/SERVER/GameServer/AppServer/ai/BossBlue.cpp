#include "StdAfx.h"
#include ".\BossBlue.h"
#include "..\Player.h"
#include "..\Monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �������ܿ������
// BOSS��IDLEʱ����Ҫ��¼ʱ�䣬��WAKEUPʱͨ����ʱ����л�Ѫ���ܣ���δ���
// 2006��4��26�� Bugs �汾ǩ��ע�� 


CBossBlue::CBossBlue(void)
{
	// ��ʼ��false
	memset(_isUsed,0,sizeof(bool)*8);
}

CBossBlue::~CBossBlue(void)
{
	memset(_isUsed,0,sizeof(bool)*8);
}
