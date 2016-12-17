### Final Project

##### Description
This is the final project for the course. It is a single player car racing game with regular w-a-s-d key controls. Camera controls are q and z moving it up and down. There are other AI players created by the game engine to act as additional obstacle. It focuses on collision detection, forward animation and textures.

##### Setup Notes
Setting up this game is not straight forward. It has a dependency on an old havok sdk (2010). I got the sdk from here - http://pastebin.com/dqR4vR5x Note that this is not the full sdk and you get a limited set of APIs from here. I wanted to use the recently popularized open source library bullet but there were not enough examples to work its sdk out. SDK is not on the repository as it's too big.
For linux it involves compiling the sdk and building it. Its simple just run 'make all' and 'build' on linux shell. For Windows/Mac it should go along similar lines but build procedure would be slightly different. I did it on linux. Do setup havok if you want to run this project otherwise it won't compile. Also if running on Windows you need to change all the file paths. The loaders for .3ds, .obj, .tga, .xml and textures are from open source examples which were written specifically with 'glut' (linux implementation of opengl) in mind. As glut is almost the same as standard opengl library you get for Windows and Mac, I believe it should not cause any compatibility issues despite the differences. For example it uses default vertex shaders and fragment shaders defined in glext.h

You can check video in this folder to see the working if you're only interested in seeing the features. To run it do the setup as per notes and compile.
