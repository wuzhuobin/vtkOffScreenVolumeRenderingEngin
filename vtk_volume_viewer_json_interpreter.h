/**
 * @file        vtk_volume_viewer_json_interpreter.h
 * @author      WUZHUOBIN jiejin2022@163.com
 * @since       Dec.18.2018
 * @date        Dec.18.2018
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
#ifndef __VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__
#define __VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__
#pragma once
// me 
#include "vtkoffscreenvolumerenderingengin_export.h"
class vtkObject;
class vtkVolumeViewer;
class vtkImageViewer2;
class vtkRenderer;
class vtkPolyDataDecorator;
//boos
#include <boost/property_tree/json_parser.hpp>
// std
#include <string>
#include <iostream>
#include <array>
/**
 * @class       vtk_volume_viewer_json_interpreter
 * @brief       Interpretering json to control the viewer. 
 * @author      WUZHUOBIN
 * @date        Dec.18.2018
 * @version     0.0
 * 
 * 
 * <h4>Example</h4>
 * @code
 * std::string json = ...;
 * vtk_volume_viewer_json_interpreter interpreter(true); // Print out debug info.
 * interpreter.read_json(json);
 * interpreter.interpret(viewer);
 * viewer->Render();
 * @endcode
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtk_volume_viewer_json_interpreter final
{
public:
  vtk_volume_viewer_json_interpreter(bool debug = false);
  ~vtk_volume_viewer_json_interpreter();
  void read_json(const std::string &content);
  void read_json_file(const std::string &filename);
  void write_json(std::string &content);
  void write_json_file(const std::string &filename);
  /**
   * @brief Function for interpreter and bridge. 
   * 
   */
  void interpret(vtkObject *viewer) const;
  void interpret(vtkImageViewer2 *viewer) const;
  void interpret(vtkVolumeViewer *viewer) const;
  void interpret(vtkRenderer *renderer) const;
  void interpret(vtkPolyDataDecorator *decorator) const;
private:
  void dolly(vtkRenderer *renderer) const;
  void pan(vtkRenderer *renderer) const;
  void spin(vtkRenderer *renderer) const;
  void rotate(vtkRenderer *renderer) const;
  template<typename T>
  bool get_value(const std::string &key, T &value) const;
  template<typename T, std::size_t N>
  bool get_values(const std::string &key, std::array<T, N>& values) const;
  vtk_volume_viewer_json_interpreter(const vtk_volume_viewer_json_interpreter &) = delete;
  void operator=(const vtk_volume_viewer_json_interpreter &) = delete;
  vtk_volume_viewer_json_interpreter(vtk_volume_viewer_json_interpreter &&) = delete;
  void operator=(vtk_volume_viewer_json_interpreter &&) = delete;
  boost::property_tree::ptree content;
  bool debug;
};
#endif // !__VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__