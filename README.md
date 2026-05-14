# Simple C++ Ray Tracer

![Example Image](./img/world.png)

A C++ implementation of a ray tracer with no external dependencies. 
## Features

- Reflection & Refraction
- Shadows
- Materials
- Transformations
- Planes and Spheres

## Installation & Usage

[CMake](https://cmake.org/) based project which can be built via command line or with your favourite IDE.

```bash
# Download the project
git clone https://github.com/richhaar/raytracer.git
cd raytracer
```

```bash
# Generate the project
cmake -S . -B bin -G Ninja
cd bin
```

```bash
# Build
ninja
# Run
./RayTracer
```

Tested and developed on Ubuntu 24.04 LTS with an Intel Pentium N5030 CPU @ 1.10GHz machine.

Taking 30s for a 2000x1000 4xMSAA image with `kMaxReflections` set to 5.

## License

MIT

## Acknowledgements

[*The Ray Tracer Challenge*](http://raytracerchallenge.com/) by Jamis Buck.