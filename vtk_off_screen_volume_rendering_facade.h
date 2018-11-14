#ifndef __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
#define __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
// me 
class vtkVolumeViewer;
// class vtk_volume_viewer_json_interpreter;
class vtkImageData;
class vtk_off_screen_volume_rendering_facade final
{
public: 
  vtk_off_screen_volume_rendering_facade(vtkImageData *imageData);
  ~vtk_off_screen_volume_rendering_facade();
private:
  vtkVolumeViewer *viewer;
  vtk_off_screen_volume_rendering_facade(const vtk_off_screen_volume_rendering_facade&) = delete;
  vtk_off_screen_volume_rendering_facade(vtk_off_screen_volume_rendering_facade&&) = delete;
  void operator=(const vtk_off_screen_volume_rendering_facade&) = delete;
  void operator=(vtk_off_screen_volume_rendering_facade&&) = delete;
};

#endif //!__VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__