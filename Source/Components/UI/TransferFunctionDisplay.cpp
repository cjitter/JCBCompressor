//==============================================================================
//
//  Copyright 2025 Juan Carlos Blancas
//  This file is part of JCBCompressor and is licensed under the GNU General Public License v3.0 or later.
//
//==============================================================================

//==============================================================================
// INCLUDES
//==============================================================================
#include "TransferFunctionDisplay.h"
#include <juce_core/juce_core.h>
#include <cmath>
#include "../../Helpers/UTF8Helper.h"

//==============================================================================
// CONSTRUCTOR
//==============================================================================

TransferFunctionDisplay::TransferFunctionDisplay()
{
    setOpaque(false);  // Fondo translúcido para mejor integración visual
    
    // Inicializar buffers de waveform con valores por defecto (silencio)
    inputWaveformBuffer.resize(waveformBufferSize, -100.0f);       // Buffer entrada (-100dB = silencio)
    processedWaveformBuffer.resize(waveformBufferSize, -100.0f);   // Buffer salida procesada
    gainReductionBuffer.resize(waveformBufferSize, 0.0f);          // Buffer GR (0dB = sin reducción)
    
    // Estado inicial: sin datos de audio disponibles
    hasWaveformData.store(false);
    isSilent.store(true, std::memory_order_relaxed);
}

//==============================================================================
// OVERRIDES DE COMPONENT
//==============================================================================

void TransferFunctionDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Fondo translúcido sutil
    g.setColour(DarkTheme::backgroundMedium.withAlpha(0.3f));
    g.fillRoundedRectangle(bounds, 4.0f);
    
    // Contorno sutil
    g.setColour(DarkTheme::borderHighlight.withAlpha(0.2f));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 1.0f);
    
    // Área de gráfico (con margen para labels)
    auto graphBounds = bounds.reduced(15.0f, 10.0f);
    
    // Dibujar elementos del gráfico
    drawGrid(g, graphBounds);
    if (envelopeVisible && !soloSidechainActive && !bypassMode) {
        if (deltaMode) {
            // En modo DELTA solo mostrar gain reduction
            drawGainReductionHistory(g, graphBounds);  // Historia de reducción de ganancia
        } else {
            // Modo normal: mostrar todo
            drawWaveformAreas(g, graphBounds);  // Formas de onda de entrada y salida
            drawGainReductionHistory(g, graphBounds);  // Historia de reducción de ganancia
        }
    }
    
    // Solo mostrar elementos de compresión en modo normal
    if (!bypassMode && !deltaMode && !soloSidechainActive) {
        drawThresholdProjection(g, graphBounds);
        drawKneeArea(g, graphBounds);
        drawTransferCurve(g, graphBounds);
    }
}

void TransferFunctionDisplay::resized()
{
    // Actualizar curva cuando cambie el tamaño
    updateCurve();
}

//==============================================================================
// MÉTODOS DE ACTUALIZACIÓN DE PARÁMETROS
//==============================================================================

void TransferFunctionDisplay::setThreshold(float thresholdDb)
{
    if (threshold != thresholdDb)
    {
        threshold = thresholdDb;
        updateCurve();
    }
}

void TransferFunctionDisplay::setRatio(float newRatio)
{
    if (ratio != newRatio)
    {
        ratio = newRatio;
        updateCurve();
    }
}

void TransferFunctionDisplay::setKnee(float kneeDb)
{
    if (knee != kneeDb)
    {
        knee = kneeDb;
        updateCurve();
    }
}

void TransferFunctionDisplay::updateCurve()
{
    repaint();
}

//==============================================================================
// MÉTODOS DE CONFIGURACIÓN Y ESTADO
//==============================================================================

void TransferFunctionDisplay::setEnvelopeVisible(bool visible)
{
    if (envelopeVisible != visible)
    {
        envelopeVisible = visible;
        repaint();
    }
}

void TransferFunctionDisplay::clearWaveformData()
{
    // Limpiar todos los buffers de forma de onda
    std::fill(inputWaveformBuffer.begin(), inputWaveformBuffer.end(), -100.0f);
    std::fill(processedWaveformBuffer.begin(), processedWaveformBuffer.end(), -100.0f);
    std::fill(gainReductionBuffer.begin(), gainReductionBuffer.end(), 0.0f);
    
    // Resetear estados de envolvente
    inputEnvelopeState.store(0.0f, std::memory_order_relaxed);
    processedEnvelopeState.store(0.0f, std::memory_order_relaxed);
    
    // Resetear índice de escritura y marcar que no hay datos
    waveformWriteIndex.store(0);
    hasWaveformData.store(false);
    
    // Forzar repintado
    repaint();
}

//==============================================================================
// OVERRIDES DE TOOLTIP CLIENT
//==============================================================================

juce::String TransferFunctionDisplay::getTooltip()
{
    // El tooltip se establece desde PluginEditor usando setHelpText()
    // Este método debe retornar el helpText establecido por el componente padre
    return getHelpText();
}

void TransferFunctionDisplay::setLogicStoppedState(bool stopped)
{
    bool wasStoppedBefore = isLogicStopped.load();
    isLogicStopped = stopped;
    
    // Si Logic acaba de parar o reanudar, limpiar los buffers de visualización
    if (!wasStoppedBefore && stopped)
    {
        // Logic acaba de parar - no hacer nada, mantener la última visualización
    }
    else if (wasStoppedBefore && !stopped)
    {
        // Logic acaba de reanudar - limpiar buffers para evitar mezclar datos antiguos
        clearWaveformData();
    }
}

//==============================================================================
// MÉTODOS PRIVADOS DE DIBUJO
//==============================================================================

void TransferFunctionDisplay::drawGrid(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    g.setColour(DarkTheme::textSecondary.withAlpha(0.1f));
    
    // Ajustar las líneas de la grilla según el zoom
    std::vector<float> dbValues;
    std::vector<float> secondaryDbValues;
    
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            dbValues = { 0.0f, -6.0f, -12.0f, -20.0f, -30.0f, -40.0f, -50.0f, -60.0f, -72.0f };
            secondaryDbValues = { -3.0f, -9.0f, -15.0f, -24.0f, -36.0f, -48.0f, -66.0f };
            break;
        case ZoomLevel::Zoomed:
            dbValues = { 0.0f, -6.0f, -12.0f, -18.0f, -24.0f, -30.0f, -36.0f, -42.0f, -48.0f };
            secondaryDbValues = { -3.0f, -9.0f, -15.0f, -21.0f, -27.0f, -33.0f, -39.0f, -45.0f };
            break;
    }
    
    // Grid vertical (Input dB)
    for (float db : dbValues)
    {
        float x = juce::jmap(db, minDb, maxDb, bounds.getX(), bounds.getRight());
        g.drawVerticalLine(int(x), bounds.getY(), bounds.getBottom());
    }
    
    // Grid horizontal (Output dB - coordenadas invertidas)
    for (float db : dbValues)
    {
        float y = juce::jmap(db, minDb, maxDb, bounds.getBottom(), bounds.getY());
        g.drawHorizontalLine(int(y), bounds.getX(), bounds.getRight());
    }
    
    // Líneas secundarias más tenues
    g.setColour(DarkTheme::textSecondary.withAlpha(0.05f));
    
    for (float db : secondaryDbValues)
    {
        float x = juce::jmap(db, minDb, maxDb, bounds.getX(), bounds.getRight());
        g.drawVerticalLine(int(x), bounds.getY(), bounds.getBottom());
        
        float y = juce::jmap(db, minDb, maxDb, bounds.getBottom(), bounds.getY());
        g.drawHorizontalLine(int(y), bounds.getX(), bounds.getRight());
    }
}

