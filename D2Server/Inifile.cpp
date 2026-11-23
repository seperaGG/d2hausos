#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\Inifile.cpp"
#include "D2Server.h"
#include "D2Macros.h"

struct INIKEYWORD
{
	int			nId;
	const char	*pszKeyword;
};

INIBLANK gszIniBlank = { 0 };
struct INISTATE
{
	REALMINFO tRealm;
	BOOL fRealmRead;
};
static INISTATE sgtIni = { 0 };
#define sgtRealm (sgtIni.tRealm)
#define sgfRealmRead (sgtIni.fRealmRead)

static INIKEYWORD gatIniKeywords[] =
{
	{ 1, "realm" },
	{ 2, "name" },
	{ 3, "desc" },
	{ 4, "adminpass" },
	{ 5, "publicpass" },
	{ 6, "bnetip" },
	{ 7, "mcpip" },
	{ 8, "charserv" },
	{ 9, "accept" },
	{ 10, "multiclient" },
	{ 11, "announce" },
};
static int gnIniKeywords = 11;

static DWORD gadwNetMasks[33] =
{
	0, 128, 192, 224, 240, 248, 252, 254,
	255, 33023, 49407, 57599, 61695, 63743, 64767, 65279,
	65535, 8454143, 12648447, 14745599, 15794175, 16318463, 16580607, 16711679,
	16777215, 0x80FFFFFF, 0xC0FFFFFF, 0xE0FFFFFF, 0xF0FFFFFF, 0xF8FFFFFF, 0xFCFFFFFF, 0xFEFFFFFF,
	0xFFFFFFFF,
};

static char *gpszAnnounceNone = gszIniBlank;

static char * __fastcall IniSkip(char *psz, const char *pszDelims);
static char * __fastcall IniGetToken(char *psz, char *pszToken, const char *pszDelims);

void __fastcall IniGetModuleDir(char *pszPath)
{
	char *pszSlash;

	GetModuleFileNameA(NULL, pszPath, MAX_PATH);
	pszPath[MAX_PATH - 1] = 0;
	pszSlash = pszPath;
	while (*pszSlash && StormSStrChr(pszSlash + 1, '\\'))
		pszSlash = StormSStrChr(pszSlash + 1, '\\');
	*pszSlash = 0;
}

BOOL __stdcall IniIsAccepted(DWORD dwAddress)
{
	struct in_addr tNet;
	int i;

	for (i = 0; i < sgtRealm.nAccept; i++)
	{
		tNet = sgtRealm.atAccept[i].tNet;
		if ((gadwNetMasks[sgtRealm.atAccept[i].nBits] & dwAddress) == tNet.s_addr)
			return TRUE;
	}
	return FALSE;
}

int __fastcall IniFindKeyword(const char *pszKeyword)
{
	int i;

	for (i = 0; i < gnIniKeywords; i++)
	{
		if (!stricmp(pszKeyword, gatIniKeywords[i].pszKeyword))
			return gatIniKeywords[i].nId;
	}
	return 0;
}

static char * __fastcall IniGetQuoted(char *psz, char *pszValue)
{
	char *pch;

	pch = IniSkip(psz, gszIniBlank);
	while (*pch && *pch != '"')
		pch++;
	if (*pch == '"')
		pch++;
	while (*pch && *pch != '"')
		*pszValue++ = *pch++;
	if (*pch == '"')
		pch++;
	*pszValue = 0;
	return IniSkip(pch, gszIniBlank);
}

static BOOL __fastcall IniAddCharServ(DWORD dwAddress)
{
	BOOL fAdded;

	fAdded = FALSE;
	if (sgtRealm.nCharServ < MAX_CHAR_SERV)
	{
		sgtRealm.adwCharServ[sgtRealm.nCharServ] = dwAddress;
		sgtRealm.nCharServ++;
		fAdded = TRUE;
	}
	return fAdded;
}

static void __fastcall IniAddAccept(DWORD dwNet, int nBits)
{
	struct in_addr tAddress;
	int i;

	if (sgtRealm.nAccept >= MAX_ACCEPT)
	{
		tAddress.s_addr = dwNet;
		ServerLog(TRUE, "Unable to add ip (%s) to accept list - maximum accept list size is %d",
			inet_ntoa(tAddress), MAX_ACCEPT);
		return;
	}
	for (i = 0; i < sgtRealm.nAccept; i++)
	{
		if (sgtRealm.atAccept[i].tNet.s_addr == dwNet)
			return;
	}
	sgtRealm.atAccept[sgtRealm.nAccept].tNet.s_addr = dwNet;
	sgtRealm.atAccept[sgtRealm.nAccept].nBits = nBits;
	sgtRealm.nAccept++;
}

