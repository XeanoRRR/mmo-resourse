/**
* @filename:RankPage.h
* @date: 2010/3/23
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ���а����UI
*/
#pragma  once
#include "../../GamePage.h"
//ǰ��������
class RankPageEvent;
///���а�UI������
class RankPage:public GamePage
{
public:    
	///
	RankPage();
	///
	virtual ~RankPage();
	///
	virtual void LoadPage();
	///
	virtual void ReleasePage();
	///
	virtual bool Open();
	///
	virtual bool Close();

	///���½����ϵ�������ʾ
	void UpdatePage();

private:
	/// UI�����¼�
	RankPageEvent* m_event;
};