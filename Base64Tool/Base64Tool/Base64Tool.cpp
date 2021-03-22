#include "Base64.h"

#include <iostream>
using namespace std;

#include "httplib.h"

#include "json.h"

// 加载Json静态库文件
#pragma comment(lib, ".\\..\\Debug\\json_vc71_libmtd.lib")

void TestJson()
{
	Json::Value value;

	// 学生新信息{name:peter, gender:男, age:18, score: 70 80 90}
	// 将需要序列化的数据放到Json的value中去
	value["name"] = "peter";
	value["gender"] = "男";
	value["age"] = 18;

	int scores[] = { 70, 80, 90 };
	for (auto e : scores)
	{
		value["score"].append(e);
	}

	// 序列化: 带有格式的序列化
	Json::StyledWriter sw;
	string strJsonData = sw.write(value);
	cout << strJsonData << endl;
	cout << "==================" << endl;

	// 序列化：不带格式序列化
	Json::FastWriter fw;
	strJsonData = fw.write(value);
	cout << strJsonData << endl;

	// 序列化之后将Student对象中的数据序列化到一个字符串中了
	// 反序列化：将接收到的字符串数据还原到一个对象中
	Json::Reader rd;
	Json::Value rdValue;
	rd.parse(strJsonData, rdValue);  // 将strJsonData中的内容解析到rdValue中
	cout << rdValue["name"] << endl;
	cout << rdValue["gender"] << endl;
	cout << rdValue["age"] << endl;

	for (auto e : rdValue["score"])
		cout << e << " ";
	cout << endl;
}


// 必须要有两个参数
// http请求
// http响应
void helloworld(const httplib::Request& req, httplib::Response& rsp)
{
	rsp.set_content("<html><head><title>hello</title></head><body><h2>helloworld</h2></body></html>", "text/html");
	rsp.status = 200;
}


void Str2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	// 解析前段数据
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}

	// 将文本转换为base64
	Base64 base64;
	string strRst = base64.Encode(value["strtextdata"].asString());

	// 先来进行序列化
	Json::StyledWriter sw;
	Json::Value wvalue;
	wvalue["base64Data"] = strRst;
	strRst = sw.write(wvalue);


	rsp.set_content(strRst, "text/html");
	rsp.status = 200;
}


void Base642Str(const httplib::Request& req, httplib::Response& rsp)
{
	// 解析前段数据
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}

	// 将文本转换为base64
	Base64 base64;
	string strRst = base64.Decode(value["base64data"].asString());

	// 先来进行序列化
	Json::StyledWriter sw;
	Json::Value wvalue;
	wvalue["strtext"] = strRst;
	strRst = sw.write(wvalue);


	rsp.set_content(strRst, "text/html");
	rsp.status = 200;
}


void Pic2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	// 从请求req中获取图片数据
	auto picFile = req.files;
	auto formData = picFile.find("strPicData")->second;  // 获取表单
	auto picData = formData.content;  // 获取表单的内容---表单内容就是一张图片

	// 将图片数据(二进制格式数据)转化成base64编码
	Base64 base64;
	string strRst;

	// 按照base64格式显示图片标签中内容填写没有问题
	strRst += "<img src='data:img/jpg;base64,";
	strRst += base64.Encode(picData);
	strRst += "'/>";

	// 将转换的结果构造一个响应包返回值客户端
	// 用JSON来组织返回的数据
	Json::Value value;
	value["picbase64Data"] = strRst;

	Json::StyledWriter sw;
	strRst = sw.write(value);
	rsp.set_content(strRst, "text/html");
	rsp.status = 200;
}


int main()
{

	httplib::Server s;

	// 注册：本次请求应该如何响应
	// Get--->链接服务器Get请求--->helloworld函数来进行处理了
	// 对请求进行注册：Post  /str_2_base64   Str2Base64

	s.set_base_dir(".//..//Debug");

	//s.Get("/", helloworld);
	s.Post("/str_2_base64", Str2Base64);
	s.Post("/base64_2_str", Base642Str);
	s.Post("/pic_2_base64", Pic2Base64);
	s.listen("192.168.31.171", 9000);
	return 0;
}