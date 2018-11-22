#include "vtkVolumeViewer.h"
// vtk
#include <vtkVolume.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkInteractorObserver.h>
#include <vtkInformation.h>
#include <vtkVolumeProperty.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkCamera.h>
static vtkSmartVolumeMapper * get_mapper(vtkVolume * &volume)
{
    return reinterpret_cast<vtkSmartVolumeMapper*>(volume->GetMapper());
}
static void set_volume_property(vtkVolume *& volume, const int & preset, const double & shift, const double & opacity);
vtkStandardNewMacro(vtkVolumeViewer);
vtkVolumeViewer::vtkVolumeViewer()
{
  this->RenderWindow    = nullptr;
  this->Renderer        = nullptr;
  this->Volume = vtkVolume::New();
  this->Volume->SetMapper(vtkSmartPointer<vtkSmartVolumeMapper>::New());
  this->Interactor      = nullptr;
  this->FirstRender = true;
  this->Preset = NONE;
  this->Shift = 0.0;
  this->Opacity = 1.0;
  // Setup the pipeline
  this->SetRenderer(vtkSmartPointer<vtkRenderer>::New());
  this->SetRenderWindow(vtkSmartPointer<vtkRenderWindow>::New());
  this->InstallPipeline();
}

vtkVolumeViewer::~vtkVolumeViewer()
{
    if (this->Volume)
    {
        this->Volume->Delete();
        this->Volume = nullptr;
    }
    if (this->Renderer)
    {
        this->Renderer->Delete();
        this->Renderer = nullptr;
    }
    if (this->RenderWindow)
    {
        this->RenderWindow->Delete();
        this->RenderWindow = nullptr;
    }
    if (this->Interactor)
    {
        this->Interactor->Delete();
        this->Interactor = nullptr;
    }
}

void vtkVolumeViewer::PrintSelf(ostream &os, vtkIndent indent)
{

}

const char * vtkVolumeViewer::GetWindowName()
{
    return this->RenderWindow->GetWindowName();
}

void vtkVolumeViewer::Render()
{
    if (this->FirstRender)
    {
        // Initialize the size if not set yet
        if (this->Renderer)
        {
            this->Renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
            this->Renderer->GetActiveCamera()->SetPosition(0, -1, 0);
            this->Renderer->GetActiveCamera()->SetViewUp(0, 0, 1);
            set_volume_property(this->Volume, this->Preset, this->Shift, this->Opacity);
            this->Renderer->ResetCamera();
            this->Renderer->ResetCameraClippingRange();
        }
        this->FirstRender = false;
    }
    if (this->GetInput())
    {
        this->RenderWindow->Render();
    }
}

void vtkVolumeViewer::ResetCamera()
{
    if(this->FirstRender)
    {
        return;
    }
    this->Renderer->ResetCamera();
}

void vtkVolumeViewer::ResetCameraClippingRange()
{
    if(this->FirstRender)
    {
        return;
    }
    this->Renderer->ResetCameraClippingRange();
}

void vtkVolumeViewer::SetInputData(vtkImageData * in)
{
    // get_mapper(this->Volume)->SetInputData(in);
    this->Volume->GetMapper()->SetInputDataObject(in);
}

vtkImageData * vtkVolumeViewer::GetInput()
{
    return vtkImageData::SafeDownCast(this->Volume->GetMapper()->GetInputDataObject(0, 0));
}   

void vtkVolumeViewer::SetInputConnection(vtkAlgorithmOutput * input)
{
    this->Volume->GetMapper()->SetInputConnection(input);
}

void vtkVolumeViewer::SetPreset(int preset)
{
    this->Preset = (preset > 26 ? 26 : preset);
    this->Preset = (preset < 0 ? 0 : preset);
    set_volume_property(this->Volume, this->Preset, this->Shift, this->Opacity);
}

void vtkVolumeViewer::SetShift(double shift)
{
    this->Shift = shift;
    set_volume_property(this->Volume, this->Preset, this->Shift, this->Opacity);
}

