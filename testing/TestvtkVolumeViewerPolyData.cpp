// me 
#include "vtkVolumeViewerPolyData.h"
// vtk
#include <vtkSmartPointer.h>
template <typename T>
using Ptr = vtkSmartPointer<T>;
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPNGWriter.h>
#include <vtkNIFTIImageReader.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkConeSource.h>
// qt
#include <QObject>
#include <QTest>
void CreateImageData(vtkImageData* imageData)
{
  // Create a spherical implicit function.
  vtkSmartPointer<vtkSphere> sphere =
    vtkSmartPointer<vtkSphere>::New();
  sphere->SetRadius(100);
  sphere->SetCenter(0.0,0.0,0.0);
 
  vtkSmartPointer<vtkSampleFunction> sampleFunction =
    vtkSmartPointer<vtkSampleFunction>::New();
  sampleFunction->SetImplicitFunction(sphere);
  sampleFunction->SetOutputScalarTypeToDouble();
  sampleFunction->SetSampleDimensions(127,127,127); // intentional NPOT dimensions.
  sampleFunction->SetModelBounds(-1.0,1.0,-1.0,1.0,-1.0,1.0);
  sampleFunction->SetCapping(false);
  sampleFunction->SetComputeNormals(false);
  sampleFunction->SetScalarArrayName("values");
  sampleFunction->Update();
 
  vtkDataArray* a = sampleFunction->GetOutput()->GetPointData()->GetScalars("values");
  double range[2];
  a->GetRange(range);
 
  vtkSmartPointer<vtkImageShiftScale> t = 
    vtkSmartPointer<vtkImageShiftScale>::New();
  t->SetInputConnection(sampleFunction->GetOutputPort());
 
  t->SetShift(-range[0]);
  double magnitude=range[1]-range[0];
  if(magnitude==0.0)
    {
    magnitude=1.0;
    }
  t->SetScale(255.0/magnitude);
  t->SetOutputScalarTypeToUnsignedChar();
 
  t->Update();
 
  imageData->ShallowCopy(t->GetOutput());
}

class TestvtkVolumeViewerPolyData : public QObject
{
  Q_OBJECT;
private Q_SLOTS:
  void initTestCase()
  {}
  void cleanupTestCase()
  {}
  void init()
  {}
  void cleanup()
  {}
  void testVolumeViewerPolyData()
  {
    Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
    CreateImageData(image);
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(0.1);
    sphereSource->Update();
    std::string tag1 = "sphere0.1";
    std::string tag2 = "sphere0.2";
    vtkPolyDataDecorator::AddToInternalStorage(tag1, sphereSource->GetOutput());
    sphereSource->SetRadius(0.2);
    sphereSource->SetCenter(1, 2, 3);
    sphereSource->Update();
    vtkPolyDataDecorator::AddToInternalStorage(tag2, sphereSource->GetOutput());
    std::string id1 = "id1";
    std::string id2 = "id2";
    Ptr<vtkVolumeViewerPolyData> viewer =
      Ptr<vtkVolumeViewerPolyData>::New();
    viewer->SetInputData(image);
    viewer->AddPolyData(tag1, id1);
    viewer->AddPolyData(tag2, id2);
    viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
    viewer->Render();
    viewer->RemovePolyData(id1);
    viewer->RemovePolyData(id2);
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag2);
    // viewer->GetInteractor()->Start();
  }

  void testVolumeViewerPolyDataTranslation()
  {
    Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
    CreateImageData(image);
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(0.1);
    sphereSource->Update();
    std::string tag1 = "sphere0.1";
    std::string tag2 = "sphere0.2";
    vtkPolyDataDecorator::AddToInternalStorage(tag1, sphereSource->GetOutput());
    std::string id1 = "id1";
    std::string id2 = "id2";
    Ptr<vtkVolumeViewerPolyData> viewer =
      Ptr<vtkVolumeViewerPolyData>::New();
    viewer->SetInputData(image);
    viewer->AddPolyData(tag1, id1);
    viewer->AddPolyData(tag1, id2);
    viewer->TranslatePolyData(id1, {1, 1, 1});
    viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
    viewer->Render();
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
    // viewer->GetInteractor()->Start();
  }

  void testVolumeViewerPolyDataRotation()
  {
    Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
    CreateImageData(image);
    vtkSmartPointer<vtkConeSource> coneSource =
        vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetRadius(0.1);
    coneSource->Update();
    std::string tag1 = "cone0.1";
    std::string tag2 = "cone0.2";
    vtkPolyDataDecorator::AddToInternalStorage(tag1, coneSource->GetOutput());
    std::string id1 = "id1";
    std::string id2 = "id2";
    Ptr<vtkVolumeViewerPolyData> viewer =
      Ptr<vtkVolumeViewerPolyData>::New();
    viewer->SetInputData(image);
    viewer->AddPolyData(tag1, id1);
    viewer->AddPolyData(tag1, id2);
    viewer->RotatePolyData(id1, {180, 180, 0});
    viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
    viewer->Render();
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
    // viewer->GetInteractor()->Start();
  }
};

QTEST_GUILESS_MAIN(TestvtkVolumeViewerPolyData)
#include "TestvtkVolumeViewerPolyData.moc"