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
  <li>Arduino fabgl 0.9.0 reduced library (included in PLATFORMIO project)</li>
  <li>Arduino bitluni 0.3.3 reduced library (included in project)</li>
 </ul>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/ttgovga32v12.jpg'></center> 

<br><br>
<h1>PlatformIO</h1>
PLATFORMIO 2.2.1 must be installed from the Visual Studio extensions. Espressif32 v3.3.2 is also required.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIOinstall.gif'></center>
Then the working directory <b>TinyNesMasterttgovga32</b> will be selected.
We must modify the <b>platformio.ini</b> file the <b>upload_port</b> option to select the COM port where we have our TTGO VGA32 board.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIO.gif'></center>
Then we will proceed to compile and upload to the board. No partitions are used, so we must upload the whole compiled binary.
It's all set up so you don't have to install the bitluni and fabgl libraries.
  
