#include <Windows.h>
//#include <memory>
//#include <iostream>

using namespace std;

#define asm_ret 0xC3
#define asm_call 0xE8
#define asm_jump 0xe9
#define asm_add	 0x83
#define asm_pushad 0x60
#define asm_pushfd 0x9C
#define asm_popfd  0x9D
#define asm_popad  0x61
#define asm_push_eax 0x50
#define asm_push_esp 0x54
#define asm_pop_eax 0x58
#define asm_pop_esp 0x5C
#define asm_mov_eax_from_ebp_ptr 0x458B //прибавить к указателю


//#define LOGGIRUEM

#define hooks_max 50
#define bytes_max 30


enum r {
	rax = 0,
	rcx,
	rdx,
	rbx,
	rsp,
	rbp,
	rsi,
	rdi
};

class connector {

	bool	boolka;
	bool	WndProcHooked;
	char*	cave;
	DWORD	cave_actual_offset = 0;
	char	original_code[hooks_max][bytes_max];
	char	original_code_sz[hooks_max];
	char*	original_code_address[hooks_max];

	//WndProc original
	__int64 oWndProc;
	
	HWND hwnd;

	//VP
	DWORD op;

	//stack?
	int slot = 0;

	template <typename... Args>
	void __inline log(const char* txt, Args... a)
	{
#ifdef LOGGIRUEM
		printf(txt, a...);
#endif
		return;
	}

	bool setVP(char* address, int sz, DWORD np = PAGE_EXECUTE_READWRITE)
	{
		if (!VirtualProtect(address, sz, np, &op))
		{
			log("\n[%s]!VirtualProtect", __func__);
			return false;
		}
		else
#ifdef	__its32
			log("\nVP set %X", np);
#elif	__its64 
			log("\nVP set %llx", np);
#endif
		return true;
	}

	void backVP(char* address, int sz)
	{
		DWORD trashbox;
		if (!VirtualProtect(address, sz, op, &trashbox))
		{
			log("\n[%s]!VirtualProtect", __func__);
		}
		else
#ifdef	__its32
			log("\nVP returned %X", op);
#elif	__its64
			log("\nVP returned %llx", op);
#endif
	}


	bool saveOriginalCode(char* address, int sz)
	{
		if (!setVP(address, sz))
			return 0;

		if (sz > bytes_max)
			return 0;

		memcpy(original_code[slot], address, sz);

		backVP(address, sz);
		return 1;
	}

	void makeJump(char* to, char* from)
	{
		

#ifdef __its32
		int jumpaddr = 0;
		jumpaddr = to - from - 5;

		//to - from - 5
		log("\nset [E9] jump from %X to %X ...", from, to);

		if (!memset(from, asm_jump, 1) || !memcpy(from + 1, (int*)&jumpaddr, 4))
			log("[%s] memset returned zero\t ??wtf???", __func__);
		else
			log(" ok");
#elif  __its64

		
		*from		= 0xFF;
		*(from + 1)	= 0x25;
		*(from + 2) = 0;
		*(from + 3) = 0;
		*(from + 4) = 0;
		*(from + 5) = 0;
		memcpy(from + 6, &to, 8);

		log("\nset [E9] jump from %llx to %llx ...", from, to);
#endif

	}

	void makeCall(char* to, char* from)
	{
		int jumpaddr = 0;
		//if ((int)from < (int)to)
			//to - from - 5
		jumpaddr = to - from - 5;
#ifdef __its32
		log("\nset [E8] call from %X to %X ...", from, to);
#elif  __its64
		log("\nset [E8] call from %llx to %llx ...", from, to);
#endif
		//to - from - 5
		if (!memset(from, asm_call, 1) || !memcpy(from + 1, (int*)&jumpaddr, 4))//записываем джамп-инструкцию
			log(" memset returned zero\t ??wtf???");
		else
			log(" ok");

	}

	void makeCall_s(char* function)
	{
		makeCall(function, cave + cave_actual_offset);
		cave_actual_offset += 5;
	}
	void makeJump_s()
	{
		makeJump(original_code_address[slot] + 5, cave + cave_actual_offset);
		cave_actual_offset += 5;
	}

