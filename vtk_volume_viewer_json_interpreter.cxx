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
}

void vtk_volume_viewer_json_interpreter::dolly(vtkRenderer *renderer) const
{
  array<double, 2> current;
  array<double, 2> last;
  double motion_factor;
  this->get_values("dolly.current", current);
  this->get_values("dolly.last", last);
  this->get_value("dolly.motionFactor", motion_factor);
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

void vtk_volume_viewer_json_interpreter::vtk_volume_viewer_json_interpreter::pan(vtkRenderer *renderer)
{
  array<double, 2> current;
  array<double, 2> last;
  double motion_factor;
  this->get_values("pan.current", current);
  this->get_values("pan.last", last);
  this->get_value("pan.motionFactor", motion_factor); 
  // // Calculate the focal depth since we'll be using it a lot
  // vtkCamera *camera = renderer->GetActiveCamera();
  // double view_focus[4]{1,1,1,1};
  // camera->GetFocalPoint(view_focus);
  // renderer->SetWorldPoint(view_focus);
  // renderer->WorldToDisplay();
  // double focal_depth = view_focus[2];
  // this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
  //                             viewFocus);
  // focalDepth = viewFocus[2];

  // this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
  //                             rwi->GetEventPosition()[1],
  //                             focalDepth,
  //                             newPickPoint);

  // // Has to recalc old mouse point since the viewport has moved,
  // // so can't move it outside the loop

  // this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
  //                             rwi->GetLastEventPosition()[1],
  //                             focalDepth,
  //                             oldPickPoint);

  // // Camera motion is reversed

  // motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  // motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  // motionVector[2] = oldPickPoint[2] - newPickPoint[2];

  // camera->GetFocalPoint(viewFocus);
  // camera->GetPosition(viewPoint);
  // camera->SetFocalPoint(motionVector[0] + viewFocus[0],
  //                       motionVector[1] + viewFocus[1],
  //                       motionVector[2] + viewFocus[2]);

  // camera->SetPosition(motionVector[0] + viewPoint[0],
  //                     motionVector[1] + viewPoint[1],
  //                     motionVector[2] + viewPoint[2]);

  // renderer->UpdateLightsGeometryToFollowCamera();
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