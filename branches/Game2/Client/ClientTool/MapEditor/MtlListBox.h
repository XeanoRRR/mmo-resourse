#pragma once

#include "..\..\engine\render.h"
#include "..\..\engine\ui.h"

enum CELL_MATERIAL
{
		CM_NULL = 0, //��
		CM_GRASS,    //�ݵ�
		CM_MUD,		 //���
		CM_SAND,     //ɳ��
		CM_WOOD,     //ľ��
		CM_STONE,    //ʯ��
		CM_WATER,    //ˮ
		CM_SNOW,	 //ѩ��
		CM_MARISH,	 //����
		CM_END
};

class MtlListBox : public ui::ListBox
{
public:

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent);

	MtlListBox(void);
	~MtlListBox(void);
};
