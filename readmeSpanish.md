# Tiny ESP32 NES
Port del emulador NES Master de PC de Jens Nyberg a la placa TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4) con ESP32.
<br>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewNES.gif'></center>
He realizado varias modificaciones:
<ul>
 <li>Portado de x86 PC a ESP32</li>
 <li>No se usa PSRAM, funcionando en ESP32 de 520 KB de RAM (TTGO VGA32 v1.x)</li> 
 <li>Uso de un sólo core</li>
 <li>OSD de bajos recursos</li>
 <li>Creado proyecto compatible con Arduino IDE y Platform IO</li>
 <li>Proyecto compatible con Web Editor y ArduinoDroid (6.3.1) con tool makeandroid.bat</li>
 <li>Emulación de ricoh2A03 (libreria fabgl) versión reducida de <b>dcrespo3d</b>(David Crespo Tascón)</li>
 <li>Ajuste de pantalla X</li>
 <li>Menú de velocidad de CPU de emulación (AUTO), sonido, teclado</li>
 <li>Soporte para modo 8 y 64 colores (versión reducida de Ricardo Massaro).</li>  
 <li>VGA 320x240</li>   
 <li>Se recomienda sólo usar cartuchos simples sin conmutación de bancos</li>
 <li>Se usa un DIRTY Sound de 3 canales que no es fiel a la emulación real</li> 
 <li>Cursores arriba, abajo, izquierda, derecha</li> 
 <li>Tecla X (botón A), tecla Z (botón B)</li>
 <li>Tecla C (botón SELECT), tecla V (botón START)</li>
</ul> 
  
<br>
<h1>Requerimientos</h1>
Se requiere:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.48.1 PLATFORMIO 2.2.1 Espressif32 v3.3.2</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>
  <li>ArduinoDroid (6.3.1)</li>
  <li>Librería reducida Arduino fabgl 0.9.0 (incluida en proyecto PLATFORMIO)</li>
  <li>Librería reducida Arduino bitluni 0.3.3 (incluida en proyecto)</li>
 </ul>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/ttgovga32v12.jpg'></center> 


<br><br>
<h1>ArduinoDroid</h1>
Se debe ejecutar, sólo una vez, el script makeandroid.bat, que nos deja toda la estructura de datos del directorio dataFlash en el directorio raiz, así como reemplazando las archivos principales con llamadas de los .h sin usar el directorio de datos dataFlash.<br>
Al finalizar, el propio script, termina borrando el directorio dataFlash.<br>
El script utiliza el fart.exe (find and replace text).<br>
Una vez, se haya ejecutado con éxito, se puede usar con el ArduinoDroid.


<br><br>
<h1>Arduino Web Editor</h1>
Se debe ejecutar, sólo una vez, el script makeandroid.bat. Una vez finalizado, se sube a la nube como un proyecto cualquiera, ya sea comprimido en zip o por archivos.
 
 
<br><br>
<h1>PlatformIO</h1>
Se debe instalar el PLATFORMIO 2.2.1 desde las extensiones del Visual Studio. Se requiere también Espressif32 v3.3.2.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIOinstall.gif'></center>
Luego se seleccionará el directorio de trabajo <b>TinyNesMasterttgovga32</b>.
Debemos modificar el fichero <b>platformio.ini</b> la opción <b>upload_port</b> para seleccionar el puerto COM donde tenemos nuestra placa TTGO VGA32.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewPlatformIO.gif'></center>
Luego procederemos a compilar y subir a la placa. No se usa particiones, así que debemos subir todo el binario compilado.
Está todo preparado para no tener que instalar las librerias de bitluni ni fabgl.


<br><br>
<h1>Arduino IDE</h1>
Todo el proyecto es compatible con la estructura de Arduino 1.8.11.
Tan sólo tenemos que abrir el <b>nes.ino</b> del directorio <b>nes</b>.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewArduinoIDEpreferences.gif'></center>
Debemos instalar las extensiones de spressif en el gestor de urls adicionales de tarjetas <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
<br>
Ya está preparado el proyecto, de forma que no se necesita ninguna librería de bitluni ni fabgl.
Debemos desactivar la opción de PSRAM, y en caso de superar 1 MB de binario, seleccionar 4 MB de partición a la hora de subir. Aunque el código no use PSRAM, si la opción está activa y nuestro ESP32 no dispone de ella, se generará una excepción y reinicio del mismo en modo bucle.



