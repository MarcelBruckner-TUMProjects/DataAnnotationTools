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

## Running

<details>
<summary>Watersheder</summary>

The WaterSheder is an marker based image segmentation tool.

- [The OpenCV description](https://docs.opencv.org/master/d3/db4/tutorial_py_watershed.html)
- [A great overview over the origin](http://www.cmm.mines-paristech.fr/~beucher/wtshed.html)

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
|`n`| Quick zoom into the image to see the enlarged region around your mouse location. |
|`q`| Quit program (Without confirmation). |
|`r`| Toggle to render the watershed mask. |
|`s`| Save the watershed results as a YAML file specified by `-o/--output`. |
|`w`, `SPACE`| Run the watershed algorithm based on the current markers. |
|`0` … `9`  | Set stroke thickness of the left mouse button. |

[comment]: <> (|``|  |)

</details>

***

## Testing

```shell
cd cpp 
ctest --verbose
```

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
