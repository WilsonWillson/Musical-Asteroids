#include "Engine/Tools/FBXMeshLoading.hpp"
#include "Engine/Tools/SkeletonInstance.hpp"
#include "Engine/Core/BinaryStream.hpp"
#include "Engine/Core/FileBinaryStream.hpp"

#if defined(TOOLS_BUILD) 
// If we have FBX IMPORTING ENABLED, make sure to include the library and header files.
//#define __PLACEMENT_NEW_INLINE 
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
// Loading a Mesh

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool FbxLoadMesh(MeshBuilder *mb, char const *filename, Skeleton *skeleton)
{
	// First - load the scene - code is identical to when you 
	// listed the file
	FbxScene *scene = FbxLoadScene(filename, false);
	if (nullptr == scene) {
		return false;
	}

	// Second, we want to convert the basis for consitancy
	ConvertSceneToEngineBasis(scene);

	// Third, our engine only deals with triangles, 
	// but authoring programs tend to deal with a lot of 
	// different surface patch types.  For we triangulate
	// (the process of converting every non-triangle polygon to triangles )
	TriangulateScene(scene);


	// Alright, now we can start importing data, for this we'll need a matrix stack 
	// [well, it helps, you can do this without since we'll mostly be deailing with 
	// global transforms outside of what is called the "geometric transform" which is
	// always local]

	// clear all old data before importing
	mb->clear();

	// My matrix stack treats the top as being IDENTITY
	MatrixStack mat_stack;

	// Next, get the root node, and start importing
	FbxNode *root = scene->GetRootNode();

	// import data into our builder
	ImportMeshes(mb, root, mat_stack, skeleton);

	// Clean up after ourself
	FbxUnloadScene(scene);


	// [HACK] After loading, I flip everything across the X axis
	// for conistancy.  Would love this to be part of ConvertSceneToEngineBasis,
	// but no mattter the transform I sent, it was always flipped on X [at best]
	//
	// Doing this causes it to be match Unity/Unreal/Editor in terms of orientation
	FlipX(mb);
	return true;
}

// So, new functions and the Fbx Code that makes them up

// Conver scene to a consistant basis.
// The name is misleading, as this actually converts to a 
// right-handed system (Y-UP, X-RIGHT, Z-BACK)
// but any attempt to convert to a left-handed
// system caused it to flip across Y, which is not what I 
// want - so I handle the final basis flip as a final step after
// loading
void ConvertSceneToEngineBasis(FbxScene *scene)
{
	fbxsdk::FbxAxisSystem local_system(
		(fbxsdk::FbxAxisSystem::EUpVector) fbxsdk::FbxAxisSystem::EUpVector::eYAxis,
		(fbxsdk::FbxAxisSystem::EFrontVector) fbxsdk::FbxAxisSystem::EFrontVector::eParityOdd,  // by default points toward me.
		(fbxsdk::FbxAxisSystem::ECoordSystem) fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded);

	FbxAxisSystem scene_system = scene->GetGlobalSettings().GetAxisSystem();
	if (scene_system != local_system) {
		local_system.ConvertScene(scene);
	}
}

// Converts the scene to only use triangles for the meshes.
void TriangulateScene(FbxScene *scene)
{
	FbxGeometryConverter converter(scene->GetFbxManager());

	// Replace being true means it will destroy the old 
	// representation of the mesh after conversion.  Setting this to 
	// false would leave the old representations in.
	converter.Triangulate(scene, /*replace=*/true);
}

// Flips all geoemetry across the X access
void FlipX(MeshBuilder *mb)
{
	// LEFT AS AN EXCERCISE (mostly as it depends on how your implementation of MeshBuilder/Mesh)

	// You just want to flip the X coordinate of all vertex
	// data (positions,normals/tangents/bitangents)
	for (unsigned int meshStep = 0; meshStep < mb->m_meshVerts.size(); meshStep++)
	{
		mb->m_meshVerts[meshStep].position.x = -mb->m_meshVerts[meshStep].position.x;
		mb->m_meshVerts[meshStep].normal.x = -mb->m_meshVerts[meshStep].normal.x;
		mb->m_meshVerts[meshStep].normal.y = -mb->m_meshVerts[meshStep].normal.y;
		mb->m_meshVerts[meshStep].tangent.x = -mb->m_meshVerts[meshStep].tangent.x;
	}
}


