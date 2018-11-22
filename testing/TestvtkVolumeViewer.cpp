// me 
#include "vtkVolumeViewer.h"
// vtk
#include <vtkSmartPointer.h>
template <typename T>
using Ptr = vtkSmartPointer<T>;
#include <vtkRenderWindowInteractor.h>
#include <vtkNIFTIImageReader.h>
// qt
#include <QObject>
#include <QTest>
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
      Ptr<vtkNIFTIImageReader> reader = 
        Ptr<vtkNIFTIImageReader>::New();
      reader->SetFileName("T2.nii");
      reader->Update();

      Ptr<vtkVolumeViewer> viewer = 
        Ptr<vtkVolumeViewer>::New();
      viewer->SetInputConnection(reader->GetOutputPort());
      viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
      viewer->Render();
      viewer->GetInteractor()->Start();
  }
private:
  
};

QTEST_GUILESS_MAIN(TestvtkVolumeViewer)
#include "TestvtkVolumeViewer.moc"