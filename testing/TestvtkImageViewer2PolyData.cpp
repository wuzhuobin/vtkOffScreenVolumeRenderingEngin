/**
 * @file		TestvtkImageViewer2PolyData.cpp
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.19.2018
 * @date    Dec.19.2018
 * @copyright
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  &nbsp;  This program is distributed in the hope that it will be useful, but	<br>
  &nbsp;  WITHOUT ANY WARRANTY; without even the implied warranty of <br>
  &nbsp;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. <br>
  &nbsp;  See the LICENSE for more detail. <br> 
  &nbsp;  Copyright (c) WUZHUOBIN. All rights reserved. <br>
  &nbsp;  See COPYRIGHT for more detail. <br>
  &nbsp;  This software is distributed WITHOUT ANY WARRANTY; without even <br>
  &nbsp;  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR <br>
  &nbsp;  PURPOSE.  See the above copyright notice for more information. <br>
  &nbsp;  Internal usage only, without the permission of the author, please DO <br>
  &nbsp;  NOT publish and distribute without the author's permission. <br>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
// me 
#include "vtkImageViewer2PolyData.h"
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
/**
 * @class   TestvtkImageViewer2PolyData
 * @brief   Test vtkImageViewer2PolyData
 * @test    Try to render a dummy image and 2 spheres. 
 */
class TestvtkImageViewer2PolyData : public QObject
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
  void testImageViewer2PolyData()
  {
    Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
    CreateImageData(image);
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1);
    sphereSource->Update();
    std::string tag1 = "sphere0.1";
    std::string tag2 = "sphere0.2";
    vtkPolyDataDecorator::AddToInternalStorage(tag1, sphereSource->GetOutput());
    sphereSource->SetRadius(0.5);
    sphereSource->SetCenter(1, 0, 0);
    sphereSource->Update();
    vtkPolyDataDecorator::AddToInternalStorage(tag2, sphereSource->GetOutput());
    std::string id1 = "id1";
    std::string id2 = "id2";
    Ptr<vtkImageViewer2PolyData> viewer =
      Ptr<vtkImageViewer2PolyData>::New();
    viewer->SetInputData(image);
    viewer->SetSize(500, 500);
    viewer->AddPolyData(tag1, id1);
    viewer->AddPolyData(tag2, id2);
    viewer->SetSlice(65);
    viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
    viewer->Render();
    viewer->GetRenderWindow()->GetInteractor()->Start();
    viewer->RemovePolyData(id1);
    viewer->RemovePolyData(id2);
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
    vtkPolyDataDecorator::RemoveFromInternalStorage(tag2);
  }

//   void testVolumeViewerPolyDataTranslation()
//   {
//     Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
//     CreateImageData(image);
//     vtkSmartPointer<vtkSphereSource> sphereSource =
//         vtkSmartPointer<vtkSphereSource>::New();
//     sphereSource->SetRadius(0.1);
//     sphereSource->Update();
//     std::string tag1 = "sphere0.1";
//     std::string tag2 = "sphere0.2";
//     vtkPolyDataDecorator::AddToInternalStorage(tag1, sphereSource->GetOutput());
//     std::string id1 = "id1";
//     std::string id2 = "id2";
//     Ptr<vtkVolumeViewerPolyData> viewer =
//       Ptr<vtkVolumeViewerPolyData>::New();
//     viewer->SetInputData(image);
//     viewer->AddPolyData(tag1, id1);
//     viewer->AddPolyData(tag1, id2);
//     viewer->TranslatePolyData(id1, {1, 1, 1});
//     viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
//     viewer->Render();
//     vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
//     // viewer->GetInteractor()->Start();
//   }

//   void testVolumeViewerPolyDataRotation()
//   {
//     Ptr<vtkImageData> image = Ptr<vtkImageData>::New();
//     CreateImageData(image);
//     vtkSmartPointer<vtkConeSource> coneSource =
//         vtkSmartPointer<vtkConeSource>::New();
//     coneSource->SetRadius(0.1);
//     coneSource->Update();
//     std::string tag1 = "cone0.1";
//     std::string tag2 = "cone0.2";
//     vtkPolyDataDecorator::AddToInternalStorage(tag1, coneSource->GetOutput());
//     std::string id1 = "id1";
//     std::string id2 = "id2";
//     Ptr<vtkVolumeViewerPolyData> viewer =
//       Ptr<vtkVolumeViewerPolyData>::New();
//     viewer->SetInputData(image);
//     viewer->AddPolyData(tag1, id1);
//     viewer->AddPolyData(tag1, id2);
//     viewer->RotatePolyData(id1, {180, 180, 0});
//     viewer->SetupInteractor(Ptr<vtkRenderWindowInteractor>::New());
//     viewer->Render();
//     vtkPolyDataDecorator::RemoveFromInternalStorage(tag1);
//     // viewer->GetInteractor()->Start();
//   }
};

QTEST_GUILESS_MAIN(TestvtkImageViewer2PolyData)
#include "TestvtkImageViewer2PolyData.moc"