//------------------------------------------------------------------------
//------------------------------------------------------------------------
// The meat of the problem - Importing a mesh
// This walking of a list, loading specific attributes will be a pretty common task
// in this class.
void ImportMeshes(MeshBuilder *out, FbxNode *node, MatrixStack &mat_stack, Skeleton *skeleton)
{
	// first, we're traversing the graph, keep track of our current world transform
	// (I will do this by by pushing directly the world transform of this node to my stack)
	// (that is, this transform becomes the new top exactly)
	Matrix4 transform = GetNodeWorldTransform(node);
	mat_stack.push_direct(transform);

	// First, traverse the scene graph, looking for FbxMesh node attributes.
	int attrib_count = node->GetNodeAttributeCount();
	for (int ai = 0; ai < attrib_count; ++ai) {
		FbxNodeAttribute *attrib = node->GetNodeAttributeByIndex(ai);
		if (attrib->GetAttributeType() == FbxNodeAttribute::eMesh) {
			ImportMesh(out, mat_stack, (FbxMesh*)attrib, skeleton);
		}
	}

	// Try to load any meshes that are children of this node
	int child_count = node->GetChildCount();
	for (int ci = 0; ci < child_count; ++ci) {
		FbxNode *child = node->GetChild(ci);
		ImportMeshes(out, child, mat_stack, skeleton);
	}

	// we're done with this node, so its transform off the stack
	mat_stack.pop();

}

// This gets the world transform of a specific node
// and converts it to OUR engine matrix format
Matrix4 GetNodeWorldTransform(FbxNode *node)
{
	FbxAMatrix fbx_mat = node->EvaluateGlobalTransform();
	return ToEngineMatrix(fbx_mat);
}

Matrix4  ToEngineMatrix(FbxMatrix const &fbx_mat)
{
	Matrix4 mb;

	FbxDouble4 row0 = fbx_mat.mData[0];
	FbxDouble4 row1 = fbx_mat.mData[1];
	FbxDouble4 row2 = fbx_mat.mData[2];
	FbxDouble4 row3 = fbx_mat.mData[3];

	Vector4 my_row0 = Vector4( (float)row0.mData[0], (float)row0.mData[1],(float)row0.mData[2], (float)row0.mData[3]);
	Vector4 my_row1 = Vector4((float)row1.mData[0], (float)row1.mData[1], (float)row1.mData[2], (float)row1.mData[3]);
	Vector4 my_row2 = Vector4((float)row2.mData[0], (float)row2.mData[1], (float)row2.mData[2], (float)row2.mData[3]);
	Vector4 my_row3 = Vector4((float)row3.mData[0], (float)row3.mData[1], (float)row3.mData[2], (float)row3.mData[3]);

	mb.m_iBasis = my_row0;
	mb.m_jBasis = my_row1;
	mb.m_kBasis = my_row2;
	mb.m_translation = my_row3;

	Matrix4 flipX;
	flipX.m_iBasis.x = -1;

	mb = flipX * mb *flipX;
	
	return mb;
}




//------------------------------------------------------------------------
//------------------------------------------------------------------------

// Import a single mesh attribute
static void ImportMesh(MeshBuilder *out, MatrixStack &mat_stack, FbxMesh *mesh, Skeleton *skeleton)
{
	// Should have been triangulated before this - sort of a late check 
	// [treat this as an ASSERT_OR_DIE]
	//ASSERT_OR_DIE(mesh->IsTriangleMesh());
	//ASSERT_RETURN_VALUE(mesh->IsTriangleMesh(), false);

	int32_t poly_count = mesh->GetPolygonCount();
	if (poly_count <= 0) 
		return;

	std::vector<fbx_skin_weight_t> skin_weights;
	if (nullptr != skeleton) 
		CalculateSkinWeights(skin_weights, mesh, skeleton);


	// Geometric Transformations only apply to the current node
	// http://download.autodesk.com/us/fbx/20112/FBX_SDK_HELP/index.html?url=WS1a9193826455f5ff1f92379812724681e696651.htm,topicNumber=d0e7429
	Matrix4 geo_trans = GetGeometricTransform(mesh->GetNode());
	mat_stack.push(geo_trans);

	// import the mesh data.
	Matrix4 transform = mat_stack.get_top();

	// Starting a draw call.
	out->begin(PRIMITIVE_TRIANGLES, false);

	// Load in the mesh - first, figure out how many polygons there are
	//int32_t poly_count = mesh->GetPolygonCount();
	for (int32_t poly_idx = 0; poly_idx < poly_count; ++poly_idx) {

		// For each polygon - get the number of vertices that make it up (should always be 3 for a triangulated mesh)
		int32_t vert_count = mesh->GetPolygonSize(poly_idx);
		//ASSERT(vert_count == 3); // Triangle Meshes should ALWAYS have 3 verts per poly
		vert_count = 3;
								 // Finally, import all the data for this vertex (for now, just position)
		for (int32_t vert_idx = 0; vert_idx < vert_count; ++vert_idx)
		{
			FbxImportVertex(out, transform, mesh, poly_idx, vert_idx, skin_weights);
		}
	}

	out->end();

	// pop the geometric transform
	mat_stack.pop();
}

