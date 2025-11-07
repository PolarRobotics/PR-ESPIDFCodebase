Overview
--------
This project now supports two selectable build "environments" that mimic the PlatformIO
`platformio.ini` environments: `robot` (default) and `depairing`.

How it works
------------
- A Kconfig choice was added at `main/Kconfig` named "Build environment". It exposes two
  options: `ENV_ROBOT` (default) and `ENV_DEPAIRING`.
- Component CMakeLists files in `components/Pairing`, `components/Robot`, and
  `components/Utilities` use the `CONFIG_ENV_*` symbols to include or exclude source
  files during build. This reproduces the PlatformIO `build_src_filter` behaviour.

Select an environment
---------------------
1. Run the ESP-IDF configuration editor:

   idf.py menuconfig

2. Open "Project configuration" -> "Build environment" and select either
   "Robot (normal full firmware)" or "Depairing (light pairing/depairing firmware)".

Command-line selection
----------------------
You can also set the option in `sdkconfig` directly by editing the file or using
`idf.py menuconfig` and then running `idf.py reconfigure`.

Build
-----
Standard build commands still apply:

  idf.py reconfigure
  idf.py build

Notes & caveats
---------------
- The filtering is conservative: only a few files that were explicitly excluded in the
  PlatformIO configuration are gated. If you need a more exact 1:1 source filter for
  additional components, we can extend the conditional filters in the corresponding
  `CMakeLists.txt` files.
- This approach uses Kconfig + CMake conditionals so the chosen environment is visible
  to the build system and to other components that might need to adapt.
