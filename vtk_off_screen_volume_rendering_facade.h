/**
 * @file    vtk_off_screen_volume_rendering_facade.h
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
 * @brief      The simplest interface of off-screen rendering. 
 * @author     WUZHUOBIN
 * @date       Dec.18.2018
 * @version    0.0
 * 
 * This class is a facade class which is a software-design pattern. A facade in an object that
 * serves as a front-facing interface masking complex underlying or structure, e.g. 
 * vtk_volume_viewer_json_interpreter, vtkImageViewer2PolyData. The detail of facade can be 
 * check <a href="https://en.wikipedia.org/wiki/Facade_pattern">Facade patter</a>. 
 * So this class is trying masking others class in this library and provides extermely simple
 * facade interface.
 * 
 * This class is to ease the off-screen rendering operations. The requirement of off-screen 
 * rendering is to convert the vtkRenderWindow to am image. And operations and interactions to
 * the render window should be revealed in an image as well. So the aim of this class is 
 * basing on the content of a json which contains some configurations or interactions to a vewer
 * (in 2D or 3D). The rendering result could be saved to an array in memory. The content of json 
 * and the interpreting process is desginged to be close to RESTful api. It is supposed to be
 * invalent to the state before, every json can represent the whole configuration and
 * interaction. 
 *  
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtk_off_screen_volume_rendering_facade final
{
public: 
  /**
   * @brief Constructor.
   * @param imageData The image data to be rendered.
   * @param _2d  true, render the image and other data in 2D, otherwise in 3D. 
   * 
   * Construct the facade object. 
   */
  vtk_off_screen_volume_rendering_facade(vtkImageData *imageData, bool _2d = false);
  /**
   * @brief Destructor.
   * 
   */
  ~vtk_off_screen_volume_rendering_facade();
  /**
   * @brief         Off-screen rendering in png format as the json requested;
   * @param[in]     json The request json to interact. 
   * @param[out]    png_data The data buffer in std::vector<unsigned char> of rener window.
   * @return        The data buffer in const unsigned char* array of render window.
   * 
   * The viewer was configured and interact as the json requested, and the result of 
   * the rendering of the viewer could be write to user provided png_data, which is a
   * std::vector<unsigned char> type, in png format. 
   */
  const unsigned char * render_png(const std::string &json, std::vector<unsigned char> &png_data);
  /**
   * @brief         Off-screen rendering in jpeg format as the json requested;
   * @param[in]     json The request json to interact. 
   * @param[out]    jpeg_data The data buffer in std::vector<unsigned char> of rener window.
   * @return        The data buffer in const unsigned char* array of render window.
   * 
   * The viewer was configured and interact as the json requested, and the result of 
   * the rendering of the viewer could be write to user provided jpeg_data, which is a
   * std::vector<unsigned char> type, in jpeg format. 
   */
  const unsigned char * render_jpeg(const std::string &json, std::vector<unsigned char> &jpeg_data);
private:
  vtk_off_screen_volume_rendering_facade(const vtk_off_screen_volume_rendering_facade&) = delete;
  vtk_off_screen_volume_rendering_facade(vtk_off_screen_volume_rendering_facade&&) = delete;
  void operator=(const vtk_off_screen_volume_rendering_facade&) = delete;
  void operator=(vtk_off_screen_volume_rendering_facade&&) = delete;
  /**
   * @brief Render the viewer.
   * 
   */
  void render_imp();
  vtkObject *viewer; ///< The internal viewer object for off-screen rendering. 
};

#endif //!__VTK_OFF_SCREEN_VOLUME_RENDERING_FACADE_H__