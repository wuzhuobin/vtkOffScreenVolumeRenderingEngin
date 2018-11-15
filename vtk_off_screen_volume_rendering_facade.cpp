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
// boost 
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

template<typename T>
using ptr = vtkSmartPointer<T>;
vtk_off_screen_volume_rendering_facade::vtk_off_screen_volume_rendering_facade(vtkImageData *imageData) :
  viewer(vtkVolumeViewer::New()),
  mutex(new boost::mutex)
{
  this->viewer->SetOffScreenRendering(true);
  this->viewer->SetInputData(imageData);
  this->viewer->SetPreset(vtkVolumeViewer::CT_AAA);
  this->viewer->Render();
}

vtk_off_screen_volume_rendering_facade::~vtk_off_screen_volume_rendering_facade()
{
  this->viewer->Delete();
}

const char * vtk_off_screen_volume_rendering_facade::render_png(const std::string &json, std::vector<char> & png_data)
{
  vtk_volume_viewer_json_interpreter interpreter;
  interpreter.read_json(json);
  interpreter.interpret(this->viewer);
  this->render_imp();
  vtkNew<vtkWindowToImageFilter> window_to_image_filter;
  window_to_image_filter->SetInput(this->viewer->GetRenderWindow());
  vtkNew<vtkPNGWriter> png_writer;
  png_writer->SetInputConnection(window_to_image_filter->GetOutputPort());
  png_writer->SetWriteToMemory(true);
  vtkUnsignedCharArray *result = png_writer->GetResult();
  png_data.resize(result->GetNumberOfValues());
  memcpy(png_data.data(),result->GetVoidPointer(0), png_data.size() * sizeof(char));
  return png_data.data();
}

const char * vtk_off_screen_volume_rendering_facade::render_jpeg(const std::string &json, std::vector<char> &jpeg_data)
{
  vtk_volume_viewer_json_interpreter interpreter;
  interpreter.read_json(json);
  interpreter.interpret(this->viewer);
  this->render_imp();
  vtkNew<vtkWindowToImageFilter> window_to_image_filter;
  window_to_image_filter->SetInput(this->viewer->GetRenderWindow());
  vtkNew<vtkJPEGWriter> jpeg_writer;
  jpeg_writer->SetInputConnection(window_to_image_filter->GetOutputPort());
  jpeg_writer->SetWriteToMemory(true);
  vtkUnsignedCharArray *result = jpeg_writer->GetResult();
  jpeg_data.resize(result->GetNumberOfValues());
  memcpy(jpeg_data.data(),result->GetVoidPointer(0), jpeg_data.size() * sizeof(char));
  return jpeg_data.data();
}

void vtk_off_screen_volume_rendering_facade::render_imp()
{
  boost::lock_guard<boost::mutex> lock(*this->mutex);
  this->viewer->Render();
}