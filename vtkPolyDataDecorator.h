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

class vtkPolyDataDecorator
{
public:
  template <typename T>
  using Ptr = vtkSmartPointer<T>;
  typedef std::tuple<Ptr<vtkPolyData>, Ptr<vtkPolyDataMapper>, Ptr<vtkActor>> Tuple;
  typedef std::unordered_map<std::string, Ptr<vtkPolyData>> InternalStorage;
  typedef std::unordered_map<std::string, Tuple> Repository;
  /**
   * @brief Adding poly data to repository.
   * 
   * @param[in]  id 
   * @param[in]  data 
   * @return     true, . false,  
   */
  static bool addPolyData(const std::string &tag, vtkPolyData *data);
  /**
   * @brief   Removing poly data from repository.
   * 
   * @param id 
   * @return true 
   * @return false 
   */
  static bool removePolyData(const std::string &tag);
  bool addToRenderer(const std::string &tag, const std::string &id);
  bool removeFromRenderer(const std::string &id);
  const Repository &getRepository() const { return this->repository; }
protected:
  virtual void install(vtkRenderer *renderer, vtkRenderWindow *renderWindow);
  virtual void uninstall();
  Repository repository;
  static InternalStorage internalStorage;
  vtkRenderer *renderer = nullptr;
  vtkRenderWindow *renderWindow = nullptr;
};
#endif //!__VTK_VIEWER_DECORATOR_H__