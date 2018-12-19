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
// boost
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
 * This class is both bridge class and interpreter class. A bridge class is trying to 
 * decouple the detail implementation os subclasses of vtkPolyDataDecorator, 
 * vtkImageViewer2 and vtkVolumeViewer. A interpret class is trying to specific how to 
 * evaluate the json string and intrepret the json for a client. The detail about 
 * <a href="https://en.wikipedia.org/wiki/Bridge_pattern">Bridge pattern</a>. The detail 
 * about <a href="https://en.wikipedia.org/wiki/Interpreter_pattern">Interpreter pattern</a>.
 * 
 * This class uses a wrong name. The class can interpret both vtkImageViewer2 and
 * vtkVolumeViewer. Basing on bridge pattern, the interpreter, viewer and decorator 
 * should be binding in abstraction. However, dual to the requirement, complexity 
 * and time limitation. The vtk_volume_viewer_json_interpreter is a final class 
 * and does not have subclasses. A bridge is binding vtkObject class and the
 * interpreter. With the help of boost::property_tree::ptree, the json can be 
 * interpreted without difficulty. 
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
  /**
   * @brief   Constructor
   * @param   debug True to print out debug info to std::cerr.
   */
  vtk_volume_viewer_json_interpreter(bool debug = false);
  /**
   * @brief Destructor.
   * 
   */
  ~vtk_volume_viewer_json_interpreter();
  /**
   * @brief       Parse the content as json to the #content.
   * @param[in]   content  Content of json which is std::string.
   * 
   */
  void read_json(const std::string &content);
  /**
   * @brief   Parse the content in the file to the #content.
   * @param   filename The path of the file. 
   * 
   */
  void read_json_file(const std::string &filename);
  /**
   * @brief       Transform the #content as json string.
   * @param[out]  content The output json in std::string. 
   * 
   */
  void write_json(std::string &content);
  /**
   * @brief     Transform the #content as json string then write it to a file.
   * @param     filename The path where json string is written.
   */
  void write_json_file(const std::string &filename);
  /**
   * @brief     Function for interpreter and bridge the viewer and decorator. 
   * @param     viewer The object to be interpreted.
   * 
   * This method is a key method for interpreter pattern and bridge. It works as 
   * a bridge and does not care the detail implementation of viewer object since 
   * the behaviour is defined by the detail implemetation of viewer itself. It
   * works as an interpreter and intrepte configurations, operations and
   * interactions in the #content, which was parsed before. It could invoke
   * #interpret(vtkObject* viewer) const, #interpret(vtkImageViewer2 *viewer) const,
   * #interpret(vtkVolumeViwer *viewer) const, etc, depending the type of viewer.
   */
  void interpret(vtkObject *viewer) const;
  /**
   * @brief     Function for interpret vtkImageViewer2 object.
   * @param     viewer The vtkImageViewer2 to be interpreted.
   */
  void interpret(vtkImageViewer2 *viewer) const;
  /**
   * @brief     Function for interpret vtkVolumeViewer object.
   * @param     viewer The vtkVolumeViewer to be interpreted.
   */
  void interpret(vtkVolumeViewer *viewer) const;
  /**
   * @brief   Interact the active camera of renderer. 
   * @param   renderer The renderer to be interacted.  
   * 
   * This method is a wrapper over #dolly, #pan, #spin, #rotate. 
   */
  void interpret(vtkRenderer *renderer) const;
  /**
   * @brief     Function for interpret vtkPolyDataDecorator object.
   * @param     decorator The decorator to be interpreted.
   */
  void interpret(vtkPolyDataDecorator *decorator) const;
private:
  /**
   * @brief   Dolly the active camera of renderer
   * @param   renderer The renderer to be interacted.  
   * 
   * This algorithm is copyed from vtkInteractorStyleTrackballCamera#Rotate.
   */
  void dolly(vtkRenderer *renderer) const;
  /**
   * @brief   Pan the active camera of renderer. 
   * @param   renderer The renderer to be interacted.  
   * 
   * This algorithm is copyed from vtkInteractorStyleTrackballCamera#Dolly.
   */
  void pan(vtkRenderer *renderer) const;
  /**
   * @brief   Spin the active camera of renderer. 
   * @param   renderer The renderer to be interacted.  
   * 
   * This algorithm is copyed from vtkInteractorStyleTrackballCamera#Spin.
   */
  void spin(vtkRenderer *renderer) const;
  /**
   * @brief   Pan the active camera of renderer. 
   * @param   renderer The renderer to be interacted.  
   * 
   * This algorithm is copyed from vtkInteractorStyleTrackballCamera#Rotate.
   */
  void rotate(vtkRenderer *renderer) const;
  /**
   * @brief    Get the value for given json key.
   * @tparam   T The type where the value trying to cast.
   * @param    key Json key.
   * @param    value The value of the json key.
   * @return   True, the casting to type T successfully. Otherwise, the casting 
   *           is failed or the key does not exist.
   * 
   * This method is a convenient method to get the casted value for a json key.
   * If the casting failed and #debug is true, it could print out some debug 
   * message to std::cerr. 
   * <h4>Example</h4>
   * For a given json: 
   * dum:{
   *   foo: 0.5
   * }
   * @code
   * std::string key = "dum.foo";
   * double value;
   * if(this->get_value(key, value))
   * {
   *   std::out << value << '\n';
   * }
   * // 0.5
   * @endcode
   */
  template<typename T>
  bool get_value(const std::string &key, T &value) const;
  /**
   * @brief      Get the array for given json key. 
   * @tparam     T The type where the element of std::array is trying to cast.
   * @tparam     N The length of std::array
   * @param      key Json key.
   * @param      values The array of the json key.
   * @return     true 
   * 
   * This method is a convenient method to get the casted array for a json key.
   * If the casting failed and #debug is true, it could print out some debug 
   * message to std::cerr. 
   * <h4>Example</h4>
   * For a given json:
   * dum:{
   *   foo: [0.5, 0.1, 0.5]
   * }
   * @code
   * std::string key = "dum.foo";
   * std::array<double, 3> values;
   * if(this->get_values(key, values))
   * {
   *   for(double v: values)
   *   {
   *     std::cout << v << ' ';
   *   }
   *   std::out << '\n';
   * }
   * // 0.5 0.1 0.5
   * @endcode
   */
  template<typename T, std::size_t N>
  bool get_values(const std::string &key, std::array<T, N>& values) const;
  vtk_volume_viewer_json_interpreter(const vtk_volume_viewer_json_interpreter &) = delete;
  void operator=(const vtk_volume_viewer_json_interpreter &) = delete;
  vtk_volume_viewer_json_interpreter(vtk_volume_viewer_json_interpreter &&) = delete;
  void operator=(vtk_volume_viewer_json_interpreter &&) = delete;
  boost::property_tree::ptree content; ///< Internal object to parse json to property tree.
  bool debug; ///< Whether to print the debug message.
};
#endif // !__VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__