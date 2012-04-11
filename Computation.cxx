#include "Computation.h"

void Computation::ReadMesh(std::string _FileName)
{
  std::cout<<"Reading mesh..."<<std::endl;
  
  std::ifstream Infile;
  std::string fileExtension, line;
  char Line[40], NbVertices[10];
  std::size_t found1, found2, length;
  CovariantVectorType Vertex1, Vertex2, Vertex3;
  int CurrentPoint;
   
  m_vVertices.clear();  
  
  Infile.open(_FileName.c_str());  

  int lastPoint = _FileName.rfind('.');
  fileExtension = _FileName.substr(lastPoint);

  if (fileExtension.compare(".meta") == 0 )
    {
      //Skips the header and gets the number of points
      while ( strncmp (Line, "NPoints =", 9) && strncmp (Line, "NPoints=", 8))
	Infile.getline (Line, 40);    
      m_NbVertices = atoi(strrchr(Line,' '));
      
      Infile.getline ( Line, 40);

      //read the points in the file and set them as vertices
      for (int i = 0; i < m_NbVertices; i++ )
	{
	  Infile >> CurrentPoint >> Vertex1[0] >> Vertex1[1] >> Vertex1[2];
	  m_vVertices.push_back(Vertex1);
	}
    }
  else if (fileExtension.compare(".vtk") == 0 )
    {
      while ( strncmp (Line, "POINTS", 6))
	Infile.getline (Line, 40);
      line = Line;
      found1 = line.find(' ');
      found2 = line.find(' ',found1+1);
      length = line.copy(NbVertices,found2-found1-1,found1+1);
      NbVertices[length]='\0';
      m_NbVertices = atoi(NbVertices);
      
      for (int i = 0; i < m_NbVertices/3; i++ )
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2] >> Vertex2[0] >> Vertex2[1] >> Vertex2[2] >> Vertex3[0] >> Vertex3[1] >> Vertex3[2];
	  m_vVertices.push_back(Vertex1);
	  m_vVertices.push_back(Vertex2);
	  m_vVertices.push_back(Vertex3);	  
	}
      if ((m_NbVertices % 3) == 1)
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2];
	  m_vVertices.push_back(Vertex1);
	}
      else if ((m_NbVertices % 3) == 2)
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2] >> Vertex2[0] >> Vertex2[1] >> Vertex2[2];
	  m_vVertices.push_back(Vertex1);
	  m_vVertices.push_back(Vertex2);
	}      
    }
  //close file
  Infile.close();
}

void Computation::ReadAttributeFile(std::string _FileName)
{
  std::cout<<"Reading attribute file..."<<std::endl;

  std::ifstream Infile;
  char Line[40];
  int NbVertices;
  int ScalarAttribute;

  Infile.open(_FileName.c_str());  
  //Skips the header and gets the number of points
  while ( strncmp (Line, "NUMBER_OF_POINTS=", 17))
    Infile.getline (Line, 40);    
  
  NbVertices = atoi(strrchr(Line,'=')+1);
  if (NbVertices != m_NbVertices)
    {
      std::cerr<<"Error! Attribute file doesn't correspond to input mesh: different number of vertices!"<<std::endl;
      exit(-1);
    }
  Infile.getline ( Line, 40);
  Infile.getline ( Line, 40);
  //read the points in the file and set them as vertices
  for (int i = 0; i < NbVertices; i++ )
    {
      Infile >> ScalarAttribute;
      m_vScalarAttribute.push_back(ScalarAttribute);
    }

  //close file
  Infile.close();
}

void Computation::ComputeAttributeValues()
{
  bool IsNewAttribute;
  int CurrentAttribute;
  for (int i = 0; i < m_NbVertices; i++ )
    {
      CurrentAttribute = m_vScalarAttribute[i];
      IsNewAttribute = true;
      for ( unsigned int j = 0; j < m_vAttributeValues.size(); j++)
	{
	  if (m_vAttributeValues[j] == CurrentAttribute)
	    IsNewAttribute = false;
	}
      if (IsNewAttribute)
	m_vAttributeValues.push_back(CurrentAttribute);
    }
}

void Computation::ComputeParticles()
{
  std::cout<<"Computing particles..."<<std::endl;
  
  int VertexIndex, NbVertices;
  double Distance, MinDistance;
  CovariantVectorType MeanVertex;
  int Attribute;
  
  for (unsigned int AttributeIndex = 0; AttributeIndex < m_vAttributeValues.size(); AttributeIndex++)
    {
      // Initialization
      MinDistance = 1000.0;
      NbVertices = 0;
      VertexIndex = -1;
      MeanVertex[0] = 0.0;
      MeanVertex[1] = 0.0;
      MeanVertex[2] = 0.0;

      Attribute = m_vAttributeValues[AttributeIndex];

      // Computing Mean vertex      
      for (int i = 0; i < m_NbVertices; i++)
	{
	  if (m_vScalarAttribute[i] == Attribute)
	    {
	      MeanVertex += m_vVertices[i];
	      NbVertices++;
	    }
	}
      MeanVertex /= (float)NbVertices;

      // Computing closest particle
      for (int i = 0; i < m_NbVertices; i++)
	{
	  if (m_vScalarAttribute[i] == Attribute)
	    {
	      Distance = sqrt(pow(m_vVertices[i][0]-MeanVertex[0],2)+pow(m_vVertices[i][1]-MeanVertex[1],2)+pow(m_vVertices[i][2]-MeanVertex[2],2));
	      if (Distance < MinDistance)
		{
		  MinDistance = Distance;
		  VertexIndex = i;
		}
	    }
	}
      m_vParticleIndex.push_back(VertexIndex);
    }
}

void Computation::SortAttributes()
{
  for (int i = 0; i < m_NbVertices; i++)
    {
      if (m_vScalarAttribute[i] != 0)
	m_vScalar.push_back(m_vScalarAttribute[i]);
    }
  std::sort(m_vScalar.begin(),m_vScalar.end());
}

void Computation::WriteAttributeFile(std::string _FileName)
{
  std::cout<<"Writing attribute file..."<<std::endl;

  std::ofstream outfile;

  std::vector<int> vParticlesAttribute(m_NbVertices,0);
  for (unsigned int i = 0; i < m_vParticleIndex.size(); i++)
    vParticlesAttribute[m_vParticleIndex[i]] = m_vAttributeValues[i];

  outfile.open(_FileName.c_str());
  outfile<<"NUMBER_OF_POINTS="<<m_NbVertices<<std::endl<<"DIMENSION=1"<<std::endl<<"TYPE=Scalar"<<std::endl;
  for (int i = 0; i < m_NbVertices; i++)
    outfile<<vParticlesAttribute[i]<<std::endl;

  outfile.close();
}

void Computation::WritePointsFile(std::string _FileName)
{
  std::cout<<"Writing points file..."<<std::endl;

  std::ofstream outfile;
  
  outfile.open(_FileName.c_str());
  for (unsigned int i = 0; i < m_vScalar.size(); i++)
    {
      for (int j = 0; j < m_NbVertices; j++)
	{
	  if (m_vScalarAttribute[j] == m_vScalar[i])
	    outfile<<m_vVertices[j][0]<<" "<<m_vVertices[j][1]<<" "<<m_vVertices[j][2]<<std::endl;
	}
    }
  outfile.close();
}

