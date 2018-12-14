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

class VTKOFFSCREENVOLUMERENDERINGENGIN_EXPORT vtkPolyDataDecorator
{
public:
  template <typename T>
  using Ptr = vtkSmartPointer<T>;
  typedef std::tuple<Ptr<vtkPolyData>, Ptr<vtkPolyDataMapper>, Ptr<vtkActor>> Tuple;
  typedef std::unordered_map<std::string, Ptr<vtkPolyData>> InternalStorage;
  typedef std::unordered_map<std::string, Tuple> Repository;
  static bool AddToInternalStorage(const std::string &tag, vtkPolyData *data);
  static bool RemoveFromInternalStorage(const std::string &tag);
  virtual bool AddPolyData(const std::string &tag, const std::string &id);
  virtual bool RemovePolyData(const std::string &id);
  virtual bool TranslatePolyData(const std::string &id, const double translation[3]);
  virtual bool TranslatePolyData(const std::string &id, const std::tuple<double, double, double> &translation);
  virtual bool TranslatePolyData(const std::string &id, double x, double y, double z);
  virtual bool RotatePolyData(const std::string &id, const double rotation[3]);
  virtual bool RotatePolyData(const std::string &id, const std::tuple<double, double, double> &rotation);
  virtual bool RotatePolyData(const std::string &id, double x, double y, double z);
  const Repository &GetRepository() const { return this->Repository_; }
protected:
  void Install(vtkRenderer *renderer);
  void Uninstall();
  Repository Repository_;
  static InternalStorage InternalStorage_;
  vtkRenderer *DecoratorRenderer = nullptr;
};
#endif //!__VTK_VIEWER_DECORATOR_H__