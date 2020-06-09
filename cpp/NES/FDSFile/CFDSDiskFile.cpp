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


CFDSDiskFile::CFDSDiskFile()
{

}

VOID CFDSDiskFile::LoadFile( CFDSStream & stream )
{
	if ( FileHeaderBlock != stream.Read<BYTE>() )
	{
		throw std::exception( "Not a vaild file header" );
	}

	m_flInfo = stream.Read<FDS_FILE_HEADER_BLOCK>();

	if ( FileDataBlock != stream.Read<BYTE>() )
	{
		throw std::exception( "Not a valid file data header" );
	}

	m_vData.clear();
	m_vData.insert( m_vData.end(), m_flInfo.uFileSize, 0 );
	stream.ReadBlock( m_vData );
}

VOID CFDSDiskFile::DumpFile( CFDSStream & stream )
{
	stream.Write( FileHeaderBlock );
	stream.Write( m_flInfo );
	stream.Write( FileDataBlock );
	stream.WriteBlock( m_vData );
}

BYTE CFDSDiskFile::GetFileId()
{
	return m_flInfo.uFileId;
}

BYTE CFDSDiskFile::GetFileNumber()
{
	return m_flInfo.uFileNumber;
}

CString CFDSDiskFile::GetFileName()
{
	if ( !m_strName.GetLength() )
	{
		char strNameA[ 9 ] = { 0 };
		memcpy( strNameA, m_flInfo.strName, sizeof( m_flInfo.strName ) );
		m_strName = strNameA;
	}

	return m_strName;
}

FDS_FILE_HEADER_BLOCK CFDSDiskFile::GetFileData()
{
	return m_flInfo;
}

BYTE & CFDSDiskFile::operator[]( size_t u )
{
	return m_vData[ u ];
}