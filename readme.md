# Syntherceptor

- Author: James Teh &lt;jamie@jantrid.net&gt;
- Copyright: 2025 James Teh
- License: GNU General Public License version 2.0

Syntherceptor is a SAPI5 synthesiser which speaks using NVDA.

## Building
1. [Download NVDA Controller Client](https://download.nvaccess.org/releases/stable/nvda_2025.3.2_controllerClient.zip).
2. Extract it into `deps/nvdaControllerClient` inside the Syntherceptor checkout directory.
3. From a command prompt, change to the Syntherceptor checkout directory and run `scons`.
4. The resulting files for usage and distribution can be found in the `dist` directory.

## Installation
1. Copy the distribution files to `C:\syntherceptor`.
2. Open `install.reg` and allow it to apply changes.
3. If desired, set Syntherceptor as the default voice in both the 32 bit and 64 bit SAPI control panels.
