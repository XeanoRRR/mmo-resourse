///
/// @file GUIDDoc.h
/// @brief GUID��غ���
/// @details ���ļ�������doxygen�Զ������ű���GUID��ʹ��˵��������C++ʹ��
///

///
/// GUID����
///
/// ������ʽ��Ϊ2�֣�
/// @code local g=CGUID() --������GUID����lua�Զ�����
/// CGUID:CreateGUID( g ) --������ֵ
/// @endcode
/// ����
/// @code local g=CGUID:new() --��Ҫ�Լ��ֶ�ɾ��
/// CGUID:CreateGUID( g )
/// g:delete() --�ֶ�ɾ��
/// @endcode
///
class CGUID
{
public:
	///
	/// ���ַ�������GUIDֵ
	///
	CGUID( const char *str );

	///
	/// �ж�����GUID�Ƿ����
	/// 
	/// @code local g1=CGUID(), g2=CGUID()
	/// if g1==g2 then something end
	/// @endcode
	/// @return true��ʾ���
	///
	bool operator == ( const CGUID &other ) const;

	///
	/// ��ȡ��GUID���ַ�����ʾ��ʽ
	///
	/// @return �ַ���
	///
	const char *tostring() const;

	///
	/// �жϸ�GUID�Ƿ�Ϊ��
	/// @return true��ʾΪ��
	///
	bool IsInvalid();

	///
	/// ���յ�GUID����һ��ֵ
	/// @code local g=CGUID()
	/// CGUID:CreateGUID( g )
	/// @endcode
	/// @param guid �����õ�GUID����
	///
	static bool CreateGUID( CGUID &guid );

	///
	/// һ���յ�GUID����
	/// @code local g=CGUID()
	/// if g==CGUID.GUID_INVALID then something end
	/// @endcode
	///
	static CGUID GUID_INVALID;
};
