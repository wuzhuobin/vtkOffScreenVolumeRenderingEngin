// me 
#include "vtkVolumeViewerPolyData.h"
// vtk
#include <vtkObjectFactory.h>
vtkStandardNewMacro(vtkVolumeViewerPolyData);

vtkVolumeViewerPolyData::vtkVolumeViewerPolyData()
{
  this->InstallPipeline();
}

vtkVolumeViewerPolyData::~vtkVolumeViewerPolyData()
{
  
}

void vtkVolumeViewerPolyData::PrintSelf(ostream &os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

void vtkVolumeViewerPolyData::InstallPipeline()
{
  Superclass::InstallPipeline();
  this->Install(this->GetRenderer());
}

void vtkVolumeViewerPolyData::UnInstallPipeline() 
{
  this->Uninstall();
  Superclass::UnInstallPipeline();
}