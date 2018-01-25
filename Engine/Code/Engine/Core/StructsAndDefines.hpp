#pragma once
#if !defined( __RENDER_VERTEX__ )
#define __RENDER_VERTEX__
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/UVector3.hpp"
#include "Engine/Math/UVector4.hpp"
#include "Engine/Core/Rgba.hpp"




float constexpr PI = (3.1415926535897932384626f);

//------------------------------------------------------------------------------
#define VERTEX_MAX_UVS (4)
#define UNUSED(x) (void)(x);
#define SAFE_DELETE(ptr)   if (nullptr != (ptr)) { delete ptr; ptr = nullptr; }
#define D2R(deg) ((deg) * (PI / 180.0f))
#if !defined UNREFERENCED
#define UNREFERENCED(v) v;
#endif
//---------------------------------------------------------------------------

struct vertex_t
{
public:
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	rgba_fl color;
	Vector3 tangent;
	UVector4 boneIndices; // uvec4 is a vec4 of 4 unsigned ints
	Vector4 boneWeights; 
public:
	vertex_t() {}
	vertex_t(Vector3 const &pos, Vector2 const &u, rgba_fl const& c)
		: position(pos),
		normal(0,0,-1),
		uv(u),
		color(c),
		tangent(0,0,0),
		boneIndices(0,0,0,0),
		boneWeights(0,0,0,0)
	{}
	vertex_t(Vector3 const &pos, Vector3 const &norm, Vector2 const &u, rgba_fl const& c)
		: position(pos),
		normal(norm),
		uv(u),
		color(c),
		tangent(0, 0, 0),
		boneIndices(0, 0, 0, 0),
		boneWeights(0, 0, 0, 0)
	{}
	vertex_t(Vector3 const &pos, Vector3 const &norm, Vector2 const &u, rgba_fl const& c, Vector3 const& tan)
		: position(pos),
		normal(norm),
		uv(u),
		color(c),
		tangent(tan),
		boneIndices(0, 0, 0, 0),
		boneWeights(0, 0, 0, 0)
	{}
	vertex_t(Vector3 const &pos, Vector3 const &norm, Vector2 const &u, rgba_fl const& c, Vector3 const& tan, UVector4 const & boneInd, Vector4 const & boneWeight)
		: position(pos),
		normal(norm),
		uv(u),
		color(c),
		tangent(tan),
		boneIndices(boneInd),
		boneWeights(boneWeight)
	{}
};

struct compute_t
{
public:
	UVector3 group_id;
	UVector3  group_idx;
	UVector3 group_thread_id;
	UVector3 dispatch_id;
public:
	compute_t() {}
	compute_t(UVector3 const &groupid, UVector3 const &groupindex, UVector3 const & groupthread, UVector3 const &dispatch)
		: group_id(groupid),
		group_idx(groupindex),
		group_thread_id(groupthread),
		dispatch_id(dispatch)
	{}
};

struct time_constant_t
{
	float time;

	Vector3 padding;
};

//---------------------------------------------------------------------
template <typename T>
inline void MemZero(T *ptr)
{
	memset(ptr, 0, sizeof(T));
}

template <typename T>
inline void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

#endif 
