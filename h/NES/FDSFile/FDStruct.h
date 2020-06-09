#pragma once

/*
	A FDA Super Mario Bros 2 [J] [public version]
	Copyright (C) 2020 ALXR aka loginsin
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NES/NES.h"

#define		FDS_DISK_SIGNATURE			"*NINTENDO-HVC*"
#define		FDS_SIDE_SIZE				65500
#define		FDS_FILE_SIGNATURE			0x1A534446		//  $46 $44 $53 $1A

typedef enum _tagFDS_BLOCK_TYPE : BYTE
{
	DiskInfoBlock = 1,
	FileAmountBlock = 2,
	FileHeaderBlock = 3,
	FileDataBlock = 4,
	fbtUnknown
} FDS_BLOCK_TYPE, *PFDS_BLOCK_TYPE;

typedef enum _tagFDS_FILE_TYPE : BYTE
{
	PRG = 0,
	CHR = 1,
	VRAM = 2
} FDS_FILE_TYPE, *PFDS_FILE_TYPE;

typedef enum _tagFDS_MANUFACTURE_CODE : BYTE
{
	fmcUnlicensed = 0x00,
	fmcNintendo = 0x01,
	fmcCapcom = 0x08,
	fmcJaleco = 0x0A,
	fmcHudsonSoft = 0x18,
	fmcIrem = 0x49,
	fmcGakken = 0x4A,
	fmcBulletProofSoftware = 0x8B,
	fmcPackInVideo = 0x99,
	fmcTecmo = 0x9B,
	fmcImagineer = 0x9C,
	fmcScorpionSoft = 0xA2,
	fmcKonami = 0xA4,
	fmcKawada = 0xA6,
	fmcTakara = 0xA7,
	fmcRoyalIndustries = 0xA8,
	fmcToeiAnimation = 0xAC,
	fmcNamco = 0xAF,
	fmcASCIICorporation = 0xB1,
	fmcBandai = 0xB2,
	fmcSoftPro = 0xB3,
	fmcHALLaboratory = 0xB6,
	fmcSunsoft = 0xBB,
	fmcToshiba = 0xBC,
	fmcTaito = 0xC0,
	fmcSunsoftAsk = 0xC1,
	fmcKemco = 0xC2,
	fmcSquare = 0xC3,
	fmcTokumaShoten = 0xC4,
	fmcDataEast = 0xC6,
	fmcTonkinHouse = 0xC6,
	fmcEastCube = 0xC7,
	fmcKonamiCA = 0xCA,
	fmcNTVIC = 0xCB,
	fmcUse = 0xCC,
	fmcPonyCanyon = 0xCE,
	fmcSofel = 0xD1,
	fmcBothtec = 0xD2,
	fmcHiro = 0xDB,
	fmcAthena = 0xE7,
	fmcAtlus = 0xEB
} FDS_MANUFACTURE_CODE, *PFDS_MANUFACTURE_CODE;

typedef enum _tagFDS_GAME_TYPE : BYTE
{
	fgtNormal = ' ',
	fgtEvent = 'E',
	fgtAdvert = 'R'
} FDS_GAME_TYPE, *PFDS_GAME_TYPE;

typedef enum _tagFDS_DISK_TYPE : BYTE
{
	fdtFDC = 0,
	fdtFSC = 1
} FDS_DISK_TYPE, *PFDS_DISK_TYPE;

#pragma pack(push, 1)

typedef struct _tagFDS_HEADER
{
	DWORD		dwSignature;
	BYTE		uDiskSides;
	BYTE		bReserved[ 11 ];
} FDS_HEADER, *PFDS_HEADER;

typedef struct _tagFDS_DISK_INFO_BLOCK
{
	BYTE					strDiskVerification[ 14 ];
	FDS_MANUFACTURE_CODE	fmc;
	BYTE					strGameName[ 3 ];
	FDS_GAME_TYPE			fgt;
	BYTE					uVersion;
	BYTE					uSideNumber;
	BYTE					uDiskNumber;
	FDS_DISK_TYPE			fdt;
	BYTE					uReserved1;
	BYTE					uBootCode;
	BYTE					uPadding1[ 5 ];
	BYTE					uManufactureDate[ 3 ];
	BYTE					uCountryCode;
	BYTE					uReserved2;
	BYTE					uReserved3;
	BYTE					uReserved4[ 2 ];
	BYTE					uReserved5[ 5 ];
	BYTE					uFormatDate[ 3 ];
	BYTE					uReserved6;
	BYTE					uReserved7;
	USHORT					uDiskSerial;
	BYTE					uReserved8;
	BYTE					uRewriteCount;
	BYTE					uActualDiskSide;
	BYTE					uReserved9;
	BYTE					uPrice;
	//	USHORT					uCRC;
} FDS_DISK_INFO_BLOCK, *PFDS_DISK_INFO_BLOCK;

typedef struct _tagFDS_FILE_AMOUNT_BLOCK
{
	BYTE					uFileAmount;
} FDS_FILE_AMOUNT_BLOCK, *PFDS_FILE_AMOUNT_BLOCK;

typedef struct _tagFDS_FILE_HEADER_BLOCK
{
	BYTE					uFileNumber;
	BYTE					uFileId;
	BYTE					strName[ 8 ];
	USHORT					uTargetPtr;
	USHORT					uFileSize;
	FDS_FILE_TYPE			fft;
} FDS_FILE_HEADER_BLOCK, *PFDS_FILE_HEADER_BLOCK;

#pragma pack(pop)
