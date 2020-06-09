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

#include "NES/FDSFile/CFDSFile.h"
#include "CBitmap.h"

class CNesColors;
extern const COLORREF g_crNesColor[];
extern const CNesColors g_hbmColor;

#define		NES_FDS_BEGIN			0x6000
#define		NES_FDS_BIOS			0xE000
#define		NES_RAM_SIZE			( NES_FDS_BIOS - NES_FDS_BEGIN )
#define		NES_PPU_CHR_ROM_SIZE	0x2000
#define		NES_PPU_VRAM_SIZE		0x1F00
#define		NES_PPU_PALETTE_SIZE	0x20
#define		NES_PPU_PADDING			0xE0
#define		NES_PPU_SIZE			( NES_PPU_CHR_ROM_SIZE + NES_PPU_VRAM_SIZE + NES_PPU_PALETTE_SIZE + NES_PPU_PADDING )
#define		NES_ATTR_COUNT			4
#define		NES_TILES_COUNT			256
#define		NES_TILE_WIDTH			8
#define		NES_TILE_HEIGHT			8
#define		NES_PAL_COLORS			16
#define		NES_TRANSPARENT_COL		RGB(0xFF, 0, 0xFF)

typedef union _tagNES_RAM_MAP
{
	DWORD		id;
	struct
	{
		BYTE			bEmptyFlag;
		BYTE			diskId;
		FDS_DISK_SIDE	diskSide;
		BYTE			fileId;
	};

	_tagNES_RAM_MAP( DWORD init ) : id( init ) {};
} NES_RAM_MAP, *PNES_RAM_MAP;

typedef struct _tagNES_PIXEL
{
	union
	{
		BYTE	bRawColor;
		struct
		{
			BYTE	bColor : 2;
			BYTE	bAttr : 2;
			BYTE	fSprite : 1;
			BYTE	bExtended : 3;
		};
	};

	operator BYTE&()
	{
		return bRawColor;
	};

	_tagNES_PIXEL() : bRawColor( 0 ) {}
	_tagNES_PIXEL( BYTE bInit ) : bRawColor( bInit ) {};
} NES_PIXEL, *PNES_PIXEL;

typedef struct _tagNES_TILE
{
	BYTE pix[ NES_TILE_HEIGHT ][ NES_TILE_WIDTH ];
} NES_TILE, *PNES_TILE;

typedef struct _tagNES_PPU_PALETTE
{
	BYTE			bPal[ NES_PAL_COLORS ];
} NES_PPU_PALETTE, *PNES_PPU_PALETTE;

class CNesColors
{
	HBITMAP			m_hbm[ 64 ];
	const HDC		m_hDCDesktop;
	const HDC		m_hDC;

public:
	CNesColors();
	HBITMAP operator[]( BYTE bColor ) const;
	HBITMAP Bitmap( BYTE bColor ) const;
	~CNesColors();
};

class CNesBitmap
{
	struct
	{
		BOOL							fInit;
		std::vector<NES_PIXEL>			vBitmap;
		struct
		{
			size_t						cx;
			size_t						cy;
		} sz;
	} m_bm;

	VOID							UnsafeBlitTile( const NES_TILE & tile, const RECT & rc, BYTE bAttr, BOOL fSprite, PNES_PIXEL pBits, BOOL fFlipHorz, BOOL fFlipVert );

public:
	CNesBitmap();
	VOID							Create( int cx, int cy );
	VOID							Destroy();
	PNES_PIXEL						Bits() const;

	VOID							BlitTile( const NES_TILE & tile, int x, int y, BYTE bAttr );
	VOID							BlitSprite( const NES_TILE & tile, int x, int y, BYTE bAttr, BOOL fFlipHorz, BOOL fFlipVert );
	int								Width() const;
	int								Height() const;
	VOID							Blit( CNesBitmap & bmSource, int x, int y, int width, int height, int src_x, int src_y );
	VOID							BlitRaw( PBYTE pSourceBits, int x, int y, int width, int height, int src_x, int src_y, int source_width, int source_height );
	VOID							RBlitRaw( 
		PBYTE pTargetBits, 
		int x, int y, int width, int height, 
		int tgt_x, int tgt_y, int target_width, int target_height,
		BOOL fFlipHorz = FALSE, BOOL fFlipVert = FALSE
	) const;
};

