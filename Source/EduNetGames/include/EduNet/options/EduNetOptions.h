#ifndef __EDUNETOPTIONS_H__
#define __EDUNETOPTIONS_H__


#include "EduNet/common/EduNetCommon.h"


//-----------------------------------------------------------------------------
class EduNetOptions
{
public:
	EduNetOptions();
	virtual ~EduNetOptions();

	int parseCommandLine(int argc, char **argv);

	void setup( void );

	bool continueProcess( void ) const;
	void setContinueProcess( bool bValue );

	static const char* getAppName( void );

	void setSelectedPlugin( const char* pluginName)
	{
		this->m_kPluginName.assign(pluginName);
	}
	const char* getSelectedPlugin( void )
	{
		return this->m_kPluginName.c_str();
	}
private:
	bool m_bContinueProcess;
	std::string m_kPluginName;
};

//-----------------------------------------------------------------------------
inline
bool EduNetOptions::continueProcess( void ) const 
{ 
	return this->m_bContinueProcess; 
}

//-----------------------------------------------------------------------------
inline
void EduNetOptions::setContinueProcess( bool bValue ) 
{ 
	this->m_bContinueProcess = bValue; 
}

#endif  // __EDUNETOPTIONS_H__