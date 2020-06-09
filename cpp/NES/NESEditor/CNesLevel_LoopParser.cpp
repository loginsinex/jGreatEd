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

#include "h\NES\NESEditor\NESEditor.h"

VOID CNesLevel::LoadLoops( const std::vector<NES_LOOP>& vLoop )
{
	if ( !vLoop.size() )
	{
		return;
	}
	
	int iSlaveCounter = 0;
	auto vLocalLoop = vLoop;
	std::sort(vLocalLoop.begin(),vLocalLoop.end(), 
		[]( const NES_LOOP & lpc1, const NES_LOOP & lpc2 )
		{
			return ( lpc1.bPageRewind < lpc2.bPageRewind );
		}
	);

	auto itLoop = vLocalLoop.begin();

	for ( auto pItem : m_obj.vpItem )
	{
		if ( NES_SPECIAL_OBJECT_D == pItem->GetRealPoint().y && 0x4B == pItem->Item().item.itemId )	// is a loop
		{
			// find suitable loop
			BYTE bPage = LOBYTE( pItem->GetRealPoint().x / 0x10 );
			itLoop = std::find_if( vLocalLoop.begin(), vLocalLoop.end(), 
				[&bPage]( const NES_LOOP & lpc )
			{
				if ( lpc.bPageNumber >= bPage ) return TRUE;
				return FALSE;
			}
			);

			if ( itLoop == vLocalLoop.end() ) break;

			const NES_LOOP & lpc = *itLoop;
			CNesLoopCommand * pLoop = static_cast<CNesLoopCommand*>( pItem );

			if ( iSlaveCounter > 0 )
			{
				iSlaveCounter--;
			}
			else
			{
				iSlaveCounter = ( lpc.bSlaveData > 1 ? lpc.bSlaveData - 1 : 0 );
			}

			pLoop->LoadLoop( lpc );
			pLoop->LoopConnected( iSlaveCounter > 0 );
			vLocalLoop.erase( itLoop );
		}
	}
}

VOID CNesLevel::DumpLoops( std::vector<NES_LOOP>& vLoop )
{
	// now set scores for loops
	std::vector<NES_LOOP> vLocalLoop;
	BYTE uSlaveData = 0;
	for ( auto pItem : m_obj.vpItem )
	{
		if ( NES_SPECIAL_OBJECT_D == pItem->GetRealPoint().y && 0x4B == pItem->Item().item.itemId )
		{
			CNesLoopCommand * pLoop = static_cast<CNesLoopCommand*>( pItem );
			NES_LOOP lpc;
			pLoop->DumpLoop( lpc );
			lpc.aptr = AreaPtr();

			if ( pLoop->LoopConnected() )
			{
				uSlaveData++;
			}
			else
			{
				BYTE uLoopVectorData = uSlaveData + 1;
				lpc.bSlaveData = uLoopVectorData;

				if ( uSlaveData > 0 )
				{
					size_t cLoops = vLocalLoop.size();
					while ( uSlaveData )
					{
						NES_LOOP & lpcp = vLocalLoop[ cLoops - uSlaveData ];
						lpcp.bSlaveData = uLoopVectorData;
						lpcp.bPageRewind = lpc.bPageRewind;
						uSlaveData--;
					}
				}				
			}

			vLocalLoop.push_back( lpc );
		}
	}

	// checkout for uSlaveData. It must be 0. If not, then set all previous loops as slaves, and last - as master
	if ( uSlaveData > 0 )
	{
		NES_LOOP & lpc = vLocalLoop.back();
		size_t cLoops = vLocalLoop.size();
		BYTE uLoopVectorData = uSlaveData + 1;
		lpc.bSlaveData = uLoopVectorData;
		while ( uSlaveData && uSlaveData < cLoops )
		{
			NES_LOOP & lpcp = vLocalLoop[ cLoops - uSlaveData - 1 ];
			lpcp.bSlaveData = uLoopVectorData;
			lpcp.bPageRewind = lpc.bPageRewind;
			uSlaveData--;
		}
	}

	vLoop = vLocalLoop;
}

VOID CNesLevel::DumpLoops( std::map<BYTE, std::vector<CNesLoopCommand*>> & mLoop )
{
	mLoop.clear();

	for ( auto it = m_obj.vpItem.begin() + 1; it != m_obj.vpItem.end(); ++it )
	{
		CNesLevelItem * pObj = *it;
		if ( 0x4B == pObj->Item().item.itemId && NES_SPECIAL_OBJECT_D == pObj->Item().item.Y )
		{
			CNesLoopCommand * pLoop = static_cast<CNesLoopCommand*>( pObj );
			if ( pLoop->TriggerPage() >= 4 && !pLoop->LoopConnected() )
			{
				mLoop[ pLoop->TriggerPage() - 4 ].push_back( pLoop );
			}
		}
	}
}

BOOL CNesLevel::FindLoopObjectId( CNesLoopCommand * pObject, size_t & idObject )
{
	size_t counter = 1;
	NES_LOOP lp;
	pObject->DumpLoop( lp );
	for ( auto it = m_obj.vpItem.begin() + 1; it < m_obj.vpItem.end(); ++it, ++counter )
	{
		CNesLevelItem * pItem = *it;
		NES_LOOP lpFind;
		if ( 0x4B == pItem->Item().item.itemId && NES_SPECIAL_OBJECT_D == pItem->Item().item.Y )
		{
			CNesLoopCommand * pLoop = static_cast<CNesLoopCommand*>( pItem );
			pLoop->DumpLoop( lpFind );
			if ( lpFind.bHeight == lp.bHeight && lpFind.bPageNumber == lp.bPageNumber && pObject->GetRealPoint().x == pItem->GetRealPoint().x )
			{
				idObject = counter;
				return TRUE;
			}
		}
	}

	return FALSE;
}

