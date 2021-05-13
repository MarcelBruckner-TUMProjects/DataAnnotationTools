# DataAnnotationTools
A collection of data annotation tools I use during my projects.

# Dependencies
- OpenCV with CUDA support
- Boost library

# Standalones
## Run the watershed algorithm to segment images using markers
```shell 
cd cpp
mkdir build && cd build
cmake ..
cmake --build . -j8
./standalone/Watersheder -h
```
