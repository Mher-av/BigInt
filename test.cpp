#include <iostream>
#include "BigInt.h"
using namespace std;
int main()
{
	BigInt i("234278477388299839288377266677");
	BigInt j("475883992884857488277488372738");
	BigInt k("2");
	BigInt c = i + j;
	cout << c.toString()<<endl;
	c = i - j;
	cout << c.toString()<<endl;
	c = i * j;
	cout << c.toString();
	c = j / k;
	cout << c.toString();
	system("pause");
}