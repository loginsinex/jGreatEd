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

#define XCHGCOLORREF(_x)              ((((_x)&0xFF00FF00L)|(((_x)&0x00FF0000L)>>16)|(((_x)&0x000000FFL)<<16)))
#define ALPHA_COLOR(front,back,alpha)	((alpha)*((front)-(back))/255+(back))
#define ALPHA_SUM(front,back)			((back) + (front) * (255 - (back)) / 255)

// #define		BITMAP_USE_LOCKDC

class CBitmap
{
	struct
	{
		HDC						hDCDesktop;
		HDC						hDC;
		RGBQUAD *				pvBuffer;
		HBITMAP					hBitmap;
		HBITMAP					hDefaultBitmap;
		SIZE					sz;
		UINT					uSize;
		BOOL					fLocked;
	} m_buffer;

	// shadow buffer
	PBYTE					*m_bmAlphaMask;
	PBYTE					*m_bmAlphaRestored;
	BOOL					m_fShadow;

	BOOL					CreateFromResource(HINSTANCE hInstance, LPCTSTR pszResource);

	void					CreateShadowBuffer();
	void					DestroyShadowBuffer();

	HDC						CheckDC();

public:
	CBitmap();
	CBitmap(const CBitmap & bm);
	CBitmap(INT cx, INT cy);
	CBitmap(HINSTANCE hInstance, LPCTSTR pszResource);
	~CBitmap();

	VOID						Create(INT cx, INT cy);
	VOID						Destroy();
	RGBQUAD *					Bits();
	// RGBQUAD *					operator[](int cy);
	CBitmap &					operator=(const CBitmap & bm);
	operator HDC();
	VOID						FreeDC();
	HDC							LockDC();
	VOID						ResetBitmapDC();
	HBITMAP						Bitmap();
	INT							Width();
	INT							Height();

	HBRUSH						CreateBrush();

	VOID						CreateFromBitmap(HBITMAP hBmp);
	VOID						FillRect(int x, int y, int cx, int cy, COLORREF clr, BYTE bAlpha = 0);
	VOID						FillRect(INT x, INT y, INT cx, INT cy, COLORREF crColor, COLORREF crBorder, INT iWidth, BYTE fInnerAlpha = 255);
	VOID						PremultiplyAlpha();
	VOID						FadeAlpha(BYTE bAlpha);
	VOID						RenderBitmap(CBitmap & bmSrc, int x, int y, int cx, int cy, int dest_x, int dest_y, BYTE bAlpha = 255, BOOL fUseSourceAlpha = FALSE);
	VOID						RenderBitmap(HDC hDC, int x, int y, int cx, int cy, int dest_x, int dest_y, BYTE bAlpha = 255, BOOL fUseSourceAlpha = FALSE);
	VOID						GradientFill(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, DWORD dwFlags);
	VOID						GradientFillEx(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, BYTE bAlpha1, BYTE bAlpha2, DWORD dwFlags);
	VOID						RenderText(INT x, INT y, INT cx, INT cy, LPCTSTR pszText, HFONT hFont, BOOL bTransparent, COLORREF crTextColor, COLORREF crBackColor, DWORD dwFlags);
	VOID						RenderAlpha(CBitmap & bmSrc, UINT xSrc, UINT ySrc, UINT cxSrc, UINT cySrc, UINT xDst, UINT yDst, BOOL fUseSourceAlpha = FALSE, USHORT bRenderAlphaValue = 255);
	VOID						CopyRect(CBitmap & bmSrc, UINT xSrc, UINT ySrc, UINT cxSrc, UINT cySrc, UINT xDst, UINT yDst);
	VOID						DropShadow(CBitmap & bmp, int width, BOOL fRectangle = FALSE, BOOL fRectUpSide = FALSE);
	VOID						DrawShadow(int xLeft, int yTop, int xWidth, int yHeight, CBitmap & bmShadow, int x, int y, BYTE bAlpha = 255, BOOL fRestoreColor = FALSE);
	VOID						GrayScale();
	VOID						AlphaGradient(INT x, INT y, INT cx, INT cy, DWORD dwFlags, INT bDirection = 1);
#ifdef UNICODE
	VOID						RenderTextA(INT x, INT y, INT cx, INT cy, LPCSTR pszText, HFONT hFont, BOOL bTransparent, COLORREF crTextColor, COLORREF crBackColor, DWORD dwFlags);
#else
#define RenderTextA				RenderText
#endif

};