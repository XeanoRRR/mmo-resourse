#pragma once

class LogClient;

//! �߼���־�ӿڷ�װ��
//! ����־������װΪ���߼�����Ķ���ָ��
class LogicLogInterface
{
public:
	LogicLogInterface(LogClient *pLogClient);


private:

	//!							�õ���ǰʱ���ַ�����00-00-00 00:00:00��ʽ��
	void						GetCurrTimeString(string &strTime);

private:

	//!							��־�ͻ��˶���
	LogClient					*m_pLogClient;

};