void vtkVolumeViewer::SetOpacity(double opacity)
{
    this->Opacity = opacity;
    set_volume_property(this->Volume, this->Preset, this->Shift, this->Opacity);
}

int *vtkVolumeViewer::GetPosition()
{
  return this->RenderWindow->GetPosition();
}

void vtkVolumeViewer::SetPosition(int a, int b)
{
  this->RenderWindow->SetPosition(a, b);
}

int * vtkVolumeViewer::GetSize()
{
    return this->RenderWindow->GetSize();
}

void vtkVolumeViewer::SetSize(int a, int b)
{
    this->RenderWindow->SetSize(a, b);
}

void vtkVolumeViewer::SetupInteractor(vtkRenderWindowInteractor *arg)
{
  if (this->Interactor == arg)
  {
    return;
  }

  this->UnInstallPipeline();

  if (this->Interactor)
  {
    this->Interactor->UnRegister(this);
  }

  this->Interactor = arg;

  if (this->Interactor)
  {
    this->Interactor->Register(this);
  }

  this->InstallPipeline();
}

//----------------------------------------------------------------------------
void vtkVolumeViewer::SetRenderWindow(vtkRenderWindow *arg)
{
  if (this->RenderWindow == arg)
  {
    return;
  }

  this->UnInstallPipeline();

  if (this->RenderWindow)
  {
    this->RenderWindow->UnRegister(this);
  }

  this->RenderWindow = arg;

  if (this->RenderWindow)
  {
    this->RenderWindow->Register(this);
  }

  this->InstallPipeline();
}

//----------------------------------------------------------------------------
void vtkVolumeViewer::SetRenderer(vtkRenderer *arg)
{
  if (this->Renderer == arg)
  {
    return;
  }

  this->UnInstallPipeline();

  if (this->Renderer)
  {
    this->Renderer->UnRegister(this);
  }

  this->Renderer = arg;

  if (this->Renderer)
  {
    this->Renderer->Register(this);
  }

  this->InstallPipeline();
}

vtkInteractorObserver * vtkVolumeViewer::GetInteractorStyle() 
{
    return this->Interactor->GetInteractorStyle();
}

void vtkVolumeViewer::SetOffScreenRendering(int off)
{
    this->RenderWindow->SetOffScreenRendering(off);
}

bool vtkVolumeViewer::GetOffScreenRendering()
{
    return this->RenderWindow->GetOffScreenRendering() == 0?false:true;
}

void vtkVolumeViewer::InstallPipeline()
{
  if(this->RenderWindow && this->Renderer)
  {
    this->RenderWindow->AddRenderer(this->Renderer);
  }
  if(this->Interactor)
  {
    if(!this->Interactor->GetInteractorStyle())
    {
      this->Interactor->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleSwitch>::New());
    }
    this->Interactor->SetRenderWindow(this->RenderWindow);
  }
  if(this->Renderer && this->Volume)
  {
    this->Renderer->AddViewProp(this->Volume);
  }
}

void vtkVolumeViewer::UnInstallPipeline()
{
//   if(this->Volume)
//   {
//     this->Volume->GetMapper()->SetInputConnection(nullptr);
//   }
  if(this->Renderer && this->Volume)
  {
    this->Renderer->RemoveViewProp(this->Volume);
  }
  if(this->RenderWindow && this->Renderer)
  {
      this->RenderWindow->RemoveRenderer(this->Renderer);
  }
  if(this->Interactor)
  {
    this->Interactor->SetInteractorStyle(nullptr);
    this->Interactor->SetRenderWindow(nullptr);
  }
}

vtkAlgorithm * vtkVolumeViewer::GetInputAlgorithm()
{
    return this->Volume->GetMapper()->GetInputAlgorithm();
}

vtkInformation * vtkVolumeViewer::GetInputInformation()
{
    return this->Volume->GetMapper()->GetInputInformation();
}
#include "vtkVolumeViewerPresetConfiguration.cxx"