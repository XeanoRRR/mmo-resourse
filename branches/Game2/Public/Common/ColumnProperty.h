/**ColumnProperty.h
** �б�������: �����ֶμ�������ݹ������
**Author: ��η
**Create Time:2010.03.23
*/

#ifndef COLUMN_PROPERTY_H
#define COLUMN_PROPERTY_H

class CGUID;
/// �����ݶ���
class CDataObject
{
public:
	///���캯��
	CDataObject();
	///��������
	~CDataObject();

	///
    bool operator ==(const CDataObject& right)
    {
        if(&right != this)
        {
            if( (right.m_Buf == m_Buf) && (right.m_Size == m_Size) )
                return true;
            else
                return false;
        }

        return false;
    }

	/// ������ݽӿ�
	void Release(void);

	/// ��ӿ�
	/// �������ݵ������Ϣ// ������ָ�� / ����4�ֽ�����
	void*   GetBuf(void)	{ return m_Buf;	}		
	/// ����������(BYTE)
	long	GetSize(void)	{ return m_Size;}		
	/// ������ָ�� / ����4�ֽ�����
	void	SetBuf(void* pVoid)	{ m_Buf	= pVoid;}	
	/// ����������(BYTE)
	void	SetSize(long lVal)	{ m_Size = lVal;}		

	//	--------------------- ���ݲ����ӿ� ---------------------	//
	///
	bool DelValue();
	///
	long GetLongValue();
	///
	long GetBufValue(void* buf, long bufSize);
	///
	const char* GetStringValue();
	///
	const CGUID& GetGUIDValue();
	///
	bool SetLongValue(long& lVal);
	///
	bool SetBufValue(void* pVoid, long lSize);
	///
	bool SetStringValue(const char* value, long lSize);
	///
	bool SetGUIDValue(const CGUID& value);

	///
	CDataObject& operator=(CDataObject& right);

private:
	///
	bool SetValueArray	(void* pVoid, long lBufSize);
	///
	bool AddValueArray(void* pVoid, long lSize);

	// �������ݵ������Ϣ
	/// ������ָ�� / ����4�ֽ�����
	void*	m_Buf;		
	/// ����������(BYTE), -1��ʾ����
	long	m_Size;		
};

/// ���ݶ��������
class CTableManager
{
public:
	///
	typedef std::map<long, CDataObject*> ColumnDataMap;
	///
	typedef ColumnDataMap::iterator ColumnDataMapItr;

	///
	CTableManager();
	///
	~CTableManager();
	///
	CTableManager& operator=(CTableManager& right);

	/// �������
	void Release(void);

	/// �������ݶ���
	CDataObject* CreateColumnData(long lAttrEnum);

	/// ɾ�����ݶ���
	bool DelColumnyData(long lAttrEnum);

	/// ȡ�����ݶ���
	CDataObject* GetColumnData(long lAttrEnum);

	/// ȡ�����ݶ���ֵ
	long	GetLongAttr(long lAttrEnum);
	///
	const char*	GetStringAttr(long lAttrEnum);
	///
	long	GetBufAttr(long lAttrEnum, void* buf, long bufSize);
	///
	long	GetGuidAttr(long lAttrEnum,  CGUID& guid);

	/// �������ݶ���ֵ
	bool	SetLongAttr(long lAttrEnum, long value);
	///
	bool	SetStringAttr(long lAttrEnum, const char* value);
	///
	bool	SetBufAttr(long lAttrEnum, void* buf, long bufSize);
	///
	bool	SetGuidAttr(long lAttrEnum, const CGUID& guid);
	

	/// �������ݶ���ֵ
	void	AddLongAttr(long lAttrEnum, long value);
	///
	void	AddStringAttr(long lAttrEnum, const char* value);
	///
	void	AddBufAttr(long lAttrEnum, void* buf, long bufSize);
	/// 
	void	AddTimeAttr(long lAttrEnum, long* buf, long bufSize);
	///
	void	AddGuidAttr(long lAttrEnum, const CGUID& guid);

	/// ɾ�����ݶ���ֵ
    void    DelAttr(long lAttrEnum);

	///
	ColumnDataMap& GetColumnPropertyMap(void) { return m_mapColumnData; }

private:
	///
	ColumnDataMap m_mapColumnData;
};


#endif//COLUMN_PROPERTY_H