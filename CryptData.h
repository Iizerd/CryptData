#pragma once

#define __CD_U8 unsigned char
#define __CD_U16 unsigned short
#define __CD_U32 unsigned int
#define __CD_U64 unsigned long long


#define __EXPAND(__X) __X

#define __CD_NAME_PREFIX __Cd_name_prefix_

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

template<__CD_STRING_LITERAL DataName>
consteval __CD_U32 __CD_Hash32(const __CD_U32 Seed)
{
	__CD_U32 Value = Seed;
	for (__CD_U32 i = 0; i < DataName.Size; ++i)
		Value = static_cast<__CD_U32>(0x1000193 * (Value ^ DataName.Data[i]));
	return Value;
}

template<__CD_STRING_LITERAL DataName>
consteval __CD_U64 __CD_Hash64(const __CD_U32 Seed)
{
	return (((__CD_U64)__CD_Hash32<DataName>(Seed)) | (((__CD_U64)__CD_Hash32<DataName>(Seed * 2)) << 32));
}

enum class __DATA_SIZE { b8, b16, b32, b64 };

template<__CD_U8> struct __CD_DATA_STORAGE;
template<> struct __CD_DATA_STORAGE<1> { using Type = __CD_U8;  };
template<> struct __CD_DATA_STORAGE<2> { using Type = __CD_U16; };
template<> struct __CD_DATA_STORAGE<4> { using Type = __CD_U32; };
template<> struct __CD_DATA_STORAGE<8> { using Type = __CD_U64; };


template<typename DataType, __CD_U8 DataSize, __CD_U64 CryptHash>
__forceinline DataType __CD_Decrypt(DataType Data)
{
	typename __CD_DATA_STORAGE<DataSize>::Type DataStorage = *(typename __CD_DATA_STORAGE<DataSize>::Type*)&Data;
	DataStorage -= (typename __CD_DATA_STORAGE<DataSize>::Type)CryptHash;
	DataStorage ^= CryptHash;
	return *(DataType*)&DataStorage;
}

template<typename DataType, __CD_U8 DataSize, __CD_U64 CryptHash>
__forceinline DataType __CD_Encrypt(DataType Data)
{
	typename __CD_DATA_STORAGE<DataSize>::Type DataStorage = *(typename __CD_DATA_STORAGE<DataSize>::Type*)&Data;
	DataStorage ^= CryptHash;
	DataStorage += (typename __CD_DATA_STORAGE<DataSize>::Type)CryptHash;
	return *(DataType*)&DataStorage;
}


// Define the data
#define CD_DEFINE(__Type, __Name)																									\
static_assert(sizeof(__Type) <= 8, "Invalid type size.\n");																			\
__Type __EXPAND(__CD_NAME_PREFIX)##__Name;																							\
__forceinline const __Type Get##__Name()																							\
{																																	\
	return __CD_Decrypt<__Type, sizeof(__Type), __CD_Hash64<#__Name>(1776)>(__EXPAND(__CD_NAME_PREFIX)##__Name);					\
}																																	\
__forceinline void Set##__Name(__Type const __Val)																					\
{																																	\
	__EXPAND(__CD_NAME_PREFIX)##__Name = __CD_Encrypt<__Type, sizeof(__Type), __CD_Hash64<#__Name>(1776)>(__Val);					\
}

#define CD_MAKE_INITIALIZER(__Name, __Val)																							\
__EXPAND(__CD_NAME_PREFIX)##__Name(__CD_Encrypt<decltype(__EXPAND(__CD_NAME_PREFIX)##__Name), sizeof(decltype(__EXPAND(__CD_NAME_PREFIX)##__Name)), __CD_Hash64<#__Name>(1776)>(__Val))