// This gets a local transform that is applied ONLY to data stored at this node,
// and is always considered local to the node.
Matrix4  GetGeometricTransform(FbxNode *node)
{
	Matrix4 ret;

	if ((node != nullptr) && (node->GetNodeAttribute() != nullptr)) {
		FbxEuler::EOrder order;

		node->GetRotationOrder(FbxNode::eSourcePivot, order);

		FbxVector4 const geo_trans = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 const geo_rot = node->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 const geo_scale = node->GetGeometricScaling(FbxNode::eSourcePivot);

		FbxAMatrix geo_mat;
		FbxAMatrix mat_rot;
		mat_rot.SetR(geo_rot, order);
		geo_mat.SetTRS(geo_trans, mat_rot, geo_scale);

		ret = ToEngineMatrix(geo_mat);
	}

	return ret;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// This loads in all the information for a specific vertex as defined by a 
// poly_idx and vert_idx.  For now, will only worry about position.
// Gets the position from a FbxMesh value.
// Every <poly_idx, vert_idx> pair will evaluate to a unique
// "control point" - that is, a position, or vertex that can be 
// manipulate by the program - which makes importing positions pretty easy
//------------------------------------------------------------------------
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, FbxMesh *mesh, int32_t poly_idx, int32_t vert_idx, std::vector<fbx_skin_weight_t> skin_weights)
{
	/** NEW BITS **/

	// Will demonstarate with normal, but this process is repeated for
	// uvs, colors, tangents, bitangents (called binormals in the SDK)
	Vector3 normal;
	if (GetNormal(&normal, transform, mesh, poly_idx, vert_idx, skin_weights)) {
		out->set_normal(normal);
	}

	//if(GetSkinWeights())

	// same process for UVs, Tangents, Bitangents

	/** END NEW BITS **/

	// If we get a position- push it.
	// This actually should always succeed.
	Vector3 position;
	if (FbxGetPosition(&position, transform, mesh, poly_idx, vert_idx, skin_weights, out)) {
		out->add_vertex(position);
	}
}

bool FbxGetPosition(Vector3 *out_pos,
	Matrix4 const &transform,
	FbxMesh *mesh,
	int poly_idx,
	int vert_idx,
	std::vector<fbx_skin_weight_t> skin_weights,
	MeshBuilder *out)
{
	// First, get the control point index for this poly/vert pair.
	int ctrl_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
	if (ctrl_idx < 0) {
		return false;
	}
	
	out->currentBoneIndicies = skin_weights[ctrl_idx].indices;
	out->currentBoneWeights = skin_weights[ctrl_idx].weights;

	// Get the position for the control point.
	FbxVector4 fbx_pos = mesh->GetControlPointAt(ctrl_idx);

	// convert to an engine vertex, and save it to the out variable
	Vector3 pos;
	pos.x = (float) fbx_pos.mData[0];
	pos.y = (float)fbx_pos.mData[1];
	pos.z = (float)fbx_pos.mData[2];
	//Vector3 pos = ToVec4(fbx_pos).xyz();
	*out_pos = transform.TransformPosition(pos);
	//*out_pos = pos* transform;

	// return success.
	return true;
}

bool GetNormal(Vector3 *out, Matrix4 const &transform, FbxMesh *mesh, int poly_idx, int vert_idx, std::vector<fbx_skin_weight_t> skin_weights)
{
	// First, we need to get the geometry element we care about...
	// you can this of this as a "layer" of data for a mesh.  In this case
	// is is the normal data.

	// Note:  0 Index - meshes can potentially contain multiple layers
	// of the same type (UVs are the most common exmaple of this).  
	// For Normals, I just use the first one and continue on.
	FbxGeometryElementNormal *element = mesh->GetElementNormal(0);
	//if (vnormal == nullptr) {
	// no layer - no data
	//return false;
	//}

	// Next, we need to figure out how to pull the normal for this particular vertex out
	// of this layer. 
	// 
	// So, vnormal is just a packed array of normals, or potentially normals, and
	// and index buffer into those normals.
	//
	// Either way, you can think of vnormal as a packed collection of normals.
	// and we're trying to figure out which element of this array we want (elem_idx)

	// So first, get the element index.
	// (Is the data tied to the control point, or the polygon vertex)
	int elem_idx = 0;
	switch (element->GetMappingMode()) {
	case FbxGeometryElement::eByControlPoint: {
		// Get the normal by control point - most direct way.
		elem_idx = mesh->GetPolygonVertex(poly_idx, vert_idx);
	} break; // case: eByControlPoint

	case FbxGeometryElement::eByPolygonVertex: {
		// array is packed by polygon vertex - so each polygon is unique
		// (think sharp edges in the case of normals)
		elem_idx = mesh->GetPolygonVertexIndex(poly_idx) + vert_idx;
	} break; // case: eByPolygonVertex

	default:
		//ASSERT(0); // Unknown mapping mode
		return false;
	}

	// next, now that we have the element index, we figure out how this is used.
	// Is this a direct array, or an indexed array.
	// If we got an element index, fetch the data based on how it's referenced
	// (Directly, or by an index into an array);
	switch (element->GetReferenceMode()) {
	case FbxGeometryElement::eDirect: {
		// this is just an array - use elem index as an index into this array
		if (elem_idx < element->GetDirectArray().GetCount()) {
			*out = ToVector3(element->GetDirectArray().GetAt(elem_idx));
			*out = transform.TransformPosition(*out);
			return true;
		}
	} break;

	case FbxGeometryElement::eIndexToDirect: {
		// This is an indexed array, so elem_idx is our offset into the 
		// index buffer.  We use that to get our index into the direct array.
		if (elem_idx < element->GetIndexArray().GetCount()) {
			int index = element->GetIndexArray().GetAt(elem_idx);
			*out = ToVector3(element->GetDirectArray().GetAt(index));
			*out = transform.TransformPosition(*out);
			return true;
		}
	} break;

	default:
		//ASSERT(0); // Unknown reference type
		return false;
	}
	return false;
}



