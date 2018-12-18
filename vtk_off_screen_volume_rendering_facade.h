/**
 * @file    vtk_off_screen_volume_rendering_facade.h
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
#ifndef __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
#define __VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__
#pragma once
// me 
#include "vtkoffscreenvolumerenderingengin_export.h"
class vtkImageData;
class vtkObject;
// std 
#include <memory>
#include <string>
#include <vector>
/**
 * @class      vtk_off_screen_volume_rendering_facade
 * @brief       
 * @authro     WUZHUOBIN
 * @date       Dec.18.2018
 * @version    0.0
 */
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
  vtkObject *viewer;
};

#endif //!__VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__