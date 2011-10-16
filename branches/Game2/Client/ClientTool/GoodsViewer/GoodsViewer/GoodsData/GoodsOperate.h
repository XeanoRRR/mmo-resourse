/**
* @filename:GoodsOperate
* @date: 2010/2/3
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ��װGoods�����ݲ���
*/

#pragma  once
#include "GoodsAttrTypeDef.h"
#include <CEGUI.h>
namespace UIData{
	const int gGoodsTypeNum = 26 ;
	using namespace CEGUI;
	class GoodsOperate
	{
	public:
		/// @brief
		/// ���캯��,�޲���
		/// @param
		/// 
		/// @return
		/// 
		GoodsOperate();

		/// @brief
		/// �����������޲���
		/// @param
		/// 
		/// @return
		/// 
		~GoodsOperate();

		/// @brief
		/// �Զ����ƴ�goodslist.dat�ж�ȡGoods��Ϣ
		/// @param const char* FileName
		/// �ļ����ƣ�����·��),Ĭ��Ϊ��goodslist.dat��
		/// @return
		/// ��ȡ�ɹ�����true,ʧ�ܷ���false
		bool LoadGoodsListData(const char* FileName = "goodslist.dat");

		/// @brief
		/// ��ȡ��Ʒ��������������
		/// @param const char* FileName
		/// �ļ����ƣ�����·��),Ĭ��Ϊ��itemtype.dat��
		/// @return
		/// �ɹ�����true��ʧ�ܷ���false
		bool LoadGoodsItemTypeData(const char* FileName = "itemtype.dat");

		
		/// @brief
		/// ������Ʒ��Ϣָ��·�����ļ���
		/// @param SavePath
		/// ������ļ�·��
		/// @return
		/// �ɹ����淵��true��ʧ�ܷ���false
		bool SaveGoodsListData(const char* SavePath = "");

		bool SaveGoodsListDataForClient();

		bool CountConvetEGoodsProperty(tagGoodAttr* pGoods );

		/// @brief
		/// ������Ʒ�����Ż�ȡ��Ʒ���ݽṹ
		/// @param Index
		/// ��Ʒ��������
		/// @return tagGoodAtttr*
		/// ����ָ�������ŵ���Ʒ���ݽṹ���ָ�룬û�оͷ���NULL;
		const tagGoodAttr* GetGoodAttrByIndex(size_t Index) const ;

		/// @brief
		/// ������Ʒԭʼ���ƻ�ȡ��Ʒ���ݽṹ��
		/// @param &OgriName
		/// 
		/// @return tagGoodAttr*
		/// ����ָ��ԭʼ���Ƶ���Ʒ���ݽṹ��ָ��
		const tagGoodAttr* GetGoodAttrByOgriName(const String &OgriName) const;

		/// @brief
		/// ������Ʒ����
		/// @param
		/// 
		/// @return size_t
		/// ������Ʒ����
		size_t GetGoodsTotalNum() const { return m_vecGoodAttr.size();}

		/// @brief
		/// ��ȡָ����������Ʒ���������ַ���
		/// @param Index
		/// 
		/// @return &string
		/// 
		static const std::string GetGoodsType(size_t Index) ;

		/// @brief
		/// ������Ʒ����������Ŀ��
		/// @param void
		/// 
		/// @return size_t
		/// 
		size_t GetGoodItemTypeSize() const { return m_vecGoodItemType.size() ;}

		//��������������Ʒ���������ַ�������
		std::string& GetGoodItemType(const size_t Index) {return m_vecGoodItemType[Index];}
	protected:
	private:
		std::vector<tagGoodAttr> m_vecGoodAttr;
		std::vector<std::string> m_vecGoodItemType;
		static const std::string s_vecGoodsType[26];
		};
}