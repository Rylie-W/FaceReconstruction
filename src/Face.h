#pragma once
#include "Image.h"
#include "FaceModel.h"
#include "Utils.h"

// Class for a reconstructed 3D face
class Face {
public:
	Face(std::string _imageFileName = "sample1", std::string _faceModel = "BFM17") {
		image = Image(_imageFileName);
		faceModel = FaceModel(_faceModel);		
		alpha = VectorXf::Zero(faceModel.getAlphaSize());
		beta = VectorXf::Zero(faceModel.getBetaSize());
		gamma = VectorXf::Zero(faceModel.getGammaSize());
		intrinsics = Matrix3f::Identity();
		extrinsics = Matrix4f::Identity();
	}

	// Calls DataHandler to write the recontructed mesh in .obj format
	void writeReconstructedFace() {
		Mesh mesh = toMesh();
		DataHandler::writeMesh(mesh, image.getFileName());
	}
	
	// Transfer the expression from the source face to the current face
	void transferExpression(const Face& sourceFace) {

	}

	// Randomize parameters (for testing purpose)
	void randomizeParameters() {
		alpha = VectorXf::Random(faceModel.getAlphaSize());
		beta = VectorXf::Random(faceModel.getBetaSize());
		gamma = VectorXf::Random(faceModel.getGammaSize());
	}

	// construct the mesh with alpha, beta, gamma and face model variables
	Mesh toMesh() {
		Mesh mesh;
		mesh.vertices = calculateVertices();
		mesh.colors = calculateColors();
		mesh.faces = faceModel.getTriangulation();
		return mesh;
	}

	// getters and setters

	void setAlpha(VectorXf _alpha) {
		alpha = _alpha;
	}
	VectorXf getAlpha() {
		return alpha;
	}
	void setBeta(VectorXf _beta) {
		beta = _beta;
	}
	VectorXf getBeta() {
		return beta;
	}
	void setGamma(VectorXf _gamma) {
		gamma = _gamma;
	}
	VectorXf getGamma() {
		return gamma;
	}
	void setIntrinsics(Matrix3f _intrinsics) {
		intrinsics = _intrinsics;
	}
	Matrix3f getIntrinsics() {
		return intrinsics;
	}
	void setExtrinsics(Matrix4f _extrinsics) {
		extrinsics = _extrinsics;
	}
	Matrix4f getExtrinsics() {
		return extrinsics;
	}
	Image getImage() {
		return image;
	}
	FaceModel getFaceModel() {
		return faceModel;
	}

private:
	VectorXf alpha, beta, gamma;	// parameters to optimize
	Matrix3f intrinsics;	// given by camera manufacturer, otherwise hardcode it?
	Matrix4f extrinsics;	// given by optimization
	Image image;	// the corresponding image
	FaceModel faceModel;	// the used face model, ie BFM17
	MatrixX3f normals; // normal of the vertices

	// geometry
	MatrixX3f calculateVertices() {
		MatrixXf vertices = faceModel.getShapeMean() + faceModel.getShapeBasis() * alpha;
		vertices.resize(faceModel.getNumVertices(), 3);
		return vertices;
	}
	// color
	MatrixX3f calculateColors() {
		MatrixXf colors = faceModel.getColorMean() + faceModel.getColorBasis() * beta;
		colors.resize(faceModel.getNumVertices(), 3);
		return colors;
	}

};