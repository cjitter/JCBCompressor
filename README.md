
![JCBCompressor Interface](Assets/screenshot.png)

Plugin compresor de audio desarrollado con [gen~ Plugin Export](https://github.com/Cycling74/gen-plugin-export) y el framework C++ [JUCE](https://github.com/juce-framework/JUCE). Este plugin es parte de un paquete de plugins de audio básicos hechos con gen~ en Max, y que uso como material didáctico en mis clases de la asignatura de Técnicas de Grabación y Masterización para Música Electroacústica en el [MCE](https://katarinagurska.com/curso-of/master-de-composicion-electroacustica-mce/). Este proyecto en su estadio básico se hizo con JUCE 6 hace unos años pero ha sido mejorado en la parte gráfica y de funcionalidad mediante el uso de vibe coding con Claude Code en junio de 2025, ver [NOTAS.md](NOTAS.md).

## Instalación macOS
1. Descarga el archivo DMG desde la página de [Releases](https://github.com/cjitter/JCBComp/releases)
2. Abre el DMG y ejecuta el instalador
3. El instalador colocará automáticamente los plugins en las ubicaciones correctas:
   - VST3: `/Library/Audio/Plug-Ins/VST3/`
   - AU: `/Library/Audio/Plug-Ins/Components/`
   - AAX: `/Library/Application Support/Avid/Audio/Plug-Ins/`

*Nota: El DMG está firmado y notarizado para macOS.*

## Requisitos del sistema

- macOS 10.13 o posterior
- Procesador Intel o Apple Silicon
- DAW compatible con VST3, AU o AAX
- Probado en: Pro Tools, Reaper, Logic, Ableton Live y Bitwig

## Compilación desde código fuente

### Requisitos previos
- Git, [CMake](https://cmake.org) 3.20 o posterior, compilador Apple Clang compatible con C++20 (incluido en Xcode 13 o superior).
- [JUCE](https://github.com/juce-framework/JUCE) 8.0.8 (se descarga automáticamente via FetchContent)
- **AAX SDK**, solo requerido para compilar formato AAX - disponible desde cuenta de desarrollador Avid, etc.

### Instrucciones de compilación

1. Clona el repositorio:
```bash
git clone https://github.com/cjitter/JCBCompressor.git
cd JCBCompressor
```

2. **Configuración del proyecto**: El archivo `CMakeLists.txt` es el generador principal del proyecto y define toda la estructura de compilación.

3. **Compilación**:
```bash
# Compilación Debug
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

# Compilación Release
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

## Características principales

- **Ajuste de entrada** (trim ±12 dB) para ambas cadenas.
- **Procesamiento sidechain** interno y externo, con filtros paso alto y bajo (Butterworth 2º/4º orden).
- **Tres modos de detección**: sharp (sliding RMS), expo RMS y slow RMS, con interpolación continua y control adicional de suavizado.
- **Ajuste independiente de reacción pico/RMS**.
- **Compresión con softknee** lineal de primer orden.
- **Controles estándar**: threshold (-60 a 0 dB), ratio (1:1 a 20:1), attack (0.1 a 250 ms), release (0.1 a 1000 ms), knee (0 a 30 dB).
- **Ganancia de compensación** (makeup gain ±12 dB) y **auto gain** (hasta 70% de recuperación).
- **Compresión paralela** mediante mezcla señal seca/comprimida.
- **Softclipping asimétrico**, aplicado tras compresión y makeup gain.
- **Control Dry/Wet** (-6 dB a 50%) para mezcla de señal post-procesado.
- **Procesamiento estéreo** con opción estéreo link.
- **Monitorización avanzada**: escucha delta (entrada/salida) y solo sidechain.
- **Visualización gráfica**: forma de onda, medidores y reducción de ganancia.
- **Gestión de presets** (usuario y fábrica).
- **Bypass interno**, independiente del DAW.
- **Formatos disponibles**: VST3, AU y AAX.

## Licencia

JCBCompressor es software libre: puedes redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General GNU publicada por la Free Software Foundation, ya sea la versión 3 de la Licencia, o (a tu elección) cualquier versión posterior. Consulta [LICENSE](LICENSE) para más detalles.

## Recursos

### Bibliografía técnica
- [Graham Wakefield & Gregory Taylor - *Generating Sound and Organizing Time*](https://cycling74.com/books/go)
- [Will C. Pirkle - *Designing Audio Effect Plugins in C++*](https://www.willpirkle.com)
- [Giannoulis, Massberg, Reiss - *Dynamic Range Compressor Design*](https://eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf)
- [Matthijs Hollemans - *The Complete Beginner's Guide to Audio Plug-in Development*](https://www.theaudioprogrammer.com/books/beginners-plugin-book)

## Por hacer

- Migrar de Plugin Export a la exportación C++ de RNBO
- Implementar oversampling con el módulo dsp de JUCE.
- Implementar modos dual y M/S
- Implementar mapeo MIDI
- Mejorar sistema de visualización entrada/salida e histograma de reducción de ganancia

## Enlaces

- **Repositorio GitHub**: [JCBCompressor](https://github.com/cjitter/JCBCompressor)
- **Documentación**: Ver [NOTAS.md](NOTAS.md) para información técnica adicional

---

*JCBCompressor v0.9.99 beta*
