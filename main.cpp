#include "CryptData.h"

#include <cstdio>


struct TEST_STRUCT
{
	CD_DEFINE(float, Temp);
	
	void* temp2;
	
	TEST_STRUCT(float startval)
		: CD_MAKE_INITIALIZER(Temp, startval)
		, temp2(NULL)
	{ }
};

int main()
{
	//__CD_DATA_STORAGE<8>::Type T = 1776;

	//auto meme = return1776<8>();

	auto Ts = new TEST_STRUCT(1.2);

	//TEST_STRUCT Ts;
	//Ts->SetTemp(1.234F);

	//unsigned long long test = 0x1776;
	printf("Temp: %.3f\n", Ts->GetTemp());
	printf("Temp: %.3f\n", Ts->__Cd_name_prefix_Temp);


	///*unsigned long long test = 0x1776;
	//test += 0xDDFFFABCFFFFFFFF;
	//printf("Test: %p\n", test);
	//test -= 0xDDFFFABCFFFFFFFF;
	//printf("Test: %p\n", test);*/

	printf("Hash64: %p\n", __CD_Hash64<"Hello">(1776));
}
