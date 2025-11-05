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
	
	a3_HierarchyStateBlend.c
	Implementation of hierarchical blend operations.
*/

#include "../a3_HierarchyStateBlend.h"
#include "stdlib.h"


//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-4: IMPLEMENT ME
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

// create blend tree
a3ret a3spatialPoseBlendTreeCreate(a3_SpatialPoseBlendTree* blendTree, a3_Hierarchy const* blendTreeDescriptor)
{
	blendTree = malloc(sizeof(a3_SpatialPoseBlendTree));
	if(blendTree)
		blendTree->nodes = malloc(sizeof(a3_SpatialPoseBlendNode) * blendTreeDescriptor->numNodes);

	if (!blendTree || !blendTreeDescriptor)
		return -1;

	return 0;
}

// release blend tree
a3ret a3spatialPoseBlendTreeRelease(a3_SpatialPoseBlendTree* blendTree)
{
	if (!blendTree)
		return -1;
	if(blendTree->nodes)
		free(blendTree->nodes);//might have to iterate we do not know c
	
	free(blendTree);

	return 0;
}

// configure node internally; set pointers
a3ret a3spatialPoseBlendTreeConfigureNode(a3_SpatialPoseBlendTree const* blendTree, a3ui32 const nodeIndex)
{
	if (!blendTree)
		return -1;

	//blendTree->nodes[nodeIndex].pose_ctrl = malloc(sizeof(*blendTree->nodes[nodeIndex].pose_ctrl) * blendTree->nodes[nodeIndex].vCount);
	//blendTree->nodes[nodeIndex].u = ;

	return 0;
}

// execute tree from leaves to root
a3ret a3spatialPoseBlendTreeExecute(a3_SpatialPoseBlendTree const* blendTree)
{
	if (!blendTree)
		return -1;
	
	for (int i = blendTree->blendTreeDescriptor->numNodes - 1; i >= 0; i--)
	{
		//blendTree->nodes[i].blendOpSet->exec(blendTree->nodes[i].pose_out, blendTree->nodes[i].pose_ctrl, );
	}
	
	return 0;
}


//-----------------------------------------------------------------------------

a3real4r a3blendOpRET4(a3real4 v_out)
{
	return v_out;//already completed
}

a3real4r a3blendOpZERO4(a3real4 v_out)
{
	a3real4Set(v_out, 0, 0, 0, 0);
	return v_out;
}

a3real4r a3blendOpONE4(a3real4 v_out)
{
	a3real4Set(v_out, 1, 1, 1, 1);
	return v_out;
}

a3real4r a3blendOpID4(a3real4 v_out)
{
	a3real4Set(v_out, 1, 0, 0, 0); //idk, guessing
	return v_out;
}

a3real4r a3blendOpCOPY4(a3real4 v_out, a3real4 const v)
{
	a3real4SetReal4(v_out, v);
	return v_out;
}

a3real4r a3blendOpNEGATE4(a3real4 v_out, a3real4 const v)
{
	a3real4GetNegative(v_out, v);
	return v_out;
}

a3real4r a3blendOpRECIP4(a3real4 v_out, a3real4 const v)
{
	v_out[0] = 1 / v[0];
	v_out[1] = 1 / v[1];
	v_out[2] = 1 / v[2];
	v_out[3] = 1 / v[3];
	return v_out;
}

a3real4r a3blendOpCONJQ4(a3real4 v_out, a3real4 const v)
{
	a3real4Set(v_out, v[0], -v[1], -v[2], -v[3]);
	return v_out;
}

a3real4r a3blendOpADD4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real4Sum(v_out, v0, v1);
	return v_out;
}

a3real4r a3blendOpSUB4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real4Diff(v_out, v0, v1);
	return v_out;
}

a3real4r a3blendOpMUL4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real4ProductComp(v_out, v0, v1);
	return v_out;
}

a3real4r a3blendOpDIV4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real4QuotientComp(v_out, v0, v1);
	return v_out;
}

a3real4r a3blendOpMULQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real3 vi0, vi1, crossV;
	a3real d;
	a3real3Set(vi0, v0[1], v0[2], v0[3]);
	a3real3Set(vi1, v1[1], v1[2], v1[3]);
	d = a3real3Dot(vi0, vi1);
	a3real3Cross(crossV, vi0, vi1);
	v_out[0] = (v0[0] * v1[0]) - d;
	v_out[1] = v0[0] * v1[1] + v1[0] * v0[1] + crossV[0];
	v_out[2] = v0[0] * v1[2] + v1[0] * v0[2] + crossV[1];
	v_out[3] = v0[0] * v1[3] + v1[0] * v0[3] + crossV[2];
	return v_out;
}

