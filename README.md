# F28HS Coursework 1  
## HPDEC Image Processing Project  

### Introduction  
This project is a **C-based image processing tool** specifically designed to handle **HPDEC format images**. The program provides functionalities to:  
- **Load and save HPDEC images**  
- **Perform pixel-by-pixel comparison between images**  
- **Apply a 3×3 blur filter**  
- **Modify and analyze images through multiple blur passes**  

The software operates via a command-line interface, ensuring efficient image processing using **C standard libraries**. Additionally, it supports **conversion** of HPDEC images into standard formats (e.g., PPM) for visualization.  

---

## Installation  

### **1. Clone the Repository**  
```sh
git clone https://github.com/GasanovaLola/CW1-image-processing-in-c.git
cd CW1-image-processing-in-c
```

### **2. Compile the Program**  
Use `gcc` to compile the `process.c` file:  
```sh
gcc -o process process.c
```
*(Ensure that `gcc` is installed by running: `sudo apt install gcc` if necessary.)*  

---

## Usage  

The program requires **three arguments** in the following format:  
```sh
./process REFERENCEFILE INPUTFILE OUTPUTFILE
```
| Argument | Description |
|----------|------------|
| `REFERENCEFILE` | The original image used for comparison |
| `INPUTFILE` | The image to be processed |
| `OUTPUTFILE` | The output file containing the processed image |

---

## Example Commands  

### **1. Apply a Basic Blur to an Image**  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/blurred_output.hpdec
```
**Process Overview:**  
 - Loads `music.hpdec`  
 - Applies a **single pass of 3×3 blur**  
 - Saves the blurred image as `blurred_output.hpdec`  

---

### **2. Apply Multiple Blur Passes for a Stronger Effect**  
To **progressively blur** an image, apply the filter multiple times:  
```sh
./process HPDEC/blurred_output.hpdec HPDEC/blurred_output.hpdec HPDEC/double_blurred_output.hpdec
./process HPDEC/double_blurred_output.hpdec HPDEC/double_blurred_output.hpdec HPDEC/triple_blurred_output.hpdec
```
**Effect:**  
- Successive applications of the blur function produce a **smoother and more blended image**.  

---

### **3. Compare Two Images for Differences**  
To detect **changes** between an original and a modified image:  
```sh
./process HPDEC/music.hpdec HPDEC/modified_music.hpdec HPDEC/comparison.txt
```
**Example Output:**  
```
Identical pixels: 73435  
Different pixels: 38743  
```
This indicates that **five pixels** have been altered.  

---

### **4. Modify an Image (Testing Purposes)**  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/modified_output.hpdec
```
- Loads `music.hpdec`  
- Adjusts the **red component of the first five pixels**  
- Saves as `modified_output.hpdec`  

To verify the changes:  
```sh
./process HPDEC/music.hpdec HPDEC/modified_output.hpdec HPDEC/comparison.txt
```

---

### **5. Convert HPDEC to PPM for Viewing**  
HPDEC images are not directly viewable. To convert them into **PPM format**:  
```sh
./hpconvert HPDEC/triple_blurred_output.hpdec HPDEC/triple_blurred_output.ppm
xdg-open HPDEC/triple_blurred_output.ppm
```
*(Ensure `xdg-open` is installed, or use an alternative image viewer.)*  

---

## Code Overview  

### **Main Functions**  
| Function | Purpose |
|----------|---------|
| `load_image()` | Loads an image from an HPDEC file |
| `save_image()` | Saves an image to an HPDEC file |
| `apply_BLUR()` | Applies a **3×3 blur filter** to an image |
| `apply_COMP()` | Performs pixel-by-pixel comparison of two images |
| `copy_image()` | Creates a deep copy of an image |

---

## Troubleshooting  

### **1. Program Fails to Run?**  
Recompile the program:  
```sh
gcc -o process process.c
```
Ensure you are providing the correct command-line arguments:  
```sh
./process REFERENCEFILE INPUTFILE OUTPUTFILE
```

---

### **2. Blurred Image Appears Too Sharp?**  
Apply the blur **multiple times**:  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/blurred_output.hpdec
./process HPDEC/blurred_output.hpdec HPDEC/blurred_output.hpdec HPDEC/double_blurred_output.hpdec
./process HPDEC/double_blurred_output.hpdec HPDEC/double_blurred_output.hpdec HPDEC/triple_blurred_output.hpdec
```

---

### **3. How to Convert HPDEC to PPM for Viewing?**  
```sh
./hpconvert HPDEC/triple_blurred_output.hpdec HPDEC/triple_blurred_output.ppm
xdg-open HPDEC/triple_blurred_output.ppm
```

---

## Conclusion  
This project provides an efficient way to **process, modify, compare, and convert HPDEC images**. With its straightforward command-line interface, users can easily manipulate images for analysis and visualization.

