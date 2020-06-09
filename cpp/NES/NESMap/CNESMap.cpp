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

#include "NES/NESMap/CNESFile.h"

CNESMap::CNESMap( CFDSFile & file, size_t u)
	: m_file( file ), m_vMAP( u, 0xFFFFFFFF ), m_vRAM( u, 0 )
{

}

VOID CNESMap::FileMapped( CFDSDiskFile & file, const NES_RAM_MAP & fileId )
{
	// do nothing. Callback for derived class
}

NES_RAM_MAP CNESMap::UpdateAddr( USHORT & ptr, USHORT & uFilePtr, CFDSDiskFile & fileTarget, CFDSDiskFile & fileSource, const NES_RAM_MAP & nrmNew )
{
	size_t uSize = m_vMAP.size(), uFileSize = fileTarget.GetFileData().uFileSize, uFileSourceSize = fileSource.GetFileData().uFileSize;
	NES_RAM_MAP nrm = m_vMAP[ ptr ];
	USHORT uSourcePtr = ptr - FileMapping( fileSource.GetFileData() );
	while ( ptr < uSize && uFilePtr < uFileSize && uSourcePtr < uFileSourceSize && nrm.id == m_vMAP[ ptr ].id )
	{
		fileSource[ uSourcePtr++ ] = m_vRAM[ ptr ];
		m_vRAM[ ptr ] = fileTarget[ uFilePtr++ ];
		m_vMAP[ ptr ] = nrmNew;
		ptr++;
	}

	if ( ptr < uSize )
	{
		return m_vMAP[ ptr ];
	}

	return NES_RAM_MAP( 0xFFFFFFFF );
}

NES_RAM_MAP CNESMap::UpdateAddr( USHORT & ptr, USHORT & uFilePtr, CFDSDiskFile & fileTarget, const NES_RAM_MAP & nrmNew )
{
	size_t uSize = m_vMAP.size(), uFileSize = fileTarget.GetFileData().uFileSize;
	NES_RAM_MAP nrm = m_vMAP[ ptr ];
	while ( ptr < uSize && uFilePtr < uFileSize && nrm.id == m_vMAP[ ptr ].id )
	{
		m_vRAM[ ptr ] = fileTarget[ uFilePtr++ ];
		m_vMAP[ ptr ] = nrmNew;
		ptr++;
	}

	if ( ptr < uSize )
	{
		return m_vMAP[ ptr ];
	}

	return NES_RAM_MAP( 0xFFFFFFFF );
}

NES_RAM_MAP CNESMap::RevertAddr( USHORT & ptr, CFDSDiskFile & fileSource )
{
	size_t uSize = m_vMAP.size(), uFileSize = fileSource.GetFileData().uFileSize;
	NES_RAM_MAP nrm = m_vMAP[ ptr ];
	USHORT uSourcePtr = ptr - FileMapping( fileSource.GetFileData() );
	while ( ptr < uSize && uSourcePtr < uFileSize && nrm.id == m_vMAP[ ptr ].id )
	{
		fileSource[ uSourcePtr++ ] = m_vRAM[ ptr ];
		ptr++;
	}

	if ( ptr < uSize )
	{
		return m_vMAP[ ptr ];
	}

	return NES_RAM_MAP( 0xFFFFFFFF );
}

VOID CNESMap::MapFile( BYTE diskId, FDS_DISK_SIDE diskSide, BYTE fileId, CFDSDiskFile & file )
{
	FDS_FILE_HEADER_BLOCK fhb = file.GetFileData();
	USHORT uBeginPtr = FileMapping( fhb ), uFilePtr = 0;
	size_t uEndPtr = DWORD( uBeginPtr ) + DWORD( fhb.uFileSize );
	NES_RAM_MAP nrmNew( 0 );
	nrmNew.bEmptyFlag = 0;
	nrmNew.diskId = diskId;
	nrmNew.diskSide = diskSide;
	nrmNew.fileId = fileId;

	if ( DWORD( uBeginPtr ) + DWORD( fhb.uFileSize ) <= m_vMAP.size() )
	{
		NES_RAM_MAP nrm = m_vMAP[ uBeginPtr ];

		while ( uBeginPtr < uEndPtr )
		{
			if ( !nrm.bEmptyFlag )
			{
				nrm = UpdateAddr( uBeginPtr, uFilePtr, file, m_file.SelectDisk( nrm.diskId ).SelectSide( nrm.diskSide ).SelectFile( nrm.fileId ), nrmNew );
			}
			else
			{
				nrm = UpdateAddr( uBeginPtr, uFilePtr, file, nrmNew );
			}
		}
	}

	FileMapped( file, nrmNew );
}

