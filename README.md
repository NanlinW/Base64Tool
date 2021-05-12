# Base64Tool
一个在线的base64转换工具，支持文本的base64可逆转化以及图片的base64转化。

# 背景
在计算机中一个字节共有256种，即ascii码表，而ascii码的128~255之间的值是不可见字符，对于一些只支持可见字符的协议，比如邮件传输协议(SMTP)只支持可见的ASCII字符的传递，如果要传输二进制文件，比如：图片、视频是无法实现的，因此就有了base64编码格式，Base64编码格式对于所有二进制格式的数据，都可以转化为可显示的字符。这样在想传输不可见字符时，就可以先将文本编码成base64格式，然后进行传输，最后再进行解码就得到了原数据

# 编写语言/环境
C++ / Visual Studio 2019

# 示例及过程
见博客：
https://blog.csdn.net/NanlinW/article/details/113480369

