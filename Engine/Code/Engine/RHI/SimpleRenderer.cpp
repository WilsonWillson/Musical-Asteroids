#include "Engine/RHI/SimpleRenderer.hpp"
#include "Engine/Tools/FBXMeshLoading.hpp"
#include "Engine/Threading/Job.hpp"
#include "Engine/Core/Profiler.hpp"
#define STRUCTBUFFER = 3;

//------------------------------Constructor/Deconstructors----------------------------
SimpleRenderer::SimpleRenderer()
	:m_device(nullptr),
	m_context(nullptr),
	m_output(nullptr),
	matrix_cb(nullptr),
	m_currentTarget(nullptr),
	current_blend_state(nullptr),
	m_StructreBuffer(nullptr)
{
	g_theRHI = new RHI();
	g_RenderTextItems = new ThreadSafeQueue<load_text_job_t>;
}

SimpleRenderer::~SimpleRenderer()
{
	SAFE_DELETE(m_defaultRasterState);

	//Shared pointer objects with Simple Renderer. So deleting this clears the memory of both
	delete m_defaultRasterState;
	delete matrix_cb;
	delete	light_cb;
	delete clear_cb;
	delete	m_LightShader;
	delete current_blend_state;
	delete	m_colorShader;
	delete default_depth_stencil;
	delete depth_stencil_state;
	delete flat_normal_texture;
	delete white_texture;
	delete g_theRHI;
}

void SimpleRenderer::Setup(HINSTANCE applicationInstanceHandle, WNDCLASSEX windowClassDescription)
{
	RHIInstance *ri = RHIInstance::GetInstance();

	m_device = new RHIDevice(ri, nullptr);
	m_output = new RHIOutput(m_device, nullptr, nullptr);
	m_context = new RHIDeviceContext(m_device, nullptr);
	
	g_theRHI->RHIStartup(applicationInstanceHandle, windowClassDescription, m_device,m_context, m_output );

	default_depth_stencil = new DX11Texture2D(m_device, 1280, 720, IMAGEFORMAT_D24S8);
	current_depth_stencil = nullptr;

   m_defaultRasterState = new RasterState(m_device);
   m_context->SetRasterState(m_defaultRasterState);

   matrix_data.model.MakeIdentity();
   matrix_data.view.MakeIdentity();
   matrix_data.projection.MakeIdentity();
   matrix_data.eye_position = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
   matrix_cb = new ConstantBuffer(m_device, &matrix_data, sizeof(matrix_data));
   SetConstantBuffer(0, matrix_cb);

   light_cb = new ConstantBuffer(m_device, &light_data, sizeof(light_data));
   SetConstantBuffer(2, light_cb);

   clear_constants_data.CLEAR_COLOR = Vector4(255, 0, 0, 255);
   clear_constants_data.X_Pan = 2.5f;
   clear_constants_data.Y_Pan = 1.0f;
   clear_constants_data.zoom_Distance = 1;
   clear_constants_data.ITERATIONCOUNT = 1000;
   clear_constants_data.PowerCount.x = 2;
   clear_cb = new ConstantBuffer(m_device, &clear_constants_data, sizeof(clear_constants_data));
   setComputeConstantBuffer();


   // white texture
   Image image;
   image.create_clear(1, 1, rgba_fl(1.0f, 1.0f, 1.0f, 1.0f));
    white_texture = new DX11Texture2D(m_device, image);

   // create a default normal texture   
   Image image2;
  image2.create_clear(1, 1, rgba_fl(.5f, .5f, 1.0f, 1.0f));
  flat_normal_texture = new DX11Texture2D(m_device, image2);

   m_colorShader = m_device->CreateShaderFromHlslFile("data/color.hlsl");
   m_LightShader = m_device->CreateShaderFromHlslFile("data/light.hlsl");
   SetRenderTarget(nullptr, nullptr);

   depth_stencil_desc.depth_test_enabled = false;
  depth_stencil_desc.depth_writing_enabled = true;
   depth_stencil_state = new DepthStencilState(m_device, depth_stencil_desc);
   set_depth_stencil_state(depth_stencil_state);
}

//---------------------Creation Methods-------------------------------------------------

VertexBuffer* SimpleRenderer::CreateQuad(float x, float y, float z, float vec2XMin, float vec2YMin, float vec2XMax, float vec2YMax, bool isConsole)
{

	vertex_t vertices[6] =
	{
		vertex_t(Vector3(-x, -y, z), Vector2(vec2XMin, vec2YMax), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(x, y, z),Vector2(vec2XMax,vec2YMin), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(-x, y, z), Vector2(vec2XMin,vec2YMin), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(-x, -y, z),Vector2(vec2XMin, vec2YMax), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(x, -y, z),Vector2(vec2XMax, vec2YMax), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(x, y, z),Vector2(vec2XMax, vec2YMin), rgba_fl(1,1,1,1)),
	};

	VertexBuffer* quadVBO = m_device->CreateVertexBuffer(vertices, 6, isConsole);

	return quadVBO;
}


//----------------------------------Drawing to Screen-------------------------------------------
void SimpleRenderer::Present()
{
	g_theRHI->RunFrame();
}

void SimpleRenderer::Draw(ePrimitiveType topology, VertexBuffer *vertexBufferObject, unsigned int const vertexCount)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->Draw(m_context, topology, vertexBufferObject, vertexCount);
}

