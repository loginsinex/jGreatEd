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

CNesPatch::CNesPatch( CNESFile & file, CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr )
{

}

BOOL CNesPatch::IsROMPatched()
{
	const USHORT uFindAreaPointerPtr = m_eptr.Pointer( eFindAreaPointerPtr );
	const USHORT uFL05PaddingPtr = m_eptr[ eFL05PaddingPtr ].ptr;
	return ( uFL05PaddingPtr == uFindAreaPointerPtr );
}

VOID CNesPatch::MakeLoopPatch()
{
	/*	:: EXACTLY 7 BYTES ::
		JSR FindAreaPointer
		STA $NES_J_VAR_STORE_AREA
		RTS
	*/
	USHORT & uFindAreaPointerPtr = m_eptr.Pointer( eFindAreaPointerPtr );
	USHORT uFL05PaddingPtr = m_eptr[ eFL05PaddingPtr ].ptr;
	const USHORT uLDAWorldIdCode = m_eptr[ eLoopLdaWorldCodePtr ].ptr;

	const BYTE bCode[] =
	{
		0x20, LOBYTE( uFindAreaPointerPtr ), HIBYTE( uFindAreaPointerPtr ),		// JSR RealFindAreaPointer
		0x8D, LOBYTE( NES_J_VAR_STORE_AREA ), HIBYTE( NES_J_VAR_STORE_AREA ),	// STA $06D8 (for loop command purposes)
		0x60																	// RTS
	};

	uFindAreaPointerPtr = uFL05PaddingPtr;

	for ( const BYTE & code : bCode )
	{
		m_file.Data<BYTE>( uFL05PaddingPtr++ ) = code;
	}

	m_file.Data<USHORT>( uLDAWorldIdCode ) = NES_J_VAR_STORE_AREA;	// write $06D8 instead of vWorldNumber[$075F] variable

	// all worlds numbers will be changed to area pointers while saving ROM
	LoadLoopRewindData();
}

VOID CNesPatch::LoadRequiredFiles( NES_AREA_SET as )
{
	USHORT uFileListPtrLo = m_eptr.Pointer( eFilesListPtrLo );
	USHORT uFileListPtrHi = m_eptr.Pointer( eFilesListPtrHi );
	USHORT uFileListPtr = MAKEWORD( m_file.Data<BYTE>( uFileListPtrLo + as ), m_file.Data<BYTE>( uFileListPtrHi + as ) );
	BYTE bFileId = 0x00;

	while ( 0xFF != ( bFileId = m_file.Data<BYTE>( uFileListPtr++ ) ) )
	{
		m_file.SelectFile( bFileId );
	}
}

USHORT CNesPatch::LoadLastSectionData()
{
	m_adData.uDataPtr[ 0 ] = m_eptr.Pointer( eEnemyAddrHOffset );
	m_adData.uDataPtr[ 1 ] = m_eptr.Pointer( eEnemyPtr );
	m_adData.uDataPtr[ 2 ] = m_eptr.Pointer( eLevelAddrHOffset );
	m_adData.uDataPtr[ 3 ] = m_eptr.Pointer( eLevelPtr );
	CopyMemory( m_adData.bEnemyHOffset, m_file.Data<BYTE[ nltTypeCount ]>( m_adData.uDataPtr[ 0 ] ), nltTypeCount );
	CopyMemory( m_adData.bAreaHOffset, m_file.Data<BYTE[ nltTypeCount ]>( m_adData.uDataPtr[ 2 ] ), nltTypeCount );
	size_t cPtrCount = ( m_adData.uDataPtr[ 2 ] - m_adData.uDataPtr[ 1 ] ) / sizeof( USHORT );

	for ( size_t n = 0; n < cPtrCount; ++n )
	{
		m_adData.vEnemyPtr.push_back( m_file.Data<USHORT>( m_adData.uDataPtr[ 1 ] ) ); m_adData.uDataPtr[ 1 ] += sizeof( USHORT );
		m_adData.vAreaPtr.push_back( m_file.Data<USHORT>( m_adData.uDataPtr[ 3 ] ) ); m_adData.uDataPtr[ 3 ] += sizeof( USHORT );
	}

	return m_adData.uDataPtr[ 0 ];
}