a3real4r a3blendOpMULCONJQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1)
{
	a3real4 conjV1;
	a3blendOpCONJQ4(conjV1, v1);
	a3blendOpMULQ4(v_out, v0, conjV1);
	return v_out;
}

a3real4r a3blendOpSCALE4(a3real4 v_out, a3real4 const v, a3real const u)
{
	a3real4ProductS(v_out, v, u);
	return v_out;
}

a3real4r a3blendOpPOW4(a3real4 v_out, a3real4 const v, a3real const u)
{
	a3real4SetReal4(v_out, v);
	if (u == 1)
	{
		return v_out;
	}
	else
	{
		for (a3real i = 1; i < u; i++)
		{
			a3real4ProductComp(v_out, v_out, v);
		}
	}
	return v_out;
}

a3real4r a3blendOpNEAR4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u)
{
	if (u < 0.5)
	{
		a3real4SetReal4(v_out, v0);
	}
	else if (u >= 0.5)
	{
		a3real4SetReal4(v_out, v1);
	}
	return v_out;
}

a3real4r a3blendOpLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u)
{
	a3real4Lerp(v_out, v0, v1, u);
	return v_out;
}

a3real4r a3blendOpNLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u)
{
	a3real4NLerp(v_out, v0, v1, u);
	return v_out;
}

a3real4r a3blendOpSLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u)
{
	a3real4Slerp(v_out, v0, v1, u);
	return v_out;
}


a3real4r a3blendOpRET4X2(a3real4 m_out)
{
	return m_out;//already completed
}

a3real4r a3blendOpID4X2(a3real4 m_out)
{
	a3blendOpID4(m_out + 0);
	a3blendOpZERO4(m_out + 4);
	return m_out;
}

a3real4r a3blendOpCOPY4X2(a3real4 m_out, a3real4 const m)
{
	a3blendOpCOPY4(m_out + 0, m + 0);
	a3blendOpCOPY4(m_out + 4, m + 4);
	return m_out;
}

a3real4r a3blendOpCONJDQ4X2(a3real4 m_out, a3real4 const m)
{
	a3blendOpCONJQ4(m_out + 0, m + 0);
	a3blendOpCOPY4(m_out + 4, m + 4);
	return m_out;
}

a3real4r a3blendOpMULDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1)
{
	// (a + bE)(c + dE)
	//	= ac + adE + bcE + bdE^2
	//	= ac + (ad + bc)E
	return m_out;
}

a3real4r a3blendOpMULCONJDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1)
{
	return m_out;
}

a3real4r a3blendOpSCALE4X2(a3real4 m_out, a3real4 const m, a3real const u)
{
	return m_out;
}

a3real4r a3blendOpNEAR4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	return m_out;
}

a3real4r a3blendOpLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	return m_out;
}

a3real4r a3blendOpNLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	return m_out;
}

a3real4r a3blendOpSCLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	// sclerp(m0, m1, u)
	return m_out;
}


a3real4r a3blendOpRET4X4(a3real4 m_out)
{
	return m_out;//already completed
}

a3real4r a3blendOpID4X4(a3real4 m_out)
{
	//a3real4x4SetIdentity(m_out);
	return m_out;
}

a3real4r a3blendOpCOPY4X4(a3real4 m_out, a3real4 const m)
{
	//a3real4x4SetReal4x4(m_out, m);
	return m_out;
}

a3real4r a3blendOpINVR4X4(a3real4 m_out, a3real4 const m)
{
	//a3real4x4GetInverse(m_out, m);
	return m_out;
}

a3real4r a3blendOpMULM4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1)
{
	//a3real4x4Product(m_out, m0, m1);
	return m_out;
}

a3real4r a3blendOpMULINVR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1)
{
	return m_out;
}

a3real4r a3blendOpSCALE4X4(a3real4 m_out, a3real4 const m, a3real const u)
{
	//a3real4x4ProductS(m_out, m, u);
	return m_out;
}

a3real4r a3blendOpNEAR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	/*if (u < 0.5)
	{
		a3real4x4SetReal4x4(m_out, m0);
	}
	else if (u >= 0.5)
	{
		a3real4x4SetReal4x4(m_out, m1);
	}*/
	return m_out;
}

a3real4r a3blendOpLERP4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u)
{
	return m_out;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	pose_out->transformMat = a3mat4_identity;
	// ...

	// done
	return pose_out;
}

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out)
{

	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-4
//-----------------------------------------------------------------------------
