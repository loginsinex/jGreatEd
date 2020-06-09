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

CFDSDisk::CFDSDisk()
	: m_fdsSelectedSide( fdsSIDES )
{

}

VOID CFDSDisk::LoadDiskSide( CFDSStream & stream )
{
	CFDSDiskSide fdsSide;
	LARGE_INTEGER ptr = stream.CurrentPointer();
	fdsSide.LoadFiles( stream );	
	if ( m_mSide.find( fdsSide.Side() ) == m_mSide.end() )
	{
		m_mSide[ fdsSide.Side() ] = fdsSide;
	}
	ptr.QuadPart += FDS_SIDE_SIZE;
	stream.Seek( ptr, SEEK_SET );
}

VOID CFDSDisk::DumpDisk( CFDSStream & stream, FDS_DISK_SIDE side )
{
	LARGE_INTEGER ptr = stream.CurrentPointer();
	auto fdsSide = m_mSide.find( side );
	if ( fdsSide != m_mSide.end() )
	{
		LARGE_INTEGER curPtr;
		fdsSide->second.DumpFiles( stream );
		curPtr = stream.CurrentPointer();

		if ( curPtr.QuadPart - ptr.QuadPart > FDS_SIDE_SIZE )
		{
			// truncate side!
			curPtr.QuadPart = ptr.QuadPart + FDS_SIDE_SIZE;
			stream.Seek( curPtr, SEEK_SET );
			stream.SetEOF();
		}
		else
		{
			// write GAP
			size_t uGAPSize = ptr.QuadPart + FDS_SIDE_SIZE - curPtr.QuadPart;
			std::vector<BYTE> vGAP( uGAPSize, 0 );
			stream.WriteBlock( vGAP );
		}
	}
}

BYTE CFDSDisk::GetSidesCount()
{
	return LOBYTE( m_mSide.size() );
}

CFDSDiskSide & CFDSDisk::SelectSide( FDS_DISK_SIDE fds )
{
	auto side = m_mSide.find( fds );
	if ( side != m_mSide.end() )
	{
		m_fdsSelectedSide = fds;
		return side->second;
	}

	throw std::exception( "No such side for disk" );
}

CFDSDiskSide & CFDSDisk::Side()
{
	auto side = m_mSide.find( m_fdsSelectedSide );
	if ( side != m_mSide.end() )
	{
		return side->second;
	}

	throw std::exception( "No side selected for disk" );
}