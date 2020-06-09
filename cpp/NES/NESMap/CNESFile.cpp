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

CNESFile::CNESFile()
	: m_ram( m_file ), m_ppu( m_file ), m_nrm( 0 )
{

}

BOOL CNESFile::LoadFile( LPCTSTR pszFile )
{
	if ( m_file.IsFileLoaded() )
	{
		return FALSE;
	}

	DWORD err = m_file.LoadFile( pszFile );

	if ( !err )
	{
		MapRAM();
		return TRUE;
	}

	return FALSE;
}

BOOL CNESFile::SaveFile( LPCTSTR pszFile )
{
	if ( !m_file.IsFileLoaded() )
	{
		return FALSE;
	}

	StoreRAM();

	return !( pszFile ? m_file.SaveFile( pszFile ) : m_file.SaveFile() );
}

BOOL CNESFile::SaveFile()
{
	return SaveFile( m_file.Filename() );
}

BOOL CNESFile::IsFileLoaded() const
{
	return m_file.IsFileLoaded();
}

CString CNESFile::Filename()
{
	return m_file.Filename();
}

VOID CNESFile::MapRAM()
{
	std::vector<FDS_FILE_HEADER_BLOCK> vFiles;	
	CFDSDiskSide & side = m_file.SelectDisk( m_nrm.diskId ).SelectSide( m_nrm.diskSide );
	BYTE bMaximumBootId = side.GetSideInfo().uBootCode;
	side.EnumFiles( vFiles );

	for ( auto & file : vFiles )
	{
		if ( file.uFileId > bMaximumBootId )
		{
			continue;
		}
		else if ( CHR == file.fft )
		{
			m_ppu.MapFile( m_nrm.diskId, m_nrm.diskSide, file.uFileId, side.SelectFile( file.uFileId ) );
		}
		else if ( PRG == file.fft )
		{
			m_ram.MapFile( m_nrm.diskId, m_nrm.diskSide, file.uFileId, side.SelectFile( file.uFileId ) );
		}
	}
}

VOID CNESFile::StoreRAM()
{
	m_ppu.StoreFiles();
	m_ram.StoreFiles();
}

BOOL CNESFile::EnumDisks( std::map<BYTE, BYTE>& mDisks )
{
	mDisks.clear();
	m_file.EnumDisks( mDisks );
	return ( mDisks.size() > 0 );
}

VOID CNESFile::SelectDisk( BYTE diskId, FDS_DISK_SIDE side )
{
	if ( diskId != m_nrm.diskId || side != m_nrm.diskSide )
	{
		StoreRAM();
		m_ram.clear();
		m_ppu.clear();
		m_nrm.diskId = diskId;
		m_nrm.diskSide = side;
		MapRAM();
	}
}

VOID CNESFile::EnumFiles( std::vector<BYTE>& vFiles )
{
	std::vector<FDS_FILE_HEADER_BLOCK > vf;
	CFDSDiskSide & side = m_file.SelectDisk( m_nrm.diskId ).SelectSide( m_nrm.diskSide );
	side.EnumFiles( vf );
	vFiles.clear();
	for ( auto & file : vf )
	{
		vFiles.push_back( file.uFileId );
	}
}

VOID CNESFile::SelectFile( BYTE uFileId )
{
	CFDSDiskFile & file = m_file.SelectDisk( m_nrm.diskId ).SelectSide( m_nrm.diskSide ).SelectFile( uFileId );
	switch ( file.GetFileData().fft )
	{
		case CHR:
			{
				m_ppu.MapFile( m_nrm.diskId, m_nrm.diskSide, uFileId, file );
				break;
			}
		case PRG:
			{
				m_ram.MapFile( m_nrm.diskId, m_nrm.diskSide, uFileId, file );
				break;
			}
	}
}

VOID CNESFile::InitBootFiles()
{
	if ( m_file.IsFileLoaded() )
	{
		StoreRAM();
		MapRAM();
	}
}

BYTE CNESFile::GetFileIdFromPtr( USHORT ref )
{
	return m_ram.GetFileIdFromPtr( ref );
}

VOID CNESFile::DrawTile( CNesBitmap & bm, DWORD x, DWORD y, BYTE tileId, BYTE bAttribute, BOOL fSprite, BOOL fFlipHorz, BOOL fFlipVert )
{
	m_ppu.DrawTile( bm, x, y, tileId, bAttribute, fSprite, fFlipHorz, fFlipVert );
}

VOID CNESFile::ReversedPPU( BOOL fReversed )
{
	m_ppu.ReverseMode( fReversed );
}