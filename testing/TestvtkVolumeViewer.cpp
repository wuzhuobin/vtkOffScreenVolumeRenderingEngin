/**
 * @file		TestvtkVolumeViewer.cpp
 * @author	WUZHUOBIN jiejin2022@163.com
 * @since   Dec.18.2018
 * @date    Dec.18.2018
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
/**
 * @brief         Create a dummy image.
 * @param[in]     imageData 
 * 
 */
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
 * @class     TestvtkVolumeViewer
 * @brief     Test vtkVolumeViewer. 
 * @test      Try to render vtkVolumeViewer with a dummy image. 
 * 
 */
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
};

QTEST_GUILESS_MAIN(TestvtkVolumeViewer)
#include "TestvtkVolumeViewer.moc"