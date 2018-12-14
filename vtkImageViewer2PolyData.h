#ifndef __VTK_IMAGE_VIEWER2_POLY_DATA_H__
#define __VTK_IMAGE_VIEWER2_POLY_DATA_H__
#pragma once
// me 
#include "vtkoffscreenvolumerenderingengin_export.h"
#include "vtkPolyDataDecorator.h"
// vtk
#include <vtkImageViewer2.h>
#include <vtkWeakPointer.h>
class vtkPlane;
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkImageViewer2PolyData: 
  public vtkImageViewer2, 
  public vtkPolyDataDecorator
{
public: 
  static vtkImageViewer2PolyData * New();
  vtkTypeMacro(vtkImageViewer2PolyData, vtkImageViewer2);
  virtual void PrintSelf(std::ostream &os, vtkIndent indent) VTK_OVERRIDE;
  virtual void UpdateDisplayExtent() VTK_OVERRIDE;
  virtual bool AddPolyData(const std::string &tag, const std::string &id) VTK_OVERRIDE;
  virtual bool RemovePolyData(const std::string &id) VTK_OVERRIDE;
protected:
  vtkImageViewer2PolyData();
  virtual ~vtkImageViewer2PolyData() VTK_OVERRIDE;
  virtual void InstallPipeline() VTK_OVERRIDE;
  virtual void UnInstallPipeline() VTK_OVERRIDE;
  virtual bool UpdateCutterPlane(const std::string &id);
  typedef std::unordered_map<std::string, vtkWeakPointer<vtkPlane>> Planes;
  Planes Planes_;
};
#endif //!__VTK_IMAGE_VIEWER2_POLY_DATA_H__