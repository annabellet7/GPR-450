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
	
	a3_DemoState-idle-update.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     UPDATING THE STATE in this file.         ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

void a3demo_applyScale_internal(a3_SceneObject *sceneObject, a3real4x4p s)
{
	if (sceneObject->scaleMode)
	{
		if (sceneObject->scaleMode == 1)
		{
			s[0][0] = s[1][1] = s[2][2] = sceneObject->scale.x;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverseUniformScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
		else
		{
			s[0][0] = sceneObject->scale.x;
			s[1][1] = sceneObject->scale.y;
			s[2][2] = sceneObject->scale.z;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverse(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
	}
	else
		a3real4x4TransformInverseIgnoreScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
}


//-----------------------------------------------------------------------------
// UPDATE SUB-ROUTINES

void a3demo_update_objects(a3f64 const dt, a3_SceneObject* sceneObjectBase,
	a3ui32 count, a3boolean useZYX, a3boolean applyScale)
{
	a3ui32 i;
	a3mat4 scaleMat = a3mat4_identity;

	if (applyScale)
		for (i = 0; i < count; ++i, ++sceneObjectBase)
		{
			// update transforms
			a3scene_updateSceneObject(sceneObjectBase, useZYX);

			// apply scale
			a3demo_applyScale_internal(sceneObjectBase, scaleMat.m);
		}
	else
		for (i = 0; i < count; ++i, ++sceneObjectBase)
		{
			a3scene_updateSceneObject(sceneObjectBase, useZYX);
		}
}

void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_SceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis)
{
	const a3f32 dr = demoState->updateAnimation ? (a3f32)dt * 15.0f : 0.0f;
	a3ui32 i;

	// do simple animation
	for (i = 0, axis %= 3; i < count; ++i, ++sceneObjectBase)
		sceneObjectBase->euler.v[axis] = a3trigValid_sind(sceneObjectBase->euler.v[axis] + dr);
}

void a3demo_update_bindSkybox(a3_SceneObject* obj_camera, a3_SceneObject* obj_skybox)
{
	// model transformations (if needed)
	const a3mat4 convertY2Z = {
		+1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, +1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};

	// bind skybox to camera
	obj_skybox->position = obj_camera->position;
	obj_skybox->modelMat.v3 = obj_camera->modelMat.v3;

	// correct rotations as needed
	// need to rotate skybox if Z-up
	a3real4x4ConcatL(obj_skybox->modelMat.m, convertY2Z.m);
}

void a3demo_update_pointLight(a3_SceneObject* obj_camera, a3_ScenePointLight* pointLightBase, a3ui32 count)
{
	a3ui32 i;

	// update lights view positions for current camera
	for (i = 0; i < count; ++i, ++pointLightBase)
	{
		// convert to view space and retrieve view position
		a3real4Real4x4Product(pointLightBase->viewPos.v, obj_camera->modelMatInv.m, pointLightBase->worldPos.v);
	}
}


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update(a3_DemoState *demoState, a3f64 const dt)
{
	demoState->sceneCallbacksPtr->handleUpdate(demoState,
		demoState->sceneCallbacksPtr->scene, dt);
}


//-----------------------------------------------------------------------------

//author annabelle

void a3SingleJointIK(a3_Scene_Animation* scene, a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, 
					a3_HierarchyPoseGroup const* poseGroup, char jointName[], a3_SceneObject const* effector)
{
	a3_SceneObject const* sceneObjectRoot = scene->obj_skeleton;
	a3_Basis const basis_obj = a3basisInit(basis_yp, basis_zp);

		// affected node
		a3ui32 const j_neck = a3hierarchyGetNodeIndex(activeHS->hierarchy, jointName);
		a3_Basis const basis_neck = a3basisInit(basis_zp, basis_yp);

		// invoke IK
		a3kinematicsUpdateLookAtIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
			sceneObjectRoot->sceneGraphIndex, effector->sceneGraphIndex,
			j_neck, basis_obj, basis_neck);
}

void a3PolyJointIK(a3_Scene_Animation* scene, a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
					char endName[], char hingeName[], char rootName[], a3_SceneObject const* effector, a3_SceneObject const* constraint)
{
	a3_SceneObject const* sceneObjectRoot = scene->obj_skeleton;
	a3_Basis const basis_obj = a3basisInit(basis_yp, basis_zp);

	// affected end node
	a3ui32 const j_wrist = a3hierarchyGetNodeIndex(activeHS->hierarchy, endName);
	a3_Basis const basis_wrist = a3basisInit(basis_zp, basis_yp);

	// affected hinge node
	a3ui32 const j_elbow = a3hierarchyGetNodeIndex(activeHS->hierarchy, hingeName);
	a3_Basis const basis_elbow = a3basisInit(basis_xn, basis_yp);

	// affected base node
	a3ui32 const j_shoulder = a3hierarchyGetNodeIndex(activeHS->hierarchy, rootName);
	a3_Basis const basis_shoulder = basis_elbow;

	// invoke IK
	a3kinematicsUpdateLimbIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
		sceneObjectRoot->sceneGraphIndex, effector->sceneGraphIndex, constraint->sceneGraphIndex,
		j_wrist, j_elbow, j_shoulder, basis_obj, basis_wrist, basis_elbow, basis_shoulder);
}

void drawEffector(a3_Scene_Animation const* scene, a3_SceneShaderProgram const* currentDemoProgram, a3_VertexDrawable const* drawable[], a3mat4 viewProjectionMat,
	a3real const* color, a3_SceneObject const* obj)
{
	a3mat4 modelViewProjectionMat, modelMat;
	a3ui32 i = (a3ui32)(obj - scene->object_scene);
	modelMat = scene->sceneGraphState->objectSpace->hpose_base[i].transformMat;
	a3real4x4Product(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, color);
	a3vertexDrawableActivateAndRender(drawable[i]);
}

void loadEffector(a3_Scene_Animation* scene, char jointName[], a3mat4 const skeletonToControl, a3_HierarchyState* hierarchyState, a3_SceneObject* sceneObject,
	a3real offsetX, a3real offsetY, a3real offsetZ)
{
	a3vec4 controlLocator;
	a3ui32 j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, jointName);
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x + offsetX;
	sceneObject->position.y = controlLocator.y + offsetY;
	sceneObject->position.z = controlLocator.z + offsetZ;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;
}