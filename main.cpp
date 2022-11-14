#include "CryptData.h"

#include <cstdio>


struct TEST_STRUCT
{
	CD_DEFINE(void*, Temp);
	
	TEST_STRUCT(void* startval)
		: CD_MAKE_INITIALIZER(Temp, startval)
	{ }
};


int main()
{
	auto Ts = new TEST_STRUCT((void*)0xFFABCDEF);
	printf("Temp:    %p\n", Ts->GetTemp());
	printf("crypted: %p\n", Ts->__Cd_np_Temp);
	Ts->SetTemp((void*)0xFFAAFFAAFFAAFFAA);
	printf("Temp:    %p\n", Ts->GetTemp());
	printf("crypted: %p\n", Ts->__Cd_np_Temp);
}
