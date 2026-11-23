#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Excel\\Excel.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"
#include "FogImports.h"

#define EXCELMAXCELLS	256
#define SYM_EOL			10

void __stdcall ExcelFileFree(PTEXCELFILE ptFile);

PTEXCELFILE __stdcall ExcelOpen(char *pBuffer, int nSize)
{
	PTEXCELFILE file;
	char *data;
	char *pEnd;
	char *pRow;
	char c;

	pEnd = pBuffer + nSize;
#line 66
	file = (PTEXCELFILE)D2ALLOCPOOL(NULL, sizeof(EXCELFILE));
	file->pHeaderCells = pBuffer;
	file->pszFirstRow = NULL;
	file->dwRowCount = 0;
	file->nCellCount = 0;
	for (data = pBuffer; data < pEnd; )
	{
		c = *data++;
		if (c == '\t')
		{
			data[-1] = 0;
			file->nCellCount++;
		}
		else if (c == '\r')
		{
			data[-1] = 0;
#line 81
			D2ASSERT(*data == SYM_EOL);
			data++;
			file->nCellCount++;
			break;
		}
	}
	if (data >= pEnd)
	{
		ExcelFileFree(file);
		return NULL;
	}
#line 92
	D2ASSERT(file->nCellCount <= EXCELMAXCELLS);
	file->pszFirstRow = data;
	while (data < pEnd)
	{
		pRow = data;
		while (data < pEnd)
		{
			c = *data++;
			if (c == '\t')
				data[-1] = 0;
			else if (c == '\r')
			{
				data[-1] = 0;
#line 106
				D2ASSERT(*data == SYM_EOL);
				data++;
				file->dwRowCount++;
				break;
			}
		}
		if (!StormSStrCmp(pRow, "Expansion", 10))
		{
			file->dwRowCount--;
			return file;
		}
	}
	return file;
}

void __stdcall ExcelFileFree(PTEXCELFILE ptFile)
{
#line 138
	D2FREEPOOL(NULL, ptFile->pHeaderCells);
#line 139
	D2FREEPOOL(NULL, ptFile);
}

DWORD __stdcall ExcelFileGetRowCount(PTEXCELFILE ptFile)
{
	return ptFile->dwRowCount;
}

struct EXCELNAMENODE
{
	struct EXCELNAMENODE	*field_00;
	DWORD		field_04;
	PTEXCELCOLDESC	field_08;
};

static inline DWORD ExcelHashBucket(DWORD dwHash)
{
	return (((((dwHash & 16711680) ^ (dwHash >> 8)) >> 8) ^ (dwHash & 65280)) >> 8) ^ (dwHash & 255);
}

static inline DWORD ExcelMapColumns(PTEXCELFILE file, PTEXCELCOLDESC ptDataTable, WORD *awColumns)
{
	struct EXCELNAMENODE atNames[EXCELMAXCELLS];
	struct EXCELNAMENODE *aptBuckets[256];
	PTEXCELCOLDESC ptData;
	struct EXCELNAMENODE *ptName;
	DWORD nameCount;
	DWORD nColumns;
	DWORD dwHash;
	DWORD dwBucket;
	const char *pszName;
	char *pszCell;
	DWORD i;

	nameCount = 0;
	nColumns = 0;
	for (ptData = ptDataTable; ptData->nType; ptData++)
	{
		if (ptData->nType == EXCEL_TYPE_BYTE_OR)
		{
			for (i = 1; i < (DWORD)ptData->nLength; i++)
			{
				ptData++;
#line 180
				D2ASSERT(ptData->nType == EXCEL_TYPE_BYTE_OR);
			}
		}
	}
	StormSMemZero(aptBuckets, 256 * sizeof(struct EXCELNAMENODE *));
	for (ptData = ptDataTable; ptData->nType; ptData++)
	{
#line 188
		D2ASSERT(nameCount < EXCELMAXCELLS);
		ptName = &atNames[nameCount++];
		pszName = ptData->pszName;
		dwHash = StormSStrHash(pszName, 0, 0);
		ptName->field_04 = dwHash;
		ptName->field_08 = ptData;
		dwBucket = ExcelHashBucket(dwHash);
		ptName->field_00 = aptBuckets[dwBucket];
		aptBuckets[dwBucket] = ptName;
	}
	pszCell = file->pHeaderCells;
	for (i = 0; i < file->nCellCount; i++)
	{
		dwHash = StormSStrHash(pszCell, 0, 0);
		for (ptName = aptBuckets[ExcelHashBucket(dwHash)]; ptName; ptName = ptName->field_00)
		{
			if (ptName->field_04 == dwHash &&
				!StormSStrCmpI(pszCell, ptName->field_08->pszName, 2147483647))
				break;
		}
		if (ptName)
		{
			ptName->field_04 = 0;
			awColumns[nColumns++] = (WORD)(ptName->field_08 - ptDataTable);
		}
		else
			awColumns[nColumns++] = 32767;
		while (*pszCell)
			pszCell++;
		pszCell++;
	}
	for (i = 0, ptName = atNames; i < nameCount; i++, ptName++)
	{
		if (ptName->field_04)
		{
			if (ptName->field_08->nType != EXCEL_TYPE_OPTSTRING &&
				ptName->field_08->nType != EXCEL_TYPE_OPTBYTE)
				FogErrorManagerLog("Excel attribute '%s' not found in file", ptName->field_08->pszName);
			awColumns[nColumns++] = (WORD)(ptName->field_08 - ptDataTable);
		}
	}
	return nColumns;
}

