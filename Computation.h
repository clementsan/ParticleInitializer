#ifndef COMPUTATION_H
#define COMPUTATION_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include <itkCovariantVector.h>

//Mesh Information
typedef itk::CovariantVector<double,3> CovariantVectorType;

class Computation
{
 public:
  Computation(){};
  ~Computation(){};
  void ReadMesh(std::string _FileName);
  void ReadAttributeFile(std::string _FileName);
  void ComputeAttributeValues();
  void ComputeParticles();
  void SortAttributes();
  void WriteAttributeFile(std::string _FileName);
  void WritePointsFile(std::string _FileName); 

 private:  
  int m_NbVertices;
  std::vector<CovariantVectorType> m_vVertices;

  // Option Mean Vertex
  std::vector<int> m_vParticleIndex;
  std::vector<int> m_vScalarAttribute; // scalar attributes for each vertex
  std::vector<int> m_vAttributeValues; // vector containing the distinct attributes values

  // Option Save Points
  std::vector<int> m_vScalar;
};

#endif
