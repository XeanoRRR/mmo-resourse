


#ifndef _OrganiseSys_H_
#define _OrganiseSys_H_
#pragma once

class COrganiseSys
{
public:
	COrganiseSys(void);
	~COrganiseSys(void);

	void Release();

	void DecodeFromDataBlock(DBReadSet& dbRead,bool bSelf);

private:
	//	����
	CGUID m_FamilyGuid;
	//	����
	CGUID m_UnionGuid;

	// �л�ͼ��CRC
	DWORD m_dwFacIconCRC;

	//	�ٽ׵ȼ�
	DWORD m_lFamilyJobLevel;
	//	������
	string	m_strFamilyName;
	//	����ƺ�
	string	m_strFamilyTitle;
};
#endif//_OrganiseSys_H_