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

CNESRAM::CNESRAM( CFDSFile & file )
	: CNESMap( file, NES_RAM_SIZE )
{
	
}

USHORT CNESRAM::FileMapping( const FDS_FILE_HEADER_BLOCK & fhb )
{
	return fhb.uTargetPtr - NES_FDS_BEGIN;
}

BOOL CNESRAM::CheckAccess( USHORT ref, size_t uSize )
{
	size_t uBeginPtr = size_t( ref ), uEndPtr = uBeginPtr + uSize - 1;
	if ( uBeginPtr < NES_FDS_BEGIN || uEndPtr >= NES_FDS_BIOS )
	{
		return FALSE;
	}

	for ( size_t u = uBeginPtr; u < uEndPtr; ++u )
	{
		if ( m_vMAP[ u - NES_FDS_BEGIN ].bEmptyFlag )
		{
			return FALSE;
		}
	}

	return TRUE;
}

BYTE CNESRAM::GetFileIdFromPtr( USHORT uPtr )
{
	if ( uPtr >= NES_FDS_BEGIN && uPtr < NES_FDS_BIOS )
	{
		uPtr -= NES_FDS_BEGIN;
		if ( !m_vMAP[ uPtr ].bEmptyFlag )
		{
			return m_vMAP[ uPtr ].fileId;
		}
	}

	return 0xFF;
}