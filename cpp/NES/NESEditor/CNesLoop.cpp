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

CNesLoop::CNesLoop( CNESFile & file, CNesPointers & ptr )
	: m_file( file ), m_eptr( ptr )
{

}

VOID CNesLoop::Load()
{
	USHORT uAreaPtr = m_eptr.Pointer( eLoopAreaNumberPtr );
	USHORT uPagePtr = m_eptr.Pointer( eLoopPageNumberPtr );
	USHORT uHeightPtr = m_eptr.Pointer( eLoopYPtr );
	USHORT uSlaveDataPtr = m_eptr.Pointer( eLoopSlaveDataPtr );
	USHORT uPageRewindPtr = m_eptr.Pointer( eLoopAreaDataOffsetPtr );
	const size_t uMaximumLoopCount = m_file.Data<BYTE>( m_eptr[ eLoopCount ].ptr );

	m_vLoop.clear();
	for ( USHORT i = 0; i < uMaximumLoopCount; ++i )
	{
		NES_LOOP lpc =
		{
			m_file.Data<NES_AREA_PTR>( uAreaPtr++ ),
			m_file.Data<BYTE>( uPagePtr++ ),
			m_file.Data<BYTE>( uHeightPtr++ ),
			m_file.Data<BYTE>( uSlaveDataPtr++ ),
			m_file.Data<BYTE>( uPageRewindPtr++ )
		};

		m_vLoop.push_back( lpc );
	}
}

VOID CNesLoop::Flush()
{
	m_vLoop.clear();
}

VOID CNesLoop::Dump()
{
	const size_t uMaximumLoopCount = m_file.Data<BYTE>( m_eptr[ eLoopCount ].ptr );

	if ( m_vLoop.size() > uMaximumLoopCount )
	{
		char strError[ 256 ];
		sprintf_s( strError, "Exceeded maximum of loop commands.\nFound %Iu commands, but available %Iu.", m_vLoop.size(), uMaximumLoopCount );
		throw std::exception( strError );
	}

	size_t c = m_vLoop.size();
	USHORT uAreaPtr = m_eptr.Pointer( eLoopAreaNumberPtr );
	USHORT uPagePtr = m_eptr.Pointer( eLoopPageNumberPtr );
	USHORT uHeightPtr = m_eptr.Pointer( eLoopYPtr );
	USHORT uSlaveDataPtr = m_eptr.Pointer( eLoopSlaveDataPtr );
	USHORT uPageRewindPtr = m_eptr.Pointer( eLoopAreaDataOffsetPtr );
	const NES_LOOP lpcFree = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	
	

	for ( USHORT i = 0; i < uMaximumLoopCount; ++i )
	{
		const NES_LOOP & lpc = ( i < c ? m_vLoop[ i ] : lpcFree );
		_tprintf( TEXT( "* Writing Loop: MLC=%d, aptr=%d, Page=0x%02x, Y=0x%02x, Rewind=0x%02x\n" ),
			lpc.bSlaveData, lpc.aptr.bPtr, lpc.bPageNumber, lpc.bHeight, lpc.bPageRewind );

		m_file.Data<NES_AREA_PTR>( uAreaPtr++ ) = lpc.aptr;
		m_file.Data<BYTE>( uPagePtr++ ) = lpc.bPageNumber;
		m_file.Data<BYTE>( uHeightPtr++ ) = lpc.bHeight;
		m_file.Data<BYTE>( uSlaveDataPtr++ ) = lpc.bSlaveData;
		m_file.Data<BYTE>( uPageRewindPtr++ ) = lpc.bPageRewind;		
	}
}

VOID CNesLoop::GetLoops( std::vector<NES_LOOP>& vLoops )
{
	vLoops = m_vLoop;
}

VOID CNesLoop::SetLoops( const std::vector<NES_LOOP>& vLoops )
{
	m_vLoop.insert( m_vLoop.end(), vLoops.begin(), vLoops.end() );
}

BOOL CNesLoop::GetAreaLoops( NES_AREA_PTR aptr, std::vector<NES_LOOP>& vLoops )
{
	vLoops.reserve( m_vLoop.size() );
	for ( const auto & lpc : m_vLoop )
	{
		if ( lpc.aptr == aptr )
		{
			vLoops.push_back( lpc );
		}
	}

	return ( vLoops.size() > 0 );
}

/*
Original engine code (in pseudo-language):

	if ( vLoopCommand && vCurrentColumnPos )
	{
		Y = 0x0c;
		while( Y >= 0 )
		{
			Y--;
			if (
				vWorld == pLoopWorld[Y] &&
				vCurrentPage == pLoopPage[Y]
				)
			{
				if (
					playerY == pLoopY[Y] &&
					playerState == 0
					)
				{
					vMultiLoopCorrect++;
				}

				vMultiLoopPass++;
				if ( vMultiLoopPass == pMultiLoop[Y] )
				{
					if ( vMultiLoopCorrect != pMultiLoop[Y] )
					{
						ExecGameLoopback();
						KillAllEnemies();
					}

					vMultiLoopPass = vMultiLoopCorrect = 0;
				}

				vLoopCommand = 0;
			}
		}
	}
*/