void SimpleRenderer::DrawVertexBufferArray(ePrimitiveType topology, std::vector<VertexBuffer *> m_vertexBufferArray, int bufferIndex)
{
	g_theRHI->SetDeviceContext(m_context);

	g_theRHI->Draw(m_context, topology, m_vertexBufferArray[bufferIndex], m_vertexBufferArray[bufferIndex]->m_vertexCount);
}

void SimpleRenderer::DrawVertexBufferArray(ePrimitiveType topology, std::vector<VertexBuffer *> m_vertexBufferArray)
{
	g_theRHI->SetDeviceContext(m_context);
	for (unsigned int bufferIndex = 0; bufferIndex < m_vertexBufferArray.size(); bufferIndex++)
	{
		g_theRHI->Draw(m_context, topology, m_vertexBufferArray[bufferIndex], m_vertexBufferArray[bufferIndex]->m_vertexCount);
	}
}

void SimpleRenderer::ClearScreen()
{
	g_theRHI->ClearScreen();
}

void SimpleRenderer::ClearDepth(float depth, unsigned char stencil)
{
	g_theRHI->clear_depth(current_depth_stencil, depth, stencil);
}

//-------------------------------Draw Functions------------------------------------------------------------

void SimpleRenderer::drawPlane(Vector3 pos, Rgba color)
{
	draw_quad3d(pos, Vector3(1.0f, 0.0f, 0.0f), -1.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);
}

void SimpleRenderer::drawTwoSidedPlane(Vector3 pos, Rgba color)
{
	draw_quad3d(pos, Vector3(1.0f, 0.0f, 0.0f), -1.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);
	draw_quad3d(pos, Vector3(1.0f, 0.0f, 0.0f), 1.0f, -1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);
}

void SimpleRenderer::DrawCube(Vector3 frontFacingPos, Rgba color)
{
	

	draw_quad3d(frontFacingPos, Vector3(1.0f, 0.0f, 0.0f), -1.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color); //Front

	//Back
	Vector3 tempPos = frontFacingPos;
	tempPos.z += 2.0f;
	draw_quad3d(tempPos, Vector3(1.0f, 0.0f, 0.0f), 1.0f, -1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);

	//Right																													
	tempPos = frontFacingPos;
	tempPos.x += 1.0f;
	tempPos.z += 1.0f;
	draw_quad3d(tempPos, Vector3(0.0f, 0.0f, 1.0f), -1.0f, 1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);

	//Left																															
	tempPos = frontFacingPos;
	tempPos.x -= 1.0f;
	tempPos.z += 1.0f;
	draw_quad3d(tempPos, Vector3(0.0f, 0.0f, 1.0f), 1.0f, -1.0f, Vector3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f, color);

	//Top
	tempPos = frontFacingPos;
	tempPos.y += 1.0f;
	tempPos.z += 1.0f;
	draw_quad3d(tempPos, Vector3(1.0f, 0.0f, 0.0f), -1.0f, 1.0f, Vector3(0.0f, 0.0f, 1.0f), -1.0f, 1.0f, color);

	//Bottom
	tempPos = frontFacingPos;
	tempPos.y -= 1.0f;
	tempPos.z += 1.0f;
	draw_quad3d(tempPos, Vector3(1.0f, 0.0f, 0.0f), -1.0f, 1.0f, Vector3(0.0f, 0.0f, -1.0f), -1.0f, 1.0f, color);

}

void SimpleRenderer::DrawBasis()
{
	draw_line(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), rgba_fl(1, 0, 0));
	draw_line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), rgba_fl(0, 1, 0));
	draw_line(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), rgba_fl(0, 0, 1));
}

//--------------------------------------Building Block Drawing Functions------------------------------------
void SimpleRenderer::draw_quad2d(Vector2 bottom_left, Vector2 top_right, Vector2 uvBottomLeft, Vector2 uvBottomRight, Rgba color)
{
	vertex_t vertices[] = {
		vertex_t(Vector3(bottom_left.x, bottom_left.y, 0.0f), Vector2(uvBottomLeft.x, uvBottomRight.y), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(top_right.x, top_right.y, 0.0f), Vector2(uvBottomRight.x, uvBottomLeft.y), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(bottom_left.x, top_right.y, 0.0f), Vector2(uvBottomLeft.x, uvBottomLeft.y), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(bottom_left.x, bottom_left.y, 0.0f), Vector2(uvBottomLeft.x, uvBottomRight.y), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(top_right.x, bottom_left.y, 0.0f), Vector2(uvBottomRight.x, uvBottomRight.y), rgba_fl(1,1,1,1)),
		vertex_t(Vector3(top_right.x, top_right.y, 0.0f), Vector2(uvBottomRight.x, uvBottomLeft.y), rgba_fl(1,1,1,1)),
	};

	VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 6, false);
	std::vector<VertexBuffer*> vertexBufferArray;
	vertexBufferArray.push_back(tempVBO);

	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}

	for (unsigned int i = 0; i < vertexBufferArray.size(); i++)
	{
		delete vertexBufferArray[i];
	}

	vertexBufferArray.clear();
}