void TransferFunctionDisplay::drawAxes(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(DarkTheme::textSecondary.withAlpha(0.4f));
    
    // Eje X (entrada) - parte inferior
    g.drawHorizontalLine(int(bounds.getBottom()), bounds.getX(), bounds.getRight());
    
    // Eje Y (salida) - parte izquierda  
    g.drawVerticalLine(int(bounds.getX()), bounds.getY(), bounds.getBottom());
    
    // Labels de ejes (pequeños)
    g.setColour(DarkTheme::textPrimary.withAlpha(0.6f));
    g.setFont(7.0f);
    
    // Label entrada (abajo centro)
    g.drawText("Input (dB)", bounds.getCentreX() - 25, bounds.getBottom() + 2, 50, 8, 
               juce::Justification::centred);
    
    // Label salida (izquierda centro, vertical)
    g.drawText("Output", bounds.getX() - 12, bounds.getCentreY() - 15, 12, 30, 
               juce::Justification::centred);
}

void TransferFunctionDisplay::drawThresholdProjection(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // Solo dibujar si el threshold está dentro del rango visible
    if (threshold < minDb || threshold > maxDb) return;
    
    // Calcular el mismo offset vertical que se usa en calculateOutput
    float thresholdOffset = 0.0f;
    if (knee > 0.0f && ratio > 1.0f)
    {
        thresholdOffset = knee * 0.15f * (1.0f - 1.0f / ratio);
    }
    
    // Aplicar el offset al threshold efectivo para la proyección
    float effectiveThreshold = threshold - thresholdOffset;
    
    g.setColour(DarkTheme::accent.withAlpha(0.6f));
    
    // Coordenadas del threshold efectivo
    float thresholdX = juce::jmap(threshold, minDb, maxDb, bounds.getX(), bounds.getRight());
    float thresholdY = juce::jmap(effectiveThreshold, minDb, maxDb, bounds.getBottom(), bounds.getY());
    
    // Línea punteada vertical desde el threshold hasta abajo (proyección en eje X)
    juce::Path verticalDash;
    for (float y = thresholdY; y < bounds.getBottom(); y += 4.0f) {
        verticalDash.addRectangle(thresholdX - 0.5f, y, 1.0f, 2.0f);
    }
    g.fillPath(verticalDash);
    
    // Línea punteada horizontal desde el threshold hasta la izquierda (proyección en eje Y)
    juce::Path horizontalDash;
    for (float x = bounds.getX(); x < thresholdX; x += 4.0f) {
        horizontalDash.addRectangle(x, thresholdY - 0.5f, 2.0f, 1.0f);
    }
    g.fillPath(horizontalDash);
    
    // Punto en la intersección del threshold
    g.setColour(DarkTheme::accent);
    g.fillEllipse(thresholdX - 2, thresholdY - 2, 4, 4);
}

void TransferFunctionDisplay::drawKneeArea(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    if (knee <= 0.0f) return; // No dibujar si knee es 0 (hard knee)
    
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // Calcular el mismo offset vertical que se usa en calculateOutput
    float thresholdOffset = 0.0f;
    if (knee > 0.0f && ratio > 1.0f)
    {
        thresholdOffset = knee * 0.15f * (1.0f - 1.0f / ratio);
    }
    
    // Aplicar el offset al threshold efectivo
    float effectiveThreshold = threshold - thresholdOffset;
    
    // Dibujar el área de transición suave del knee - como una sombra muy sutil
    g.setColour(DarkTheme::textSecondary.withAlpha(0.03f));
    
    // Calcular los puntos de inicio y fin del knee usando el threshold efectivo
    float kneeStart = effectiveThreshold - knee;
    float kneeEnd = effectiveThreshold + knee;
    
    // Solo dibujar la parte visible del knee
    kneeStart = juce::jmax(kneeStart, minDb);
    kneeEnd = juce::jmin(kneeEnd, maxDb);
    
    if (kneeStart >= maxDb || kneeEnd <= minDb) return; // Knee completamente fuera del rango visible
    
    // Convertir a coordenadas de píxel (usar threshold original para X, efectivo para Y)
    float kneeStartX = juce::jmap(threshold - knee, minDb, maxDb, bounds.getX(), bounds.getRight());
    float kneeEndX = juce::jmap(threshold + knee, minDb, maxDb, bounds.getX(), bounds.getRight());
    
    // Dibujar área sombreada vertical para indicar la zona de transición
    juce::Rectangle<float> kneeRect(kneeStartX, bounds.getY(), kneeEndX - kneeStartX, bounds.getHeight());
    g.fillRect(kneeRect);
    
    // Dibujar líneas verticales punteadas en los extremos del knee - aún más sutiles
    g.setColour(DarkTheme::textSecondary.withAlpha(0.05f));
    
    // Línea inicio del knee (solo si está visible)
    if (threshold - knee > minDb)
    {
        juce::Path startDash;
        for (float y = bounds.getY(); y < bounds.getBottom(); y += 4.0f) {
            startDash.addRectangle(kneeStartX - 0.5f, y, 1.0f, 2.0f);
        }
        g.fillPath(startDash);
    }
    
    // Línea fin del knee (solo si está visible)
    if (threshold + knee < maxDb)
    {
        juce::Path endDash;
        for (float y = bounds.getY(); y < bounds.getBottom(); y += 4.0f) {
            endDash.addRectangle(kneeEndX - 0.5f, y, 1.0f, 2.0f);
        }
        g.fillPath(endDash);
    }
}

void TransferFunctionDisplay::drawTransferCurve(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    juce::Path curvePath;
    bool firstPoint = true;
    
    // Generar puntos de la curva dentro del rango visible
    for (float inputDb = minDb; inputDb <= maxDb; inputDb += 0.5f)
    {
        float outputDb = calculateOutput(inputDb);
        auto point = dbToPixel(inputDb, outputDb, bounds);
        
        if (firstPoint)
        {
            curvePath.startNewSubPath(point);
            firstPoint = false;
        }
        else
        {
            curvePath.lineTo(point);
        }
    }
    
    // Solo dibujar la curva blanca si NO estamos en modos especiales
    // (BYPASS, DELTA, SOLO SC ocultan la curva para mejor lectura de textos)
    if (!bypassMode && !deltaMode && !soloSidechainActive)
    {
        // Dibujar la curva en blanco
        g.setColour(juce::Colours::white);
        g.strokePath(curvePath, juce::PathStrokeType(2.0f));
    }
}