VOID CNESMap::StoreFiles()
{
	size_t uSize = m_vMAP.size();
	USHORT uPtr = 0;
	NES_RAM_MAP nrm = m_vMAP[ 0 ];

	while ( uPtr < uSize )
	{
		if ( !nrm.bEmptyFlag )
		{
			nrm = RevertAddr( uPtr, m_file.SelectDisk( nrm.diskId ).SelectSide( nrm.diskSide ).SelectFile( nrm.fileId ) );
		}
		else
		{
			nrm = m_vMAP[ uPtr++ ];
		}
	}
}

PBYTE CNESMap::Data()
{
	return m_vRAM.data();
}

VOID CNESMap::clear()
{
	size_t uSize = m_vRAM.size();
	m_vRAM.clear(); m_vMAP.clear();
	m_vRAM.insert( m_vRAM.end(), uSize, 0 );
	m_vMAP.insert( m_vMAP.end(), uSize, 0xFFFFFFFF );
}

// nes bitmap

CNesBitmap::CNesBitmap()
{
	m_bm.sz.cx = m_bm.sz.cy = 0;
	m_bm.fInit = FALSE;
}

VOID CNesBitmap::Create( int cx, int cy )
{
	if ( cx > 0 && cy > 0 )
	{
		Destroy();
		m_bm.sz.cx = cx;
		m_bm.sz.cy = cy;
		m_bm.vBitmap.insert( m_bm.vBitmap.end(), cx * cy, { 0 } );
		m_bm.fInit = TRUE;
	}
}

VOID CNesBitmap::Destroy()
{
	if ( m_bm.fInit )
	{
		m_bm.sz.cx = m_bm.sz.cy = 0;
		m_bm.fInit = FALSE;
		m_bm.vBitmap.clear();
	}
}

PNES_PIXEL CNesBitmap::Bits() const
{
	if ( m_bm.fInit )
	{
		return const_cast<PNES_PIXEL>( m_bm.vBitmap.data() );
	}

	return nullptr;
}

VOID CNesBitmap::BlitTile( const NES_TILE & tile, int x, int y, BYTE bAttr )
{
	const RECT rcSrc = { x, y, x + NES_TILE_WIDTH - 1, y + NES_TILE_HEIGHT - 1 };
	const RECT rcTgt = { 0, 0, Width() - 1, Height() - 1 };
	RECT rc = { 0 };

	if ( IntersectRect( &rc, &rcTgt, &rcSrc ) )
	{
		UnsafeBlitTile( tile, rc, bAttr, FALSE, Bits(), FALSE, FALSE );
	}
}

VOID CNesBitmap::BlitSprite( const NES_TILE & tile, int x, int y, BYTE bAttr, BOOL fFlipHorz, BOOL fFlipVert )
{
	const RECT rcSrc = { x, y, x + NES_TILE_WIDTH - 1, y + NES_TILE_HEIGHT - 1 };
	const RECT rcTgt = { 0, 0, Width() - 1, Height() - 1 };
	RECT rc = { 0 };

	if ( IntersectRect( &rc, &rcTgt, &rcSrc ) )
	{
		UnsafeBlitTile( tile, rc, bAttr, TRUE, Bits(), fFlipHorz, fFlipVert );
	}
}

