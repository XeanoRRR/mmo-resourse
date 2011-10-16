/********************************************************************
	created:	2010/11/04
	created:	4:11:2010   16:52
	filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\NotifySys\NotyfySys.h
	file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\NotifySys
	file base:	NotyfySys
	file ext:	h
	author:		zhaohang(kaba) (kuailekaba@gmail.com)
	
	purpose:	����������߼����ݹ���
*********************************************************************/
/**�����ַ������ݹ�����*/
class NotifyRenderStringMgr{
public:
	/**��Ⱦ���ȼ�����*/
	enum RenderQueue{
		RQ_FST,
		RQ_SEC,
		RQ_TRD,
		RQ_FORTH,
		RQ_FIFTH
	};
public:
	/**�����ȼ�������ַ����б�*/
	struct tagRQString{
		RenderQueue rq;
		std::string str;
		tagRQString(){
			rq = RQ_FST;
			str = "";
		}
		bool operator < (const tagRQString& ref){
			return rq < ref.rq;
		}
	};
	///���캯��,��ȡ����ʾ����
	NotifyRenderStringMgr();
	///��������
	~NotifyRenderStringMgr();

	/**������Ⱦ�ַ�����,ÿ����һ�������Զ�����
	@param rq
		���ȼ�
	@param str
	    �ַ�������
	*/
	static void AddText(std::string str,RenderQueue rq = RQ_FIFTH);
	/**��ȡ��Ⱦ�ַ��б�ĵ�һ��ֵ*/
	static string& FrontText();
	/**�Ƿ�Ϊ��*/
	static bool IsEmpty();

	/**	�Թ���ʱ���ʱ,ÿ֡����,�����������ʾʱ�䣬
	*	������ǰ��ʾ�ַ�����֪ͨ������ʾ��һ���������¼�ʱ
	*/
	static void UpdateTime();
private:
	///�ַ����б�
	static std::list<tagRQString> strList;
	///�������ʾʱ��
	static int d_maxRollTime;
	///��������
	static int d_iDir;
	///�����ٶ�,����/ÿ��
	static float d_fSpeed;
	///
	static int d_elapseTime;
	///������
	static void LoadSetup();
};