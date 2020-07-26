# BSP TextInfo Obfuscator
This tool is similar to the IID NoDraw by Omnicoder except that it support Surface Flags.
It contains 2 different methods: keep surface flags or not... (I'll enumerate them as "flags" and "nodraw")
Both have unfortunately their __limitations__.

## Flags method
This method has as a goal to keep the same Surface Flags that the original map. (Best choice if we don't want to broke any game features)
    1. Only one different "[$surfaceprop](https://developer.valvesoftware.com/wiki/$surfaceprop)" per SurfaceFlags (When choosing the texture). Still better than the NoDraw method.
    2. The obfuscation is less disturbing.
    
![Flags method in Hammer](https://raw.githubusercontent.com/FeepsDev/BSP-TexInfo-Obfuscator/master/images/flags.PNG)
   
    
## NoDraw method
It doesn't keep the Surface Flags but give you the choice of the texture which will fill the whole map.
Limitations:
    1. No Skybox support (SurfaceFlags are not kept).
    2. Only one "[$surfaceprop](https://developer.valvesoftware.com/wiki/$surfaceprop)" for the whole map. (But can technically be chosen when choosing our texture).

![NoDraw method in Hammer](https://raw.githubusercontent.com/FeepsDev/BSP-TexInfo-Obfuscator/master/images/nodraw.PNG)
    
## How it works ?
It's simply editing the texinfo value on the [brushside lump (Lump 19)](https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Brush_and_brushside) used by [BSPSource](https://github.com/ata4/bspsrc) of the BSP file.

## How to use ?
Open BSP-TexInfo-Obfuscator.exe (or your compilated program) in a console. It'll tell you how to use it.
> Example: BSP-TexInfo-Obfuscator.exe -flags c:\Users\Feeps\Desktop\test.bsp

## How to compile
I don't provide pre-compiled binaries for other systems than Windows.
You can simply use CMake..