void SimpleRenderer::draw_quad2d(AABB2 bounds, AABB2 uvbounds, rgba_fl color)
{
	vertex_t vertices[] = {
		vertex_t(Vector3(bounds.m_mins.x, bounds.m_mins.y, 0.0f), Vector2(uvbounds.m_mins.x, uvbounds.m_maxs.y), color),
		vertex_t(Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f), Vector2(uvbounds.m_maxs.x, uvbounds.m_mins.y), color),
		vertex_t(Vector3(bounds.m_mins.x, bounds.m_maxs.y, 0.0f), Vector2(uvbounds.m_mins.x, uvbounds.m_mins.y), color),
		vertex_t(Vector3(bounds.m_mins.x, bounds.m_mins.y, 0.0f), Vector2(uvbounds.m_mins.x, uvbounds.m_maxs.y), color),
		vertex_t(Vector3(bounds.m_maxs.x, bounds.m_mins.y, 0.0f), Vector2(uvbounds.m_maxs.x, uvbounds.m_maxs.y), color),
		vertex_t(Vector3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f), Vector2(uvbounds.m_maxs.x, uvbounds.m_mins.y), color),
	};

	VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 6, false);
	std::vector<VertexBuffer*> vertexBufferArray;
	vertexBufferArray.push_back(tempVBO);

	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}

	for (unsigned int i = 0; i < vertexBufferArray.size(); i++)
	{
		delete vertexBufferArray[i];
	}

	vertexBufferArray.clear();
}

void SimpleRenderer::draw_quad3d(Vector3 const &origin, Vector3 const &right, float neg_x_extents, float pos_x_extents, Vector3 const &up, float neg_y_extents, float pos_y_extents, Rgba color)
{
	Vector3 bl = origin + neg_x_extents * right + neg_y_extents * up;
	Vector3 tl = origin + neg_x_extents * right + pos_y_extents * up;
	Vector3 br = origin + pos_x_extents * right + neg_y_extents * up;
	Vector3 tr = origin + pos_x_extents * right + pos_y_extents * up;

	float r = RangeMapFloat((float)color.r, 0.f, 255.f, 0.f, 1.f);
	float g = RangeMapFloat((float)color.g, 0.f, 255.f, 0.f, 1.f);
	float b = RangeMapFloat((float)color.b, 0.f, 255.f, 0.f, 1.f);
	float a = RangeMapFloat((float)color.a, 0.f, 255.f, 0.f, 1.f);
															  
	vertex_t vertices[] = {
		vertex_t(bl, Vector2(0.0f, 1.0f),rgba_fl(r,g,b,a)),
		vertex_t(tr, Vector2(1.0f, 0.0f),rgba_fl(r,g,b,a)),
		vertex_t(tl, Vector2(0.0f, 0.0f), rgba_fl(r,g,b,a)),
		vertex_t(bl, Vector2(0.0f, 1.0f), rgba_fl(r,g,b,a)),
		vertex_t(br, Vector2(1.0f, 1.0f), rgba_fl(r,g,b,a)),
		vertex_t(tr, Vector2(1.0f, 0.0f), rgba_fl(r,g,b,a)),
	};

	VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 6, false);
	std::vector<VertexBuffer*> vertexBufferArray;
	vertexBufferArray.push_back(tempVBO);

	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}

void SimpleRenderer::draw_line(Vector3 startPoint, Vector3 endPoint, rgba_fl color)
{
	vertex_t vertices[] = {
		vertex_t(startPoint, Vector2(0.0f, 0.0f), color),
		vertex_t(endPoint, Vector2(1.0f, 1.0f), color),
	};

	VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 2, false);
	std::vector<VertexBuffer*> vertexBufferArray;
	vertexBufferArray.push_back(tempVBO);
	DrawVertexBufferArray(PRIMITIVE_LINES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}

void SimpleRenderer::draw_point(Vector3 const &p, rgba_fl const &color /*= rgba_fl::WHITE*/)
{
	Vector3 r = Vector3(0.2f, 0.0f, 0.0f);
	Vector3 u = Vector3(0.0f, 0.2f, 0.0f);
	Vector3 f = Vector3(0.0f, 0.0f, 0.2f);

	vertex_t vertices[] = {
		vertex_t(p - r, Vector2(0.0f, 0.0f), color),
		vertex_t(p + r, Vector2(0.0f, 0.0f), color),
		vertex_t(p - u, Vector2(0.0f, 0.0f), color),
		vertex_t(p + u, Vector2(0.0f, 0.0f), color),
		vertex_t(p - f, Vector2(0.0f, 0.0f), color),
		vertex_t(p + f, Vector2(0.0f, 0.0f), color),
	};

	VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, ARRAYSIZE(vertices), false);
	std::vector<VertexBuffer*> vertexBufferArray;
	vertexBufferArray.push_back(tempVBO);
	DrawVertexBufferArray(PRIMITIVE_LINES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}

void SimpleRenderer::DrawDX11Text2D(Vector2 cursor, float scale, Rgba color, DX11TextRenderer text, char* const textToWrite)
{

	char const* c = textToWrite;
	int previousCharacterCode = 0;
	//scale = 1;
	std::map<int, FontData*>::iterator it;
	std::map<int, FontData*>::iterator nextIt;
	Vector2 bottom_left;
	Vector2 top_right;
	//SetTexture2d();

	while (*c != NULL)
	{
		it = text.letter_list.find(*c);
		if (it == text.letter_list.end())
		{
			//Invalid Letter
		}
			//Consturct UV and Tex Cords
			Vector2 top_left = cursor + Vector2((float)it->second->xoffset / 1600.f, (float)-it->second->yoffset / 900.f);
			bottom_left = top_left + Vector2(0.f, (float)-it->second->height / 900.f); // Draw this
			top_right = top_left + Vector2((float)it->second->width / 1600.f, 0.f);//Draw this
																				   //Vector2 bottom_right = bottom_left + Vector2(it->second->width, 0);

			Vector2 uvTopLeft = Vector2((float)it->second->x / (float)text.scaleWidth, (float)it->second->y / (float)text.scaleHeight);
			Vector2 uvBottomLeft = uvTopLeft + Vector2(0.f, (float)it->second->height / (float)text.scaleHeight);//Draw this
			Vector2 uvBottomRight = uvTopLeft + Vector2((float)it->second->width / (float)text.scaleWidth, 0.f); // Draw This

			Vector2 tempPlaceHolder = uvBottomLeft;
			uvBottomLeft.x = uvBottomRight.x;
			uvBottomRight.x = tempPlaceHolder.x;

			draw_quad2d(top_right, bottom_left, uvBottomLeft, uvBottomRight, color);

		cursor.x += (float)it->second->xadvance / 1600.f;
		previousCharacterCode = *c;
		++c;

		int kerning = 0;
		nextIt = text.letter_list.find(*c);

		//if (nextIt == text.letter_list.end())
		//return;

		for (std::map<int, KearningCount*>::iterator step = text.kearning_list.begin(); step != text.kearning_list.end(); ++step)
		{
			
			if (step->second->first == it->first && step->second->second == nextIt->first)
				kerning = step->second->amount;
		}

		cursor.x += kerning;
	}

	draw_quad2d(top_right * scale, bottom_left * scale, Vector2(0, 0), Vector2(0, 0), color);
}

