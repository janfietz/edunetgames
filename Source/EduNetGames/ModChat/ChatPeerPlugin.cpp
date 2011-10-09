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

#include "ChatPeerPlugin.h"
#include "EduNetCommon/EduNetDraw.h"

#include "OpenSteer/Renderer.h"

#include <boost/foreach.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace OpenSteer;
using namespace boost::xpressive;
namespace EduNet
{
	namespace ModChat
	{
		ChatPeerPlugin::ChatPeerPlugin():BaseClass(false),
			m_pMessageTextBox(NULL),
			m_receivedMesssages(MessageList(20))
		{

		}

		ChatPeerPlugin::~ChatPeerPlugin()
		{

		}

		//-----------------------------------------------------------------------------
		// callback function
		void onSendMessageCallBack(GLUI_Control* pkControl )
		{
			ChatPeerPlugin* pPlugin = (ChatPeerPlugin*)pkControl->ptr_val;
			pPlugin->onTypedMessage();
			
		}
		void ChatPeerPlugin::initGui( void* pkUserdata )
		{
			GLUI* glui = ::getRootGLUI();
			GLUI_Panel* pluginPanel = static_cast<GLUI_Panel*>( pkUserdata );

			// add a textbox
			m_pMessageTextBox = ET_NEW GLUI_TextBox( pluginPanel, true, Id_MessageBox );
			m_pMessageTextBox->set_h(100);
			m_pMessageTextBox->set_w(200);
			m_pMessageTextBox->set_text("Enter message here!!!");
			m_pMessageTextBox->set_ptr_val( this );

			
			GLUI_Control* pkControl;
			pkControl = glui->add_button_to_panel(pluginPanel,"Send", Id_SendMessage, onSendMessageCallBack);
			pkControl->set_ptr_val( this );
		}
		
		void ChatPeerPlugin::prepareOpen( void )
		{

		}

		void ChatPeerPlugin::open( void )
		{
			// allocate a network interface
			m_networkInterface = RakNetworkFactory::GetRakPeerInterface();
		}

		void ChatPeerPlugin::close( void )
		{
			// do not forget to deallocate it
			RakNetworkFactory::DestroyRakPeerInterface(m_networkInterface);
		}

		void ChatPeerPlugin::reset( void )
		{

		}

		void ChatPeerPlugin::update( const float currentTime, const float elapsedTime )
		{
			// first try to process all network packets
			receivedPackets();
		}

		void ChatPeerPlugin::receivedPackets()
		{
			if (m_networkInterface != NULL)
			{
				Packet* p;
				while ( (p = m_networkInterface->Receive()) != NULL)
				{

					//do not forget to deallocate the packet
					m_networkInterface->DeallocatePacket(p);
				}
			}
		}

		void ChatPeerPlugin::redraw( const float currentTime, const float elapsedTime )
		{
			m_receivedMesssages.redraw(Vec3(5, drawGetWindowHeight()-30, 0));
		}

		void ChatPeerPlugin::handleFunctionKeys( int keyNumber )
		{

		}

		void ChatPeerPlugin::printMiniHelpForFunctionKeys( void ) const
		{

		}

		const AVGroup& ChatPeerPlugin::allVehicles( void ) const
		{
			static AVGroup pGroup;
			return pGroup;
		}

		AVGroup& ChatPeerPlugin::allVehicles( void )
		{
			static AVGroup pGroup;
			return pGroup;
		}

		void ChatPeerPlugin::onTypedMessage()
		{
			enString_t& chatMessage = m_pMessageTextBox->text;
			std::ostringstream msg;
			msg << "Want to send \"" << chatMessage.c_str() << "\"" << std::endl;
			Log::printMessage(msg);

			sendMessage(chatMessage);

			m_pMessageTextBox->set_text("");
		}

		void ChatPeerPlugin::sendMessage( const enString_t& content )
		{
			// TODO: send message to the other peers

			// for now forward to our own received list
			onReceivedMessage("Me",content);
		}

		void ChatPeerPlugin::onReceivedMessage( const enString_t& sender, const enString_t& content )
		{
			if (content.empty() == false)
			{
				m_receivedMesssages.add(sender, content);
			}
		}

		//////////////////////////////////////////////////////////////////////////

		void MessageList::add( const enString_t& sender, const enString_t& content )
		{
			messages.push_back(Message(sender, content));
		}

		MessageList::MessageList( boost::uint32_t size /*= 10*/ )
		{
			messages = List(size);
		}

		void MessageList::redraw(const Vec3& pos) const
		{
			Vec3 linePos(pos);
			
			const float windowWidth(drawGetWindowWidth());
			const float windowHeight(drawGetWindowHeight());
			
			// create regular xpression to split message content
			sregex re= sregex::compile("\\n+");
			
			float xOld = linePos.x;
			BOOST_FOREACH(const Message& msg, messages)
			{
				std::string strTRec = to_simple_string(msg.timeStamp);
				std::ostringstream str;
				str << "[" << strTRec.c_str() << "] " << msg.sender.c_str() << " - ";
				
				sregex_token_iterator begin( msg.content.begin(), msg.content.end(), re, -1 ), end;
				while (begin != end)
				{
					str << (*begin).str().c_str() << std::endl;
					draw2dTextAt2dLocation (str, linePos, gGreen, windowWidth, windowHeight);

					//reset stream
					str.seekp(0);

					linePos.y -= 11;
					linePos.x = xOld + 100;
					++begin;
				}
				linePos.x = xOld;
			}
			
		}

	}
}
