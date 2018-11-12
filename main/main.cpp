#include <vtkSmartPointer.h>
#include <vtkVolumeViewer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkCamera.h>
// std 
#include <string>
#include <set>
template<typename T>
using Ptr = vtkSmartPointer<T>;
int main(int argc, char* argv[])
{
  auto niftiImageReader =  Ptr<vtkNIFTIImageReader>::New();
  niftiImageReader->SetFileName("C:/Users/jieji/Desktop/T2propeller&MRA/T2.nii");
  niftiImageReader->Update();
  auto interactor = Ptr<vtkRenderWindowInteractor>::New();
  auto volumeViewer = Ptr<vtkVolumeViewer>::New();
  volumeViewer->SetInputConnection(niftiImageReader->GetOutputPort());
  volumeViewer->SetupInteractor(interactor);
  volumeViewer->SetPreset(vtkVolumeViewer::MR_DEFAULT);
  volumeViewer->Render();
  auto windowToImageFilter = Ptr<vtkWindowToImageFilter>::New();
  windowToImageFilter->SetInput(volumeViewer->GetRenderWindow());
  auto pngWriter = Ptr<vtkPNGWriter>::New();
  pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
  pngWriter->SetFileName("png.png");
//   volumeViewer->OffScreenRenderingOn();
  std::string in;
  std::set<std::string> options{"preset", "shift", "opacity", "dolly", "roll", "azimuth", "yaw", "elevation", "pitch"};
  auto type = options.cend();
  while(std::cin >> in)
  {
    auto cit = options.find(in);
    if(cit != options.cend()){
        type = cit; 
        continue;
    }
    else if(type != options.end()){
        if(*type == "preset")
        {
          int preset = std::stoi(in);
          volumeViewer->SetPreset(preset);
        }
        else if(*type == "shift")
        {
          double shift = std::stod(in);
          volumeViewer->SetShift(shift);
        }
        else if(*type == "opacity")
        {
          double opacity = std::stod(in);
          volumeViewer->SetOpacity(opacity);
        }
        volumeViewer->Render();
        windowToImageFilter->Modified();
        windowToImageFilter->Update();
        pngWriter->Write();
    }
  }
  return EXIT_SUCCESS;
}