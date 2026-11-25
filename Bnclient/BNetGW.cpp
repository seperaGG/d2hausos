#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\BNetGW.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "StormImports.h"
#include <stdlib.h>

BNGATEWAYS gtGateways;

static char sgszDefaultGateways[] =
	"uswest.battle.net  8  U.S. West\r\n"
	"useast.battle.net  5  U.S. East\r\n"
	"asia.battle.net   -9  Asia\r\n"
	"europe.battle.net -1  Europe\r\n";

void BNGATEWAYS::Load()
{
	TIME_ZONE_INFORMATION tZone;
	char *p;
	char *pszEnd;
	int nTexts;
	int nBytes;
	int nLength;

	if (regionList)
		return;
	nGateways = 0;
	nGateway = 0;
	regionList = NULL;
	regionListSize = 0;
	regionListVersion = 0;
	cNone = 0;
	ReadList();
	if (regionList && regionListVersion < BASE_REGIONLIST_VERSION)
	{
		BnclientLogPrintf("Obsolete gateway list (%d) - autodeleting", regionListVersion);
		fChosen = FALSE;
		Save();
		StormSRegDeleteValue("Configuration", "Battle.net gateways", 2);
	}
	if (!regionList)
		WriteDefault();
#line 71
	D2ASSERTMSG(regionListVersion >= BASE_REGIONLIST_VERSION, "regionListVersion >= BASE_REGIONLIST_VERSION");

	p = regionList;
	nTexts = -2;
	for (nBytes = 0; nBytes < regionListSize; nBytes += nLength)
	{
		nLength = StormSStrLen(p) + 1;
		p += nLength;
		nTexts++;
	}
	nGateways = nTexts / 3;
	p = regionList;
	p += StormSStrLen(p) + 1;
	nGateway = (int)strtoul(p, &pszEnd, 10);
	fChosen = FALSE;
	if (nGateway < 1 || nGateway > nGateways)
	{
		BnclientLogPrintf("Auto-picking gateway based on GMT");
		StormSMemZero(&tZone, sizeof(tZone));
		GetTimeZoneInformation(&tZone);
		PickByBias(tZone.Bias);
	}
}

void BNGATEWAYS::PickByBias(int nBias)
{
	int nHours;
	int nMinutes;
	int nBest;
	int nBestDistance;
	int nDistance;
	int nZone;
	int i;

	BnclientLogControl(2, 4);
	nMinutes = nBias % 60;
	nHours = nBias / 60;
	BnclientLogPrintf("Find gateway closest to %02d.%02d", nHours, nMinutes);
	nBest = 1;
	nBestDistance = 23 * 60;
	for (i = 1; i <= nGateways; i++)
	{
		nDistance = abs(GetZone(i) * 60 - nBias);
		if (nDistance < nBestDistance)
		{
			nBestDistance = nDistance;
			nBest = i;
		}
	}
	nZone = GetZone(nBest);
	BnclientLogPrintf("Gateway %d (%s at GMT %02d) is closest to %02d.%02d", nBest,
		GetHost(nBest), nZone, nHours, nMinutes);
	BnclientLogControl(2, -4);
	SetGateway(nBest);
}

void BNGATEWAYS::Save()
{
	char *p, *pList;
	const char *pszValue;

	if (fChosen && regionList)
	{
		BnclientLogPrintf("Recording gateway list selection");
		pList = regionList; p = pList; p += StormSStrLen(p) + 1;
		p[0] = (char)('0' + nGateway / 10);
		p[1] = (char)('0' + nGateway % 10);
		pszValue = fOverride ? "Override Battle.net gateways" : "Battle.net gateways";
#line 151
		D2ASSERTMSG(regionList[regionListSize - 1] == 0 && regionList[regionListSize - 2] == 0, "regionList[regionListSize-1] == 0 && regionList[regionListSize-2] == 0");
		StormSRegSaveData("Configuration", pszValue, 130, regionList, regionListSize);
	}
	if (regionList)
	{
#line 163
		StormSMemFree(regionList, D2_THIS_FILE, __LINE__, 0);
		regionList = NULL;
		regionListSize = 0;
	}
}

char * BNGATEWAYS::GetGateway(int nGateway)
{
	char *p;
	int nText;
	int nBytes;
	int nLength;

	if (!regionList || nGateway < 1 || nGateway > nGateways)
		return &cNone;
	p = regionList;
	nBytes = 0;
	for (nText = 1; nText < 3 * nGateway && nBytes < regionListSize; nText++)
	{
		nLength = StormSStrLen(p) + 1;
		p += nLength;
		nBytes += nLength;
	}
	if (nBytes >= regionListSize || nText > 3 * nGateway)
		return &cNone;
	return p;
}

