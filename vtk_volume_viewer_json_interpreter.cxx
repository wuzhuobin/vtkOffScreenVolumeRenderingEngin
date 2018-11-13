// me 
#include "vtk_volume_viewer_json_interpreter.h"
#include "vtkVolumeViewer.h"
// vtk
#include <vtkRenderer.h>
#include <vtkCamera.h>
// boost
using namespace boost::property_tree;
// std
#include <iostream>
#include <sstream>
using namespace std;
vtk_volume_viewer_json_interpreter::vtk_volume_viewer_json_interpreter(bool debug) :
  debug(debug)
{

}

vtk_volume_viewer_json_interpreter::~vtk_volume_viewer_json_interpreter()
{
}

void vtk_volume_viewer_json_interpreter::read_json(const std::string &content)
{
  try
  {
    stringstream ss;
    ss << content;
    boost::property_tree::read_json(ss, this->content);
  }
  catch (json_parser::json_parser_error &e)
  {
    cerr << e.what() << '\n';
    cerr << "Cannot parse this json.\n";
  }
}

void vtk_volume_viewer_json_interpreter::read_json_file(const std::string &filename)
{
  try
  {
    boost::property_tree::read_json(filename, this->content);
  }
  catch(json_parser::json_parser_error &e)
  {
    cerr << e.what() << '\n';
    cerr << "Cannot parse this json.\n";
  }
}

void vtk_volume_viewer_json_interpreter::write_json(std::string &content)
{
  stringstream ss;
  boost::property_tree::write_json(ss, this->content);
  content = ss.str();
}

void vtk_volume_viewer_json_interpreter::write_json_file(const std::string &filename)
{
  boost::property_tree::write_json(filename, this->content);
}

void vtk_volume_viewer_json_interpreter::interpret(vtkVolumeViewer *viewer) const 
{
  if(viewer == nullptr)
  {
      cerr << "viewer is nullptr\n" ;
      return;
  }
  int preset;
  if(this->get_value("preset", preset))
  {
    viewer->SetPreset(preset);
  }
  double shift;
  if(this->get_value("shift", shift))
  {
    viewer->SetShift(shift);
  }
  double opacity;
  if(this->get_value("opacity", opacity))
  {
      viewer->SetOpacity(opacity);
  }
  array<int, 2> size;
  if(this->get_values("size", size)){
    viewer->SetSize(reinterpret_cast<int*>(&size));
  }
  this->interpret(viewer->GetRenderer());

}

void vtk_volume_viewer_json_interpreter::interpret(vtkRenderer *renderer) const 
{
  if(renderer == nullptr)
  {
    cerr << "renderer is nullptr. \n";
    return;
  }
  this->dolly(renderer);
  this->pan(renderer);
  this->spin(renderer);
  this->rotate(renderer);
}

void vtk_volume_viewer_json_interpreter::dolly(vtkRenderer *renderer) const
{
  array<double, 2> current;
  array<double, 2> last;
  double motion_factor = 1;
  this->get_value("dolly.motionFactor", motion_factor);
  if(!this->get_values("dolly.current", current))
  {
    return;
  }
  if(!this->get_values("dolly.last", last))
  {
    return;
  }
  // copy from vtkInteractorStyleTrackballCamera#Dolly. 
  double *center = renderer->GetCenter();
  int dy = current[1] - last[1];
  double dyf = motion_factor * dy / center[1];
  double factor = pow(1.1, dyf);
  vtkCamera *camera = renderer->GetActiveCamera();
  if (camera->GetParallelProjection())
  {
    camera->SetParallelScale(camera->GetParallelScale() / factor);
  }
  else
  {
    camera->Dolly(factor);
    renderer->ResetCameraClippingRange();
  }
  renderer->UpdateLightsGeometryToFollowCamera();
}

