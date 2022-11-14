#include "CryptData.h"

#include <cstdio>


struct TEST_STRUCT
{
	CD_DEFINE(unsigned long long, Temp);
	
	void* temp2;
	
	TEST_STRUCT(unsigned long long startval)
		: CD_MAKE_INITIALIZER(Temp, startval)
		, temp2(NULL)
	{ }
};

int main()
{
	auto Ts = new TEST_STRUCT(0xFFABCDEF);
	printf("Temp: %p\n", Ts->GetTemp());
	printf("Temp: %p\n", Ts->__Cd_np_Temp);
	Ts->SetTemp(0xFFAAFFAAFFAAFFAA);
	printf("Temp: %p\n", Ts->GetTemp());
	printf("Temp: %p\n", Ts->__Cd_np_Temp);


	///*unsigned long long test = 0x1776;
	//test += 0xDDFFFABCFFFFFFFF;
	//printf("Test: %p\n", test);
	//test -= 0xDDFFFABCFFFFFFFF;
	//printf("Test: %p\n", test);*/

	printf("Hash64: %p\n", __CD_Hash64<"Hello">(1776));

}
