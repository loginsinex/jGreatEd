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

#define		NES_DRAW_QUEUE_LENGTH			3

#include "h/NES/NESEditor/NesObjects/NesLevelItems/GroundDefinition.h"

class CNesObject;

typedef struct _tagNES_DRAW_QUEUE_ITEM
{
	CNesObject			* pObject;
	NES_GROUND_PLACE	* pGround;
	int					left;
	int					right;
	int					y;
} NES_DRAW_QUEUE_ITEM, *PNES_DRAW_QUEUE_ITEM;

class CNesDrawQueue
{
	const int							m_qLength;
	int									m_wCurrentPage;
	std::vector<NES_DRAW_QUEUE_ITEM>	m_vFront;
	std::vector<NES_DRAW_QUEUE_ITEM>	m_vBack;

	std::vector<NES_DRAW_QUEUE_ITEM>	& m_vgFront;
	std::vector<NES_DRAW_QUEUE_ITEM>	& m_vgBack;


	BOOL					FindSuitableSlot( const NES_DRAW_QUEUE_ITEM & dqi );
	VOID					FlushQueuedObjects();
	VOID					DrawObject( NES_DRAW_QUEUE_ITEM & item );

public:
	CNesDrawQueue(
		std::vector<NES_DRAW_QUEUE_ITEM> & vFrontObj,
		std::vector<NES_DRAW_QUEUE_ITEM> & vBackObj,
		int length = NES_DRAW_QUEUE_LENGTH );

	BOOL					NextObject( NES_DRAW_QUEUE_ITEM & object );
	VOID					FinalizeQueue();
};
