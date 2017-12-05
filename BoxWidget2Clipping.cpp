#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkBoxRepresentation.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>
#include <vtkPlanes.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPlaneCollection.h>
#include <vtkSphereSource.h>
#include <vtkBoxWidget2.h>

// Callback for the interaction
class vtkBoxWidgetCallback : public vtkCommand
{
public:
  static vtkBoxWidgetCallback *New()
  {
    return new vtkBoxWidgetCallback;
  }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
  {
    vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkBoxWidget2::SafeDownCast(caller);
    vtkSmartPointer<vtkPlanes> planes = vtkSmartPointer<vtkPlanes>::New();
    vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planes);

    vtkSmartPointer<vtkPlaneCollection> planesColl = vtkSmartPointer<vtkPlaneCollection>::New();
    planesColl->AddItem(planes->GetPlane(0));
    planesColl->AddItem(planes->GetPlane(1));
    planesColl->AddItem(planes->GetPlane(2));
    planesColl->AddItem(planes->GetPlane(3));
    planesColl->AddItem(planes->GetPlane(4));
    planesColl->AddItem(planes->GetPlane(5));
    m_clipPolyData->SetClipFunction(planes);
    m_clipPolyData->InsideOutOn();
  }
  vtkBoxWidgetCallback() {}

  vtkSmartPointer<vtkClipPolyData> m_clipPolyData;
};

int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();

  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetThetaResolution(32);
  sphereSource->SetPhiResolution(32);
  sphereSource->Update();

  vtkSmartPointer<vtkPlanes> planesClipping = vtkSmartPointer<vtkPlanes>::New();
  vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planesClipping);

  vtkSmartPointer<vtkClipPolyData> clipperPoly = vtkSmartPointer<vtkClipPolyData>::New();
  clipperPoly->SetInputConnection(sphereSource->GetOutputPort()); //also clipperPoly->SetInputData(sphereSource->GetOutput());
  clipperPoly->SetClipFunction(planesClipping); 

  vtkSmartPointer<vtkDataSetMapper> selectMapper = vtkSmartPointer<vtkDataSetMapper>::New();
  selectMapper->SetInputConnection(clipperPoly->GetOutputPort());
  selectMapper->Update();

  vtkSmartPointer<vtkActor> selectActor = vtkSmartPointer<vtkActor>::New();
  selectActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  selectActor->SetMapper(selectMapper);

  // Create graphics stuff
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(.2, .2, .25);

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);
  renWin->SetSize(512, 512);

  vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  windowInteractor->SetRenderWindow(renWin);

  boxWidget->SetInteractor(windowInteractor);
  boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
  boxWidget->GetRepresentation()->PlaceWidget(selectActor->GetBounds());
  boxWidget->On();

  vtkSmartPointer<vtkBoxWidgetCallback> myCallback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
  myCallback->m_clipPolyData = clipperPoly;
  boxWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);

  // Add the actor
  ren1->AddActor(selectActor);

  // Generate a camera
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> cameraStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  windowInteractor->SetInteractorStyle(cameraStyle);
  ren1->ResetCamera();

  windowInteractor->Initialize();
  windowInteractor->Start();

  return EXIT_SUCCESS;
}