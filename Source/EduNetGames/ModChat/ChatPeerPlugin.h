#ifndef ChatPeerPlugin_h__
#define ChatPeerPlugin_h__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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
//-----------------------------------------------------------------------------
/*! 
 *  \brief     Plugin to realize basic chat functions.
 *  \details   This plugin shows the main steps to instantiate
 *             a network interface and implement a P2P chat system.
 *  \author    Jan Fietz
 */

#include "EduNetConnect/EduNetConnect.h"
#include "EduNetConnect/ReplicationTypes.h"
#include "OpenSteer/Plugin.h"
#include "OpenSteer/AbstractVehicle.h"

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/circular_buffer.hpp>

class GLUI_TextBox;
namespace EduNet
{
	namespace ModChat
	{		
		struct Message
		{
		public:
			Message(const enString_t& s, const enString_t& c):
			 sender(s),content(c)
			 {
				 timeStamp = boost::posix_time::second_clock::universal_time();
			 }
			enString_t sender;
			enString_t content;
			boost::posix_time::ptime timeStamp;
		};

		class MessageList
		{
		public:
			MessageList(boost::uint32_t size = 10);

			void add(const enString_t& sender, const enString_t& content);

			void redraw(const OpenSteer::Vec3& pos) const;
		private:
			typedef boost::circular_buffer<Message> List;
			List messages;
		};
		class ChatPeerPlugin :	public OpenSteer::Plugin
		{
			ET_DECLARE_BASE( OpenSteer::Plugin );
		public:

			enum GuiIds
			{
				Id_MessageBox,
				Id_SendMessage,
				Id_Count
			};

			OS_IMPLEMENT_CLASSNAME( ChatPeerPlugin )

			ChatPeerPlugin();
			virtual ~ChatPeerPlugin();

			const char* name( void ) const { return this->getClassName(); }

			// override OpenSteer::Plugin
			virtual void initGui( void* pkUserdata );
			virtual void prepareOpen (void);
			virtual void open(void);
			virtual void close(void);
			virtual void reset (void);
			void update (const float currentTime, const float elapsedTime);
			void redraw (const float currentTime, const float elapsedTime);
			virtual void handleFunctionKeys (int keyNumber);
			virtual void printMiniHelpForFunctionKeys (void) const;

			virtual const OpenSteer::AVGroup& allVehicles (void) const;
			virtual OpenSteer::AVGroup& allVehicles(void);
			void onTypedMessage( );
		private:

			void onReceivedMessage(const enString_t& sender, const enString_t& content);
			void sendMessage(const enString_t& content);
			void receivedPackets();
			RakPeerInterface* m_networkInterface;
			GLUI_TextBox* m_pMessageTextBox;

			MessageList m_receivedMesssages;

		};
	}
}
#endif // ChatPeerPlugin_h__