#include "itkImage.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"
#include "itkImageFileWriter.h"
 
typedef itk::Image<unsigned char, 2>       ImageType;
typedef ImageType::PixelType     PixelType;
 
static void CreateImage(ImageType::Pointer image);
 
int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
 
  typedef itk::BinaryBallStructuringElement<PixelType, 2>
    StructuringElementType;
  StructuringElementType::RadiusType elementRadius;
  elementRadius[0]=10;
  elementRadius[1]=5;
 
  StructuringElementType structuringElement;
  structuringElement.SetRadius(elementRadius);
  structuringElement.CreateStructuringElement();
 
  typedef itk::ShapedNeighborhoodIterator<ImageType> IteratorType;
  IteratorType siterator(structuringElement.GetRadius(),
                         image,
                         image->GetLargestPossibleRegion());
 
  siterator.CreateActiveListFromNeighborhood(structuringElement);
  siterator.NeedToUseBoundaryConditionOff();
 
  IteratorType::IndexType location;
  location[0] = 20;
  location[1] = 20;
  siterator.SetLocation(location);
  IteratorType::Iterator i;
  for (i = siterator.Begin(); !i.IsAtEnd(); ++i)
  {
  i.Set(255);
  }

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
 
  writer->SetFileName( argv[1] );
  writer->SetInput( image );
 
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
 
 /*
  // Now show the results
  typedef itk::ImageRegionConstIterator<ImageType> ImageIteratorType;
  ImageIteratorType imit(image, image->GetLargestPossibleRegion());
  imit.GoToBegin();
  unsigned int col = 0;
  while( !imit.IsAtEnd() )
    {
    PixelType value = imit.Get();
    ++imit;
    ++col;
    std::cout << value << " ";
    if ((col % 10) == 0)
      {
      std::cout << std::endl;
      }
    }
 */
  return EXIT_SUCCESS;
}
 
void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(100);
 
  ImageType::RegionType region(start,size);
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
}