	void setOriginalCode()
	{
		memcpy(cave + cave_actual_offset, original_code[slot], original_code_sz[slot]);
		cave_actual_offset += original_code_sz[slot];
	}

	void GetSlot()
	{
		for (int i = 0; i < hooks_max; i++)
			if (!original_code_address[i])
			{
				slot = i;
				boolka = true;
				log("\n[%s]took slot %d", __func__, i);
				break;
			}
			else
				boolka = false;
	}

	void SetStealerCode(char* address, int sz, char* container, r RegToSteal)
	{
		if (RegToSteal == r::rax)
		{
			memset(cave + cave_actual_offset, 0x51, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x48, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0xB9, 1);
			cave_actual_offset += 1;
			memcpy(cave + cave_actual_offset, (__int64*)&container, 8);
			cave_actual_offset += 8;
			memset(cave + cave_actual_offset, 0x48, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x89, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 1, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x59, 1);
			cave_actual_offset += 1;

		}
		else {
			memset(cave + cave_actual_offset, 0x50, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x48, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0xB8, 1);
			cave_actual_offset += 1;
			memcpy(cave + cave_actual_offset, (__int64*)&container, 8);
			cave_actual_offset += 8;
			memset(cave + cave_actual_offset, 0x48, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x89, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, RegToSteal * 8, 1);
			cave_actual_offset += 1;
			memset(cave + cave_actual_offset, 0x58, 1);
			cave_actual_offset += 1;
		}
	}

public:
	connector()
	{	
		cave = (char*)VirtualAlloc(0, 0xFFF, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!cave)
		{
			log("\n!VirtualAlloc\\AllocNeedToBeForced");
			return;
		}

		log("\nnew memory allocated at %X", cave);
		{
			DWORD trash = 0;
			//log(VirtualProtect(from, sz, PAGE_EXECUTE_READWRITE, &trash) ? "\n--VP succn\nprev: %X new %X--" : "\n--VP fail\nprev %X new %X--",);
			boolka = VirtualProtect(cave, 0xFFF, PAGE_EXECUTE_READWRITE, &trash);
			if (boolka)
				log("\n--VP succ\\prev %X new %X--", cave);
			else
			{
				"\n--VirtualProtect fail\\eto ochen ploxo";
				return;
			}
			log("\nsupposed to be ready for inject");
		}

	}

	void BreakTrampoline(int i)
	{
		setVP(original_code_address[i], original_code_sz[i]);
		memcpy(original_code_address[i], original_code[i], original_code_sz[i]);
		backVP(original_code_address[i], original_code_sz[i]);
		original_code_address[i] = 0;
	}
#ifdef __its32
	int set_hook(char* address, int sz, char* callback_function,char** o)
	{

		for (int i = 0; i < hooks_max; i++)
			if (!original_code_address[i])
			{
				slot = i;
				boolka = true;
				log("\n[%s]took slot %d", __func__, i);
				break;
			}
			else
				boolka = false;



		if (!boolka)
		{
			log("\n!boolka");
			return 0;
		}
		if (!saveOriginalCode(address, sz))
		{
			log("\n!saveOriginalCode()");
			return 0;
		}
		original_code_address[slot] = address;
		original_code_sz[slot] = sz;
		
		makeJump(address + sz, cave + cave_actual_offset);
		//Если код засейвили, то всё ок, и пора сохранить его размер
		setVP(address, sz);
		for (int i = 0; i < sz; i++)
			memset(address + i, 0x90, 1);
		makeJump(cave + cave_actual_offset, address);
		backVP(address, sz);

		//setOriginalCode();

		//setPrologue();

		makeJump(callback_function, cave + cave_actual_offset);
		cave_actual_offset += 5;
		//makeCall_s(callback_function);
		/*int* retAddr = findRetAddr(callback_function);
		setVP((char*)retAddr, 4);
		setRetAddr(callback_function);
		backVP((char*)retAddr, 4);*/
		*o = cave + cave_actual_offset;
		setOriginalCode();
		makeJump(address + sz, cave + cave_actual_offset);


		//setEpilogue();

		

		//makeJump_s();

		/*for (int i = 0; i < sz - 5; i++)
			memset(address+i, 0x90, 1);*/

		log("\n------------done-----------------");



		return slot;
	}

	
#endif
	
#ifdef __its64

