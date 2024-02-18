# FGST_v0.1 / FOREST GRAPHICAL SIMULATION TOOL

'FGST' stands for Forest Graphical Simulation Tool. It is an application that allows you to generate an environment and simulate plant populations growth in it. The idea came from my fascination with how people read the landscape and my investigation into forest ecosystem simulation in the French scientific community. Since that there was no way to figure out what was the tangible changes those scientists were talking about, i thought that it would be useful and didactic to create a tool that allows everyone to explore their way of reading forest landscape. It allows you to model a simulation by choosing variables such as species distributions, density, latitude etc. With the output staitstics and 3d rendering, it allows the user to understand further forestry choices and the impact of climate changes.


[FGST_1](https://github.com/BriacNaux/FGST/tree/master/images/FGST_1.png)

One of the other interests that i will try to develop in the future is to allow the user to take control of the variables of the environment, (not only the forest variables) and link them to external inputs or projects by hardware or software. Therefore, anyone with an artistic approach can create generative artwork linking forest 3d growth and sound for example or even performance.

The project is currently in development (V.0.1).

The project uses the OpenGL API to communicate with the GPU and the ImGUI library to create the GUI. Currently, the software includes a 3D viewport controlled by a speculative ecological simulation component. The features are for now :
- Very basics Phong lightings.
- Two modes : the edit mode and the game mode. It basically changes the way of controlling the camera.
- Two graphic modes : Line mode and Polygon mode.
- A simulation editor.

The logic behind the tree growth is based on a repetition of a Stem object that represents the single reproductive unit of a plant (the phytomer). Then those stems are live rendered and are driven by an apical meristematic growth before being split according to an architectural development class containing the rules. This way, everyone can create their own architecture classes. Those i am working on are inspired by Francis Hallé's research on tree architectures knowing Massart and Leeuwenberg models. It is similar to an L-system result but it is coded to allow a good amount of stochastic in the future.


