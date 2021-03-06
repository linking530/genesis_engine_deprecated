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
#ifndef _APPRENDEROBJECT_H_
#define _APPRENDEROBJECT_H_
#include "graphicsystem/Renderable/RenderObject.h"
namespace App
{
	class RenderComponent;
	class AppRenderObject : public Graphic::RenderObject
	{
		__DeclareSubClass(AppRenderObject, Graphic::RenderObject)
	public:
		virtual ~AppRenderObject();

		virtual void AddToCollection(Graphic::RenderDataCollection* collection, const Graphic::RenderConfig* renderConfig);
		virtual void OnWillRenderObject(Graphic::Camera* sender);
		void SetOwner(RenderComponent* owner);
		template<typename T> 
		T* GetOwner() const;
	protected:	
		virtual void setShaderMask(Graphic::Renderable* pRenderable, const Graphic::RenderConfig* renderConfig);
		void setModelParam(const Graphic::Renderable* renderable, Graphic::RenderPassType passType, const Graphic::Material* material);

		AppRenderObject();
		RenderComponent* mOwner;
	};

	template<typename T> 
	inline T* AppRenderObject::GetOwner() const
	{
		return static_cast<T*>(this->mOwner);
	}
}


#endif // _APPRENDEROBJECT_H_