void vtk_volume_viewer_json_interpreter::pan(vtkRenderer *renderer) const
{
  array<double, 2> current;
  array<double, 2> last;
  if (!this->get_values<double, 2>("pan.current", current))
  {
    return;
  }
  if (!this->get_values<double, 2>("pan.last", last))
  {
    return;
  }
  // Calculate the focal depth since we'll be using it a lot
  vtkCamera *camera = renderer->GetActiveCamera();
  double view_focus[4]{1,1,1,1};
  camera->GetFocalPoint(view_focus);
  renderer->SetWorldPoint(view_focus);
  renderer->WorldToDisplay();
  renderer->GetDisplayPoint(view_focus);

  double last_world[4]{1,1,1,1};
  renderer->SetDisplayPoint(last[0], last[1], view_focus[2]);
  renderer->DisplayToWorld();
  renderer->GetWorldPoint(last_world);
  double current_world[4]{1,1,1,1};
  renderer->SetDisplayPoint(current[0], current[1], view_focus[2]);
  renderer->DisplayToWorld();
  renderer->GetWorldPoint(current_world);
  double motion_vector[3];
  vtkMath::Subtract(last_world, current_world, motion_vector);
  double view_point[3];
  camera->GetFocalPoint(view_focus);
  camera->GetPosition(view_point);
  vtkMath::Add(view_focus, motion_vector, view_focus);
  vtkMath::Add(view_point, motion_vector, view_point);
  camera->SetFocalPoint(view_focus);
  camera->SetPosition(view_point);
  renderer->UpdateLightsGeometryToFollowCamera();
}

void vtk_volume_viewer_json_interpreter::spin(vtkRenderer *renderer) const
{
  array<double, 2> current;
  array<double, 2> last;
  if (!this->get_values<double, 2>("spin.current", current))
  {
    return;
  }
  if (!this->get_values<double, 2>("spin.last", last))
  {
    return;
  }

  double *center = renderer->GetCenter();

  double new_angle =
    vtkMath::DegreesFromRadians( atan2( current[1] - center[1],
                                        current[0] - center[0] ) );

  double old_angle =
    vtkMath::DegreesFromRadians( atan2( last[1] - center[1],
                                        last[0] - center[0] ) );

  vtkCamera *camera = renderer->GetActiveCamera();
  camera->Roll( new_angle - old_angle );
  camera->OrthogonalizeViewUp();
}

void vtk_volume_viewer_json_interpreter::rotate(vtkRenderer *renderer) const
{
  array<double, 2> current;
  array<double, 2> last;
  double motion_factor = 1;
  this->get_value("rotate.motionFactor", motion_factor);
  if (!this->get_values<double, 2>("rotate.current", current))
  {
    return;
  }
  if (!this->get_values<double, 2>("rotate.last", last))
  {
    return;
  }
  int dx = current[0] - last[0];
  int dy = current[1] - last[1];
  int *size = renderer->GetSize();

  double delta_elevation = -20.0 / size[1];
  double delta_azimuth = -20.0 / size[0];

  double rxf = dx * delta_azimuth * motion_factor;
  double ryf = dy * delta_elevation * motion_factor;

  vtkCamera *camera = renderer->GetActiveCamera();
  camera->Azimuth(rxf);
  camera->Elevation(ryf);
  camera->OrthogonalizeViewUp();

  renderer->ResetCameraClippingRange();
  renderer->UpdateLightsGeometryToFollowCamera();
}

template<typename T>
bool vtk_volume_viewer_json_interpreter::get_value(const std::string &key, T &value) const
{
  try
  {
    const ptree &child = this->content.get_child(key);
    T attribute = child.get_value<T>();
    value = attribute;
  }
  catch(ptree_bad_path &e)
  {
    if(debug)
    {
      cerr << e.what() << '\n';
      cerr << "The json file does not contain this child: " << key << '\n';
    }
    return false;
  }
  catch(ptree_bad_data &e)
  {
    if(debug)
    {
      cerr << e.what() << '\n';
      cerr << "The key does not contain value. \n";
    }
    return false;
  }
  return true;
}

template<typename T, size_t N>
bool vtk_volume_viewer_json_interpreter::get_values(const std::string &key, std::array<T, N>& values) const
{
  try
  {
    const ptree &child = this->content.get_child(key);
    typename array<T, N>::iterator it_v = values.begin();
    typename ptree::const_iterator it_p = child.begin();
    while (it_v != values.cend() && it_p != child.end())
    {
      *it_v = it_p->second.get_value<T>();
      ++it_p;
      ++it_v;
    }
  }
  catch (ptree_bad_path &e)
  {
    if(debug)
    {
      cerr << e.what() << '\n';
      cerr << "The json file does not contain this child: " << key << '\n';
    }
    return false;
  }
  catch (ptree_bad_data &e)
  {
    if(debug)
    {
      cerr << e.what() << '\n';
      cerr << "The key does not contain value. \n";
    }
    return false;
  }
  return true;
}