//------------------------------------------------------------------------
// So skin weights are by control point - so we'll start off
// by collecting them all up front before we even start building our mesh
void CalculateSkinWeights(std::vector<fbx_skin_weight_t>& skin_weights, FbxMesh const *mesh, Skeleton *skeleton)
{
	// default them all to zero
	unsigned int ctrl_count = (unsigned int)mesh->GetControlPointsCount();
	skin_weights.reserve(ctrl_count);
	skin_weights.resize(ctrl_count);

	for (unsigned int i = 0; i < ctrl_count; ++i) {
		skin_weights[i].reset();
	}

	if (skeleton == nullptr) {
		skin_weights.clear();
		return;
	}

	// for each deformer [probably will only be one] - figure out how it affects
	// my vertices
	int deformer_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int didx = 0; didx < deformer_count; ++didx) {
		FbxSkin *skin = (FbxSkin*)mesh->GetDeformer(didx, FbxDeformer::eSkin);
		if (nullptr == skin) {
			continue;
		}

		// cluster is how a single joint affects the mesh, 
		// so go over the vertices it affects and keep track of how much!
		int cluster_count = skin->GetClusterCount();
		for (int cidx = 0; cidx < cluster_count; ++cidx) {
			FbxCluster *cluster = skin->GetCluster(cidx);
			FbxNode const *link_node = cluster->GetLink();

			// Can't use it without a link node [shouldn't happen!]
			if (nullptr == link_node) {
				continue;
			}

			// Find the joint - if this skeleton doesn't have a joint
			// then we skip this cluster!
			unsigned int joint_idx = skeleton->get_joint_index(link_node->GetName());
			if (joint_idx == -1) {
				continue;
			}

			// cluster stores things in an index buffer again,
			// it will store indices to the control points it affects, 
			// as well as how much weight we apply
			int *indices = cluster->GetControlPointIndices();
			int index_count = cluster->GetControlPointIndicesCount();
			double *weights = cluster->GetControlPointWeights();

			for (int i = 0; i < index_count; ++i) {
				int control_idx = indices[i];
				double weight = weights[i];


				AddHighestWeight(&skin_weights[control_idx], (unsigned int)joint_idx, (float)weight);
			}
		}
	}
}

//------------------------------------------------------------------------
// Keep track of this influencer if it influences more than an already existing
// bone. [all bones influence at 0 at the start]
void AddHighestWeight(fbx_skin_weight_t* skin_weight, unsigned int bone_idx, float weight)
{
	if (skin_weight->weights.x < weight && skin_weight->weights.x <= skin_weight->weights.y && skin_weight->weights.x <= skin_weight->weights.z && skin_weight->weights.x <= skin_weight->weights.w)
	{
		skin_weight->weights.x = weight;
		skin_weight->indices.x = bone_idx;
		return;
	}
	if (skin_weight->weights.y < weight && skin_weight->weights.y <= skin_weight->weights.x && skin_weight->weights.y <= skin_weight->weights.z && skin_weight->weights.y <= skin_weight->weights.w)
	{
		skin_weight->weights.y = weight;
		skin_weight->indices.y = bone_idx;
		return;
	}
	if (skin_weight->weights.z < weight && skin_weight->weights.z <= skin_weight->weights.y && skin_weight->weights.z <= skin_weight->weights.x && skin_weight->weights.z <= skin_weight->weights.w)
	{
		skin_weight->weights.z = weight;
		skin_weight->indices.z = bone_idx;
		return;
	}
	if (skin_weight->weights.w < weight&& skin_weight->weights.w <= skin_weight->weights.x && skin_weight->weights.w <= skin_weight->weights.y && skin_weight->weights.w <= skin_weight->weights.z)
	{
		skin_weight->weights.w = weight;
		skin_weight->indices.w = bone_idx;
		return;
	}



}


//------------------------------------------------------------------------
bool HasSkinWeights(FbxMesh const *mesh)
{
	int deformer_count = mesh->GetDeformerCount(FbxDeformer::eSkin);
	return deformer_count > 0;
}


