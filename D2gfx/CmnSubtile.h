#ifndef D2RE_D2GFX_CMNSUBTILE_H
#define D2RE_D2GFX_CMNSUBTILE_H

struct D2GFXCMNINFO
{
 	BYTE		*pbBuffer;
 	int			nPitch;
 	DWORD		dwHeight;
 	DWORD		field_0c;
};

extern D2GFXCMNINFO gtCmnInfo;
extern D2GFXENVINFO *gptCmnEnvInfo;
extern BYTE gaabCmnLightTables[49][256];

void __fastcall CmnSubtileBuildGouraud(void);
void __fastcall CmnSubtileSetClip(void);
void __fastcall CmnSubtileBuildRowOffsets(void);

#endif
