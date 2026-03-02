# Real-Time Rasterizer

<img src="https://github.com/user-attachments/assets/2ac6b513-6b16-4327-b350-3d3c465517d0"/>

## Table of Contents
- [Description](#description)
- [Learning Objectives](#learning-objectives)
- [Features](#features)
- [Rendering Stages & Screenshots](#rendering-stages--screenshots)
- [Controls](#controls)
- [Installation](#installation)
- [Conclusion](#conclusion)

## Description
A fully CPU-based real-time rasterizer written from scratch using the Win32 API.  
No external graphics libraries — no OpenGL, DirectX, or Vulkan. Everything is implemented manually, including the rendering pipeline, memory management, and math library.

The renderer walks step-by-step through the complete graphics pipeline.

- Press **T** to cycle through rendering stages (cyclical)
- Press **Space** to switch between loaded models

### Tools
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

## Learning Objectives
The goal of this project is to deeply understand how modern graphics pipelines work by reconstructing them entirely in software.


## Features
### Rendering Pipeline (From Scratch)
- Wireframe triangle rasterization
- Filled triangle rasterization
- Back-face culling
- Z-buffer depth testing
- Perspective projection
- MVP matrix (Model → View → Projection)
- Clip space → NDC → Screen space conversion
- Frustum culling (triangles outside the camera frustum are discarded)

### Shading & Lighting
- Per-triangle normals (flat shading)
- Per-pixel normals via barycentric interpolation (smooth shading)
- Phong lighting model
- Diffuse mapping
- Specular mapping
- Global-space normal mapping

### Mathematics & Transformations
- Handmade linear algebra library (no dependencies)
- Barycentric coordinates for:
  - Inside-triangle tests
  - Normal interpolation
  - Depth interpolation
- Change of basis (camera space ↔ world space)
- Model transforms (position, rotation, scale)
- Rotation order: XYZ

### Memory & Architecture
- Custom memory arena allocator:
  - Permanent memory
  - Temporary memory
- Manual memory management
- Simple asset manager for loading `.obj` models
- All model data loaded into permanent memory
- Parallelized rasterization using `#pragma omp parallel for`

### Platform Layer
- Win32 API
- Software backbuffer rendering
- Display using `StretchDIBits`
- Custom Windows message loop
- No graphics APIs

## Rendering Stages & Screenshots
The renderer is a learning project — each stage exposes a deeper level of the graphics pipeline.

### Wireframe Rendering
<p align="center">
  <img src="https://github.com/user-attachments/assets/14e7c024-e081-493e-97b8-a1cefa56506b" width="48%" />
  <img src="https://github.com/user-attachments/assets/7405e2cf-4a10-4522-b818-101c158797f9" width="48%" />
</p>

### Filled Triangles (Random Colors)
<p align="center">
  <img src="https://github.com/user-attachments/assets/48eb0eaf-d3ff-4611-a040-d75f2480fd76" width="48%" />
  <img src="https://github.com/user-attachments/assets/6f47dd1c-aa57-4d1a-9cb2-4c0b7a5fed50" width="48%" />
</p>

### Flat Shading (Per-Triangle Normals)
<p align="center">
  <img src="https://github.com/user-attachments/assets/f4ccb16d-2cbd-487b-a277-9ff0647004b0" width="48%" />
  <img src="https://github.com/user-attachments/assets/7a047878-c208-4e66-9de8-e3e3dec26a19" width="48%" />
</p>

### Smooth Shading (Per-Pixel Normals via Barycentric Interpolation)
<p align="center">
  <img src="https://github.com/user-attachments/assets/1d078383-c352-4b0d-8255-f9e0ca303e7a" width="48%" />
  <img src="https://github.com/user-attachments/assets/81591b1c-2be2-47af-8f64-9e490f427b53" width="48%" />
</p>

### Normal Mapping
<p align="center">
  <img src="https://github.com/user-attachments/assets/da846b37-19c6-4d4b-af00-76a27fc0a8b1" width="48%" />
  <img src="https://github.com/user-attachments/assets/8d518300-42c5-4c58-b6e0-ddd1266ec94c" width="48%" />
</p>

### Diffuse + Specular Mapping (Final Result)
<p align="center">
  <img src="https://github.com/user-attachments/assets/12dabcac-1950-4f3b-b9ab-ba6acb74957c" width="48%" />
  <img src="https://github.com/user-attachments/assets/cd08e1bb-1566-4355-88f9-3eba7888c92e" width="48%" />
</p>

## Controls
- **T** → Advance to next rendering stage (cyclical)
- **Space** → Switch models
- **WASD** → Camera movement
- **CTRL + Mouse** → Camera rotation

## Installation
### Pre-Compiled Windows amd64
[v0.1.0 Release](https://github.com/WhoSayin52/rasterizer/releases/download/v0.1.0/rasterizer.zip)
- Download the zip folder
- Extrace files
- Run excutable inside `bin/` folder

---

### Build From Source
#### Prerequiste
Before building the project, make sure you have the following installed:

- **CMake** (version 4.2 or higher recommended)
- **MSVC Compiler** (Visual Studio 2022 or newer)
- **Git** (for cloning the repository)

#### Steps
Follow these steps to clone and build the project:

1. **Open the Developer Shell on Windows**  
   This ensures all necessary compiler tools are available.
   
3. Clone the repository: 
```bash
git clone <repo-url>
cd ./rasterizer
```
3. Build the project:
```bash
cmake --preset default
cmake --build build --config Release # or Debug if desired
```
4. Run the project:
```bash
cd ./build/Release # or Debug
explorer .
```

## Conclusion
While there is still plenty of room for improvement, this project successfully achieved its goal of building a strong foundation in graphics programming and understanding the graphics pipeline.

<img src="https://github.com/user-attachments/assets/369c4f2b-43f5-4b97-9ba6-f51f28e8586e"/>


