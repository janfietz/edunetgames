#include "PlugInArray.h"

using namespace OpenSteer;


//-----------------------------------------------------------------------------
PlugInArray::PlugInArray(bool bAddToRegistry)
{

}

//-----------------------------------------------------------------------------
PlugInArray::~PlugInArray()
{
	this->removeAllPlugIns();
}
//-----------------------------------------------------------------------------
void PlugInArray::removeAllPlugIns( void )
{
	AbstractPlugin* pkPlugin = this->getPlugIn( 0 );
	while( NULL != pkPlugin )
	{
		this->removePlugIn( pkPlugin );
		pkPlugin = this->getPlugIn( 0 );
	}
	bool bTest = true;
	bTest = false;
}
//-----------------------------------------------------------------------------
void PlugInArray::addPlugIn( AbstractPlugin* pkPlugin )
{
	AbstractPlugInPtr spPlugin(pkPlugin);
	this->push_back( spPlugin );
}

//-----------------------------------------------------------------------------
void PlugInArray::removePlugIn( AbstractPlugin* pkPlugin )
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkArrayPlugin = (*kIter).get();
		if( pkArrayPlugin == pkPlugin )
		{
			pkArrayPlugin->close();
			this->erase( kIter );
			break;
		}
		++kIter;
	}
}

//-----------------------------------------------------------------------------
AbstractPlugin* PlugInArray::getPlugIn( size_t uiIdx ) const
{
	if( uiIdx < this->size() )
	{
		return (*this)[uiIdx].get();
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void PlugInArray::open(void)
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->open();
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PlugInArray::update(const float currentTime, const float elapsedTime)
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->update( currentTime, elapsedTime );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PlugInArray::redraw(const float currentTime, const float elapsedTime)
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->redraw( currentTime, elapsedTime );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PlugInArray::close(void)
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->close(  );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PlugInArray::reset(void)
{
	this->close();
	this->open();
/*
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->reset(  );
		++kIter;
	}
*/
}

//-----------------------------------------------------------------------------
const char* PlugInArray::name(void) const
{
	return "PlugInArray";
}

//-----------------------------------------------------------------------------
float PlugInArray::selectionOrderSortKey(void) const
{
	return 1.0f;
}

//-----------------------------------------------------------------------------
bool PlugInArray::requestInitialSelection(void) const
{
	return false;
}

//-----------------------------------------------------------------------------
void PlugInArray::handleFunctionKeys(int keyNumber)
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->handleFunctionKeys( keyNumber );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PlugInArray::printMiniHelpForFunctionKeys(void) const
{
	TPlugInArray::const_iterator kIter = this->begin();
	TPlugInArray::const_iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugin* pkPlugin = (*kIter).get();
		pkPlugin->printMiniHelpForFunctionKeys( );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
const AVGroup& PlugInArray::allVehicles(void) const
{
	return this->m_kVehicles;
}

//-----------------------------------------------------------------------------
class FooPlugIn : public Plugin
{
	ET_DECLARE_BASE(OpenSteer::Plugin);
public:
	FooPlugIn (bool bAddToRegistry = false):BaseClass(bAddToRegistry){};

	// required methods:
	const char* name (void) const {return "Foo";}
	void open (void) { }
	void update (const float currentTime, const float elapsedTime) { }
	void redraw (const float currentTime, const float elapsedTime) { }
	void close (void) { }
	const AVGroup& allVehicles (void) const { return m_kVehicles; }

	// optional methods (see comments in AbstractPlugin for explanation):
	void reset (void) { } // default is to reset by doing close-then-open
	float selectionOrderSortKey (void) const {return 1234;}
	bool requestInitialSelection (void) const {return true;}
	void handleFunctionKeys (int keyNumber) { } // fkeys reserved for PlugIns
	void printMiniHelpForFunctionKeys (void) { } // if fkeys are used
private:
	AVGroup m_kVehicles;
	ET_IMPLEMENT_CLASS_NO_COPY(FooPlugIn);

};

//-----------------------------------------------------------------------------
AbstractPlugin* PlugInArray::next(void) const 
{ 
	return OpenSteer::Plugin::findNextPlugin( this );
};


//PlugInArray gTestPluginArray;

void PlugInArray::TestPluginArray( void )
{
	{
		PlugInArray gTestPluginArray;

		PlugInArray* pkAdd[] = 
		{
			new PlugInArray,
			new PlugInArray,
			new PlugInArray
		};
		gTestPluginArray.addPlugIn( pkAdd[0] );
		gTestPluginArray.addPlugIn( pkAdd[1] );
		gTestPluginArray.addPlugIn( pkAdd[2] );


		gTestPluginArray.removePlugIn( pkAdd[0] );

		bool bTest = true;
		bTest = false;
	}

}
