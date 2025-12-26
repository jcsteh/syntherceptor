# Syntherceptor

<!--beginDownload-->
[Downloads available on the Syntherceptor website](https://syntherceptor.jantrid.net/)
<!--endDownload-->

- Author: James Teh &lt;jamie@jantrid.net&gt;
- Copyright: 2025 James Teh
- License: GNU General Public License version 2.0

Syntherceptor is a SAPI5 synthesiser which speaks using NVDA.
This is useful for apps like Komplete Kontrol, Surge, etc. which use SAPI5 but you'd prefer the speech output to happen using your NVDA synthesiser and settings.
This also prevents different voices from speaking at the same time, allows you to interrupt speech with a key press, etc.

## Download and Installation
Download the latest installer using the link at the top of this page.
Then, run the installer and follow the instructions.

## Building

### Dependencies
1. Install Build Tools for Visual Studio 2022 .
2. Install Python and SCons.
3. [Download NVDA Controller Client](https://download.nvaccess.org/releases/stable/nvda_2025.3.2_controllerClient.zip). Extract it into `deps/nvdaControllerClient` inside the Syntherceptor checkout directory.

### How to Build
From a command prompt, change to the Syntherceptor checkout directory and run `scons`.
The resulting installer can be found as `build/syntherceptor_unknown.exe`.
