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

#include "Controls.h"

std::map<HINSTANCE, std::map<DWORD, tstring>> g_mStrList;
DWORD g_idRef = 0;

BOOL CALLBACK CSysString::_EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	std::vector<LPCTSTR> * pvResList = (std::vector<LPCTSTR>*) lParam;
	
	pvResList->push_back( lpszName );
	return TRUE;
}

void CSysString::LoadInstanceStrings(HINSTANCE hInstance)
{
	if ( !hInstance)
		return;

	HRSRC hResInfo = NULL;
	HGLOBAL hStringSeg = NULL;
	std::vector<LPCTSTR> vResList;
	int cch = 0;
	LPTSTR lpsz = 0;

	EnumResourceNames( 
		hInstance, 
		RT_STRING, 
		CSysString::_EnumResNameProc, 
		(ULONG_PTR) &vResList 
	);
    

	if ( vResList.size() < 1 )
		return;

	std::map<DWORD,tstring> & strings = m_lStrList[ hInstance ];
    /*
     * String Tables are broken up into 16 string segments.  Find the segment
     * containing the string we are interested in.
     */

	for(size_t n = 0; n < vResList.size(); ++n)
	{
		LPCTSTR szRes = vResList[ n ];

		if (hResInfo = FindResource(hInstance, szRes, RT_STRING))
		{
			/*
			 * Load that segment.
			 */
			hStringSeg = LoadResource(hInstance, hResInfo);

			/*
			 * Lock the resource.
			 */
			if (lpsz = (LPTSTR)LockResource(hStringSeg)) 
			{

				/*
				 * Move past the other strings in this segment.
				 * (16 strings in a segment -> & 0x0F)
				 */
				int counter = 0;
				for(int idr = 0; idr <= 0x0F; ++idr)
				{
					cch = *((TCHAR *)lpsz++);       // PASCAL like string count
													// first UTCHAR is count if TCHARs
					if ( !cch ) continue;
					
					DWORD dwStrId = idr | ( ( LODWORD( vResList[ n ] ) - 1 ) << 4 );

					std::vector<TCHAR> vStr( cch + 1, 0 );
					CopyMemory( vStr.data(), lpsz, cch * sizeof(TCHAR) );
					
					strings[ dwStrId ] = vStr.data();
					
					lpsz += cch;                    // Step to start if next string
				}

				UnlockResource( lpsz );
			}
		}
	}
}

CSysString::CSysString(BOOL fPrivate)
	: m_fPrivate( fPrivate ), m_lStrList( fPrivate ? m_mStrList : g_mStrList )
{
	if ( !m_fPrivate )
		g_idRef++;
}

CSysString::~CSysString()
{
	if ( !m_fPrivate )
		g_idRef--;
}

tstring CSysString::GetString(HINSTANCE hInstance, DWORD idStr)
{
	tstring sClearStr;

	std::map<HINSTANCE, std::map<DWORD,tstring>>::iterator v = m_lStrList.find( hInstance );

	if ( v == m_lStrList.end() )
	{
		LoadInstanceStrings( hInstance );
		v = m_lStrList.find( hInstance );
		if ( v == m_lStrList.end() )
			return sClearStr;
	}

	std::map<DWORD,tstring> & mStrList = v->second;
	std::map<DWORD,tstring>::iterator vStr = mStrList.find( idStr );

	if ( vStr != mStrList.end() )
		return vStr->second;
		
	return sClearStr;
}

BOOL CSysString::GetStringsList(HINSTANCE hInstance, std::map<DWORD, tstring> & strings)
{
	std::map<HINSTANCE, std::map<DWORD,tstring>>::iterator v = m_lStrList.find( hInstance );
	if ( v != m_lStrList.end() )
	{
		strings = v->second;
		return TRUE;
	}
	else
	{
		LoadInstanceStrings( hInstance );

		if ( m_lStrList.find( hInstance ) != m_lStrList.end() )
		{
			strings = m_lStrList[ hInstance ];
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CSysString::FreeStringsList(HINSTANCE hInstance)
{
	std::map<HINSTANCE, std::map<DWORD,tstring>>::iterator v = m_lStrList.find( hInstance );	
	if ( v != m_lStrList.end() )
	{
		m_lStrList.erase( v );
		return TRUE;
	}
	return FALSE;
}