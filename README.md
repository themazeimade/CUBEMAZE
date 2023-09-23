** IMPORTANT RUN THE COMPILE.BAT TO COMPILE SHADERS**
GLSL required (bundled with Vulkan SDK)

# CUBEMAZE
Explorable 3d maze generator

When executing the program, first you need to type the size of the maze on the console. Then, with escape key you can deactivate/or active the camera and use the ImGui stuff (ensure you click on the viewport in order the escape key to work) . There you can manually move the camera, see how gravity works, move the maze, and go to a freecamera mode where you can float

## Compile it
Only tested on windows but 100% sure it works on linux (Vulkan FTW)
if you are crazy like me use the MinGW64 makefiles, otherwise make the visual studio build files.
If the project uses vulkan, make sure you have installed the SDK and set as an enviroment variable in your OS system/session.
Also the project needs the GLFW include files and binaries, you need to compile them. Down below are the instructions. Finally, run the compile.bat on the main directory to compile the shaders needed to run the project.
<pre>
// GLFW compilation
  cd dependencies/glfw
  mkdir build
  cd build
  //either
  cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Debug // or Release
  // or
  cmake -G "Visual Studio 16" -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF ..
  //finally
  cmake --build --config Debug // or Release
  cd ../../../
// Main executable
  mkdir build
  cd build
  //either
  cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Debug // or Release
  //or
  cmake -G "Visual Studio 16" ..
  //finally
  cmake --build . --config Debug // or Release
</pre>
</pre>

## Known issues
Jumping doesn't work :( 