	__int64 set_WndProc_hook(HWND _hwnd, __int64 WndProc)
	{
		hwnd = _hwnd;
		WndProcHooked = 1;
		oWndProc = SetWindowLongPtr(_hwnd, GWLP_WNDPROC, WndProc);
		return oWndProc;
	}

	int set_hook(char* address, int sz, char* callback_function, char** o)
	{

		GetSlot();

		if (!boolka)
		{
			log("\n!boolka");
			return 0;
		}
		if (!saveOriginalCode(address, sz))
		{
			log("\n!saveOriginalCode()");
			return 0;
		}
		original_code_address[slot] = address;
		original_code_sz[slot] = sz;

		makeJump(address + sz, cave + cave_actual_offset);
		//Если код засейвили, то всё ок, и пора сохранить его размер
		setVP(address, sz);
		for (int i = 0; i < sz; i++)
			memset(address + i, 0x90, 1);
		makeJump(cave + cave_actual_offset, address);
		backVP(address, sz);

		//setOriginalCode();

		//setPrologue();

		makeJump(callback_function, cave + cave_actual_offset);
		cave_actual_offset += 14;
		//makeCall_s(callback_function);
		/*int* retAddr = findRetAddr(callback_function);
		setVP((char*)retAddr, 4);
		setRetAddr(callback_function);
		backVP((char*)retAddr, 4);*/
		*o = cave + cave_actual_offset;
		setOriginalCode();
		makeJump(address + sz, cave + cave_actual_offset);
		cave_actual_offset += 14;

		//setEpilogue();



		//makeJump_s();

		/*for (int i = 0; i < sz - 5; i++)
			memset(address+i, 0x90, 1);*/

		log("\n------------done-----------------");



		return slot;
	}


	int set_reg_stealer(char* address, int sz, char* container, r RegToSteal)
	{
		/*
F7F6EC0000 - 50						- push rax
F7F6EC0002 - 48 B8 5000ECF6F7000000 - mov rax,000000F7F6EC0050
F7F6EC0016 - 48 89 08				- mov [rax],rcx
F7F6EC0019 - 58						- pop rax
F7F6EC001B - EB E3					- jmp F7F6EC0000
		*/
		setVP(address, sz);
		GetSlot();

		if (!boolka)
		{
			log("\n!boolka");
			return 0;
		}
		if (!saveOriginalCode(address, sz))
		{
			log("\n!saveOriginalCode()");
			return 0;
		}
		original_code_address[slot] = address;
		original_code_sz[slot] = sz;
		memset(address, 0x90, sz);
		makeJump(cave + cave_actual_offset, address);

		SetStealerCode(address, sz, container, RegToSteal);
		
		memcpy(cave + cave_actual_offset, original_code[slot], sz);
		cave_actual_offset += sz;
		makeJump(address+14+(sz-14), cave + cave_actual_offset);
		cave_actual_offset += 14;
		backVP(address, sz);
		return slot;
	}

