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

std::map<UINT, HDC>			l_mDesktopDC;
std::map<UINT, INT_PTR>		l_mDesktopDCRef;

HDC & __get_thread_dc()
{
	return l_mDesktopDC[ GetCurrentThreadId() ];
}

INT_PTR & __get_thread_dc_ref()
{
	return l_mDesktopDCRef[ GetCurrentThreadId() ];
}

VOID __init_desktop_dc_allocator()
{
	HDC & hDC = __get_thread_dc();

	if ( !hDC )
	{
		hDC = GetDC( GetDesktopWindow() );
	}
}

VOID __free_desktop_dc_allocator()
{
	HDC & hDC = __get_thread_dc();
	if ( hDC )
	{
		// ReleaseDC( GetDesktopWindow(), hDC );
	}
	// hDC = nullptr;
}

HDC __alloc_desktop_dc()
{
	INT_PTR & ref = __get_thread_dc_ref();
	if ( !ref )
	{
		__init_desktop_dc_allocator();
	}

	ref++;
	return __get_thread_dc();
}

VOID __free_desktop_dc()
{
	INT_PTR & ref = __get_thread_dc_ref();

	if ( ref > 0 )
	{
		ref--;
		if ( ref <= 0 )
		{
			__free_desktop_dc_allocator();
		}
	}
}

CBitmap::CBitmap()
{
	m_fShadow = FALSE;
	m_bmAlphaMask = NULL;
	m_bmAlphaRestored = NULL;
	m_buffer.hDC = m_buffer.hDCDesktop = NULL;
	m_buffer.hBitmap = m_buffer.hDefaultBitmap = NULL;
	m_buffer.pvBuffer = NULL;
	m_buffer.sz.cx = m_buffer.sz.cy = 0;
	m_buffer.uSize = 0;
	m_buffer.fLocked = FALSE;
}

CBitmap::CBitmap(INT cx, INT cy)
{
	m_fShadow = FALSE;
	m_bmAlphaMask = NULL;
	m_bmAlphaRestored = NULL;

	m_buffer.fLocked = FALSE;
	if ( cx <= 0 || cy <= 0 )
		throw std::exception("Invalid size");

	Create(cx, cy);
}

CBitmap::CBitmap(const CBitmap & bm)
{
	m_fShadow = FALSE;
	m_bmAlphaMask = NULL;
	m_bmAlphaRestored = NULL;

	this->operator=(bm);
}

CBitmap::CBitmap(HINSTANCE hInstance, LPCTSTR pszResource)
{
	m_fShadow = FALSE;
	m_bmAlphaMask = NULL;
	m_bmAlphaRestored = NULL;

	m_buffer.fLocked = FALSE;
	if ( !CreateFromResource(hInstance, pszResource) )
		throw std::exception("Invalid bitmap resource");
}

CBitmap & CBitmap::operator=(const CBitmap & bm)
{
	if ( &bm == this )
		return *this;

	Create(bm.m_buffer.sz.cx, bm.m_buffer.sz.cy);
	CopyMemory(Bits(), bm.m_buffer.pvBuffer, m_buffer.uSize);
	return *this;
}

