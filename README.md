See Wiki for:

- [Building from source on Windows / Linux](https://github.com/willmh93/bitloop/wiki/Build-library-from-source)
- [Building with Emscripten](https://github.com/willmh93/bitloop/wiki/Build-library-with-Emscripten)
- [Creating a standalone project with the command-line tool](https://github.com/willmh93/bitloop/wiki/Creating-a-new-project-(CLI))

## Development progress

### Building

|                                                                      | Windows                  | Linux                   
| -------------------------------------------------------------------- | ------------------------ | ------------------------
| Modular nested projects                                              | :heavy_check_mark:       | :heavy_check_mark:      
| Building standalone projects (with child projects as dependencies)   | :heavy_check_mark:       | :heavy_check_mark:      
| Command-line tools for projects                                      | :heavy_check_mark:       | :heavy_check_mark:      
| C++23 support                                                        | :heavy_check_mark:       | :heavy_check_mark:    
| Visual Studio                                                        | :heavy_check_mark:       | :heavy_check_mark:    
| Visual Studio Code                                                   | :heavy_check_mark:       | :heavy_check_mark:    

### Architecture

|                                           | Windows                  | Linux                    | Web (Emscripten)                                          |
| ----------------------------------------- | ------------------------ | ------------------------ | --------------------------------------------------------- |
| Multithreading                            | :heavy_check_mark:       | :heavy_check_mark:       | :heavy_check_mark: (pthreads with COOP/COEP Headers)      |
| 128-bit floating point support (wip)      | :hourglass:              | :hourglass:              | :hourglass:                                               |

### Graphics

|                                           | Windows                  | Linux                    | Web (Emscripten)                                          |
| ----------------------------------------- | ------------------------ | ------------------------ | --------------------------------------------------------- |
| High-DPI support                          | :heavy_check_mark:       | :heavy_check_mark:       | :heavy_check_mark:                                        |
| Mulitple Viewports (Split Screen)         | :heavy_check_mark:       | :heavy_check_mark:       | :heavy_check_mark:                                        |
| 2D Viewport (NanoVG wrapper)              | :heavy_check_mark:       | :heavy_check_mark:       | :heavy_check_mark:                                        |
| 3D Viewport                               | N/A                      | N/A                      | N/A                                                       |

### Recording / Image Capture

|                                           | Windows                  | Linux                    | Web (Emscripten)                                          |
| ----------------------------------------- | ------------------------ | ------------------------ | --------------------------------------------------------- |
| Custom-resolution image renders           | N/A                      | N/A                      | N/A                                                       |
| FFmpeg recording                          | N/A                      | N/A                      | N/A                                                       |

### Other features

|                                           | Windows                  | Linux                    | Web (Emscripten)                                          |
| ----------------------------------------- | ------------------------ | ------------------------ | --------------------------------------------------------- |
| Timeline support                          | N/A                      | N/A                      | N/A                                                       |
