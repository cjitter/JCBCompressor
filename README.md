# JCBCompressor

![JCBCompressor Interface](Assets/screenshot.png)

Plugin compresor de audio desarrollado con [Gen~ Plugin Export (Cycling '74)](https://cycling74.com) y el framework C++ [JUCE](https://github.com/juce-framework/JUCE). Este plugin es parte de un paquete de plugins de audio básicos hechos con Gen~ en Max, y que uso como material didáctico en mis clases de la asignatura de Técnicas de Grabación y Masterización para Música Electroacústica de los másteres [MCE](https://katarinagurska.com/curso-of/master-de-composicion-electroacustica-mce/) y [MCAV](https://katarinagurska.com/curso-of/master-composicion-medios-audiovisuales-mcav/). La exportación y creación de los plugins básicos con JUCE se hizo hace unos años y se ha mejorado, sobre todo la parte visual y de funcionalidad, extensivamente mediante el uso de Claude Code (ver NOTAS.md al respecto).

## Instalación

### macOS
1. Descarga el archivo DMG desde la página de [Releases](https://github.com/cjitter/JCBComp/releases)
2. Abre el DMG y ejecuta el instalador
3. El instalador colocará automáticamente los plugins en las ubicaciones correctas:
   - VST3: `/Library/Audio/Plug-Ins/VST3/`
   - AU: `/Library/Audio/Plug-Ins/Components/`
   - AAX: `/Library/Application Support/Avid/Audio/Plug-Ins/`

*Nota: El DMG está firmado y notarizado para macOS.*

## Requisitos del Sistema

### macOS
- macOS 10.13 o posterior
- Procesador Intel o Apple Silicon
- DAW compatible con VST3, AU o AAX
- Probado en: Pro Tools, Reaper, Logic, Ableton Live y Bitwig

## Compilación desde Código Fuente

### Requisitos Previos
- Git, [CMake](https://cmake.org) 3.20 o posterior, compilador Apple Clang compatible con C++20 (incluido en Xcode 13 o superior).
- [JUCE](https://github.com/juce-framework/JUCE) 8.0.8 (se descarga automáticamente via FetchContent)
- **AAX SDK** (requerido para compilar formato AAX - disponible desde cuenta de desarrollador Avid)

### Instrucciones de Compilación

1. Clona el repositorio:
```bash
git clone https://github.com/cjitter/JCBCompressor.git
cd JCBCompressor
```

2. **Configuración del proyecto**: El archivo `CMakeLists.txt` es el generador principal del proyecto y define toda la estructura de compilación.

3. **Compilación**:
```bash
# Compilación Debug
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug

# Compilación Release
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

### Notas Importantes de Compilación
- El directorio `exported-code/` contiene código Gen~ auto-generado - no modificar
- Usa directorios de compilación separados para cualquier experimento con cmake

## Estructura del Proyecto

```
JCBCompressor/
├── Assets/                          # Recursos gráficos y presets
│   ├── code/                       # Código fuente Gen~
│   ├── FactoryPresets/             # Presets incluidos
│   └── *.png                       # Imágenes de la interfaz
├── Source/                         # Código fuente C++
│   ├── PluginProcessor.cpp/.h      # Procesador principal del plugin
│   ├── PluginEditor.cpp/.h         # Editor principal del plugin
│   ├── Components/                 # Componentes de interfaz
│   │   ├── UI/                    # Componentes de UI personalizados
│   │   │   ├── CustomComboBox.*
│   │   │   ├── CustomSlider.*
│   │   │   ├── CustomTooltip.*
│   │   │   ├── GradientMeter.*
│   │   │   ├── TransferFunctionDisplay.*
│   │   │   └── TriSlider.*
│   │   └── Windows/               # Ventanas auxiliares
│   │       ├── CodeWindow.*
│   │       ├── CreditsWindow.*
│   │       ├── CustomDialog.*
│   │       ├── DarkThemeColors.*
│   │       └── UndoableParameterAttachment.*
│   └── Helpers/                   # Clases de utilidad
│       ├── ParameterChangeAction.*
│       ├── UndoableParameter.*
│       └── UTF8Helper.*
├── exported-code/                 # Código Gen~ exportado (auto-generado)
│   └── gen_dsp/                  # Archivos DSP de Gen~
├── Max/                          # Patch original de Max/MSP
│   └── JCBCompressor.maxpat
├── cmake/                        # Configuración CMake
├── CMakeLists.txt               # Archivo principal de configuración
├── LICENSE                      # Licencia GPL v3
└── README.md                    # Este archivo
```

## Características

- **Compresión con DSP Gen~** de Max/MSP
- **Procesamiento estéreo en modo Link**
- **Tres modos de detección**: Sliding RMS, Exponential RMS y Slow RMS
- **Interpolación continua** entre detección de pico y RMS
- **Control de suavizado adicional** (smoothing) para ajuste fino de la respuesta
- **Filtros Butterworth de 2º y 4º orden** para las cadenas principal y sidechain
- **Controles estándar**: Threshold, Ratio, Knee, Attack, Release, Auto-Release y más
- **Soft clipping** integrado
- **Procesamiento sidechain** con filtros dedicados
- **Visualización** con display de forma de onda y medidores
- **Menú de presets**
- **Bypass interno** independiente del DAW
- **Monitorización delta** (diferencia entrada/salida), solo de filtros sidechain
- **Formatos soportados**: VST3, AU y AAX

## Uso

### Operación Básica
1. **Threshold**: Establece el nivel al que comienza la compresión (-60 a 0 dB)
2. **Ratio**: Ajusta la cantidad de compresión (1:1 a 20:1)
3. **Attack**: Controla qué tan rápido actúa la compresión (0.1 a 250 ms)
4. **Release**: Define qué tan rápido se libera la compresión (0.1 a 1000 ms)
5. **Knee**: Ajusta la transición hacia la compresión (0 a 30 dB)
6. **Makeup gain**: Ganancia de compensación para recuperar el nivel reducido (-12 +12 dB)

### Características Adicionales
- **Modos de detección**:
  - Sharp (sliding RMS), Expo RMS (clásico) y Slow RMS
  - Interpolación continua entre detección de pico y RMS
  - Control de smoothing adicional para personalizar la respuesta
- **Compresión con softknee**: cálculo de reducción con softknee lineal primer orden
- **Sidechain externo**: Usa una señal externa para la detección de compresión
- **Filtros**: Butterworth de 2º y 4º orden para compresión dependiente de frecuencia
- **Auto gain**: hasta el 70% recuperación de ganancia, funciona en conjunto con el makeup gain
- **Compresión paralela**: sumatorio señal seca con señal comprimida
- **Softclipping**: softclipping después de auto gain, makeup gain y compresión paralela
- **Bypass interno**: Independiente del bypass del DAW
- **Monitorización**: Escucha solo la diferencia entre entrada y salida y solo filtros

## Licencia

JCBCompressor es software libre: puedes redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General GNU publicada por la Free Software Foundation, ya sea la versión 3 de la Licencia, o (a tu elección) cualquier versión posterior.

Consulta [LICENSE](LICENSE) para más detalles.

## Recursos

### Bibliografía Técnica
- [Graham Wakefield & Gregory Taylor - *Generating Sound and Organizing Time*](https://cycling74.com/books/go)
- [Will C. Pirkle - *Designing Audio Effect Plugins in C++*](https://www.willpirkle.com)
- [Giannoulis, Massberg, Reiss - *Dynamic Range Compressor Design*](https://eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf)
- [Matthijs Hollemans - *The Complete Beginner's Guide to Audio Plug-in Development*](https://www.theaudioprogrammer.com/books/beginners-plugin-book)

### Contribuciones de Código Base
- **Código base para vincular Gen~ Plugin Export con JUCE APVTS** - [Kengo Suzuki](https://github.com/szkkng/jr-granular)
- **Código base para medidores** - [A. Murthy](https://www.youtube.com/watch?v=ILMdPjFQ9ps)
- **Tooltips** - [F. Becker](https://github.com/francoisbecker/fb-utils/blob/master/include/fbu/tooltip_component.hpp)
- **Skin base/Presets** - J. Peña

### Herramientas de Desarrollo
- Desarrollado con asistencia de [Claude](https://claude.ai/)

## Por Hacer

- Migrar de Plugin Export a la exportación C++ de RNBO
- Implementar oversampling
- Implementar modo dual y M/S
- Implementar MIDI mapping
- Mejorar sistema de visualización entrada/salida e histograma de reducción de ganancia

## Enlaces

- **Repositorio GitHub**: [JCBCompressor](https://github.com/cjitter/JCBCompressor)
- **Documentación**: Ver [NOTAS.md](NOTAS.md) para información adicional

---

*JCBCompressor v0.9.99 beta*