/** 
 * @file llfloaterxuitrace.cpp
 * @brief LLFloaterXUITrace class implementation
 *
 * $LicenseInfo:firstyear=2002&license=viewergpl$
 * 
 * Copyright (c) 2002-2009, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlifegrid.net/programs/open_source/licensing/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlifegrid.net/programs/open_source/licensing/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 */

#include "llviewerprecompiledheaders.h"

#include "llfloaterxuitrace.h"

#include "llerror.h"
#include "llstring.h"
#include "llviewertexteditor.h"
#include "lluictrlfactory.h"
#include "llviewercontrol.h"

//
// Member Functions
//
LLFloaterXUITrace::LLFloaterXUITrace()
:	LLFloater(std::string("trace floater"))
{
	LLUICtrlFactory::getInstance()->buildFloater(this,"floater_xui_trace.xml");
}

LLFloaterXUITrace::~LLFloaterXUITrace()
{
	// Children all cleaned up by default view destructor.
}

BOOL LLFloaterXUITrace::postBuild()
{
	LLView::setGlobalTraceConsoleCallback( cbAddTrace, this);

	return TRUE;
}

// static
void LLFloaterXUITrace::cbAddTrace(LLView::trace_info& info, void* userdata)
{
	LLFloaterXUITrace* self = (LLFloaterXUITrace*)userdata;
	self->addTrace(info);
}

void LLFloaterXUITrace::addTrace(LLView::trace_info& info)
{
	LLColor4 color = gColors.getColor( "TextTraceColor" );

	LLViewerTextEditor*	trace_editor = getChild<LLViewerTextEditor>("XUI trace display");

	trace_editor->setParseHTML(TRUE);
	trace_editor->setParseHighlights(TRUE);

	trace_editor->appendTime(true);

	std::string message = *info.mFunction;
	
	if (info.mChannel && info.mChannel->length() > 0)
	{
		message += " /" + (*info.mChannel);
	}
	
	message += " \"" + (*info.mName) + "\" " + (*info.mAction);

	if (info.mResult)
	{
		message += " " + (*info.mResult);
	}

	trace_editor->appendColoredText(message, false, false, color);
}

// public virtual
void LLFloaterXUITrace::onClose(bool app_quitting)
{
	if(this == LLView::getGlobalTraceConsoleUserdata() )
	{
		LLView::setGlobalTraceConsoleCallback(NULL, NULL);
	}

	LLUICtrlFactory::getInstance()->removeFloater(this);

	// destroy ourselves
	LLFloater::onClose (app_quitting);
}

void LLFloaterXUITrace::setTraceCursorAndScrollToEnd()
{
	LLViewerTextEditor*	trace_editor = getChild<LLViewerTextEditor>("XUI trace display");
	
	if (trace_editor) 
	{
		trace_editor->setCursorAndScrollToEnd();
	}
}