void SimpleRenderer::DrawOBJObject(ObjReader* objObject)
{
	
	std::vector<VertexBuffer*> vertexBufferArray;

	for (unsigned int i = 0; i < objObject->m_facesIndexBuffer.size(); i++)
	{
		vertex_t vertices[3];
		if (objObject->m_invertAxis == false) {
			vertex_t tempVertex;
			tempVertex.position = objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].x - 1.0f)];
			tempVertex.uv = Vector2(0.0f, 0.0f);
			tempVertex.color = rgba_fl(1.0f, 1.0f, 1.0f, 1.0f);
			vertices[0] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].x - 1)], Vector2(0.0f, 0.0f), rgba_fl(1.0f, 1.0f, 1.0f, 1.0f));
			vertices[1] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].y - 1)], Vector2(1.0f, 0.0f), rgba_fl(1, 1, 1, 1));
			vertices[2] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].z - 1)], Vector2(0.0f, 1.0f), rgba_fl(1, 1, 1, 1));
		}
		else
		{
			vertices[0] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].z - 1)], Vector2(0.0f, 0.0f), rgba_fl(1, 1, 1, 1));
			vertices[1] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].y - 1)], Vector2(1.0f, 0.0f), rgba_fl(1, 1, 1, 1));
			vertices[2] = vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].x - 1)], Vector2(0.0f, 1.0f), rgba_fl(1, 1, 1, 1));
		}
		VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 3, false);
		vertexBufferArray.push_back(tempVBO);
	}

	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);


	std::vector<VertexBuffer*>::iterator glyphIter;
	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}

void SimpleRenderer::DrawOBJObjectFlipBasis(ObjReader* objObject)
{
	std::vector<VertexBuffer*> vertexBufferArray;
	int counter = 0;
	for (unsigned int i = 0; i < objObject->m_facesIndexBuffer.size(); i++)
	{
		counter++;
		vertex_t vertices[] = {
			vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].z - 1)], Vector2(0.0f, 0.0f),rgba_fl(1,1,1,1)),
			vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].y - 1)], Vector2(1.0f, 0.0f),rgba_fl(1,1,1,1)),
			vertex_t(objObject->m_pointsVertexBuffer[(unsigned int)(objObject->m_facesIndexBuffer[i].x - 1)], Vector2(0.0f, 1.0f), rgba_fl(1,1,1,1)),
		};

		VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertices, 3, false);
		vertexBufferArray.push_back(tempVBO);
	}

	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}


void SimpleRenderer::DrawMesh(MeshBuilder* mesh)
{
	std::vector<VertexBuffer*> vertexBufferArray;

		VertexBuffer* tempVBO = m_device->CreateVertexBuffer(mesh->m_meshVerts.data(), mesh->m_meshVerts.size(), false);
		vertexBufferArray.push_back(tempVBO);



	DrawVertexBufferArray(PRIMITIVE_TRIANGLES, vertexBufferArray);

	std::vector<VertexBuffer*>::iterator glyphIter;

	for (glyphIter = vertexBufferArray.begin(); glyphIter != vertexBufferArray.end(); ++glyphIter)
	{
		delete *glyphIter;
		*glyphIter = nullptr;
	}
	vertexBufferArray.clear();
}

void SimpleRenderer::DrawSkeleton(Skeleton* skeleton)
{
	std::vector<VertexBuffer*> vertexBufferArray;
	for (unsigned int skeletonStep = 0; skeletonStep < skeleton->m_skeleton.size(); skeletonStep++)
	{
		vertex_t vertex;
		vertex_t vertex2;
		std::vector<vertex_t> vertexes;
		//Child
		Vector4 position = skeleton->m_skeleton[skeletonStep]->m_transform.m_translation;
		vertex.position = Vector3(position.x, position.y, position.z);
		vertex.uv = Vector2(0, 0);
		vertex.color = rgba_fl(255, 255, 255, 255);
		vertexes.push_back(vertex);

		//parent
		std::string parentName = skeleton->m_skeleton[skeletonStep]->parentBoneName;
			for (unsigned int parentStep = 0; parentStep < skeleton->m_skeleton.size(); parentStep++)
			{
				if (parentName == skeleton->m_skeleton[parentStep]->boneName)
				{
					Vector4 positionParent = skeleton->m_skeleton[parentStep]->m_transform.m_translation;
					vertex2.position = Vector3(positionParent.x, positionParent.y, positionParent.z);
					vertex2.uv = Vector2(0, 0);
					vertex2.color = rgba_fl(255, 255, 255, 255);
					vertexes.push_back(vertex2);
				}
			}
		VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertexes.data(), vertexes.size(), false);
		vertexBufferArray.push_back(tempVBO);
	}
	//vertex_t(Vector3 const &pos, Vector2 const &u, rgba_fl const& c)
	//VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertexes.data(), vertexes.size(), false);
	//vertexBufferArray.push_back(tempVBO);
	DrawVertexBufferArray(PRIMITIVE_LINES, vertexBufferArray);
}

