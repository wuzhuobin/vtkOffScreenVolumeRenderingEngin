// me 
#include "vtk_volume_viewer_json_interpreter.h"
#include "vtkVolumeViewer.h"
// boost
using namespace boost::property_tree;
// std
#include <iostream>
#include <sstream>
using namespace std;
vtk_volume_viewer_json_interpreter::vtk_volume_viewer_json_interpreter() 
{

}

vtk_volume_viewer_json_interpreter::~vtk_volume_viewer_json_interpreter()
{
}

void vtk_volume_viewer_json_interpreter::read_json(std::istream &in)
{
  boost::property_tree::read_json(in, this->content);
}

void vtk_volume_viewer_json_interpreter::read_json(const std::string &content)
{
//   boost::property_tree::read_json(content, this->content);
  stringstream ss;
  ss << content;
  boost::property_tree::read_json(ss, this->content);
}

void vtk_volume_viewer_json_interpreter::read_json_file(const std::string &filename)
{
  boost::property_tree::read_json(filename, this->content);
}

void vtk_volume_viewer_json_interpreter::write_json(std::ostream &out)
{

}

void vtk_volume_viewer_json_interpreter::write_json(const std::string &content)
{
//   boost::property_tree::write_json(filename, this->content);
}

void vtk_volume_viewer_json_interpreter::write_json_file(const std::string &filename)
{
  boost::property_tree::write_json(filename, this->content);
}

void vtk_volume_viewer_json_interpreter::interpret(vtkVolumeViewer *viewer)
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
  if(this->get_value("opacity"), opacity)
  {
      viewer->SetOpacity(opacity);
  }
  this->interpret(viewer->GetRenderer());
  
}

void vtk_volume_viewer_json_interpreter::interpret(vtkRenderer *renderer)
{
  string line;
  this->get_value("dolly.last", line);
  cout << line << '\n';
}

template<typename T>
bool vtk_volume_viewer_json_interpreter::get_value(const std::string &key, T &value)
{
//   boost::optional<ptree> child = this->content.get_child_optional(key);
  try
  {
    ptree &child = this->content.get_child(key);
    T attribute = child.get_value<T>();
    value = attribute;
  }
  catch(ptree_bad_path &e){
    cerr << e.what();
    cerr << "The json file does not contain this child: " << key << '\n';
    return false;
  }
  catch(ptree_bad_data &e){
    cerr << e.what();
    cerr << "The key does not contain value. \n";
    return false;
  }
  return true;
}