char * BNGATEWAYS::GetHost(int nGateway)
{
	char *p;
	int nText;
	int nBytes;
	int nLength;

	if (!regionList || nGateway < 1 || nGateway > nGateways)
		return &cNone;
	p = regionList;
	nBytes = 0;
	for (nText = 1; nText < 3 * nGateway && nBytes < regionListSize; nText++)
	{
		nLength = StormSStrLen(p) + 1;
		p += nLength;
		nBytes += nLength;
	}
	if (nBytes >= regionListSize || nText > 3 * nGateway)
		return &cNone;
	return p;
}

long BNGATEWAYS::GetZone(int nGateway)
{
	char *p;
	char *pszEnd;

	p = GetHost(nGateway);
	if (*p)
		p += StormSStrLen(p) + 1;
	return strtol(p, &pszEnd, 10);
}

char * BNGATEWAYS::GetName(int nGateway)
{
	char *p;

	p = GetHost(nGateway);
	if (*p)
		p += StormSStrLen(p) + 1;
	if (*p)
		p += StormSStrLen(p) + 1;
	return p;
}

void BNGATEWAYS::SetGateway(int nGateway)
{
	if (!regionList || nGateway < 1 || nGateway > nGateways)
		return;
	if (nGateway > 99)
		nGateway = 99;
	this->nGateway = nGateway;
	fChosen = TRUE;
}

void BNGATEWAYS::Read(const char *pszValue)
{
	DWORD dwBytes;
	char *pszEnd;

#line 258
	D2ASSERTMSG(regionList == NULL, "regionList == NULL");
	dwBytes = 0;
	if (!StormSRegLoadData("Configuration", pszValue, 130, 0, 0, &dwBytes) || !dwBytes)
		return;
	regionListSize = dwBytes;
#line 274
	regionList = (char *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 0);
	if (!StormSRegLoadData("Configuration", pszValue, 130, regionList,
		regionListSize, 0))
	{
#line 282
		StormSMemFree(regionList, D2_THIS_FILE, __LINE__, 0);
		regionList = NULL;
		regionListSize = 0;
	}
	regionListVersion = 0;
	if (regionList)
	{
		pszEnd = regionList;
		regionListVersion = strtoul(regionList, &pszEnd, 10);
		if (regionList == pszEnd)
			regionListVersion = 0;
	}
}

void BNGATEWAYS::ReadList()
{
	fOverride = FALSE;
	Read("Override Battle.net gateways");
	if (regionList)
	{
		fOverride = TRUE;
		return;
	}
	Read("Battle.net gateways");
}

void BNGATEWAYS::WriteDefault()
{
	const char *pszDefault;
	const char *p;
	const char *pEnd;
	char *buffer;
	char *curr;
	int bufferlength;
	int nText;

	BnclientLogPrintf("Writing default gateway list");
	pszDefault = sgszDefaultGateways;
	nText = strlen(pszDefault);
	if (!pszDefault)
		return;
	bufferlength = 2 * nText;
#line 386
	buffer = (char *)StormSMemAlloc(bufferlength, D2_THIS_FILE, __LINE__, 0);
	p = pszDefault;
	pEnd = pszDefault + nText;
	sprintf(buffer, "%d", BASE_REGIONLIST_VERSION);
	curr = buffer + StormSStrLen(buffer) + 1;
	*curr++ = '0';
	*curr++ = '0';
	*curr++ = 0;
	while (p < pEnd)
	{
		if (p < pEnd && *p == '#')
		{
			p = BNetGWSkipLineBreaks(BNetGWSkipToLineEnd((char *)p, (char *)pEnd), (char *)pEnd);
			continue;
		}
		while (p < pEnd && !isspace(*p))
			*curr++ = *p++;
		*curr++ = 0;
		while (p < pEnd && isspace(*p))
			p++;
		while (p < pEnd && !isspace(*p))
			*curr++ = *p++;
		*curr++ = 0;
		while (p < pEnd && isspace(*p))
			p++;
		while (p < pEnd && *p != '\r' && *p != '\n')
			*curr++ = *p++;
		*curr++ = 0;
		p = BNetGWSkipLineBreaks((char *)p, (char *)pEnd);
	}
	*curr++ = 0;
#line 446
	D2ASSERT(curr - buffer < bufferlength);
	bufferlength = curr - buffer;
#line 448
	D2ASSERT(buffer[bufferlength-1] == 0 && buffer[bufferlength-2] == 0);
	StormSRegSaveData("Configuration", "Battle.net gateways", 130, buffer, bufferlength);
	regionList = buffer;
	regionListSize = bufferlength;
	regionListVersion = BASE_REGIONLIST_VERSION;
}

char * __stdcall BNetGWSkipToLineEnd(char *p, char *pEnd)
{
	while (p < pEnd && *p && *p != '\r' && *p != '\n')
		p++;
	return p;
}

