/**
 * @file    vtk_off_screen_volume_rendering_facade.cpp
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.17.2018
 * @date    Dec.17.2018
 * @copyright
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  &nbsp;  This program is distributed in the hope that it will be useful, but	<br>
  &nbsp;  WITHOUT ANY WARRANTY; without even the implied warranty of <br>
  &nbsp;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. <br>
  &nbsp;  See the LICENSE for more detail. <br> 
  &nbsp;  Copyright (c) WUZHUOBIN. All rights reserved. <br>
  &nbsp;  See COPYRIGHT for more detail. <br>
  &nbsp;  This software is distributed WITHOUT ANY WARRANTY; without even <br>
  &nbsp;  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR <br>
  &nbsp;  PURPOSE.  See the above copyright notice for more information. <br>
  &nbsp;  Internal usage only, without the permission of the author, please DO <br>
  &nbsp;  NOT publish and distribute without the author's permission. <br>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// me 
#include "vtk_off_screen_volume_rendering_facade.h"
#include "vtkVolumeViewer.h"
#include "vtkVolumeViewerPolyData.h"
#include "vtkImageViewer2PolyData.h"
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

vtk_off_screen_volume_rendering_facade::vtk_off_screen_volume_rendering_facade(vtkImageData *imageData, bool _2d) :
  viewer(nullptr)
{
  if(_2d)
  {
    vtkImageViewer2 *imageViewer2 = vtkImageViewer2PolyData::New();
    imageViewer2->SetOffScreenRendering(true);
    imageViewer2->SetInputData(imageData);
    imageViewer2->Render();
    this->viewer = imageViewer2;
  }
  else
  {
    vtkVolumeViewer *volumeViewer = vtkVolumeViewerPolyData::New();
    volumeViewer->SetOffScreenRendering(true);
    volumeViewer->SetInputData(imageData);
    volumeViewer->Render();
    this->viewer = volumeViewer;
  }
}

vtk_off_screen_volume_rendering_facade::~vtk_off_screen_volume_rendering_facade()
{
  this->viewer->Delete();
}

const unsigned char * vtk_off_screen_volume_rendering_facade::render_png(const std::string &json, std::vector<unsigned char> & png_data)
{
  vtk_volume_viewer_json_interpreter interpreter(false);
  interpreter.read_json(json);
  vtkNew<vtkWindowToImageFilter> window_to_image_filter;
  vtkVolumeViewer *volumeViewer = vtkVolumeViewer::SafeDownCast(this->viewer);
  vtkImageViewer2 *imageViewer2 = vtkImageViewer2::SafeDownCast(this->viewer);
  vtkPolyDataDecorator *decorator = dynamic_cast<vtkPolyDataDecorator*>(this->viewer);
  if(volumeViewer)
  {
    interpreter.interpret(volumeViewer);
    window_to_image_filter->SetInput(volumeViewer->GetRenderWindow());
  }
  if(imageViewer2)
  {
    interpreter.interpret(imageViewer2);
    window_to_image_filter->SetInput(imageViewer2->GetRenderWindow());
  }
  if(decorator)
  {
    interpreter.interpret(decorator);
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
  vtkVolumeViewer *volumeViewer = vtkVolumeViewer::SafeDownCast(this->viewer);
  vtkImageViewer2 *imageViewer2 = vtkImageViewer2::SafeDownCast(this->viewer);
  if(volumeViewer)
  {
    interpreter.interpret(volumeViewer);
    window_to_image_filter->SetInput(volumeViewer->GetRenderWindow());
  }
  if(imageViewer2)
  {
    interpreter.interpret(imageViewer2);
    window_to_image_filter->SetInput(imageViewer2->GetRenderWindow());
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
  vtkVolumeViewer *volumeViewer = vtkVolumeViewer::SafeDownCast(this->viewer);
  vtkImageViewer2 *imageViewer2 = vtkImageViewer2::SafeDownCast(this->viewer);
  if(volumeViewer)
  {
    volumeViewer->Render();
  }
  if(imageViewer2)
  {
    imageViewer2->Render();
  }
}