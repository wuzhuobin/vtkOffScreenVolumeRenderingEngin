#include <vtkPolyDataDecorator.h>
// std
#include <iostream>
// vtk
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
vtkPolyDataDecorator::InternalStorage vtkPolyDataDecorator::InternalStorage_;
bool vtkPolyDataDecorator::AddToInternalStorage(const std::string &tag, vtkPolyData *data)
{
  auto polyData = Ptr<vtkPolyData>::New();
  polyData->ShallowCopy(data);
  return !InternalStorage_.emplace(tag, polyData).second;
}

bool vtkPolyDataDecorator::RemoveFromInternalStorage(const std::string &tag)
{
  auto cit = InternalStorage_.find(tag);
  if(cit == InternalStorage_.cend())
  {
    return false;
  }
  InternalStorage_.erase(cit);
  return true;
}

bool vtkPolyDataDecorator::AddPolyData(const std::string &tag, const std::string &id)
{
  auto cit = InternalStorage_.find(tag);
  if(cit == InternalStorage_.cend())
  {
    return false;
  }
  auto cit2 = this->Repository_.find(id);
  if(cit2 == this->Repository_.cend())
  {
    Tuple tuple(nullptr, Ptr<vtkPolyDataMapper>::New(), Ptr<vtkActor>::New());
    this->DecoratorRenderer->AddActor(std::get<2>(tuple));
    std::get<2>(tuple)->SetMapper(std::get<1>(tuple));
    cit2 = this->Repository_.emplace(id, tuple).first;
  }
  std::get<0>(cit2->second) = cit->second;
  std::get<1>(cit2->second)->SetInputData(cit->second);
  return true;
}

bool vtkPolyDataDecorator::RemovePolyData(const std::string &id)
{
  auto cit = this->Repository_.find(id);
  if(cit == this->Repository_.cend())
  {
    return false;
  }
  this->DecoratorRenderer->RemoveActor(std::get<2>(cit->second));
  this->Repository_.erase(cit);
  return true;
}

bool vtkPolyDataDecorator::TranslatePolyData(const std::string &id, const double translation[3])
{
  return this->TranslatePolyData(id, translation[0], translation[1], translation[2]);
}

bool vtkPolyDataDecorator::TranslatePolyData(const std::string &id, const std::tuple<double, double, double> &translation)
{
  return this->TranslatePolyData(id, std::get<0>(translation), std::get<1>(translation), std::get<2>(translation));
}

bool vtkPolyDataDecorator::TranslatePolyData(const std::string &id, double x, double y, double z)
{
  auto cit = this->Repository_.find(id);
  if(cit == this->Repository_.cend())
  {
    std::cerr << "id: " << id << " does not exist. \n";
    return false;
  }
  std::get<2>(cit->second)->SetPosition(x, y, z);
  return true;
}

bool vtkPolyDataDecorator::RotatePolyData(const std::string &id, const double rotation[3])
{
  return this->RotatePolyData(id, rotation[0], rotation[1], rotation[2]);
}

bool vtkPolyDataDecorator::RotatePolyData(const std::string &id, const std::tuple<double, double, double> &rotation)
{
  return this->RotatePolyData(id, std::get<0>(rotation), std::get<1>(rotation), std::get<2>(rotation));
}

bool vtkPolyDataDecorator::RotatePolyData(const std::string &id, double x, double y, double z)
{
  auto cit = this->Repository_.find(id);
  if(cit == this->Repository_.cend())
  {
    std::cerr << "id: " << id << " does not exist. \n";
    return false;
  }
  vtkActor *actor = std::get<2>(cit->second);
  actor->SetOrigin(actor->GetPosition());
  actor->SetOrientation(x, y, z);
  return true;
}

void vtkPolyDataDecorator::Install(vtkRenderer * renderer)
{
  this->DecoratorRenderer = renderer;
  for(auto cit = this->Repository_.cbegin(); cit != this->Repository_.cend(); ++cit)
  {
    vtkActor *actor = std::get<2>(cit->second);
    this->DecoratorRenderer->AddActor(actor);
  }
}

void vtkPolyDataDecorator::Uninstall()
{
  for(auto cit = this->Repository_.cbegin(); cit != this->Repository_.cend(); ++cit)
  {
    vtkActor *actor = std::get<2>(cit->second);
    this->DecoratorRenderer->RemoveActor(actor);
  }
  this->DecoratorRenderer = nullptr;
}