Vector3 ToVector3(FbxVector4& elemId)
{
	Vector3 pos;
	pos.x = (float)elemId.mData[0];
	pos.y = (float)elemId.mData[1];
	pos.z = (float)elemId.mData[2];

	return pos;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool FbxLoadSkeleton(Skeleton *skel, char const *filename)
{
	// Same as you did for Meshes.
	FbxScene *fbx_scene = FbxLoadScene(filename, false);
	if (nullptr == fbx_scene) {
		return false;
	}

	ConvertSceneToEngineBasis(fbx_scene);

	// First, get the bind post for the scene
	skel->clear();


	// Now, traverse the scene, and build the skeleton out with 
	// the bind pose positions
	FbxNode *root = fbx_scene->GetRootNode();

	// Bind pose - the pose which assets are authored for.
	FbxPose *pose = GetBindPose(fbx_scene);

	// Import the skeleton by traversing the scene.
	ImportSkeleton(skel, root, nullptr, nullptr, pose);

	// Flip the Axis 
	//FlipX(skel);

	FbxUnloadScene(fbx_scene);

	// Success if I've loaded in at least ONE bone/joint.
	return (skel->get_joint_count() > 0U);
}

// Get the bind pose - that is, the pose that the resources are authored
// for. If none exists - we'll assume ONLY the bind pose exists, and use
// the global transformations.
FbxPose* GetBindPose(FbxScene *scene)
{
	int pose_count = scene->GetPoseCount();
	for (int i = 0; i < pose_count; ++i) {
		FbxPose *pose = scene->GetPose(i);
		if (pose->IsBindPose()) {
			// DEBUG - Print all nodes part of this pose
			/*
			int count = pose->GetCount();
			for (int j = 0; j < count; ++j) {
			FbxNode *node = pose->GetNode(j);
			Trace( "fbx", "Pose Node: %s", node->GetName() );
			}
			*/

			return pose;
		}
	}

	return nullptr;
}

// Imports the skeleton.

void ImportSkeleton(Skeleton *out,  // skeleton I'm loading into
	FbxNode *node,               // Current node I'm looking at.
	FbxSkeleton *root_bone,      // The head bone in this skeleton (keeping track purely for future support for nested skeletons in an FBX)
	FbxSkeleton *parent_bone,    // The bone to consider the parent of any subsequently loaded bone.
	FbxPose *pose)              // Pose we're loading.
{
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {

		// If this node is a skeleton node (a joint), 
		// then it should have a skeleton attribute. 
		FbxNodeAttribute *na = node->GetNodeAttributeByIndex(i);
		if (na->GetAttributeType() == FbxNodeAttribute::eSkeleton) {

			// Cast it, and figure out the type of bone. 
			FbxSkeleton *skel = (FbxSkeleton*)na;
			FbxSkeleton::EType type = skel->GetSkeletonType();

			// From FBXSDK Documentation.
			// eRoot,			/*!< First element of a chain. */
			// eLimb,			/*!< Chain element. */
			// eLimbNode,		/*!< Chain element. */
			// eEffector		/*!< Last element of a chain. */

			// If this is a root - we better have not have another root higher 
			// up (for now).  This is just here to catch this happening so
			// I know if I should support it in the future - you could cut this.
			if ((type == FbxSkeleton::eRoot) || (type == FbxSkeleton::eEffector)) {
				// this is a root bone - so can treat it as the root
				// of a skeleton.
				// TODO: no support for nested skeletons yet.
				//ASSERT(root_bone == nullptr);
				root_bone = nullptr;
				root_bone = skel;
			} // else, it is a limb or limb node, which we treat the same

			  // See if this bone exists int he pose, and if so, 
			  // get its index in the pose.
			int pose_node_idx = -1;
			if (pose != nullptr) {
				pose_node_idx = pose->Find(node);
			}

			// Get the global transform for this bone
			Matrix4 bone_transform;
			if (pose_node_idx != -1) {
				// If it exists in the pose - use the poses transform.
				// Since we're only doing the bind pose now, the pose should ONLY
				// be storing global transforms (as per the documentation).
				//
				// When we update this code to load in animated poses, this willc hange.
				//bool is_local = pose->IsLocalMatrix(pose_node_idx);
			//	ASSERT(false == is_local); // for now, should always be global (pose transforms always are)
				FbxMatrix fbx_matrix = pose->GetMatrix(pose_node_idx);
				bone_transform = ToEngineMatrix(fbx_matrix);
			}
			else {
				// Not in the pose - so we have two options
				if (parent_bone == nullptr) {
					// We have no parent [first in the skeleton], so just use 
					// the global transform of the node.
					bone_transform = ToEngineMatrix(node->EvaluateGlobalTransform());
				}
				else {
					// Otherwise, calculate my world based on my parent transform
					// and my nodes local.
					//
					// The reason behind this is EvaluateGlobalTransform actually
					// gets the animated transform, while getting the Pose transform
					// will get an unanimated transform.  So if we're a child of a transform
					// that was gotten from a pose, I should be evaluating off that.
					//
					// TODO: Evaluate Local from Parent - just in case their is siblings between this
					Matrix4 local = ToEngineMatrix(node->EvaluateLocalTransform());
					Matrix4 ptrans = out->get_joint_transform(GetBoneName(parent_bone));
					bone_transform = local * ptrans;
				}
			}

			// Add a joint.
			out->add_joint(GetBoneName(skel), (parent_bone != nullptr) ? GetBoneName(parent_bone) : nullptr, bone_transform);

			// set this as the next nodes parent, and continue down the chain
			parent_bone = skel;
			break;
		}
	}

	// import the rest
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		ImportSkeleton(out, node->GetChild(i), root_bone, parent_bone, pose);
	}
}



