#ifndef __VTK_VOLUME_VIEWER_POLY_DATA_H__
#define __VTK_VOLUME_VIEWER_POLY_DATA_H__
#pragma once
//me 
#include "vtkoffscreenvolumerenderingengin_export.h"
#include "vtkVolumeViewer.h"
#include "vtkPolyDataDecorator.h"
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkVolumeViewerPolyData :
  public vtkVolumeViewer,
  public vtkPolyDataDecorator
{
  static vtkVolumeViewerPolyData * New();
  vtkTypeMacro(vtkVolumeViewerPolyData, vtkVolumeViewer);
  virtual void PrintSelf(ostream &os, vtkIndent indent) VTK_OVERRIDE;
};
#endif // !__VTK_VOLUME_VIEWER_POLY_DATA_H__