float TransferFunctionDisplay::calculateKneeOutput(float inputDb, float threshold, float knee, float ratio)
{
    // Función auxiliar para calcular output solo en la zona del knee
    if (knee <= 0.0f || inputDb < threshold - knee || inputDb > threshold + knee)
    {
        // Fuera de la zona del knee - no debería llamarse aquí
        return inputDb;
    }
    
    // Posición en el knee normalizada de 0 a 1
    float kneePos = (inputDb - (threshold - knee)) / (2.0f * knee);
    
    // Interpolación suave usando función coseno
    float blend = 0.5f * (1.0f - std::cos(juce::MathConstants<float>::pi * kneePos));
    
    // Calcular outputs para ambos extremos
    float noCompressionOutput = inputDb;
    float fullCompressionOutput = threshold + (inputDb - threshold) / ratio;
    
    // Interpolar entre sin compresión y compresión completa
    return blend * fullCompressionOutput + (1.0f - blend) * noCompressionOutput;
}

//==============================================================================
// FUNCIONES MATEMÁTICAS DEL COMPRESOR
//==============================================================================

float TransferFunctionDisplay::calculateOutput(float inputDb)
{
    inputDb = juce::jlimit(-72.0f, 0.0f, inputDb);

    if (knee <= 0.0f)
    {
        // Hard knee
        if (inputDb <= threshold)
            return inputDb;
        else
            return threshold + (inputDb - threshold) / ratio;
    }

    // Soft knee
    float kneeHalf = knee * 0.5f;
    float kneeStart = threshold - kneeHalf;
    float kneeEnd = threshold + kneeHalf;

    if (inputDb <= kneeStart)
    {
        // Línea 1:1 (sin compresión)
        return inputDb;
    }
    else if (inputDb >= kneeEnd)
    {
        // Línea comprimida
        return threshold + (inputDb - threshold) / ratio;
    }
    else
    {
        // Dentro del knee: interpolación hermite cúbica
        float t = (inputDb - kneeStart) / knee; // normalizar 0-1

        // Puntos de conexión exactos
        float y0 = kneeStart;  // punto inicial (línea 1:1)
        float y1 = threshold + (kneeEnd - threshold) / ratio;  // punto final (línea comprimida)

        // Derivadas en los extremos (pendientes de las líneas)
        float m0 = 1.0f;  // pendiente línea 1:1
        float m1 = 1.0f / ratio;  // pendiente línea comprimida

        // Interpolación Hermite que respeta puntos y derivadas
        float t2 = t * t;
        float t3 = t2 * t;

        float h00 = 2*t3 - 3*t2 + 1;
        float h10 = t3 - 2*t2 + t;
        float h01 = -2*t3 + 3*t2;
        float h11 = t3 - t2;

        return h00 * y0 + h10 * knee * m0 + h01 * y1 + h11 * knee * m1;
    }
}


juce::Point<float> TransferFunctionDisplay::dbToPixel(float inputDb, float outputDb, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // X: Input va de izquierda (minDb) a derecha (maxDb)
    float x = juce::jmap(inputDb, minDb, maxDb, bounds.getX(), bounds.getRight());
    
    // Y: Output va de abajo (minDb) a arriba (maxDb) - coordenadas invertidas
    float y = juce::jmap(outputDb, minDb, maxDb, bounds.getBottom(), bounds.getY());
    
    return {x, y};
}

juce::Point<float> TransferFunctionDisplay::pixelToDb(juce::Point<float> pixel, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // Convertir coordenadas de píxel a dB
    float inputDb = juce::jmap(pixel.x, bounds.getX(), bounds.getRight(), minDb, maxDb);
    float outputDb = juce::jmap(pixel.y, bounds.getBottom(), bounds.getY(), minDb, maxDb);
    
    return {inputDb, outputDb};
}

//==============================================================================
// MÉTODOS DE DATOS DE WAVEFORM (THREAD-SAFE)
//==============================================================================

