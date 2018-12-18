/**
 * @file		vtkVolumeViewerPolyData.h
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
#ifndef __VTK_VOLUME_VIEWER_POLY_DATA_H__
#define __VTK_VOLUME_VIEWER_POLY_DATA_H__
#pragma once
//me 
#include "vtkoffscreenvolumerenderingengin_export.h"
#include "vtkVolumeViewer.h"
#include "vtkPolyDataDecorator.h"
/**
 * @class       vtkVolumeViewerPolyData
 * @brief       A volume viewer with rendering external poly data functionality.
 * @author      WUZHUOBIN
 * @date        Dec.18.2018
 * @version     0.0
 * 
 * To satisfy the requirments, volume rendering, poly data rendering and 
 * transforming, the volume viewer class inherits vtkPolyDataDecorator to 
 * handle the reponsibilities. It is a simple setup.  
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkVolumeViewerPolyData :
  public vtkVolumeViewer,
  public vtkPolyDataDecorator
{
public:
  static vtkVolumeViewerPolyData * New();
  vtkTypeMacro(vtkVolumeViewerPolyData, vtkVolumeViewer);
  virtual void PrintSelf(ostream &os, vtkIndent indent) VTK_OVERRIDE;
protected:
  /**
   * @brief Constructor
   * 
   */
  vtkVolumeViewerPolyData();
  /**
   * @brief Destructor.
   * 
   */
  virtual ~vtkVolumeViewerPolyData() VTK_OVERRIDE;
  /**
   * @brief   Install the rendering pipeline.
   * 
   * Invoke vtkVolumeViewer#InstallPipeline, vtkPolyDataDecorator#Install.
   */
  virtual void InstallPipeline() VTK_OVERRIDE;
  /**
   * @brief   Uninstall the rendering pipeline.
   * 
   * Invoke vtkVolumeViewer#UnInstallPipeline, vtkPolyDataDecorator#Uninstall. 
   */
  virtual void UnInstallPipeline() VTK_OVERRIDE;
};
#endif // !__VTK_VOLUME_VIEWER_POLY_DATA_H__