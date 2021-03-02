#include <iostream>
#include "Base64.h"
using namespace std;

int main()
{
	Base64 b;
	std::string res = b.Encode("Lucy");
	cout << res << endl;
	std::string res2 = b.Decode(res);
	cout << res2 << endl;
	return 0;
}