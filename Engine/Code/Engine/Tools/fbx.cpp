#include "Engine/Tools/fbx.hpp"
#if defined(TOOLS_BUILD) 
// If we have FBX IMPORTING ENABLED, make sure to include the library and header files.
//#define __PLACEMENT_NEW_INLINE 
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")

void FbxListFile(char const *filename)
{
	FbxScene *scene = FbxLoadScene(filename, false);
	
	if (scene == nullptr)
		return;

	DebuggerPrintf("FBX: Listing File: %s", filename);

	FbxListScene(scene);

	FbxUnloadScene(scene);
}

FbxScene* FbxLoadScene(char const *filename, bool import_embedded)
{
	// Manager - think of it as a running process of the FbxSdk - you could only ever load one of these
	// and call it good - though I usually just keep one around per import.
	FbxManager *fbx_manager = FbxManager::Create();
	if (nullptr == fbx_manager) {
		//ERROR_MSG("Could not create FBX Manager.");
		return nullptr;
	}

	// Next, set the IO settings - this is how we configure what we're importing
	// By default, we import everything, but if you wanted to explicitly not import some parts
	// you could do that here)
	FbxIOSettings *io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	io_settings->SetBoolProp(IMP_FBX_EXTRACT_EMBEDDED_DATA, import_embedded);

	fbx_manager->SetIOSettings(io_settings);


	// Next, create our importer (since we're loading an FBX, not saving one)
	// Second argument is the name - we don't need to name it.
	FbxImporter *importer = FbxImporter::Create(fbx_manager, "");

	bool result = importer->Initialize(filename,
		-1, // File format, -1 will let the program figure it out
		io_settings);

	if (result) {
		// First, create a scene to be imported into
		// Doesn't need a name, we're not going to do anything with it
		FbxScene *scene = FbxScene::Create(fbx_manager, "");

		// Import into the scene
		result = importer->Import(scene);
		importer->Destroy();
		importer = nullptr;

		if (result) {
			return scene;
		}
	}

	// Failed somewhere, so clean up after ourselves.
	FBX_SAFE_DESTROY(io_settings);
	FBX_SAFE_DESTROY(importer);
	FBX_SAFE_DESTROY(fbx_manager);

	return nullptr;
}

//---------------------PRINT LIST FILE---------------------------------------------------
void FbxListScene(FbxScene *scene)
{
	// Print the node
	FbxNode *root = scene->GetRootNode();
	FbxPrintNode(root, 0);
}

void FbxPrintNode(FbxNode *node, int depth)
{
	// Print the node's attributes.
	DebuggerPrintf("FBX: %i node [%s]", depth, node->GetName());
	//Trace("fbx", "%*sNode [%s]", depth, " ", node->GetName());
	for (int i = 0; i < node->GetNodeAttributeCount(); i++) {
		FbxPrintAttribute(node->GetNodeAttributeByIndex(i), depth);
	}

	// Print the nodes children
	for (int32_t i = 0; i < node->GetChildCount(); ++i) {
		FbxPrintNode(node->GetChild(i), depth + 1);
	}
}

void FbxPrintAttribute(FbxNodeAttribute* pAttribute, int depth)
{
	if (nullptr == pAttribute) {
		return;
	}

	FbxNodeAttribute::EType type = pAttribute->GetAttributeType();

	char const* typeName = GetAttributeTypeName(type);
	char const* attrName = pAttribute->GetName();

	//DebuggerPrintf("RUN-TIME FATAL ERROR on line %i of %s, in %s()\n", lineNum, fileName, functionName);
	DebuggerPrintf("FBX: Depth: %i , Type= %s, name = %s", depth, typeName, attrName);

	//Trace("fbx", "%*s- type='%s' name='%s'", depth, " ", typeName, attrName);
}

char const* GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}


//------------------------DELETE SCENE------------------------------------------------
void FbxUnloadScene(FbxScene *scene)
{
	if (nullptr == scene) {
		return;
	}

	FbxManager *manager = scene->GetFbxManager();
	FbxIOSettings *io_settings = manager->GetIOSettings();

	FBX_SAFE_DESTROY(scene);
	FBX_SAFE_DESTROY(io_settings);
	FBX_SAFE_DESTROY(manager);
}



#else 
// Stubs so that if we don't have the FBX SDK Available, our program does not crash
//void FbxListFile(char const*) {}
//FbxScene* FbxLoadScene(char const *filename, bool import_embedded) {}
//void FbxUnloadScene(FbxScene *scene) {}
//void FbxListScene(FbxScene *scene) {}
//void FbxPrintNode(FbxNode *node, int depth) {}
//void FbxPrintAttribute(FbxNodeAttribute* pAttribute, int depth) {}
//char const* GetAttributeTypeName(FbxNodeAttribute::EType type) {}
#endif