char * __stdcall BNetGWSkipLineBreaks(char *p, char *pEnd)
{
	while (p < pEnd && *p && (*p == '\r' || *p == '\n'))
		p++;
	return p;
}

char * BNGATEWAYS::FindSection(char *pszText, const char *pszName)
{
	char *p;
	int nText;
	int nName;

	nText = strlen(pszText);
	nName = strlen(pszName);
	p = pszText;
	while (*p)
	{
		while (*p && *p != '[')
			p++;
#line 501
		D2ASSERT(*p != 0);
		if (_strnicmp(p + 1, pszName, nName))
			p += nName;
		else
			return BNetGWSkipLineBreaks(BNetGWSkipToLineEnd(p + nName + 2, pszText + nText), pszText + nText);
	}
	return NULL;
}

char * __stdcall BNetGWFindKey(char *pszText, const char *pszKey)
{
	char *p;
	int nText;
	int nKey;

	nText = strlen(pszText);
	nKey = strlen(pszKey);
	p = pszText;
	while (*p)
	{
		if (*p == ']')
			break;
		if (!_strnicmp(p, pszKey, nKey))
			return p + nKey + 1;
		p = BNetGWSkipLineBreaks(BNetGWSkipToLineEnd(p, pszText + nText), pszText + nText);
	}
	return NULL;
}

void BNGATEWAYS::Update(char *pszIni)
{
	char *pszEnd;
	char *versHdr;
	char *vers;
	char *pszNumberEnd;
	char *buffer;
	char *curr;
	char *pszSection;
	char *pszLine;
	char *pszHost;
	char *pszServer;
	char *pszValue;
	char szKey[16];
	char szLanguage[64];
	DWORD dwVersion;
	int bufferlength;
	int nBias;
	int nLength;
	int i;

	pszEnd = pszIni + strlen(pszIni);
	versHdr = FindSection(pszIni, "Server List Version");
#line 550
	D2ASSERT(versHdr);
	vers = BNetGWFindKey(versHdr, "VER");
#line 555
	D2ASSERT(vers);
	pszNumberEnd = vers;
	dwVersion = strtoul(vers, &pszNumberEnd, 10);
	if (!(dwVersion == regionListVersion))
	{
		BnclientLogPrintf("Updating to new list received from Battle.net");
#line 569
		D2ASSERTMSG(regionList != NULL, "regionList != NULL");
		nBias = GetZone(nGateway) * 60;
		fChosen = FALSE;
		Save();
		fOverride = FALSE;
		fChosen = TRUE;
		regionListVersion = dwVersion;
		bufferlength = 2 * strlen(pszIni);
#line 583
		buffer = (char *)StormSMemAlloc(bufferlength, D2_THIS_FILE, __LINE__, 0);
		sprintf(buffer, "%d", dwVersion);
		curr = buffer + StormSStrLen(buffer) + 1;
		*curr++ = '0';
		*curr++ = '0';
		*curr++ = 0;
		pszSection = FindSection(pszIni, "Server Gateways");
		for (i = 1; ; i++)
		{
			sprintf(szKey, "%d", i);
			pszLine = BNetGWFindKey(pszSection, szKey);
			if (!pszLine)
				break;
			nLength = BNetGWSkipToLineEnd(pszLine, pszEnd) - pszLine;
			StormSStrCopy(curr, pszLine, nLength + 1);
			pszHost = curr;
			pszServer = FindSection(pszIni, pszHost);
			if (!pszServer)
				continue;
			curr += nLength + 1;
			pszValue = BNetGWFindKey(pszServer, "ZONE");
			if (pszValue)
			{
				nLength = BNetGWSkipToLineEnd(pszValue, pszEnd) - pszValue;
				StormSStrCopy(curr, pszValue, nLength + 1);
			}
			else
			{
				nLength = 1;
				StormSStrCopy(curr, "0", 2);
			}
			curr += nLength + 1;
			D2LangStrGetLanguageName(szLanguage, 0);
			pszValue = BNetGWFindKey(pszServer, szLanguage);
			if (!pszValue)
				GetLocaleInfoA(1033, LOCALE_SABBREVLANGNAME, szLanguage, sizeof(szLanguage));
			if (pszValue)
			{
				nLength = BNetGWSkipToLineEnd(pszValue, pszEnd) - pszValue;
				StormSStrCopy(curr, pszValue, nLength + 1);
			}
			else
			{
				nLength = strlen(pszHost);
				StormSStrCopy(curr, pszHost, nLength + 1);
			}
			curr += nLength + 1;
		}
		*curr++ = 0;
#line 658
		D2ASSERT(curr - buffer < bufferlength);
		bufferlength = curr - buffer;
#line 660
		D2ASSERT(buffer[bufferlength-1] == 0 && buffer[bufferlength-2] == 0);
		regionList = buffer;
		regionListSize = bufferlength;
		PickByBias(nBias); }
}
