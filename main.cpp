#include "CryptData.h"

#include <cstdio>


//struct TEST_STRUCT
//{
//	CD_DEFINE(void*, Temp);
//	
//	TEST_STRUCT(void* startval)
//		: CD_MAKE_INITIALIZER(Temp, startval)
//	{ }
//};

struct TEST_STRUCT
{
	CD_DEFINE(double, Temp);

	TEST_STRUCT(double startval)
		: CD_MAKE_INITIALIZER(Temp, startval)
	{ }
};


//unsigned char meme[] = { 0xFF };
//
//__forceinline const float __Name(float value)
//{
//	typedef __CD_DATA_STORAGE<4>::Type(*memefn)(__CD_DATA_STORAGE<4>::Type);
//
//	//return ((float(*)(__CDDS(float)))(&meme))(__COERCE_TYPE(value));
//	return ((float(*)(__CDDS(float)))(&meme))(__COERCE_TYPE(meme));
//}


int main()
{

	//constexpr auto meme = __CD_CRYPT_SHELLCODE<1776, int>::Data;

	//auto Ts = new TEST_STRUCT((void*)0x1000231);
	////Ts->SetTemp((void*)0x1992);
	//printf("Temp:    %p\n", Ts->GetTemp());
	//printf("crypted: %p\n", Ts->__Cd_np_Temp);
	//Ts->SetTemp((void*)0x1776);
	//printf("Temp:    %p\n", Ts->GetTemp());
	//printf("crypted: %p\n", Ts->__Cd_np_Temp);


	auto Ts = new TEST_STRUCT(1.2);
	printf("Temp:    %.3f\n", Ts->GetTemp());
	printf("crypted: %.3f\n", Ts->__Cd_np_Temp);
	Ts->SetTemp(1.52312);
	printf("Temp:    %.3f\n", Ts->GetTemp());
	printf("crypted: %.3f\n", Ts->__Cd_np_Temp);

}