//------------------------------------------------------------------------
// Just a helper - checks for null - but otherwise gets the node name
static char const* GetNodeName(FbxNode const *node)
{
	if (nullptr == node) {
		return "";
	}
	else {
		return node->GetName();
	}
}


//------------------------------------------------------------------------
// Get the name to use for this bone (ideally, use the node, but if the bone
// doesn't exist, use the skeleton attributes name)
static char const* GetBoneName(FbxSkeleton const *skel)
{
	if (nullptr == skel) {
		return nullptr;
	}

	char const *node_name = GetNodeName(skel->GetNode());
	if (!StringIsNullOrEmpty(node_name))
		return skel->GetNode()->GetName();
	else 
		return skel->GetName();
}

void FbxUpdateSkeleton(float deltaseconds, Motion *motion)
{
	motion->m_skeletonInstance->apply_motion(motion, deltaseconds);
}

//----------------------------------------------------------------------------------

//------------------------------------------------------------------------
// Just as an extra - if you want to use the FBX files frame rate, this
// is how you can fetch it.
/*
static float GetNativeFramefrate(FbxScene  *scene)
{
	// Get the scenes authored framerate
	FbxGlobalSettings &settings = scene->GetGlobalSettings();
	FbxTime::EMode time_mode = settings.GetTimeMode();
	double framerate;
	if (time_mode == FbxTime::eCustom) 
		framerate = settings.GetCustomFrameRate();
	else 
		framerate = FbxTime::GetFrameRate(time_mode);
}
*/
//------------------------------------------------------------------------
static bool FbxImportMotion(Motion *motion,	Skeleton const *skeleton,	FbxScene *scene, FbxAnimStack *anim, float framerate)
{
	// First, set the scene to use this animation - helps all the evaluation functions
	scene->SetCurrentAnimationStack(anim);

	// how far do we move through this
	FbxTime advance;
	advance.SetSecondDouble((double)(1.0f / framerate));

	// Get the start, end, and duration of this animation.
	FbxTime local_start = anim->LocalStart;
	FbxTime local_end = anim->LocalStop;
	float local_start_fl = (float)local_start.GetSecondDouble();
	float local_end_fl = (float)local_end.GetSecondDouble();

	// Okay, so local start will sometimes start BEFORE the first frame - probably to give it something to T
	// pose?  This was adding a jerky frame movement to UnityChan's walk cycle. 
	// Whatever, we'll always start at least 0.
	local_start_fl = max(0.0f, local_start_fl);
	float duration_fl = local_end_fl - local_start_fl;

	// no duration, no animation!
	if (duration_fl <= 0.0f) {
		return false;
	}

	// Alright, we have a motion, we start getting data from it
	char const *motion_name = anim->GetName();
	//float time_span = duration_fl;

	// set some identifying information
	motion->m_framerate = framerate;
	motion->set_name(motion_name);
	motion->set_duration(duration_fl);

	for (unsigned int durationStep = 0; durationStep < motion->m_poses.size(); durationStep++)
	{
		Pose* newPose = new Pose(skeleton);
		motion->m_poses[durationStep] = newPose;
	}

	// number of frames should encompasses the duration, so say we are at 10hz,
	// but have an animation that is 0.21f seconds.  We need at least...
	//    3 intervals (Ceiling(10 * 0.21) = Ceiling(2.1) = 3)
	//    4 frames (0.0, 0.1, 0.2, 0.3)
	unsigned int frame_count = (unsigned int)ceil(duration_fl * framerate) + 1;


	// Now, for each joint in our skeleton
	// not the most efficient way to go about this, but whatever - tool step
	// and it is not slow enough to matter.
	unsigned int joint_count = skeleton->get_joint_count();
	for (unsigned int joint_idx = 0; joint_idx < joint_count; ++joint_idx) {
		char const *name = skeleton->get_joint_name(joint_idx);

		// get the node associated with this bone
		// I rely on the names to find this - so author of animations
		// should be sure to be using the same rig/rig names.
		// (this is only important for the import process, after that
		// everything is assuming matching indices)
		FbxNode *node = scene->FindNodeByName(name);
		FbxNode *parent = nullptr;
		unsigned int parent_idx = skeleton->getParentJoint(joint_idx);
		//ASSERT(node != nullptr);

		// get the parent node
		if (parent_idx != -1) {
			char const *parent_name = skeleton->get_joint_name(parent_idx);
			parent = scene->FindNodeByName(parent_name);
			//ASSERT(parent != nullptr);
		}

		// Now, for this entire animation, evaluate the local transform for this bone at every interval
		// number of frames is 
		FbxTime eval_time = FbxTime(0);
		for (unsigned int frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
			// Okay, get the pose we want
			Pose *pose = motion->get_pose(frame_idx);
			Matrix4 joint_world = GetNodeWorldTransformAtTime(node, eval_time);
			Matrix4 joint_local = joint_world;
			if (nullptr != parent) {
				Matrix4 parent_world = GetNodeWorldTransformAtTime(parent, eval_time);
				Matrix4 parent_world_Inverse = parent_world.getInverse(parent_world);
				joint_local = joint_world * parent_world_Inverse;
			}

			pose->local_transforms[joint_idx] = joint_local;
			eval_time += advance;
		}
	}

	return true;
}

