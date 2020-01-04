# Weiss

> Weiss is a 2D and 3D rendering engine for windows built with DirectX in c++17. It is intended to be a wrapper around low-level apis to make the developpement of 2D and 3D applications simpler and faster.

---

## Usage

Clone the repository with [Git](https://git-scm.com/)

```
git clone "https://github.com/PolarToCartesian/Weiss"
```

The last time you include Weiss, please `#define __WEISS_LAST_INCLUDE`.

While debugging, you can `#define __WEISS_SHOW_DEBUG_ERRORS` to get pop-ups when errors occur. If you want to have the console you can use the `ENABLE_CONSOLE()` function macro.

Fore more information, visit the wiki and/or checkout my [Minecraft Clone](https://github.com/PolarToCartesian/Minecraft)

Example Application :

```c++
#define __WEISS_SHOW_DEBUG_ERRORS
#define __WEISS_LAST_INCLUDE

#include "../engine/Include.h"

class Sandbox : public Engine
{
public:
	Sandbox(HINSTANCE hInstance)
	{
		EngineDescriptor ed = {
			WindowDescriptor { 0u, 0u, 1920u, 1080u, "Weiss Demo", nullptr, true, hInstance },
			OrthographicCameraDescriptor { Vec2f{ 0.0f, 0.0f        }, 0.0f },
			PerspectiveCameraDescriptor  { Vec3f{ 0.0f, 20.0f, 0.0f }, Vec3f{}, HALF_PI, 0.01f, 1000.f }
		};

		this->InitEngine(ed);
		this->PlayWavFile("res/sounds/soundtrack.wav");
	}

	virtual void OnRender(const float elapsed) override
	{
		this->Fill(255, 0, 255);
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ENABLE_CONSOLE();

	Sandbox sandbox(hInstance);
	sandbox.Run(false, 30);
}
```

## Compiling

Remember to set your subsystem to windows. If you are using the visual studio developer command prompt and cl.exe, specify `/link /SUBSYSTEM:WINDOWS`

Example : `cl Source.cpp /link /SUBSYSTEM:WINDOWS`

---

## Features

### Graphics

#### Cameras

+ [x] Perspective
+ [x] Orthographic

#### Lighting

+ [ ] Point Light
+ [ ] Directional Light
+ [ ] Materials
+ [ ] Shadows

#### Abstract Renderers

+ [ ] 2D Abstracted Renderer
+ [ ] 3D Abstracted Renderer

#### Misc

+ [x] 2D Texturing
+ [ ] Post Processing
+ [ ] Multithreaded Rendering

### Audio

+ [x] Play
+ [ ] Pause
+ [ ] Volume Control

### Networking

+ [x] Sockets
   + [x] Client Sockets
  + [x] Server Sockets
+ [ ] HTTP
+ [ ] HTTPS
