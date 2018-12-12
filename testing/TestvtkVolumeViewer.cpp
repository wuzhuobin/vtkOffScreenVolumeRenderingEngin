// me 
#include "vtkVolumeViewer.h"
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
class TestvtkVolumeViewer: public QObject
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
  void testVolumeViewer()
  {
      // Ptr<vtkNIFTIImageReader> reader = 
      //   Ptr<vtkNIFTIImageReader>::New();
      // reader->SetFileName("T2.nii");
      // reader->Update();
      // image = reader->GetOutput();
      Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
      CreateImageData(image);

      Ptr<vtkVolumeViewer> viewer = 
        Ptr<vtkVolumeViewer>::New();
      viewer->SetInputData(image);
      viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
      viewer->Render();
      // viewer->GetInteractor()->Start();
  }
private:
  
};

QTEST_GUILESS_MAIN(TestvtkVolumeViewer)
#include "TestvtkVolumeViewer.moc"