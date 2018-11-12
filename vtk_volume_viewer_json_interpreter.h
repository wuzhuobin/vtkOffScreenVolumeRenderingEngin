#ifndef __VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__
#define __VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__
// me 
class vtkVolumeViewer;
class vtkRenderer;
//boos
#include <boost/property_tree/json_parser.hpp>
// std
#include <string>
#include <iostream>
class vtk_volume_viewer_json_interpreter final
{
public:
  vtk_volume_viewer_json_interpreter();
  ~vtk_volume_viewer_json_interpreter();
  void read_json(std::istream &in);
  void read_json(const std::string &content);
  void read_json_file(const std::string &filename);
  void write_json(std::ostream &out);
  void write_json(const std::string &content);
  void write_json_file(const std::string &filename);
  /**
   * @brief Function for interpreter and bridge. 
   * 
   */
  void interpret(vtkVolumeViewer *viewer);
  void interpret(vtkRenderer *renderer);
private:
  boost::property_tree::ptree content;
  template<typename T>
  bool get_value(const std::string &key, T &value);
  vtk_volume_viewer_json_interpreter(const vtk_volume_viewer_json_interpreter &) = delete;
  void operator=(const vtk_volume_viewer_json_interpreter &) = delete;
  vtk_volume_viewer_json_interpreter(vtk_volume_viewer_json_interpreter &&) = delete;
  void operator=(vtk_volume_viewer_json_interpreter &&) = delete;
};

#endif // !__VTK_VOLUME_VIEWER_JSON_INTERPRETER_H__