BOOL CBitmap::CreateFromResource(HINSTANCE hInstance, LPCTSTR lpBitmapName)
{
	HRSRC hRes = FindResource( hInstance, lpBitmapName, (LPCTSTR) RT_BITMAP);
	if ( hRes )
	{
		DWORD uSize = SizeofResource( hInstance, hRes );
		HGLOBAL hData = LoadResource( hInstance, hRes );
		if ( hData )
		{
			PVOID pvBits = LockResource( hData );
			PVOID pvBmpBits = PBYTE( pvBits ) + 40;
			INT x = 0, y = 0, bppxl = 0;
			x			= ((LPDWORD) pvBits)[1];
			y			= ((LPDWORD) pvBits)[2];
			bppxl		= ((USHORT*) pvBits)[7];


			if ( 32 == bppxl )
			{
				Create( x, y );

				if ( m_buffer.uSize > 0 )
				{
					RGBQUAD * pvBuffer = Bits();
					for(int i = 0; i < y; ++i)
						CopyMemory(&pvBuffer[i*x], &((RGBQUAD*) pvBmpBits)[(y - i - 1)*x], x * sizeof(RGBQUAD));
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


CBitmap::~CBitmap()
{
	DestroyShadowBuffer();
	Destroy();
}

VOID CBitmap::Create(INT cx, INT cy)
{
	Destroy();
	
	if ( !cx || !cy )
		return;

	m_buffer.hDCDesktop = __alloc_desktop_dc();

	if ( !m_buffer.hDCDesktop )
	{
		char sError[1024] = { 0 };
		_sprintf_p(sError, sizeof(sError), "Lock DC desktop failed: %d", GetLastError());
		throw std::exception( sError );
	}

	m_buffer.hDC = CreateCompatibleDC( m_buffer.hDCDesktop );

	if ( !m_buffer.hDC )
	{
		char sError[1024] = { 0 };
		_sprintf_p(sError, sizeof(sError), "DC creation failed: %d", GetLastError());
		throw std::exception( sError );
	}
	
	UINT uSize = m_buffer.uSize = sizeof(RGBQUAD)*cx*cy;

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi);
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = -cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = uSize;

	m_buffer.hBitmap = CreateDIBSection( m_buffer.hDCDesktop, &bmi, DIB_RGB_COLORS, (PVOID*) &m_buffer.pvBuffer, NULL, 0 );

	if ( !m_buffer.hBitmap )
	{
		char sError[1024] = { 0 };
		_sprintf_p(sError, sizeof(sError), "Bitmap creation failed: %d (%d:%d)", GetLastError(), cx, cy);
		throw std::exception( sError );
	}

#ifndef BITMAP_USE_LOCKDC
	m_buffer.hDefaultBitmap = (HBITMAP) SelectObject(m_buffer.hDC, m_buffer.hBitmap);
#endif

	m_buffer.sz.cx = cx;
	m_buffer.sz.cy = cy;
	__free_desktop_dc();
	m_buffer.hDCDesktop = nullptr;
	GdiFlush();
}

VOID CBitmap::Destroy()
{
	if ( m_buffer.uSize )
	{		
#ifndef BITMAP_USE_LOCKDC
		SelectObject(m_buffer.hDC, m_buffer.hDefaultBitmap);
#endif
		FreeDC();
		DeleteObject(m_buffer.hBitmap);
		DeleteDC(m_buffer.hDC);
		__free_desktop_dc();

		m_buffer.hDC = m_buffer.hDCDesktop = NULL;
		m_buffer.hBitmap = m_buffer.hDefaultBitmap = NULL;
		m_buffer.pvBuffer = NULL;
		m_buffer.sz.cx = m_buffer.sz.cy = 0;
		m_buffer.uSize = 0;
	}
}

VOID CBitmap::FreeDC()
{
	if ( m_buffer.fLocked )
	{
#ifdef BITMAP_USE_LOCKDC
		SelectObject(m_buffer.hDC, m_buffer.hDefaultBitmap);
#endif
		m_buffer.fLocked = FALSE;
	}

	GdiFlush();
}

HDC CBitmap::LockDC()
{
	if ( !m_buffer.fLocked )
	{
		m_buffer.fLocked = TRUE;
#ifdef BITMAP_USE_LOCKDC
		m_buffer.hDefaultBitmap = (HBITMAP) SelectObject(CheckDC(), m_buffer.hBitmap);
#endif
	}

	return CheckDC();
}

HDC CBitmap::CheckDC()
{
	if ( !GetBkMode( m_buffer.hDC ) )
	{
		if ( ERROR_INVALID_PARAMETER == GetLastError() )
		{
			ResetBitmapDC();
		}
	}

	return m_buffer.hDC;
}

VOID CBitmap::ResetBitmapDC()
{
	if ( m_buffer.uSize )
	{
#ifndef BITMAP_USE_LOCKDC
		SelectObject( m_buffer.hDC, m_buffer.hDefaultBitmap );
#else
		if ( m_buffer.fLocked )
		{
			SelectObject( m_buffer.hDC, m_buffer.hDefaultBitmap );
		}
#endif

		m_buffer.hDefaultBitmap = NULL;
		DeleteDC( m_buffer.hDC );
		m_buffer.hDC = CreateCompatibleDC( __alloc_desktop_dc() );
		__free_desktop_dc();

#ifndef BITMAP_USE_LOCKDC
		m_buffer.hDefaultBitmap = (HBITMAP) SelectObject( m_buffer.hDC, m_buffer.hBitmap );
#else
		if ( m_buffer.fLocked )
		{
			SelectObject( m_buffer.hDC, m_buffer.hBitmap );
		}
#endif
	}
}

RGBQUAD * CBitmap::Bits()
{
	FreeDC();
	return m_buffer.pvBuffer;
}

CBitmap::operator HDC()
{
	return LockDC();
}

HBITMAP CBitmap::Bitmap()
{
	INT cx = m_buffer.sz.cx, cy = m_buffer.sz.cy, uSize = sizeof(RGBQUAD) * cx * cy;
	PVOID pvBuffer = NULL;

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi);
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = -cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = uSize;

	HBITMAP hBitmap = CreateDIBSection( m_buffer.hDC, &bmi, DIB_RGB_COLORS, (PVOID*) &pvBuffer, NULL, 0 );
	if ( hBitmap )
	{
		CopyMemory(pvBuffer, Bits(), uSize);
		return hBitmap;
	}

	return NULL;
}

INT CBitmap::Width()
{
	return m_buffer.sz.cx;
}

INT CBitmap::Height()
{
	return m_buffer.sz.cy;
}

VOID CBitmap::FillRect(int x, int y, int cx, int cy, COLORREF clr, BYTE bAlpha)
{
	RECT rc = { x, y, x + cx, y + cy };
	HBRUSH hBrush = CreateSolidBrush(clr);
	::FillRect(*this, &rc, hBrush);
	DeleteBrush(hBrush);

	RGBQUAD * pvBuffer = Bits();

	if ( bAlpha > 0 )
	{
		for(UINT i = 0; i < m_buffer.uSize / sizeof(RGBQUAD); ++i)
			pvBuffer[i].rgbReserved = bAlpha;
	}
}

VOID CBitmap::PremultiplyAlpha()
{
	BOOL fLocked = m_buffer.fLocked;
	UINT uSize = m_buffer.uSize / sizeof( RGBQUAD );
	FreeDC();
	for(UINT i = 0; i < uSize; ++i)
	{
		RGBQUAD & w = m_buffer.pvBuffer[i];
		w.rgbBlue = (BYTE) ( int(w.rgbReserved) * int(w.rgbBlue) / 255 );
		w.rgbGreen = (BYTE) ( int(w.rgbReserved) * int(w.rgbGreen) / 255 );
		w.rgbRed = (BYTE) ( int(w.rgbReserved) * int(w.rgbRed) / 255 );
	}

	if ( fLocked )
		LockDC();
}


VOID CBitmap::RenderBitmap(CBitmap & bmSrc, int x, int y, int cx, int cy, int dest_x, int dest_y, BYTE bAlpha, BOOL fUseSourceAlpha)
{
	BLENDFUNCTION bf = { 0 };
	bf.AlphaFormat = ( fUseSourceAlpha ? AC_SRC_ALPHA : 0 );
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = bAlpha;

	GdiAlphaBlend(*this, dest_x, dest_y, cx, cy, bmSrc, x, y, cx, cy, bf);
}


VOID CBitmap::RenderBitmap(HDC hDC, int x, int y, int cx, int cy, int dest_x, int dest_y, BYTE bAlpha, BOOL fUseSourceAlpha)
{
	BLENDFUNCTION bf = { 0 };
	bf.AlphaFormat = ( fUseSourceAlpha ? AC_SRC_ALPHA : 0 );
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = bAlpha;

	GdiAlphaBlend(hDC, dest_x, dest_y, cx, cy, *this, x, y, cx, cy, bf);
}

VOID CBitmap::GradientFill(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, DWORD dwFlags)
{
	if (!m_buffer.uSize) return;		// return if bitmap was not created
	GradientFillEx(x, y, cx, cy, crColor1, crColor2, 255, 255, dwFlags);
}

VOID CBitmap::GradientFillEx(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, BYTE bAlpha1, BYTE bAlpha2, DWORD dwFlags)
{
	if (!m_buffer.uSize) return;		// return if bitmap was not created

	TRIVERTEX tvi[2] = { 0 };
	GRADIENT_RECT grc = { 0 };
	tvi[0].Alpha = WORD(bAlpha1) << 8;
	tvi[0].Red = GetRValue(crColor1) << 8;
	tvi[0].Green = GetGValue(crColor1) << 8;
	tvi[0].Blue = GetBValue(crColor1) << 8;
	tvi[0].x = x;
	tvi[0].y = y;
	
	tvi[1].Alpha = WORD(bAlpha1) << 8;
	tvi[1].Red = GetRValue(crColor2) << 8;
	tvi[1].Green = GetGValue(crColor2) << 8;
	tvi[1].Blue = GetBValue(crColor2) << 8;
	tvi[1].x = x + cx;
	tvi[1].y = y + cy;

	grc.UpperLeft = 0; grc.LowerRight = 1;
	::GdiGradientFill(*this, tvi, 2, &grc, 1, dwFlags);
}

HBRUSH CBitmap::CreateBrush()
{
	return CreatePatternBrush(m_buffer.hBitmap);
}

VOID CBitmap::CreateFromBitmap(HBITMAP hBmp)
{
	BITMAP obj = { 0 };
	
	if ( GetObject( hBmp, sizeof(obj), &obj ) )
	{
		Destroy();
		Create(obj.bmWidth, obj.bmHeight);

		HDC hDesktopDC = __alloc_desktop_dc();
		HDC hMyDC = CreateCompatibleDC( hDesktopDC );
		
		HGDIOBJ hOldBm = SelectObject(hMyDC, hBmp);
		BitBlt( LockDC(), 0, 0, obj.bmWidth, obj.bmHeight, hMyDC, 0, 0, SRCCOPY );
		SelectObject(hMyDC, hOldBm);
		DeleteDC( hMyDC );
		__free_desktop_dc();
	}
}

VOID CBitmap::RenderText(INT x, INT y, INT cx, INT cy, LPCTSTR pszText, HFONT hFont, BOOL bTransparent, COLORREF crTextColor, COLORREF crBackColor, DWORD dwFlags)
{
	if (!m_buffer.uSize) return;		// return if bitmap was not created

	INT iright = x+cx,ibottom = y+cy;
	BOOL fLocked = m_buffer.fLocked;

	int bkMode = GetBkMode( *this );
	COLORREF clrText = GetTextColor( *this );
	COLORREF clrBg = GetBkColor( *this );
	HGDIOBJ hfntOld = SelectObject( *this, hFont );

	SetBkMode( *this,  bTransparent ? TRANSPARENT : OPAQUE );
	SetTextColor( *this, crTextColor );
	SetBkColor( *this, crBackColor );
	RECT rc = { x, y, x + cx, y + cy };

	DrawText( *this, pszText, lstrlen(pszText), &rc, dwFlags);

	SetBkMode( *this,  bkMode );
	SetTextColor( *this, clrText );
	SetBkColor( *this, clrBg );
	SelectObject( *this, hfntOld );

	if ( !fLocked )
		this->FreeDC();
}

#ifdef UNICODE
VOID CBitmap::RenderTextA(INT x, INT y, INT cx, INT cy, LPCSTR pszText, HFONT hFont, BOOL bTransparent, COLORREF crTextColor, COLORREF crBackColor, DWORD dwFlags)
{
	if (!m_buffer.uSize) return;		// return if bitmap was not created

	BOOL fLocked = m_buffer.fLocked;

	INT iright = x+cx,ibottom = y+cy;

	int bkMode = GetBkMode( *this );
	COLORREF clrText = GetTextColor( *this );
	COLORREF clrBg = GetBkColor( *this );
	HGDIOBJ hfntOld = SelectObject( *this, hFont );

	SetBkMode( *this,  bTransparent ? TRANSPARENT : OPAQUE );
	SetTextColor( *this, crTextColor );
	SetBkColor( *this, crBackColor );
	RECT rc = { x, y, x + cx, y + cy };

	DrawTextA( *this, pszText, lstrlenA(pszText), &rc, dwFlags);

	SetBkMode( *this,  bkMode );
	SetTextColor( *this, clrText );
	SetBkColor( *this, clrBg );
	SelectObject( *this, hfntOld );

	if ( !fLocked )
		this->FreeDC();
}
#endif

VOID CBitmap::RenderAlpha(CBitmap & bmSrc, UINT xSrc, UINT ySrc, UINT cxSrc, UINT cySrc, UINT xDst, UINT yDst, BOOL fUseSourceAlpha, USHORT uRenderAlphaValue)
{
	if ( !bmSrc.m_buffer.uSize )
		return;

	BYTE bRenderAlphaValue = LOBYTE( uRenderAlphaValue );
	BOOL fNoRenderAlphaChannel = ( HIBYTE( uRenderAlphaValue ) != 0 );

	if ( xSrc >= static_cast<UINT>( bmSrc.m_buffer.sz.cx ) || ySrc >= static_cast<UINT>( bmSrc.m_buffer.sz.cy ) ||
		xDst >= static_cast<UINT>( m_buffer.sz.cx ) || yDst >= static_cast<UINT>( m_buffer.sz.cy ) )
	{
		return;
	}

	RECT rcDst =
	{
		static_cast<LONG>( xDst ),
		static_cast<LONG>( yDst ),
		static_cast<LONG>( xDst + cxSrc ),
		static_cast<LONG>( yDst + cySrc )
	};

	RECT rcSrc =
	{
		static_cast<LONG>( xSrc ),
		static_cast<LONG>( ySrc ),
		static_cast<LONG>( xSrc + cxSrc ),
		static_cast<LONG>( ySrc + cySrc )
	};

	rcDst.left = min( this->m_buffer.sz.cx, rcDst.left );
	rcDst.top = min( this->m_buffer.sz.cy, rcDst.top );
	rcDst.right = min( this->m_buffer.sz.cx, rcDst.right );
	rcDst.bottom = min( this->m_buffer.sz.cy, rcDst.bottom );

	if ( rcDst.left == rcDst.right || rcDst.top == rcDst.bottom )
		return;

	rcSrc.left = min( bmSrc.m_buffer.sz.cx, rcSrc.left );
	rcSrc.top = min( bmSrc.m_buffer.sz.cy, rcSrc.top );
	rcSrc.right = min( bmSrc.m_buffer.sz.cx, rcSrc.right );
	rcSrc.bottom = min( bmSrc.m_buffer.sz.cy, rcSrc.bottom );

	if ( rcSrc.left == rcSrc.right || rcSrc.top == rcSrc.bottom )
		return;

	const UINT yFrom = rcDst.top, yTo = rcDst.bottom;
	const UINT xFrom = rcDst.left, xTo = rcDst.right;

	if ( !bRenderAlphaValue )
		return;

	BOOL fLockedDstDC = this->m_buffer.fLocked;
	BOOL fLockedSrcDC = bmSrc.m_buffer.fLocked;

	if ( !fUseSourceAlpha && 255 == bRenderAlphaValue )
	{
		// just blit one bitmap to another
		BitBlt( *this, rcDst.left, rcDst.top, rcDst.right - rcDst.left, rcDst.bottom - rcDst.top, bmSrc, rcSrc.left, rcSrc.top, SRCCOPY );
		this->FreeDC();
		bmSrc.FreeDC();
	}
	else if ( !fUseSourceAlpha && 255 > bRenderAlphaValue )
	{
		this->FreeDC();
		bmSrc.FreeDC();

		RGBQUAD * qSrc = bmSrc.m_buffer.pvBuffer + bmSrc.m_buffer.sz.cx * rcSrc.top + rcSrc.left;
		RGBQUAD * qDst = this->m_buffer.pvBuffer + this->m_buffer.sz.cx * rcDst.top + rcDst.left;

		for(UINT y = yFrom; y < yTo; ++y)
		{
			RGBQUAD * qiSrc = qSrc;
			RGBQUAD * qiDst = qDst;

			for(UINT x = xFrom; x < xTo; ++x)
			{
				qiDst->rgbBlue = ALPHA_COLOR( qiSrc->rgbBlue, qiDst->rgbBlue, bRenderAlphaValue );
				qiDst->rgbGreen = ALPHA_COLOR( qiSrc->rgbGreen, qiDst->rgbGreen, bRenderAlphaValue );
				qiDst->rgbRed = ALPHA_COLOR( qiSrc->rgbRed, qiDst->rgbRed, bRenderAlphaValue );
				
				qiSrc++;
				qiDst++;
			}

			qSrc += bmSrc.m_buffer.sz.cx;
			qDst += this->m_buffer.sz.cx;
		}
	}
	else if ( fUseSourceAlpha && 255 == bRenderAlphaValue )
	{
		this->FreeDC();
		bmSrc.FreeDC();
	
		RGBQUAD * qSrc = bmSrc.m_buffer.pvBuffer + bmSrc.m_buffer.sz.cx * rcSrc.top + rcSrc.left;
		RGBQUAD * qDst = this->m_buffer.pvBuffer + this->m_buffer.sz.cx * rcDst.top + rcDst.left;

		for(UINT y = yFrom; y < yTo; ++y)
		{
			RGBQUAD * qiSrc = qSrc;
			RGBQUAD * qiDst = qDst;

			for(UINT x = xFrom; x < xTo; ++x)
			{
				qiDst->rgbBlue = ALPHA_COLOR( qiSrc->rgbBlue, qiDst->rgbBlue, qiSrc->rgbReserved );
				qiDst->rgbGreen = ALPHA_COLOR( qiSrc->rgbGreen, qiDst->rgbGreen, qiSrc->rgbReserved );
				qiDst->rgbRed = ALPHA_COLOR( qiSrc->rgbRed, qiDst->rgbRed, qiSrc->rgbReserved );
				qiDst->rgbReserved = ALPHA_SUM( qiSrc->rgbReserved, qiDst->rgbReserved ); // (BYTE) min( 255, qiSrc->rgbReserved + qiDst->rgbReserved );
				qiSrc++;
				qiDst++;
			}

			qSrc += bmSrc.m_buffer.sz.cx;
			qDst += this->m_buffer.sz.cx;
		}
	}
	else if ( fUseSourceAlpha && 255 > bRenderAlphaValue && !fNoRenderAlphaChannel )
	{
		this->FreeDC();
		bmSrc.FreeDC();

		RGBQUAD * qSrc = bmSrc.m_buffer.pvBuffer + bmSrc.m_buffer.sz.cx * rcSrc.top + rcSrc.left;
		RGBQUAD * qDst = this->m_buffer.pvBuffer + this->m_buffer.sz.cx * rcDst.top + rcDst.left;

		register RGBQUAD r = { 0 };

		for(UINT y = yFrom; y < yTo; ++y)
		{
			RGBQUAD * qiSrc = qSrc;
			RGBQUAD * qiDst = qDst;

			for(UINT x = xFrom; x < xTo; ++x)
			{
				r.rgbBlue = ALPHA_COLOR( qiSrc->rgbBlue, qiDst->rgbBlue, qiSrc->rgbReserved );
				r.rgbGreen = ALPHA_COLOR( qiSrc->rgbGreen, qiDst->rgbGreen, qiSrc->rgbReserved );
				r.rgbRed = ALPHA_COLOR( qiSrc->rgbRed, qiDst->rgbRed, qiSrc->rgbReserved );
				qiDst->rgbBlue = ALPHA_COLOR( r.rgbBlue, qiDst->rgbBlue, bRenderAlphaValue );
				qiDst->rgbGreen = ALPHA_COLOR( r.rgbGreen, qiDst->rgbGreen, bRenderAlphaValue );
				qiDst->rgbRed = ALPHA_COLOR( r.rgbRed, qiDst->rgbRed, bRenderAlphaValue );
				qiDst->rgbReserved = ALPHA_SUM( qiSrc->rgbReserved, qiDst->rgbReserved ) * bRenderAlphaValue / 255;

				qiSrc++;
				qiDst++;
			}

			qSrc += bmSrc.m_buffer.sz.cx;
			qDst += this->m_buffer.sz.cx;
		}
	}
	else if ( fUseSourceAlpha && 255 > bRenderAlphaValue && fNoRenderAlphaChannel )
	{
		this->FreeDC();
		bmSrc.FreeDC();

		RGBQUAD * qSrc = bmSrc.m_buffer.pvBuffer + bmSrc.m_buffer.sz.cx * rcSrc.top + rcSrc.left;
		RGBQUAD * qDst = this->m_buffer.pvBuffer + this->m_buffer.sz.cx * rcDst.top + rcDst.left;

		register RGBQUAD r = { 0 };

		for(UINT y = yFrom; y < yTo; ++y)
		{
			RGBQUAD * qiSrc = qSrc;
			RGBQUAD * qiDst = qDst;

			for(UINT x = xFrom; x < xTo; ++x)
			{
				r.rgbBlue = ALPHA_COLOR( qiSrc->rgbBlue, qiDst->rgbBlue, qiSrc->rgbReserved );
				r.rgbGreen = ALPHA_COLOR( qiSrc->rgbGreen, qiDst->rgbGreen, qiSrc->rgbReserved );
				r.rgbRed = ALPHA_COLOR( qiSrc->rgbRed, qiDst->rgbRed, qiSrc->rgbReserved );
				qiDst->rgbBlue = ALPHA_COLOR( r.rgbBlue, qiDst->rgbBlue, bRenderAlphaValue );
				qiDst->rgbGreen = ALPHA_COLOR( r.rgbGreen, qiDst->rgbGreen, bRenderAlphaValue );
				qiDst->rgbRed = ALPHA_COLOR( r.rgbRed, qiDst->rgbRed, bRenderAlphaValue );

				qiSrc++;
				qiDst++;
			}

			qSrc += bmSrc.m_buffer.sz.cx;
			qDst += this->m_buffer.sz.cx;
		}
	}

	if ( fLockedDstDC )
		this->LockDC();


	if ( fLockedSrcDC )
		bmSrc.LockDC();

}

VOID CBitmap::CopyRect(CBitmap & bmSrc, UINT xSrc, UINT ySrc, UINT cxSrc, UINT cySrc, UINT xDst, UINT yDst)
{
	if ( !bmSrc.m_buffer.uSize )
		return;

	RECT rcDst =
	{
		static_cast<LONG>( xDst ),
		static_cast<LONG>( yDst ),
		static_cast<LONG>( xDst + cxSrc ),
		static_cast<LONG>( yDst + cySrc )
	};

	RECT rcSrc =
	{
		static_cast<LONG>( xSrc ),
		static_cast<LONG>( ySrc ),
		static_cast<LONG>( xSrc + cxSrc ),
		static_cast<LONG>( ySrc + cySrc )
	};

	rcDst.left = min( this->m_buffer.sz.cx, rcDst.left );
	rcDst.top = min( this->m_buffer.sz.cy, rcDst.top );
	rcDst.right = min( this->m_buffer.sz.cx, rcDst.right );
	rcDst.bottom = min( this->m_buffer.sz.cy, rcDst.bottom );

	if ( rcDst.left == rcDst.right || rcDst.top == rcDst.bottom )
		return;

	rcSrc.left = min( bmSrc.m_buffer.sz.cx, rcSrc.left );
	rcSrc.top = min( bmSrc.m_buffer.sz.cy, rcSrc.top );
	rcSrc.right = min( bmSrc.m_buffer.sz.cx, rcSrc.right );
	rcSrc.bottom = min( bmSrc.m_buffer.sz.cy, rcSrc.bottom );

	if ( rcSrc.left == rcSrc.right || rcSrc.top == rcSrc.bottom )
		return;

	const UINT yFrom = rcDst.top, yTo = rcDst.bottom;
	const UINT xSize = rcDst.right - rcDst.left;

	RGBQUAD * qSrc = bmSrc.m_buffer.pvBuffer + bmSrc.m_buffer.sz.cx * rcSrc.top + rcSrc.left;
	RGBQUAD * qDst = this->m_buffer.pvBuffer + this->m_buffer.sz.cx * rcDst.top + rcDst.left;

	for(UINT y = yFrom; y < yTo; ++y)
	{
		MoveMemory(qDst, qSrc, xSize * sizeof(RGBQUAD));
		qSrc += bmSrc.m_buffer.sz.cx;
		qDst += this->m_buffer.sz.cx;
	}
}


void CBitmap::DropShadow(CBitmap & bmp, int width, BOOL fRectangle, BOOL fRectUpSide)
{
	if ( !m_buffer.pvBuffer || width <= 0 )
		return;

	int c = m_buffer.sz.cx * m_buffer.sz.cy;

	CBitmap bmAlphaMask;
	bmAlphaMask.Create(m_buffer.sz.cx, m_buffer.sz.cy);
	bmp.Create(m_buffer.sz.cx + width, m_buffer.sz.cy + width);
	HDC hdc = bmp.LockDC();
	HGDIOBJ holdpn = SelectObject(hdc, GetStockPen(NULL_PEN));

	for(int w = width - 1; w >= 0; w--)
	{
		BYTE bmMask[256];
		for(int k = 0; k < 256; ++k)
			bmMask[k] = BYTE( k - k * w / width );
		bmMask[255] = ( 255 == bmMask[255] ? 0 : bmMask[255] );

		for(int i = 0; i < c; ++i)
			((RGBQUAD*) bmAlphaMask.m_buffer.pvBuffer)[i].rgbReserved = bmMask[fRectangle ? 255 : ((RGBQUAD*) m_buffer.pvBuffer)[i].rgbReserved ];

		HBRUSH hBrush = bmAlphaMask.CreateBrush();
		RECT rc = { w, w, m_buffer.sz.cx + w, m_buffer.sz.cy + w };
		HGDIOBJ holdbr = SelectObject(hdc, hBrush);
		::RoundRect(hdc, w, w, m_buffer.sz.cx + w, m_buffer.sz.cy + w, w, w);
		
		if ( fRectUpSide )
			::Rectangle(hdc, w, 0, m_buffer.sz.cx + w, w + w);


		SelectObject(hdc, hBrush);
		DeleteObject(hBrush);
	}

	SelectObject(hdc, holdpn);
	bmp.FreeDC();

	bmp.m_fShadow = TRUE;
	bmp.CreateShadowBuffer();

}

void CBitmap::DrawShadow(int xLeft, int yTop, int xWidth, int yHeight, CBitmap & bmShadow, int xSrc, int ySrc, BYTE bAlpha, BOOL fRestoreAlpha)
{
	if ( !bmShadow.m_fShadow )
		return;

	// LARGE_INTEGER li1 = { 0 }, li2 = { 0 };
	// QueryPerfomanceCounter(&li1);
	xWidth = ( xWidth < 0 ? bmShadow.Width() : xWidth );
	yHeight = ( yHeight < 0 ? bmShadow.Height() : yHeight );

	int xRight = xLeft + xWidth, yBottom = yTop + yHeight;

	BYTE bAlphaMask[256];
	if ( bAlpha < 255 )
		for(int i = 0; i < 256; ++i)
			bAlphaMask[i] = BYTE( i * LONG(bAlpha) / 255 );
	else
		for(int i = 0; i < 256; ++i)
			bAlphaMask[i] = i;


	yBottom = min(yBottom, min(m_buffer.sz.cy - 1, bmShadow.m_buffer.sz.cy - ySrc + yTop - 1));
	xRight = min(xRight, min(m_buffer.sz.cx - 1, bmShadow.m_buffer.sz.cx - xSrc + xLeft - 1));

	int lineDestStart = yTop * m_buffer.sz.cx + xLeft;
	int lineSrcStart = ySrc * bmShadow.m_buffer.sz.cx + xSrc;

	for(int y = yTop; y < yBottom; ++y)
	{
		int lineDest = lineDestStart;
		int lineSrc = lineSrcStart;

		for(int x = xLeft; x < xRight; ++x)
		{
			RGBQUAD & wDest = ((RGBQUAD*) m_buffer.pvBuffer )[lineDest++];
			BYTE bAlphaSrc = 255 - bAlphaMask[((RGBQUAD*) bmShadow.m_buffer.pvBuffer)[lineSrc++].rgbReserved];

			if ( 255 == bAlphaSrc )
				continue;
			
			if ( 0 != wDest.rgbReserved && 255 != wDest.rgbReserved && fRestoreAlpha )
			{ 
				wDest.rgbRed = bmShadow.m_bmAlphaRestored[wDest.rgbRed][wDest.rgbReserved];
				wDest.rgbGreen = bmShadow.m_bmAlphaRestored[wDest.rgbGreen][wDest.rgbReserved];
				wDest.rgbBlue = bmShadow.m_bmAlphaRestored[wDest.rgbBlue][wDest.rgbReserved];
			}

			wDest.rgbRed = bmShadow.m_bmAlphaMask[bAlphaSrc][wDest.rgbRed];
			wDest.rgbGreen = bmShadow.m_bmAlphaMask[bAlphaSrc][wDest.rgbGreen];
			wDest.rgbBlue =  bmShadow.m_bmAlphaMask[bAlphaSrc][wDest.rgbBlue];
			wDest.rgbReserved = bAlphaSrc;
		}

		lineDestStart += m_buffer.sz.cx;
		lineSrcStart += bmShadow.m_buffer.sz.cx;
	}

	// QueryPerformanceCounter(&li2);
	// _tprintf(TEXT("Shadow rendered in: %d c (%d:%d-%d:%d, %d:%d, %d, %d)\n"), li2.LowPart - li1.LowPart, xLeft, yTop, xWidth, yHeight, xSrc, ySrc, bAlpha, fRestoreAlpha);
}

void CBitmap::CreateShadowBuffer()
{
	if ( m_bmAlphaMask || m_bmAlphaRestored )	// already done
		return;

	m_bmAlphaMask = new PBYTE[256];
	m_bmAlphaRestored = new PBYTE[256];
	m_bmAlphaMask[0] = new BYTE[65536];
	m_bmAlphaRestored[0] = new BYTE[65536];

	for(int i = 1; i < 256; ++i)
	{
		m_bmAlphaMask[i] = &m_bmAlphaMask[0][256*i];
		m_bmAlphaRestored[i] = &m_bmAlphaRestored[0][256*i];
	}

	// fill buffers with values
	for(int src = 0; src < 256; ++src)
		for(int dst = 0; dst < 256; ++dst)
		{
			m_bmAlphaMask[src][dst] = BYTE(src * dst / 255);
			if ( dst && dst < 255 )
				m_bmAlphaRestored[src][dst] = BYTE(255 * src / dst);
		}
}

void CBitmap::DestroyShadowBuffer()
{
	if ( m_bmAlphaMask )
	{
		delete[] m_bmAlphaMask[0];

		delete[] m_bmAlphaMask;
	}

	if ( m_bmAlphaRestored )
	{
		delete[] m_bmAlphaRestored[0];
		delete[] m_bmAlphaRestored;
	}

	m_bmAlphaMask = m_bmAlphaRestored = NULL;
}

VOID CBitmap::FillRect(INT x, INT y, INT cx, INT cy, COLORREF crColor, COLORREF crBorder, INT iWidth, BYTE fInnerAlpha)
{
	BOOL fLockedDC = this->m_buffer.fLocked;


	HPEN hPen = ( iWidth > 0 ? CreatePen(PS_SOLID, iWidth, crBorder) : GetStockPen(NULL_PEN) );
	HBRUSH hBrush = CreateSolidBrush( crColor );
	HDC hDC = LockDC();

	HGDIOBJ holdPen = SelectObject(hDC, hPen);
	HGDIOBJ holdBr = SelectObject(hDC, hBrush);

	::Rectangle(hDC, x, y, cx + x, cy + y);

	SelectObject(hDC, holdBr);
	SelectObject(hDC, holdPen);

	if ( iWidth > 0 ) DeletePen( hPen );
	DeleteBrush( hBrush );
	FreeDC();


//	crColor = XCHGCOLORREF( crColor );
//	crBorder = XCHGCOLORREF( crBorder );

	RECT rcDst = { x, y, x + cx, y + cy };

	rcDst.left = min( this->m_buffer.sz.cx, rcDst.left );
	rcDst.top = min( this->m_buffer.sz.cy, rcDst.top );
	rcDst.right = min( this->m_buffer.sz.cx, rcDst.right );
	rcDst.bottom = min( this->m_buffer.sz.cy, rcDst.bottom );

	if ( rcDst.left != rcDst.right && rcDst.top != rcDst.bottom )
	{
		const int xFrom = rcDst.left, xTo = rcDst.right;
		const int yFrom = rcDst.top, yTo = rcDst.bottom;

		RGBQUAD * qDst = Bits() + yFrom * this->m_buffer.sz.cx + xFrom;

		if ( fInnerAlpha < 255 && iWidth > 0 )
		{
			for(int y = yFrom; y < yTo; ++y)
			{
				RGBQUAD * qiDst = qDst;
				BOOL fBorder = ( y - yFrom < iWidth || yTo - y <= iWidth );
		
				for(int x = xFrom; x < xTo; ++x)
				{
//					volatile COLORREF & clr = *( (COLORREF*)qiDst );

					if ( fBorder )
					{
//						clr = crBorder;
						qiDst->rgbReserved = 255;
					}
					else if ( x - xFrom < iWidth || xTo - x <= iWidth )
					{
//						clr = crBorder;
						qiDst->rgbReserved = 255;
					}
					else
					{
//						clr = crColor;
						qiDst->rgbReserved = fInnerAlpha;
					}
					qiDst++;
				}

				qDst += this->m_buffer.sz.cx;
			}
		}
		else if ( fInnerAlpha == 255 )
		{
			for(int y = yFrom; y < yTo; ++y)
			{
				RGBQUAD * qiDst = qDst;
		
				for(int x = xFrom; x < xTo; ++x)
				{
//					volatile COLORREF & clr = *( (COLORREF*)qiDst );
//					clr = crColor;
					qiDst->rgbReserved = 255;
					qiDst++;
				}

				qDst += this->m_buffer.sz.cx;
			}
		}
		else if ( !iWidth && fInnerAlpha < 255 )
		{
			for(int y = yFrom; y < yTo; ++y)
			{
				RGBQUAD * qiDst = qDst;
		
				for(int x = xFrom; x < xTo; ++x)
				{
//					volatile COLORREF & clr = *( (COLORREF*)qiDst );
//					clr = crColor;
					qiDst->rgbReserved = fInnerAlpha;
					qiDst++;
				}

				qDst += this->m_buffer.sz.cx;
			}
		}
	}

	if ( fLockedDC )
		LockDC();
}

VOID CBitmap::GrayScale()
{
	BOOL fLocked = m_buffer.fLocked;

	SIZE sz = { Width(), Height() };
	RGBQUAD * w = Bits();

	for(int y = 0; y < sz.cy; ++y)
	{
		int line = y * sz.cx;

		for(int x = 0; x < sz.cx; ++x)
		{
			RGBQUAD & px = w[line++];
			px.rgbGreen =
				px.rgbRed =
				px.rgbBlue = BYTE( 0.21 * double(px.rgbRed) + 0.72 * double(px.rgbGreen) + 0.07 * double(px.rgbBlue) );
		}
	}

	if ( fLocked )
		LockDC();
}

VOID CBitmap::FadeAlpha(BYTE bAlpha)
{
	if ( 255 == bAlpha )
		return;

	BOOL fLockedDC = this->m_buffer.fLocked;
	RGBQUAD * p = Bits();
	size_t c = Width() * Height();
	int iAlpha = bAlpha;

	for(size_t i = 0; i < c; ++i)
		p[i].rgbReserved = LOBYTE( iAlpha * int(p[i].rgbReserved) / 255 );

	if ( fLockedDC )
		LockDC();
}

VOID CBitmap::AlphaGradient(INT x, INT y, INT cx, INT cy, DWORD dwFlags, INT bDirection)
{
	BOOL fLockedDC = this->m_buffer.fLocked;

	RECT rcDst = { x, y, x + cx, y + cy };

	rcDst.left = min( this->m_buffer.sz.cx, rcDst.left );
	rcDst.top = min( this->m_buffer.sz.cy, rcDst.top );
	rcDst.right = min( this->m_buffer.sz.cx, rcDst.right );
	rcDst.bottom = min( this->m_buffer.sz.cy, rcDst.bottom );

	if ( rcDst.left != rcDst.right && rcDst.top != rcDst.bottom )
	{
		const int xFrom = rcDst.left, xTo = rcDst.right;
		const int yFrom = rcDst.top, yTo = rcDst.bottom;

		RGBQUAD * qDst = Bits();

		switch(dwFlags)
		{
		case GRADIENT_FILL_RECT_H:
			{
				break;
			}						
		case GRADIENT_FILL_RECT_V:
			{
				break;
			}
		}
	}

	if ( fLockedDC )
		LockDC();
}
