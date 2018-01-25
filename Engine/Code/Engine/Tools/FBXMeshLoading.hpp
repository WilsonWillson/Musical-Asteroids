#pragma once
#if defined(TOOLS_BUILD) 
// If we have FBX IMPORTING ENABLED, make sure to include the library and header files.
//#define __PLACEMENT_NEW_INLINE 
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma warning( disable : 4239)
#include "Engine/Tools/MeshBuilder.hpp"
#include "Engine/Tools/MatrixStack.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/RHI/RHI.hpp"
#include "Engine/Tools/FBXList.hpp"
#include "Engine/Tools/Skeleton.hpp"
#include "Engine/Tools/Motion.hpp"
#include "Engine/Tools/Pose.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/UVector4.hpp"

//class FbxScene;
//class FbxNode;
//class FbxMatrix;
//class FbxMesh;

class FileBinaryStream;
class Pose;


struct fbx_skin_weight_t
{
	fbx_skin_weight_t() :
		indices(0, 0, 0, 0),
		weights(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	void reset()
	{
		indices = UVector4(0,0,0,0);
		weights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	UVector4 indices;
	Vector4 weights;
};

bool FbxLoadMesh(MeshBuilder *mb, char const *filename, Skeleton *skeleton);
void ConvertSceneToEngineBasis(FbxScene* scene);
void TriangulateScene(FbxScene* scene);
void FlipX(MeshBuilder *mb);
void ImportMeshes(MeshBuilder *out, FbxNode* node, MatrixStack &mat_stack, Skeleton *skeleton);
Matrix4  GetNodeWorldTransform(FbxNode* node);
Matrix4  ToEngineMatrix(FbxMatrix const &fbx_mat);
static void ImportMesh(MeshBuilder *out, MatrixStack &mat_stack, FbxMesh *mesh, Skeleton *skeleton);
Matrix4 GetGeometricTransform(FbxNode *node);
bool FbxGetPosition(Vector3 *out_pos, Matrix4 const &transform, FbxMesh *mesh, int poly_idx, int vert_idx, std::vector<fbx_skin_weight_t> skin_weights, MeshBuilder *out);
void FbxImportVertex(MeshBuilder *out, Matrix4 const &transform, FbxMesh *mesh, int32_t poly_idx, int32_t vert_idx, std::vector<fbx_skin_weight_t> skin_weights);
void CalculateSkinWeights(std::vector<fbx_skin_weight_t>& skin_weights, FbxMesh const *mesh, Skeleton *skeleton);
void AddHighestWeight(fbx_skin_weight_t* skin_weight, unsigned int bone_idx, float weight);
bool HasSkinWeights(FbxMesh const *mesh);
Vector3 ToVector3(FbxVector4& elemId);
bool GetNormal(Vector3 *out, Matrix4 const &transform, FbxMesh *mesh, int poly_idx, int vert_idx, std::vector<fbx_skin_weight_t> skin_weights);

//Skeleton
void ImportSkeleton(Skeleton *out, FbxNode *node, FbxSkeleton *root_bone,  FbxSkeleton *parent_bone,  FbxPose *pose);
bool FbxLoadSkeleton(Skeleton *skel, char const *filename);
FbxPose* GetBindPose(FbxScene *scene);
void FlipX(Skeleton *mb);
static char const* GetNodeName(FbxNode const *node);
static char const* GetBoneName(FbxSkeleton const *skel);
void FbxUpdateSkeleton(float deltaseconds, Motion *motion);

//Motion
static float GetNativeFramefrate(FbxScene *scene);
static bool FbxImportMotion(Motion *motion, Skeleton const *skeleton, FbxScene *scene, FbxAnimStack *anim, float framerate);
bool FbxLoadMotion(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate /*= 10*/);
Matrix4 GetNodeWorldTransformAtTime(FbxNode *node, FbxTime time);


//Saving
unsigned int ConvertFBXFileToMotionFiles(char const *root_output_name,  Motion* motion);
unsigned int ConvertFBXFileToSkeletonFile(char const *root_output_name, Skeleton* skeletonBuild);
unsigned int ConvertFBXFileToMeshFile(char const *root_output_name,  MeshBuilder* meshBuild);

unsigned int ConvertMotionFileToData(char const *root_output_name,  Motion& motion, Skeleton* skeletonBuild);
Pose* ReadPoseFromBinary(FileBinaryStream* fileToSave);
Matrix4 ReadMatrix4FromBinary(FileBinaryStream* fileToSave);
unsigned int ConvertSkeletonFileToData(char const *root_output_name,  Skeleton* skeletonBuild);
unsigned int ConvertMeshFileToData(char const *root_output_name, MeshBuilder* meshBuild);
#else
#endif