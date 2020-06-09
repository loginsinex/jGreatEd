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

#include "stdafx.h"

CLimitsDlg::CLimitsDlg( HINSTANCE hInstance )
	: CDialog( hInstance, 300, 300, TEXT( "Current limits" ), FALSE ),
	m_lv( hInstance, 0x100, 7, 7, -7, -( 7 + 15 + 7 ), WS_VISIBLE | WS_BORDER | LVS_NOSORTHEADER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS ),
	m_btnClose( hInstance, IDCANCEL, MB_GetString( IDCLOSE - 1 ).c_str(), -( 7 + 50 ), -( 7 + 15 ), 50, 15, TRUE )
{
	pushctl( m_lv );
	pushctl( m_btnClose );
}

VOID CLimitsDlg::Set_Param( const std::map<NES_AREA_SET, NES_LIMITS>& mLimits )
{
	m_mLimits = mLimits;
}

BOOL CLimitsDlg::OnInit( LPARAM lParam )
{
	size_t uSharedCurrent = 0, uSharedMax = 0;
	CString strTitle[] =
	{
		TEXT( "Worlds 1-4" ),
		TEXT( "Worlds 5-8" ),
		TEXT( "World 9" ),
		TEXT( "Worlds A-D" ),
		TEXT( "Shared section" )
	};
	
	INT id[ asWorldSetsCount + 1 ] = { 0 };
	m_lv.AddColumn( TEXT( "Limit" ), 250 );
	m_lv.AddColumn( TEXT( "Current" ), 100 );
	m_lv.AddColumn( TEXT( "Available" ) );
	m_lv.ExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );
	m_lv.EnableGroups( TRUE );

	for ( const auto & lim : m_mLimits )
	{
		const auto & as = lim.first;
		const auto & lm = lim.second;

		id[ as ] = LODWORD( m_lv.AddGroup( as, strTitle[ as ] ) );

		InsertLimits( id[ as ], lm );
		uSharedCurrent = max( uSharedCurrent, lm.nSharedSection );
		uSharedMax = max( uSharedMax, lm.nMaximumSharedSection );
	}

	id[ asWorldSetsCount ] = LODWORD( m_lv.AddGroup( asWorldSetsCount, strTitle[ asWorldSetsCount ] ) );
	InsertSharedSection( id[ asWorldSetsCount ], uSharedCurrent, uSharedMax );

	return FALSE;
}

VOID CLimitsDlg::InsertLimits( INT iGroupId, const NES_LIMITS & as )
{
	INT id = LODWORD( m_lv.AddItem( TEXT( "Section size" ), 0, iGroupId ) );
	m_lv.SetItem( id, 1, LODWORD( as.uDataSize ) );
	m_lv.SetItem( id, 2, LODWORD( as.nMaximumDataSize ) );

	id = LODWORD( m_lv.AddItem( TEXT( "Areas count" ), 0, iGroupId ) );
	m_lv.SetItem( id, 1, LODWORD( as.nAreasSlots ) );
	m_lv.SetItem( id, 2, LODWORD( as.nMaxAreasSlots ) );

	id = LODWORD( m_lv.AddItem( TEXT( "Worlds slots" ), 0, iGroupId ) );
	m_lv.SetItem( id, 1, LODWORD( as.uAreas ) );
	m_lv.SetItem( id, 2, LODWORD( as.nMaxAreas ) );
}

VOID CLimitsDlg::InsertSharedSection( INT iGroupId, size_t uCurrent, size_t uMax )
{
	INT id = LODWORD( m_lv.AddItem( TEXT( "Section size" ), 0, iGroupId ) );
	m_lv.SetItem( id, 1, LODWORD( uCurrent ) );
	m_lv.SetItem( id, 2, LODWORD( uMax ) );
}