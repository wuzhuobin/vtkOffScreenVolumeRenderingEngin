// me 
#include "vtk_off_screen_volume_rendering_facade.h"
#include "vtkVolumeViewer.h"
#include "vtk_volume_viewer_json_interpreter.h"
// vtk 
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkUnsignedCharArray.h>
template<typename T>
using ptr = vtkSmartPointer<T>;
vtk_off_screen_volume_rendering_facade::vtk_off_screen_volume_rendering_facade(vtkImageData *imageData) :
  viewer(vtkVolumeViewer::New())
{
  this->viewer->SetOffScreenRendering(true);
  this->viewer->SetInputData(imageData);
}

vtk_off_screen_volume_rendering_facade::~vtk_off_screen_volume_rendering_facade()
{
  this->viewer->Delete();
}

vtk_off_screen_volume_rendering_facade::render(const std::string &json, std::vector<char> png_data)
{
  vtkNew<vtkWindowToImageFilter> windowToImageFilter;
  windowToImageFilter->SetInput(this->viewer->GetRenderWindow());
  vtkNew<vtkPNGWriter> pngWriter;
  pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
}