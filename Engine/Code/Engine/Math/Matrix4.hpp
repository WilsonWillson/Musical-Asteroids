#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StructsAndDefines.hpp"
#include <math.h>


//-----------------------------------------------------------------------------------------------
class Matrix4
{
public:

	Matrix4();
	Matrix4(float initialX, float initialY, float initalZ);
	Matrix4(const  Matrix4& copy);
	Matrix4(const float* arrayOfFloats);
	Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation = Vector2(0.f, 0.f));
	Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation = Vector3(0.f, 0.f, 0.f));
	Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation = Vector4(0.f, 0.f, 0.f, 1.f));
	
	//Create Matrix's Based off of Main Matrix
	static Matrix4 CreateTranslation(const Vector2& translation2D); // z translation assumed to be 0
	static Matrix4 CreateTranslation(const Vector3& translation3D);
	static Matrix4 CreateScale(float uniformScale);
	static Matrix4 CreateScale(const Vector2& nonUniformScale2D); // z scale assumed to be 1
	static Matrix4 CreateScale(const Vector3& nonUniformScale3D);
	static Matrix4 CreateRotationDegreesAboutX(float degrees);
	static Matrix4 CreateRotationDegreesAboutY(float degrees);
	static Matrix4 CreateRotationDegreesAboutZ(float degrees); // a.k.a. CreateRotationDegrees2D
	static Matrix4 CreateRotationRadiansAboutX(float radians);
	static Matrix4 CreateRotationRadiansAboutY(float radians);
	static Matrix4 CreateRotationRadiansAboutZ(float radians); // a.k.a. CreateRotationRadians2D

	//Matrix Transforms
	Vector2 TransformPosition(const Vector2& position2D) const; // Assumes z=0, w=1
	Vector3 TransformPosition(const Vector3& position3D) const; // Assumes w=1
	Vector2 TransformDirection(const Vector2& direction2D) const; // Assumes z=0, w=0
	Vector3 TransformDirection(const Vector3& direction3D) const; // Assumes w=0
	Vector4 TransformVector(const Vector4& homogeneousVector) const; // w is provided


	//Do stuff to Matrix
	void MakeIdentity();
	void Translate(const Vector2& translation2D); // z translation assumed to be 0
	void Translate(const Vector3& translation3D);
	void Scale(float uniformScale);
	void Scale(const Vector2& nonUniformScale2D); // z scale assumed to be 1
	void Scale(const Vector3& nonUniformScale3D);
	void RotateDegreesAboutX(float degrees);
	void RotateDegreesAboutY(float degrees);
	void RotateDegreesAboutZ(float degrees); // Possibly also offered as RotateDegrees2D
	void RotateRadiansAboutX(float radians);
	void RotateRadiansAboutY(float radians);
	void RotateRadiansAboutZ(float radians); // Possibly also offered as RotateRadians2D


	//Others
	void ConcatenateTransform(Matrix4 otherMatrix);
	void Transpose();
	Matrix4 MatrixMakeLookAt(Vector3 const &position, Vector3 const &target, Vector3 const &up = Vector3(0.0f, 1.0f, 0.0f));
	//Vector3 transform(Vector3 const &v);
	void invert_orthonormal();
	Matrix4 MatrixMakeOrthoProjection(float nx, float fx, float ny, float fy, float nz /*= 0.0f*/, float fz /*= 1.0f*/);
	Matrix4 MatrixMakePerspectiveProjection(float const fov_radians, float const aspect_ratio, float const nz, float const fz);


	//Getters and Setter
	Matrix4 getTransform(Matrix4 otherMatrix);
	Matrix4 getInverse(Matrix4 baseMatrix);
	Matrix4 getTranspose() const;
	Matrix4 getInverseOrthonormal() const;
	Vector4 getColumn(unsigned int idx);
	Vector3 getTranslation() const;
	float getDeterminate();
	float* GetAsFloatArray();
	const float* GetAsFloatArray() const;

	void set_translation(Vector3 const &pos);

	//OPERATORS
	bool operator== (const Matrix4& vectorToEqual) const;
	Matrix4 operator* ( const Matrix4& mulB);
	Matrix4 operator* (const float& number);

public:
	Vector4 m_iBasis;
	Vector4 m_jBasis;
	Vector4 m_kBasis;
	Vector4 m_translation;


};