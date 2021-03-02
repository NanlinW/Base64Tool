#include "Base64.h"

std::string Base64::Encode(const std::string& strData)//����ģ��
{
	std::string strEncode;
	std::string strEncodeTable("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"); //Base64�����
	unsigned char temp[4];
	size_t index = 0;
	size_t lineLength = 0;//��¼base64ÿ�е��ֽ���
	for (size_t i = 0; i < strData.size()/3; ++i)
	{
		//3���ַ�Ϊһ�����base64����ת��
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		temp[3] = strData[index++];

		//ת����ÿ���ֽڶ��� ��λ��00 + ƴ�Ӻ��6λ ���
		//ȡ��һ���ֽڵĸ�6λ
		strEncode += strEncodeTable[temp[1] >> 2];

		//ȡ��һ���ֽڵĵ�2λ + �ڶ��ֽڵĸ���λƴ��
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];

		//ȡ�ڶ����ֽڵĵ�4λ + �������ֽڵĸ�2λƴ��
		strEncode += strEncodeTable[((temp[2] << 2) | (temp[3] >> 6)) & 0x3F];

		//ȡ�������ֽڵĵ�6λ
		strEncode += strEncodeTable[temp[3] & 0x3F];

		lineLength += 4;
		if (lineLength == 76)//base64һ��ֻ����ʾ76���ֽ�
		{
			strEncode += "\r\n";
			lineLength = 0;
		}
	}

	size_t mod = strData.size() % 3;//�鿴���������ֽ������3�ı��������ֽ�
	if (mod == 1)//������3�ı���һ���ֽڣ������� = ����
	{
		temp[1] = strData[index++];
		//ȡ�����������ֽڵĸ�6λ
		strEncode += strEncodeTable[temp[1] >> 2];
		//ȡ�����������ֽڵĵ�2λ + ��4λ��0����ƴ��
		strEncode += strEncodeTable[(temp[1] & 0x03) << 4];
		//������� = ����
		strEncode += "==";
	}
	else if (mod == 2)//������3�ı��������ֽڣ���һ�� = ����
	{
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		//ȡ����ĵ�һ���ֽڵĸ�6λ
		strEncode += strEncodeTable[temp[1] >> 2];
		//ȡ����ĵ�һ���ֽڵĵ�2λ + ����ĵڶ����ֽڵĸ�4λƴ��
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];
		//ȡ����ĵڶ����ֽڵĵ�4λ + ��2λ��0����ƴ��
		strEncode += strEncodeTable[(temp[2] & 0x0F) << 2];
		//���һ�� = ����
		strEncode += "=";
	}
	return strEncode;
}

std::string Base64::Decode(const std::string& strData)//����ģ��
{
	std::string strDecode;
	//���ٽ����
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
	size_t value = 0; //��������4��6������λ���ܹ�24������λ��
	size_t index = 0;
	while (index < strData.size())
	{
		//����ʱһ��ֻ�ܷ���76���ַ�������76���ַ���ŵ���һ��
		if (strData[index] != '\r' && strData[index + 1] != '\n')//˵��һ�л�û�������
		{
			//������һ������
			value = DecodeTable[strData[index++]] << 18;
			//�����ڶ�������
			value = (DecodeTable[strData[index++]] << 12) | value;
			strDecode += ((value >> 16) & 0xFF);//�洢��һ������������16-23λ

			if (strData[index] != '=')
			{
				//��������������
				value = (DecodeTable[strData[index++]] << 6) | value;
				strDecode += ((value >> 8) & 0xFF);//�洢�ڶ�������������08-15λ

				if (strData[index] != '=')
				{
					//�������ĸ�����
					value = (DecodeTable[strData[index++]]) | value;
					strDecode += (value & 0xFF);//�洢����������������00-07λ
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			//���뵽���е�ĩβ��
			index += 2; //���� \r\n
		}
	}
	return strDecode;
}