void SimpleRenderer::DrawPose(Motion* currentMotion, Skeleton* skeleton)
{
	
	std::vector<VertexBuffer*> vertexBufferArray;
	for (unsigned int skeletonStep = 0; skeletonStep < skeleton->m_skeleton.size(); skeletonStep++)
	{
		vertex_t vertex;
		vertex_t vertex2;
		std::vector<vertex_t> vertexes;
		//Child
		Vector4 position = currentMotion->m_skeletonInstance->get_joint_global_transform(skeletonStep).m_translation;
		vertex.position = Vector3(position.x, position.y, position.z);
		vertex.uv = Vector2(0, 0);
		vertex.color = rgba_fl(255, 255, 255, 255);
		vertexes.push_back(vertex);

		//parent
		std::string parentName = skeleton->m_skeleton[skeletonStep]->parentBoneName;
		for (unsigned int parentStep = 0; parentStep < skeleton->m_skeleton.size(); parentStep++)
		{
			if (parentName == skeleton->m_skeleton[parentStep]->boneName)
			{
				Vector4 parentPosition = currentMotion->m_skeletonInstance->get_joint_global_transform(parentStep).m_translation;
				//Vector4 position;
				vertex2.position = Vector3(parentPosition.x, parentPosition.y, parentPosition.z);
				vertex2.uv = Vector2(0, 0);
				vertex2.color = rgba_fl(255, 255, 255, 255);
				vertexes.push_back(vertex2);
			}
		}
		VertexBuffer* tempVBO = m_device->CreateVertexBuffer(vertexes.data(), vertexes.size(), false);
		vertexBufferArray.push_back(tempVBO);
	}
	DrawVertexBufferArray(PRIMITIVE_LINES, vertexBufferArray);
}

void SimpleRenderer::SetStructureBuffer(const void* buffer, unsigned int structureSize, unsigned int structureCounter)
{
	if (m_StructreBuffer == nullptr)
	{
		m_StructreBuffer = new StructuredBuffer(m_device, buffer, structureSize, structureCounter);
	}

	m_context->set_structured_buffer(3, m_StructreBuffer);
}

void SimpleRenderer::UpdateStructureBuffer(const void* buffer)
{
	if (m_StructreBuffer != nullptr)
	{
		m_StructreBuffer->update(m_context, buffer);
	}
}


//---------------------------BLEND---------------------------------------------
void SimpleRenderer::enable_blend(eBlendFactor src, eBlendFactor dest)
{
	if (current_blend_state != nullptr) {
		if ((blend_state.enabled == true) && (blend_state.src_factor == src) && (blend_state.dst_factor == dest))
		{
			return;
		}
	}

	SAFE_DELETE(current_blend_state);

	DX11BlendState *bs = new DX11BlendState(m_device, true, src, dest);
	g_theRHI->SetBlendState(bs);

	blend_state.enabled = true;
	blend_state.src_factor = src;
	blend_state.dst_factor = dest;

	current_blend_state = bs;
}

void SimpleRenderer::disable_blend()
{
	if (!blend_state.enabled) {
		return;
	}

	SAFE_DELETE(current_blend_state);
	DX11BlendState *bs = new DX11BlendState(m_device, false);
	g_theRHI->SetBlendState(bs);

	blend_state.enabled = false;

	current_blend_state = bs;
}

//----------------------------------Setters----------------------------------------
void SimpleRenderer::SetRenderTarget(DX11Texture2D *colorTarget, DX11Texture2D *depthTarget)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->SetOutput(m_output);
	if (colorTarget != nullptr)
	{
		m_currentTarget = colorTarget;
	}
	else
	{
		m_currentTarget = m_output->GetRenderTarget();
	}

	m_context->m_dx11Context->OMSetRenderTargets(1, &m_currentTarget->m_dxRtv, nullptr);

	if (depthTarget == nullptr) {
		depthTarget = default_depth_stencil;
	}

	current_depth_stencil = depthTarget;
	g_theRHI->set_color_target(m_currentTarget, depthTarget);
}

void SimpleRenderer::SetTexture2d(unsigned int texIndex, DX11Texture2D *tex)
{
	g_theRHI->SetDeviceContext(m_context);
    g_theRHI->SetTexture2D(texIndex, tex);
}

void SimpleRenderer::UnBindUAVTexture2d(unsigned int texIndex)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->UnBindUAV(texIndex);
}

void SimpleRenderer::SetUAVTexture2d(unsigned int texIndex, DX11Texture2D *tex)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->BindUAV(texIndex, tex);
}

void SimpleRenderer::SetShader(ShaderProgram *shader)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->SetShader(shader);
}

void SimpleRenderer::SetComputeShader(ComputeShaderProgram *shader)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->SetComputeShader(shader);
}

void SimpleRenderer::UpdateClearColorBuffer()
{
	SetConstantBuffer(2, clear_cb);
	clear_cb->Update(m_context, &clear_constants_data);
}

