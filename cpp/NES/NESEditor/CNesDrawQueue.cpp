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

#include "h/NES/NESEditor/NESEditor.h"

CNesDrawQueue::CNesDrawQueue(
	std::vector<NES_DRAW_QUEUE_ITEM> & vFrontObj,
	std::vector<NES_DRAW_QUEUE_ITEM> & vBackObj,
	int length )
	: m_wCurrentPage( -1 ),
	m_vgFront( vFrontObj ), m_vgBack( vBackObj ), m_qLength( length )
{
	NES_DRAW_QUEUE_ITEM uNullItem = { nullptr, nullptr, -1, -1 };
	m_vFront.insert( m_vFront.end(), m_qLength, uNullItem );
}

BOOL CNesDrawQueue::FindSuitableSlot( const NES_DRAW_QUEUE_ITEM & dqi )
{
	for ( auto & slot : m_vFront )
	{
		if ( slot.right <= dqi.left )
		{
			slot = dqi;
			return TRUE;
		}
	}

	return FALSE;
}

VOID CNesDrawQueue::FlushQueuedObjects()
{
	m_vgBack.insert( m_vgBack.end(), m_vBack.begin(), m_vBack.end() );
	m_vBack.clear();
}

VOID CNesDrawQueue::DrawObject( NES_DRAW_QUEUE_ITEM & item )
{
	m_vgFront.push_back( std::move( item ) );
}

BOOL CNesDrawQueue::NextObject( NES_DRAW_QUEUE_ITEM & dqi )
{
	int wCurrentPage = dqi.left / 0x10;

	if ( wCurrentPage != m_wCurrentPage )
	{
		FlushQueuedObjects();
		m_wCurrentPage = wCurrentPage;
	}

	if ( m_vBack.empty() )
	{
		if ( FindSuitableSlot( dqi ) )
		{
			DrawObject( dqi );
			return TRUE;
		}
	}

	m_vBack.push_back( std::move( dqi ) );
	return FALSE;
}

VOID CNesDrawQueue::FinalizeQueue()
{
	NES_DRAW_QUEUE_ITEM uNullItem = { nullptr, nullptr, -1, -1 };
	FlushQueuedObjects();
	for ( auto & dqi : m_vFront )
	{
		dqi = uNullItem;
	}
}
