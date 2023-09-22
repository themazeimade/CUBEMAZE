# CUBEMAZE
Explorable 3d maze generator

When executing the program, first you need to type the size of the maze on the console. Then, with escape you can deactivate the camera and use the ImGui stuff. There you can manually move the camera, see how gravity works, move the maze, and go to a freecamera mode where you can float

## Compile it
Only tested on windows but 100% sure it works on linux (Vulkan FTW)
if you are crazy like me use the MinGW64 makefiles, otherwise make the visual studio build files.
If the project uses vulkan, make sure you have installed the SDK and set as an enviroment variable in your OS system/session.
<pre>
  mkdir build
  cd build
  //either
  cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Debug
  //or
  cmake -G "Visual Studio 15" .. -DCMAKE_BUILD_TYPE=Debug
  //finally
  cmake --build . --config Debug
</pre>

## Known issues
Jumping doesn't work :( 
