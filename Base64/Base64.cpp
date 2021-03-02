#include "Base64.h"

std::string Base64::Encode(const std::string& strData)//编码模块
{
	std::string strEncode;
	std::string strEncodeTable("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"); //Base64编码表
	unsigned char temp[4];
	size_t index = 0;
	size_t lineLength = 0;//记录base64每行的字节数
	for (size_t i = 0; i < strData.size()/3; ++i)
	{
		//3个字符为一组进行base64编码转换
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		temp[3] = strData[index++];

		//转换后每个字节都由 补位的00 + 拼接后的6位 组成
		//取第一个字节的高6位
		strEncode += strEncodeTable[temp[1] >> 2];

		//取第一个字节的低2位 + 第二字节的高四位拼接
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];

		//取第二个字节的低4位 + 第三个字节的高2位拼接
		strEncode += strEncodeTable[((temp[2] << 2) | (temp[3] >> 6)) & 0x3F];

		//取第三个字节的低6位
		strEncode += strEncodeTable[temp[3] & 0x3F];

		lineLength += 4;
		if (lineLength == 76)//base64一行只能显示76的字节
		{
			strEncode += "\r\n";
			lineLength = 0;
		}
	}

	size_t mod = strData.size() % 3;//查看传入数据字节数多出3的倍数几个字节
	if (mod == 1)//如果多出3的倍数一个字节，补两个 = 符号
	{
		temp[1] = strData[index++];
		//取多出来的这个字节的高6位
		strEncode += strEncodeTable[temp[1] >> 2];
		//取多出来的这个字节的低2位 + 补4位的0进行拼接
		strEncode += strEncodeTable[(temp[1] & 0x03) << 4];
		//最后补两个 = 符号
		strEncode += "==";
	}
	else if (mod == 2)//如果多出3的倍数两个字节，补一个 = 符号
	{
		temp[1] = strData[index++];
		temp[2] = strData[index++];
		//取多出的第一个字节的高6位
		strEncode += strEncodeTable[temp[1] >> 2];
		//取多出的第一个字节的低2位 + 多出的第二个字节的高4位拼接
		strEncode += strEncodeTable[((temp[1] << 4) | (temp[2] >> 4)) & 0x3F];
		//取多出的第二个字节的低4位 + 补2位的0进行拼接
		strEncode += strEncodeTable[(temp[2] & 0x0F) << 2];
		//最后补一个 = 符号
		strEncode += "=";
	}
	return strEncode;
}

std::string Base64::Decode(const std::string& strData)//解码模块
{
	std::string strDecode;
	//快速解码表
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
	size_t value = 0; //保存解码的4组6个比特位（总共24个比特位）
	size_t index = 0;
	while (index < strData.size())
	{
		//编码时一行只能放置76个字符，超过76个字符会放到下一行
		if (strData[index] != '\r' && strData[index + 1] != '\n')//说明一行还没解码完毕
		{
			//解析第一个编码
			value = DecodeTable[strData[index++]] << 18;
			//解析第二个编码
			value = (DecodeTable[strData[index++]] << 12) | value;
			strDecode += ((value >> 16) & 0xFF);//存储第一个解码后的数据16-23位

			if (strData[index] != '=')
			{
				//解析第三个编码
				value = (DecodeTable[strData[index++]] << 6) | value;
				strDecode += ((value >> 8) & 0xFF);//存储第二个解码后的数据08-15位

				if (strData[index] != '=')
				{
					//解析第四个编码
					value = (DecodeTable[strData[index++]]) | value;
					strDecode += (value & 0xFF);//存储第三个解码后的数据00-07位
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			//解码到该行的末尾了
			index += 2; //跳过 \r\n
		}
	}
	return strDecode;
}