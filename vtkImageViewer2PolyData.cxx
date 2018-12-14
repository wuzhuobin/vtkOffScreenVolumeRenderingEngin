// me 
#include "vtkImageViewer2PolyData.h"
// vtk 
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkTransform.h>
class vtkStripperPolygon : public vtkStripper 
{
public :
	static vtkStripperPolygon* New() { return new vtkStripperPolygon; }
	vtkTypeMacro(vtkStripperPolygon, vtkStripper);
protected:
	// Usual data generation method
	virtual int RequestData(
		vtkInformation *request,
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) VTK_OVERRIDE {
		int returnValue = Superclass::RequestData(request, inputVector, outputVector);
		// get the info objects
		vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
		vtkInformation *outInfo = outputVector->GetInformationObject(0);
		vtkPolyData *output = vtkPolyData::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));
		output->SetPolys(output->GetLines());
		return returnValue;
	}
};

vtkStandardNewMacro(vtkImageViewer2PolyData);
vtkImageViewer2PolyData::vtkImageViewer2PolyData()
{
  this->InstallPipeline();
}

vtkImageViewer2PolyData::~vtkImageViewer2PolyData()
{
  
}

void vtkImageViewer2PolyData::PrintSelf(std::ostream &os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

void vtkImageViewer2PolyData::UpdateDisplayExtent()
{
  vtkAlgorithm *input = this->GetInputAlgorithm();
  if (!input || !this->ImageActor)
  {
    return;
  }
  Superclass::UpdateDisplayExtent();
  for(auto cit = this->Repository_.cbegin(); cit != this->Repository_.cend();++cit)
  {
    this->UpdateCutterPlane(cit->first);
  }
}

void vtkImageViewer2PolyData::AddPolyData(const std::string &tag, const std::string &id)
{
  auto cit = InternalStorage_.find(tag);
  if(cit == InternalStorage_.cend())
  {
    return false;
  }
  auto cit2 = this->Repository_.find(id);
  if(cit2 == this->Repository_.cend())
  {
    Tuple tuple(nullptr, Ptr<vtkPolyDataMapper>::New(), Ptr<vtkActor>::New());
    this->DecoratorRenderer->AddActor(std::get<2>(tuple));
    std::get<2>(tuple)->SetMapper(std::get<1>(tuple));
    cit2 = this->Repository_.emplace(id, tuple).first;
  }
  std::get<0>(cit2->second) = cit->second;
  auto plane = vtkSmartPointer<vtkPlane>::New();
  vtkWeakPointer<vtkPlane> weakPlane = plane;
  this->Planes_.emplace(id, weakPlane);
  auto cutter = vtkSmartPointer<vtkCutter>::New();
  cutter->SetInputData(cit->second);
  cutter->SetCutFunction(plane);
  auto stripper = vtkSmartPointer<vtkStripperPolygon>::New();
  stripper->SetInputConnection(cutter->GetOutputPort());
  auto triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
  triangleFilter->SetInputConnection(stripper->GetOutputPort());
  std::get<1>(cit2->second)->SetInputConnection(triangleFilter->GetOutputPort());
  return true;
}

void vtkImageViewer2PolyData::RemovePolyData(const std::string &id)
{
  if(Superclass::RemovePolyData(id))
  {
    auto cit = this->Planes_.find(id);
    if(cit == this->Planes_.cend())
    {
      return false;
    }
    this->Planes_.erase(cit);
    return true;
  }
}

void vtkImageViewer2PolyData::InstallPipeline()
{
  vtkSmartPointer<vtkRenderer> overlay = 
    vtkSmartPointer<vtkRenderer>::New();
  overlay->SetLayer(1);
  overlay->SetActiveCamera(this->GetRenderer()->GetActiveCamera());
  this->GetRenderer()->SetLayer(0);
  this->GetRenderWindow()->AddRenderer(overlay);
  this->GetRenderWindow()->SetNumberOfLayers(2);
  Superclass::InstallPipeline();
  this->Install(overlay);
}

void vtkImageViewer2PolyData::UnInstallPipeline()
{
  this->GetRenderWindow()->SetNumberOfLayers(1);
  this->GetRenderWindow()->RemoveRenderer(this->DecoratorRenderer);
  this->Uninstall();
  Superclass::UnInstallPipeline();
}

bool vtkImageViewer2PolyData::UpdateCutterPlane(const std::string &id)
{
  auto cit1 = this->Planes_.find(id);
  auto cit2 = this->Repository_.find(id);
  if(cit1 == this->Planes_.cend() || cit2 == this->Repository_.cend())
  {
    std::cerr << "id: " << id << " does not exist. \n";
    return false;
  }
  vtkActor *actor = std::get<2>(cit2->second);
  vtkSmartPointer<vtkTransform> transform = 
    vtkSmartPointer<vtkTransform>::New(); 
  transform->SetMatrix(actor->GetMatrix());
  double normal[3]{0};
  double origin[3]{0};
  const double *origin = this->GetInput()->GetOrigin();
  const double *spacing = this->GetInput()->GetSpacing();
  normal[this->SliceOrientation] = 1;
  origin[this->SliceOrientation] =
    (this->Slice * spacing[this->SliceOrientation]) + origin[this->SliceOrientation];
  vtkPlane *plane = cit1->second;
  plane->SetOrigin(origin);
  plane->SetNormal(normal);
  plane->SetTransform(transform);
  return true;
}