VOID CNesBitmap::UnsafeBlitTile( const NES_TILE & tile, const RECT & rc, BYTE bAttr, BOOL fSprite, PNES_PIXEL pBits, BOOL fFlipHorz, BOOL fFlipVert )
{
	bAttr &= 3;

	const int im_x = rc.left;
	const int im_y = rc.top;
	const int im_r = rc.right;
	const int im_b = rc.bottom;
	int tx = 0, ty = 0;
	PNES_PIXEL ppixY = pBits + im_y * Width() + im_x;

	if ( !fSprite )
	{
		for ( int iy = im_y; iy <= im_b; ++iy, ++ty )
		{
			PNES_PIXEL ppixX = ppixY;
			tx = 0;
			for ( int ix = im_x; ix <= im_r; ++ix, ++tx )
			{
				NES_PIXEL & pix = *( ppixX++ );
				pix.bAttr = bAttr;
				pix.bColor = tile.pix[ ty ][ tx ];
				pix.fSprite = FALSE;
				pix.bExtended = 0;
			}

			ppixY += Width();
		}
	}
	else
	{
		if ( !fFlipHorz && !fFlipVert )
		{
			for ( int iy = im_y; iy <= im_b; ++iy, ++ty )
			{
				PNES_PIXEL ppixX = ppixY;
				tx = 0;
				for ( int ix = im_x; ix <= im_r; ++ix, ++tx )
				{
					NES_PIXEL & pix = *( ppixX++ );
					const BYTE & bColor = tile.pix[ ty ][ tx ];
					pix.fSprite = TRUE;

					if ( bColor )
					{
						pix.bAttr = bAttr;
						pix.bColor = bColor;
						pix.bExtended = 0;
					}
				}

				ppixY += Width();
			}
		}
		else if ( !fFlipHorz && fFlipVert )
		{
			ty = NES_TILE_HEIGHT - 1;
			for ( int iy = im_y; iy <= im_b; ++iy, --ty )
			{
				PNES_PIXEL ppixX = ppixY;
				tx = 0;
				for ( int ix = im_x; ix <= im_r; ++ix, ++tx )
				{
					NES_PIXEL & pix = *( ppixX++ );
					const BYTE & bColor = tile.pix[ ty ][ tx ];
					pix.fSprite = TRUE;

					if ( bColor )
					{
						pix.bAttr = bAttr;
						pix.bColor = bColor;
						pix.bExtended = 0;
					}
				}

				ppixY += Width();
			}
		}
		else if ( fFlipHorz && !fFlipVert )
		{
			for ( int iy = im_y; iy <= im_b; ++iy, ++ty )
			{
				PNES_PIXEL ppixX = ppixY;
				tx = NES_TILE_WIDTH - 1;
				for ( int ix = im_x; ix <= im_r; ++ix, --tx )
				{
					NES_PIXEL & pix = *( ppixX++ );
					const BYTE & bColor = tile.pix[ ty ][ tx ];
					pix.fSprite = TRUE;

					if ( bColor )
					{
						pix.bAttr = bAttr;
						pix.bColor = bColor;
						pix.bExtended = 0;
					}
				}

				ppixY += Width();
			}
		}
		else
		{
			ty = NES_TILE_HEIGHT - 1;
			for ( int iy = im_y; iy <= im_b; ++iy, --ty )
			{
				PNES_PIXEL ppixX = ppixY;
				tx = NES_TILE_WIDTH - 1;
				for ( int ix = im_x; ix <= im_r; ++ix, --tx )
				{
					NES_PIXEL & pix = *( ppixX++ );
					const BYTE & bColor = tile.pix[ ty ][ tx ];
					pix.fSprite = TRUE;

					if ( bColor )
					{
						pix.bAttr = bAttr;
						pix.bColor = bColor;
						pix.bExtended = 0;
					}
				}

				ppixY += Width();
			}
		}
	}
}

int CNesBitmap::Width() const
{
	return LODWORD( m_bm.sz.cx );
}

int CNesBitmap::Height() const
{
	return LODWORD( m_bm.sz.cy );
}

VOID CNesBitmap::Blit( CNesBitmap & bmSource, int x, int y, int width, int height, int src_x, int src_y )
{
	BlitRaw( (PBYTE)bmSource.Bits(), x, y, width, height, src_x, src_y, bmSource.Width(), bmSource.Height() );
}

VOID CNesBitmap::BlitRaw( PBYTE pSourceBits, int x, int y, int width, int height, int src_x, int src_y, int src_width, int src_height )
{
	PNES_PIXEL pBits = (PNES_PIXEL)pSourceBits;
	const RECT rcTgt = { x, y, x + width - 1, y + height - 1 };
	const RECT rcSrc = { src_x, src_y, src_x + width - 1, src_y + height - 1 };
	const RECT rcFullTgt = { 0, 0, Width() - 1, Height() - 1 };
	const RECT rcFullSrc = { 0, 0, src_width - 1, src_height - 1 };

	RECT rcSrcVal, rcTgtVal;

	if ( IntersectRect( &rcSrcVal, &rcFullSrc, &rcSrc ) && IntersectRect( &rcTgtVal, &rcFullTgt, &rcTgt ) )
	{
		const int im_src_x = rcSrcVal.left;
		const int im_src_y = rcSrcVal.top;
		const int im_src_w = rcSrcVal.right - rcSrcVal.left + 1;
		const int im_src_h = rcSrcVal.bottom - rcSrcVal.top + 1;
		const int im_tgt_x = rcTgtVal.left;
		const int im_tgt_y = rcTgtVal.top;
		const int im_tgt_w = rcTgtVal.right - rcTgtVal.left + 1;
		const int im_tgt_h = rcTgtVal.bottom - rcTgtVal.top + 1;
		const int im_w = min( im_src_w, im_tgt_w );
		const int im_h = min( im_src_h, im_tgt_h );
		const int im_src_r = im_src_x + im_w - 1;
		const int im_src_b = im_src_y + im_h - 1;
		const int im_tgt_r = im_tgt_x + im_w - 1;
		const int im_tgt_b = im_tgt_y + im_h - 1;

		PNES_PIXEL pBitsSrcY = pBits + im_src_y * src_width + im_src_x;
		PNES_PIXEL pBitsTgtY = Bits() + im_tgt_y * Width() + im_tgt_x;

		for ( int ty = im_tgt_y; ty <= im_tgt_b; ++ty )
		{
			PNES_PIXEL pBitsSrcX = pBitsSrcY;
			PNES_PIXEL pBitsTgtX = pBitsTgtY;

			for ( int tx = im_tgt_x; tx <= im_tgt_r; ++tx )
			{
				NES_PIXEL & src = *( pBitsSrcX++ );
				NES_PIXEL & tgt = *( pBitsTgtX++ );

				if ( src.fSprite && src.bColor || !src.fSprite || src.bExtended )
				{
					tgt.bRawColor = src.bRawColor;
				}
			}

			pBitsSrcY += Width();
			pBitsTgtY += src_width;
		}
	}
}

