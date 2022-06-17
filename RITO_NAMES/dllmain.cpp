#include "Offsets.h"
#include <iostream>
#include <fstream>

#define LODWORD(x)  (*((DWORD*)&(x)))  

FnamePool* namepool;
std::ofstream file{ "Names_Dump.txt" };
// auto  sub_7FF657D53350 = (__int64(*)())0;
unsigned int rito_key = 0;
bool b_key_found = false;
__int64 __fastcall decrypt_name(FnameEntry* fnameentry, char* buffer, int reallen)
{
	char* v2; __int64 result; unsigned int v5; __int64 v6; __int64 v7; int v8;
brutforcekey:
	v2 = buffer;
	memcpy(buffer, (const void*)(&fnameentry->first_char_encrypted), reallen);
	auto v3 = fnameentry->info;
	//result = sub_7FF657D53350(); // 0x2CD3350
	result = rito_key;
	v5 = (unsigned int)v3 >> 1;
	v8 = result;
	if (v5)
	{
		v6 = 0i64;
		v7 = v5;
		do
		{
			++v2;
			result = v6++ & 3;
			*(v2 - 1) ^= v5 ^ *((BYTE*)&v8 + result);
			--v7;
		} while (v7);
	}
	if (!b_key_found)
	{
		if (!strncmp(buffer, "None", 4))
		{
			b_key_found = true;
		}
		else
		{
			rito_key++;
			goto brutforcekey;
		}
	}
	return result;
}


void dumpblock(int numchunk)
{
	auto blockchunk = namepool->chunks[numchunk];
	FnameEntry* currentfname = (FnameEntry*)(((unsigned __int64)blockchunk));
	while (true)
	{
		auto len = currentfname->info >> 1; // for char
		bool iswide = currentfname->info & 1;
		if (iswide)
			len = currentfname->info & 0xFFFE;

		if (len == 0) break;

	static	char tempbuf[0x2000];
		ZeroMemory(tempbuf, 0x2000);
		decrypt_name(currentfname, tempbuf, len);
		auto firstchar = (currentfname);
		file << "[" << currentfname->index << "] " << tempbuf << "\n";
		auto ost = ((len + 2) % 4);
		if (ost)
			ost = 1;
		auto next = (((unsigned __int64)(len + 2) / 4) * 4) + static_cast<unsigned long long>(ost * 4);
		auto pnext = ((unsigned __int64)&currentfname->info) + next;
		currentfname = (FnameEntry*)pnext;
		auto deltachunk = (pnext - (unsigned __int64)blockchunk);
		auto maxid = (unsigned __int64)((numchunk + 1) * 0x10000);

		if (currentfname->index >= maxid)
			break;
		if (deltachunk > (static_cast<unsigned long long>(0x40000) - 6 - static_cast<unsigned long long>(next))) //chunk size
			break;
	}
	return;
};


void	dumpNames()
{
	file << "Name Count: " << namepool->namecount << "\n";
	for (size_t i = 0; i < static_cast<unsigned long long>(namepool->chunkcount); i++)
		dumpblock(i);
	file.close();
};



HRESULT onAttach(HMODULE hModule)
{
	auto moduleBase = *(unsigned long long*)(__readgsqword(0x60) + 0x10);
	//sub_7FF657D53350 = (__int64(*)())(moduleBase + 0x2CD3350); get key
	namepool = (FnamePool*)(moduleBase + Offsets::NamePool);
	dumpNames();

	MessageBoxA(nullptr, "Done", "v", MB_OK);

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(
			CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(onAttach), hModule, 0, nullptr)
		);
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
