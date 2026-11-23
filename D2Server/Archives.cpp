#include "D2Server.h"
#include "Archive.h"

static HD2ARCHIVE sghArchiveData = 0;
static HD2ARCHIVE sghArchiveChar = 0;

BOOL __fastcall CharServOpenArchives(void)
{
	char szPath[MAX_PATH];

	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	sghArchiveData = ArchiveOpen(szPath, "d2data.mpq", "D2DATA", NULL, NULL);
	sghArchiveChar = ArchiveOpen(szPath, "d2char.mpq", "D2CHAR", NULL, NULL);
	if (sghArchiveData && sghArchiveChar)
		return TRUE;
	return FALSE;
}

HD2ARCHIVE __fastcall CharServGetArchive(void)
{
	return NULL;
}

void __fastcall CharServCloseArchives(void)
{
	if (sghArchiveData)
		ArchiveClose(sghArchiveData);
	sghArchiveData = NULL;
	if (sghArchiveChar)
		ArchiveClose(sghArchiveChar);
	sghArchiveChar = NULL;
}

#include "StormNew.h"
