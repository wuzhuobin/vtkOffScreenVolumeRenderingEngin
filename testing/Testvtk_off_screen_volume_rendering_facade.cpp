/**
 * @file		Testvtk_off_screen_volume_rendering_facade.cpp
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
 * @class    Testvtk_off_screen_volume_rendering_facade
 * @brief    Test vtk_off_screen_volume_rendering_facade.
 * @test     Tests to write the render window image as png or jpeg to disk. 
 */
class Testvtk_off_screen_volume_rendering_facade : public QObject
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
    QFile(this->PNG).remove(); 
    QFile(this->JPEG).remove();
  }
  void init(){}
  void cleanup()
  {
  }
  void testJPG()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":0.5," 
        "\"size\":[5000,500]" 
      "}";
    vtk_off_screen_volume_rendering_facade facade(this->image);
    std::vector<unsigned char> data;
    facade.render_jpeg(json, data);
    std::ofstream fout(this->JPEG.toStdString(), std::ofstream::out | std::ofstream::binary);
    // qDebug() << "size: " << data.size();
    fout.write((const char*)(data.data()), data.size()* sizeof(char));
    fout.close();
  }
  void testPNG()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":0.5," 
        "\"size\":[500,5000]" 
      "}";
    vtk_off_screen_volume_rendering_facade facade(this->image);
    std::vector<unsigned char> data;
    facade.render_png(json, data);
    std::ofstream fout(this->PNG.toStdString(), std::ofstream::out | std::ofstream::binary);
    // qDebug() << "size: " << data.size();
    fout.write((const char*)(data.data()), data.size()* sizeof(char));
    fout.close();
  }
private:
  vtkSmartPointer<vtkImageData> image;
  const QString PNG = "png.png";
  const QString JPEG = "jpg.jpg";
};
QTEST_GUILESS_MAIN(Testvtk_off_screen_volume_rendering_facade)
#include "Testvtk_off_screen_volume_rendering_facade.moc"