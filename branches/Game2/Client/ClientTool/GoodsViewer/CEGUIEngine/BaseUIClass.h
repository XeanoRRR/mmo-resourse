/**
* @filename:BaseUIClass.h
* @date: 2010/1/27
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ����UI��(���洰�ڹ����¼���Ӧ)�Ĵ�����࣬����UI��ʱ�����BaseUIClass������,\
*           ���������Ϊ�˷���CEGUIManagerִ��UI��Ĺ��캯��
*/

#pragma  once
///ǰ������
class CEGUIManager;
#include <CEGUI.h>
namespace UIClass
{
	using namespace CEGUI;
	class BaseUIClass
	{
	public:
		BaseUIClass(){};
		virtual ~BaseUIClass(){};
		//virtual  BaseUIClass* CreateObjClass() = 0;
		//virtual bool DestroyObjClass(BaseUIClass *pBaseUIClass) = 0;
		virtual bool Init()=0;///<���麯������������д�����������CEGUI���ڴ��������ص������ڣ��¼���������ע��ȹ���
	protected:
		Window* m_RootWindow;///< ָ��layout��RootWindow
	private:
		
		friend CEGUIManager;///<CEGUIManager��Ϊ��Ԫ�࣬ʹ�ÿ��Ե���Create*��Destory*����
	};
}