void TransferFunctionDisplay::updateWaveformData(const float* inputSamples, const float* processedSamples, int numSamples)
{
    if (numSamples <= 0) return;
    
    int writeIndex = waveformWriteIndex.load();
    
    // Verificar si hay señal o silencio
    float maxSignalLevel = 0.0f;
    for (int i = 0; i < numSamples; i += 64)  // Chequeo rápido cada 64 muestras
    {
        maxSignalLevel = juce::jmax(maxSignalLevel, std::abs(inputSamples[i]));
        if (maxSignalLevel > silenceLevel) break;
    }
    
    // Actualizar contador de silencio
    if (maxSignalLevel < silenceLevel)
    {
        int currentSilenceCounter = silenceCounter.load(std::memory_order_relaxed);
        silenceCounter.store(currentSilenceCounter + 1, std::memory_order_relaxed);
        
        if (currentSilenceCounter > silenceThreshold)
        {
            isSilent.store(true, std::memory_order_relaxed);
            // Comenzar fade out progresivo
            float currentFade = fadeOutFactor.load(std::memory_order_relaxed);
            fadeOutFactor.store(currentFade * 0.95f, std::memory_order_relaxed);  // Desvanecimiento gradual
            
            // NO limpiar el buffer inmediatamente, solo aplicar fade
            float currentInputEnv = inputEnvelopeState.load(std::memory_order_relaxed);
            float currentProcessedEnv = processedEnvelopeState.load(std::memory_order_relaxed);
            inputEnvelopeState.store(currentInputEnv * 0.95f, std::memory_order_relaxed);
            processedEnvelopeState.store(currentProcessedEnv * 0.95f, std::memory_order_relaxed);
            
            // Solo limpiar cuando el fade es muy bajo
            if (currentFade < 0.05f)
            {
                fadeOutFactor.store(0.0f, std::memory_order_relaxed);
                // Mantener los datos pero con fade completo
            }
        }
    }
    else
    {
        silenceCounter.store(0, std::memory_order_relaxed);
        isSilent.store(false, std::memory_order_relaxed);
        fadeOutFactor.store(1.0f, std::memory_order_relaxed);  // Restaurar opacidad completa
        hasWaveformData.store(true);
    }
    
    // Procesar TODO el bloque de una vez para obtener un solo valor de envolvente
    float maxInput = 0.0f;
    float maxProcessed = 0.0f;
    
    // Obtener el máximo del bloque completo
    // NOTA: Intercambiamos input y processed porque vienen invertidos del processor
    for (int i = 0; i < numSamples; ++i)
    {
        maxInput = juce::jmax(maxInput, std::abs(processedSamples[i]));  // processed es realmente la entrada post-TRIM
        maxProcessed = juce::jmax(maxProcessed, std::abs(inputSamples[i]));  // input es realmente la salida procesada
    }
    
    // Si estamos en silencio, aplicar fade progresivo
    if (isSilent.load(std::memory_order_relaxed))
    {
        float currentFade = fadeOutFactor.load(std::memory_order_relaxed);
        maxInput *= currentFade;
        maxProcessed *= currentFade;
    }
    
    // Detectar cambios rápidos en la señal
    float prevLevel = previousInputLevel.load(std::memory_order_relaxed);
    float inputChange = std::abs(maxInput - prevLevel);
    float currentDetector = changeDetector.load(std::memory_order_relaxed);
    changeDetector.store(currentDetector * 0.9f + inputChange * 0.1f, std::memory_order_relaxed);
    previousInputLevel.store(maxInput, std::memory_order_relaxed);
    
    // Cambiar a modo rápido si detectamos transientes
    useFastMode.store(currentDetector > 0.1f, std::memory_order_relaxed);
    
    // Parámetros para envelope más "encrespado" y abrupto
    float releaseTime = 0.3f;   // Release MUY rápido para máxima textura
    
    // Filtro de envolvente mínimo para mantener el carácter "encrespado"
    {
        // Para entrada - seguimiento directo sin ruido artificial
        float clampedInput = juce::jmin(maxInput, 0.99f); // Evitar que supere 0 dBFS
        float currentInputEnv = inputEnvelopeState.load(std::memory_order_relaxed);
        
        if (clampedInput > currentInputEnv * 1.1f)  // Umbral más bajo para más respuesta
        {
            // Attack instantáneo
            inputEnvelopeState.store(clampedInput, std::memory_order_relaxed);
        }
        else
        {
            // Release muy abrupto para mantener detalles
            float newValue = currentInputEnv * releaseTime + clampedInput * (1.0f - releaseTime);
            
            // Corte abrupto en silencio
            if (newValue < 0.001f) newValue = 0.0f;
            inputEnvelopeState.store(newValue, std::memory_order_relaxed);
        }
        
        // Para salida procesada - igual de directo sin ruido
        float currentProcessedEnv = processedEnvelopeState.load(std::memory_order_relaxed);
        
        if (maxProcessed > currentProcessedEnv * 1.1f)
        {
            // Attack instantáneo
            processedEnvelopeState.store(maxProcessed, std::memory_order_relaxed);
        }
        else
        {
            // Mezclar con la señal actual para más detalle
            float newValue = currentProcessedEnv * releaseTime + maxProcessed * (1.0f - releaseTime);
            
            // Corte abrupto
            if (newValue < 0.001f) newValue = 0.0f;
            processedEnvelopeState.store(newValue, std::memory_order_relaxed);
        }
        
        // Re-leer los valores finales para convertir a dB
        float finalInputEnv = inputEnvelopeState.load(std::memory_order_relaxed);
        float finalProcessedEnv = processedEnvelopeState.load(std::memory_order_relaxed);
        
        // Convertir a dB sin ruido artificial
        float inputDb = finalInputEnv > 0.0001f ? 
            20.0f * std::log10(finalInputEnv) : -80.0f;
        float processedDb = finalProcessedEnv > 0.0001f ? 
            20.0f * std::log10(finalProcessedEnv) : -80.0f;
        
        // Limitar al rango del gráfico (-80 dB a 0 dB)
        // Permitir llegar hasta 0 dB para consistencia con el rango visual
        inputDb = juce::jlimit(-80.0f, 0.0f, inputDb);
        processedDb = juce::jlimit(-80.0f, 0.0f, processedDb);
        
        inputWaveformBuffer[writeIndex] = inputDb;
        processedWaveformBuffer[writeIndex] = processedDb;
        
        // Calcular y almacenar la reducción de ganancia (positivo = reducción)
        float gainReduction = inputDb - processedDb;
        
        // Condición especial para ratio 1:1 - no debe haber gain reduction
        if (ratio <= 1.01f) // Pequeño margen para errores de punto flotante
        {
            gainReduction = 0.0f;
            // Forzar que las envolventes sean idénticas cuando ratio es 1:1
            processedDb = inputDb;
            processedWaveformBuffer[writeIndex] = inputDb;
        }
        
        // Verificar si la señal está por debajo del threshold (considerando knee)
        // Si la señal está por debajo de (threshold - knee), no debería haber compresión
        if (inputDb < (threshold - knee))
        {
            gainReduction = 0.0f;
            // Forzar que las envolventes sean idénticas cuando no hay compresión
            processedDb = inputDb;
            processedWaveformBuffer[writeIndex] = inputDb;
        }
        
        // Verificar si EXT KEY está activo pero no hay señal en sidechain
        // En este caso, no debería haber compresión visible
        if (extKeyActive && sidechainLevel < -60.0f)
        {
            gainReduction = 0.0f;
            processedDb = inputDb;
            processedWaveformBuffer[writeIndex] = inputDb;
        }
        
        
        gainReductionBuffer[writeIndex] = juce::jmax(0.0f, gainReduction);
    }
    
    // Solo escribir UN valor por llamada (no múltiples)
    writeIndex = (writeIndex + 1) % waveformBufferSize;
    
    waveformWriteIndex.store(writeIndex);
    hasWaveformData.store(true);
}