VOID CNesBitmap::RBlitRaw( 
	PBYTE pTargetBits, 
	int x, int y, int width, int height, 
	int tgt_x, int tgt_y, int target_width, int target_height, 
	BOOL fFlipHorz, BOOL fFlipVert 
) const
{
	PNES_PIXEL pBits = (PNES_PIXEL)pTargetBits;
	const RECT rcSrc = { x, y, x + width - 1, y + height - 1 };
	const RECT rcTgt = { tgt_x, tgt_y, tgt_x + width - 1, tgt_y + height - 1 };
	const RECT rcFullSrc = { 0, 0, Width() - 1, Height() - 1 };
	const RECT rcFullTgt = { 0, 0, target_width - 1, target_height - 1 };

	RECT rcSrcVal, rcTgtVal;

	if ( IntersectRect( &rcSrcVal, &rcFullSrc, &rcSrc ) && IntersectRect( &rcTgtVal, &rcFullTgt, &rcTgt ) )
	{
		const int im_src_x = rcSrcVal.left;
		const int im_src_y = rcSrcVal.top;
		const int im_src_w = rcSrcVal.right - rcSrcVal.left + 1;
		const int im_src_h = rcSrcVal.bottom - rcSrcVal.top + 1;
		const int im_tgt_x = rcTgtVal.left;
		const int im_tgt_y = rcTgtVal.top;
		const int im_tgt_w = rcTgtVal.right - rcTgtVal.left + 1;
		const int im_tgt_h = rcTgtVal.bottom - rcTgtVal.top + 1;
		const int im_w = min( im_src_w, im_tgt_w );
		const int im_h = min( im_src_h, im_tgt_h );
		const int im_src_r = im_src_x + im_w - 1;
		const int im_src_b = im_src_y + im_h - 1;
		const int im_tgt_r = im_tgt_x + im_w - 1;
		const int im_tgt_b = im_tgt_y + im_h - 1;

		PNES_PIXEL pBitsTgtY = pBits + ( im_tgt_y + ( fFlipVert ? im_tgt_h - 1 : 0 ) ) * target_width + im_tgt_x + ( fFlipHorz ? im_tgt_w - 1 : 0 );
		PNES_PIXEL pBitsSrcY = Bits() + im_src_y * Width() + im_src_x;
		ptrdiff_t pTgtIncY = ( fFlipVert ? -1 : 1 ) * target_width;
		ptrdiff_t pTgtIncX = ( fFlipHorz ? -1 : 1 );

		for ( int ty = im_tgt_y; ty <= im_tgt_b; ++ty )
		{
			PNES_PIXEL pBitsSrcX = pBitsSrcY;
			PNES_PIXEL pBitsTgtX = pBitsTgtY;

			for ( int tx = im_tgt_x; tx <= im_tgt_r; ++tx )
			{
				NES_PIXEL & src = *( pBitsSrcX++ );
				NES_PIXEL & tgt = *( pBitsTgtX );

				if ( src.fSprite && src.bColor || !src.fSprite || src.bExtended )
				{
					tgt.bRawColor = src.bRawColor;
				}

				pBitsTgtX += pTgtIncX;
			}

			pBitsSrcY += Width();
			pBitsTgtY += pTgtIncY;
		}
	}
}