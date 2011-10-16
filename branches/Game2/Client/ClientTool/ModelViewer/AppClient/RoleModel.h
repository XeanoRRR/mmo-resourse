class CRoleModel
{
	int			    m_iCurrentSelActionIndex;		//��ǰѡ��Ķ�������
	string			m_sFilePathName;				//ģ���ļ�·����
	CDisplayModel * m_pDisplayModel;				//��ɫ����ʾģ��

public:
	CRoleModel();
	~CRoleModel();

	int				GetCurrentSelActionIndex()						{ return m_iCurrentSelActionIndex; }
	string		  & GetFilePathName()								{ return m_sFilePathName; }
	CDisplayModel * GetDisplayModel()								{ return m_pDisplayModel; }
	void			SetCurrentSelActionIndex(int iActionIndex)		{ m_iCurrentSelActionIndex = iActionIndex; }
	void			SetFilePathName(string & sPathName)				{ m_sFilePathName = sPathName; }
	void			SetDisplayModel(CDisplayModel * pDisplayModel)	{ m_pDisplayModel = pDisplayModel; }
};