void TransferFunctionDisplay::updateWaveformDataWithGR(const float* inputSamples, const float* processedSamples, const float* gainReductionSamples, int numSamples)
{
    if (numSamples <= 0) return;
    
    int writeIndex = waveformWriteIndex.load();
    
    // Procesar todas las muestras disponibles
    for (int i = 0; i < numSamples; i++)
    {
        // Encontrar el valor máximo en las muestras (detector de picos)
        float maxInput = std::abs(inputSamples[i]);
        float maxProcessed = std::abs(processedSamples[i]);
        
        // Actualizar el estado del envelope follower
        // Similar al método anterior pero usando la GR real de Gen~
        
        // Detectar cambios rápidos en la señal
        float inputChange = std::abs(maxInput - previousInputLevel);
        changeDetector = changeDetector * 0.9f + inputChange * 0.1f;
        previousInputLevel = maxInput;
        
        // Cambiar a modo rápido si detectamos transientes
        useFastMode = changeDetector > 0.1f;
        
        // Parámetros independientes para diferentes tipos de visualización
        float envelopeReleaseTime = 0.3f;   // Release rápido para envolventes (comportamiento original)
        
        // Factor de suavizado por defecto para gain reduction (valor medio)
        float grSmoothingFactor = 0.7f;  // Equivalente a ~100ms de release
        
        // TEMPORAL DEBUG - BYPASS ENVELOPE FILTERING
        /*
        // Filtro de envolvente original (COMENTADO TEMPORALMENTE)
        {
            // Para entrada - seguimiento directo sin ruido artificial
            float clampedInput = juce::jmin(maxInput, 0.99f); // Evitar que supere 0 dBFS
            float currentInputEnv = inputEnvelopeState.load(std::memory_order_relaxed);
            
            if (clampedInput > currentInputEnv * 1.1f)  // Umbral más bajo para más respuesta
            {
                // Attack instantáneo
                inputEnvelopeState.store(clampedInput, std::memory_order_relaxed);
            }
            else
            {
                // Release muy abrupto para mantener detalles
                float newValue = currentInputEnv * envelopeReleaseTime + clampedInput * (1.0f - envelopeReleaseTime);
                
                // Corte abrupto en silencio
                if (newValue < 0.001f) newValue = 0.0f;
                inputEnvelopeState.store(newValue, std::memory_order_relaxed);
            }
            
            // Para salida procesada - igual de directo sin ruido
            float currentProcessedEnv = processedEnvelopeState.load(std::memory_order_relaxed);
            
            if (maxProcessed > currentProcessedEnv * 1.1f)
            {
                // Attack instantáneo
                processedEnvelopeState.store(maxProcessed, std::memory_order_relaxed);
            }
            else
            {
                // Mezclar con la señal actual para más detalle
                float newValue = currentProcessedEnv * envelopeReleaseTime + maxProcessed * (1.0f - envelopeReleaseTime);
                
                // Corte abrupto
                if (newValue < 0.001f) newValue = 0.0f;
                processedEnvelopeState.store(newValue, std::memory_order_relaxed);

            }
            
            // Re-leer los valores finales para convertir a dB
            float finalInputEnv = inputEnvelopeState.load(std::memory_order_relaxed);
            float finalProcessedEnv = processedEnvelopeState.load(std::memory_order_relaxed);
            
            // Convertir a dB sin ruido artificial
            float inputDb = finalInputEnv > 0.0001f ? 
                20.0f * std::log10(finalInputEnv) : -80.0f;
            float processedDb = finalProcessedEnv > 0.0001f ? 
                20.0f * std::log10(finalProcessedEnv) : -80.0f;
            
            // Limitar al rango del gráfico (-80 dB a 0 dB)
            inputDb = juce::jlimit(-80.0f, 0.0f, inputDb);
            processedDb = juce::jlimit(-80.0f, 0.0f, processedDb);
            
            inputWaveformBuffer[writeIndex] = inputDb;
            processedWaveformBuffer[writeIndex] = processedDb;
        }
        */
        
        // DIRECT CONVERSION - NO FILTERING (DEBUG)
        {
            float inputDb = maxInput > 0.0001f ? 
                20.0f * std::log10(maxInput) : -80.0f;
            float processedDb = maxProcessed > 0.0001f ? 
                20.0f * std::log10(maxProcessed) : -80.0f;
            
            inputWaveformBuffer[writeIndex] = inputDb;
            processedWaveformBuffer[writeIndex] = processedDb;
            
            // USAR LA GAIN REDUCTION REAL DE GEN~ EN LUGAR DE CALCULARLA
            float gainReduction = -gainReductionSamples[i]; // Negativo porque Gen~ da valores negativos en dB
            
            // Condición especial para ratio 1:1 - no debe haber gain reduction
            if (ratio <= 1.01f) // Pequeño margen para errores de punto flotante
            {
                gainReduction = 0.0f;
                // Forzar que las envolventes sean idénticas cuando ratio es 1:1
                processedDb = inputDb;
                processedWaveformBuffer[writeIndex] = inputDb;
            }
            
            // Verificar si la señal está por debajo del threshold (considerando knee)
            // Si la señal está por debajo de (threshold - knee), no debería haber compresión
            if (inputDb < (threshold - knee))
            {
                gainReduction = 0.0f;
                // Forzar que las envolventes sean idénticas cuando no hay compresión
                processedDb = inputDb;
                processedWaveformBuffer[writeIndex] = inputDb;
            }
            
            // Verificar si EXT KEY está activo pero no hay señal en sidechain
            // En este caso, no debería haber compresión visible
            if (extKeyActive && sidechainLevel < -60.0f)
            {
                gainReduction = 0.0f;
                processedDb = inputDb;
                processedWaveformBuffer[writeIndex] = inputDb;
            }
            
            // Aplicar suavizado al gain reduction para visualización más estable
            gainReduction = juce::jmax(0.0f, gainReduction);
            
            // Factor de suavizado por defecto para attack del gain reduction
            float grAttackFactor = 0.5f;  // Valor medio para respuesta equilibrada
            
            // Suavizado del gain reduction con factores independientes para attack y release
            float currentGRSmoothed = gainReductionSmoothed.load(std::memory_order_relaxed);
            float newGRSmoothed;
            
            if (gainReduction > currentGRSmoothed)
            {
                // Attack con suavizado basado en el parámetro Attack
                newGRSmoothed = currentGRSmoothed * grAttackFactor + 
                              gainReduction * (1.0f - grAttackFactor);
            }
            else
            {
                // Release suavizado para evitar saltos
                newGRSmoothed = currentGRSmoothed * grSmoothingFactor + 
                              gainReduction * (1.0f - grSmoothingFactor);
            }
            
            gainReductionSmoothed.store(newGRSmoothed, std::memory_order_relaxed);
            gainReductionBuffer[writeIndex] = newGRSmoothed;
        }
        
        // Solo escribir UN valor por llamada (no múltiples)
        writeIndex = (writeIndex + 1) % waveformBufferSize;
    }
    
    waveformWriteIndex.store(writeIndex);
    hasWaveformData.store(true);
}