VOID CNesPatch::StoreLastSectionData( USHORT uPtr )
{
	USHORT & uEnemyAddrHOffset = m_eptr.Pointer( eEnemyAddrHOffset );
	USHORT & uEnemyPtrLo = m_eptr.Pointer( eEnemyPtr );
	USHORT & uEnemyPtrHi = m_eptr.Pointer( eEnemyPtrHi );
	USHORT & uAreaAddrHOffset = m_eptr.Pointer( eLevelAddrHOffset );
	USHORT & uAreaPtrLo = m_eptr.Pointer( eLevelPtr );
	USHORT & uAreaPtrHi = m_eptr.Pointer( eLevelPtrHi );
	
	uEnemyAddrHOffset = uPtr; uPtr += nltTypeCount;
	CopyMemory( m_file.Data<BYTE[ nltTypeCount ]>( uEnemyAddrHOffset ), m_adData.bEnemyHOffset, nltTypeCount );
	uEnemyPtrLo = uPtr;
	uEnemyPtrHi = uPtr + 1;

	for ( const auto & ptr : m_adData.vEnemyPtr )
	{
		m_file.Data<USHORT>( uPtr ) = ptr;
		uPtr += sizeof( USHORT );
	}

	uAreaAddrHOffset = uPtr; uPtr += nltTypeCount;
	CopyMemory( m_file.Data<BYTE[ nltTypeCount ]>( uAreaAddrHOffset ), m_adData.bAreaHOffset, nltTypeCount );
	uAreaPtrLo = uPtr;
	uAreaPtrHi = uPtr + 1;

	for ( const auto & ptr : m_adData.vAreaPtr )
	{
		m_file.Data<USHORT>( uPtr ) = ptr;
		uPtr += sizeof( USHORT );
	}

	m_adData.uDataPtr[ 0 ] = uEnemyAddrHOffset;
	m_adData.uDataPtr[ 1 ] = uEnemyPtrLo;
	m_adData.uDataPtr[ 2 ] = uAreaAddrHOffset;
	m_adData.uDataPtr[ 3 ] = uAreaPtrLo;
}

VOID CNesPatch::LoadLoopRewindData()
{
	const size_t uMaximumLoopCount = m_file.Data<BYTE>( m_eptr[ eLoopCount ].ptr );
	USHORT uPageRewindPtr = m_eptr.Pointer( eLoopAreaDataOffsetPtr );
	m_adData.vLoopRewindData.clear();
	m_adData.vLoopRewindData.reserve( uMaximumLoopCount );
	for ( size_t n = 0; n < uMaximumLoopCount; ++n )
	{
		m_adData.vLoopRewindData.push_back( m_file.Data<BYTE>( uPageRewindPtr++ ) );
	}
}

USHORT CNesPatch::StoreLoopRewindData()
{
	USHORT uPageRewindPtr = m_eptr.Pointer( eLoopAreaDataOffsetPtr );
	for ( const auto & b : m_adData.vLoopRewindData )
	{
		m_file.Data<BYTE>( uPageRewindPtr++ ) = b;
	}

	return uPageRewindPtr;
}

VOID CNesPatch::MakeWorldsADPatch()
{
	LoadRequiredFiles( asWorldAtoD );
	NES_WORLD_A_D_HWP_DATA hwp = m_file.Data<NES_WORLD_A_D_HWP_DATA>( m_eptr[ eHalfwayADData ].ptr );
	USHORT uNewPtr = LoadLastSectionData();
	hwp.bProc[ 3 ] = LOBYTE( uNewPtr );
	hwp.bProc[ 4 ] = HIBYTE( uNewPtr );
	m_eptr.Pointer( eHalfwayADCallPtr ) = uNewPtr + _ARRAYSIZE( hwp.bHalfwayData );
	m_file.Data<NES_WORLD_A_D_HWP_DATA>( uNewPtr ) = hwp;
	uNewPtr = StoreLoopRewindData();
	StoreLastSectionData( uNewPtr );
}

BOOL CNesPatch::DoPatch()
{
	m_file.InitBootFiles();
	if ( IsROMPatched() )
	{
		return FALSE;
	}

	MakeLoopPatch();
	MakeWorldsADPatch();
	return TRUE;
}

/*
sbrScrollLockObject_Warp:

X=0x80

if ( !vHardWorldFlag )
{
	if( !vWorldNumber )
	{
		if ( 1 == vAreaType )
		{
			X++;
		}
		else if ( vAreaId > 0 )
		{
			X += 2;
		}
	}
	else
	{
		X = 0x83;
		if ( vWorldNumber != #World_3 )
		{
			X++;
			if ( vWorldNumber == #World_5 )
			{
				if ( vAreaId != 0x0B )
				{
					if ( vAreaType ) X++;
					else X +=2;
				}
			}
			else X += 3;
		}
	}
	vWarpZoneCtl = X;
}
else
{
	vWarpZoneCtl = 0x87+vLevelNumber;
}

///////////////////
World_1
{
	vAreaType == 1:					vWarpZoneCtl = 0x81;
	vAreaType != 1 && vAreaId > 0: 	vWarpZoneCtl = 0x82;
	vAreaType != 1 && vAreaId == 0:	vWarpZoneCtl = 0x80;
}
World_2
World_4
World_6
World_7
World_8
World_9
{
	vWarpZoneCtl = 0x87
}
World_3
{
	vWarpZoneCtl = 0x83
}
World_5
{
	vAreaId == 0x0B:					vWarpZoneCtl = 0x84;
	vAreaId != 0x0B && 1 != vAreaType:	vWarpZoneCtl = 0x85;
	vAreaId != 0x0B && 1 == vAreaType:	vWarpZoneCtl = 0x86;
}
World_A:
World_B:
World_C:
World_D:
{
	vWarpZoneCtl = 0x87 + vLevelNumber;
}

*/