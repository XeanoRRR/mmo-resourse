///
/// @file BaseFunc.h
/// @brief ȫ�ֻ���֧�ֺ���
/// @see BaseFunc.cpp
///
#ifndef ___BASE_FUNC_H_
#define ___BASE_FUNC_H_

struct lua_State;
namespace ScriptFuncs
{
	void OpenBaseLibs( lua_State *L );

	// registered by tolua++ below
	//tolua_begin
	///
	/// ����һ����ɫֵ��������AddInfo֮��ĺ�������
	/// 
	/// @param a Alpha����
	/// @param r Red����
	/// @param g Green����
	/// @param b Blue����
	/// @return ��ɫֵ
	ulong ARGB( uchar a, uchar r, uchar g, uchar b );

	///
	/// ע��һ���ű���ʱ������ʱ������ʱִ��һ���ű�
	///
	/// ��ʱ������ִ�еĽű�ʵ����Դ/Ŀ�����Ϊ��
	/// @param file �ű��ļ���
	/// @param userData �û���ʶ��������ע����ʱ��
	/// @param trigerTime ��һ�δ���ʱ��
	/// @param interTime ÿ�δ����ļ��ʱ�䣬Ϊ0ʱ��ֻ����һ��
	/// @return ��ʱ��ID
	/// @see UnRegisterTime
	long RegisterTimer( const char *file, long userData, long trigerTime, long interTime );

	///
	/// ע��һ���ű���ʱ��
	///
	/// @param timeID ��ʱ��ID����RegisterTime����
	/// @param userData �û���ʶ��
	/// @see RegisterTimer
	void UnRegisterTimer( long timerID, long userData );
	//tolua_end

}

#endif
