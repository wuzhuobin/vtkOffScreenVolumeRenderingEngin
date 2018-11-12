#include <vtkSmartPointer.h>
#include <vtkVolumeViewer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
// std 
#include <string>
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
//   volumeViewer->OffScreenRenderingOn();
  std::string in;
  while(std::cin >> in)
  {
    std::cout << in << '\n';
  }
  volumeViewer->Render();
//   interactor->Start();
  auto windowToImageFilter = Ptr<vtkWindowToImageFilter>::New();
  windowToImageFilter->SetInput(volumeViewer->GetRenderWindow());
  windowToImageFilter->Update();

  auto pngWriter = Ptr<vtkPNGWriter>::New();
  pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
  pngWriter->SetFileName("png.png");
  pngWriter->Write();

//   cin.get();
  return EXIT_SUCCESS;
}