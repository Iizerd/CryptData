#pragma once

#define __CD_U8 unsigned char
#define __CD_U16 unsigned short
#define __CD_U32 unsigned int
#define __CD_U64 unsigned long long


#define __EXPAND(__X) __X

#define __CD_NAME_PREFIX __Cd_np_

#pragma section(".cdata") // Specifying execute,read,write here does not work, have to use linker comment
#pragma comment(linker, "/SECTION:.cdata,ERW")


template<typename Incomming_type, __CD_U32 String_size>
struct __CD_STRING_LITERAL
{
	using Char_type = Incomming_type;
	using This_type = __CD_STRING_LITERAL<Char_type, String_size>;

	Char_type Data[String_size - 1];
	constexpr static __CD_U32 Size = String_size - 1;

	consteval __CD_STRING_LITERAL(const Char_type(&Raw)[String_size])
	{
		for (__CD_U32 i = 0; i < String_size - 1; ++i)
			Data[i] = Raw[i];
	}
};

template<__CD_STRING_LITERAL Data_name>
consteval __CD_U32 __CD_Hash32(const __CD_U32 Seed)
{
	__CD_U32 Value = Seed;
	for (__CD_U32 i = 0; i < Data_name.Size; ++i)
		Value = static_cast<__CD_U32>(0x1000193 * (Value ^ Data_name.Data[i]));
	return Value;
}

template<__CD_STRING_LITERAL Data_name>
consteval __CD_U64 __CD_Hash64(const __CD_U32 Seed)
{
	return (((__CD_U64)__CD_Hash32<Data_name>(Seed)) | (((__CD_U64)__CD_Hash32<Data_name>(Seed * 2)) << 32));
}


#define CD_DEFINE(__Type, __Name)																															\
static_assert(sizeof(__Type) <= 8, "Invalid type size.");																									\
__Type __Cd_np_##__Name;																																	\
constexpr static __CD_U64 __Cd_np_##__Name##_seed = __CD_Hash64<#__Name>(1776)/* + __LINE__ + __TIME__[7] + __TIME__[6] + __TIME__[4]*/;					\
__declspec(allocate(".cdata")) inline static __CD_U8 __Cd_np_crypt_##__Name[] = {																			\
		0x0F, 0x31,												/*rdtsc*/																					\
		0x48, 0x01, 0x05, 0x0B, 0x00, 0x00, 0x00,				/*add [hash],rax*/																			\
		0x66, 0xC7, 0x05, 0xEE, 0xFF, 0xFF, 0xFF, 0xEB, 0x10,	/*mov start, disp to past setup*/															\
		0x48, 0xB8, 																																		\
		((0xFFull << 0x0)& __Cd_np_##__Name##_seed),																										\
		(((0xFFull << 0x8)& __Cd_np_##__Name##_seed) >> 0x8), 																								\
		(((0xFFull << 0x10)& __Cd_np_##__Name##_seed) >> 0x10), 																							\
		(((0xFFull << 0x18)& __Cd_np_##__Name##_seed) >> 0x18), 																							\
		(((0xFFull << 0x20)& __Cd_np_##__Name##_seed) >> 0x20), 																							\
		(((0xFFull << 0x28) & __Cd_np_##__Name##_seed) >> 0x28), 																							\
		(((0xFFull << 0x30)& __Cd_np_##__Name##_seed) >> 0x30), 																							\
		(((0xFFull << 0x38)& __Cd_np_##__Name##_seed) >> 0x38), 																							\
		0x48, 0x31, 0xC8, 										/*xor rax,rcx*/																				\
		0xC3																																				\
};																																							\
																																							\
																																							\
__forceinline const __Type Get##__Name() const																												\
{																																							\
	return ((__Type(*)(__Type))(&__Cd_np_crypt_##__Name))(__Cd_np_##__Name);																				\
}																																							\
__forceinline void Set##__Name(__Type const __Val)																											\
{																																							\
	__Cd_np_##__Name = ((__Type(*)(__Type))(&__Cd_np_crypt_##__Name))(__Val);																				\
}

#define CD_MAKE_INITIALIZER(__Name, __Val)																													\
__Cd_np_##__Name(((decltype(__Cd_np_##__Name)(*)(decltype(__Cd_np_##__Name)))(&__Cd_np_crypt_##__Name))(__Val))




//OLD STUFF
/*

template<__CD_U8> struct __CD_DATA_STORAGE;
template<> struct __CD_DATA_STORAGE<1> { using Type = __CD_U8;  };
template<> struct __CD_DATA_STORAGE<2> { using Type = __CD_U16; };
template<> struct __CD_DATA_STORAGE<4> { using Type = __CD_U32; };
template<> struct __CD_DATA_STORAGE<8> { using Type = __CD_U64; };

template<typename Data_type, __CD_U8 Data_size, __CD_U64 Crypt_hash>
__forceinline Data_type __CD_Decrypt(Data_type Data)
{
	typename __CD_DATA_STORAGE<Data_size>::Type DataStorage = *(typename __CD_DATA_STORAGE<Data_size>::Type*)&Data;
	DataStorage -= (typename __CD_DATA_STORAGE<Data_size>::Type)Crypt_hash;
	DataStorage ^= Crypt_hash;
	return *(Data_type*)&DataStorage;
}

template<typename Data_type, __CD_U8 Data_size, __CD_U64 Crypt_hash>
__forceinline Data_type __CD_Encrypt(Data_type Data)
{
	__declspec(allocate(".cdata")) static __CD_U32 test_arr[] = { Crypt_hash & 0xFF };

	test_arr[0] = 0xC3;
	typedef void(*TestCall)();
	TestCall tc = (TestCall)&test_arr;
	tc();

	typename __CD_DATA_STORAGE<Data_size>::Type DataStorage = *(typename __CD_DATA_STORAGE<Data_size>::Type*)&Data;
	DataStorage ^= Crypt_hash;
	DataStorage += (typename __CD_DATA_STORAGE<Data_size>::Type)Crypt_hash;
	return *(Data_type*)&DataStorage;
}

__forceinline const __Type Get##__Name() const																																						\
{																																																	\
	return __CD_Decrypt<__Type, sizeof(__Type), __CD_Hash64<#__Name>(__Cd_np_##__Name##_seed)>(__Cd_np_##__Name);																					\
}																																																	\
__forceinline void Set##__Name(__Type const __Val)																																					\
{																																																	\
	__Cd_np_##__Name = __CD_Encrypt<__Type, sizeof(__Type), __CD_Hash64<#__Name>(__Cd_np_##__Name##_seed)>(__Val);																					\
}

#define CD_MAKE_INITIALIZER(__Name, __Val)																														\
__Cd_np_##__Name(__CD_Encrypt<decltype(__Cd_np_##__Name), sizeof(decltype(__Cd_np_##__Name)), __CD_Hash64<#__Name>(__Cd_np_##__Name##_seed)>(__Val))

*/