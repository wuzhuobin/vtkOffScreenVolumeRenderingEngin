/**
 * @file		vtkPolyDataDecorator.h
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
#ifndef __VTK_VIEWER_DECORATOR_H__
#define __VTK_VIEWER_DECORATOR_H__
#pragma once
// std 
#include <string>
#include <unordered_map>
#include <tuple>
// vtk 
#include <vtkSmartPointer.h>
class vtkRenderWindow;
class vtkRenderer;
class vtkPolyData;
class vtkActor;
class vtkPolyDataMapper;
// me 
#include "vtkoffscreenvolumerenderingengin_export.h" 
/**
 * @class     vtkPolyDataDecorator
 * @brief     A decorator class to provide register, retrieval and management poly data and its rendering. 
 * @author    WUZHUOBIN
 * @date      Dec.18.2018
 * @version   0.0
 * 
 * This is a decorator class which is a design pattern that allows behavor to be added 
 * to another class, without affecting the behavior of others objects. 
 * Detail about <a href="https://en.wikipedia.org/wiki/Decorator_pattern">Decorator pattern</a>.
 * However, this class is not fully implemented the decorator functionality, the 
 * reponsibilities cannot be added dynamically at run-time, this class 
 * can only be added in compile-time through multi-inheritance. 
 * 
 * By inherit this class, a viewr class is able to manage poly data and its rendering.
 * By register poly data with specified tags, a viewer class can retrieve it globally,
 * and add it to a rendering pipeline. Functions #Install should be call prior to 
 * rendering to set up a #DecoratorRender. This class also provides functions to 
 * manipulate the translation and rotation of the rendering pipeline. 
 */
