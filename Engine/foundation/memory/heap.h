/****************************************************************************
Copyright (c) 2006, Radon Labs GmbH
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
#pragma once

#include "core/config.h"
#if (__WIN64__ || __WIN32__)
#include "memory/win360/win360heap.h"
namespace Memory
{
typedef Win360::Win360Heap Heap;
}
#elif __WII__
#include "memory/wii/wiiheap.h"
namespace Memory
{
typedef Wii::WiiHeap Heap;
}
#elif __PS3__
#include "memory/ps3/ps3heap.h"
namespace Memory
{
typedef PS3::PS3Heap Heap;
}
#elif __OSX__
#include "memory/osx/osxheap.h"
namespace Memory
{
typedef OSX::OSXHeap Heap;
}
#elif __ANDROID__
#include "memory/android/androidheap.h"
namespace Memory
{
typedef Android::AndroidHeap Heap;
}
#else
#error "IMPLEMENT ME!"
#endif
