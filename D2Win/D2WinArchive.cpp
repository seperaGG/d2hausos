#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinArchive.cpp"
#include "D2Win.h"
#include "D2Macros.h"
#include "Archive.h"

extern char *const gpszD2WinDll;

static HD2ARCHIVE sghArchiveMusic = { 0 };
static HD2ARCHIVE sghArchiveSpeech = { 0 };
static HD2ARCHIVE sghArchiveUnknown = { 0 };
static HD2ARCHIVE sghArchiveChar = { 0 };
static HD2ARCHIVE sghArchiveData = { 0 };
static HD2ARCHIVE sghArchiveVideo = { 0 };
static HD2ARCHIVE sghArchiveDelta = { 0 };
static HD2ARCHIVE sghArchivePatch = { 0 };
static HD2ARCHIVE sghArchiveSfx = { 0 };

HCELFILE __fastcall D2WinArchiveLoadCel(const char *pszName, DWORD eType)
{
	HCELFILE hCelFile;
	char szPath[MAX_PATH];
	void *pFile;

	strcpy(szPath, pszName);
	strcat(szPath, D2CMPSpriteCacheGetExtension(eType));
#line 58
	pFile = ArchiveAllocBufferAndRead(NULL, szPath, NULL, D2_THIS_FILE, __LINE__);
	D2CMPCelFileLoadSerialized(pFile, &hCelFile, D2_THIS_FILE, __LINE__, -1, 0);
	return hCelFile;
}

HCELFILE __fastcall D2WinArchiveLoadCelWithSize(const char *pszName, DWORD *pdwSize, DWORD eType)
{
	HCELFILE hCelFile;
	char szPath[MAX_PATH];
	void *pFile;

	strcpy(szPath, pszName);
	strcat(szPath, D2CMPSpriteCacheGetExtension(eType));
#line 75
	pFile = ArchiveAllocBufferAndRead(NULL, szPath, pdwSize, D2_THIS_FILE, __LINE__);
	D2CMPCelFileLoadSerialized(pFile, &hCelFile, D2_THIS_FILE, __LINE__, -1, 0);
	return hCelFile;
}

void __fastcall D2WinArchiveFreeCel(HCELFILE hCelFile)
{
	D2CMPCelFileFreeHardware(hCelFile);
	if (hCelFile)
	{
#line 85
		D2FREE2(hCelFile);
	}
}

BOOL __fastcall D2WinArchiveAskPlayDisc(void)
{
	return MessageBoxA(D2gfxWindowGetHandle(), "Insert Play Disc", "Diablo II",
		MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK;
}

BOOL __fastcall D2WinArchiveAskCinematicsDisc(void)
{
	return MessageBoxA(D2gfxWindowGetHandle(), "Insert Cinematics Disc", "Diablo II",
		MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK;
}

BOOL __fastcall D2WinArchiveOpenInstalled(void)
{
	sghArchiveData = ArchiveOpen(gpszD2WinDll, "d2data.mpq", "D2DATA", NULL, NULL);
	sghArchiveSfx = ArchiveOpen(gpszD2WinDll, "d2sfx.mpq", "D2SFX", NULL, NULL);
	sghArchiveSpeech = ArchiveOpen(gpszD2WinDll, "d2speech.mpq", "D2SPEECH", NULL, NULL);
	sghArchiveDelta = ArchiveOpen(gpszD2WinDll, "d2delta.mpq", "D2DELTA", NULL, NULL);
	sghArchivePatch = ArchiveOpen(gpszD2WinDll, "patch_d2.mpq", "PATCH_D2", NULL, NULL);
	if (!FogSafesockGetFileOption() && (!sghArchiveData || !sghArchiveSfx || !sghArchiveSpeech))
		return FALSE;
	return TRUE;
}

void __fastcall D2WinArchiveCloseAll(void)
{
	if (sghArchiveData)
	{
		ArchiveClose(sghArchiveData);
		sghArchiveData = NULL;
	}
	if (sghArchiveUnknown)
	{
		ArchiveClose(sghArchiveUnknown);
		sghArchiveUnknown = NULL;
	}
	if (sghArchiveSpeech)
	{
		ArchiveClose(sghArchiveSpeech);
		sghArchiveSpeech = NULL;
	}
	if (sghArchiveSfx)
	{
		ArchiveClose(sghArchiveSfx);
		sghArchiveSfx = NULL;
	}
	if (sghArchiveDelta)
	{
		ArchiveClose(sghArchiveDelta);
		sghArchiveDelta = NULL;
	}
	if (sghArchivePatch)
	{
		ArchiveClose(sghArchivePatch);
		sghArchivePatch = NULL;
	}
	if (sghArchiveMusic)
	{
		ArchiveClose(sghArchiveMusic);
		sghArchiveMusic = NULL;
	}
	if (sghArchiveChar)
	{
		ArchiveClose(sghArchiveChar);
		sghArchiveChar = NULL;
	}
	if (sghArchiveVideo)
	{
		ArchiveClose(sghArchiveVideo);
		sghArchiveVideo = NULL;
	}
}

HD2ARCHIVE __fastcall D2WinArchiveGetArchive(void)
{
	return NULL;
}

BOOL __fastcall D2WinArchiveNeedPlayDisc(void)
{
	char szPath[MAX_PATH];

	FogSafesockGetInstallPath(szPath, MAX_PATH);
	StormSStrAppend(szPath, "d2char.mpq", MAX_PATH);
	if (GetFileAttributesA(szPath) != (DWORD)-1)
		return FALSE;
	StormSStrCopy(szPath, "d2char.mpq", MAX_PATH);
	if (GetFileAttributesA(szPath) != (DWORD)-1)
		return FALSE;
	return !FogSafesockGetFileOption();
}

BOOL __fastcall D2WinArchiveOpenPlayDisc(BOOL (*pfnRetry)(void))
{
	if (FogSafesockGetFileOption())
		pfnRetry = NULL;
	sghArchiveChar = ArchiveOpen(gpszD2WinDll, "d2char.mpq", "D2CHAR", NULL, pfnRetry);
	sghArchiveMusic = ArchiveOpen(gpszD2WinDll, "d2music.mpq", "D2MUSIC", NULL, pfnRetry);
	if (!FogSafesockGetFileOption() && (!sghArchiveChar || !sghArchiveMusic))
		return FALSE;
	return TRUE;
}

BOOL __fastcall D2WinArchiveOpenCinematicsDisc(BOOL (*pfnRetry)(void))
{
	if (FogSafesockGetFileOption())
		pfnRetry = NULL;
	sghArchiveVideo = ArchiveOpen(gpszD2WinDll, "d2video.mpq", "D2VIDEO", NULL, pfnRetry);
	if (!FogSafesockGetFileOption() && !sghArchiveVideo)
		return FALSE;
	return TRUE;
}

void __fastcall D2WinArchiveCloseDiscs(void)
{
	if (sghArchiveMusic)
	{
		ArchiveClose(sghArchiveMusic);
		sghArchiveMusic = NULL;
	}
	if (sghArchiveChar)
	{
		ArchiveClose(sghArchiveChar);
		sghArchiveChar = NULL;
	}
	if (sghArchiveVideo)
	{
		ArchiveClose(sghArchiveVideo);
		sghArchiveVideo = NULL;
	}
}
