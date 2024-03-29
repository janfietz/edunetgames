/*
//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
*/

#ifndef NETBOIDCLIENTPLUGIN_H
#define NETBOIDCLIENTPLUGIN_H

#include "EduNetConnect/AbstractEntityReplicaConnection.h"
#include "EduNetConnect/ClientPlugin.h"

#include "BoidsPlugin.h"

//-----------------------------------------------------------------------------
class NetClientBoidPlugin : public ClientPlugin<OpenSteer::NetBoidsPlugin>
{
	ET_DECLARE_BASE(ClientPlugin<OpenSteer::NetBoidsPlugin>);
public:
	NetClientBoidPlugin(bool bAddToRegistry = false);
	virtual ~NetClientBoidPlugin(){};

	OS_IMPLEMENT_CLASSNAME( NetClientBoidPlugin )
	virtual const char* name() const { return this->getClassName(); };

	virtual float selectionOrderSortKey (void) const { return 3.0f ;}

	virtual void StartNetworkSession( void );
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

private:
	
	AbstractEntityReplicaManager* m_kReplicaManager;
};

#endif // NETBOIDCLIENTPLUGIN_H
