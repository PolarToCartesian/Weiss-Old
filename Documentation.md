# Documentation
---

## Introduction

Weiss often uses descriptors which are structs for object creation 

## Overview Of Classes

### ```class EngineCore```

It is the rendering engine which implements basic directx and winapi abstraction. It can create windows and handle events. It can issue render calls with indexed and non-indexed rendering. It stores all of your shaders, buffers, textures and texture samplers.

### ```class Engine```

Fully abstracted Engine Class for 2d and 3d rendering which inherits from ```class EngineCore```. It uses batch renderers and has support for lighting, texturing, coloring and texturing.

### ```class Window```

Implements window creation abstraction and handles window events.

### ```class Mouse```

Implements an interface for getting information about the mouse and handling / catching mouse related events.

### ```class Keyboard```

Implements an interface for getting information about the keyboard and handling / catching keyboard related events.

### ```class OrthographicCamera```

Implements an abstracted Orthographic Camera which has support for translation, rotation and keyboard input and window resizing.

### ```class PerspectiveCamera```

Implements an abstracted Perspective Camera which has support for translation, rotation, keyboard input, mouse input and window resizing.

### ```class IndexBuffer```

Implements IndexBuffer Abstraction.

### Same for ```class VertexBuffer```, ```class ConstantBuffer```, ```class VertexShader```, ```class PixelShader```
