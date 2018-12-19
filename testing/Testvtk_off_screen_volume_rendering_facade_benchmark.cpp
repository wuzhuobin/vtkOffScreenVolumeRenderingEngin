/**
 * @file		Testvtk_off_screen_volume_rendering_facade_benchmark.cpp
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
// qt 
#include <QTest>
#include <QObject>
#include <QFile>
#include <QDebug>
// vtk
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
// me 
#include "vtk_off_screen_volume_rendering_facade.h"
// std 
#include <string>
#include <fstream>
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
 * @class    Testvtk_off_screen_volume_rendering_facade_benchmark
 * @brief    A benchmark test of vtk_off_screen_volume_rendering_facade_benchmark.
 * @test     A benchmark test comparing rendering png and jpeg in different size.
 */
class Testvtk_off_screen_volume_rendering_facade_benchmark : public QObject
{
  Q_OBJECT;
private Q_SLOTS:
  void initTestCase()
  {
    this->image = vtkSmartPointer<vtkImageData>::New();
    CreateImageData(image);
  }
  void cleanupTestCase()
  {
  }
  void init(){}
  void cleanup()
  {
  }
  void benmarkPNG_data()
  {
    QTest::addColumn<QString>("json");
    QString jsonTemplate = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[%1,%1]" 
      "}";
    for(int size = 50; size < 10000; size+=50)
    {
      QTest::newRow(QString("Size: [%1,%1]: ").arg(size).toUtf8().constData()) << jsonTemplate.arg(size);
    }
  }
  void benmarkPNG()
  {
    QSKIP("Skip benchmark");
    QFETCH(QString, json);
    vtk_off_screen_volume_rendering_facade facade(this->image);
    QBENCHMARK_ONCE
    {
      std::vector<unsigned char> data;
      facade.render_png(json.toStdString(), data);
    }
  } 
  void benmarkJPEG_data()
  {
    QTest::addColumn<QString>("json");
    QString jsonTemplate = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[%1,%1]" 
      "}";
    for(int size = 50; size < 10000; size+=50)
    {
      QTest::newRow(QString("Size: [%1,%1]: ").arg(size).toUtf8().constData()) << jsonTemplate.arg(size);
    }
  }
  void benmarkJPEG()
  {
    QSKIP("Skip benchmark");
    QFETCH(QString, json);
    vtk_off_screen_volume_rendering_facade facade(this->image);
    QBENCHMARK_ONCE
    {
      std::vector<unsigned char> data;
      facade.render_jpeg(json.toStdString(), data);
    }
  } 
private:
  vtkSmartPointer<vtkImageData> image;
};
QTEST_GUILESS_MAIN(Testvtk_off_screen_volume_rendering_facade_benchmark)
#include "Testvtk_off_screen_volume_rendering_facade_benchmark.moc"