void SimpleRenderer::SetSampler(unsigned int samplerIndex, Sampler *sampler)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->SetSampler(samplerIndex, sampler);
}

void SimpleRenderer::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	g_theRHI->SetDeviceContext(m_context);
	g_theRHI->SetViewpoint(m_context, x, y, w, h);
}

void SimpleRenderer::set_depth_stencil_state(DepthStencilState *dss)
{
	m_context->m_dx11Context->OMSetDepthStencilState(dss->m_dx11DepthStencilState, 0U);
}

void SimpleRenderer::SetConstantBuffer(unsigned int index, ConstantBuffer * timeConstants)
{
	m_context->m_dx11Context->VSSetConstantBuffers(index, 1, &timeConstants->m_dxBuffer);
	m_context->m_dx11Context->PSSetConstantBuffers(index, 1, &timeConstants->m_dxBuffer);
}

void SimpleRenderer::setComputeConstantBuffer()
{
	m_context->m_dx11Context->CSSetConstantBuffers(3, 1, &clear_cb->m_dxBuffer);
}
//----------------------Matrix Setters--------------------------------------------------
void SimpleRenderer::set_projection_matrix(Matrix4 const &proj)
{
	matrix_data.projection = proj.getTranspose();
	matrix_cb->Update(m_context, &matrix_data);
}

void SimpleRenderer::set_model_matrix(Matrix4 const &model)
{
	matrix_data.model = model.getTranspose();
	matrix_cb->Update(m_context, &matrix_data);
}

void SimpleRenderer::set_view_matrix(Matrix4 const &view)
{
	matrix_data.view = view.getTranspose();
	matrix_cb->Update(m_context, &matrix_data);
}


//------------------------------------------------------------------------
void SimpleRenderer::set_camera_matrix(Matrix4 const &camera)
{
	Vector3 position = camera.getTranslation();
	matrix_data.eye_position = Vector4(position, 0.0f);

	// this will also update the buffer
	set_view_matrix(camera.getInverseOrthonormal());
}

//------------------------------------------------------------------------
void SimpleRenderer::set_eye_position(Vector4 const &eye_position)
{
	matrix_data.eye_position = eye_position;
	matrix_cb->Update(m_context, &matrix_data);
}


void SimpleRenderer::set_ortho_projection(Vector2 const &bottom_left, Vector2 const &top_right)
{
	Matrix4 proj;
	proj = proj.MatrixMakeOrthoProjection(bottom_left.x, top_right.x, bottom_left.y, top_right.y, 0.0f, 1.0f);
	set_projection_matrix(proj);
}

void SimpleRenderer::set_perspective_projection(float const fov_radians, float aspect_ratio, float const nz, float const fz)
{
	Matrix4 proj;
	proj = proj.MatrixMakePerspectiveProjection(fov_radians, aspect_ratio, nz, fz);
	set_projection_matrix(proj);
}

void SimpleRenderer::set_model_projection(float const degrees, Vector4 translation)
{
	Matrix4 proj;
	proj.RotateDegreesAboutX(degrees);
	proj.m_translation = translation;
	set_model_matrix(proj);
}

void SimpleRenderer::setViewProjection(Vector3 m_pos, float pitchAroundX, float yawAroundY, float rollAroundZ)
{
	Matrix4 proj;

	Matrix4 pitch;
	pitch.m_jBasis.y = CosDegrees(pitchAroundX);
	pitch.m_jBasis.z = -SinDegrees(pitchAroundX);
	pitch.m_kBasis.y = SinDegrees(pitchAroundX);
	pitch.m_kBasis.z = CosDegrees(pitchAroundX);

	Matrix4 yaw;
	yaw.m_iBasis.x = CosDegrees(yawAroundY);
	yaw.m_iBasis.z = -SinDegrees(yawAroundY);
	yaw.m_kBasis.x = SinDegrees(yawAroundY);
	yaw.m_kBasis.z = CosDegrees(yawAroundY);

	Matrix4 roll;
	roll.m_iBasis.x = CosDegrees(rollAroundZ);
	roll.m_iBasis.y = -SinDegrees(rollAroundZ);
	roll.m_jBasis.x = SinDegrees(rollAroundZ);
	roll.m_jBasis.y = CosDegrees(rollAroundZ);


	//Y
	proj = proj * yaw;

	//X
	proj = proj * pitch;

	//Z
	proj = proj * roll;

	//Movement
	proj.m_translation.x -= m_pos.x;
	proj.m_translation.y -= m_pos.y;
	proj.m_translation.z -= m_pos.z;


	//Get Inverse
	//proj = proj.getInverse(proj);

	set_view_matrix(proj);
}

void SimpleRenderer::setBillboardProjection(Vector3 m_pos, float pitchAroundX, float yawAroundY, float rollAroundZ)
{
	Matrix4 proj;

	Matrix4 pitch;
	pitch.m_jBasis.y = CosDegrees(pitchAroundX);
	pitch.m_jBasis.z = -SinDegrees(pitchAroundX);
	pitch.m_kBasis.y = SinDegrees(pitchAroundX);
	pitch.m_kBasis.z = CosDegrees(pitchAroundX);

	Matrix4 yaw;
	yaw.m_iBasis.x = CosDegrees(yawAroundY);
	yaw.m_iBasis.z = -SinDegrees(yawAroundY);
	yaw.m_kBasis.x = SinDegrees(yawAroundY);
	yaw.m_kBasis.z = CosDegrees(yawAroundY);

	Matrix4 roll;
	roll.m_iBasis.x = CosDegrees(rollAroundZ);
	roll.m_iBasis.y = -SinDegrees(rollAroundZ);
	roll.m_jBasis.x = SinDegrees(rollAroundZ);
	roll.m_jBasis.y = CosDegrees(rollAroundZ);

	//Movement


	//Y
	proj = proj * yaw;

	//X
	proj = proj * pitch;

	//Z
	proj = proj * roll;


	//Get Inverse
	proj = proj.getInverse(proj);

	proj.m_translation.x -= m_pos.x;
	proj.m_translation.y -= m_pos.y;
	proj.m_translation.z -= m_pos.z;

	set_model_matrix(proj);
}



