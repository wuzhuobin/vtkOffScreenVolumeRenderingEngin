/**
 * @file		vtkVolumeViewerPolyData.cxx
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.18.2018
 * @date    Dec.18.2018
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