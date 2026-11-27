#include <windows.h>
#include <string.h>
#include "D2Cmp.h"

#pragma warning(disable: 4035)

DWORD __cdecl GetFirstBit(DWORD dwValue)
{
	DWORD dwBit;

	if (!dwValue)
		return 0;
	__asm
	{
		bsf eax, dwValue
		inc eax
		mov dwBit, eax
	}
	return dwBit;
}

void * __fastcall MemVirtualAlloc(DWORD dwSize)
{
	return VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void __fastcall MemVirtualFree(void *pMemory)
{
	VirtualFree(pMemory, 0, MEM_RELEASE);
}

void __fastcall MemCopyRect(BYTE *pbDst, const BYTE *pbSrc, int nDstPitch, int nSrcPitch, DWORD nRows,
	DWORD dwWidth)
{
	int nSrcSkip;

	nSrcSkip = nSrcPitch - dwWidth;
	nSrcPitch = nDstPitch - dwWidth;
	__asm
	{
		mov edi, pbDst
		mov esi, pbSrc
		mov ebx, nRows
	copy_row:
		mov edx, dwWidth
		mov ecx, edx
		and edx, 3
		je copy_dwords
		cmp edx, 1
		je copy_one
		cmp edx, 2
		je copy_two
		mov al, [esi]
		mov [edi], al
		mov al, [esi + 1]
		mov [edi + 1], al
		mov al, [esi + 2]
		mov [edi + 2], al
		add edi, 3
		add esi, 3
		jmp copy_dwords
	copy_two:
		mov al, [esi]
		mov [edi], al
		mov al, [esi + 1]
		mov [edi + 1], al
		add edi, 2
		add esi, 2
		jmp copy_dwords
	copy_one:
		mov al, [esi]
		mov [edi], al
		inc edi
		inc esi
	copy_dwords:
		shr ecx, 2
		rep movsd
		add esi, nSrcSkip
		add edi, nSrcPitch
		dec ebx
		jne copy_row
	}
}

#pragma optimize("", off)
BOOL __fastcall MemCompare(const BYTE *pbFirst, const BYTE *pbSecond, DWORD dwSize)
{
	__asm
	{
		mov edi, pbFirst
		mov esi, pbSecond
		mov edx, dwSize
		mov ecx, edx
		and edx, 3
		je cmp_dwords
		cmp edx, 1
		je cmp_one
		cmp edx, 2
		je cmp_two
		mov al, [esi]
		mov bl, [edi]
		cmp al, bl
		jne cmp_differ
		mov al, [esi + 1]
		mov bl, [edi + 1]
		cmp al, bl
		jne cmp_differ
		mov al, [esi + 2]
		mov bl, [edi + 2]
		cmp al, bl
		jne cmp_differ
		add edi, 3
		add esi, 3
		jmp cmp_dwords
	cmp_two:
		mov al, [esi]
		mov bl, [edi]
		cmp al, bl
		jne cmp_differ
		mov al, [esi + 1]
		mov bl, [edi + 1]
		cmp al, bl
		jne cmp_differ
		add edi, 2
		add esi, 2
		jmp cmp_dwords
	cmp_one:
		mov al, [esi]
		mov bl, [edi]
		cmp al, bl
		jne cmp_differ
		inc edi
		inc esi
	cmp_dwords:
		shr ecx, 2
		repe cmpsd
		jne cmp_differ
		push 1
		pop eax
		jmp cmp_done
	cmp_differ:
		xor eax, eax
	cmp_done:
	}
}
