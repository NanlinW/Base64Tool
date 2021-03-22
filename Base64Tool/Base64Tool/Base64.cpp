#include "Base64.h"


std::string Base64::Encode(const std::string& strData)
{
	std::string strEncode;
	std::string strEncodeTable("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	unsigned char temp[4];

	size_t index = 0;
	size_t lineLength = 0;
	for (size_t i = 0; i < strData.size()/3; ++i)
	{
		// 3���ַ�һ��������ת��
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		temp[3] = strData[index++];

		// ȡ��һ���ֽڵĸ�6λ
		strEncode += strEncodeTable[temp[1] >> 2];

		// ȡ��һ���ֽڵĵ�2λ�����2���ֽڵĸ�4λƴ�ӳ�6������λ
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];

		// ȡ�ڶ����ֽڵĵ�4λ�����3���ֽڵĸ�2λƴ�ӳ�6������λ
		strEncode += strEncodeTable[((temp[2] << 2) | (temp[3] >> 6)) & 0x3F];

		// �������ֽ�ʣ��6������λ
		strEncode += strEncodeTable[temp[3] & 0x3F];

		lineLength += 4;
		if (76 == lineLength)
		{
			strEncode += "\r\n";
			lineLength = 0;
		}
	}

	size_t mod = strData.size() % 3;
	if (1 == mod)
	{
		// ģ��֮��ʣ��1���ֽ�,ע�⣺��==
		temp[1] = strData[index++];
		strEncode += strEncodeTable[(temp[1] & 0xFC) >> 2];
		strEncode += strEncodeTable[(temp[1] & 0x03) << 4];
		strEncode += "==";
	}
	else if (2 == mod)
	{
		// ģ��֮��ʣ��2���ֽ�,ע�⣺��=
		temp[1] = strData[index++];
		temp[2] = strData[index++];

		// ȡ��һ���ֽڵĸ�6������λ
		strEncode += strEncodeTable[(temp[1] & 0xFC) >> 2];

		// ȡ�ڶ����ֽڵĵ�2������λ����ڶ����ֽڵĸ�4λ
		strEncode += strEncodeTable[((temp[1] & 0x03) << 4) | ((temp[2] & 0xF0) >> 4)];

		strEncode += strEncodeTable[(temp[2] & 0x0F) << 2];
		strEncode += "=";
	}
	
	return strEncode;
}

std::string Base64::Decode(const std::string& strData)
{
	std::string strDecode;
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	size_t value = 0;  // ������������4��6������λ---�ܹ�24������λ
	size_t index = 0;
	while (index < strData.size())
	{
		// ע�⣺base64����ʱһ������ܹ�����76���ַ�������76���ַ�ʱ�����õ���һ��
		if (strData[index] != '\r' && strData[index + 1] != '\n')
		{
			// һ��û�н������

			// ������һ������--->�Ըñ�����Ϊ�������±꣬����������Ҹñ����ڱ�����е��±�
			value = DecodeTable[strData[index++]]<<18;

			// �����ڶ�������
			value += DecodeTable[strData[index++]] << 12;
			strDecode +=  ((value >> 16)&0xFF);

			if (strData[index] != '=')
			{
				// ��������������
				value += DecodeTable[strData[index++]] << 6;
				strDecode += ((value >> 8)&0xFF);

				if (strData[index] != '=')
				{
					// �������ĸ�����
					value += DecodeTable[strData[index++]];
					strDecode += (value&0xFF);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			// �����������е�ĩβ
			// ����/r/n
			index += 2;
		}
	}

	return strDecode;
}