void SimpleRenderer::enable_depth(bool test, bool write)
{
	if ((depth_stencil_desc.depth_writing_enabled != write)|| (depth_stencil_desc.depth_test_enabled != test)) 
	{
		depth_stencil_desc.depth_writing_enabled = write;
		depth_stencil_desc.depth_test_enabled = test;
		SAFE_DELETE(depth_stencil_state);
		depth_stencil_state = new DepthStencilState(m_device, depth_stencil_desc);
		set_depth_stencil_state(depth_stencil_state);
	}
}

void SimpleRenderer::set_ambient_light(float intensity, rgba_fl const &color /*= rgba_fl::WHITE*/)
{
	light_data.ambient = rgba_fl(color.r, color.g, color.b, intensity);
}

void SimpleRenderer::set_point_light(int index, Vector3 const &pos, rgba_fl const &color, float intensity /*= 1.0f*/, Vector3 const &attenuation /*= vec3( 0, 0, 1 )*/, Vector3 const &spec_attenuation)
{
	light_data.lightBufferArray[index].light_position = Vector4(pos, 1.0f);
	light_data.lightBufferArray[index].light_color = rgba_fl(color.r, color.g, color.b, intensity);
	light_data.lightBufferArray[index].attenuation = Vector4(attenuation, 0.0f);
	light_data.lightBufferArray[index].spec_attenuation = Vector4(spec_attenuation, 0.0f);
}

void SimpleRenderer::UpdateLighting()
{
	SetConstantBuffer(2, light_cb);
	light_cb->Update(m_context, &light_data);
}
void SimpleRenderer::set_specular_constants(float const spec_power, float const spec_factor /*= 1.0f*/)
{
	light_data.spec_factor = spec_factor;
	light_data.spec_power = spec_power;

	light_cb->Update(m_context, &light_data);
}

#if defined(TOOLS_BUILD) 

//----------------------------Create or Get Mesh/Skeleton/Motion----------------------------------------------------------------------
MeshBuilder* SimpleRenderer::CreateOrGetMesh(const char* meshFilePath, Skeleton* skeleton, bool readFromBinaryFile)
{
	// Try to find that texture from those already loaded
	MeshBuilder* mesh = GetMesh(meshFilePath);
	if (mesh)
		return mesh;

	mesh = new MeshBuilder();
	if(readFromBinaryFile)
		ConvertMeshFileToData(meshFilePath,  mesh);
	else
		FbxLoadMesh(mesh, meshFilePath, skeleton);
	
	m_alreadyLoadedMeshes[meshFilePath] = mesh;
	return mesh;
}


//-----------------------------------------------------------------------------------------------
MeshBuilder* SimpleRenderer::GetMesh(const char* meshFilePath)
{
	if (m_alreadyLoadedMeshes.empty())
		return nullptr;
	
	if (m_alreadyLoadedMeshes.find(meshFilePath) == m_alreadyLoadedMeshes.end())
		return nullptr;
	else 
		return m_alreadyLoadedMeshes[meshFilePath];
}

Skeleton* SimpleRenderer::CreateOrGetSkeleton(const char* skeletonFilePath, bool readFromBinaryFile)
{
	// Try to find that texture from those already loaded
	Skeleton* skeleton = GetSkeleton(skeletonFilePath);
	if (skeleton)
		return skeleton;

	skeleton = new Skeleton();
	if(readFromBinaryFile)
		ConvertSkeletonFileToData(skeletonFilePath, skeleton);
	else
		FbxLoadSkeleton(skeleton,  skeletonFilePath);

	m_alreadyLoadedSkeletons[skeletonFilePath] = skeleton;
	return skeleton;
}

Skeleton* SimpleRenderer::GetSkeleton(const char* skeletonFilePath)
{
	if (m_alreadyLoadedSkeletons.empty())
		return nullptr;

	if (m_alreadyLoadedSkeletons.find(skeletonFilePath) == m_alreadyLoadedSkeletons.end())
		return nullptr;
	else
		return m_alreadyLoadedSkeletons[skeletonFilePath];

}

Motion* SimpleRenderer::CreateOrGetMotion(const char* motionFilePath, Skeleton* skeleton, bool readFromBinaryFile)
{
	// Try to find that texture from those already loaded
	Motion* motion = GetMotion(motionFilePath);
	if (motion)
		return motion;

	motion = new Motion();
	if(readFromBinaryFile)
		ConvertMotionFileToData(motionFilePath, *motion, skeleton);
	else
	    FbxLoadMotion(motion, skeleton,  motionFilePath, 60);

	m_alreadyLoadedMotions[motionFilePath] = motion;
	return motion;
}

Motion* SimpleRenderer::GetMotion(const char* motionFilePath)
{
	if (m_alreadyLoadedMotions.empty())
		return nullptr;

	if (m_alreadyLoadedMotions.find(motionFilePath) == m_alreadyLoadedMotions.end())
		return nullptr;
	else
		return m_alreadyLoadedMotions[motionFilePath];

}

