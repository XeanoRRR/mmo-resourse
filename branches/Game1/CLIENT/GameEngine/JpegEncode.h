#pragma once

namespace jpeg
{
	class JpegEncode
	{

	public:
		FILE  *m_pOutFile;

		int    buffWidth;
		int	   buffHeight;

		int    m_iWidth; //ԭʼλͼ���
		int    m_iHeight;//ԭʼλͼ�߶�

		struct tagRGB
		{
			BYTE r,g,b;
		};

        tagRGB *m_pData;//ԭʼλͼ���� rgb

		void CreateBitmap(int iWidth,int iHeight);
		void SetPixel(int iX,int iY,DWORD dwColor);
		void DestroyBitmap();

		void Write(const char *pszFileName, int Q);

		JpegEncode(void);
		~JpegEncode(void);

	};
}