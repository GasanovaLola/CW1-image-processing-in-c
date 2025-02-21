# F28HS Coursework 1  
## HPDEC Image Processing Project  

## Introduction  
This project is a **C-based image processing tool** designed to work with **HPDEC format images**. It allows users to:  
- **Load and save HPDEC images**  
- **Compare two images pixel-by-pixel**  
- **Apply a 3×3 blur effect**  
- **Modify and analyze images using multiple blur passes**  

The program operates via the command line and processes images efficiently using **C standard libraries**. It also provides **conversion support** to view HPDEC images in standard formats (e.g., PPM).  

---

## Installation  

### **1️ Clone the repository**  
```sh
git clone https://github.com/your-username/hpdec-image-processing.git
cd hpdec-image-processing
```

### **2️ Compile the program**  
Use `gcc` to compile the `process.c` file:  
```sh
gcc -o process process.c
```
*(Make sure `gcc` is installed: `sudo apt install gcc` if needed.)*  

---

## Usage  

The program requires **three arguments**:  
```sh
./process REFERENCEFILE INPUTFILE OUTPUTFILE
```
| Argument | Description |
|----------|------------|
| `REFERENCEFILE` | Original image (for comparison) |
| `INPUTFILE` | The image to be processed |
| `OUTPUTFILE` | The result after processing |

---

## Example Usage  

### **1️ Apply a Basic Blur to an Image**  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/blurred_output.hpdec
```
**What happens?**  
 - The program loads `music.hpdec`  
 - Applies **one pass of 3×3 blur**  
 - Saves the blurred image as `blurred_output.hpdec`  

---

### **2️ Apply Multiple Blur Passes (Stronger Blur Effect)**  
Each run **further blurs** the previous output:  
```sh
./process HPDEC/blurred_output.hpdec HPDEC/blurred_output.hpdec HPDEC/double_blurred_output.hpdec
./process HPDEC/double_blurred_output.hpdec HPDEC/double_blurred_output.hpdec HPDEC/triple_blurred_output.hpdec
./process HPDEC/triple_blurred_output.hpdec HPDEC/triple_blurred_output.hpdec HPDEC/quadro_blurred_output.hpdec
```
**What happens?**  
- The image is **progressively blurred** over multiple iterations.  
- The more times the blur is applied, the **smoother the effect**.  
- Stronger blur makes **edges softer and noise reduced**.  

---

### **3️ Compare Two Images**  
To check **differences** between an original and modified image:  
```sh
./process HPDEC/music.hpdec HPDEC/modified_music.hpdec HPDEC/comparison.txt
```
**Expected output example:**  
```
Identical pixels: 73435  
Different pixels: 5
```
This means **only 5 pixels changed** between the images.

---

### **4️ Modify an Image (For Testing)**  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/modified_output.hpdec
```
- Loads `music.hpdec`  
- Modifies the **first 5 pixels** (adds +50 to red component)  
- Saves as `modified_output.hpdec`  

Then, compare using:  
```sh
./process HPDEC/music.hpdec HPDEC/modified_output.hpdec HPDEC/comparison.txt
```
*(This will print how many pixels are identical/different.)*

---

### **5️ Convert HPDEC to PPM for Viewing**  
HPDEC images cannot be viewed directly, so convert them to **PPM format**:  
```sh
./hpconvert HPDEC/triple_blurred_output.hpdec HPDEC/triple_blurred_output.ppm
xdg-open HPDEC/triple_blurred_output.ppm
```
*(Make sure `xdg-open` is installed, or use an alternative image viewer.)*

---

## Code Overview  

### **Main Functions**  
| Function | Purpose |
|----------|---------|
| `load_image()` | Loads an image from an HPDEC file |
| `save_image()` | Saves an image to an HPDEC file |
| `apply_BLUR()` | Applies a **3×3 blur** to an image |
| `apply_COMP()` | Compares two images pixel by pixel |
| `copy_image()` | Creates an exact copy of an image |

---

## Troubleshooting  

### **1️ Program Won't Run?**  
Try **compiling again**:  
```sh
gcc -o process process.c
```
Check if you **provided the correct arguments**:  
```sh
./process REFERENCEFILE INPUTFILE OUTPUTFILE
```

---

### **2️ Blurred Image Looks Too Sharp?**  
Try running the **blur multiple times**:  
```sh
./process HPDEC/music.hpdec HPDEC/music.hpdec HPDEC/blurred_output.hpdec
./process HPDEC/blurred_output.hpdec HPDEC/blurred_output.hpdec HPDEC/double_blurred_output.hpdec
./process HPDEC/double_blurred_output.hpdec HPDEC/double_blurred_output.hpdec HPDEC/triple_blurred_output.hpdec
```

---

### **3️ How to Convert HPDEC to PPM (for viewing)?**  
```sh
./hpconvert HPDEC/triple_blurred_output.hpdec HPDEC/triple_blurred_output.ppm
xdg-open HPDEC/triple_blurred_output.ppm
```