ComputeShaderProgram* SimpleRenderer::CreateorGetComputeShaderProgram(const char* shaderFilePath)
{
	// Try to find that texture from those already loaded
	ComputeShaderProgram* shader = GetComputeShaderProgram(shaderFilePath);
	if (shader)
		return shader;

	shader = m_device->CreateComputeShaderFromHlslFile(shaderFilePath);

	m_alreadyLoadedComputeShaders[shaderFilePath] = shader;
	return shader;
}


ComputeShaderProgram* SimpleRenderer::GetComputeShaderProgram(const char* shaderFilePath)
{
	if (m_alreadyLoadedComputeShaders.empty())
		return nullptr;

	if (m_alreadyLoadedComputeShaders.find(shaderFilePath) == m_alreadyLoadedComputeShaders.end())
		return nullptr;
	else
		return m_alreadyLoadedComputeShaders[shaderFilePath];
}


ShaderProgram* SimpleRenderer::CreateorGetShaderProgram(const char* shaderFilePath)
{
	// Try to find that texture from those already loaded
	ShaderProgram* shader = GetShaderProgram(shaderFilePath);
	if (shader)
		return shader;

	shader = m_device->CreateShaderFromHlslFile(shaderFilePath);

	m_alreadyLoadedShaders[shaderFilePath] = shader;
	return shader;
}

ShaderProgram* SimpleRenderer::GetShaderProgram(const char* shaderFilePath)
{
	if (m_alreadyLoadedShaders.empty())
		return nullptr;

	if (m_alreadyLoadedShaders.find(shaderFilePath) == m_alreadyLoadedShaders.end())
		return nullptr;
	else
		return m_alreadyLoadedShaders[shaderFilePath];
}

DX11Texture2D* SimpleRenderer::CreateOrGetDX11Texture2D(const char* dx11FilePath)
{
	// Try to find that texture from those already loaded
	DX11Texture2D* texture = GetDX11Texture2D(dx11FilePath);
	if (texture)
		return texture;

	texture = new DX11Texture2D(m_device, dx11FilePath);


	m_alreadyLoadedTextures[dx11FilePath] = texture;
	return texture;
}

DX11Texture2D* SimpleRenderer::GetDX11Texture2D(const char* dx11FilePath)
{
	if (m_alreadyLoadedTextures.empty())
		return nullptr;

	if (m_alreadyLoadedTextures.find(dx11FilePath) == m_alreadyLoadedTextures.end())
		return nullptr;
	else
		return m_alreadyLoadedTextures[dx11FilePath];

}

void LoadImageFromFileJob(void *job_data2)
{
	load_texture_job_t *jobs = (load_texture_job_t*) job_data2;
	jobs->image->LoadFromFile(jobs->filename);
}

void LoadTextureFromImageJob(void* job_data)
{
	// if image is empty in this scheme, the previous step failed
	// so fill texture with something ugly, 
	// otherwise do the normal texture creation code
	load_texture_job_t *jobs = (load_texture_job_t*)job_data;

	if(jobs->image == nullptr)
		jobs->image->LoadFromFile("cube.jpg");
	jobs->texture = jobs->renderer->CreateOrGetDX11Texture2D(jobs->filename);
}

void CleanupJob(void *ptr)
{
	delete ptr;
}


void CallLoadTextureCallbackJob(void *user_arg)
{
	load_texture_job_t *jobs = (load_texture_job_t*)user_arg;
	jobs->renderer->Texture2d = jobs->texture;
}

DX11Texture2D* SimpleRenderer::LoadTextureAsync(char const *filename, on_texture_loaded_cb cb /*= nullptr*/, void *user_arg /*= nullptr*/)
{
	cb; user_arg;
	
	DX11Texture2D* texture = CreateOrGetDX11Texture2D(filename);
	if (nullptr == texture) {
		return nullptr;
	}


		// alright, texture is loaded
		// let's create the job; 

		// we new this off - since it, or parts of it, will be used by multiple threads.
		load_texture_job_t* job_data = new load_texture_job_t();
		job_data->filename = filename;
		job_data->image = new Image();
		job_data->texture = texture;
		job_data->renderer = this;

		// I'm using some existing helpers for parameter passing.  
		Job *load_image = JobCreate(JOB_RENDER, LoadImageFromFileJob, job_data);
		Job *image_to_texture = JobCreate(JOB_RENDER, LoadTextureFromImageJob, job_data);
		image_to_texture->dependent_on(load_image);
		JobDispatchAndRelease(load_image);

		// who frees are resources in this scheme?
		// no one - jobs are generic, so we need a cleanup job that will
		// free the job_data 
		JobDispatchAndRelease(image_to_texture);


		//if (cb != nullptr)
	//	{
			Job *cb_job = JobCreate(JOB_MAIN, CallLoadTextureCallbackJob, job_data);
			cb_job->dependent_on(image_to_texture);

			// we dispatch immediately - it won't actually 
			// run yet since image_to_texture has not been dispatched
			// and we have a dependency.
			JobDispatchAndRelease(cb_job);
			Job *cleanup = JobCreate(JOB_GENERIC, CleanupJob, job_data);
			cleanup->dependent_on(cb_job); // after this job, no one uses anything in job_data
			JobDispatchAndRelease(cleanup);
	//	}

		// dispatch and release the final two jobs

	// will return whether loaded or not - up to user to 
	// check loaded state before use in this example.
	return texture;
}

#else
#endif