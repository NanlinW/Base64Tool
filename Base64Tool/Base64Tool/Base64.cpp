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
		// 3个字符一组来进行转换
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		temp[3] = strData[index++];

		// 取第一个字节的高6位
		strEncode += strEncodeTable[temp[1] >> 2];

		// 取第一个字节的低2位，与第2个字节的高4位拼接成6个比特位
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];

		// 取第二个字节的低4位，与第3个字节的高2位拼接成6个比特位
		strEncode += strEncodeTable[((temp[2] << 2) | (temp[3] >> 6)) & 0x3F];

		// 第三个字节剩余6个比特位
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
		// 模完之后剩余1个字节,注意：补==
		temp[1] = strData[index++];
		strEncode += strEncodeTable[(temp[1] & 0xFC) >> 2];
		strEncode += strEncodeTable[(temp[1] & 0x03) << 4];
		strEncode += "==";
	}
	else if (2 == mod)
	{
		// 模完之后剩余2个字节,注意：补=
		temp[1] = strData[index++];
		temp[2] = strData[index++];

		// 取第一个字节的高6个比特位
		strEncode += strEncodeTable[(temp[1] & 0xFC) >> 2];

		// 取第二个字节的低2个比特位，与第二个字节的高4位
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

	size_t value = 0;  // 用来保存解码的4组6个比特位---总共24个比特位
	size_t index = 0;
	while (index < strData.size())
	{
		// 注意：base64编码时一行最多能够放置76个字符，超过76个字符时，放置到下一行
		if (strData[index] != '\r' && strData[index + 1] != '\n')
		{
			// 一行没有解码完毕

			// 解析第一个编码--->以该编码作为解码表的下标，到解码表中找该编码在编码表中的下标
			value = DecodeTable[strData[index++]]<<18;

			// 解析第二个编码
			value += DecodeTable[strData[index++]] << 12;
			strDecode +=  ((value >> 16)&0xFF);

			if (strData[index] != '=')
			{
				// 解析第三个编码
				value += DecodeTable[strData[index++]] << 6;
				strDecode += ((value >> 8)&0xFF);

				if (strData[index] != '=')
				{
					// 解析第四个编码
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
			// 解码来到该行的末尾
			// 跳过/r/n
			index += 2;
		}
	}

	return strDecode;
}