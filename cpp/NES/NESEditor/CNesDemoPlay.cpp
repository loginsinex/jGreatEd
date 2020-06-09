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

#include "h/NES/NESEditor/NESEditor.h"

CNesDemoPlay::CNesDemoPlay( CNESFile & file, CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr )
{

}

VOID CNesDemoPlay::LoadData()
{
	USHORT uDemoTimingDataPtr = m_eptr.Pointer( eDemoTimingArray ) + 1;
	USHORT uDemoActionDataPtr = m_eptr.Pointer( eDemoActionArray ) + 1;
	m_vDemoData.clear();

	for(size_t n=0;n< NES_J_SIZE_OF_DEMO_DATA;++n)
	{
		BYTE b = m_file.Data<BYTE>( uDemoTimingDataPtr++ );
		BYTE act = m_file.Data<BYTE>( uDemoActionDataPtr++ );
		m_vDemoData.push_back( { b, act } );
	}
}

VOID CNesDemoPlay::DumpData()
{
	USHORT uDemoTimingDataPtr = m_eptr.Pointer( eDemoTimingArray ) + 1;
	USHORT uDemoActionDataPtr = m_eptr.Pointer( eDemoActionArray ) + 1;

	if ( m_vDemoData.size() > NES_J_SIZE_OF_DEMO_DATA )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of demo data array.\nRequested %Iu items, but available %d.", m_vDemoData.size(), NES_J_SIZE_OF_DEMO_DATA );
		throw std::exception( strError );
	}

	for ( const auto & act : m_vDemoData )
	{
		const BYTE bTimer = act.first;
		const BYTE bAction = act.second;
		m_file.Data<BYTE>( uDemoTimingDataPtr++ ) = bTimer;
		m_file.Data<BYTE>( uDemoActionDataPtr++ ) = bAction;
	}

	m_file.Data<BYTE>( uDemoTimingDataPtr++ ) = 0;
	//m_file.Data<BYTE>( uDemoActionDataPtr++ ) = 0;
}

VOID CNesDemoPlay::DemoPlayData( std::vector<std::pair<BYTE, BYTE>>& vDemoData )
{
	vDemoData = m_vDemoData;
}

VOID CNesDemoPlay::WriteDemoPlayData( const std::vector<std::pair<BYTE, BYTE>>& vDemoData )
{
	if ( vDemoData.size() < 255 )
	{
		m_vDemoData = vDemoData;
	}
}