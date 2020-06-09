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

CFDSFile::CFDSFile()
	: m_uSelectedDisk( -1 ), m_fLoaded( FALSE ), m_fAloneDisk( FALSE )
{

}

VOID CFDSFile::LoadNextDisk( CFDSStream & stream )
{
	LARGE_INTEGER ptr = stream.CurrentPointer();

	if ( DiskInfoBlock != stream.Read<FDS_BLOCK_TYPE>() )
	{
		throw std::exception( "Not a disk info block found as expected" );
	}

	FDS_DISK_INFO_BLOCK fdsib = stream.Read<FDS_DISK_INFO_BLOCK>();
	stream.Seek( ptr, SEEK_SET );
	m_vDisk[ fdsib.uDiskNumber ].LoadDiskSide( stream );
}

DWORD CFDSFile::LoadFile( CString strFilename )
{
	CFDSStream stream;

	if ( m_fLoaded )
	{
		return ERROR_ALREADY_EXISTS;
	}

	DWORD err = stream.OpenFile( strFilename, FDS_STREAM_READ );
	if ( !err )
	{
		FDS_HEADER fdsh = stream.Read<FDS_HEADER>();
		if ( FDS_FILE_SIGNATURE != fdsh.dwSignature )
		{
			// just one disk
			LARGE_INTEGER li = { 0 };
			stream.Seek( li, SEEK_SET );
			LoadNextDisk( stream );
			m_fAloneDisk = TRUE;
		}
		else
		{
			for ( size_t n = 0; n < fdsh.uDiskSides; ++n )
			{
				LoadNextDisk( stream );
			}
		}

		m_strFilename = strFilename;
		m_fLoaded = TRUE;
	}

	return err;
}

BOOL CFDSFile::IsFileLoaded() const
{
	return m_fLoaded;
}

CString CFDSFile::Filename()
{
	return m_strFilename;
}

DWORD CFDSFile::SaveFile()
{
	return SaveFile( m_strFilename );
}

DWORD CFDSFile::SaveFile( CString strFilename )
{
	if ( !m_fLoaded )
	{
		return ERROR_FILE_NOT_FOUND;
	}

	CFDSStream stream;
	DWORD err = stream.OpenFile( strFilename, FDS_STREAM_WRITE );

	if ( !err )
	{
		if ( m_fAloneDisk && 1 == DiskCount() )
		{
			auto disk = m_vDisk.begin();
			disk->second.DumpDisk( stream );
		}
		else
		{
			FDS_HEADER fdsh = { 0 };
			fdsh.dwSignature = FDS_FILE_SIGNATURE;
			fdsh.uDiskSides = GetSidesCount();
			stream.Write( fdsh );
			m_fAloneDisk = FALSE;

			// write sideA
			for ( auto disk : m_vDisk )
			{
				disk.second.DumpDisk( stream, fdsSideA );
			}

			// write sideB
			for ( auto disk : m_vDisk )
			{
				disk.second.DumpDisk( stream, fdsSideB );
			}			
		}
		m_strFilename = strFilename;
	}

	return err;
}

size_t CFDSFile::DiskCount()
{
	return m_vDisk.size();
}

BYTE CFDSFile::GetSidesCount()
{
	BYTE bSidesCount = 0;
	for ( auto & disk : m_vDisk )
	{
		bSidesCount += disk.second.GetSidesCount();
	}

	return bSidesCount;
}

CFDSDisk & CFDSFile::SelectDisk( BYTE id )
{
	auto disk = m_vDisk.find( id );
	if ( disk != m_vDisk.end() )
	{
		m_uSelectedDisk = id;
		return disk->second;
	}

	throw std::exception( "No such disk" );
}

CFDSDisk & CFDSFile::Disk()
{
	auto disk = m_vDisk.find( m_uSelectedDisk );
	if ( disk != m_vDisk.end() )
	{
		return disk->second;
	}

	throw std::exception( "No disk selected" );
}

VOID CFDSFile::EnumDisks( std::map<BYTE, BYTE>& mDisk )
{
	for ( auto & disk : m_vDisk )
	{
		mDisk[ disk.first ] = disk.second.GetSidesCount();
	}
}
