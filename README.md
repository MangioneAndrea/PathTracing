# ComputerGraphicsSamples

Repo containing many mini projects for very simple rendering with cpp

## Requisites

To run any of the projects, you need to download [SDL](https://www.libsdl.org/download-2.0.php) and put the `.dll`
and `.lib` files into a `lib` folder in the root and all the include files into the `inc` folder

## Content

### Empty

This program is the draft for other projects, as it contains all the boilerplate

### ColorsShowdown

This program creates a window drawing a emissionColor palette pixel by pixel

On the top side it's just a linear RGB palette. On the bottom side it's a gamma adjusted sRGB palette

![ColorsShowdown.png](./imgs/ColorsShowdown.png)

### Cornell Box

This program creates a window drawing a Cornell box with ray tracing and simple spheres logic
![img.png](./imgs/Cornell.png)

### Raytracing v2 Bouncing light

This is an extension of the previous program, this time each pixel bounces till it finds a light source. This render has
4096 rays per pixel
![img.png](./imgs/BouncingLight.png)