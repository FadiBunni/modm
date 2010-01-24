// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: default_postman.cpp 77 2009-10-15 18:34:29Z thundernail $
 */
// ----------------------------------------------------------------------------

#include "postman.hpp"

xpcc::StlPostman::StlPostman()
{

}

// ----------------------------------------------------------------------------

xpcc::StlPostman::StlPostman(const EventMap& eventMap, const RequestMap& requenstMap) :
	eventMap ( eventMap ),
	requenstMap ( requenstMap )
{

}

// ----------------------------------------------------------------------------

xpcc::StlPostman::~StlPostman()
{
}

// ----------------------------------------------------------------------------
xpcc::StlPostman::DeliverInfo
xpcc::StlPostman::deliverPacket(const BackendInterface& backend)
{
	const xpcc::Header &header = backend.getPacketHeader();

	if( header.destination == 0 ) {
		// EVENT
		EventMap::const_iterator lowerBound( this->eventMap.lower_bound( header.packetIdentifier) );
		EventMap::const_iterator upperBound( this->eventMap.upper_bound( header.packetIdentifier) );
		if( lowerBound != upperBound ) {
			do {
				lowerBound->second.handleResponse( backend );
				lowerBound++;
			} while( lowerBound != upperBound );
			return OK;
		}
		return NO_EVENT;
	}
	else {
		// REQUEST
		RequestMap::const_iterator iterDestination( this->requenstMap.find( header.destination ) );
		if( iterDestination != this->requenstMap.end() ) {
			CallbackMap::const_iterator iterCallback( iterDestination->second.find( header.packetIdentifier ) );
			if( iterCallback != iterDestination->second.end() ) {
				iterCallback->second.handleResponse( backend );
				return OK;
			}
			else {
				return NO_ACTION;
			}
		}
		else {
			return NO_COMPONENT;
		}
	}
}

// ----------------------------------------------------------------------------

xpcc::StlPostman::DeliverInfo
xpcc::StlPostman::deliverPacket(const Header &header, SmartPointer& payload)
{
	(void) header;
	(void) payload;
	// TODO ???
	return NOT_IMPLEMENTED_YET_ERROR;
}

// ----------------------------------------------------------------------------

xpcc::StlPostman::DeliverInfo
xpcc::StlPostman::deliverPacket(const Header &header)
{
	(void) header;
	// TODO ???
	return NOT_IMPLEMENTED_YET_ERROR;
}

// ----------------------------------------------------------------------------

bool
xpcc::StlPostman::isComponentAvaliable(const Header& header) const
{
	RequestMap::const_iterator iterDestination( this->requenstMap.find( header.destination ) );
	return ( iterDestination != this->requenstMap.end() );
}
