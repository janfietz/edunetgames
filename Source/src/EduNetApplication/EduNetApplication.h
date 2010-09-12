#ifndef __EDUNETAPPLICATION_H__
#define	__EDUNETAPPLICATION_H__
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


//-----------------------------------------------------------------------------
#include "glui/glui_internal_control.h"
#include "EduNetCommon/TUpdatePeriod.h"
#include "EduNetCommon/EduNetCommon.h"

namespace EduNet
{
	
	extern void initializeStaticPlugins( );
	extern void shutdownStaticPlugins( );
	
//-----------------------------------------------------------------------------
class Application
{
public:

	void addGuiElements(GLUI *glui);

	// do a simulation update for the currently selected plug-in
	void updateSelectedPlugin(const float currentTime, const float elapsedTime);

	// redraw graphics for the currently selected plug-in
	void redrawSelectedPlugin(const float currentTime, const float elapsedTime);

	//! draw profiler output
	void drawProfile(const float currentTime, const float elapsedTime);

	bool isProfileVisible(void) const;
	bool isOpenSteerProfileVisible(void) const;

	void onPluginSelected(OpenSteer::AbstractPlugin* pkPlugin);

	void initialize(void);


	void initializeGraphics(int argc, char **argv);
	void runGraphics(void);

	static Application& AccessApplication(void);

	static void _SDMInit(void);
	static void _SDMCleanup(void);
	static void _SDMShutdown(void);

	bool allowLocalPlayer(void)
	{
		return m_bAllowLocalPlayer;
	}
	void setAllowLocalPlayer(bool bValue)
	{
		m_bAllowLocalPlayer = bValue;
	}

	float m_fSimulationFPS;
	float m_fTimeFactor;
	int m_bFixedSimulationFPS;
	int m_bEnableAnnotation;

	int m_bShowOpenSteerProfile;
	int m_bShowCPUProfile;
	int m_bShowCPUProfileGraph;
	int m_bUpdateCPUProfile;

private:
	Application(void);
	virtual ~Application(void);

	TUpdatePeriod<osScalar, FloatMathLimits> m_kUpdatePeriod;
	OpenSteer::Clock m_kUpdateClock;
	osScalar m_fUpdateCPUTime;
	bool m_bAllowLocalPlayer;

	OpenSteer::PluginArray m_plugins;

ET_DECLARE_SINGLETON(Application);

};

}

#endif // __EDUNETAPPLICATION_H__
