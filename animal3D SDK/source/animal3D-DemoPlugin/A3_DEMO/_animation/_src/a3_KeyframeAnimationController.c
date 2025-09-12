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
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 		   Olivia Notes
//-----------------------------------------------------------------------------
// 
		//1. pre-resolution: apply time step: single line of code applied twice to increment "keyframe time" and "clip time" by the time step
//	a. time step: take in only a positive time step (1/30 of dt)
//	b. numberical integration for time itself
// 
		clipCtrl->clipTime_sec += dt;
		clipCtrl->keyframeTime_sec += dt;
// 
//2. resolve time: while unresolved, continue playback behavior to determine new "keyframe time" and "clip time"
//	a. 7 cases to check for resolution total
//		i. resolve keyframe
//			1. paused: dt = 0
//			2. forward: dt > 0
//				a. stop
//				b. step(s) taken
//				b. step(s) taken
//				c. clip exited
//			3. reverse: dt < 0
//				a. stop
//				b. step(s) taken
//				c. clip exited
//			4. loop
//				a. calculate overstep t-t1
//				b. t -> overstep + t0 
//				c. direction = direction
//			5. ping-pong
//				a. t = t1 - overstep
//				b. 1 -> 3
//				c. direction = -direction
//			6. (go to and) stop
//				a. t = duration of clip
//				b. i = ??? (i = 3 or 4, problems either way)
//				c. direction = 0
//			7. it says 7 but I only counted 6 :(
		
		//if clip time exits clip bounds
		while (/*time is unresolved*/clipCtrl->clipTime_sec >= clipCtrl->clip->duration_sec || clipCtrl->clipTime_sec < 0)
		{
			if (dt > 0)
			{
				//time is greater than or equal to the clip duration
				
				//looping
				// calculate overstep (how far into the clip/keyframe time should be after loop)
				//		?: offset = clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec
				//set keyframe index to 0
				//add offset
				//check if in correct keyframe

				a3f64 overstep = clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec;
				clipCtrl->keyframeIndex = 0;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				clipCtrl->keyframeTime_sec = overstep;
				clipCtrl->clipTime_sec = overstep;
				/*while (clipCtrl->keyframeTime_sec >= clipCtrl->keyframe->duration_sec || clipCtrl->keyframeTime_sec < 0)
				{
					clipCtrl->keyframeTime_sec -= clipCtrl->keyframe->duration_sec;
					clipCtrl->keyframeIndex++;
				}*/
			}
			else if (dt < 0)
			{
				//time is less than 0 in respect to the clip

				//looping
				//pretty much same as above
				// ?: offset = clipCtrl->clipTimme_sec
				//set keyframe index to clipCtrl->clip->keyframeCount(- 1?)
				//add offset (offset should be negative)
				//check correct keyframe
				a3f64 overstep = clipCtrl->clipTime_sec;
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount - 1;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec + overstep;
				clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec + overstep;
				/*while (clipCtrl->keyframeTime_sec >= clipCtrl->keyframe->duration_sec || clipCtrl->keyframeTime_sec < 0)
				{
					clipCtrl->keyframeIndex--;
					clipCtrl->keyframeTime_sec += clipCtrl->keyframe->duration_sec;
				}*/
			}// if we need else is dt == 0
		}
		
		//if keyframe timme exits keyframe bounds
		while(clipCtrl->keyframeTime_sec >= clipCtrl->keyframe->duration_sec || clipCtrl->keyframeTime_sec < 0)
		{
			//same thing as exiting clip bounds but with keyframe
			if (dt >= 0)
			{
				clipCtrl->keyframeTime_sec -= clipCtrl->keyframe->duration_sec;
				clipCtrl->keyframeIndex++;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				//clipCtrl->keyframeIndex = clipCtrl->clip->keyframeCount == clipCtrl->keyframeIndex ? 0 : clipCtrl->keyframeIndex;
			}
			else
			{
				clipCtrl->keyframeIndex--;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				clipCtrl->keyframeTime_sec += clipCtrl->keyframe->duration_sec;
			}
		}
// 
//3. post-resolution: normalize time/parameters: one time, single line of code applied twice to calculate normalized "keyframe time" and "clip time"
//	a. normalize keyframe/clip time: relative time / duration
//	b. 0-1 (not clamped)

		clipCtrl->clipParam = clipCtrl->clipTime_sec / clipCtrl->clip->duration_sec;
		clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec / clipCtrl->keyframe->duration_sec;
//4. add a new instance of the clip controller, new data, and link them to something in the scene to produce a new animation effect
//	a. required for team of 3
// 
//BONUS. Parse the custom clip data file format provided to automate the preparation of clips and keyframe data
// 
// 
//-----------------------------------------------------------------------------
//			Victor Notes
//-----------------------------------------------------------------------------


		//step 2 check if you're on another keyframe (resolve keyframe)
		// while realtime is greater than t1 move to the next time
		//relative time -= duration of the keyframe  &  keyframe moves to the next one
		//pause
		//forward
		//	stop
		//	step(s) taken
		//  clip exited
		//reverse
		//	stop
		//	step(s) taken
		//  clip exited

		//step 3 evaluate where you are in your keyframe
		//u = (t-t0)/(t1-t0)  ||  this is the value from 0-1 of how far into the clip we are
		//u: [0, 1)

		// keep stepping condition below, exit is the opposite
		//while(t>=t1 || t<t0)

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