class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkPolyDataDecorator
{
public:
  /**
   * @brief Abbreviation of vtkSmartPointer
   * @tparam T vtkObject
   * @see vtkSmartPointer
   * @see vtkObject
   */
  template <typename T>
  using Ptr = vtkSmartPointer<T>;
  /**
   * @brief Tuple class to save the reference of vtkPolyData rendering pipeline. 
   * @see vtkPolyData
   * @see vtkPolyDataMapper
   * @see vtkActor
   */
  typedef std::tuple<Ptr<vtkPolyData>, Ptr<vtkPolyDataMapper>, Ptr<vtkActor>> Tuple;
  /**
   * @brief A map class to save tag-polydata pair.
   * 
   * The class is a key-value pair map class. The  key value is a std::string tag
   * which is used for retrieving poly data. 
   */
  typedef std::unordered_map<std::string, Ptr<vtkPolyData>> InternalStorage;
  /**
   * @brief A map class to save id-tuple pair.
   * @see Tuple
   * 
   * The class is a key-value pair map class. The key value is a std::string id,
   * which is used for retrieving a Tuple.
   */
  typedef std::unordered_map<std::string, Tuple> Repository;
  /**
   * @brief A global static method to register a tag to data.
   * @static
   * @param tag A tag should bu unique.
   * @param data vtkPolyData, the data to be storage globally. 
   * @return If true, the tag has been registerd before and the 
   *         old data was replaced. Otherwise, a new tag was created and 
   *         was registerd with the data.
   * @see AddPolyData
   * 
   * This global static function is used for register an unique tag with a vtkPolyData.
   * This function does not care the tag was registerd before, if it was registered,
   * the old vtkPolyData could be replace with the new one. The registerd tag and data
   * is saved in #InternalStorage_. And data need to be used
   * in #AddPolyData must be registerd with this function.
   */
  static bool AddToInternalStorage(const std::string &tag, vtkPolyData *data);
  /**
   * @brief A global static method to remove a registerd tag and its data. 
   * @static
   * @param tag  A tag need to be removed. 
   * @return If true, the tag was was removed successfully. Otherwise, failed. 
   *         The failure could be caused of the inexistence of tag.
   * @see AddToInternalStorage
   * @see AddPolyData
   * 
   * This global static function is used for unregister tag, which is the 
   * complementary function of #AddToInternalStorage. However the unregister tag
   * could not effect the data was in used(by call #AddPolyData). 
   */
  static bool RemoveFromInternalStorage(const std::string &tag);
  /**
   * @brief      Retrieve and render a poly data with tag and set its id. 
   * @param      tag  A tag to retrieve in #InternalStorage_.
   * @param      id   An id to retrieve a poly data in #Repository_.
   * @return     True, if the given tag is registerd in #InternalStorage_. 
   * @see AddToInternalStorage
   * 
   * Retrieve a poly data with tag and render it in #DecoratorRenderer. The function 
   * install the rendering pipeline, save the reference as a tuple object and save 
   * it to #Repository_ with an id. The function could not check whether the id 
   * was used before. If it was used, the renderering poly data could be replaced by
   * the new one. 
   */
  virtual bool AddPolyData(const std::string &tag, const std::string &id);
  /**
   * @brief      Stop rendering a data with an id. 
   * @param      id  An id to retrieve in #Repository_.
   * @return     True, if data was removed sucessfully. 
   * @see AddPolyData
   * 
   * Stop renderering a data with an id and the rendering pipeline saved in tuple 
   * would alse be removed too. This is complementary function of #AddPolyData. 
   */
  virtual bool RemovePolyData(const std::string &id);
  /**
   * @overload
   * @param         translation 
   * 
   * @copydoc       #TranslatePolyData(const std::string &id, double x, double y, double z)
   */
  virtual bool TranslatePolyData(const std::string &id, const double translation[3]);
  /**
   * @overload
   * @param         translation 
   * 
   * @copydoc       #TranslatePolyData(const std::string &id, double x, double y, double z)
   */
  virtual bool TranslatePolyData(const std::string &id, const std::tuple<double, double, double> &translation);
  /**
   * @brief      Set the position of the specified data.
   * @param id   The unique id of the data.
   * @param x    Translation x.
   * @param y    Translation y.
   * @param z    Translation z. 
   * @return     True, if the id existes, otherwise false.
   * 
   * Set the position of the specified data in #DecoratorRenderer. The poly data is assumed to be
   * at (0, 0, 0), so the translation is relative to (0, 0, 0). 
   * [x y z 1] = [x y z 1] Translate(-origin) Scale(scale) Rot(y) Rot(x) Rot (z) Trans(origin) Trans(position)
   */
  virtual bool TranslatePolyData(const std::string &id, double x, double y, double z);
  /**
   * @overload
   * @param rotation   Rotate in degree.
   * 
   * @copydoc          #RotatePolyData(const std::string &id, double x, double y, double z)
   */
  virtual bool RotatePolyData(const std::string &id, const double rotation[3]);
  /**
   * @overload
   * @param rotation   Rotate in degree.
   * 
   * @copydoc          #RotatePolyData(const std::string &id, double x, double y, double z)
   */
  virtual bool RotatePolyData(const std::string &id, const std::tuple<double, double, double> &rotation);
  /**
   * @brief     Set the orientation of the specified data.
   * @param id  The unique id of the data.
   * @param x   Rotate x-axis, in degree.
   * @param y   Rotate y-axis, in degree.
   * @param z   Rotate z-axis, in degree.
   * @return    True, if the id exists, otherwise false.
   * 
   * Set the orientation of the specified data in #DecoratorRenderer. 
   * [x y z 1] = [x y z 1] Translate(-origin) Scale(scale) Rot(y) Rot(x) Rot (z) Trans(origin) Trans(position)
   * 
   */
  virtual bool RotatePolyData(const std::string &id, double x, double y, double z);
protected:
  /**
   * @brief Set the #DecoratorRenderer and install the pipeline.
   * @param renderer 
   * 
   * Function can add all actors in #Repository_ to #DecoratorRenderer
   */
  void Install(vtkRenderer *renderer);
  /**
   * @brief Uninstall the pipeline.
   * 
   */
  void Uninstall();
  Repository Repository_; ///< Id-tuple pair to retrieve.  
  static InternalStorage InternalStorage_; ///< Static, global storage for tag-data pair.
  vtkRenderer *DecoratorRenderer = nullptr; ///< Internal reference of renderer.
};
#endif //!__VTK_VIEWER_DECORATOR_H__