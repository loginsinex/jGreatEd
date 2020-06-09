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

CFDSDiskSide::CFDSDiskSide()
	: m_uSelected( -1 )
{

}

VOID CFDSDiskSide::LoadFiles( CFDSStream & stream )
{
	if ( DiskInfoBlock != stream.Read<FDS_BLOCK_TYPE>() )
	{
		throw std::exception( "Not a valid disk header" );
	}

	m_fdsInfo = stream.Read<FDS_DISK_INFO_BLOCK>();
	if ( memcmp( m_fdsInfo.strDiskVerification, FDS_DISK_SIGNATURE, sizeof( m_fdsInfo.strDiskVerification ) ) )
	{
		throw std::exception( "Not a valid disk signature" );
	}

	if ( FileAmountBlock != stream.Read<FDS_BLOCK_TYPE>() )
	{
		throw std::exception( "Not a valid file amount block" );
	}

	FDS_FILE_AMOUNT_BLOCK fdsAmount = stream.Read<FDS_FILE_AMOUNT_BLOCK>();

	if ( !fdsAmount.uFileAmount )
	{
		throw std::exception( "Can't load empty disk" );
	}

	// load files
	for ( size_t u = 0; u < fdsAmount.uFileAmount; ++u )
	{
		CFDSDiskFile file;
		file.LoadFile( stream );
		m_vFile.push_back( file );
	}
}

VOID CFDSDiskSide::DumpFiles( CFDSStream & stream )
{
	stream.Write( DiskInfoBlock );
	stream.Write( m_fdsInfo );
	stream.Write( FileAmountBlock );
	stream.Write( LOBYTE( m_vFile.size() ) );

	for ( auto & file : m_vFile )
	{
		file.DumpFile( stream );
	}
}

FDS_DISK_SIDE CFDSDiskSide::Side()
{
	return static_cast<FDS_DISK_SIDE>( m_fdsInfo.uSideNumber );
}

CFDSDiskFile & CFDSDiskSide::SelectFile( BYTE uId )
{
	size_t uSelected = 0;
	for ( auto & fl : m_vFile )
	{
		if ( fl.GetFileId() == uId )
		{
			m_uSelected = uSelected;
			return fl;
		}

		uSelected++;
	}

	throw std::exception( "Can't find file specified" );
}

CFDSDiskFile & CFDSDiskSide::File()
{
	if ( m_uSelected < m_vFile.size() )
	{
		return m_vFile[ m_uSelected ];
	}

	throw std::exception( "No file selected" );
}

VOID CFDSDiskSide::EnumFiles( std::vector<FDS_FILE_HEADER_BLOCK>& vFiles )
{
	vFiles.clear();
	for ( auto file : m_vFile )
	{
		vFiles.push_back( file.GetFileData() );
	}
}

FDS_DISK_INFO_BLOCK CFDSDiskSide::GetSideInfo()
{
	return m_fdsInfo;
}