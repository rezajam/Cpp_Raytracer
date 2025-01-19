# C++ Raytracer 

# Ray Tracing Project (So Far) 

This project implements the foundational steps of a ray tracer. It generates a gradient image and outputs it in the PPM format. The code includes basic vector math (`vec3`) and color utilities for RGB representation.

---

## **Setup Instructions**

### **Requirements**
- A C++ compiler (e.g., `g++`).
- Basic command-line knowledge.

### **Clone/Download the Code**
- Ensure `main.cc`, `vec3.h`, and `color.h` are in the same directory.

---

## **Compilation**

To compile the program, run:
```bash
g++ -o raytracer main.cc
```

## **Run the Program**

To generate the `.ppm` image file::
```bash
./raytracer > output.ppm
```
This will create a `output.ppm` file in the same directory.

Use any ppm viewer to see the output image

