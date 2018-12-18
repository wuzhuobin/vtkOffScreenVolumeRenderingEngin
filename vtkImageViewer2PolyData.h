/**
 * @file    vtkImageViewer2PolyData.h
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.17.2018
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
/**
 * @class     vtkImageViewer2PolyData
 * @brief     A image viewer with rendering poly data in 2d way functionality. 
 * @author    WUZHUOBIN
 * @date      Dec.18.2018
 * @version   0.0
 * 
 * 
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkImageViewer2PolyData: 
  public vtkImageViewer2, 
  public vtkPolyDataDecorator
{
public: 
  static vtkImageViewer2PolyData * New();
  vtkTypeMacro(vtkImageViewer2PolyData, vtkImageViewer2);
  virtual void PrintSelf(std::ostream &os, vtkIndent indent) VTK_OVERRIDE;
  /**
   * @brief    Update the input display extent and all cutting planes. 
   * 
   */
  virtual void UpdateDisplayExtent() VTK_OVERRIDE;
  /**
   * @copydoc  vtkPolyDataDecorator#AddPolyData 
   * The 2D rendering pipeline is different from 3D. The function also assembles 
   * the 2d rendering pipeline in the following order. 
   * data->cutter->stripper->triangle->pass array->mapper->actor.
   */
  virtual bool AddPolyData(const std::string &tag, const std::string &id) VTK_OVERRIDE;
  virtual bool RemovePolyData(const std::string &id) VTK_OVERRIDE;
protected:
  /**
   * @brief Constructor.
   * 
   */
  vtkImageViewer2PolyData();
  /**
   * @brief Destructor.
   * 
   */
  virtual ~vtkImageViewer2PolyData() VTK_OVERRIDE;
  /**
   * @brief     Install the rendering pipeline. 
   * 
   * Create an overlay renderer which places above the vtkImageViewer2#Renderer. 
   * So there are 2 layer renderers. And set the overlay render as #DecoratorRenderer. 
   */
  virtual void InstallPipeline() VTK_OVERRIDE;
  /**
   * @brief     Uninstall the rendering pipeline. 
   * 
   * Remove the #DecoratorRenderer and roll back to 1 layer renderer.
   */
  virtual void UnInstallPipeline() VTK_OVERRIDE;
  /**
   * @brief     Update the cutting plane in cutter filter. 
   * @param     id,  
   * @return    True, id exists.  
   */
  virtual bool UpdateCutterPlane(const std::string &id);
  /**
   * @brief A map class to save id-plane pair class. Similar to Repository.
   * @see   Repository
   */
  typedef std::unordered_map<std::string, vtkWeakPointer<vtkPlane>> Planes;
  Planes Planes_; ///< A id-plane pair map for retrieving relative a cutting plane.
};
#endif //!__VTK_IMAGE_VIEWER2_POLY_DATA_H__