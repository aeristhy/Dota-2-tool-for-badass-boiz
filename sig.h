#pragma once
/// <summary>
/// god, i swear, i'll deal with this zombie code sometime :CCC
/// </summary>
namespace d //database
{
#define ArrayCapacity 50
	enum n { //name
		VBE,
		Entity
	};

	const char* Module	[ArrayCapacity];
	char		Offset	[ArrayCapacity];
	int			Vtable	[ArrayCapacity];
	const char* Mask	[ArrayCapacity];
	char		MaskSz	[ArrayCapacity];

	void Init()
	{
#pragma region Visible by enemy 
		/*===============================================================================================================
			client.dll.text+B7C5C0 - 48 89 5C 24 08        - mov [rsp+08],rbx
			client.dll.text+B7C5C5 - 57                    - push rdi
			client.dll.text+B7C5C6 - 48 83 EC 20           - sub rsp,20
			client.dll.text+B7C5CA - 48 8B 05 AF33B302     - mov rax,[client.dll+36B0980]
			client.dll.text+B7C5D1 - 8B FA                 - mov edi,edx -- mask starts here
			client.dll.text+B7C5D3 - 48 8B D9              - mov rbx,rcx
			client.dll.text+B7C5D6 - 48 85 C0              - test rax,rax
			client.dll.text+B7C5D9 - 74 0F                 - je client.dll.text+B7C5EA
			client.dll.text+B7C5DB - 8B 40 70              - mov eax,[rax+70]
			client.dll.text+B7C5DE - 83 C0 FA              - add eax,-06
			client.dll.text+B7C5E1 - 83 F8 01              - cmp eax,01
		*/
		Module	[n::VBE]	= "client.dll";
		Offset	[n::VBE]	= 0x11;
		Vtable	[n::VBE]	= 207; //slot
		Mask	[n::VBE]	= "\x8B\xFA\x48\x8B\xD9\x48\x85\xC0\x74\x0F\x8B\x40\x70\x83\xC0\xFA\x83\xF8\x01";
		MaskSz	[n::VBE]	= sizeof("\x8B\xFA\x48\x8B\xD9\x48\x85\xC0\x74\x0F\x8B\x40\x70\x83\xC0\xFA\x83\xF8\x01")-1;
		//===================================================================================================
#pragma endregion CBaseEntity


	}

	
///
}


__inline bool CheckSeq(char* bytes, int offset, const char* mask, int mask_sz)
{
    bool ok = false;
    for (int iterator = 0; iterator < mask_sz; iterator++)
    {

        if (bytes[iterator + offset] == (char)mask[iterator])
        {
            ok = true;
            iterator++;
        }
        else
        {
            ok = false;
            break;
        }
    }
    if (ok)
    {
#ifdef seqReport
        printf("\nSeq:\n");
        char q = 0;
        for (int i = 0; i < mask_sz; i++)
        {
            printf("%c ", mask[i]);
            q++;
            if (q > 7)
            {
                printf("\n");
                q = 0;
            }
        }
        printf("\nFound at %lld", bytes);
#endif
        return 1;
    }
    return 0;
}

bool CheckSigInVtable(unsigned __int64** memory, int methods, d::n var, int* ret)
{
    printf("\nmethods: %d object: %d seqSz: %d", methods, var, d::MaskSz[var]);
    unsigned __int64* vtable = memory[0];
    /*while (validate(vtable[method_count]))
        method_count++; //мега у€звима€ конструкци€, нужно что-то мен€ть
    method_count++;
    printf("\nmethods:\t%d", method_count);
    */
    char* bytes = (char*)vtable[d::Vtable[var]];

    if (CheckSeq(bytes, d::Offset[var], d::Mask[var], d::MaskSz[var]))
    {
        *ret = d::Vtable[var];
        return 1;
    }
    printf("\nMask not found at supposed position. Searching...");

    for (int i = 0; i < methods; i++)
    {
        char* bytes = (char*)vtable[i];

        if (CheckSeq(bytes, d::Offset[var], d::Mask[var], d::MaskSz[var]))
        {
            printf("\nMask found in method number %d\nSupposed to be in %d", i, d::Vtable[var]);
            *ret = i;
            return 1;
        }

    }
    printf("\nwe lost it.");
    return 0;

}