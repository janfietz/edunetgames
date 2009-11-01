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
	AbstractPlugIn* pkPlugin = this->getPlugIn( 0 );
	while( NULL != pkPlugin )
	{
		this->removePlugIn( pkPlugin );
		pkPlugin = this->getPlugIn( 0 );
	}
	bool bTest = true;
	bTest = false;
}
//-----------------------------------------------------------------------------
void PlugInArray::addPlugIn( AbstractPlugIn* pkPlugin )
{
	AbstractPlugInPtr spPlugin(pkPlugin);
	this->push_back( spPlugin );
}

//-----------------------------------------------------------------------------
void PlugInArray::removePlugIn( AbstractPlugIn* pkPlugin )
{
	TPlugInArray::iterator kIter = this->begin();
	TPlugInArray::iterator kEnd = this->end();
	while( kIter != kEnd  )
	{
		AbstractPlugIn* pkArrayPlugin = (*kIter).get();
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
AbstractPlugIn* PlugInArray::getPlugIn( size_t uiIdx ) const
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
		AbstractPlugIn* pkPlugin = (*kIter).get();
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
class FooPlugIn : public PlugIn
{
	ET_DECLARE_BASE(OpenSteer::PlugIn);
public:
	FooPlugIn (bool bAddToRegistry = false):BaseClass(bAddToRegistry){};

	// required methods:
	const char* name (void) const {return "Foo";}
	void open (void) { }
	void update (const float currentTime, const float elapsedTime) { }
	void redraw (const float currentTime, const float elapsedTime) { }
	void close (void) { }
	const AVGroup& allVehicles (void) const { return m_kVehicles; }

	// optional methods (see comments in AbstractPlugIn for explanation):
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
AbstractPlugIn* PlugInArray::next(void) const 
{ 
	return OpenSteer::PlugIn::findNextPlugin( this );
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
