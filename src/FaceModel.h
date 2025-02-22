#pragma once
#include "DataHandler.h"

// Basis size. ALPHA and GAMMA could be as maximum 199 and BETA 100 for BFM17
#define BFM_ALPHA_SIZE 100
#define BFM_BETA_SIZE 100
#define BFM_GAMMA_SIZE 100

// Class for a face model (BFM17)
class FaceModel{
public:
	FaceModel() {
	}
	// contructor to ni
	FaceModel(std::string _faceModel) {
		faceModelName = _faceModel;
		DataHandler::readBasis(_faceModel, "shape", shapeBasis, BFM_ALPHA_SIZE);
		DataHandler::readBasis(_faceModel, "color", colorBasis, BFM_BETA_SIZE);
		DataHandler::readBasis(_faceModel, "expression", expBasis, BFM_GAMMA_SIZE);
		DataHandler::readMean(_faceModel, "shape", shapeMean);
		DataHandler::readMean(_faceModel, "color", colorMean);
		DataHandler::readMean(_faceModel, "expression", expMean);
		DataHandler::readVariance(_faceModel, "shape", shapeVar);
		DataHandler::readVariance(_faceModel, "color", colorVar);
		DataHandler::readVariance(_faceModel, "expression", expVar);
		DataHandler::readTriangulation(_faceModel, triangulation);
		DataHandler::readFaceModelLandmarks(_faceModel, landmarks);
		updateBasisWithStd();	// basis with std applied
		extraVertices = 0;
		shapeMean = shapeMean * 1.2 / 1000.;
		expMean = expMean * 1.2 / 1000.;
		shapeBasis = shapeBasis * 1.2 / 1000.;
		expBasis = expBasis * 1.2 / 1000.;
	}
	// getter and setters
	std::string getFaceModelName() {
		return faceModelName;
	}
	unsigned int getNumVertices() const {
		return shapeBasis.rows()/3 + extraVertices;
	}

	unsigned int getAlphaSize() const {
		return shapeBasis.cols();
	}

	unsigned int getBetaSize() const {
		return colorBasis.cols();
	}

	unsigned int getGammaSize() const {
		return expBasis.cols();
	}

	VectorXd getShapeMean() const {
		return shapeMean;
	}

	VectorXd getShapeMeanBlock(unsigned startRow, unsigned numRows) const {
		return shapeMean.block(startRow, 0, numRows, 1);
	}

	double getShapeMeanElem(unsigned idx) const {
		return shapeMean(idx, 0);
	}

	VectorXd getColorMean() const {
		return colorMean;
	}

	VectorXd getColorMeanBlock(unsigned startRow, unsigned numRows) const {
		return colorMean.block(startRow, 0, numRows, 1);
	}

	double getColorMeanElem(unsigned idx) const {
		return colorMean(idx, 0);
	}

	VectorXd getExpMean() const {
		return expMean;
	}

	VectorXd getExpMeanBlock(unsigned startRow, unsigned numRows) const {
		return expMean.block(startRow, 0, numRows, 1);
	}

	double getExpMeanElem(unsigned idx) const {
		return expMean(idx, 0);
	}

	VectorXd getShapeVar() const {
		return shapeVar;
	}

	VectorXd getColorVar() const {
		return colorVar;
	}

	VectorXd getExpVar() const {
		return expVar;
	}

	MatrixXd getShapeBasis() const {
		return shapeBasis;
	}

	MatrixXd getShapeBasisRowBlock(unsigned startRow, unsigned numRows) const {
		return shapeBasis.middleRows(startRow,  numRows);
	}

	double getShapeBasisElem(unsigned i, unsigned j) const {
		return shapeBasis(i, j);
	}

	MatrixXd getColorBasis() const {
		return colorBasis;
	}

	MatrixXd getColorBasisRowBlock(unsigned startRow, unsigned numRows) const {
		return colorBasis.middleRows(startRow, numRows);
	}

	double getColorBasisElem(unsigned i, unsigned j) const {
		return colorBasis(i, j);
	}

	MatrixXd getExpBasis() const {
		return expBasis;
	}

	MatrixXd getExpBasisRowBlock(unsigned startRow, unsigned numRows) const {
		return expBasis.middleRows(startRow, numRows);
	}

	double getExpBasisElem(unsigned i, unsigned j) const {
		return expBasis(i, j);
	}

	void setTriangulation(MatrixX3i triangulation) {
		this->triangulation = triangulation;
	}

	void setExtraVertices(int numExtraVertices) {
		this->extraVertices = numExtraVertices;
	}

	MatrixX3i getTriangulation() const {
		return triangulation;
	}

	Vector3i getTriangulationByRow(int row) const {
		return triangulation.row(row);
	}

	unsigned getTriangulationRows() const {
		return triangulation.rows();
	}

	unsigned getNumLandmarks() const {
		return landmarks.rows();
	}

	VectorXi getLandmarks() const {
		return landmarks;
	}

	unsigned getLandmarkVertexIdx(unsigned i) const {
		return landmarks.row(i).value();
	}

private:
  std::string faceModelName;
  // Mean of the eigenvectors in the basis. Shape = 3*num_vertices (because each vertex is a 3D point)
  VectorXd shapeMean, colorMean, expMean;
  // Variance of the eigenvectors in the basis. Shape = num_eigenvectors
  VectorXd shapeVar, colorVar, expVar;
  // Basis formed by the eigvectors. Shape = [3*num_vectices, num_eigenvectors]
  MatrixXd shapeBasis, colorBasis, expBasis;	// note that the basis stored after the initialization in  the constructor already has the std multiplied
  // triangulation of the vertices. Shape = [num_faces, 3]
  MatrixX3i triangulation;
  // vector with index of the vertices corresponding to the facial landmarks. Shape = [68, 1]
  VectorXi landmarks;
  // update the basis with its corresponding std
  void updateBasisWithStd() {
	  shapeBasis = (shapeVar.cwiseSqrt().asDiagonal() * shapeBasis.transpose()).transpose();
	  colorBasis = (colorVar.cwiseSqrt().asDiagonal() * colorBasis.transpose()).transpose();
	  expBasis = (expVar.cwiseSqrt().asDiagonal() * expBasis.transpose()).transpose();
  }
  // extra vertices that doesnt belong to the face model
  int extraVertices;
};