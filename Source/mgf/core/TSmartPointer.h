#ifndef __MGFTSMARTPOINTER_H__
#define __MGFTSMARTPOINTER_H__


#ifndef __specstrings
#include "specstrings.h"
#endif

#define MGF_SMARTPOINTER(classname) namespace mgf {\
	typedef rnTSmartPointer<classname> classname##Ptr;\
}

//-----------------------------------------------------------------------------
template <class Type>
class rnTSmartPointer
{
public:
	rnTSmartPointer() throw();
	rnTSmartPointer(__in_opt Type* pkInstance) throw();
	rnTSmartPointer(__in const rnTSmartPointer& kPtr) throw();
	virtual ~rnTSmartPointer();

	rnTSmartPointer& operator=(__in const rnTSmartPointer& kPtr) throw();
	rnTSmartPointer& operator=(__in_opt Type* pkInstance) throw();

	bool operator==(__in_opt Type* pkPtr) const throw();
	bool operator!=(__in_opt Type* pkPtr) const throw();
	bool operator==(__in const rnTSmartPointer& kPtr) const throw();
	bool operator!=(__in const rnTSmartPointer& kPtr) const throw();
	bool IsNull() const throw();

	operator Type*() const throw();
	operator Type&() const throw();
	Type& operator*() const throw();
	Type* operator->() const throw();

protected:
	void SetInstance( Type* pkInstance ) throw();

private:
	Type* m_pkInstance;


};

//-----------------------------------------------------------------------------
template<class Type>
void rnTSmartPointer<Type>::SetInstance( Type* pkInstance ) throw()
{
	if(m_pkInstance != pkInstance)
	{
		Type* const pkPrevInstance = m_pkInstance;
		m_pkInstance = pkInstance;
//		Type* const pkPrevInstance = static_cast<Type*>(::InterlockedExchangePointer(&m_pkInstance, pkInstance));
		if(m_pkInstance != NULL)
		{
			m_pkInstance->AddRef();
		}
		if(pkPrevInstance != NULL)
		{
			pkPrevInstance->Release();
		}
	}
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::rnTSmartPointer():m_pkInstance(NULL)
{
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::rnTSmartPointer(Type* pkInstance):m_pkInstance(NULL)
{
	SetInstance( pkInstance );
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::rnTSmartPointer(const rnTSmartPointer& kPtr):m_pkInstance(NULL)
{
	SetInstance( kPtr.m_pkInstance );
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::~rnTSmartPointer()
{
	SetInstance( NULL );
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>& rnTSmartPointer<Type>::operator=(const rnTSmartPointer<Type>& kPtr) throw()
{
	SetInstance( kPtr.m_pkInstance );
	return *this;
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>& rnTSmartPointer<Type>::operator=(Type* pkInstance) throw()
{
	SetInstance( pkInstance );
	return *this;
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::operator Type*() const throw()
{
	return m_pkInstance;
}

//-----------------------------------------------------------------------------
template<class Type>
rnTSmartPointer<Type>::operator Type&() const throw()
{
	return *m_pkInstance;
}

//-----------------------------------------------------------------------------
template<class Type>
Type& rnTSmartPointer<Type>::operator*() const throw()
{
	return *m_pkInstance;
}

//-----------------------------------------------------------------------------
template<class Type>
Type* rnTSmartPointer<Type>::operator->() const throw()
{
	return m_pkInstance;
}

//-----------------------------------------------------------------------------
template<class T>
bool rnTSmartPointer<T>::operator==(T* pkPtr) const throw()
{
	return m_pkInstance == pkPtr;
};

//-----------------------------------------------------------------------------
template<class T>
bool rnTSmartPointer<T>::operator==(const rnTSmartPointer<T>& kPtr) const throw()
{
	return m_pkInstance == kPtr.m_pkInstance;
};

//-----------------------------------------------------------------------------
template<class T>
bool rnTSmartPointer<T>::operator!=(T* pkPtr) const throw()
{
	return m_pkInstance != pkPtr;
};

//-----------------------------------------------------------------------------
template<class T>
bool rnTSmartPointer<T>::operator!=(const rnTSmartPointer<T>& kPtr) const throw()
{
	return m_pkInstance != kPtr.m_pkInstance;
};

//-----------------------------------------------------------------------------
template<class Type>
bool rnTSmartPointer<Type>::IsNull() const throw()
{
	return (m_pkInstance == NULL);
}



#endif // __MGFTSMARTPOINTER_H__