void TransferFunctionDisplay::drawWaveformAreas(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    if (!hasWaveformData.load()) return;
    
    // Si Logic está parado, no dibujar las envolventes
    if (isLogicStopped.load()) return;
    
    const int readIndex = waveformWriteIndex.load();
    const float currentFadeOutFactor = fadeOutFactor.load(std::memory_order_relaxed);
    
    // NO expandir bounds - mantener dentro del área del gráfico
    // bounds = bounds.expanded(60.0f, 10.0f);  // COMENTADO - causaba el bloque azul
    
    // Determinar el rango de dB según el nivel de zoom (una sola vez, fuera del loop)
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // Crear paths para áreas rellenas
    juce::Path inputAreaPath;
    juce::Path processedAreaPath;   // Salida procesada
    
    // Línea base (parte inferior del gráfico) - con pequeño offset para evitar artefactos
    const float baseLine = bounds.getBottom() - 1.0f;
    
    // Colectar todos los puntos primero para suavizado
    std::vector<juce::Point<float>> inputPoints;
    std::vector<juce::Point<float>> processedPoints;    // Salida procesada
    
    // Mostrar los últimos 'displayPoints' valores del buffer circular con menos delay
    for (int i = 0; i < displayPoints; ++i)
    {
        // Leer con offset mínimo para entrada instantánea
        int samplesBack = displayPoints - i + 5;  // Offset mínimo para sincronía audio-visual
        int bufferIndex = (readIndex - samplesBack + waveformBufferSize) % waveformBufferSize;
        
        float inputDb = inputWaveformBuffer[bufferIndex];
        float processedDb = processedWaveformBuffer[bufferIndex];
        
        // Si los valores son muy bajos (silencio), clampear al mínimo del rango visible
        if (inputDb < minDb) inputDb = minDb;
        if (processedDb < minDb) processedDb = minDb;
        
        // Calcular posición X - usar todo el ancho disponible
        float normalizedTime = float(i) / float(displayPoints - 1);
        float x = bounds.getX() + normalizedTime * bounds.getWidth();
        
        // Calcular posiciones Y usando el rango de zoom actual
        // Añadir un pequeño offset para evitar que las envolventes toquen exactamente el borde superior
        float topOffset = 2.0f; // 2 píxeles de margen desde el borde superior
        float inputY = juce::jmap(inputDb, minDb, maxDb, bounds.getBottom(), bounds.getY() + topOffset);
        float processedY = juce::jmap(processedDb, minDb, maxDb, bounds.getBottom(), bounds.getY() + topOffset);
        
        // Amplificar visualmente la diferencia cuando hay reducción
        float reduction = inputDb - processedDb; // Positivo cuando hay reducción
        // Solo aplicar amplificación visual si el ratio es mayor que 1:1
        if (reduction > 0.05f && ratio > 1.01f) {
            // Factor de amplificación variable según la cantidad de reducción
            // Más amplificación para reducciones pequeñas (para hacerlas más visibles)
            float visualAmplification;
            if (reduction < 3.0f) {
                // Para reducciones pequeñas (0-3dB), amplificación aumentada
                visualAmplification = 3.0f;
            } else if (reduction < 6.0f) {
                // Para reducciones medias (3-6dB), amplificación moderada
                visualAmplification = 2.5f;
            } else {
                // Para reducciones grandes (>6dB), menos amplificación
                visualAmplification = 2.0f;
            }
            
            float midPoint = (inputY + processedY) * 0.5f;
            float separation = (inputY - processedY) * visualAmplification;
            
            // Mantener los valores dentro de los límites
            processedY = juce::jlimit(bounds.getY(), bounds.getBottom(), 
                                     midPoint + separation * 0.5f);
            inputY = juce::jlimit(bounds.getY(), bounds.getBottom(), 
                                 midPoint - separation * 0.5f);
        }
        
        // Sin fade en los extremos - usar todo el ancho disponible
        // Limitar con pequeño margen para evitar artefactos en los bordes
        inputY = juce::jlimit(bounds.getY() + 1.0f, bounds.getBottom() - 1.0f, inputY);
        processedY = juce::jlimit(bounds.getY() + 1.0f, bounds.getBottom() - 1.0f, processedY);
        
        inputPoints.push_back({x, inputY});
        processedPoints.push_back({x, processedY});
    }
    
    // Crear path suavizado para salida procesada
    if (!processedPoints.empty())
    {
        processedAreaPath.startNewSubPath(processedPoints[0].x, baseLine);
        processedAreaPath.lineTo(processedPoints[0]);
        
        // Interpolación lineal simple para evitar artefactos
        for (size_t i = 1; i < processedPoints.size(); ++i)
        {
            processedAreaPath.lineTo(processedPoints[i]);
        }
        
        processedAreaPath.lineTo(processedPoints.back().x, baseLine);
        processedAreaPath.closeSubPath();
    }
    
    // Crear path suavizado para entrada
    if (!inputPoints.empty())
    {
        inputAreaPath.startNewSubPath(inputPoints[0].x, baseLine);
        inputAreaPath.lineTo(inputPoints[0]);
        
        // Interpolación lineal simple para evitar artefactos
        for (size_t i = 1; i < inputPoints.size(); ++i)
        {
            inputAreaPath.lineTo(inputPoints[i]);
        }
        
        inputAreaPath.lineTo(inputPoints.back().x, baseLine);
        inputAreaPath.closeSubPath();
    }
    
    // Dibujar sin máscara para usar todo el ancho
    
    // 1. PRIMERO dibujar área de entrada (DETRÁS) - Morado translúcido
    // Se dibuja primero para que la salida azul se vea encima cuando haya compresión
    if (!processedAreaPath.isEmpty())
    {
        // Gradiente vertical para entrada - MORADO MÁS TRANSLÚCIDO
        juce::ColourGradient processedGradient(
            juce::Colour(0x9C, 0x27, 0xB0).withAlpha(0.25f * currentFadeOutFactor), bounds.getCentreX(), bounds.getY(),
            juce::Colour(0x7B, 0x1F, 0xA2).withAlpha(0.15f * currentFadeOutFactor), bounds.getCentreX(), bounds.getBottom(),
            false
        );
        g.setGradientFill(processedGradient);
        g.fillPath(processedAreaPath);
        
        // Línea superior morada para entrada - más sutil
        g.setColour(juce::Colour(0x9C, 0x27, 0xB0).withAlpha(0.4f * currentFadeOutFactor));
        juce::Path processedLine;
        if (!processedPoints.empty())
        {
            processedLine.startNewSubPath(processedPoints[0]);
            for (size_t i = 1; i < processedPoints.size(); ++i)
            {
                processedLine.lineTo(processedPoints[i]);
            }
            g.strokePath(processedLine, juce::PathStrokeType(1.5f));  // Línea más gruesa para entrada
        }
    }
    
    // 2. DESPUÉS dibujar área de salida procesada (ENCIMA) - Usando gradiente del medidor de salida
    // Se dibuja encima para mostrar la compresión aplicada
    if (!inputAreaPath.isEmpty())
    {
        // Usar el mismo gradiente que los medidores de salida
        const juce::Colour outputBlue = juce::Colour(0xFF6495ED);     // Azul de OUTPUT
        const juce::Colour darkPurple = juce::Colour(0xFF202245);     // Morado oscuro personalizado
        const juce::Colour deepBlue = juce::Colour(0xFF0D3B52);       // Azul profundo intermedio
        
        // Crear gradiente con transparencia para la envolvente
        auto outputGradient = juce::ColourGradient(
            outputBlue.withAlpha(0.6f * currentFadeOutFactor), bounds.getCentreX(), bounds.getY(),
            darkPurple.withAlpha(0.4f * currentFadeOutFactor), bounds.getCentreX(), bounds.getBottom(),
            false
        );
        outputGradient.addColour(0.15, outputBlue.withAlpha(0.58f * currentFadeOutFactor));  // Azul claro se mantiene más arriba
        outputGradient.addColour(0.4, outputBlue.interpolatedWith(deepBlue, 0.3f).withAlpha(0.55f * currentFadeOutFactor));  // Transición más suave
        outputGradient.addColour(0.65, deepBlue.withAlpha(0.5f * currentFadeOutFactor));
        outputGradient.addColour(0.85, deepBlue.interpolatedWith(darkPurple, 0.5f).withAlpha(0.45f * currentFadeOutFactor));
        
        g.setGradientFill(outputGradient);
        g.fillPath(inputAreaPath);
        
        // Línea superior con el color azul OUTPUT
        g.setColour(outputBlue.withAlpha(0.95f * currentFadeOutFactor));
        juce::Path inputLine;
        if (!inputPoints.empty())
        {
            inputLine.startNewSubPath(inputPoints[0]);
            for (size_t i = 1; i < inputPoints.size(); ++i)
            {
                inputLine.lineTo(inputPoints[i]);
            }
            g.strokePath(inputLine, juce::PathStrokeType(1.0f));  // Línea más delgada para salida
        }
    }
    
}

