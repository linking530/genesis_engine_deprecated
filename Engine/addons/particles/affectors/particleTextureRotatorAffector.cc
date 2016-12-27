/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU

http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "stdneb.h"
#include "particleTextureRotatorAffector.h"
#include "particles/particleaffector.h"
#include "particles/particlesystem.h"
#include "particles/particle.h"

namespace Particles
{
	using namespace Math;

	__ImplementClass(Particles::TextureRotatorAffector, CPFCC::AFFECTOR_TEXTUREROTATOR, Particles::ParticleAffector);

	//-----------------------------------------------------------------------
	TextureRotatorAffector::TextureRotatorAffector(void) : ParticleAffector()
	{
		mAffectType = AT_Rotation;
		mName = "TextureRotatorAffector";
	}
	//------------------------------------------------------------------------
	void TextureRotatorAffector::_preProcessParticles(void)
	{

	}
	//-----------------------------------------------------------------------
	void TextureRotatorAffector::_affect(Particle* particle)
	{

		if(!GetEnable())
			return;
		float percent = particle->mTimeFraction;

		float rotatorAngularVelocity = mMinMaxRotation.Calculate(percent,particle->mRandom0);

		float rotatorRadian = rotatorAngularVelocity * N_PI / 180;
		particle->mZRotation = particle->mZRotation + (float)(rotatorRadian * mParentSystem->GetCurrentFrameTime());

	}

}