// me 
#include "vtk_volume_viewer_json_interpreter.h"
#include "vtkVolumeViewer.h"
// qt 
#include <QTest>
#include <QObject>
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

class Testvtk_volume_viewer_json_interpreter : public QObject
{
  Q_OBJECT;
private Q_SLOTS:
  void initTestCase()
  {
    image = vtkSmartPointer<vtkImageData>::New();
    CreateImageData(image);
  }
  void cleanupTestCase(){}
  void init(){}
  void cleanup(){}
  void testPresetShiftOpacitySize()
  {
    // vtkSmartPointer<vtkNIFTIImageReader> niftiImageReader = 
    //   vtkSmartPointer<vtkNIFTIImageReader>::New();
    // niftiImageReader->SetFileName("../../../T2.nii");
    // niftiImageReader->Update();
    std::string json = 
      "{"  
        "\"preset\": 0,"  
        "\"shift\": -100," 
        "\"opacity\": 0.5," 
        "\"size\":[500,500]" 
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(false);
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
        "\"dolly\":"
          "{"
            "\"last\":[10,10], "
            "\"current\":[10,100],"
            "\"motionFactor\":1"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(false);
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
        "\"pan\":"
          "{"
            "\"last\":[10,10], "
            "\"current\":[10,100]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(false);
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
        "\"rotate\":"
          "{"
            "\"last\":[10,10], "
            "\"current\":[10,500],"
            "\"motionFactor\":1"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(false);
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
        "\"rotate\":"
          "{"
            "\"last\":[10,10], "
            "\"current\":[10,200]"
          "}"
      "}";
    vtkSmartPointer<vtkVolumeViewer> viewer = 
      vtkSmartPointer<vtkVolumeViewer>::New();
    viewer->SetOffScreenRendering(true);
    viewer->SetInputData(this->image);
    vtk_volume_viewer_json_interpreter interpreter(false);
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
private:
  vtkSmartPointer<vtkImageData> image;
  const QString PRESET_SHIFT_OPACITY_SIZE = "preset_shift_opacity_size.png";
  const QString DOLLY = "dolly.png";
  const QString PAN = "pan.png"; 
  const QString ROTATE = "rotate.png";
  const QString SPIN = "spin.png";
};
QTEST_GUILESS_MAIN(Testvtk_volume_viewer_json_interpreter)
#include <Testvtk_volume_viewer_json_interpreter.moc>