#pragma once
#include "Include/d3d9.h"
#include "Include/d3dx9math.h"

typedef HRESULT(APIENTRY* ES)(LPDIRECT3DDEVICE9 pDevice);

typedef D3DXMATRIX*(APIENTRY* qq) (D3DXMATRIX* a, FLOAT b, FLOAT c, FLOAT d, FLOAT e);

typedef void (__thiscall* t0) (__int64* ecx, __int64* ptr, __int32 index);
typedef void (__thiscall* t1) (__int64* ecx, __int64* ptr, __int32 index);
typedef __int32(__thiscall* t2) (__int64* ecx);