#pragma once

__int64 GetAddressFromInstruction(__int64 instruction, char offset, char len, int extra_offset = 0)
{
	auto meow = instruction;
	if (extra_offset != 0)
		meow += extra_offset;

	auto relative_address = *(__int32*)(meow + offset);
	return meow + relative_address + len;
}