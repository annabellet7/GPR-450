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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// fread
				// -> create (pose group function)

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// fwrite
				// -> 

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; i++)
		{
			a3real4x4TransformInverse(state->localSpaceInv->hpose_base[i].transformMat.m,
				state->localSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		//author: class demo code
		for (i = 0; i < state->hierarchy->numNodes; i++)
		{
			a3real4x4TransformInverse(state->objectSpaceInv->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		//author: class demo code
		for (i = 0; i < state->hierarchy->numNodes; i++)
		{
			a3real4x4Product(state->objectSpaceBindToCurrent->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m,
				state_bind->objectSpaceInv->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		FILE *file = fopen(resourceFilePath, "r");

		//https://www.w3schools.com/c/c_ref_stdio.php stdio file reading
		//https://www.w3schools.com/c/c_ref_string.php string.h c strings
		//https://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/HTR.html 

		{
			//order of the sections is enforced [Header], then [SegmentNames&Hierarchy], then [BasePosition], then [Frames]
			//note: I'm pretty sure in the header of HTR files the order of fields isn't enforced

			size_t part = 0;
			float secondsPerSample;
			float frameRate;
			float globalScale = 100;//100 is for intended units (cm)

			char charBuffer[256];
			char extraBuffer[256];

			//i think this is right but not sure about bitwise operations here also scale could be just x since we only get one scale value in this file, 
			//	but i put it as 3 since it would theoretically effect x, y, and z directions
			poseGroup_out->channel = malloc(sizeof(a3_SpatialPoseChannel) * 4);
			*poseGroup_out->channel = a3poseChannel_rotate_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz | a3poseChannel_user_xyz;

			while (!feof(file))
			{
				//printf("number %zu\n", part);
				strcpy(charBuffer, "");
				strcpy(extraBuffer, "");
				switch (part)
				{
				case 0: //Before Header
					fgets(charBuffer, sizeof(charBuffer), file);
					
					size_t len = strlen(charBuffer);
					if (strcmp(charBuffer, "[Header]\n") == 0)
						part++;
					
					break;
				case 1: //Header
					if (fgets(charBuffer, sizeof(charBuffer), file))
					{
						if (strcmp(charBuffer, "[SegmentNames&Hierarchy]\n\0") == 0)
							part++;
						else if (strncmp(charBuffer, "NumSegments ", strlen("NumSegments ")) == 0)
						{
							strcpy(extraBuffer, charBuffer + strlen("NumSegments "));
							hierarchy_out->numNodes = atoi(extraBuffer);
						}
						else if (strncmp(charBuffer, "NumFrames ", strlen("NumFrames ")) == 0)
						{
							strcpy(extraBuffer, charBuffer + strlen("NumFrames "));
							hierarchy_out->numNodes = atoi(extraBuffer);
						}
						else if (strncmp(charBuffer, "DataFrameRate ", strlen("DataFrameRate ")) == 0)
						{
							strcpy(extraBuffer, charBuffer + strlen("DataFrameRate "));
							frameRate = (float)atoi(extraBuffer);
							secondsPerSample = 1 / frameRate;
						}
						else if (strncmp(charBuffer, "EulerRotationOrder ", strlen("EulerRotationOrder ")) == 0)//Might be a problem here with memory and c, not sure
						{
							strcpy(extraBuffer, charBuffer + strlen("EulerRotationOrder "));
							
							if (strncmp(extraBuffer, "ZYX", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_zyx;
							}
							else if (strncmp(extraBuffer, "XZY", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_xzy;
							}
							else if (strncmp(extraBuffer, "YXZ", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_yxz;
							}
							else if (strncmp(extraBuffer, "ZXY", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_zxy;
							}
							else if (strncmp(extraBuffer, "YZX", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_yzx;
							}
							else if (strncmp(extraBuffer, "XYZ", 3) == 0)
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_xyz;
							}
							else
							{
								poseGroup_out->order = malloc(sizeof(a3_SpatialPoseEulerOrder));
								*poseGroup_out->order = a3poseEulerOrder_zyx;
							}
						}
						else if (strncmp(charBuffer, "CalibrationUnits ", strlen("CalibrationUnits ")) == 0)//only implementing mm
						{
							strcpy(extraBuffer, charBuffer + strlen("CalibrationUnits "));
							if (strncmp(extraBuffer, "mm", strlen("mm")))
								globalScale *= (float)0.001;
							frameRate = (float)atoi(extraBuffer);
							secondsPerSample = 1 / frameRate;
						}
						else if (strncmp(charBuffer, "ScaleFactor ", strlen("ScaleFactor ")) == 0)
						{
							strcpy(extraBuffer, charBuffer + strlen("CalibrationUnits "));
							globalScale *= (float)atof(extraBuffer);
						}
						else
						{
							//Things we arent doing stuff for: FileType (assumed HTR), DataType (assumed HTRS), FileVersion (assumed as 1), 
							//	RotationUnits (assumed to be degrees, but double check), GlobalAxisofGravity (assumed to be Y), BoneLengthAxis(assumed it is Y)
						}
					}
					break;
				case 2: //Hierarchy
					for (a3ui32 i = 0; i < hierarchy_out->numNodes; i++)
					{
						fgets(charBuffer, sizeof(charBuffer), file);

						//strcspn for length of a string up to the first occurence of char
						
						strncpy(extraBuffer, charBuffer, strcspn(charBuffer, "\t"));
						extraBuffer[strcspn(charBuffer, "\t")] = '\0';
						
						/*int bleh;
						memcpy(bleh, extraBuffer, strlen(extraBuffer));

						memcpy(extraBuffer, charBuffer, bleh);*/
						if (hierarchy_out->nodes != NULL)
						{
							hierarchy_out->nodes[i].index = i;
							strcpy(hierarchy_out->nodes[i].name, extraBuffer);
							hierarchy_out->nodes[i].parentIndex = -1;

							strcpy(extraBuffer, "");
							strncpy(extraBuffer, charBuffer + strcspn(charBuffer, "\t") + 1, strcspn(charBuffer, "\n") - (strcspn(charBuffer, "\t") + 1));
							extraBuffer[strlen(charBuffer) - strcspn(charBuffer, "\t") - 2] = '\0';

							for (a3ui32 j = 0; j < i; j++)
							{
								if (strcmp(hierarchy_out->nodes[j].name, extraBuffer) == 0)
									hierarchy_out->nodes[i].parentIndex = hierarchy_out->nodes[j].index;
							}
						}
						else
						{
							hierarchy_out->nodes = malloc(hierarchy_out->numNodes * sizeof(a3_HierarchyNode));

							hierarchy_out->nodes[i].index = 0;
							strcpy(hierarchy_out->nodes[i].name, extraBuffer);
							hierarchy_out->nodes[i].parentIndex = -1;
						}
					}

					while (true)
					{
						if (feof(file)) break;
						
						fgets(charBuffer, sizeof(charBuffer), file);
						if (strncmp(charBuffer, "[BasePosition]\n", strlen("[BasePosition]\n")) == 0)
						{
							part++;
							break;
						}
					}
					
					
					printf(charBuffer);
					break;
				case 3: //Base Position
					fgets(charBuffer, sizeof(charBuffer), file);
					break;
				case 4: //Other Poses
					break;
				default:
					fgets(charBuffer, sizeof(charBuffer), file);
					break;
				}
			}
		}

		//should concat to get hierarchy pose

		//poseGroup_out->channel

		//used to test if hierarchy is defined properly
		/*for (a3ui32 i = 0; i < hierarchy_out->numNodes; i++)
		{
			printf(hierarchy_out->nodes[i].name);
			printf("\t%d", hierarchy_out->nodes[i].index);
			printf("\t%d\n", hierarchy_out->nodes[i].parentIndex);
		}*/

		fclose(file);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
