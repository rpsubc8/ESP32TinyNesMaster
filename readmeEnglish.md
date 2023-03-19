# Tiny ESP32
Port of Jens Nyberg's NES Master PC emulator to TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4) board with ESP32.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewNES.gif'></center>
I have made several modifications:
<ul>
 <li>Ported from x86 PC to ESP32</li>
 <li>No PSRAM is used, running on ESP32 with 520 KB RAM (TTGO VGA32 v1.x).</li> 
 <li>Use of a single core</li>
 <li>Low resource OSD</li>
 <li>Created project compatible with Arduino IDE and Platform IO</li>
 <li>Web Editor and ArduinoDroid (6.3.1) compatible project with tool makeandroid.bat</li>
 <li>Emulation of ricoh2A03 (fabgl library) reduced version of <b>dcrespo3d</b>(David Crespo Tascón)</li>
 <li>Screen adjustment X</li>
 <li>Emulation CPU speed menu (AUTO), sound, keyboard</li>
 <li>Support for 8 and 64 color mode (reduced version by Ricardo Massaro).</li>  
 <li>VGA 320x240</li>   
 <li>It is recommended to use only single cartridges without bank switching.</li>
 <li>A 3-channel DIRTY Sound is used which is not faithful to the real emulation.</li> 
 <li>Up, down, left, right cursors</li> 
 <li>X key (button A), Z key (button B)</li>
 <li>C key (SELECT button), V key (START button)</li>
</ul>

<br>
<h1>Requirements</h1>
Required:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.48.1 PLATFORMIO 2.2.1 Espressif32 v3.3.2</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
  <li>ArduinoDroid (6.3.1)</li>
  <li>Arduino fabgl 0.9.0 reduced library (included in PLATFORMIO project)</li>
  <li>Arduino bitluni 0.3.3 reduced library (included in project)</li>
 </ul>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/ttgovga32v12.jpg'></center> 


<br><br>
<h1>ArduinoDroid</h1>
You must run, just once, the script makeandroid.bat, which leaves the entire data structure of the dataFlash directory in the root directory, as well as replacing the main files with .h calls without using the dataFlash data directory.<br>
At the end, the script itself ends up deleting the dataFlash directory.<br>
The script uses fart.exe (find and replace text).<br>
Once, it has been successfully executed, it can be used with the ArduinoDroid.


<br><br>
<h1>Arduino Web Editor</h1>
The makeandroid.bat script must be run once only. Once finished, it is uploaded to the cloud as any other project, either compressed in zip or by files.


<br><br>
<h1>PlatformIO</h1>
PLATFORMIO 2.2.1 must be installed from the Visual Studio extensions. Espressif32 v3.3.2 is also required.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIOinstall.gif'></center>
Then the working directory <b>TinyNesMasterttgovga32</b> will be selected.
We must modify the <b>platformio.ini</b> file the <b>upload_port</b> option to select the COM port where we have our TTGO VGA32 board.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIO.gif'></center>
Then we will proceed to compile and upload to the board. No partitions are used, so we must upload the whole compiled binary.
It's all set up so you don't have to install the bitluni and fabgl libraries.
  
<br><br>
<h1>Arduino IDE</h1>
The whole project is compatible with the Arduino 1.8.11 framework.
We only have to open the <b>nes.ino</b> in the <b>nes</b> directory.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewArduinoIDEpreferences.gif'></center>
We must install the spressif extensions in the additional card url manager <b>https://dl.espressif.com/dl/package_esp32_index.json</b>.
<br>
The project is already prepared, so no bitluni or fabgl libraries are needed.
We must deactivate the PSRAM option, and in case of exceeding 1 MB of binary, select 4 MB of partition when uploading. Although the code does not use PSRAM, if the option is active and our ESP32 does not have it, an exception will be generated and it will restart in loop mode.

<br><br>
<h1>Usability</h1>
The following actions are allowed from the menu (F1 key):
 <ul>
  <li>Select ROM allows you to choose game ROMs.</li>
  <li>Offset X of the screen.</li>
  <li>Change polling milliseconds for video, keyboard, mouse and sound.</li>
  <li>Sound Volume (100%, 75,%, 50%, 25%, 5%)</li>
  <li>Sound active or muted.</li>
  <li>Cpu wait in AUTO mode (set 20 ms per real frame) or wait in ms as desired.</li>
 </ul>
A basic low-resource OSD is available, i.e. very simple, which is displayed by pressing the <b>F1</b> key.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewOSD.gif'></center>
The files must be converted to .h in hexadecimal. The online tool can be used:<br>
<a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>


<br><br>
<h1>Options</h1>
The <b>gbConfig.h</b> file options are selected:
<ul>
 <li><b>use_lib_vga8colors:</b> Forces to use RGB 8-color mode (3 pins). Outputs 8 colors, as opposed to 64 in normal mode (6 RRGGBB pins).</li>
 <li><b>use_lib_sound_ricoh2A03:</b> A 3-channel mixer is used in dirty mode, emulating the ricoh2A03. Consume un poco de RAM. It requires the reduced fabgl 0.9.0 library, already included in the project.</li>
 <li><b>use_lib_log_serial:</b> Logs are sent via usb serial port.</li>
 <li><b>gb_ms_keyboard:</b> The number of polling milliseconds for the keyboard must be specified.</li>
 <li><b>gb_ms_sound:</b> The number of polling milliseconds for the sound must be specified.</li>
 <li><b>gb_delay_emulate_ms:</b> Milliseconds to wait for each completed frame.</li>
</ul>


<br><br>
<h1>Tool rom2h</h1>
I have created a very basic tool(win32), to convert .nes files to .h in ready mode to be processed by the emulator. We just have to leave the .nes files in the <b>input/roms</b> folder and run the <b>rom2h.exe</b> file, so that an output will be generated in the <b>output/dataFlash</b> directory. For testing purposes, a file has been left in the <b>input/roms</b> directory, which it is recommended to delete in case of making a new custom list. It is also recommended to delete the files in the <b>nes/dataFlash/gbrom.h</b> and <b>nes/dataFlash/rom</b> directory to have a clean project.<br><br><br>
<a href='https://github.com/rpsubc8/ESP32TinyNesMaster/tree/main/tools'>Tool rom2h</a>
<br><br>
<pre>
 input/
  roms/
   AlfonzoMeleeHB.nes
   ChaseHB.nes
 output/
  dataFlash/
   rom/
</pre>
Subsequently we must copy the <b>dataFlash</b> directory into the <b>ESP32TinyNesMaster\nes</b> project overwriting the previous dataFlash folder. It is recommended that you clean up the project and recompile.<br>
This tool is very simple, and does not check for errors, so it is recommended to leave the files with very simple names and as simple as possible.
The project in PLATFORM.IO is prepared for 1MB of Flash. If we need the 4MB of flash, we will have to modify the entry in the <b>platformio.ini</b> file.
<pre>board_build.partitions = huge_app.csv</pre>
In the Arduino IDE, we must choose the option <b>Partition Scheme (Huge APP)</b>.


<br><br>
<h1>DIY circuit</h1>
If we don't want to use a TTGO VGA32 v1.x board, we can build it following the <b>fabgl</b> schematic:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/fabglcircuit.gif'></center>