static BOOL __fastcall IniGetNumber(char *psz, int *pnValue)
{
	char szNumber[1024];
	BOOL fNumber;

	fNumber = FALSE;
	IniGetToken(psz, szNumber, gszIniBlank);
	if (szNumber[0] == '-')
	{
		if (isdigit(szNumber[1]))
			fNumber = TRUE;
	}
	else if (isdigit(szNumber[0]))
		fNumber = TRUE;
	if (fNumber)
		*pnValue = atoi(szNumber);
	return fNumber;
}

BOOL __fastcall IniRead(void)
{
	char szPath[1024];
	char szToken[1024];
	char szLine[1024];
	int nLine;
	char *pszToken;
	BOOL fSelected;
	DWORD dwLocalAddress;
	FILE *pFile;
	char *psz;
	char *pszNext;
	char *pch;
	DWORD dwAddress;
	struct in_addr tAddress;
	BOOL fRead;
	BOOL fDone;
	int nBits;
	int nValue;

	nLine = 0;
	fSelected = FALSE;
	fDone = FALSE;
	IniGetModuleDir(szPath);
	strcat(szPath, "\\");
	strcat(szPath, "realms.ini");
	ServerLog(TRUE, "[INIFILE]  Opening intialization file: %s", szPath);
	pFile = fopen(szPath, "rt");
	if (!pFile)
	{
		ServerLog(TRUE, "[INIFILE]  Unable to open initialization file: %s", szPath);
		return FALSE;
	}
	dwLocalAddress = inet_addr(FogSafesockGetLocalAddress());
	if (dwLocalAddress == INADDR_NONE)
	{
		ServerLog(TRUE, "[INIFILE]  Unable to get local ip address.");
		return FALSE;
	}
	memset(&sgtRealm, 0, sizeof(sgtRealm));
	while (1)
	{
		if (feof(pFile))
			break;
		pszToken = szToken;
		if (!fgets(szLine, 1024, pFile))
			break;
		nLine++;
		psz = IniGetToken(szLine, szToken, "=");
		if (!szToken[0] || szToken[0] == ';')
			continue;
		if (szToken[0] == '[')
		{
			if (!szToken[1])
				psz = IniGetToken(psz, szToken, "=");
			else
				pszToken = szToken + 1;
			pch = strchr(pszToken, ']');
			if (!pch)
			{
				ServerLog(TRUE,
					"[INIFILE]  Error parsing line (%d) -- Missing close bracket in section header.",
					nLine);
				continue;
			}
			*pch = 0;
		}
		switch (IniFindKeyword(pszToken))
		{
		case 1:
			if (fSelected)
			{
				fDone = TRUE;
				break;
			}
			memset(&sgtRealm, 0, sizeof(sgtRealm));
			ServerLog(TRUE, gszIniBlank);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:REALM", pszToken);
			break;

		case 2:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read realm name.", nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szName, pszToken, 16);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:NAME         VALUE:%s", pszToken);
			break;

		case 3:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read realm description.",
					nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szDesc, pszToken, 128);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:DESC         VALUE:%s", pszToken);
			break;

		case 4:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE,
					"[INIFILE]  Error parsing line (%d)  -- Unable to read admin password.", nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szAdminPass, pszToken, 16);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:ADMINPASS    VALUE:%s", pszToken);
			break;

		case 5:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE,
					"[INIFILE]  Error parsing line (%d)  -- Unable to read admin password.", nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szPublicPass, pszToken, 16);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:PUBLICPASS   VALUE:%s", pszToken);
			break;

		case 6:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE,
					"[INIFILE]  Error parsing line (%d)  -- Unable to read battle net ip.", nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szBnetIp, pszToken, 1024);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:BNETIP       VALUE:%s", pszToken);
			break;

		case 7:
			psz = IniGetToken(psz, pszToken, gszIniBlank);
			if (!*pszToken)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read MCP ip.", nLine);
				break;
			}
			dwAddress = inet_addr(pszToken);
			if (dwAddress == INADDR_NONE)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read MCP ip.", nLine);
				break;
			}
			StormSStrCopy(sgtRealm.szMcpIp, pszToken, 24);
			sgtRealm.dwMcpIp = dwAddress;
			tAddress.s_addr = dwAddress;
			ServerLog(TRUE, "[INIFILE]  KEYWORD:MCPIP        VALUE:%s", inet_ntoa(tAddress));
			break;

		case 8:
			fRead = FALSE;
			psz = IniGetToken(psz, pszToken, ",");
			while (*pszToken)
			{
				if (*pszToken == ';')
					break;
				dwAddress = inet_addr(pszToken);
				if (dwAddress == INADDR_NONE)
				{
					ServerLog(TRUE,
						"[INIFILE]  Error parsing line (%d)  -- Unable to read character server ip.", nLine);
					break;
				}
				fRead = TRUE;
				if (!IniAddCharServ(dwAddress))
				{
					ServerLog(TRUE,
						"[INIFILE]  Error parsing line (%d)  -- Exceeded maximum # of character servers.",
						nLine);
					break;
				}
				tAddress.s_addr = dwAddress;
				ServerLog(TRUE, "[INIFILE]  KEYWORD:CSIP(%d)      VALUE:%s", sgtRealm.nCharServ,
					inet_ntoa(tAddress));
				psz = IniGetToken(psz, pszToken, ",");
			}
			if (!fRead)
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read battle net ip.", nLine);
			break;

		case 9:
			fRead = FALSE;
			psz = IniGetToken(psz, pszToken, ",");
			pszNext = psz;
			while (*pszToken)
			{
				nBits = 32;
				if (*pszToken == ';')
					break;
				pch = strchr(pszToken, '/');
				if (pch)
				{
					*pch = 0;
					if (!IniGetNumber(pch + 1, &nBits))
					{
						ServerLog(TRUE,
							"[INIFILE]  Error parsing line (%d)  -- Unable to read acceptlist ip.", nLine);
						break;
					}
					if (nBits < 0 || nBits > 32)
					{
						ServerLog(TRUE,
							"[INIFILE]  Error parsing line (%d)  -- Unable to read acceptlist ip.", nLine);
						break;
					}
				}
				dwAddress = inet_addr(pszToken);
				if (dwAddress == INADDR_NONE)
				{
					ServerLog(TRUE,
						"[INIFILE]  Error parsing line (%d)  -- Unable to read acceptlist ip.", nLine);
					break;
				}
				fRead = TRUE;
				IniAddAccept(dwAddress, nBits);
				tAddress.s_addr = dwAddress;
				ServerLog(TRUE, "[INIFILE]  KEYWORD:ACCEPT       VALUE:%s/%d", inet_ntoa(tAddress), nBits);
				psz = IniGetToken(pszNext, pszToken, ",");
				pszNext = psz;
				if (IniIsAccepted(dwLocalAddress))
				{
					fSelected = TRUE;
					ServerLog(TRUE, "[INIFILE]  Selected realm.");
				}
			}
			if (!fRead)
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read battle net ip.", nLine);
			break;

		case 10:
			if (IniGetNumber(psz, &nValue))
			{
				if (nValue >= 0 && nValue <= 1024)
				{
					sgtRealm.nMultiClient = nValue;
					ServerLog(TRUE, "[INIFILE]  KEYWORD:MULTICLIENT  VALUE:%d.", nValue);
					break;
				}
			}
			ServerLog(TRUE,
				"[INIFILE]  Error parsing line (%d)  -- Invalid value for MULTICLIENT key.", nLine);
			break;

		case 11:
			psz = IniGetQuoted(psz, pszToken);
			if (!*pszToken)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Unable to read announcement.", nLine);
				break;
			}
			if (sgtRealm.nAnnounce >= MAX_ANNOUNCE)
			{
				ServerLog(TRUE, "[INIFILE]  Error parsing line (%d)  -- Exceeded maximum # of announcements.",
					nLine);
				break;
			}
			StormSStrCopy(sgtRealm.aszAnnounce[sgtRealm.nAnnounce++], pszToken, ANNOUNCE_LEN);
			ServerLog(TRUE, "[INIFILE]  KEYWORD:ANNOUNCE     VALUE:%s", pszToken);
			break;

		default:
			ServerLog(TRUE, "[INIFILE]  Error parsing line (%d) -- Unknown keyword (%s).", nLine, pszToken);
			break;
		}
		if (fDone)
			break;
	}
	fclose(pFile);
	ServerLog(TRUE, "[INIFILE]  Succesfully read intialization file: %s", szPath);
	if (!fSelected)
	{
		ServerLog(TRUE, "[INIFILE]  Error -- Unable to find realm for this machine.");
		return FALSE;
	}
	if (!sgtRealm.szName[0])
	{
		ServerLog(TRUE, "[INIFILE]  Error -- Realm info didn't contain name of realm.");
		return FALSE;
	}
	if (!sgtRealm.szBnetIp[0])
	{
		ServerLog(TRUE, "[INIFILE]  Error -- Realm info didn't contain battle net ip.");
		return FALSE;
	}
	if (sgtRealm.nAccept <= 0)
	{
		ServerLog(TRUE, "[INIFILE]  Error -- Realm info missing accept list.");
		return FALSE;
	}
	if (sgtRealm.nCharServ <= 0)
	{
		ServerLog(TRUE, "[INIFILE]  Error -- Realm info missing character server list.");
		return FALSE;
	}
	sgfRealmRead = TRUE;
	return TRUE;
}

