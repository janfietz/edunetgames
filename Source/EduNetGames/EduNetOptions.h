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
private:
	bool m_bContinueProcess;
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