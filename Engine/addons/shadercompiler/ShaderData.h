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
#ifndef _SHADERUNIT_H_
#define _SHADERUNIT_H_

#include "core/refcounted.h"
#include "util/string.h"
#include "util/array.h"

namespace ShaderProgramCompiler
{
	class GpuProgramCompiler;
	class ShaderAnalyzer;

class ShaderData : public Core::RefCounted 
{
	__DeclareSubClass(ShaderData, Core::RefCounted);

public:

	ShaderData();

	virtual ~ShaderData();

public:

	void SetName(const Util::String& name);

	void Init();

	void Compile();

	Util::String Output() const;

private:

	Util::String    m_Name;
	Util::String    m_OriginalContent;

	ShaderAnalyzer*     m_pAnalyzer;     
	GpuProgramCompiler* m_pCompilerD3D9;
	GpuProgramCompiler* m_pCompilerGLES;
};

inline void ShaderData::SetName(const Util::String& name)
{
	m_Name = name;
}

}

#endif