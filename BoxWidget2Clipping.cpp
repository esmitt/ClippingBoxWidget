#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkBoxRepresentation.h>
#include <vtkDataSetMapper.h>
#include <vtkCallbackCommand.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkBox.h>
#include <vtkClipPolyData.h>
#include <vtkPlanes.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSphereSource.h>
#include <vtkBoxWidget2.h>

int main(int argc, char *argv[])
{
  //box widget
  vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();

  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetThetaResolution(32);
  sphereSource->SetPhiResolution(32);
  sphereSource->Update();

  // storing planes of the bounding box
  vtkSmartPointer<vtkPlanes> planesClipping = vtkSmartPointer<vtkPlanes>::New();
  vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planesClipping);

  // clipping structure
  vtkSmartPointer<vtkClipPolyData> clipperPoly = vtkSmartPointer<vtkClipPolyData>::New();
  clipperPoly->SetInputConnection(sphereSource->GetOutputPort()); //also clipperPoly->SetInputData(sphereSource->GetOutput());
  clipperPoly->SetClipFunction(planesClipping);

  //mapper
  vtkSmartPointer<vtkDataSetMapper> selectMapper = vtkSmartPointer<vtkDataSetMapper>::New();
  selectMapper->SetInputConnection(clipperPoly->GetOutputPort());
  selectMapper->Update();

  //actor
  vtkSmartPointer<vtkActor> selectActor = vtkSmartPointer<vtkActor>::New();
  selectActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
  selectActor->SetMapper(selectMapper);

  // Create graphics stuff
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(.2, .2, .25);

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);
  renWin->SetSize(512, 512);

  // window interactor
  vtkSmartPointer<vtkRenderWindowInteractor> windowInteractor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  windowInteractor->SetRenderWindow(renWin);

  //the box widget
  boxWidget->SetInteractor(windowInteractor);
  boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
  boxWidget->GetRepresentation()->PlaceWidget(selectActor->GetBounds());
  boxWidget->On();

  //callback function
  auto interactionFcn = [](vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
    vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkBoxWidget2::SafeDownCast(caller);
    vtkSmartPointer<vtkPlanes> planesClipping = vtkSmartPointer<vtkPlanes>::New();
    vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planesClipping);

    //get the planes
    vtkClipPolyData* clipperPoly = static_cast<vtkClipPolyData*>(clientData);
    clipperPoly->SetClipFunction(planesClipping);
    clipperPoly->InsideOutOn();
    clipperPoly = nullptr;
  };
  vtkSmartPointer<vtkCallbackCommand> interactionCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  interactionCallback->SetCallback(interactionFcn);
  interactionCallback->SetClientData(clipperPoly);  //pass the clipper to update it
  boxWidget->AddObserver(vtkCommand::InteractionEvent, interactionCallback);

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