void __stdcall ExcelParseRecords(PTEXCELFILE file, PTEXCELCOLDESC ptDataTable, BYTE *pRecords, DWORD nMemHgt,
	DWORD nMemSpan)
{
	WORD awColumns[EXCELMAXCELLS];
	PTEXCELCOLDESC ptData;
	DWORD nColumns;
	DWORD i;
	DWORD nRow;
	char *data;
	char *pCell;
	BYTE *pField;
	BYTE *pLimit;
	BYTE abCode[4];
	int nValue;

#line 248
	D2ASSERT(nMemHgt > 0);
#line 249
	D2ASSERT(nMemSpan > 0);
	nColumns = ExcelMapColumns(file, ptDataTable, awColumns);
	data = (char *)file->pszFirstRow;
	for (nRow = 0; nRow < file->dwRowCount; nRow++)
	{
		for (i = 0; i < file->nCellCount; i++)
		{
			if (awColumns[i] == 32767)
			{
				while (*data)
					data++;
				data++;
				continue;
			}
			ptData = &ptDataTable[awColumns[i]];
			pCell = data;
			pField = pRecords + ptData->nOffset;
			pLimit = pField + ptData->nLength;
			switch (ptData->nType)
			{
			case EXCEL_TYPE_STRING:
			case EXCEL_TYPE_OPTSTRING:
				while (*data)
				{
					if (pField >= pLimit)
						break;
					*pField++ = *data++;
				}
				while (*data)
					data++;
				data++;
				*pField = 0;
				break;
			case EXCEL_TYPE_CODE:
				memset(abCode, 0, sizeof(abCode));
				if (*data)
				{
					abCode[0] = *data++;
					if (*data)
					{
						abCode[1] = *data++;
						if (*data)
							abCode[2] = *data++;
					}
				}
				while (*data)
					data++;
				data++;
				pField[0] = abCode[0] ? abCode[0] : ' ';
				pField[1] = abCode[1] ? abCode[1] : ' ';
				pField[2] = abCode[2] ? abCode[2] : ' ';
				pField[3] = abCode[3] ? abCode[3] : ' ';
				break;
			case EXCEL_TYPE_DWORD:
			case EXCEL_TYPE_WORD:
			case EXCEL_TYPE_BYTE:
			case EXCEL_TYPE_BYTE_OR:
			case EXCEL_TYPE_OPTBYTE:
			case 8:
				if (*data == '-')
					data++;
				nValue = 0;
				while (*data)
					nValue = nValue * 10 + *data++ - '0';
				if (*pCell == '-')
					nValue = -nValue;
				data++;
				switch (ptData->nType)
				{
				case EXCEL_TYPE_DWORD:
				case 8:
					*(int *)pField = nValue;
					break;
				case EXCEL_TYPE_WORD:
					*(WORD *)pField = (WORD)nValue;
					break;
				case EXCEL_TYPE_BYTE:
				case EXCEL_TYPE_OPTBYTE:
					*pField = (BYTE)nValue;
					break;
				case EXCEL_TYPE_BYTE_OR:
					if (pCell + 1 != data)
						*pField = (BYTE)nValue;
					break;
				}
				break;
			default:
				__assume(0);
			}
		}
		for (; i < nColumns; i++)
		{
			ptData = &ptDataTable[awColumns[i]];
			pField = pRecords + ptData->nOffset;
			switch (ptData->nType)
			{
			case EXCEL_TYPE_DWORD:
				*(DWORD *)pField = 0;
				break;
			case 8:
				*(DWORD *)pField = 0;
				break;
			case EXCEL_TYPE_CODE:
				*(DWORD *)pField = 0;
				break;
			case EXCEL_TYPE_WORD:
				*(WORD *)pField = 0;
				break;
			case EXCEL_TYPE_STRING:
				*pField = 0;
				break;
			case EXCEL_TYPE_BYTE:
				*pField = 0;
				break;
			case EXCEL_TYPE_BYTE_OR:
				*pField = 0;
				break;
			case EXCEL_TYPE_OPTBYTE:
				*pField = 0;
				break;
			case EXCEL_TYPE_OPTSTRING:
				*pField = 0;
				break;
			default:
				__assume(0);
			}
		}
#line 345
		D2ASSERT(*data == SYM_EOL);
		data++;
		pRecords += nMemSpan;
	}
}
