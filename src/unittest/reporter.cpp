// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
// ----------------------------------------------------------------------------

#include "reporter.hpp"

namespace
{
	FLASH_STRING(invaildName) = "invalid";
	
	FLASH_STRING(failHeader) = "FAIL: ";
	FLASH_STRING(failColon) = " : ";
	
	FLASH_STRING(reportPassed) = "\nPassed ";
	FLASH_STRING(reportFailed) = "\nFailed ";
	FLASH_STRING(reportOf) = " of ";
	FLASH_STRING(reportTests) = " tests\n";
	FLASH_STRING(reportOk) = "OK!\n";
	FLASH_STRING(reportFail) = "FAIL!\n";
}

unittest::Reporter::Reporter(xpcc::IODevice* device) :
	outputStream(device), testName(xpcc::toFlashPointer(invaildName)),
	testsPassed(0), testsFailed(0)
{
}

void
unittest::Reporter::nextTestSuite(xpcc::FlashPointer<char> name)
{
	testName = name;
}

void
unittest::Reporter::reportPass()
{
	testsPassed++;
}

xpcc::IOStream&
unittest::Reporter::reportFailure(unsigned int lineNumber)
{
	testsFailed++;
	outputStream << xpcc::toFlashPointer(failHeader)
				 << testName
				 << ':'
				 << lineNumber
				 << xpcc::toFlashPointer(failColon);
	return outputStream;
}

void
unittest::Reporter::printSummary()
{
	if (testsFailed == 0) {
		outputStream << xpcc::toFlashPointer(reportPassed)
					 << testsPassed
					 << xpcc::toFlashPointer(reportTests)
					 << xpcc::toFlashPointer(reportOk)
					 << xpcc::endl;
	}
	else {
		outputStream << xpcc::toFlashPointer(reportFailed)
					 << testsFailed
					 << xpcc::toFlashPointer(reportOf)
					 << (testsFailed + testsPassed)
					 << xpcc::toFlashPointer(reportTests)
					 << xpcc::toFlashPointer(reportFail)
					 << xpcc::endl;
	}
}
