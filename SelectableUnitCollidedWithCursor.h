#pragma once

#include "GlobalVars.h"

/*
  v3 = *(HeroSkeleton + 528); //lmp_model

  if ( !v3 || !*v3 )
  {
	*reserved = 0;
	return reserved;
  }
*/

char __fastcall SelectableUnitCollidedWithCursor(NetworkVar_m_skeletonInstance* HeroSkeleton, char* reserved, char* SomeText)
{
    if (HeroSkeleton)
	if (UnderCursor && fuckingMatrix && fuckingMatrixValid)
	{
		undercursor_screen.x = 0;
		undercursor_screen.y = 0;

		long long temp = fuckingMatrix + 0x288;

		WorldToScreen(*(D3DVECTOR*)HeroSkeleton->GetPos(), &undercursor_screen, (float*)temp, view.Width, view.Height);

	
		memset(undercursor_buff, 0, 256);
        auto ent = (CBaseEntity*)HeroSkeleton->GetHeroBySkeleton();
        sprintf(undercursor_buff, "%d", ent->GetCurrentHealth());
        undercursor_time = GetTickCount();
		
		undercursor_screen_point.x = undercursor_screen.x - 100;
		undercursor_screen_point.y = undercursor_screen.y - 50;
	}
	return SelectableUnitCollidedWithCursor_Original(HeroSkeleton, reserved, SomeText);
}




//function at moment 5 september 2021
 /*
 char __fastcall sub_7FFBF2751950(__int64 C_DOTA_Unit_Hero, const char *a2, float *Coordinate)
{
  float *v3; // rbp
  const char *v4; // rdi
  __int64 v5; // rax
  __int64 v6; // rsi
  _QWORD *v7; // rax
  unsigned int v8; // ebx
  __int64 v9; // r9
  unsigned int (__fastcall ***v10)(_QWORD, _QWORD, char *); // rcx
  unsigned __int8 v11; // dl
  __int64 *v12; // rcx
  unsigned __int8 v14; // [rsp+40h] [rbp+8h]
  unsigned __int8 v15; // [rsp+58h] [rbp+20h]

  v3 = Coordinate;
  v4 = a2;
  v5 = (*(**(C_DOTA_Unit_Hero + 784) + 64i64))();
  v6 = v5;
  v7 = *(v5 + 512);
  if ( v7 && *v7 )
  {
    v8 = 0;
    if ( v4 )
    {
      if ( *v4 )
      {
        v8 = MurmurHash2LowerCase(v4, 0x31415926u);
        if ( g_bUpdateStringTokenDatabase )
        {
          LOBYTE(v9) = 1;
          RegisterStringToken(v8, v4, 0i64, v9);
        }
      }
    }
    v10 = *(v6 + 952);
    if ( !v10 || (v15 = 0, (**v10)(v10, v8, &v15)) )
    {
      v12 = *(v6 + 512);
      if ( v12 )
      {
        sub_7FFBF3C062A0(*v12, &v14, v8);
        v11 = v14;
      }
      else
      {
        v11 = 0;
        v14 = 0;
      }
    }
    else
    {
      v11 = v15;
      v14 = v15;
    }
  }
  else
  {
    v11 = 0;
  }
  return sub_7FFBF2817B50(v6, v11, v3);
}


    */
//==================================================================================================================================
//function at moment 27 september 2021 
 /*
 _BYTE *__fastcall sub_7FFF554CDB30(__int64 HeroSkeleton, _BYTE *Reserved, const char *SomeText)
 {
   _QWORD *v3; // rax
   const char *v4; // rdi
   _BYTE *v5; // r14
   __int64 v6; // rbp
   unsigned int v7; // esi
   signed __int64 v8; // rbx
   int v9; // eax
   __int64 *v10; // r8
   unsigned int v11; // edx
   unsigned __int64 v12; // r9
   unsigned int v13; // eax
   int v14; // ebx
   int v15; // ebx
   unsigned int v16; // ebx
   __int64 v17; // r9
   unsigned int (__fastcall ***v18)(_QWORD, _QWORD, char *); // rcx
   _BYTE *result; // rax
   _QWORD *v20; // rcx
   int v21; // [rsp+30h] [rbp-A8h]
   int v22; // [rsp+34h] [rbp-A4h]
   __int64 *v23; // [rsp+38h] [rbp-A0h]
   char v24; // [rsp+E0h] [rbp+8h]
   char v25; // [rsp+F8h] [rbp+20h]

   v3 = *(HeroSkeleton + 528);
   v4 = SomeText;
   v5 = Reserved;
   v6 = HeroSkeleton;
   if ( !v3 || !*v3 )
   {
     *Reserved = 0;
     return Reserved;
   }
   v7 = 0;
   if ( SomeText && *SomeText )
   {
     v8 = -1i64;
     do
       ++v8;
     while ( SomeText[v8] );
     v23 = 0i64;
     v22 = -1073741696;
     v21 = 0;
     CBufferString::Insert(&v21, 0, SomeText, v8, 0);
     CBufferString::ToLowerFast(&v21, 0);
     v9 = v22;
     if ( _bittest(&v9, 0x1Eu) )
     {
       v10 = &v23;
     }
     else
     {
       v10 = &byte_7FFF56BB59AA;
       if ( v22 & 0x3FFFFFFF )
         v10 = v23;
     }
     v11 = v8 ^ 0x31415926;
     if ( v8 >= 4 )
     {
       v12 = v8 >> 2;
       LODWORD(v8) = v8 - 4 * (v8 >> 2);
       do
       {
         v13 = 1540483477 * *v10;
         v10 = (v10 + 4);
         v11 = 1540483477 * (v13 ^ (v13 >> 24)) ^ 1540483477 * v11;
         --v12;
       }
       while ( v12 );
     }
     v14 = v8 - 1;
     if ( v14 )
     {
       v15 = v14 - 1;
       if ( v15 )
       {
         if ( v15 != 1 )
           goto LABEL_20;
         v11 ^= *(v10 + 2) << 16;
       }
       v11 ^= *(v10 + 1) << 8;
     }
     v11 = 1540483477 * (v11 ^ *v10);
 LABEL_20:
     v16 = 1540483477 * (v11 ^ (v11 >> 13));
     CBufferString::Purge(&v21, 0);
     v7 = v16 ^ (v16 >> 15);
     if ( g_bUpdateStringTokenDatabase )
     {
       LOBYTE(v17) = 1;
       RegisterStringToken(v7, v4, 0i64, v17);
     }
   }
   v18 = *(v6 + 968);
   if ( !v18 || (v24 = 0, (**v18)(v18, v7, &v24)) )
   {
     v20 = *(v6 + 528);
     if ( v20 )
     {
       sub_7FFF568E6CA0(*v20, &v25, v7);
       *v5 = v25;
     }
     else
     {
       *v5 = 0;
     }
     result = v5;
   }
   else
   {
     *v5 = v24;
     result = v5;
   }
   return result;
 }
 */