## pigpen-cpp

PigPen (dot and boxes) in C++ using SDL2

![PigPen](https://raw.githubusercontent.com/gdonald/pigpen-cpp/master/pigpen.png)

### Install Build Dependencies

#### Mac OS

Install Xcode from the app store, and homebrew, then:

    brew install cmake sdl2 sdl2_gfx sdl2_ttf sdl2_image
    
#### Debian Linux

    apt install build-essential cmake clang \
                libsdl2-dev libsdl2-gfx-dev \
                libsdl2-ttf-dev libsdl2-image-dev

### Build
    cmake .
    make

### Run
    ./pigpen
