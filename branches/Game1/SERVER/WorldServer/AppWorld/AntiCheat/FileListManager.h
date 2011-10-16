///
/// @file FileNameListManager.h
/// @author Kevin Lynx
/// @brief to manage some file buffer lists in AntiCheat module.
///
#ifndef ___FILE_LIST_MANAGER_H_
#define ___FILE_LIST_MANAGER_H_

#include <vector>
#include <string>

namespace AntiCheat
{
	// set record file name flag
#define RECORD_NAME

	class CFileListManager
	{
	public:
		struct Buffer
		{
			void *buf;
			size_t size;
#ifdef RECORD_NAME
			std::string name;
#endif
		};
		typedef std::vector<Buffer> FileBufferList;
	public:
		/// װ������ͼƬ��Դ�������б�
		bool Init();

		/// ��������ļ��б�
		void Release();

		/// �ض���������ٻ�ȡ
		bool ReInit();

		/// �����ȡ����ͼ�ļ�
		const Buffer &GetRandBack() const;

		/// �����ȡСͼ�ļ�
		const Buffer &GetRandSmall() const;

		/// �����ȡ��X��ͼƬ�ļ�
		const Buffer &GetRandX() const;

		/// ��ȡ��A����B����C����D�������ļ���
		/// sel : 1-A, 2-B, 3-C, 4-D
		const Buffer &GetSelSign( int sel ) const;

		/// ��ȡALPHAͨ��ͼ
		const Buffer &GetAlpha() const;

	private:
		/// װ��һ���ļ��������ָ��������
		bool LoadFile( const std::string &file, FileBufferList &file_list );

		/// װ��һ���ļ�
		bool LoadFileList( const std::list<std::string> &file_name_list, FileBufferList &file_list );

		/// ���һ���ļ��б�
		void ReleaseFileList( FileBufferList &file_list );

	private:
		FileBufferList m_BackList;
		FileBufferList m_SmallList;
		FileBufferList m_XList;
		FileBufferList m_SelList;
		FileBufferList m_AlphaList;
	};

}

#endif