//------------------------------------------------------------------------
bool FbxLoadMotion(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate /*= 10*/)
{

	// Normal things
	FbxScene *scene = FbxLoadScene(filename, false);
	if (nullptr == scene) {
		return false;
	}
	ConvertSceneToEngineBasis(scene);

	// Framerate - how often are we sampling this
	float fr = (float)framerate;


	// Next get the animation count.
	int anim_count = scene->GetSrcObjectCount<FbxAnimStack>();
	if (anim_count > 0U) {
		// great, we have a motion
		// NOTE:  Multiple motions may be embedded in this file, you could update to extract all 
		FbxAnimStack *anim = scene->GetSrcObject<FbxAnimStack>(0);
		FbxImportMotion(motion, skeleton, scene, anim, fr);
	}

	FbxUnloadScene(scene);

	//FlipX();

	return (motion->get_duration() > 0.0f);
}

//------------------------------------------------------------------------
// Missed Functions
//------------------------------------------------------------------------
Matrix4 GetNodeWorldTransformAtTime(FbxNode *node, FbxTime time)
{
	Matrix4 mb;
	if (nullptr == node) {
		return mb;
	}

	FbxMatrix fbx_mat = node->EvaluateGlobalTransform(time);
	return ToEngineMatrix(fbx_mat);
}

unsigned int ConvertFBXFileToMotionFiles(char const *root_output_name, Motion* motion)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();
	fileToSave->open_for_write(root_output_name);


	size_t len = motion->m_name.size();
	fileToSave->write_bytes(&len, sizeof(size_t));
	fileToSave->write_bytes(motion->m_name.c_str(), motion->m_name.size());


	//float duration = motion->get_duration();
	//fileToSave->write_bytes(&duration, sizeof(duration));

	fileToSave->write_bytes(&motion->m_framerate, sizeof(motion->m_framerate));


   for (unsigned int poseStep = 0; poseStep < motion->m_poses.size(); poseStep++)
   {
	   size_t sizeOfVector = motion->m_poses[poseStep]->local_transforms.size();
	   fileToSave->write_bytes(&sizeOfVector, sizeof(sizeOfVector));
	   for (unsigned int localTransformStep = 0; localTransformStep < motion->m_poses[poseStep]->local_transforms.size(); localTransformStep++)
	   {
		   fileToSave->write_bytes(&motion->m_poses[poseStep]->local_transforms[localTransformStep], sizeof(motion->m_poses[poseStep]->local_transforms[localTransformStep]));
	   }
   }
   fileToSave->close();

	return 0;
}

unsigned int ConvertFBXFileToSkeletonFile(char const *root_output_name, Skeleton* skeletonBuild)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();

	fileToSave->open_for_write(root_output_name);

	for (unsigned int skeletonStep = 0; skeletonStep < skeletonBuild->m_skeleton.size(); skeletonStep++)
	{
		fileToSave->write_bytes(&skeletonBuild->m_skeleton[skeletonStep]->m_transform, sizeof(skeletonBuild->m_skeleton[skeletonStep]->m_transform));

		size_t len = skeletonBuild->m_skeleton[skeletonStep]->boneName.size();
		fileToSave->write_bytes(&len, sizeof(size_t));
		fileToSave->write_bytes(skeletonBuild->m_skeleton[skeletonStep]->boneName.c_str(), skeletonBuild->m_skeleton[skeletonStep]->boneName.size()); 

		len = skeletonBuild->m_skeleton[skeletonStep]->parentBoneName.size();
		fileToSave->write_bytes(&len, sizeof(size_t));
		fileToSave->write_bytes(skeletonBuild->m_skeleton[skeletonStep]->parentBoneName.c_str(), skeletonBuild->m_skeleton[skeletonStep]->parentBoneName.size()); 
	}
	fileToSave->close();

	return 0;
}

unsigned int ConvertFBXFileToMeshFile(char const *root_output_name, MeshBuilder* meshBuild)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();

	fileToSave->open_for_write(root_output_name);
	fileToSave->write_bytes(&meshBuild->m_primative, sizeof(meshBuild->m_primative));
	for (unsigned int meshStep = 0; meshStep < meshBuild->m_meshVerts.size(); meshStep++)
	{
		fileToSave->write_bytes(&meshBuild->m_meshVerts[meshStep], sizeof(meshBuild->m_meshVerts[meshStep]));
	}
	fileToSave->close();
	return 0;
}




