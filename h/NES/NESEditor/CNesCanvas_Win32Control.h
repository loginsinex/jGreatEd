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

class CCanvasCtl : public CControl
{
	CNesCanvas *				m_pCanvas;

	VOID		PostCreate() override;

public:
	CCanvasCtl( HINSTANCE hInstance, USHORT uId,
		INT x, INT y, INT cx, INT cy,
		DWORD dwStyle, DWORD dwStyleEx = 0, WORD wAnchor = 0 );

	BOOL		GetSelectedObject( std::vector<CNesObject*> & vpObject );
	VOID		DropSelection();
	POINT		GetLastPoint();
	VOID		SetView( CNesLevelInterface * pInterface );
	CNesLevelInterface * GetView();
	VOID		DelView();
	VOID		RenderView();
	VOID		ScrollTo( BYTE uTargetPage );
	VOID		SetSelection( std::vector<CNesObject*> & vpObj );
};