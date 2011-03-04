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

#include "OpenSteer/AABBox.h"
#include "OpenSteer/AbstractVehicle.h"

//-----------------------------------------------------------------------------
namespace OpenSteer {

	AABBox::AABBox( const Vec3 &min, const Vec3& max ): m_min( min ), m_max( max )
	{
		Vec3 distance = m_max - m_min;
		distance *= 0.5f;
		m_center = m_min + distance;
		m_extent = distance;
	}

	void AABBox::initializeWithCenterAndExtent( const Vec3 &center, const Vec3& extent )
	{
		m_center = center;
		m_extent = extent;
		m_min = m_center;
		m_min -= m_extent;
		m_max = m_center;
		m_max += m_extent;
	}

	bool AABBox::insideX(const Vec3& p) const 
	{
		if(p.x < m_min.x || p.x > m_max.x)	
			return false;
		return true;
	}

	bool AABBox::insideY(const Vec3& p) const 
	{
		if(p.y < m_min.y || p.y > m_max.y)	
			return false;
		return true;
	}

	bool AABBox::insideZ(const Vec3& p) const 
	{
		if(p.z < m_min.z || p.z > m_max.z)	
			return false;
		return true;
	}

	bool AABBox::inside( const Vec3& p ) const
	{
		if( false == this->insideX( p ) )
		{
			return false;
		}
		if( false == this->insideY( p ) )
		{
			return false;
		}
		if( false == this->insideZ( p ) )
		{
			return false;
		}
		return true;
	}

	bool AABBox::inside( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		return this->inside(position);
	}

	bool AABBox::insideXZ( const Vec3& p ) const
	{
		if( false == this->insideX( p ) )
		{
			return false;
		}
		if( false == this->insideZ( p ) )
		{
			return false;
		}
		return true;
	}

	bool AABBox::insideXZ( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		return this->insideXZ( position );
	}

	bool AABBox::insideXZWithRadius( const AbstractLocalSpace& localSpace ) const
	{
		const Vec3& position = localSpace.position();
		osScalar radius = 0.0f;
		const AbstractVehicle* vehicle = dynamic_cast<const AbstractVehicle*>(&localSpace);
		if( NULL != vehicle )
		{
			radius = vehicle->radius();
		}
		if( 0 != radius )
		{
			// increase with radius
			Vec3 tempExtent( this->m_extent );
 			tempExtent.x += radius;
			tempExtent.y += radius;
			tempExtent.z += radius;
			AABBox tempBox;
			tempBox.initializeWithCenterAndExtent( this->m_center, tempExtent );
			return tempBox.insideXZ( position );
		}
		return this->insideXZ( position );
	}

	void AABBox::draw( OpenSteer::AbstractRenderer* pRenderer, const OpenSteer::Color& color ) const 
	{
		Vec3 b,c;
		b = Vec3( m_min.x, 0, m_max.z);
		c = Vec3( m_max.x, 0, m_min.z);
		pRenderer->drawLineAlpha(m_min, b, color, 1.0f);
		pRenderer->drawLineAlpha(b, m_max, color, 1.0f);
		pRenderer->drawLineAlpha(m_max, c, color, 1.0f);
		pRenderer->drawLineAlpha(c,m_min, color, 1.0f);
	}

	void AABBox::draw( OpenSteer::AbstractRenderer* pRenderer ) const 
	{
		Vec3 b,c;
		b = Vec3( m_min.x, 0, m_max.z);
		c = Vec3( m_max.x, 0, m_min.z);
		Color color(1.0f,1.0f,0.0f);
		pRenderer->drawLineAlpha(m_min, b, color, 1.0f);
		pRenderer->drawLineAlpha(b, m_max, color, 1.0f);
		pRenderer->drawLineAlpha(m_max, c, color, 1.0f);
		pRenderer->drawLineAlpha(c,m_min, color, 1.0f);
	}

} //! namespace OpenSteer    



