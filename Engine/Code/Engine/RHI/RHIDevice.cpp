#include "Engine/RHI/RHIDevice.hpp"



//--------------------------Creation and Destruction-----------------------------------------------
RHIDevice::RHIDevice(RHIInstance *instance, ID3D11Device *dx11Device)
{
	m_instance = instance;
	m_dx11Device = dx11Device;
	m_immediateContext = nullptr;
}

 ShaderProgram* RHIDevice::CreateShaderFromHlslFile(char const *filename)
{
	 size_t sourceSize = 0U;
	 void *sourceCode = FileReadToBuffer(filename, &sourceSize);
	 if (sourceCode == nullptr) 
		 return nullptr;

	 ID3DBlob *vertexShaderByteCode = nullptr;
	 ID3DBlob *fragmentShaderByteCode = nullptr;

	// Compile hlsl to byte code (generic compiled version)
	 vertexShaderByteCode = CompileHLSLToShaderBlob(filename, sourceCode, sourceSize, "VertexFunction", "vs_5_0");
	 fragmentShaderByteCode = CompileHLSLToShaderBlob(filename, sourceCode, sourceSize, "FragmentFunction", "ps_5_0");
	// delete sourceCode;


	 if (vertexShaderByteCode != nullptr && fragmentShaderByteCode != nullptr)
	 {
		 ID3D11VertexShader *vertexShader = nullptr;
		 ID3D11PixelShader *fragmentShader = nullptr;

		 m_dx11Device->CreateVertexShader(vertexShaderByteCode->GetBufferPointer(), vertexShaderByteCode->GetBufferSize(), nullptr, &vertexShader);
		 m_dx11Device->CreatePixelShader(fragmentShaderByteCode->GetBufferPointer(), fragmentShaderByteCode->GetBufferSize(), nullptr,&fragmentShader);
		 ShaderProgram *program = new ShaderProgram(this, vertexShader, fragmentShader, vertexShaderByteCode, fragmentShaderByteCode);
		 return program;
	 }

	return nullptr;
}


 ComputeShaderProgram* RHIDevice::CreateComputeShaderFromHlslFile(char const *filename)
 {
	 size_t sourceSize = 0U;
	 void *sourceCode = FileReadToBuffer(filename, &sourceSize);
	 if (sourceCode == nullptr)
		 return nullptr;

	 ID3DBlob *fragmentShaderByteCode = nullptr;

	 fragmentShaderByteCode = CompileHLSLToShaderBlob(filename, sourceCode, sourceSize, "FragmentFunction", "cs_5_0");

	 if (fragmentShaderByteCode != nullptr)
	 {

		 ID3D11ComputeShader* computeShader = nullptr;

		 m_dx11Device->CreateComputeShader(fragmentShaderByteCode->GetBufferPointer(), fragmentShaderByteCode->GetBufferSize(), nullptr, &computeShader);


		ComputeShaderProgram * program = new ComputeShaderProgram(computeShader, fragmentShaderByteCode);
		 return program;
	 }

	 return nullptr;
 }

 ID3DBlob* RHIDevice::CompileHLSLToShaderBlob(char const *optionalFileName,  void const *sourceCode, size_t const sourceCodeSize, char const *entryPoint,  char const* target)                                            
 {
	 DWORD compile_flags = 0U;
#if defined(DEBUG_SHADERS)
	 compile_flags |= D3DCOMPILE_DEBUG;
	 compile_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	 compile_flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else 
	 compile_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // Yay, fastness (default is level 1)
#endif

	 ID3DBlob *code = nullptr;
	 ID3DBlob *errors = nullptr;

	 HRESULT hr = ::D3DCompile(sourceCode,
		 sourceCodeSize, // plain text source code
		 optionalFileName, // optional, used for error messages (If you HLSL has includes - it will not use the includes names, it will use this name)
		 nullptr, // pre-compiler defines - used more for compiling multiple versions of a single shader (different quality specs, or shaders that are mostly the same outside some constants)
		 D3D_COMPILE_STANDARD_FILE_INCLUDE, // include rules - this allows #includes in the shader to work relative to the src_file path or my current working directly
		 entryPoint,// Entry Point for this shader
		 target, // Compile Target (MSDN - "Specifying Compiler Targets")
		 compile_flags,// Flags that control compilation
		 0, // Effect Flags (we will not be doing Effect Files)
		 &code, // [OUT] ID3DBlob (buffer) that will store the byte code.
		 &errors);  // [OUT] ID3DBlob (buffer) that will store error information


	 //HLSL Error File Printer
	 if (FAILED(hr) || (errors != nullptr))
	 {
		 if (errors != nullptr) 
		 {
			 char *error_string = (char*)errors->GetBufferPointer();
			 DebuggerPrintf("Failed to compile [%s].  Compiler gave the following output;\n%s",optionalFileName, error_string);
		 }
	 }

	 return code; // will be nullptr if it failed to compile
 }

 VertexBuffer* RHIDevice::CreateVertexBuffer(vertex_t* vertices, unsigned int vertexCount, bool isConsole)
 {
	 VertexBuffer *buffer = new VertexBuffer(this, vertices, vertexCount, isConsole);
	 return buffer;
 }

DX11Texture2D* RHIDevice::createRenderTarget(unsigned int width, unsigned int height)
 {
	 DX11Texture2D *tex = new DX11Texture2D(this, width, height, IMAGEFORMAT_RGBA8);
	 return tex;
 }

DX11Texture2D* RHIDevice::createDepthTarget(unsigned int width, unsigned int height)
 {
	 DX11Texture2D *tex = new DX11Texture2D(this, width, height, IMAGEFORMAT_D24S8);
	 return tex;
 }




 RHIDevice::~RHIDevice()
 {
	 delete m_immediateContext;
	 DX_SAFE_RELEASE(m_dx11Device);
	 delete m_instance;
 }


 //-----------------------------------Getters----------------------------------------
 RHIDeviceContext* RHIDevice::GetImmediateContext()
 {
	 return m_immediateContext;
 }