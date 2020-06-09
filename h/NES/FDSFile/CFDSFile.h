#pragma once

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

#include "NES/FDSFile/FDStruct.h"

#define		FDS_STREAM_READ		1
#define		FDS_STREAM_WRITE	2

typedef enum _tagFDS_DISK_SIDE : BYTE
{
	fdsSideA,
	fdsSideB,
	fdsSIDES
} FDS_DISK_SIDE, *PFDS_DISK_SIDE;

class CFDSStream
{
	HANDLE							m_hFile;

public:
	CFDSStream( LPCTSTR pszFile = nullptr, DWORD streamAccess = FDS_STREAM_READ )
		: m_hFile( INVALID_HANDLE_VALUE )
	{
		if ( pszFile )
		{
			if ( OpenFile( pszFile, streamAccess ) )
			{
				throw std::exception( "File cannot be opened" );
			}
		}
	}

	~CFDSStream()
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			CloseHandle( m_hFile );
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	DWORD							OpenFile( LPCTSTR pszFile, DWORD streamAccess )
	{
		DWORD genericFlags = 0, openFlags = 0;

		if ( m_hFile != INVALID_HANDLE_VALUE )
		{
			return ERROR_ALREADY_EXISTS;
		}

		if ( FDS_STREAM_READ == ( FDS_STREAM_READ & streamAccess ) )
		{
			openFlags = OPEN_EXISTING;
			genericFlags |= GENERIC_READ;
		}

		if ( FDS_STREAM_WRITE == ( FDS_STREAM_WRITE & streamAccess ) )
		{
			openFlags = CREATE_ALWAYS;
			genericFlags |= GENERIC_WRITE;
		}

		m_hFile = CreateFile( pszFile, genericFlags, FILE_SHARE_READ, nullptr, openFlags, 0, nullptr );

		if ( INVALID_HANDLE_VALUE == m_hFile )
		{
			return GetLastError();
		}

		return 0;
	}

	LARGE_INTEGER					Seek( LARGE_INTEGER ptr, DWORD seek )
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			LARGE_INTEGER li = { 0 };
			if ( SetFilePointerEx( m_hFile, ptr, &li, seek ) )
			{
				return li;
			}
		}

		throw std::exception( "Seek file failed" );
	}

	VOID							SetEOF()
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			SetFileValidData( m_hFile, CurrentPointer().QuadPart );
			SetEndOfFile( m_hFile );
		}

		throw std::exception( "Set EOF failed" );
	}

	LARGE_INTEGER					CurrentPointer()
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			LARGE_INTEGER ptr = { 0 };
			return Seek( ptr, SEEK_CUR );
		}

		throw std::exception( "Get file pointer failed" );
	}

	template<class T>				T Read()
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			T data;
			DWORD dwLength = 0;
			if ( ReadFile( m_hFile, &data, sizeof( data ), &dwLength, nullptr ) && dwLength == sizeof( data ) )
			{
				return data;
			}
		}

		throw std::exception( "File cannot be read" );
	}

	VOID		ReadBlock( std::vector<BYTE> & data )
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			DWORD dwLength = 0;
			if ( ReadFile( m_hFile, data.data(), DWORD( data.size() & 0xFFFFFFFF ), &dwLength, nullptr ) && dwLength == data.size() )
			{
				return;
			}
		}

		throw std::exception( "File cannot be read" );
	}

	VOID		WriteBlock( const std::vector<BYTE> & data )
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			DWORD dwLength = 0;
			if ( WriteFile( m_hFile, data.data(), DWORD( data.size() & 0xFFFFFFFF ), &dwLength, nullptr ) && dwLength == data.size() )
			{
				return;
			}
		}

		throw std::exception( "File cannot be written" );
	}

	template<class T>				VOID Write( const T & data )
	{
		if ( INVALID_HANDLE_VALUE != m_hFile )
		{
			DWORD dwLength = 0;
			if ( WriteFile( m_hFile, &data, sizeof( data ), &dwLength, nullptr ) && dwLength == sizeof( data ) )
			{
				return;
			}
		}

		throw std::exception( "File cannot be written" );
	}
};

class CFDSDiskFile
{
	FDS_FILE_HEADER_BLOCK							m_flInfo;
	CString											m_strName;
	std::vector<BYTE>								m_vData;
	
public:
	CFDSDiskFile();
	VOID											LoadFile( CFDSStream & stream );
	VOID											DumpFile( CFDSStream & stream );
	BYTE											GetFileId();
	BYTE											GetFileNumber();
	CString											GetFileName();
	FDS_FILE_HEADER_BLOCK							GetFileData();
	BYTE & operator[]( size_t u );
};

class CFDSDiskSide
{
	FDS_DISK_INFO_BLOCK								m_fdsInfo;
	std::vector<CFDSDiskFile>						m_vFile;
	size_t											m_uSelected;

public:
	CFDSDiskSide();
	VOID											LoadFiles( CFDSStream & stream );
	VOID											DumpFiles( CFDSStream & stream );
	CFDSDiskFile &									SelectFile( BYTE uId );
	CFDSDiskFile &									File();
	FDS_DISK_SIDE									Side();
	VOID											EnumFiles( std::vector<FDS_FILE_HEADER_BLOCK> & vFiles );
	FDS_DISK_INFO_BLOCK								GetSideInfo();
};

class CFDSDisk
{
	std::map<FDS_DISK_SIDE, CFDSDiskSide>			m_mSide;
	FDS_DISK_SIDE									m_fdsSelectedSide;

public:
	CFDSDisk();
	VOID								LoadDiskSide( CFDSStream & stream );
	CFDSDiskSide &						SelectSide( FDS_DISK_SIDE fds );
	CFDSDiskSide &						Side();

	VOID								DumpDisk( CFDSStream & stream, FDS_DISK_SIDE side = fdsSideA );
	BYTE								GetSidesCount();
};

class CFDSFile
{
	CString								m_strFilename;
	std::map<BYTE, CFDSDisk>			m_vDisk;
	BYTE								m_uSelectedDisk;
	BOOL								m_fLoaded;
	BOOL								m_fAloneDisk;

	VOID								LoadNextDisk( CFDSStream & stream );

public:
	CFDSFile();
	DWORD					LoadFile( CString strFilename );
	DWORD					SaveFile();
	DWORD					SaveFile( CString strFilename );
	BOOL					IsFileLoaded() const;
	CString					Filename();

	size_t					DiskCount();
	BYTE					GetSidesCount();

	CFDSDisk			&	SelectDisk( BYTE id );
	CFDSDisk			&	Disk();
	VOID					EnumDisks( std::map<BYTE, BYTE> & mDisk );
};