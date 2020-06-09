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

class CEngineHackDlg: public CDialog
{
	NES_ENGINE_HACK			& m_hack;
	CControl				m_ctlFrame;

protected:
	NES_ENGINE_HACK &		Hack();
	VOID					Changed();

public:
	CEngineHackDlg( HINSTANCE hInstance, LPCTSTR pszTitle, NES_ENGINE_HACK & hack );
	virtual ~CEngineHackDlg();
};


class CEngineHackSheet
{
	const HINSTANCE							m_hInstance;
	std::vector<CEngineHackDlg*>			m_vpDialog;
	NES_ENGINE_HACK							m_hack;

	HINSTANCE								GetInstance();

	CEngineHackDlg &						pushdlg( CEngineHackDlg * pDlg );
	VOID									InitDialogs();
	VOID									FreeDialogs();

public:
	CEngineHackSheet( HINSTANCE hInstance );
	~CEngineHackSheet();

	VOID									Set_Param( const NES_ENGINE_HACK & hack );
	VOID									Get_Param( NES_ENGINE_HACK & hack );

	INT_PTR									Show( CWindow * pParent );
};

// coins

class CEngineHackCoins1UP: public CEngineHackDlg
{
	CStaticControl							m_stWorld;
	CEditControl							m_edWorld;
	CUpDownControl							m_udWorld;

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineHackCoins1UP( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineHackWarpZones: public CEngineHackDlg
{
	CStaticControl							m_stZone[ 11 ];
	CControl								m_cbWorld[ 11 ];

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineHackWarpZones( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineHackCoinColors: public CEngineHackDlg
{
	CButtonControl			m_btnCol[ 6 ];

	VOID					InitColor( USHORT i, BYTE bCol );
	BOOL					OnInit( LPARAM lParam ) override;
	VOID					OnNCDestroy() override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnButton( USHORT uId ) override;

public:
	CEngineHackCoinColors( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineHackHardLevel: public CEngineHackDlg
{
	CStaticControl			m_stWorld;
	CControl				m_cbWorld;
	CStaticControl			m_stLevel;
	CEditControl			m_edLevel;
	CUpDownControl			m_udLevel;

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineHackHardLevel( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineBowserHammers: public CEngineHackDlg
{
	CStaticControl			m_stWorld;
	CControl				m_cbWorld;

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineBowserHammers( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineTimerValues: public CEngineHackDlg
{
	CStaticControl			m_stTimerSet[ 4 ];
	CControl				m_cbTimerSet[ 4 ];

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineTimerValues( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};

class CEngineSpinyEggBehavior: public CEngineHackDlg
{
	CStaticControl			m_stBehavior;
	CControl				m_cbBehavior;

	BOOL					OnInit( LPARAM lParam ) override;
	BOOL					PSOnApply( BOOL fOkPressed ) override;
	VOID					OnCommand( USHORT uCmd, USHORT uId, HWND hCtl ) override;

public:
	CEngineSpinyEggBehavior( HINSTANCE hInstance, NES_ENGINE_HACK & hack );
};