class CNESMap
{
	CFDSFile						&	m_file;

	NES_RAM_MAP							UpdateAddr( USHORT & ptr, USHORT & uFilePtr, CFDSDiskFile & fileTarget, CFDSDiskFile & fileSource, const NES_RAM_MAP & nrmNew );
	NES_RAM_MAP							UpdateAddr( USHORT & ptr, USHORT & uFilePtr, CFDSDiskFile & fileTarget, const NES_RAM_MAP & nrmNew );
	NES_RAM_MAP							RevertAddr( USHORT & ptr, CFDSDiskFile & fileSource );

protected:	
	std::vector<NES_RAM_MAP>			m_vMAP;
	std::vector<BYTE>					m_vRAM;

	CNESMap( CFDSFile & file, size_t u );

	virtual VOID FileMapped( CFDSDiskFile & file, const NES_RAM_MAP & fileId );
	virtual USHORT FileMapping( const FDS_FILE_HEADER_BLOCK & fhb ) PURE;

public:
	VOID MapFile( BYTE diskId, FDS_DISK_SIDE diskSide, BYTE fileId, CFDSDiskFile & file );
	VOID StoreFiles();
	PBYTE			Data();
	VOID			clear();
};

class CNESPPU: public CNESMap
{
	NES_TILE			m_tiles[ 2 ][ NES_TILES_COUNT ];
	BOOL				m_fReversed;

	USHORT				FileMapping( const FDS_FILE_HEADER_BLOCK & fhb );
	VOID				FileMapped( CFDSDiskFile & file, const NES_RAM_MAP & fileId );
	VOID				LoadTiles();

public:
	CNESPPU( CFDSFile & file );
	VOID	DrawTile( CNesBitmap & bm, DWORD x, DWORD y, BYTE tileId, BYTE bAttribute, BOOL fSprite, BOOL fFlipHorz, BOOL fFlipVert );
	VOID	ReverseMode( BOOL fReverse );
};

class CNESRAM: public CNESMap
{
	USHORT FileMapping( const FDS_FILE_HEADER_BLOCK & fhb );

public:
	CNESRAM( CFDSFile & file );
	BOOL		CheckAccess( USHORT ref, size_t uSize );
	BYTE		GetFileIdFromPtr( USHORT uPtr );
};

class CNESFile
{
	NES_RAM_MAP				m_nrm;
	CFDSFile				m_file;
	CNESRAM					m_ram;
	CNESPPU					m_ppu;

	VOID					MapRAM();
	VOID					StoreRAM();

public:
	CNESFile();

	BOOL					LoadFile( LPCTSTR pszFile );
	BOOL					SaveFile( LPCTSTR pszFile );
	BOOL					SaveFile();
	BOOL					IsFileLoaded() const;
	CString					Filename();
	BOOL					EnumDisks( std::map<BYTE, BYTE> & mDisks );
	VOID					SelectDisk( BYTE diskId, FDS_DISK_SIDE side );
	VOID					EnumFiles( std::vector<BYTE> & vFiles );
	VOID					SelectFile( BYTE uFileId );

	VOID					DrawTile( CNesBitmap & bm, DWORD x, DWORD y, BYTE tileId, BYTE bAttribute, BOOL fSprite, BOOL fFlipHorz, BOOL fFlipVert );
	VOID					ReversedPPU( BOOL fReversed );
	BYTE					GetFileIdFromPtr( USHORT ref );
	VOID					InitBootFiles();

	template<typename T> T & Data( UINT ref )
	{
		if ( !m_ram.CheckAccess(ref, sizeof(T) ) )
			throw std::exception( "Invalid pointer operation" );

		PBYTE pRet = &m_ram.Data()[ ref - NES_FDS_BEGIN ];

		return *( (T*)(pRet) );
	}
};