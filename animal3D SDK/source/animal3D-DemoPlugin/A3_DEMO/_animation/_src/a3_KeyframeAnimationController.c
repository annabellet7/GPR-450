/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

/*
	Short description of code goes here.

	Author: Victor Diab, Olivia Santamauro, Annabelle Thompson
	Class: GPR-450 Advanced Animation Programming
	Assignment: Interactive Keyframe & Time Control

	Certification of Authenticity:
	I certify that this is entirely my own work, except where I have given
	fully-documented references to the work of others. I understand the definition
	and consequences of plagiarism and acknowledge that the assessor of this
	assignment may, for the purpose of assessing this assignment:
	- Reproduce this assignment and provide a copy to another member of academic
	- staff; and/or Communicate a copy of this assignment to a plagiarism checking
	- service (which may then retain a copy of this assignment on its database for
	- the purpose of future plagiarism checking)
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}



// update clip controller
// Authors: Victor, Olivia, Annabelle
//	-Code blocks do not have individual authors as most code was 
//	-written while we were working together in a group.
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3f64 timeStep;
		switch (clipCtrl->state)
		{
		case(FORWARD):
			timeStep = dt;
			break;
		case(BACKWARD):
			timeStep = -dt;
			break;
		case(STOP):
			timeStep = 0;
			break;
		default:
			timeStep = 0;
		}
		clipCtrl->clipTime_sec += timeStep;
		clipCtrl->keyframeTime_sec += timeStep;
		
		//if clip time exits clip bounds
		while (/*time is unresolved*/clipCtrl->clipTime_sec > clipCtrl->clip->duration_sec || clipCtrl->clipTime_sec < 0)
		{
			//time is greater than or equal to the clip duration (forward)
			if (timeStep >= 0)
			{	
				//loops back to first key frame after last one
				if (clipCtrl->clip->transitionForward->flag == a3clip_playFlag)
				{
					a3f64 overstep = clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec;
					clipCtrl->keyframeIndex = 0;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = overstep;
					clipCtrl->clipTime_sec = overstep;
				}
				//ping-pongs once the end of the clip is reached 
				else if (clipCtrl->clip->transitionForward->flag == a3clip_reverseFlag)
				{
					a3f64 overstep = clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec;
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount - 1;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec - overstep;
					clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec - overstep;

					//flag is changed to backward so dt is properly changed 
					clipCtrl->state = dt > 0 ? BACKWARD : FORWARD;
				}
				//playback stops once end of clip is reached
				else if (clipCtrl->clip->transitionForward->flag == a3clip_stopFlag)
				{
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount - 1;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec;
					clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec;

					//flag is changed so dt is 0
					clipCtrl->state = STOP;
				}
			}
			//time is less than 0 in respect to the clip (backwards)
			else if (timeStep < 0)
			{
				//loops back to last keyframe after first one
				if (clipCtrl->clip->transitionReverse->flag == a3clip_playFlag)
				{
					a3f64 overstep = clipCtrl->clipTime_sec;
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount - 1;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec + overstep;
					clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec + overstep;
				}
				//ping-pongs once the beginning of the clip is reached
				else if (clipCtrl->clip->transitionReverse->flag == a3clip_reverseFlag)
				{
					a3f64 overstep = clipCtrl->clipTime_sec;
					clipCtrl->keyframeIndex = 0;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = -overstep;
					clipCtrl->clipTime_sec = -overstep;

					//flag is changed to backward so dt is properly changed 
					clipCtrl->state = dt < 0 ? BACKWARD : FORWARD;
				}
				//playback stops once start of clip is reached
				else if (clipCtrl->clip->transitionReverse->flag == a3clip_stopFlag)
				{
					clipCtrl->keyframeIndex = 0;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					clipCtrl->keyframeTime_sec = 0;
					clipCtrl->clipTime_sec = 0;

					//flag is changed so dt is 0
					clipCtrl->state = STOP;
				}
			}
		}
		
		//if keyframe time exits keyframe bounds
		while(clipCtrl->keyframeTime_sec >= clipCtrl->keyframe->duration_sec || clipCtrl->keyframeTime_sec < 0)
		{
			//time is greater than or equal to the keyframe duration (forward)
			if (timeStep >= 0)
			{
				//failsafe to make sure we dont go past the amount of keyframes in the clip
				if (clipCtrl->keyframeIndex == clipCtrl->clip->keyframeCount - 1)
				{
					break;
				}					

				clipCtrl->keyframeTime_sec -= clipCtrl->keyframe->duration_sec;
				clipCtrl->keyframeIndex++;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
			}
			//time is less than 0 in respect to the keyframe (backwards)
			else
			{
				clipCtrl->keyframeIndex--;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				clipCtrl->keyframeTime_sec += clipCtrl->keyframe->duration_sec;
			}
		}

		//normalize clip and keyframe time 
		clipCtrl->clipParam = clipCtrl->clipTime_sec / clipCtrl->clip->duration_sec;
		clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec / clipCtrl->keyframe->duration_sec;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
