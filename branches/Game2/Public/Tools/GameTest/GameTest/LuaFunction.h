#pragma once

class CLuaFunction
{
public:
	CLuaFunction(void);
	~CLuaFunction(void);

	static CLuaFunction* AddGoods(CString OriginName,int Num,int lLevel);
private:
	CString OriginName;			//ԭʼ��
	int Num;				//����
	int lLevel;				//�ȼ�
};
