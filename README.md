# LazyImg

This is A lightweight library to read image

## How To build

### Clone The Project

If build in windows run the Developer PowerShell For vs20xx

```bash
git clone https://github.com/ashji/FastLazyImageReader.git
cd FastLazyImageReader
```

### Choose The Buiding OS

```bash
# build in linux
cd linux 
# build in wndows
cd windows
```

### Choose Modules

Run the build.py

|            MODULES MACRO            |           COMMAND           |
| :----------------------------------: | :-------------------------: |
|    READ "*.TIFF OR "*.TIF" FILE    |      ADD_TIFF_MODULES      |
|            ONLY NEED READ            |          READ_ONLY          |
|    NOT SHOW IMAGE DATA IN WINDOW    | NOT_ADD_WINDOW_SHOW_MODULES |
| READ OR WIRTE IMAGE BY SYSTEM IO PTR |    NOT_USE_C_STD_STREAM    |

```bash
# linux
python3 build.py ADD_TIFF_MODULES NOT_ADD_WINDOW_SHOW_MODULES
# windows
python build.py ADD_TIFF_MODULES NOT_ADD_WINDOW_SHOW_MODULES
```
