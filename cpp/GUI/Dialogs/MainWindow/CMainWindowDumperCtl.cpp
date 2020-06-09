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

VOID CMainWindow::DumpArea( LPCTSTR pszFile )
{
	std::vector<BYTE> bData, bLevel, bEnemy;
	m_cursel.pSublevel->GetLevelBinaryData( bLevel, bEnemy );
	bData.push_back( LOBYTE( bLevel.size() ) );
	bData.push_back( LOBYTE( bEnemy.size() ) );
	bData.insert( bData.end(), bLevel.begin(), bLevel.end() );
	bData.insert( bData.end(), bEnemy.begin(), bEnemy.end() );

	HANDLE hFile = CreateFile( pszFile, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, 0, nullptr );
	if ( INVALID_HANDLE_VALUE != hFile )
	{
		DWORD w;
		WriteFile( hFile, bData.data(), LODWORD( bData.size() ), &w, nullptr );
		CloseHandle( hFile );
	}
	else
	{
		ShowAlertF( TEXT( "Unable to create file '%s'" ), pszFile );
	}
}

VOID CMainWindow::LoadArea( LPCTSTR pszFile )
{
	std::vector<BYTE> bData, bLevel, bEnemy;
	std::vector<NES_LINK> vLink;
	HANDLE hFile = CreateFile( pszFile, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr );
	if ( INVALID_HANDLE_VALUE != hFile )
	{
		LARGE_INTEGER li = { 0 };
		if ( GetFileSizeEx( hFile, &li ) )
		{
			if ( li.QuadPart > 2 && li.QuadPart < 0x210 )
			{
				DWORD w;
				bData.insert( bData.end(), li.QuadPart, 0 );
				ReadFile( hFile, bData.data(), LODWORD( bData.size() ), &w, nullptr );

				if ( bData.size() >= bData[ 0 ] + bData[ 1 ] )
				{
					auto it = bData.begin() + 2;
					bLevel.insert( bLevel.end(), it, it + bData[ 0 ] );
					it += bData[ 0 ];
					bEnemy.insert( bEnemy.end(), it, it + bData[ 1 ] );

					if ( bLevel.size() > 0 && bEnemy.size() > 0 )
					{
						if ( bLevel.back() == 0xFD && bEnemy.back() == 0xFF )
						{
							bLevel.pop_back();
							bEnemy.pop_back();
							try
							{
								TakeLevelSnapshot();
								m_cursel.pSublevel->LoadLevelData( bLevel, bEnemy, vLink );
								m_cursel.pSublevel->InitObjects();
								LoadSubLevel( m_cursel.bWorld, m_cursel.pSublevel );
							}
							catch ( std::exception & e )
							{
								PopLevelSnapshot();
								UNREFERENCED_PARAMETER( e );
							}
						}
					}
				}
			}
		}
		CloseHandle( hFile );
	}
	else
	{
		ShowAlertF( TEXT( "Unable to open file '%s'" ), pszFile );
	}
}