	int set_reg_stealer_reverse(char* address, int sz, char* container, r RegToSteal)
	{
		/*
F7F6EC0000 - 50						- push rax
F7F6EC0002 - 48 B8 5000ECF6F7000000 - mov rax,000000F7F6EC0050
F7F6EC0016 - 48 89 08				- mov [rax],rcx
F7F6EC0019 - 58						- pop rax
F7F6EC001B - EB E3					- jmp F7F6EC0000
		*/
		setVP(address, sz);
		GetSlot();

		if (!boolka)
		{
			log("\n!boolka");
			return 0;
		}
		if (!saveOriginalCode(address, sz))
		{
			log("\n!saveOriginalCode()");
			return 0;
		}
		original_code_address[slot] = address;
		original_code_sz[slot] = sz;
		memset(address, 0x90, sz);
		makeJump(cave + cave_actual_offset, address);

		memcpy(cave + cave_actual_offset, original_code[slot], sz);
		cave_actual_offset += sz;
		
		SetStealerCode(address, sz, container, RegToSteal);
		
		makeJump(address + 14 + (sz - 14), cave + cave_actual_offset);
		cave_actual_offset += 14;
		backVP(address, sz);
	}

#endif
	~connector()
	{
#ifdef __its64
		if (WndProcHooked)
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, oWndProc);
#endif
		log("\nremoving hooks... ");
		for (int i = 0; i < hooks_max; i++)
			if (original_code_address[i] != 0)
			{
				log("\n%X %d", original_code_address[i], original_code_sz[i]);
				setVP(original_code_address[i], original_code_sz[i]);
				memcpy(original_code_address[i], original_code[i], original_code_sz[i]);
				backVP(original_code_address[i], original_code_sz[i]);
			}
		log("\nreleasing memory");
		VirtualFree(cave, 0, 0x00008000);
		log("\n\nSee you around!");
	}

	//int makeConnector(char* from, int sz, char* memory_card = 0)
	//{
	//	unsigned char iterator = 0;
	//	{
	//		std::cout << (int*)from;

	//		if (sz < 5)
	//		{
	//			MessageBoxA(0, "Are you retarded?\nHookin' canceled", "", MB_OK);
	//			return 0;
	//		}
	//		if (sz > 9)
	//		{
	//			MessageBoxA(0, "max instruction lenght > 9\nhookin' aborted", "", MB_OK);
	//			return 0;
	//		}
	//		
	//		for (int i = 1; i <= 10; i++)
	//		{
	//			if (!connector[i])
	//			{
	//				iterator = i;
	//				connector[i] = memory_card;
	//				std::cout << "\ntook slot " << (int*)iterator;
	//				break;

	//			}
	//		}
	//		memcpy(original_code[iterator], from, sz);
	//		for (int i = sz - 5; i < sz; i++)
	//			memset(from + i, 0x90, 1);
	//		std::cout << "\nsaving original from " << (int*)from << " size " << std::hex << sz;
	//		original_code_sz[iterator] = sz;
	//		jump_list[iterator] = from;

	//	}

	//	makeJump(connector[iterator], from);
	//	memcpy(connector[iterator], original_code[iterator], sz); //записываем то что заменили на хук в оригинале
	//	//makeJump(from + 5, connector[iterator] + 1019);
	//	connector_ret_val[iterator] = from + 5;
	//	connector_offset[iterator] = sz;
	//	//makeJump(from+5,connector[iterator] + sz);

	//	return iterator;
	//}
};


/*

rax == hero pointer
*rax == baseCombatCharacter vtable
client.dll.text+1A17BC8 - 44 8B 64 24 3C        - mov r12d,[rsp+3C]
client.dll.text+1A17BCD - 48 8D 15 CC853B01     - lea rdx,[client.dll.rdata+DC61A0]
client.dll.text+1A17BD4 - 44 8B 6C 24 40        - mov r13d,[rsp+40]
client.dll.text+1A17BD9 - 48 8D 8D A0000000     - lea rcx,[rbp+000000A0]
client.dll.text+1A17BE0 - 45 8B CC              - mov r9d,r12d
client.dll.text+1A17BE3 - 45 8B C5              - mov r8d,r13d
client.dll.text+1A17BE6 - E8 853390FE           - call client.dll.text+31AF70
client.dll.text+1A17BEB - 48 8B 85 A4000000     - mov rax,[rbp+000000A4]
client.dll.text+1A17BF2 - 0FBA E0 1E            - bt eax,1E
client.dll.text+1A17BF6 - 73 09                 - jae client.dll.text+1A17C01
client.dll.text+1A17BF8 - 48 8D 95 A8000000     - lea rdx,[rbp+000000A8]
client.dll.text+1A17BFF - EB 10                 - jmp client.dll.text+1A17C11
client.dll.text+1A17C01 - A9 FFFFFF3F           - test eax,3FFFFFFF
client.dll.text+1A17C06 - 49 8B D7              - mov rdx,r15
client.dll.text+1A17C09 - 48 0F47 95 A8000000   - cmova rdx,[rbp+000000A8]
client.dll.text+1A17C11 - 48 8B 8B 88000000     - mov rcx,[rbx+00000088]
client.dll.text+1A17C18 - 41 B8 02000000        - mov r8d,00000002
client.dll.text+1A17C1E - 48 8B 01              - mov rax,[rcx]
client.dll.text+1A17C21 - FF 90 70020000        - call qword ptr [rax+00000270]

*/