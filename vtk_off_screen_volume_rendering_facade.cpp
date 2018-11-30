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
#include <vtkJPEGWriter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkRenderWindow.h>
#include <vtkImageViewer2.h>

template<typename T>
using ptr = vtkSmartPointer<T>;

vtk_off_screen_volume_rendering_facade::vtk_off_screen_volume_rendering_facade(vtkImageData *imageData, bool _2d) :
  imageViewer2(nullptr),
  volumeViewer(nullptr)
{
  if(_2d)
  {
    this->imageViewer2 = vtkImageViewer2::New();
    this->imageViewer2->SetOffScreenRendering(true);
    this->imageViewer2->SetInputData(imageData);
    this->imageViewer2->Render();
  }
  else
  {
    this->volumeViewer = vtkVolumeViewer::New();
    this->volumeViewer->SetOffScreenRendering(true);
    this->volumeViewer->SetInputData(imageData);
    this->volumeViewer->Render();
  }
}

vtk_off_screen_volume_rendering_facade::~vtk_off_screen_volume_rendering_facade()
{
  if(this->volumeViewer)
  {
    this->volumeViewer->Delete();
  }
  if(this->imageViewer2)
  {
    this->imageViewer2->Delete();
  }
}

const unsigned char * vtk_off_screen_volume_rendering_facade::render_png(const std::string &json, std::vector<unsigned char> & png_data)
{
  vtk_volume_viewer_json_interpreter interpreter(false);
  interpreter.read_json(json);
  vtkNew<vtkWindowToImageFilter> window_to_image_filter;
  if(this->volumeViewer)
  {
    interpreter.interpret(this->volumeViewer);
    window_to_image_filter->SetInput(this->volumeViewer->GetRenderWindow());
  }
  if(this->imageViewer2)
  {
    interpreter.interpret(this->imageViewer2);
    window_to_image_filter->SetInput(this->imageViewer2->GetRenderWindow());
  }
  this->render_imp();
  vtkNew<vtkPNGWriter> png_writer;
  png_writer->SetInputConnection(window_to_image_filter->GetOutputPort());
  png_writer->SetWriteToMemory(true);
  png_writer->Write();
  vtkUnsignedCharArray *result = png_writer->GetResult();
  png_data.resize(result->GetNumberOfValues());
  memcpy(png_data.data(),result->GetVoidPointer(0), png_data.size() * sizeof(unsigned char));
  return png_data.data();
}

const unsigned char * vtk_off_screen_volume_rendering_facade::render_jpeg(const std::string &json, std::vector<unsigned char> &jpeg_data)
{
  vtk_volume_viewer_json_interpreter interpreter(false);
  interpreter.read_json(json);
  vtkNew<vtkWindowToImageFilter> window_to_image_filter;
  if(this->volumeViewer)
  {
    interpreter.interpret(this->volumeViewer);
    window_to_image_filter->SetInput(this->volumeViewer->GetRenderWindow());
  }
  if(this->imageViewer2)
  {
    interpreter.interpret(this->imageViewer2);
    window_to_image_filter->SetInput(this->imageViewer2->GetRenderWindow());
  }
  this->render_imp();
  vtkNew<vtkJPEGWriter> jpeg_writer;
  jpeg_writer->SetInputConnection(window_to_image_filter->GetOutputPort());
  jpeg_writer->SetWriteToMemory(true);
  jpeg_writer->Write();
  vtkUnsignedCharArray *result = jpeg_writer->GetResult();
  jpeg_data.resize(result->GetNumberOfValues());
  memcpy(jpeg_data.data(),result->GetVoidPointer(0), jpeg_data.size() * sizeof(unsigned char));
  return jpeg_data.data();
}

void vtk_off_screen_volume_rendering_facade::render_imp()
{
  // boost::lock_guard<boost::mutex> lock(*this->mutex);
  if(this->volumeViewer)
  {
    this->volumeViewer->Render();
  }
  if(this->imageViewer2)
  {
    this->imageViewer2->Render();
  }
}