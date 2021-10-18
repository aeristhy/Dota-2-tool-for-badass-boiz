#pragma once
#ifdef _DEBUG
void EnterScope(__int64* rcx, const char* request, int a2, const char* panel, bool boolka, int a5)
{
	printf("[%llx][1]%s [2]%d [3]%s [4]%s [5]%d", rcx, request, a2, panel, (boolka ? "true" : "false"), a5);
	EnterScope_orig(rcx, request, a2, panel, boolka, a5);
}
#endif