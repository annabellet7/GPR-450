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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"


//-----------------------------------------------------------------------------

// single FK helpers
static inline void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_object] = T[parent_object] * T[this_local]
	a3real4x4Product(
		hierarchyState->objectSpace->hpose_base[index].transformMat.m,		// Result: this node object-space.
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->localSpace->hpose_base[index].transformMat.m		// Right-hand: this node local space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_object] = T[root_local]
	hierarchyState->objectSpace->hpose_base[index] = hierarchyState->localSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement forward kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveForwardSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveForwardRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

// single IK helpers
static inline void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_local] = T[parent_object]^-1 * T[this_object]
	a3real4x4Product(
		hierarchyState->localSpace->hpose_base[index].transformMat.m,		// Result: this node local space.
		hierarchyState->objectSpaceInv->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->objectSpace->hpose_base[index].transformMat.m		// Right-hand: this node object space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	hierarchyState->localSpace->hpose_base[index] = hierarchyState->objectSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement inverse kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveInverseSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveInverseRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

void a3kinematicsUpdateHierarchyStateFK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//	-> concatenate base pose
		//	-> convert poses to local-space matrices
		//	-> perform recursive FK
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
			activeHS->animPose,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3kinematicsSolveForward(activeHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		//	-> perform recursive IK
		//	-> restore local-space matrices to poses
		//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3kinematicsSolveInverse(activeHS);
		a3hierarchyPoseRestore(
			activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3hierarchyPoseDeconcat(
			activeHS->animPose,
			activeHS->localSpace,
			baseHS->localSpace,
			activeHS->hierarchy->numNodes);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateSkin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		//	-> update local-space inverse matrices
		//	-> update object-space inverse matrices
		//	-> update transform from base to current
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}


//-----------------------------------------------------------------------------

// helper to resolve single-joint IK after solver
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj /*something basis related*/)
{
	// post-IK resolution for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	//similar to a3spatialPoseConvert in a3_spatialpose.c

	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);
	// objInv = obj x identity
	a3real4x4TransformInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m);
	
	//localMatrix = parentObjInv x nodeObj
	// -maybe use a3kinematicsSolveInverseSingle()
	a3kinematicsSolveInverseSingle(activeHS, nodeIndex, activeHS->hierarchy->nodes[nodeIndex].parentIndex);
	a3spatialPoseRestore(&activeHS->localSpace->hpose_base[nodeIndex], *poseGroup->channel, *poseGroup->order);
	a3spatialPoseDeconcat(&activeHS->animPose->hpose_base[nodeIndex], 
						  &activeHS->localSpace->hpose_base[nodeIndex], 
						  &baseHS->localSpace->hpose_base[nodeIndex]
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLookAtIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector,
	a3ui32 const hierarchyObjIndex_affected, a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected)
{
	a3mat3 m_hierarchyObj, m_affected;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	
	// FIRST STEP:
		// transform everything into the space of the skeleton/hierarchy (inverse function)
		//	-> look at target
	/*a3kinematicsSolveInverseSingle(activeHS,
								   hierarchyObjIndex_affected, 
								   poseGroup->hierarchy->nodes[hierarchyObjIndex_affected].parentIndex
	);*/

	// MAIN STEP:
		// solver: build an orthonormal basis (joint-to-object)
		// 1. direction basis = target - joint position

	
	//a3real4x4 mat;

	//a3real4x4Product(
	//	mat,		// Result: this node local space.
	//	baseHS->objectSpaceInv->hpose_base[hierarchyObjIndex_affected].transformMat.m,// Left-hand: parent node object-space.
	//	sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_effector].transformMat.m		// Right-hand: this node object space.
	//);

	a3real4x4Product(sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector].transformMat.m,
		sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m,
		sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.m);
	
	a3real4 basisVector, sideBasis, up, upBasis, targetPos, jointPos;
	jointPos[0] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.x;
	jointPos[1] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.y;
	jointPos[2] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.z;
	jointPos[3] = 0;

	targetPos[0] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector].transformMat.v3.x;
	targetPos[1] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector].transformMat.v3.y;
	targetPos[2] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector].transformMat.v3.z;
	targetPos[3] = 0;

	a3_BasisAxis upAxis, forwardAxis;
	a3basisExtract(&forwardAxis, &upAxis, basis_affected);
	switch (upAxis)
	{
	case basis_xp:
		a3real4Set(up, 1, 0, 0, 0);
		break;
	case basis_yp:
		a3real4Set(up, 0, 1, 0, 0);
		break;
	case basis_zp:
		a3real4Set(up, 0, 0, 1, 0);
		break;
	case basis_xn:
		a3real4Set(up, -1, 0, 0, 0);
		break;
	case basis_yn:
		a3real4Set(up, 0, -1, 0, 0);
		break;
	case basis_zn:
		a3real4Set(up, 0, 0, -1, 0);
		break;
	case basis_invalid:
		return;
		break;
	default:
		return;
		break;
	}

	a3real4x4 obj;
	a3real4x4 objInv;
	//a3real4x4MakeLookAt(obj, objInv, jointPos, targetPos, up);

	{
		a3real3Diff(obj[2], targetPos, jointPos);//direction basis
		a3real3Normalize(obj[2]);
		a3real3CrossUnit(obj[0], up, obj[2]);//side basis
		a3real3Cross(obj[1], obj[2], obj[0]);//up basis

		a3real4SetReal3W(obj[3], jointPos, a3real_one);
		obj[0][3] = obj[1][3] = obj[2][3] = a3real_zero;
		a3real4x4TransformInverseIgnoreScale(objInv, obj);
	}

	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, obj);
	return;

	a3real4Diff(basisVector, targetPos, jointPos);
		// 2. side basis = known up x direction basis


	a3real3Cross(sideBasis, up, basisVector);
			// cancels out if lookAt target is directly above character
		// 3. up basis = direction basis x side basis
	a3real3Cross(upBasis, basisVector, sideBasis);
		// 4. normalize all
	a3real3Normalize(basisVector);
	a3real3Normalize(sideBasis);
	a3real3Normalize(upBasis);
	//a3_Basis basis = a3basisInit(basis_yp, basis_zp);
	//a3basisToMat4();
	a3real4 lastLine;
	basisVector[3] = 0;
	sideBasis[3] = 0;
	upBasis[3] = 0;
	a3real4Set(lastLine, 0, 0, 0, 1);
	//a3real3x3MakeLookAt();
	a3real4x4 temptempMat, tempMat, orthobasis;
	a3real4x4SetMinors(temptempMat, basisVector, sideBasis, upBasis, lastLine);
	a3real4x4SetMajors(tempMat, basisVector, sideBasis, upBasis, lastLine);
	a3real4x4SetMinors(orthobasis, tempMat[1], tempMat[0], tempMat[2], tempMat[3]);
	//a3real4x4Set();
	// LAST STEP:
		// resolve every affected joint
		//a3kinematicResolvePostIK
	a3real4x4Product(tempMat, activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.m, tempMat);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, tempMat);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLimbIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector_end, a3ui32 const sceneGraphIndex_constraint,
	a3ui32 const hierarchyObjIndex_affected_end, a3ui32 const hierarchyObjIndex_affected_hinge, a3ui32 const hierarchyObjIndex_affected_base,
	a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected_end, a3_Basis const basis_affected_hinge, a3_Basis const basis_affected_base)
{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected_end.m, basis_affected_end))
		return;
	if (!a3basisToMat3(m_affected_hinge.m, basis_affected_hinge))
		return;
	if (!a3basisToMat3(m_affected_base.m, basis_affected_base))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// FIRST STEP:
		// transform everything into the space of the skeleton/hierarchy (inverse function)
		// -> wrist/ankle effector
		// -> pole vector constraint
	a3real4x4Product(sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector_end].transformMat.m,
		sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m,
		sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.m);

	a3real4x4Product(sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_constraint].transformMat.m,
		sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m,
		sceneGraphState->localSpace->hpose_base[sceneGraphIndex_constraint].transformMat.m);
	
	// MAIN STEP:
		// solve jpint-to-object for end, hinge, base (wrist, elbow, shoulder; ankle, knee, hip)
		a3real4 /*hingeForwardBasis, *//*endForwardBasis,*/ baseToEndForwardBasis, baseToPoleForwardBasis;
		//a3real4 hingeSideBasis, endSideBasis, baseSideBasis;
		//a3real4 hingeUpBasis, endUpBasis, baseUpBasis;
		a3real4 hingePos, endPos, basePos, effectorPos, constraintPos;
		a3real4 up;
		
		a3_BasisAxis upAxis, forwardAxis;
		a3basisExtract(&forwardAxis, &upAxis, basis_affected_base);
		switch (forwardAxis)
		{
		case basis_xp:
			a3real4Set(up, 1, 0, 0, 0);
			break;
		case basis_yp:
			a3real4Set(up, 0, 1, 0, 0);
			break;
		case basis_zp:
			a3real4Set(up, 0, 0, 1, 0);
			break;
		case basis_xn:
			a3real4Set(up, -1, 0, 0, 0);
			break;
		case basis_yn:
			a3real4Set(up, 0, -1, 0, 0);
			break;
		case basis_zn:
			a3real4Set(up, 0, 0, -1, 0);
			break;
		case basis_invalid:
			return;
			break;
		default:
			return;
			break;
		}
		
		// -> end position*
		endPos[0] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat.v3.x;
		endPos[1] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat.v3.y;
		endPos[2] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat.v3.z;
		endPos[3] = 0;
		
		// -> hinge position*
		hingePos[0] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat.v3.x;
		hingePos[1] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat.v3.y;
		hingePos[2] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat.v3.z;
		hingePos[3] = 0;

		// -> base position*
		basePos[0] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3.x;
		basePos[1] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3.y;
		basePos[2] = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3.z;
		basePos[3] = 0;

		// -> effector position*
		effectorPos[0] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector_end].transformMat.v3.x;
		effectorPos[1] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector_end].transformMat.v3.y;
		effectorPos[2] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_effector_end].transformMat.v3.z;
		effectorPos[3] = 0;

		// -> constraint position*
		constraintPos[0] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_constraint].transformMat.v3.x;
		constraintPos[1] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_constraint].transformMat.v3.y;
		constraintPos[2] = sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_constraint].transformMat.v3.z;
		constraintPos[3] = 0;
		
		// *if the target is too far away, you can just calculate the position between the base and the target
		a3real armLength = a3real4Distance(basePos, endPos);
		a3real effectorDist = a3real4Distance(basePos, effectorPos);

		if (effectorDist > armLength)
		{
			a3kinematicsUpdateLookAtIK(sceneGraphState, activeHS, baseHS, poseGroup, sceneGraphIndex_hierarchyObj, sceneGraphIndex_effector_end,
										hierarchyObjIndex_affected_base, basis_hierarchyObj, basis_affected_end);
			a3kinematicsUpdateLookAtIK(sceneGraphState, activeHS, baseHS, poseGroup, sceneGraphIndex_hierarchyObj, sceneGraphIndex_effector_end,
										hierarchyObjIndex_affected_hinge, basis_hierarchyObj, basis_affected_hinge);
			a3kinematicsUpdateLookAtIK(sceneGraphState, activeHS, baseHS, poseGroup, sceneGraphIndex_hierarchyObj, sceneGraphIndex_effector_end,
										hierarchyObjIndex_affected_end, basis_hierarchyObj, basis_affected_end);
			return;
		}

		// 1. base joint to end effector vector (and distance)
		a3real4Diff(baseToEndForwardBasis, effectorPos, basePos);

		// 2. base joint to pole vector constraint
		a3real4Diff(baseToPoleForwardBasis, constraintPos, basePos);

		// 3. plane normal = (base to pole) x (base to end)
		a3real3 planeNormal;
		a3real3CrossUnit(planeNormal, baseToPoleForwardBasis, baseToEndForwardBasis);

		// 4. geometric (heron's formula) or algebraic (law of cosines)
		// -> solves elbow position
		a3real3 heightDir;
		a3real3CrossUnit(heightDir, baseToEndForwardBasis, planeNormal);

		a3real baseHingeDist, hingeEndDist;
		baseHingeDist = a3real3Distance(basePos, hingePos);
		hingeEndDist = a3real3Distance(hingePos, endPos);
		a3real s = a3real_onehalf * (armLength + baseHingeDist + hingeEndDist);
		a3real A = a3sqrtf(s * (s - armLength) * (s - baseHingeDist) * (s - hingeEndDist));
		a3real H = (2 * A) / armLength;

		a3real D = a3sqrtf(baseHingeDist * baseHingeDist - H * H);
		a3real3 d;
		a3real3SetReal3(d, baseToEndForwardBasis);
		a3real3Normalize(d);
		a3real3ProductS(d, d, D);
		a3real3ProductS(heightDir, heightDir, H);
		a3real4 middlePos;
		a3real3Sum(middlePos, basePos, d); 
		a3real3Sum(middlePos, middlePos, heightDir);
		middlePos[3] = 0;

		a3real4x4 obj;
		a3real4x4 objInv;
		//a3real4x4MakeLookAt(obj, objInv, jointPos, targetPos, up);
		
		{
			a3real3Diff(obj[2], middlePos, basePos);//direction basis
			a3real3Normalize(obj[2]);
			a3real3CrossUnit(obj[0], up, obj[2]);//side basis
			a3real3Cross(obj[1], obj[2], obj[0]);//up basis

			a3real4SetReal3W(obj[3], basePos, a3real_one);
			obj[0][3] = obj[1][3] = obj[2][3] = a3real_zero;
			a3real4x4TransformInverseIgnoreScale(objInv, obj);
		}

		a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_base, obj);
		//a3real3Proj();

		// 5. "look at" solves shoulder and elbow rotations

	// LAST STEP:
		// resolve every affected joint
		//  -> work from root to leaf
		//a3kinematicResolvePostIK (closest to root)
		//a3kinematicResolvePostIK
		//a3kinematicResolvePostIK (closest to end)
	/*a3real4x4 obj;
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_base, obj);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_hinge, obj);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_end, obj);*/


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
