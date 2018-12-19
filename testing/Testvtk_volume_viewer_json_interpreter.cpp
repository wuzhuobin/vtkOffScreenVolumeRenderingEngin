/**
 * @file		Testvtk_volume_viewer_json_interpreter.cpp
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
#include "vtk_volume_viewer_json_interpreter.h"
#include "vtkVolumeViewer.h"
#include "vtkVolumeViewerPolyData.h"
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
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPNGWriter.h>
#include <vtkNIFTIImageReader.h>
#include <vtkConeSource.h>
#include <vtkPolyData.h>
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
 * @class   Testvtk_volume_viewer_json_interpreter
 * @brief   Test vtk_volume_viewer_json_interpreter.
 * @test    Interpret json to set preset, set shift, set opacity, set size.
 *          Interpret json to set dolly. 
 *          Interpret json to set pan.
 *          Interpret json to set spin. 
 *          Interpret json to render a sphere, render a sphere rotated and translated
 *          and a sphere then remove it.
 */
class Testvtk_volume_viewer_json_interpreter : public QObject
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
   QFile(this->PRESET_SHIFT_OPACITY_SIZE).remove(); 
   QFile(this->DOLLY).remove(); 
   QFile(this->PAN).remove(); 
   QFile(this->ROTATE).remove(); 
   QFile(this->SPIN).remove();
   QFile(this->POLY_DATA).remove();
  }
  void init(){}
  void cleanup()
  {

  }
  void testPresetShiftOpacitySize()
  {
    // vtkSmartPointer<vtkNIFTIImageReader> niftiImageReader = 
    //   vtkSmartPointer<vtkNIFTIImageReader>::New();
    // niftiImageReader->SetFileName("../../../T2.nii");
    // niftiImageReader->Update();
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":-100," 
        "\"opacity\":0.5," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(viewer);
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->PRESET_SHIFT_OPACITY_SIZE.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
  void testDolly()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(viewer);
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->DOLLY.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }

  void testPan()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,100]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "}"
      "}";

    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(viewer);
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->PAN.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
  void testRotate()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,100],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(viewer);
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->ROTATE.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
  void testSpin()
  {
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,100]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(viewer);
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->SPIN.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
  
  void testPolyData()
  {
    vtkSmartPointer<vtkConeSource> coneSource =
      vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetRadius(0.1);
    coneSource->Update();
    vtkPolyDataDecorator::AddToInternalStorage("tag", coneSource->GetOutput());
    std::string json = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"polyData\":"
        "{"
          "\"tag\": \"tag\","
          "\"id\": \"id\","
          "\"existence\": true,"
          "\"translation\": [0, 0, 0],"
          "\"rotation\": [0, 0, 0]"
        "}"
      "}";
    std::string json2 = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"polyData\":"
        "{"
          "\"tag\": \"tag\","
          "\"id\": \"id2\","
          "\"existence\": true,"
          "\"translation\": [1, 1, 1],"
          "\"rotation\": [180, 180, 0]"
        "}"
      "}";
    std::string json3 = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"polyData\":"
        "{"
          "\"tag\": \"tag\","
          "\"id\": \"id3\","
          "\"existence\": true,"
          "\"translation\": [1, 1, 1],"
          "\"rotation\": [180, 180, 0]"
        "}"
      "}";
    std::string json4 = 
      "{"  
        "\"preset\":0,"  
        "\"shift\":0," 
        "\"opacity\":1," 
        "\"size\":[500,500]," 
        "\"dolly\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"pan\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"rotate\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0],"
            "\"motionFactor\":1"
          "},"
        "\"spin\":"
          "{"
            "\"last\":[0,0],"
            "\"current\":[0,0]"
          "},"
        "\"polyData\":"
        "{"
          "\"tag\": \"tag\","
          "\"id\": \"id3\","
          "\"existence\": false,"
          "\"translation\": [1, 1, 1],"
          "\"rotation\": [180, 180, 0]"
        "}"
      "}";
    vtkSmartPointer<vtkVolumeViewerPolyData> viewer = 
      vtkSmartPointer<vtkVolumeViewerPolyData>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(true);
    interpreter.read_json(json);
    interpreter.interpret(static_cast<vtkVolumeViewer*>(viewer));
    interpreter.interpret(static_cast<vtkPolyDataDecorator*>(viewer));
    interpreter.read_json(json2);
    interpreter.interpret(static_cast<vtkVolumeViewer*>(viewer));
    interpreter.interpret(static_cast<vtkPolyDataDecorator*>(viewer));
    interpreter.read_json(json3);
    interpreter.interpret(static_cast<vtkVolumeViewer*>(viewer));
    interpreter.interpret(static_cast<vtkPolyDataDecorator*>(viewer));
    interpreter.read_json(json4);
    interpreter.interpret(static_cast<vtkVolumeViewer*>(viewer));
    interpreter.interpret(static_cast<vtkPolyDataDecorator*>(viewer));
    viewer->Render();
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(viewer->GetRenderWindow());
     vtkSmartPointer<vtkPNGWriter> pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
    pngWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
    pngWriter->SetWriteToMemory(true);
    pngWriter->Write();
    vtkUnsignedCharArray *result = pngWriter->GetResult();
    std::ofstream fout(this->POLY_DATA.toStdString(), std::ofstream::out | std::ofstream::binary);
    fout.write(static_cast<const char*>(result->GetVoidPointer(0)), result->GetNumberOfValues() * sizeof(char));
    fout.close();
  }
private:
  vtkSmartPointer<vtkImageData> image;
  const QString PRESET_SHIFT_OPACITY_SIZE = "preset_shift_opacity_size.png";
  const QString DOLLY = "dolly.png";
  const QString PAN = "pan.png"; 
  const QString ROTATE = "rotate.png";
  const QString SPIN = "spin.png";
  const QString POLY_DATA = "polydata.png";
};
QTEST_GUILESS_MAIN(Testvtk_volume_viewer_json_interpreter)
#include "Testvtk_volume_viewer_json_interpreter.moc"