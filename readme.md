# Syntherceptor

- Author: James Teh &lt;jamie@jantrid.net&gt;
- Copyright: 2025 James Teh
- License: GNU General Public License version 2.0

Syntherceptor is a SAPI5 synthesiser which speaks using NVDA.

## Building

### Dependencies
1. Install Build Tools for Visual Studio 2022 .
2. Install Python and SCons.
3. [Download NVDA Controller Client](https://download.nvaccess.org/releases/stable/nvda_2025.3.2_controllerClient.zip). Extract it into `deps/nvdaControllerClient` inside the Syntherceptor checkout directory.

### How to Build
From a command prompt, change to the Syntherceptor checkout directory and run `scons`.
The resulting installer can be found as `build/syntherceptor.exe`.
