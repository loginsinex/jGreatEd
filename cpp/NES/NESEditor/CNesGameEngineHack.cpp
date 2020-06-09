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

#include "h\NES\NESEditor\NESEditor.h"

CNesGameEngineHack::CNesGameEngineHack( CNESFile & file, CNesPointers & eptr )
	: m_file( file ), m_eptr( eptr )
{

}

VOID CNesGameEngineHack::LoadData()
{
	m_data.coins = m_file.Data<NES_COINS_1UP_WORLD>( m_eptr[ eCoinsFor1Up ].ptr );
	m_data.warp = m_file.Data<NES_WARP_WORLD>( m_eptr.Pointer( eWarpWorlds ) );
	m_data.rotateCols = m_file.Data<NES_COIN_PAL_ROTATE_COLOR>( m_eptr.Pointer( ePalRotateColors ) );

	m_eptr.Select( eHardModeWorld );
	m_data.hardMode.bWorld = m_file.Data<BYTE>( m_eptr[ eHardModeWorld ].ptr );
	m_data.hardMode.bLevel = m_file.Data<BYTE>( m_eptr[ eHardModeLevel ].ptr );

	m_eptr.Select( eBowserHammersWorld );
	m_data.bowserHammers = m_file.Data< NES_BOWSER_HAMMERS_WORLD>( m_eptr[ eBowserHammersWorld ].ptr );

	m_data.defaultEggBehavior = !IsSpinyEggPatched();
}

VOID CNesGameEngineHack::DumpData()
{
	m_file.Data<NES_COINS_1UP_WORLD>( m_eptr[ eCoinsFor1Up ].ptr ) = m_data.coins;
	m_file.Data<NES_WARP_WORLD>( m_eptr.Pointer( eWarpWorlds ) ) = m_data.warp;
	m_file.Data<NES_COIN_PAL_ROTATE_COLOR>( m_eptr.Pointer( ePalRotateColors ) ) = m_data.rotateCols;

	m_eptr.Select( eHardModeWorld );
	m_file.Data<BYTE>( m_eptr[ eHardModeWorld ].ptr ) = m_data.hardMode.bWorld;
	m_file.Data<BYTE>( m_eptr[ eHardModeLevel ].ptr ) = m_data.hardMode.bLevel;

	m_eptr.Select( eBowserHammersWorld );
	m_file.Data< NES_BOWSER_HAMMERS_WORLD>( m_eptr[ eBowserHammersWorld ].ptr ) = m_data.bowserHammers;
	SetSpinyEggPatch( !m_data.defaultEggBehavior );
}

BOOL CNesGameEngineHack::IsSpinyEggPatched()
{
	return !(
		0x20 == m_file.Data<BYTE>( m_eptr[ eSpinyEggPatchPlace ].ptr ) &&		// JSR SmallBBox on its own place at original
		m_eptr[ eSmallBBox ].ptr == m_file.Data<USHORT>( m_eptr[ eSpinyEggPatchPlace ].ptr + 1 )
		);
}

VOID CNesGameEngineHack::SetSpinyEggPatch( BOOL fPatch )
{
	USHORT uPatchPtr = m_eptr[ eSpinyEggPatchPlace ].ptr;

	if ( fPatch )
	{
		m_file.Data<USHORT>( uPatchPtr ) = 0x02A0;							// LDY #02
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x5895;							// STA     vExplosionGfxCounter,X
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x00C9;							// CMP     #00
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x0130;							// BMI     lbSpinyRte
		uPatchPtr += 2;
		m_file.Data<BYTE>( uPatchPtr++ ) = 0x88;							// DEY
		m_file.Data<BYTE>( uPatchPtr++ ) = 0x20;							// JSR ...
		m_file.Data<USHORT>( uPatchPtr ) = m_eptr[ eSmallBBox ].ptr;		// ... SmallBBox
	}
	else
	{
		m_file.Data<BYTE>( uPatchPtr++ ) = 0x20;							// JSR ...
		m_file.Data<USHORT>( uPatchPtr ) = m_eptr[ eSmallBBox ].ptr;		// ... SmallBBox
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x02A0;							// LDY #02
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x5895;							// STA     vExplosionGfxCounter,X
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x00C9;							// CMP     #00
		uPatchPtr += 2;
		m_file.Data<USHORT>( uPatchPtr ) = 0x0130;							// BMI     lbSpinyRte
		uPatchPtr += 2;
		m_file.Data<BYTE>( uPatchPtr++ ) = 0x88;							// DEY
	}
}

const NES_ENGINE_HACK & CNesGameEngineHack::Read() const
{
	return m_data;
}

VOID CNesGameEngineHack::Write( const NES_ENGINE_HACK & hack )
{
	m_data = hack;
}