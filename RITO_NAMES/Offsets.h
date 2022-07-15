#pragma once
#include <Windows.h>

namespace Offsets
{
	/*5.01*/ constexpr auto	NamePool = 0x867dc40; // 5.01
	constexpr auto  GObjects = 0x0;
	constexpr auto  GObjectskey = GObjects + 0x38;

}



struct FnamePool
{
	__int64 gap;
	unsigned int chunkcount;
	unsigned int namecount;
	__int64* chunks[];
};
struct FnameEntry
{
	unsigned int index;
	unsigned __int16 info; 
	char first_char_encrypted;
};

struct FName
{
	unsigned int Index;
	unsigned int Number;
};


class UObject
{
public:
	void* Vtable;
	unsigned int Flags;
	unsigned int ObjectID;
	class UObject* Class;
	FName name;
	char gap[0x8];
	class UObject* Outer;
	const char* GetName()
	{


	};
};



