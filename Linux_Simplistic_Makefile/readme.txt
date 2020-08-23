DO NOT USE THIS LONGTERM
See readme.txt in parent directory.
You can run this to check your installation if you wish - copy it to your src directory and hopefully it will just compile everything if you type 'make'.

First install SDL2 devel, SDL ttf devel and SDL image devel, to get the libraries to link against installed. 
For example on Ubuntu(and derivatives) you use "sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev"

Next copy the makefile from this directory into the same directory as the source files (../src if you haven't moved anything).

Finally, in a terminal move to the source directory (src) and run 'make'

If it complains about missing libraries, then the libraries libsdl2-dev, libsdl2-image-dev and libsdl2-ttf-dev need to be installed - see step 1.
Install these dependencies according to your Linux installation. 

Feel free to change the first line (SDL_DIR = ../SDL/include) to point to the directory where your system SDL header files are included if you install different versions to the ones I used.