<br><br>
<h1>Usabilidad</h1>
Se permiten las siguientes acciones desde el menú (tecla F1):
 <ul>  
  <li>Seleccionar ROM permite elegir ROM de juego.</li>
  <li>Offset X de la pantalla</li>
  <li>Cambiar los milisegundos de polling para video, teclado, ratón y sonido</li>  
  <li>Sonido Volumen (100%, 75,%, 50%, 25%, 5%)</li>
  <li>Sonido activo o en silencio</li>
  <li>Espera de CPU en modo AUTO (ajuste 20 ms por frame real) o la espera en ms que queramos</li>    
 </ul>
 Se dispone de un OSD básico de bajos recursos, es decir, muy simple, que se visualiza pulsando la tecla <b>F1</b>.
 <center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/previewOSD.gif'></center>
 Los ficheros deben ser convertidos a .h en hexadecimal. Puede usarse la herramienta online:<br>
 <a href='http://tomeko.net/online_tools/file_to_hex.php?lang=en'>http://tomeko.net/online_tools/file_to_hex.php?lang=en</a>
 
 
<br><br>
<h1>Opciones</h1>
El archivo <b>gbConfig.h</b> se seleccionan las opciones:
<ul> 
 <li><b>use_lib_vga8colors:</b> Obliga a usar RGB modo de 8 colores (3 pines). Saca 8 colores, frente a los 64 del modo normal (6 pines RRGGBB).</li>
 <li><b>use_lib_sound_ricoh2A03:</b> Se utiliza un mezclador de 3 canales en modo dirty, emulando el ricoh2A03. Consume un poco de RAM. Se requiere la librería fabgl 0.9.0 reducida, ya incluida en el proyecto</li>
 <li><b>use_lib_log_serial:</b> Se envian logs por puerto serie usb</li>
 <li><b>gb_ms_keyboard:</b> Se debe especificar el número de milisegundos de polling para el teclado.</li>
 <li><b>gb_ms_sound:</b> Se debe especificar el número de milisegundos de polling para el sonido.</li>
 <li><b>gb_delay_emulate_ms:</b> Milisegundos de espera por cada frame completado.</li>
</ul>


<br><br>
<h1>Tool rom2h</h1>
He creado una herramienta muy básica (win32), para convertir los archivos .rom en .h en modo lista para ser procesados por el emulador. Tan sólo tenemos que dejar los archivos .rom en la carpeta <b>input/roms</b> y ejecutar el archivo <b>rom2h.exe</b>, de forma que se generará una salida en el directorio <b>output/dataFlash</b>. Para pruebas, se ha dejado un archivo en el directorio <b>input/roms</b>, que se recomienda borrar en caso de realizar una nueva lista personalizada. También se recomienda borrar los archivos del directorio <b>nes/dataFlash/gbrom.h</b> y <b>nes/dataFlash/rom</b> para tener un proyecto limpio.<br><br>
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
Posteriormente debemos copiar el directorio <b>dataFlash</b> en el proyecto <b>TinyNesMasterttgovga32\nes</b> sobreescribiendo la carpeta dataFlash previa. Se recomienda limpiar el proyecto y volver a compilar.<br>
Esta herramienta es muy simple, y no controla los errores, por lo que se recomienda dejarle los archivos con nombres muy simples y lo más sencillo posible.<br>
El proyecto en PLATFORM.IO está preparado para 1 MB de Flash. Si necesitamos los 4MB de flash, tendremos que modificar la entrada del archivo <b>platformio.ini</b>
<pre>board_build.partitions = huge_app.csv</pre>
En el Arduino IDE, debemos elegir la opción <b>Partition Scheme (Huge APP)</b>.


<br><br>
<h1>DIY circuito</h1>
Si no queremos usar una placa TTGO VGA32 v1.x, podemos construirla siguiendo el esquema de <b>fabgl</b>:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyNesMaster/main/preview/fabglcircuit.gif'></center>
