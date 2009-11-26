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

#include "AbstractVehicleMath.h"
#include "EduNetCommon/EduNetMath.h"
#include "EduNetCommon/TCompressed.h"


namespace OpenSteer
{

	//-------------------------------------------------------------------------
	using namespace OpenSteer;

	//-----------------------------------------------------------------------------
	osVector3& getVector3( const btVector3& kSource, osVector3& kTarget )
	{
		kTarget.x = kSource.getX();
		kTarget.y = kSource.getY();
		kTarget.z = kSource.getZ();
		return kTarget;
	}

	//-------------------------------------------------------------------------
	btVector3& getVector3( const osVector3& kSource, btVector3& kTarget )
	{
		kTarget.setX( kSource.x );
		kTarget.setY( kSource.y );
		kTarget.setZ( kSource.z );
		return kTarget;
	}

	// test conversion
	// 	EduNet::EmptyVehicle v1, v2;
	// 	btTransform kWorldTransform;
	// 	btTransform kTargetWorldTransform;
	// 	kWorldTransform.setIdentity();
	// 	readFromMatrix( v2, kWorldTransform );
	// 	writeToMatrix( v2, kTargetWorldTransform );
	//-------------------------------------------------------------------------
	void readFromRotationMatrix( osLocalSpaceData& kLocalSpace, const btMatrix3x3& kWorldRotation )
	{
		// calculate local coordinate system
		const btVector3& kRow0 = kWorldRotation.getRow(0);
		const btVector3& kRow1 = kWorldRotation.getRow(1);
		const btVector3& kRow2 = kWorldRotation.getRow(2);
		btVector3 kAxis[3];
		kAxis[0].setX( kRow0.getX() );
		kAxis[0].setY( kRow1.getX() );
		kAxis[0].setZ( kRow2.getX() );

		kAxis[1].setX( kRow0.getY() );
		kAxis[1].setY( kRow1.getY() );
		kAxis[1].setZ( kRow2.getY() );

		kAxis[2].setX( kRow0.getZ() );
		kAxis[2].setY( kRow1.getZ() );
		kAxis[2].setZ( kRow2.getZ() );


#if OS_HAS_BULLET
		kLocalSpace._side = kAxis[0];
		kLocalSpace._forward = kAxis[1];
		kLocalSpace._up = kAxis[2];
#else
		osVector3 kTemp;
#if OPENSTEER_Z_ISUP
		kLocalSpace._side = getVector3( kAxis[0], kTemp );
		kLocalSpace._forward = getVector3( kAxis[1], kTemp );
		kLocalSpace._up = getVector3( kAxis[2], kTemp );
#else
		kLocalSpace._side = -getVector3( kAxis[0], kTemp );
		kLocalSpace._forward = getVector3( kAxis[2], kTemp );
		kLocalSpace._up = getVector3( kAxis[1], kTemp );
#endif
#endif
	}

	//-------------------------------------------------------------------------
	void readFromMatrix( osLocalSpaceData& kLocalSpace, const btTransform& kWorldTransform )
	{
		//	const btTransform& kWorldTransform = pkRigidBody->getWorldTransform();
		const btMatrix3x3& kWorldRotation = kWorldTransform.getBasis();
		OpenSteer::readFromRotationMatrix( kLocalSpace, kWorldRotation );
#if OS_HAS_BULLET
		kLocalSpace._position = kWorldTransform.getOrigin();
#else
		osVector3 kTemp;
		kLocalSpace._position = getVector3( kWorldTransform.getOrigin(), kTemp );
#endif
	}

	//-------------------------------------------------------------------------
	void writeToRotationMatrix( const osLocalSpaceData& kLocalSpace, btMatrix3x3& kWorldRotation )
	{
#if OS_HAS_BULLET
		const btVector3 kTargetRow[3] =
		{
			kVehicle.side(),
			kVehicle.forward(),
			kVehicle.up(),
		};
#else
		btVector3 kTemp;
		const btVector3 kTargetRow[3] =
		{
#if OPENSTEER_Z_ISUP
			getVector3( kLocalSpace._side, kTemp ),
			getVector3( kLocalSpace._forward, kTemp ),
			getVector3( kLocalSpace._up, kTemp ),
#else
			getVector3( -kLocalSpace._side, kTemp ),
			getVector3( kLocalSpace._up, kTemp ),
			getVector3( kLocalSpace._forward, kTemp ),
#endif
		};
#endif
		kWorldRotation.setValue( 
			kTargetRow[0].x(), kTargetRow[1].x(), kTargetRow[2].x(),
			kTargetRow[0].y(), kTargetRow[1].y(), kTargetRow[2].y(),
			kTargetRow[0].z(), kTargetRow[1].z(), kTargetRow[2].z());
	}

