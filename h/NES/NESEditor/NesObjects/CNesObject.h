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

#define NES_SPECIAL_OBJECT_C		0x0C
#define NES_SPECIAL_OBJECT_D		0x0D
#define NES_SPECIAL_OBJECT_E		0x0E
#define NES_SPECIAL_OBJECT_F		0x0F

#pragma pack(push,1)
typedef struct _tagNES_AREA_ITEM
{
	union
	{
		BYTE		bData[ 2 ];
		struct
		{
			BYTE		Y : 4;
			BYTE		X : 4;
			BYTE		itemId : 7;
			BYTE		fNewPage : 1;
		};
	};
} NES_AREA_ITEM, *PNES_AREA_ITEM;

typedef struct _tagNES_ENEMY_ITEM
{
	union
	{
		BYTE		bData[ 3 ];
		struct
		{
			BYTE		Y : 4;
			BYTE		X : 4;
			BYTE		objectId : 7;
			BYTE		fNewPage : 1;
			BYTE		bUnused : 8;
		};
	};
} NES_ENEMY_ITEM, *PNES_ENEMY_ITEM;
#pragma pack(pop)

typedef struct _tagNES_LEVEL_ITEM
{
	POINT				pt;
	NES_AREA_ITEM		item;
} NES_LEVEL_ITEM, *PNES_LEVEL_ITEM;

typedef struct _tagNES_LEVEL_ENEMY_ITEM
{
	POINT				pt;
	NES_ENEMY_ITEM		item;
} NES_LEVEL_ENEMY_ITEM, *PNES_LEVEL_ENEMY_ITEM;

typedef std::map<int, std::map<int, NES_METATILE>>		NES_MAP_POINT;
typedef std::map<int, std::map<int, NES_SPRITE>>		NES_MAP_SPRITE_POINT;

class CNesLevel;

class CNesObject
{
	CNesLevel								& m_nLevel;
	CNESFile								& m_file;
	int										m_uFixedY;
	BOOL									m_fPutBehind;

	RECT									m_rcObject;
	NES_MAP_POINT							m_mpt;
	NES_MAP_SPRITE_POINT					m_msprt;

	const BOOL								m_fSprite;
	BYTE									m_uOverrideY;
	BOOL									IsHeightFixed() const;
	int										FixedHeight();
	VOID									DrawMetatile( CNesLevelView & view, int X, int Y, const NES_METATILE & mtile );
	VOID									DrawSprite( CNesLevelView & view, int X, int Y, const NES_SPRITE & msprite );

protected:
	CNESFile &				ROM();
	VOID					FixHeight( int y );
	VOID					UnfixHeight();
	VOID					SetMetatile( int X, int Y, const NES_METATILE & mtile );
	VOID					EraseMetatile( int X, int Y );
	VOID					FreeMetatiles();
	VOID					SetSprite( BYTE bSprite, BYTE bAttribute, int X, int Y );
	VOID					EraseSprite( int X, int Y );
	VOID					FreeSprites();
	VOID					SetObjWidth( int cx );
	VOID					SetObjHeight( int cy );
	VOID					SetObjPlace( int x, int y );

	virtual VOID			DrawSpriteExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject );
	virtual VOID			DrawExtra( CNesLevelView & view, BYTE bWorldId, int x, int y, BOOL fAfterObject );
	virtual NES_METATILE	HandleMetatile( BYTE bWorldId, int X, int Y, const NES_METATILE & currentTile, const NES_METATILE & newTile );
	virtual BOOL			WidthChanging( int & cx );
	virtual VOID			WidthChanged( int cx );
	virtual BOOL			HeightChanging( int & cy );
	virtual VOID			HeightChanged( int cy );
	virtual BOOL			PlaceChanging( int & x, int & y );
	virtual VOID			PlaceChanged( int x, int y );
	virtual VOID			OverrideSpecial( BYTE y );
	virtual int				GetRealY();
	virtual int				GetRealX();

public:
	CNesObject( CNesLevel & nLevel, BOOL fSprite );
	virtual					~CNesObject();

	CNesLevel &				Level();
	VOID					PlaceObject( int x, int y );
	int						Width();
	int						Height();
	VOID					SetWidth( int cx );
	VOID					SetHeight( int cy );
	POINT					GetObjectPoint();
	VOID					DrawObject( CNesLevelView & view, BYTE bWorldId, int x, int y );
	VOID					DrawSprite( CNesLevelView & view, BYTE bWorldId, int x, int y );
	VOID					DrawObject( CNesLevelView & view, BYTE bWorldId );
	virtual VOID			InitObject();
	virtual VOID			LevelSizeChanged( const SIZE & sz );
	const RECT 				GetObjectRect() const;
	BOOL					IsSprite();

	POINT					GetRealPoint();
	VOID					EraseObjectCanvas();
	VOID					PutBehind( BOOL fBehind );
	BOOL					IsBehindObject();
};


/*
	vAreaParserTaskNum:
		0 - increment column
		1 - render0
		2 - render1
		3 - parse area for new column
		4 - increment column
		5 - render0
		6 - render1
		7 - parse area for new column

	__is_even_column = vCurrentColumn & 1;
	__render0 = !( vAreaParserTaskNum & 1 );

	__ram_buffer_offset = vRAM_Buffer_Offset;
	vRAM_Buffer[ __ram_buffer_offset ] = vCurrentNT_Addr_Hi;
	vRAM_Buffer[ __ram_buffer_offset+1 ] = vCurrentNT_Addr_Lo;
	vRAM_Buffer[ __ram_buffer_offset+2 ] = 154;

	_buffer_offset = 0;

	do
	{
		_metatile_buffer_offset = _buffer_offset;
		__attribute = vMetatileBuffer[ _metatile_buffer_offset ] & 0xC0;

		__arr_ptr_lo = 2 * ptrMetatileLo[__attribute >> 6 ];
		__arr_ptr_hi = 2 * ptrMetatileHi[__attribute >> 6 ];
		BYTE * vArr = ( 256 * __arr_ptr_hi + __arr_ptr_lo );

		__metatile_offset = vMetatileBuffer[ _metatile_buffer_offset ] << 2;
		vRAM_Buffer[__ram_buffer_offset + 3] = vArr[ __metatile_offset + 2 * __render0 ) + 0 ];
		vRAM_Buffer[__ram_buffer_offset + 4] = vArr[ __metatile_offset + 2 * __render0 ) + 1 ];

		if ( __is_even_column )
		{
			if ( _metatile_buffer_offset & 1 )
			{
				_buffer_offset++;
			}
			else
			{
				__attribute >>= 4;
			}
		}
		else if ( _metatile_buffer_offset & 1 )
		{
			__attribute >>= 2;
		}
		else
		{
			__attribute <<= 3;
		}


		vAttributeBuffer[ _buffer_offset ] |= __attribute;
		__ram_buffer_offset += 2;
		_metatile_buffer_offset++;
	} while( _metatile_buffer_offset < 13 );

	__ram_buffer_offset += 3;
	vRAM_Buffer[__ram_buffer_offset] = 0;
	vRAM_Buffer_Offset = __ram_buffer_offset;
	vCurrentNT_Addr_Lo++;
	if ( !( vCurrentNT_Addr_Lo & 0x1F ) )
	{
		vCurrentNT_Addr_Lo = 0x80;
		vCurrentNTAddr_High ^= 4;
	}

	// do render from VRAM_Buffer2 
	vVRAM_Buffer_AddrCtrl = #sc_VRAM_Buffer2;
*/