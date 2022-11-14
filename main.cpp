#include "CryptData.h"

#include <cstdio>


//struct TEST_STRUCT
//{
//	CD_DEFINE(void*, Temp);
//	
//	TEST_STRUCT(void* startval)
//		: CD_INIT(Temp, startval)
//	{ }
//};

struct TEST_STRUCT
{
	CD_DEFINE(double, TempDouble);
	CD_DEFINE(unsigned long long, TempUll)
	TEST_STRUCT(double startd, unsigned long long startull)
		: CD_INIT(TempDouble, startd)
		, CD_INIT(TempUll, startull)
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


	auto Ts = new TEST_STRUCT(1.2, 1776);
	printf("Temp:    %.3f\n", Ts->GetTempDouble());
	printf("crypted: %.3f\n", Ts->__Cd_np_TempDouble);
	Ts->SetTempDouble(1.52312);
	printf("Temp:    %.3f\n", Ts->GetTempDouble());
	printf("crypted: %.3f\n\n", Ts->__Cd_np_TempDouble);

	printf("Temp:    %llu\n", Ts->GetTempUll());
	printf("crypted: %llu\n", Ts->__Cd_np_TempUll);
	Ts->SetTempUll(1992);
	printf("Temp:    %llu\n", Ts->GetTempUll());
	printf("crypted: %llu\n", Ts->__Cd_np_TempUll);

}
