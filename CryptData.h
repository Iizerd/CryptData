#pragma once

#include <array>

#define __CD_U8 unsigned char
#define __CD_U16 unsigned short
#define __CD_U32 unsigned int
#define __CD_U64 unsigned long long

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

template<__CD_U8> struct __CD_DATA_STORAGE;
template<> struct __CD_DATA_STORAGE<1> { using Type = __CD_U8; };
template<> struct __CD_DATA_STORAGE<2> { using Type = __CD_U16; };
template<> struct __CD_DATA_STORAGE<4> { using Type = __CD_U32; };
template<> struct __CD_DATA_STORAGE<8> { using Type = __CD_U64; };

// Every type that can be coerced into an 8 byte value and is returned in gpr
template<__CD_U64 Hash_value, typename> struct __CD_CRYPT_SHELLCODE
{
	constexpr static std::array<__CD_U8, 37> Data =
	{
		0x0F, 0x31,												/*rdtsc*/
		0x48, 0x01, 0x05, 0x0B, 0x00, 0x00, 0x00,				/*add [hash],rax*/
		0x66, 0xC7, 0x05, 0xEE, 0xFF, 0xFF, 0xFF, 0xEB, 0x10,	/*mov start, disp to past setup*/
		0x48, 0xB8, 											/*movabs rax,hash*/
		((0xFFull << 0x0) & Hash_value >> 0x0),					/*hash*/
		(((0xFFull << 0x8) & Hash_value) >> 0x8),
		(((0xFFull << 0x10) & Hash_value) >> 0x10),
		(((0xFFull << 0x18) & Hash_value) >> 0x18),
		(((0xFFull << 0x20) & Hash_value) >> 0x20),
		(((0xFFull << 0x28) & Hash_value) >> 0x28),
		(((0xFFull << 0x30) & Hash_value) >> 0x30),
		(((0xFFull << 0x38) & Hash_value) >> 0x38),
		0x48, 0x31, 0xC8, 										/*xor rax,rcx*/
		0xC3,
		0x90, 0x90, 0x90, 0x90, 0x90							/*padding so same size*/
	};
};
// Float specialization
template<__CD_U64 Hash_value> struct __CD_CRYPT_SHELLCODE<Hash_value, float>
{
	constexpr static std::array<__CD_U8, 37> Data = {
			0x0F, 0x31,												/*rdtsc*/
			0x48, 0x01, 0x05, 0x0B, 0x00, 0x00, 0x00,				/*add [hash],rax*/
			0x66, 0xC7, 0x05, 0xEE, 0xFF, 0xFF, 0xFF, 0xEB, 0x10,	/*mov start, disp to past setup*/
			0x48, 0xB8, 											/*movabs rax,hash*/
			((0xFFull << 0x0) & Hash_value >> 0x0),					/*hash*/
			(((0xFFull << 0x8) & Hash_value) >> 0x8),
			(((0xFFull << 0x10) & Hash_value) >> 0x10),
			(((0xFFull << 0x18) & Hash_value) >> 0x18),
			(((0xFFull << 0x20) & Hash_value) >> 0x20),
			(((0xFFull << 0x28) & Hash_value) >> 0x28),
			(((0xFFull << 0x30) & Hash_value) >> 0x30),
			(((0xFFull << 0x38) & Hash_value) >> 0x38),
			0x48, 0x31, 0xC8, 										/*xor rax,rcx*/
			0x66, 0x48, 0x0f, 0x6e, 0xc0,							/*movq xmm0,rax*/
			0xC3,
	};
};
// Double specialization
template<__CD_U64 Hash_value> struct __CD_CRYPT_SHELLCODE<Hash_value, double>
{
	constexpr static std::array<__CD_U8, 37> Data = __CD_CRYPT_SHELLCODE<Hash_value, float>::Data;
};

#define __CDDS(__Type) __CD_DATA_STORAGE<sizeof(__Type)>::Type

#define __COERCE_TYPE(__Val) (*(typename __CD_DATA_STORAGE<sizeof(decltype(__Val))>::Type*)&__Val)

#define CD_DEFINE(__Type, __Name)																															\
static_assert(sizeof(__Type) <= 8, "Invalid type size.");																									\
__Type __Cd_np_##__Name;																																	\
__declspec(allocate(".cdata")) constexpr static auto __Cd_np_crypt_##__Name = __CD_CRYPT_SHELLCODE<__CD_Hash64<#__Name>(1776), __Type>::Data;				\
																																							\
__forceinline const __Type Get##__Name() const																												\
{																																							\
	return ((__Type(*)(__CDDS(__Type)))(__Cd_np_crypt_##__Name.data()))(__COERCE_TYPE(__Cd_np_##__Name));													\
}																																							\
__forceinline void Set##__Name(__Type __Val)																												\
{																																							\
	__Cd_np_##__Name = ((__Type(*)(__CDDS(__Type)))(__Cd_np_crypt_##__Name.data()))(__COERCE_TYPE(__Val));													\
}

#define CD_INIT(__Name, __Val)																													\
	__Cd_np_##__Name(((decltype(__Cd_np_##__Name)(*)(__CDDS(decltype(__Cd_np_##__Name))))(&__Cd_np_crypt_##__Name))(__COERCE_TYPE(__Val)))

