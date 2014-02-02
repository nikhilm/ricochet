ricochet
========

[old-project] Use mirrors to guide the light to the switch and go to the next level. Python + Pygame

Requirements
------------

Install [SDL](http://libsdl.org). On Linux use your package manager. You'll need sdl-image and sdl-ttf (including the dev packages).

Install CMake.

Install
-------

    git clone git://github.com/nikhilm/ricochet.git
    cd ricochet
    cmake .
    make
    
Run
---

    ./ricochet.sh
    
    
- Click gun to start the photon.
- Walls rebound the photon.
- Normal mirrors reflect the photon 90 degrees, but destroy it when hit on the wrong side.
- Triangular mirrors reflect the photon 90 degrees, but let it pass through when hit on the wrong side.
- Arrows always guide the photon in the direction they point to.
- Triangular switches keep toggling on/off.
- The purple things energize the photon, so it will pass through a solid wall or another mirror (at which point it loses the energy).