	//-------------------------------------------------------------------------
	bool writeToMatrix( const osLocalSpaceData& kLocalSpace, btTransform& kWorldTransform )
	{
		if( false == OpenSteer::isValidVector( kLocalSpace._forward ) )
		{
			return false;
		}
		if( false == OpenSteer::isValidVector( kLocalSpace._side ) )
		{
			return false;
		}
		if( false == OpenSteer::isValidVector( kLocalSpace._up ) )
		{
			return false;
		}
		if( false == OpenSteer::isValidVector( kLocalSpace._position ) )
		{
			return false;
		}

#if OS_HAS_BULLET
		kWorldTransform.setOrigin( kLocalSpace._position );
#else
		btVector3 kTemp;
		kWorldTransform.setOrigin( getVector3( kLocalSpace._position, kTemp ) );
#endif
		btMatrix3x3& kWorldRotation = kWorldTransform.getBasis();
		OpenSteer::writeToRotationMatrix( kLocalSpace, kWorldRotation );
		return true;
	}

	//-------------------------------------------------------------------------
	void readFromMatrix( osAbstractLocalSpace& kLocalSpace, const btTransform& kWorldTransform )
	{
		OpenSteer::readFromMatrix( kLocalSpace.accessLocalSpaceData(), kWorldTransform );
	}

	//-------------------------------------------------------------------------
	void readFromMatrix( osAbstractVehicle& kVehicle, const btTransform& kWorldTransform )
	{
		OpenSteer::readFromMatrix( kVehicle.accessLocalSpaceData(), kWorldTransform );
	}

	//-------------------------------------------------------------------------
	bool writeToMatrix( const osAbstractVehicle& kVehicle, btTransform& kWorldTransform )
	{
		return OpenSteer::writeToMatrix( kVehicle.getLocalSpaceData(), kWorldTransform );
	}

	//-------------------------------------------------------------------------
	void calculateVelocity( const btTransform& kWorldTransform0, const btTransform& kWorldTransform1,
		osScalar fDeltaTime,
		osVector3& kLinearVelocity, osVector3& kAngularVelocity )
	{
		btVector3 _LinearVelocity, _AngularVelocity;
		btTransformUtil::calculateVelocity( 
			kWorldTransform0, kWorldTransform1, fDeltaTime, 
			_LinearVelocity, _AngularVelocity );
		getVector3( _LinearVelocity, kLinearVelocity );
		getVector3( _AngularVelocity, kAngularVelocity );
	}

