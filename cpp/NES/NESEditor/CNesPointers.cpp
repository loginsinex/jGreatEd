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

#include "h/NES/NESEditor/CNesPointers.h"

CNesPointers::CNesPointers( CNESFile & fds )
	: m_file( fds )
{
	Version( ver_J );
}

VOID CNesPointers::Version( NES_VERSION id )
{
	m_uVersion = id;
	LoadPointers();
}

VOID CNesPointers::LoadSharedPointers()
{

}

VOID CNesPointers::LoadPointers()
{
	switch ( m_uVersion )
	{
		case ver_J: LoadPointersJ(); break;
		default: throw std::exception( "Unknown version" ); break;
	}
	LoadSharedPointers();
}

NES_EPOINTER CNesPointers::operator[]( NES_EPOINTERS eptr ) const
{
	auto ptr = m_eptr.find( eptr );
	if ( ptr == m_eptr.end() )
	{
		throw std::exception( "No such pointer" );
	}

	return ptr->second;
}

VOID CNesPointers::Select( NES_EPOINTERS eptr ) const
{
	if ( !m_file.IsFileLoaded() )
	{
		throw std::exception( "Trying to read from an empty file" );
	}

	auto e = this->operator[]( eptr );

	if ( e.bFileId != 0xFF )
	{
		const_cast<CNESFile*>( &m_file )->SelectFile( e.bFileId );
	}
}

USHORT & CNesPointers::Pointer( NES_EPOINTERS eptr ) const
{
	auto e = this->operator[]( eptr );
	Select( eptr );

	return m_file.Data<USHORT>( e.ptr );
}