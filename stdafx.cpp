#include "stdafx.h"

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

__declspec( noinline )
void * __cdecl operator new( size_t size )
{
	PVOID pParam = GlobalAlloc( GMEM_ZEROINIT | GMEM_FIXED, size );

	if ( !pParam )
		FatalAppExit( 0, TEXT( "Can't allocate memory\n\nProgram will abnormal closed" ) );

	return (void *)pParam;
}

__declspec( noinline )
void operator delete( void* const block, size_t const )
{

}

__declspec( noinline )
void __cdecl operator delete( void *ptr )
{
	GlobalFree( ptr );
}

__declspec( noinline )
void __cdecl operator delete[]( void *ptr )
{
	GlobalFree( ptr );
}