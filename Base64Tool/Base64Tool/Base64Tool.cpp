#include "Base64.h"

#include <iostream>
using namespace std;

#include "httplib.h"

#include "json.h"

// ����Json��̬���ļ�
#pragma comment(lib, ".\\..\\Debug\\json_vc71_libmtd.lib")

void TestJson()
{
	Json::Value value;

	// ѧ������Ϣ{name:peter, gender:��, age:18, score: 70 80 90}
	// ����Ҫ���л������ݷŵ�Json��value��ȥ
	value["name"] = "peter";
	value["gender"] = "��";
	value["age"] = 18;

	int scores[] = { 70, 80, 90 };
	for (auto e : scores)
	{
		value["score"].append(e);
	}

	// ���л�: ���и�ʽ�����л�
	Json::StyledWriter sw;
	string strJsonData = sw.write(value);
	cout << strJsonData << endl;
	cout << "==================" << endl;

	// ���л���������ʽ���л�
	Json::FastWriter fw;
	strJsonData = fw.write(value);
	cout << strJsonData << endl;

	// ���л�֮��Student�����е��������л���һ���ַ�������
	// �����л��������յ����ַ������ݻ�ԭ��һ��������
	Json::Reader rd;
	Json::Value rdValue;
	rd.parse(strJsonData, rdValue);  // ��strJsonData�е����ݽ�����rdValue��
	cout << rdValue["name"] << endl;
	cout << rdValue["gender"] << endl;
	cout << rdValue["age"] << endl;

	for (auto e : rdValue["score"])
		cout << e << " ";
	cout << endl;
}


// ����Ҫ����������
// http����
// http��Ӧ
void helloworld(const httplib::Request& req, httplib::Response& rsp)
{
	rsp.set_content("<html><head><title>hello</title></head><body><h2>helloworld</h2></body></html>", "text/html");
	rsp.status = 200;
}


void Str2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	// ����ǰ������
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}

	// ���ı�ת��Ϊbase64
	Base64 base64;
	string strRst = base64.Encode(value["strtextdata"].asString());

	// �����������л�
	Json::StyledWriter sw;
	Json::Value wvalue;
	wvalue["base64Data"] = strRst;
	strRst = sw.write(wvalue);


	rsp.set_content(strRst, "text/html");
	rsp.status = 200;
}


void Base642Str(const httplib::Request& req, httplib::Response& rsp)
{
	// ����ǰ������
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}

	// ���ı�ת��Ϊbase64
	Base64 base64;
	string strRst = base64.Decode(value["base64data"].asString());

	// �����������л�
	Json::StyledWriter sw;
	Json::Value wvalue;
	wvalue["strtext"] = strRst;
	strRst = sw.write(wvalue);


	rsp.set_content(strRst, "text/html");
	rsp.status = 200;
}


void Pic2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	// ������req�л�ȡͼƬ����
	auto picFile = req.files;
	auto formData = picFile.find("strPicData")->second;  // ��ȡ��
	auto picData = formData.content;  // ��ȡ��������---�����ݾ���һ��ͼƬ

	// ��ͼƬ����(�����Ƹ�ʽ����)ת����base64����
	Base64 base64;
	string strRst;

	// ����base64��ʽ��ʾͼƬ��ǩ��������дû������
	strRst += "<img src='data:img/jpg;base64,";
	strRst += base64.Encode(picData);
	strRst += "'/>";

	// ��ת���Ľ������һ����Ӧ������ֵ�ͻ���
	// ��JSON����֯���ص�����
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

	// ע�᣺��������Ӧ�������Ӧ
	// Get--->���ӷ�����Get����--->helloworld���������д�����
	// ���������ע�᣺Post  /str_2_base64   Str2Base64

	s.set_base_dir(".//..//Debug");

	//s.Get("/", helloworld);
	s.Post("/str_2_base64", Str2Base64);
	s.Post("/base64_2_str", Base642Str);
	s.Post("/pic_2_base64", Pic2Base64);
	s.listen("192.168.31.171", 9000);
	return 0;
}