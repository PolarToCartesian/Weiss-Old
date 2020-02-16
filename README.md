# Weiss

> Weiss is a header-only rendering engine for windows built with DirectX 11 in c++17. It is intended to be a wrapper around low-level apis to make the developpement of 2D and 3D applications simpler and faster.

## Usage

Clone the repository with [Git](https://git-scm.com/)

The last time you include Weiss, please `#define __WEISS_LAST_INCLUDE`. While debugging, you can `#define __WEISS_SHOW_DEBUG_ERRORS` to get pop-ups when errors occur. If you want to have the console you can use the `ENABLE_CONSOLE()` function macro.

## Compiling

Remember to set your subsystem to windows. If you are using the visual studio developer command prompt and cl.exe, specify `/std:c++17 /link /SUBSYSTEM:WINDOWS`.

Example : `cl Source.cpp /std:c++17 /link /SUBSYSTEM:WINDOWS`

## Roadmap

| DirectX 11 Abstraction    | Status             | Window Abstraction       | Status             | Networking Abstraction | Status              |
| ------------------------- | ------------------ | ------------------------ | ------------------ | ---------------------- | ------------------- |
| Shader   Creation         | :heavy_check_mark: | Window   Creation        | :heavy_check_mark: | Socket Creation        | :heavy_check_mark:  |
| Buffer   Creation         | :heavy_check_mark: | Window   Getters         | :heavy_check_mark: | Send / Receive         | :heavy_check_mark:  |
| Image    Loading          | :heavy_check_mark: | Event    Handling        | :heavy_check_mark: | Disconnect             | :heavy_check_mark:  |
| Texture  Loading          | :heavy_check_mark: | Mouse    Getters         | :heavy_check_mark: | Bind / Accept          | :heavy_check_mark:  |
| Z-Buffer Creation         | :heavy_check_mark: | Keyboard Getters         | :heavy_check_mark: | HTTP Requests          | :x:                 |
| | | | | HTTPS Requests | :x:

| Renderer                | Status             | 2D Renderer            | Status             | 3D Renderer            | Status             |
| ----------------------- | ------------------ | ---------------------- | ------------------ | ---------------------- | ------------------ |
| Material System         | :heavy_check_mark: | Orthographic Camera    | :heavy_check_mark: | Perspective Camera     | :heavy_check_mark: |
| Scene Graph             | :x:                | 2D Object Abstraction  | :x:                | 3D Object Abstraction  | :x:                | 
| 2D-3D Switch            | :x:                | 2D Object Transforms   | :x:                | 3D Object Transforms   | :x:                |
|                         |                    | 2D Lighting            | :x:                | 3D Lighting            | :x:                |
|                         |                    | Batch Renderer         | :x:                | Batch Renderer         | :x:                |
| Post Processing         | :x:                | Sprite Sheet Generator | :heavy_check_mark: |                        |                    |
| Multithreaded Rendering | :x:                | 2D Animation           | :x:                | 3D Animation           | :x:                |