static char * __fastcall IniSkip(char *psz, const char *pszDelims)
{
	while (*psz && (isspace(*psz) || strchr(pszDelims, *psz)))
		psz++;
	return psz;
}

static char * __fastcall IniGetToken(char *psz, char *pszToken, const char *pszDelims)
{
	psz = IniSkip(psz, pszDelims);
	while (*psz && !isspace(*psz) && !strchr(pszDelims, *psz))
		*pszToken++ = (char)tolower(*psz++);
	*pszToken = 0;
	return IniSkip(psz, pszDelims);
}

char * __fastcall IniGetName(void)
{
	return sgtRealm.szName;
}

char * __fastcall IniGetDesc(void)
{
	return sgtRealm.szDesc;
}

char * __fastcall IniGetAdminPass(void)
{
	return sgtRealm.szAdminPass;
}

char * __fastcall IniGetPublicPass(void)
{
	return sgtRealm.szPublicPass;
}

char * __fastcall IniGetBnetIp(void)
{
	return sgtRealm.szBnetIp;
}

char * __fastcall IniGetMcpIp(void)
{
	return sgtRealm.szMcpIp;
}

int __fastcall IniGetMultiClient(void)
{
	if (sgtRealm.nMultiClient)
		return sgtRealm.nMultiClient;
	return 1;
}