//==============================================================================
// Eventos de mouse

void TransferFunctionDisplay::mouseEnter(const juce::MouseEvent&)
{
    // Tooltip ya está configurado en el constructor
}

void TransferFunctionDisplay::mouseExit(const juce::MouseEvent&)
{
    // Resetear cursor
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void TransferFunctionDisplay::mouseDown(const juce::MouseEvent& e)
{
    auto bounds = getLocalBounds().toFloat().reduced(15.0f, 10.0f);
    auto mousePos = e.position;
    
    // Detectar qué elemento se está arrastrando
    currentDragMode = detectDragMode(mousePos, bounds);
    lastMousePos = mousePos;
    
    // Guardar valor inicial para el drag
    switch (currentDragMode) {
        case DragMode::Threshold:
            dragStartValue = threshold;
            break;
        case DragMode::Knee:
            dragStartValue = knee;
            break;
        case DragMode::Ratio:
            dragStartValue = ratio;
            break;
        default:
            break;
    }
}

void TransferFunctionDisplay::mouseDrag(const juce::MouseEvent& e)
{
    if (currentDragMode == DragMode::None) return;
    
    auto bounds = getLocalBounds().toFloat().reduced(15.0f, 10.0f);
    auto mousePos = e.position;
    auto deltaPixels = mousePos - lastMousePos;
    
    switch (currentDragMode) {
        case DragMode::Threshold: {
            // Arrastrar horizontalmente para cambiar threshold
            float deltaDb = juce::jmap(deltaPixels.x, 0.0f, bounds.getWidth(), 0.0f, 60.0f);
            float newThreshold = juce::jlimit(-60.0f, 0.0f, threshold + deltaDb);
            setThreshold(newThreshold);
            if (onThresholdChange) onThresholdChange(newThreshold);
            break;
        }
        case DragMode::Knee: {
            // Arrastrar horizontalmente para cambiar knee
            float deltaDb = juce::jmap(deltaPixels.x, 0.0f, bounds.getWidth(), 0.0f, 30.0f);
            float newKnee = juce::jlimit(0.0f, 20.0f, knee + deltaDb);
            setKnee(newKnee);
            if (onKneeChange) onKneeChange(newKnee);
            break;
        }
        case DragMode::Ratio: {
            // Arrastrar verticalmente para cambiar ratio
            float deltaRatio = juce::jmap(deltaPixels.y, 0.0f, bounds.getHeight(), 0.0f, 20.0f);
            float newRatio = juce::jlimit(1.0f, 20.0f, ratio + deltaRatio);
            setRatio(newRatio);
            if (onRatioChange) onRatioChange(newRatio);
            break;
        }
        default:
            break;
    }
    
    lastMousePos = mousePos;
}

void TransferFunctionDisplay::mouseUp(const juce::MouseEvent&)
{
    currentDragMode = DragMode::None;
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

//==============================================================================
// Métodos de detección

TransferFunctionDisplay::DragMode TransferFunctionDisplay::detectDragMode(juce::Point<float> mousePos, juce::Rectangle<float> bounds)
{
    // Prioridad: Threshold > Knee > Ratio
    if (isNearThresholdLine(mousePos, bounds)) {
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        return DragMode::Threshold;
    }
    if (isNearKneeArea(mousePos, bounds)) {
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        return DragMode::Knee;
    }
    if (isNearTransferCurve(mousePos, bounds)) {
        setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
        return DragMode::Ratio;
    }
    
    setMouseCursor(juce::MouseCursor::NormalCursor);
    return DragMode::None;
}

bool TransferFunctionDisplay::isNearThresholdLine(juce::Point<float> mousePos, juce::Rectangle<float> bounds)
{
    // Determinar el rango de dB según el nivel de zoom
    float minDb, maxDb;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            minDb = -72.0f;
            maxDb = 0.0f;
            break;
        case ZoomLevel::Zoomed:
            minDb = -48.0f;
            maxDb = 0.0f;
            break;
    }
    
    // Solo verificar si el threshold está dentro del rango visible
    if (threshold < minDb || threshold > maxDb) return false;
    
    float thresholdX = juce::jmap(threshold, minDb, maxDb, bounds.getX(), bounds.getRight());
    return std::abs(mousePos.x - thresholdX) < 8.0f; // Tolerancia de 8 píxeles
}

bool TransferFunctionDisplay::isNearKneeArea(juce::Point<float> mousePos, juce::Rectangle<float> bounds)
{
    if (knee <= 0.0f) return false; // No hay área de knee si es hard knee
    
    // Verificar si el mouse está en la zona del knee
    auto dbPos = pixelToDb(mousePos, bounds);
    float kneeStart = threshold - knee;
    float kneeEnd = threshold + knee;
    
    // Verificar si está dentro del rango horizontal del knee
    return dbPos.x >= kneeStart && dbPos.x <= kneeEnd;
}

bool TransferFunctionDisplay::isNearTransferCurve(juce::Point<float> mousePos, juce::Rectangle<float> bounds)
{
    // Calcular si el mouse está cerca de la curva de transferencia
    auto dbPos = pixelToDb(mousePos, bounds);
    float expectedOutput = calculateOutput(dbPos.x);
    float distanceToCurve = std::abs(dbPos.y - expectedOutput);
    return distanceToCurve < 4.0f; // Tolerancia de 4 dB
}

void TransferFunctionDisplay::drawGainReductionHistory(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    if (!hasWaveformData.load()) return;
    
    // Si Logic está parado, no dibujar el histograma
    if (isLogicStopped.load()) return;
    
    const int readIndex = waveformWriteIndex.load();
    const float currentFadeOutFactor = fadeOutFactor.load(std::memory_order_relaxed);
    
    // Determinar el rango de gain reduction según el nivel de zoom
    float maxGainReduction;
    bool useNonLinearScale = false;
    switch (currentZoom)
    {
        case ZoomLevel::Normal:
            maxGainReduction = 72.0f;  // Mostrar todo el rango posible
            break;
        case ZoomLevel::Zoomed:
            maxGainReduction = 48.0f;  // Mostrar hasta 48 dB en modo zoom
            break;
            maxGainReduction = 72.0f;  // Usar todo el rango pero con escala no lineal
            useNonLinearScale = true;
            break;
    }
    
    // Crear path para la línea de reducción de ganancia
    juce::Path grPath;
    std::vector<juce::Point<float>> grPoints;
    
    // Recolectar puntos de reducción de ganancia
    for (int i = 0; i < displayPoints; ++i)
    {
        int samplesBack = displayPoints - i + 5;
        int bufferIndex = (readIndex - samplesBack + waveformBufferSize) % waveformBufferSize;
        
        float grDb = gainReductionBuffer[bufferIndex];
        
        // Posición X - usar todo el ancho
        float normalizedTime = float(i) / float(displayPoints - 1);
        float x = bounds.getX() + normalizedTime * bounds.getWidth();
        
        // Posición Y - mapear GR al rango vertical según el zoom
        float normalizedGR = grDb / maxGainReduction;
        float y;
        
        if (useNonLinearScale)
        {
            // Aplicar escala no lineal para expandir visualmente el rango 0 a -6 dB
            // pow con exponente < 1 expande los valores pequeños
            float expandedGR = std::pow(normalizedGR, 0.75f);  // pow 0.75 para expansión menos extrema
            y = bounds.getY() + expandedGR * bounds.getHeight();
        }
        else
        {
            // Escala lineal normal
            y = bounds.getY() + normalizedGR * bounds.getHeight();
        }
        
        grPoints.push_back({x, y});
    }
    
    // Crear path suavizado
    if (!grPoints.empty())
    {
        if (deltaMode)
        {
            // En modo DELTA: usar color verde (mismo que el fondo de delta)
            // Color verde tipo teal/cyan
            auto deltaGreen = juce::Colour(0x00, 0xC8, 0x96);  // Verde turquesa
            
            // Crear gradiente verde para el área
            juce::ColourGradient grGradient(
                deltaGreen.withAlpha(0.6f * currentFadeOutFactor), bounds.getX(), bounds.getY(),
                deltaGreen.darker(0.3f).withAlpha(0.3f * currentFadeOutFactor), bounds.getRight(), bounds.getY(),
                false
            );
            
            // Dibujar área bajo la curva - empezar sin offset
            grPath.startNewSubPath(grPoints[0].x, bounds.getY());
            grPath.lineTo(grPoints[0]);
            
            // Suavizado con spline
            for (size_t i = 1; i < grPoints.size(); ++i)
            {
                if (i < grPoints.size() - 1)
                {
                    auto p0 = (i > 1) ? grPoints[i - 2] : grPoints[i - 1];
                    auto p1 = grPoints[i - 1];
                    auto p2 = grPoints[i];
                    auto p3 = (i < grPoints.size() - 1) ? grPoints[i + 1] : grPoints[i];
                    
                    for (float t = 0.0f; t < 1.0f; t += 0.2f)
                    {
                        float t2 = t * t;
                        float t3 = t2 * t;
                        
                        float x = 0.5f * ((2.0f * p1.x) + 
                                         (-p0.x + p2.x) * t +
                                         (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                                         (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);
                        
                        float y = 0.5f * ((2.0f * p1.y) +
                                         (-p0.y + p2.y) * t +
                                         (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                                         (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);
                        
                        grPath.lineTo(x, y);
                    }
                }
                else
                {
                    grPath.lineTo(grPoints[i]);
                }
            }
            
            grPath.lineTo(grPoints.back().x, bounds.getY());
            grPath.closeSubPath();
            
            // En modo DELTA: dibujar área rellena verde
            g.setGradientFill(grGradient);
            g.fillPath(grPath);
            
            // Dibujar la línea verde más brillante
            juce::Path grLine;
            grLine.startNewSubPath(grPoints[0]);
            for (size_t i = 1; i < grPoints.size(); ++i)
            {
                grLine.lineTo(grPoints[i]);
            }
            g.setColour(deltaGreen.withAlpha(0.95f * currentFadeOutFactor));
            g.strokePath(grLine, juce::PathStrokeType(2.5f)); // Línea más gruesa
        }
        else
        {
            // Modo normal: usar colores morados originales
            juce::ColourGradient grGradient(
                DarkTheme::accentSecondary.withAlpha(0.5f * currentFadeOutFactor), bounds.getX(), bounds.getY(),
                DarkTheme::accentSecondary.darker(0.3f).withAlpha(0.3f * currentFadeOutFactor), bounds.getRight(), bounds.getY(),
                false
            );
            
            // Dibujar área bajo la curva - empezar sin offset
            grPath.startNewSubPath(grPoints[0].x, bounds.getY());
            grPath.lineTo(grPoints[0]);
            
            // Suavizado con spline
            for (size_t i = 1; i < grPoints.size(); ++i)
            {
                if (i < grPoints.size() - 1)
                {
                    auto p0 = (i > 1) ? grPoints[i - 2] : grPoints[i - 1];
                    auto p1 = grPoints[i - 1];
                    auto p2 = grPoints[i];
                    auto p3 = (i < grPoints.size() - 1) ? grPoints[i + 1] : grPoints[i];
                    
                    for (float t = 0.0f; t < 1.0f; t += 0.2f)
                    {
                        float t2 = t * t;
                        float t3 = t2 * t;
                        
                        float x = 0.5f * ((2.0f * p1.x) + 
                                         (-p0.x + p2.x) * t +
                                         (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                                         (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);
                        
                        float y = 0.5f * ((2.0f * p1.y) +
                                         (-p0.y + p2.y) * t +
                                         (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                                         (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);
                        
                        grPath.lineTo(x, y);
                    }
                }
                else
                {
                    grPath.lineTo(grPoints[i]);
                }
            }
            
            grPath.lineTo(grPoints.back().x, bounds.getY());
            grPath.closeSubPath();
            
            // Modo normal: No dibujar área rellena para gain reduction
            // g.setGradientFill(grGradient);
            // g.fillPath(grPath);
            
            // Solo dibujar la línea morada con currentFadeOutFactor
            juce::Path grLine;
            grLine.startNewSubPath(grPoints[0]);
            for (size_t i = 1; i < grPoints.size(); ++i)
            {
                grLine.lineTo(grPoints[i]);
            }
            g.setColour(DarkTheme::accentSecondary.withAlpha(0.9f * currentFadeOutFactor));
            g.strokePath(grLine, juce::PathStrokeType(2.0f)); // Línea un poco más gruesa para mejor visibilidad
        }
    }
}