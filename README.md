## ClippingBoxWidget

A single project made in [VTK] to handle a `BoxWidget2` as a clipping box. It uses 6 planes which are manipulated using the widget. 

A [vtkClipPolyData] is used with a clip function defined as six `vtkPlanes`, extracted from the `vtkBoxWidget2`. Also, a callback (i.e. `vtkCommand::InteractionEvent` type) is create to update on real time the clipped area.

### Installation

Dillinger requires [Cmake] to create the project, using a GUI or a simple `cmake` command.

>To do: close the clipped area with new polydata


[VTK]: <https://www.vtk.org/>
[vtkClipPolyData]: <https://www.vtk.org/doc/nightly/html/classvtkClipPolyData.html>
[Cmake]: <https://cmake.org/>