# SuperBLT Plugin Template

A basic CMake-based template for building SuperBLT C++ plugins.


## How to use:

* Install [CMake](https://cmake.org/download/), if you haven't already. CMake is
considered the standard build tool for all parts of SuperBLT, and in the interests
of compatibility it's use is **highly** recomended. I'd also recommend selecting the installer
option to add it to your path, if you're on Windows
* If you're on Windows, install Visual Studio and the C++ modules
* Download this template, and extract it somewhere
* Edit CMakeLists.txt, setting the plugin and project names (plugin name is
the name of the DLL or SO that will be built, project name is the name of the
Visual Studio project)
* Run `build/configure.bat`, which will generate a Visual Studio project
* Open said Visual Studio project
* Read and edit the appropriate parts of `legal.cpp`

That's it! Now build the project, and a plugin will be built.

If you want to automatically copy the plugin into one of your mod folders,
edit `CMakeLists.txt` at the end where it talks about that, and rerun `configure.bat`.

Place your source files in `src/`, and headers in `include/`.

Note that whenever you add a source or header file, you must rerun `configure.bat` to make the
changes show up.

(TODO: Add GNU+Linux support)

## Licence

This project is licenscd under the GNU General Public Licence, version 3. A copy of this
licence is included in the form of `LICENSE.txt`.

