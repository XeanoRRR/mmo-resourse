
#import "../../../dbaccess/msado15.dll" no_namespace rename("EOF","adoEOF")
#include <string>
using namespace std;


//! ---------------------------------------��־���ݿ�����ķ�װ---------------------------------------
//!	ֻ��ִ���޷��ص�SQL���
//! ������ʱ�Զ�����DB������ʱ�ͷ�����
//! ������ʧЧ��ÿִ��SQL���ʱ�������³�������
//!---------------------------------------------------------------------------------------------------
class BillAdoBase
{
public:
	BillAdoBase(string strConnectionString);

	virtual ~BillAdoBase(void);

public:
	//! ִ��SQL���
	bool ExecuteCn(LPCSTR sql);
	// ȡ�����Ӷ���
	_ConnectionPtr& GetConnPtr(void) { return m_cnPtr; }
	
	//! ������
	bool OpenCn(void);
	//! �ر�����
	bool CloseCn(void);

	//! �����ݿ����Ӷ���
	_ConnectionPtr m_cnPtr;

	//! �����ַ�������
	string m_strConnectionString;

	// ��������#
	void PrintErr(LPCSTR msg, _com_error &e);
	void PrintErr(LPCSTR msg);
};