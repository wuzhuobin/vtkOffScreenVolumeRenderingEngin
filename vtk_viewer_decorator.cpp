#include <vtk_viewer_decorator.h>
// std
#include <iostream>
// vtk
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
vtk_viewer_decorator::InternalStorage vtk_viewer_decorator::internalStorage;
bool vtk_viewer_decorator::addPolyData(const std::string &tag, vtkPolyData *data)
{
  auto polyData = Ptr<vtkPolyData>::New();
  polyData->ShallowCopy(data);
  return !internalStorage.emplace(tag, polyData).second;
}

bool vtk_viewer_decorator::removePolyData(const std::string &tag)
{
  auto cit = internalStorage.find(tag);
  if(cit == internalStorage.cend())
  {
    return false;
  }
  internalStorage.erase(cit);
  return true;
}

bool vtk_viewer_decorator::addToRenderer(const std::string &tag, const std::string &id)
{
  auto cit = internalStorage.find(tag);
  if(cit == internalStorage.cend())
  {
    std::cerr << "tag: " << tag << " does not exist. \n";
    return false;
  }
  auto cit2 = this->repository.find(id);
  if(cit2 == this->repository.cend())
  {
    Tuple tuple(nullptr, Ptr<vtkPolyDataMapper>::New(), Ptr<vtkActor>::New());
    this->renderer->AddActor(std::get<2>(tuple));
    std::get<2>(tuple)->SetMapper(std::get<1>(tuple));
    cit2 = this->repository.emplace(id, tuple).first;
  }
  std::get<0>(cit2->second) = cit->second;
  std::get<1>(cit2->second)->SetInputData(cit->second);
  return true;
}

bool vtk_viewer_decorator::removeFromRenderer(const std::string &id)
{
  auto cit = this->repository.find(id);
  if(cit == this->repository.cend())
  {
    std::cerr << "id: " << id << " does not exist. \n";
    return false;
  }
  this->renderer->RemoveActor(std::get<2>(cit->second));
  this->repository.erase(cit);
  return true;
}

void vtk_viewer_decorator::install(vtkRenderer * renderer, vtkRenderWindow * renderWindow)
{
  this->renderer = renderer;
  this->renderWindow = renderWindow;
}

void vtk_viewer_decorator::uninstall()
{
  this->renderer = nullptr;
  this->renderWindow = nullptr;
}