	//-------------------------------------------------------------------------
	void localToWorldSpace( osAbstractVehicle& kVehicle, const osVector3& kSource, osVector3& kTarget )
	{
		btTransform kWorldTransform;
		writeToMatrix( kVehicle, kWorldTransform );
		btVector3 _kSource, _kWorld;
		getVector3( kSource, _kSource );
		_kWorld = kWorldTransform.getBasis() * _kSource;
		getVector3( _kWorld, kTarget );
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::worldDirectionToLocal( const OpenSteer::LocalSpaceData& kLocalSpaceData, const osVector3& kWorld )
	{
		osVector3 kTarget;
		btTransform kWorldTransform;
		writeToMatrix( kLocalSpaceData, kWorldTransform );
		btVector3 _kSource, _kLocal;
		getVector3( kWorld, _kSource );
		_kLocal = kWorldTransform.getBasis().inverse() * _kSource;
		getVector3( _kLocal, kTarget );
		return kTarget;
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::localDirectionToWorld( const OpenSteer::LocalSpaceData& kLocalSpaceData, const osVector3& kLocal )
	{
		osVector3 kTarget;
		btTransform kWorldTransform;
		writeToMatrix( kLocalSpaceData, kWorldTransform );
		btVector3 _kSource, _kWorld;
		getVector3( kLocal, _kSource );
		_kWorld = kWorldTransform.getBasis() * _kSource;
		getVector3( _kWorld, kTarget );
		return kTarget;
	}

	//-------------------------------------------------------------------------
	btQuaternion AbstractVehicleMath::readRotation( const OpenSteer::LocalSpaceData& kLocalSpaceData )
	{
		btMatrix3x3 kWorldRotation;
		OpenSteer::writeToRotationMatrix( kLocalSpaceData, kWorldRotation );
		btQuaternion kRotation;
		kWorldRotation.getRotation( kRotation );
		kRotation.normalize();
		return kRotation;
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::writeRotation( const btQuaternion& kRotation, OpenSteer::LocalSpaceData& kLocalSpaceData )
	{
		if( false == OpenSteer::isValidQuaterion( kRotation ) )
		{
			return;
		}
		btMatrix3x3 kWorldRotation;
		kWorldRotation.setRotation( kRotation );
		OpenSteer::readFromRotationMatrix( kLocalSpaceData, kWorldRotation );
	}

	//-------------------------------------------------------------------------
	osVector3 AbstractVehicleMath::compressQuaternion( const btQuaternion& kRotation, char& wSign )
	{
		btQuaternion kUnitRotation = kRotation.normalized();
		osVector3 kCompressed( kUnitRotation.getX(), kUnitRotation.getY(), kUnitRotation.getZ() );
		wSign = ( kUnitRotation.getW() < 0 ) ? -1 : 1;
		return kCompressed;
	}

	//-------------------------------------------------------------------------
	btQuaternion AbstractVehicleMath::expandQuaternion( const osVector3& kCompressed, float wSign )
	{
		// A unit quaternion has the following property
		// w2 + x2 + y2 + z2=1
		// ->
		//	w2 = 1 - (x2 + y2 + z2)
		// w = sqrt( 1 - (x2 + y2 + z2) )
		btQuaternion kUnitRotation( kCompressed.x, kCompressed.y, kCompressed.z, 0 );
		kUnitRotation.setW( 
			wSign * btSqrt( btScalar(1) - 
			( kUnitRotation.getX() * kUnitRotation.getX() +
			kUnitRotation.getY() * kUnitRotation.getY() +
			kUnitRotation.getZ() * kUnitRotation.getZ()
			) ) );
		return kUnitRotation;
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::compressUnitVector( const osVector3& kSource, char* kTarget )
	{
		const osScalar x = ::etClamp( kSource.x, -1.0f, 1.0f );
		const osScalar y = ::etClamp( kSource.y, -1.0f, 1.0f );
		const osScalar z = ::etClamp( kSource.z, -1.0f, 1.0f );

		kTarget[0] = TCompressedFixpoint<float,char,8>::writeCompress( x , -1.0f, 1.0f );
		kTarget[1] = TCompressedFixpoint<float,char,8>::writeCompress( y , -1.0f, 1.0f );
		kTarget[2] = TCompressedFixpoint<float,char,8>::writeCompress( z , -1.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::expandUnitVector( const char* kSource, osVector3& kTarget  )
	{
		kTarget.x = TCompressedFixpoint<float,char,8>::readInflate( kSource[0] , -1.0f, 1.0f );
		kTarget.y = TCompressedFixpoint<float,char,8>::readInflate( kSource[1] , -1.0f, 1.0f );
		kTarget.z = TCompressedFixpoint<float,char,8>::readInflate( kSource[2] , -1.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::compressFixedLengthVector( const osVector3& kSource, float fMaxLength, CompressedVector& kTarget )
	{
		osVector3 kFixedSource = kSource;
		kFixedSource.truncateLength( fMaxLength );
		const float fLength = kFixedSource.length();
		osVector3 kUnitSource = kFixedSource;
		float fUnitFactor;
		if( fLength > 0 )
		{
			kUnitSource /= fLength;
			kUnitSource.x = etClamp( kUnitSource.x, -1.0f, 1.0f );
			kUnitSource.y = etClamp( kUnitSource.y, -1.0f, 1.0f );
			kUnitSource.z = etClamp( kUnitSource.z, -1.0f, 1.0f );
			fUnitFactor = fLength / fMaxLength;
			fUnitFactor = etClamp( fUnitFactor, 0.0f, 1.0f );
		}
		else
		{
			fUnitFactor = 
			kUnitSource.x = 
			kUnitSource.y = 
			kUnitSource.z = 0;
		}
		kTarget.m_cValues[0] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.x , -1.0f, 1.0f );
		kTarget.m_cValues[1] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.y , -1.0f, 1.0f );
		kTarget.m_cValues[2] = TCompressedFixpoint<float,char,8>::writeCompress( kUnitSource.z , -1.0f, 1.0f );
		kTarget.m_cUnitFactor = TCompressedFixpoint<float,short,16>::writeCompress( fUnitFactor, 0.0f, 1.0f );
	}

	//-------------------------------------------------------------------------
	void AbstractVehicleMath::expandFixedLengthVector( const CompressedVector& kSource, float fMaxLength, osVector3& kTarget  )
	{
		// expand
		kTarget.x = TCompressedFixpoint<float,char,8>::readInflate( kSource.m_cValues[0] , -1.0f, 1.0f );
		kTarget.y = TCompressedFixpoint<float,char,8>::readInflate( kSource.m_cValues[1] , -1.0f, 1.0f );
		kTarget.z = TCompressedFixpoint<float,char,8>::readInflate( kSource.m_cValues[2] , -1.0f, 1.0f );
		const float fLength = kTarget.length();
		if( fLength > 0 )
		{
			kTarget /= fLength;
			const float fUnitFactor = TCompressedFixpoint<float,short,16>::readInflate( kSource.m_cUnitFactor, 0.0f, 1.0f );
			kTarget *= fUnitFactor;
			kTarget *= fMaxLength;
		}
	}



} // end namespace OpenSteer

