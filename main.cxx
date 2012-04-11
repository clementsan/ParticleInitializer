#include <iostream>

#include "argio.h"
#include "Computation.h"

int main (int argc, const char * argv[])
{
  if ( argc < 5 || ipExistsArgument(argv, "-usage") || ipExistsArgument(argv, "-help") )
    {
      std::cout <<"Usage:" << std::endl ;
      std::cout << argv[0] << " <InputMesh> <OutputFilename> [options]"<<std::endl;
      std::cout << "     -meanVertex <AttributeFile>"<<std::endl;
      std::cout << "     -savePoints <AttributeFile>"<<std::endl;
      
      return EXIT_SUCCESS;
    }

  bool meanVertexOn = ipExistsArgument(argv, "-meanVertex");
  bool savePointsOn = ipExistsArgument(argv, "-savePoints");
  
  Computation m_Computation;
  m_Computation.ReadMesh(argv[1]);
  if (meanVertexOn)
    {
      m_Computation.ReadAttributeFile(ipGetStringArgument(argv, "-meanVertex", NULL));
      m_Computation.ComputeAttributeValues();
      m_Computation.ComputeParticles();
      m_Computation.WriteAttributeFile(argv[2]);
    }
  else if (savePointsOn)
    {
      m_Computation.ReadAttributeFile(ipGetStringArgument(argv, "-savePoints", NULL));
      m_Computation.SortAttributes();
      m_Computation.WritePointsFile(argv[2]);
    }
  return EXIT_SUCCESS;
}