unsigned int ConvertMotionFileToData(char const *root_output_name, Motion& motion, Skeleton* skeletonBuild)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();
	fileToSave->open_for_read(root_output_name);


	size_t len;
	fileToSave->read_bytes(&len, sizeof(size_t));
	char* temp = new char[len + 1];
	fileToSave->read_bytes(temp, len);
	temp[len] = '\0';
	motion.m_name = temp;
	delete[] temp;


	//fileToSave->read_bytes(&duration, sizeof(duration));
	//motion->set_duration(duration);

	fileToSave->read_bytes(&motion.m_framerate, sizeof(motion.m_framerate));

	while (feof(fileToSave->file_pointer) == 0)
	{

		motion.m_poses.push_back(ReadPoseFromBinary(fileToSave));
	}
	motion.m_poses.pop_back();
	fileToSave->close();

	motion.m_skeletonInstance->skeleton = skeletonBuild;
	motion.m_skeletonInstance->m_currentPose = new Pose();

	return 0;
}

Pose* ReadPoseFromBinary(FileBinaryStream* fileToSave)
{
	Pose* pose = new Pose();
	size_t length;
	Matrix4  mb;
	fileToSave->read_bytes(&length, sizeof(size_t));
	if (length > 100000) // #HACK
		return nullptr;
	for (unsigned int localTransformStep = 0; localTransformStep < length; localTransformStep++)
	{
		pose->local_transforms.push_back(ReadMatrix4FromBinary(fileToSave));
		//pose->local_transforms.push_back(mb);
	}
	return pose;
}

Matrix4 ReadMatrix4FromBinary(FileBinaryStream* fileToSave)
{
	Matrix4 localTransform;
	fileToSave->read_bytes(&localTransform, sizeof(Matrix4));
	return localTransform;
}

unsigned int ConvertSkeletonFileToData(char const *root_output_name, Skeleton* skeletonBuild)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();
	int skeletonStep = 0;
	Matrix4 mb;
	fileToSave->open_for_read(root_output_name);

	while(feof(fileToSave->file_pointer) == 0)
	{
		BoneStructure* newBone = new BoneStructure();
		fileToSave->read_bytes(&mb, sizeof(mb));
		newBone->m_transform = mb;

		size_t len;
		fileToSave->read_bytes(&len, sizeof(size_t));
		char* temp = new char[len + 1];
		fileToSave->read_bytes(temp, len);
		temp[len] = '\0';
		newBone->boneName = temp;
		delete[] temp;


		fileToSave->read_bytes(&len, sizeof(size_t));
		char* temp2 = new char[len + 1];
		fileToSave->read_bytes(temp2, len);
		temp2[len] = '\0';
		newBone->parentBoneName = temp2;
		delete[] temp2;



		skeletonBuild->m_skeleton.push_back(newBone);
		skeletonStep++;
	}
	skeletonBuild->m_skeleton.pop_back();
	fileToSave->close();

	return 0;
}

unsigned int ConvertMeshFileToData(char const *root_output_name, MeshBuilder* meshBuild)
{
	FileBinaryStream* fileToSave = new FileBinaryStream();

	fileToSave->open_for_read(root_output_name);
	fileToSave->read_bytes(&meshBuild->m_primative, sizeof(meshBuild->m_primative));
	while (feof(fileToSave->file_pointer) == 0)
	{
		vertex_t newVertex;
		fileToSave->read_bytes(&newVertex, sizeof(vertex_t));
		meshBuild->m_meshVerts.push_back(newVertex);
	}
	meshBuild->m_meshVerts.pop_back();
	fileToSave->close();
	return 0;
}

#else 
// Stubs so that if we don't have the FBX SDK Available, our program does not crash
//bool FbxLoadMesh(MeshBuilder *mb, char const *filename) { return false; }
//void ConvertSceneToEngineBasis(FbxScene* scene) {}
//void TriangulateScene(FbxScene* scene) {}
//void FlipX(MeshBuilder *mb) {}
//void ImportMeshes(MeshBuilder *out, FbxNode* node, MatrixStack &mat_stack) {}
//Matrix4  GetNodeWorldTransform(FbxNode* node) { Matrix4 cb; return cb; }
//Matrix4  ToEngineMatrix(FbxMatrix const &fbx_mat) { Matrix4 cb; return cb; }
//static void ImportMesh(MeshBuilder *out, MatrixStack &mat_stack, FbxMesh *mesh) {}
//Matrix4 GetGeometricTransform(FbxNode *node) { Matrix4 cb; return cb; }
//bool FbxGetPosition(Vector3 *out_pos, Matrix4 const &transform, FbxMesh *mesh, int poly_idx, int vert_idx) { return false; }
//void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, FbxMesh *mesh, int32_t poly_idx, int32_t vert_idx) {}
//bool GetNormal(Vector3 *out, Matrix4 const &transform, FbxMesh *mesh, int poly_idx, int vert_idx) { return false; }
#endif
