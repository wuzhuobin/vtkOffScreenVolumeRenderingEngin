#ifndef __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
#define __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
#pragma once
// me 
#include "vtkoffscreenvolumerenderingengin_export.h"
class vtkVolumeViewer;
class vtkImageViewer2;
// class vtk_volume_viewer_json_interpreter;
class vtkImageData;
// boost
namespace boost
{
  class mutex;
}
// std 
#include <memory>
#include <string>
#include <vector>
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtk_off_screen_volume_rendering_facade final
{
public: 
  vtk_off_screen_volume_rendering_facade(vtkImageData *imageData, bool _2d = false);
  ~vtk_off_screen_volume_rendering_facade();
  const unsigned char * render_png(const std::string &json, std::vector<unsigned char> &png_data);
  const unsigned char * render_jpeg(const std::string &json, std::vector<unsigned char> &jpeg_data);
private:
  vtk_off_screen_volume_rendering_facade(const vtk_off_screen_volume_rendering_facade&) = delete;
  vtk_off_screen_volume_rendering_facade(vtk_off_screen_volume_rendering_facade&&) = delete;
  void operator=(const vtk_off_screen_volume_rendering_facade&) = delete;
  void operator=(vtk_off_screen_volume_rendering_facade&&) = delete;
  void render_imp();

  vtkVolumeViewer *volumeViewer;
  vtkImageViewer2 *imageViewer2;
};

#endif //!__VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__