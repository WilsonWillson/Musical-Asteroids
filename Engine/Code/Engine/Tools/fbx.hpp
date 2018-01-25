#pragma once
#if !defined( __TOOLS_FBXLOAD__ )
#define __TOOLS_FBXLOAD__

#if defined(TOOLS_BUILD) 
// If we have FBX IMPORTING ENABLED, make sure to include the library and header files.
//#define __PLACEMENT_NEW_INLINE 
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#include "Engine/Core/ErrorWarningAssert.hpp"
//#include <fbxsdk.h>


void FbxListFile(char const *filename);
FbxScene* FbxLoadScene(char const *filename, bool import_embedded);
void FbxPrintAttribute(FbxNodeAttribute* pAttribute, int depth);
void FbxUnloadScene(FbxScene *scene);
void FbxListScene(FbxScene *scene);
void FbxPrintNode(FbxNode *node, int depth);
char const* GetAttributeTypeName(FbxNodeAttribute::EType type);

#else
#endif
//class FbxScene;
//class FbxNodeAttribute;
//class FbxNode;
#endif
