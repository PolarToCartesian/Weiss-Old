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

class Sandbox : public Engine {
public:
	Sandbox(HINSTANCE hInstance) {
		this->InitEngineCore(WindowDescriptor{ 0, 0, 1920, 1080, "Weiss Demo", nullptr, false, hInstance });

		LightingDescriptor ld{ Colorf32{1.f,1.f,1.f,1.f} };

		OrthographicCameraDescriptor ocd { Vec2f{0.f, 0.f}, 0.f };
		PerspectiveCameraDescriptor  pcd { Vec3f{0.f, 0.f, 0.f}, Vec3f{0.f, 0.f, 0.f}, HALF_PI, 0.1f, 1000.f };

		this->InitEngine(EngineDescriptor{ 
			Engine2DDescriptor { ocd, ld }, 
			Engine3DDescriptor { pcd, ld }
		});
	}

	// Called Each Frame
	virtual void OnRender(const float elapsed) override {
		// Fill With White
		this->Fill(Coloru8 { 255, 255, 255, 0 });
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
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
