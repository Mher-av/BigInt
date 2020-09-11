#include <iostream>
#include "BigInt.h"

int main()
{
	BigInt i("234278477388299839288377266677");
	BigInt j("475883992884857488277488372738");
	BigInt k("2");
	BigInt c = i + j;
	std::cout << c.toString()<<endl;
	c = i - j;
	std::cout << c.toString()<<endl;
	c = i * j;
	std::cout << c.toString();
	c = j / k;
	std::cout << c.toString();
}
