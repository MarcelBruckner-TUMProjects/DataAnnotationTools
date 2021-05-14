# Data Annotation Tools

This serves as a collection of data annotation tools for data labelling, i.e. image segmentation.

***

# Running in Standalone Mode

### See below for the necessary dependencies.

## Compiling

```shell
mkdir build && cd build
cmake ..
cmake --build . -j8
```

#### CMake Configure Options

```shell
# Default is OFF for all options

-DWITH_TESTS=ON/OFF     # Build with tests.
```

***

## Testing

```shell
cd cpp 
ctest --verbose
```

## Apps

<details>
<summary>WaterSheder</summary>
<br>

### A Marker-based Image Segmentation Tool

The algorithm fills the image from drawn marker locations until the image gradient surpasses a specific threshold. This
results in a segmentation around the border of objects with the same color.

To segment an image using the WaterSheder mark connected regions with the mouse. All connected markers form one region,
and you can extend a region by extending the marker. The biggest component needs to be the background, which itself must
have a marker region. This means that at least 2 markers need to be placed.

- [The OpenCV description](https://docs.opencv.org/master/d3/db4/tutorial_py_watershed.html)
- [A great overview over the origin](http://www.cmm.mines-paristech.fr/~beucher/wtshed.html)

![Example execution of the WaterSheder](https://github.com/Brucknem/DataAnnotationTools/blob/main/cpp/misc/watersheder.gif?raw=true)

### Running

```shell
./cpp/app/WaterSheder -h  # Prints the help message.
                          # See it for the required input data and further usage.
```

### Mouse Commands

| Key | Description |  
| ---------- | ----------- |
| `Left` | Draw markers. |

### Keyboard Commands

| Key | Description |  
| ---------- | ----------- |
|`b`| Quick zoom out of the image to see the full image. |
|`c`| Clear all markers in the current visible region. |
|`d`| Toggle delete mode. In delete mode left mouse button strokes remove marked pixels. |
|`n`| Quick zoom into the image to see the enlarged region around your mouse location. <br> The resulting zoom level is based on the `Quick Zoom` trackbar value. |
|`q`| Quit program (Without confirmation). |
|`r`| Toggle to render the watershed mask. |
|`s`| Save the watershed results as a YAML file specified by `-o/--output`. |
|`w`, `SPACE`| Run the watershed algorithm based on the current markers. |
|`0` … `9`  | Set stroke thickness of the left mouse button. |

### Sliders

| Key | Description |  
| ---------- | ----------- |
| `Pos X` | Sets the X pixel position of the top left corner of the visible region. |
| `Pos Y` | Sets the Y pixel position of the top left corner of the visible region. |
| `Zoom` | The current zoom level. 0 = Total zoomed in, 100 = Full image shown. |
| `Quick Zoom` | The zoom level after hitting `n`.  |
| `Thickness` | The thickness of the left mouse button stroke. |

### Best Practices

- Save often! There is no way to reload when the program was closed.
- Zoom in and out of the image with `n` and `b`.
- Repeatedly using `n` follows the mouse movement through the zoomed image.
- If the algorithm does not correctly detect the object border, try to roughly outline the object by extending the
  background marker.
- If you enclose a marker with a loop of another marker, the enclosed region of the inner marker will be part of the
  outer markers region.

</details>

***

# Dependencies

| Dependency | Usage | Installation | Required | 
| ---------- | ----------- | ------------ | -------- |
| [Boost](https://www.boost.org/) | Command line parsing | `sudo apt install libboost-all-dev` | Yes |
| [OpenCV](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) | Rendering during evaluation | `extern/setup_opencv.sh` <br><br> Please only run if you don't use OpenCV elsewhere. This might cause problems if you have your own OpenCV setup installed, as it compiles OpenCV with the minimal flags needed and overrides the system installation. | `-DWITH_OPENCV=ON` |

## Internal Dependencies

These dependencies are pulled by CMake when the project is built. You `do not` have to install them manually.

| Dependency | Usage | Required | 
| ---------- | ----------- | -------- |
| [YAML-CPP](https://github.com/jbeder/yaml-cpp.git) | YAML parser for objects and pixels | Yes |
| [GoogleTest](https://github.com/google/googletest) | Google unit testing framework | `-DWITH_TESTS=ON` |