int __fastcall IniGetNumCharServ(void)
{
	return sgtRealm.nCharServ;
}

DWORD __fastcall IniGetCharServ(int i)
{
#line 778
	D2ASSERT(i >= 0 && i < MAX_CHAR_SERV);
	return sgtRealm.adwCharServ[i];
}

int __fastcall IniGetNumAnnounce(void)
{
	return sgtRealm.nAnnounce;
}

char * __fastcall IniGetAnnounce(int nIndex)
{
	if (nIndex >= sgtRealm.nAnnounce)
		return gpszAnnounceNone;
	return sgtRealm.aszAnnounce[nIndex];
}

void __fastcall IniClearAnnounce(void)
{
	sgtRealm.nAnnounce = 0;
}

BOOL __fastcall IniDeleteAnnounce(int nIndex)
{
	if (nIndex < 0 || nIndex >= sgtRealm.nAnnounce)
		return FALSE;
	sgtRealm.nAnnounce--;
	if (nIndex < sgtRealm.nAnnounce)
		memmove(sgtRealm.aszAnnounce[nIndex], sgtRealm.aszAnnounce[nIndex + 1],
			(sgtRealm.nAnnounce - nIndex) * ANNOUNCE_LEN);
	return TRUE;
}

BOOL __fastcall IniSetAnnounce(int nIndex, const char *pszText)
{
	if (nIndex < 0 || nIndex >= sgtRealm.nAnnounce)
		return FALSE;
	StormSStrCopy(sgtRealm.aszAnnounce[nIndex], pszText, ANNOUNCE_LEN);
	return TRUE;
}

BOOL __fastcall IniAddAnnounce(const char *pszText)
{
	if (sgtRealm.nAnnounce >= MAX_ANNOUNCE)
		return FALSE;
	StormSStrCopy(sgtRealm.aszAnnounce[sgtRealm.nAnnounce], pszText, ANNOUNCE_LEN);
	sgtRealm.nAnnounce++;
	return TRUE;
}

BOOL __fastcall IniInsertAnnounce(int nIndex, const char *pszText)
{
	if (nIndex < 0 || nIndex > sgtRealm.nAnnounce || sgtRealm.nAnnounce >= MAX_ANNOUNCE)
		return FALSE;
	if (nIndex < sgtRealm.nAnnounce - 1)
		memmove(sgtRealm.aszAnnounce[nIndex + 1], sgtRealm.aszAnnounce[nIndex],
			(sgtRealm.nAnnounce - nIndex) * ANNOUNCE_LEN);
	StormSStrCopy(sgtRealm.aszAnnounce[nIndex], pszText, ANNOUNCE_LEN);
	sgtRealm.nAnnounce++;
	return TRUE;
}
