#****************************************************************************
# Copyright (c) 2011-2013,WebJet Business Division,CYOU
#  
# http://www.genesis-3d.com.cn
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#****************************************************************************

##################################################################################
# Build InAppPurchase 
##################################################################################



# folder
SET ( _HEADER_FILES 
	InAppPurchase.h
	ALSystem.h
)

# folder
SET ( _SOURCE_FILES
	InAppPurchase.cc
		ALSystem.cc
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/extincludes	
	${CMAKE_SOURCE_DIR}/depends/boostWraper

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/
	${CMAKE_SOURCE_DIR}/
)

ADD_LIBRARY(
	ALSystem
	STATIC 
	#header
	${_HEADER_FILES}
	#source
	${_SOURCE_FILES}
 )
ADD_DEFINITIONS(
	-D_UNICODE
	-DUNICODE
)

 

#Organize projects into folders
SET_PROPERTY(TARGET ALSystem PROPERTY FOLDER "3.Addons")

#Compile properties
_MACRO_TREAT_WARRINGS_AS_ERRORS( ALSystem )

