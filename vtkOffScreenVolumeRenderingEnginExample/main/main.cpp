// vtk 
#include <vtkSmartPointer.h>
#include <vtkVolumeViewer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkUnsignedCharArray.h>
template<typename T>
using Ptr = vtkSmartPointer<T>;
// std
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;
// me 
#include "vtk_volume_viewer_json_interpreter.h"
int main(int argc, char *argv[])
{
  if(argc < 3){
    std::cerr << "Should have 2 input parameters. The first one is nifti image, the second one is json file.\n";
    return EXIT_FAILURE;
  }
  auto niftiImageReader = Ptr<vtkNIFTIImageReader>::New();
  // niftiImageReader->SetFileName("../../../T2.nii");
  niftiImageReader->SetFileName(argv[1]);
  niftiImageReader->Update();
  auto interactor = Ptr<vtkRenderWindowInteractor>::New();
  auto volumeViewer = Ptr<vtkVolumeViewer>::New();
  volumeViewer->SetInputConnection(niftiImageReader->GetOutputPort());
  volumeViewer->SetupInteractor(interactor);
  volumeViewer->SetPreset(vtkVolumeViewer::MR_DEFAULT);
  volumeViewer->OffScreenRenderingOn();
  volumeViewer->Render();
  auto windowToImageFilter = Ptr<vtkWindowToImageFilter>::New();
  windowToImageFilter->SetInput(volumeViewer->GetRenderWindow());
  auto pngWriter = Ptr<vtkPNGWriter>::New();
  pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
  pngWriter->SetWriteToMemory(true);
  ofstream fout;
  vtk_volume_viewer_json_interpreter interpreter(true);
  cout << "START!\n";
  while(cin.get())
  {
    interpreter.read_json_file(argv[2]);
    std::string aa;
    interpreter.write_json(aa);
    cerr << aa;
    interpreter.interpret(volumeViewer);
    volumeViewer->Render();
    windowToImageFilter->Modified();
    windowToImageFilter->Update();
    pngWriter->Write();
    auto result = pngWriter->GetResult();
    fout.open("png.png", fstream::out | fstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
  return EXIT_SUCCESS;
}