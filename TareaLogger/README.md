# Keylogger en C

Este proyecto es un keylogger simple implementado en C que registra las teclas presionadas en un archivo de texto. El código está diseñado para ejecutarse en sistemas Linux y utiliza las interfaces de entrada de bajo nivel proporcionadas por el sistema operativo.

## Requisitos

- Linux
- Compilador de C (por ejemplo, `gcc`)

## Instalación

1. **Clona el repositorio:**

   `git clone https://github.com/tu_usuario/tu_repositorio.git`
   `cd tu_repositorio`
   
3. **Compila el código**

   `gcc -o keylogger keylogger.c`
   
## Uso
Ejecuta el keylogger con permisos de superusuario (necesarios para acceder a los dispositivos de entrada):

  `sudo ./keylogger`
  
El keylogger empezará a registrar las teclas presionadas y almacenará la información en el archivo log.txt especificado en el código.

##Puedes modificar la ruta del archivo de log en el código fuente si es necesario:

`define LOGFILEPATH "/home/kali/Documents/escuela/Sec/TareaLogger/log.txt"`

## Supuestos del Programa

- Configuración del Teclado: El keylogger asume que el teclado está configurado de acuerdo con la disposición estándar de un teclado estadounidense. La asignación de teclas puede no ser precisa en otros layouts de teclado.
- Permisos de Ejecución: El programa asume que se ejecutará con permisos de superusuario para acceder al dispositivo de entrada del teclado.
- Entorno de Ejecución: El keylogger está diseñado para ejecutarse en entornos Linux con acceso a las interfaces de entrada del sistema.

## Casos de Uso

- Monitoreo de Entrada de Teclado: Para registrar todas las teclas presionadas en un sistema para propósitos educativos o de investigación en ciberseguridad.
- Pruebas de Seguridad: Para analizar el comportamiento de las aplicaciones en términos de manejo de entradas de teclado y seguridad en aplicaciones que usan datos de entrada del usuario.
- Investigación Académica: En un entorno de laboratorio para estudiar la seguridad de sistemas y el impacto de los keyloggers en la seguridad de la información.

## Cómo Funciona
**Detección del Dispositivo de Entrada:**

El programa determina automáticamente el dispositivo de entrada del teclado utilizando la información del sistema disponible en __/proc/bus/input/devices__

**Registro de Eventos de Teclado:**

El keylogger abre el dispositivo de entrada del teclado y escucha eventos de teclado (EV_KEY). Cada vez que se detecta una pulsación de tecla, el programa actualiza el estado de las teclas modificadoras (Shift, Ctrl, Alt) y registra el carácter correspondiente en el archivo de log.

**Manejo de Teclas Modificadoras:**

El programa maneja la tecla modificadora shift para registrar correctamente caracteres en función de si Shift está presionado o no.

## Limitaciones
**Permisos de Superusuario:** La ejecución requiere permisos de superusuario para acceder a los dispositivos de entrada.
**Compatibilidad:** Este keylogger está diseñado para sistemas Linux y puede no funcionar en otros sistemas operativos; además la asignación de teclas fue adecuada para un teclado latinoamericano usando la configuración de teclas de un dispositivo estadounidense dada por la configuración del sistema Kali Linux.
**Alcance:** El registro de teclas no llegará a capturar combinaciones con *AltGR*, teclas especiales como *SUPR* o combinaciones con *Alt*, *CTRL*, *Fn*, *Fx* (F1,F2 ... F12), sólo capturará letras, números, puntuaciones, signos especiales como {];._- entre otros.

## Advertencia
Este keylogger debe usarse únicamente para fines educativos o en sistemas donde tengas permiso explícito para registrar las teclas. El uso de keyloggers sin consentimiento es ilegal y antiético.

###### Elaborado por Joshua René Cruz Ramírez para la clase de Seguridad Informática en la FES Aragón
