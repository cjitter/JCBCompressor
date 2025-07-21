# ==============================================================================
#
#  Copyright 2025 Juan Carlos Blancas
#
#  JCBCompressor is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  JCBCompressor is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with JCBCompressor. If not, see <http://www.gnu.org/licenses/>.
#
# ==============================================================================

# Pluginval.cmake - Módulo para testing automático de plugins con pluginval

# Buscar el ejecutable de pluginval
find_program(PLUGINVAL_EXECUTABLE
    NAMES pluginval
    PATHS
        /Applications/pluginval.app/Contents/MacOS
        /usr/local/bin
        /opt/homebrew/bin
        ${CMAKE_SOURCE_DIR}/tools
    DOC "Path to pluginval executable"
)

if(PLUGINVAL_EXECUTABLE)
    message(STATUS "Found pluginval: ${PLUGINVAL_EXECUTABLE}")
else()
    message(WARNING "pluginval not found. Plugin validation tests will be skipped.")
    message(STATUS "Download pluginval from: https://github.com/Tracktion/pluginval/releases")
endif()

# Función para agregar tests de pluginval a un target
function(add_pluginval_tests target)
    if(NOT PLUGINVAL_EXECUTABLE)
        message(WARNING "Cannot add pluginval tests for ${target}: pluginval executable not found")
        return()
    endif()

    # Obtener el formato del plugin desde el nombre del target
    string(REGEX MATCH "_([A-Z0-9]+)$" format_match ${target})
    if(format_match)
        set(plugin_format ${CMAKE_MATCH_1})
    else()
        message(WARNING "Cannot determine plugin format from target name: ${target}")
        return()
    endif()

    # Verificar que el formato sea compatible con pluginval
    if(plugin_format STREQUAL "AAX")
        message(STATUS "Skipping pluginval tests for ${target}: AAX format not supported by pluginval")
        return()
    endif()

    # Definir la ruta del plugin según el formato
    if(plugin_format STREQUAL "VST3")
        set(plugin_path "${CMAKE_CURRENT_BINARY_DIR}/JCBCompressor_artefacts/$<CONFIG>/VST3/JCBCompressor.vst3")
    elseif(plugin_format STREQUAL "AU")
        set(plugin_path "${CMAKE_CURRENT_BINARY_DIR}/JCBCompressor_artefacts/$<CONFIG>/AU/JCBCompressor.component")
    else()
        message(WARNING "Unsupported plugin format for pluginval: ${plugin_format}")
        return()
    endif()

    # Agregar test básico de validación
    add_test(
        NAME pluginval_${target}_basic
        COMMAND ${PLUGINVAL_EXECUTABLE}
            --validate-in-process
            --output-dir ${CMAKE_CURRENT_BINARY_DIR}/pluginval_logs
            --timeout-ms 30000
            ${plugin_path}
    )

    # Configurar propiedades del test
    set_tests_properties(pluginval_${target}_basic PROPERTIES
        TIMEOUT 60
        LABELS "pluginval;${plugin_format}"
    )

    # El test depende de que el plugin esté construido
    set_tests_properties(pluginval_${target}_basic PROPERTIES
        FIXTURES_REQUIRED ${target}_built
    )

    # Agregar test extendido (más riguroso pero más lento)
    add_test(
        NAME pluginval_${target}_extended
        COMMAND ${PLUGINVAL_EXECUTABLE}
            --validate-in-process
            --randomise
            --timeout-ms 60000
            --repeat 3
            --output-dir ${CMAKE_CURRENT_BINARY_DIR}/pluginval_logs
            ${plugin_path}
    )

    set_tests_properties(pluginval_${target}_extended PROPERTIES
        TIMEOUT 300
        LABELS "pluginval;${plugin_format};extended"
    )

    set_tests_properties(pluginval_${target}_extended PROPERTIES
        FIXTURES_REQUIRED ${target}_built
    )

    message(STATUS "Added pluginval tests for ${target} (${plugin_format})")
endfunction()

# Función para crear fixture que asegura que el plugin esté construido antes del test
function(add_pluginval_build_fixture target)
    add_test(
        NAME setup_${target}
        COMMAND ${CMAKE_COMMAND} --build . --target ${target} --config $<CONFIG>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    set_tests_properties(setup_${target} PROPERTIES
        FIXTURES_SETUP ${target}_built
        LABELS "build"
    )
endfunction()