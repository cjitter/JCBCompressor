//==============================================================================
//
//  Copyright 2025 Juan Carlos Blancas
//  This file is part of JCBCompressor and is licensed under the GNU General Public License v3.0 or later.
//
//==============================================================================

//==============================================================================
// INCLUDES
//==============================================================================
#include <cmath> // para std::sqrt del equal power xfade processBlockBypassed

// Archivos del proyecto
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Helpers/UTF8Helper.h"

//==============================================================================
// CONSTRUCTOR Y DESTRUCTOR
//==============================================================================
JCBCompressorAudioProcessor::JCBCompressorAudioProcessor()
    : juce::AudioProcessor(createBusesProperties()),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout()),  // nullptr = no automatic undo (proven solution tested on jr-granular)
      m_CurrentBufferSize(0),
      editorSize(1260, 360),
      lastPreset(0),
      currentProgram(-1)
{
    // Configurar límites del guiUndoManager para optimizar rendimiento
    guiUndoManager.setMaxNumberOfStoredUnits(0, 20); // Solo 20 transacciones exactas (ahorro de memoria)
    
    // Inicializar Gen~ state
    m_PluginState = (CommonState *)JCBCompressor::create(44100, 64);
    JCBCompressor::reset(m_PluginState);

    // Inicializar buffers de Gen~
    m_InputBuffers = new t_sample *[JCBCompressor::num_inputs()];
    m_OutputBuffers = new t_sample *[JCBCompressor::num_outputs()];
    
    for (int i = 0; i < JCBCompressor::num_inputs(); i++) {
        m_InputBuffers[i] = nullptr;
    }
    for (int i = 0; i < JCBCompressor::num_outputs(); i++) {
        m_OutputBuffers[i] = nullptr;
    }

    // Vincular listeners de parámetros de gen~ a APVTS
    for (int i = 0; i < JCBCompressor::num_params(); i++)
    {
        const char* rawName = JCBCompressor::getparametername(m_PluginState, i);
        auto name = juce::String(rawName ? rawName : "");
        apvts.addParameterListener(name, this);
    }
    
    // IMPORTANTE: Sincronizar valores iniciales con Gen~
    // Esto asegura que Gen~ tenga los valores correctos desde el principio
    for (int i = 0; i < JCBCompressor::num_params(); i++)
    {
        const char* rawName = JCBCompressor::getparametername(m_PluginState, i);
        auto paramName = juce::String(rawName ? rawName : "");
        if (auto* param = apvts.getRawParameterValue(paramName)) {
            float value = param->load();
            
            // Aplicar misma validación que en parameterChanged()
            if (paramName == "d_ATK" && value < 0.1f) {
                value = 0.1f;
            }
            if (paramName == "e_REL" && value < 0.1f) {
                value = 0.1f;
            }
            
            JCBCompressor::setparameter(m_PluginState, i, value, nullptr);
        }
    }

    // Añadir parámetro AAX directamente al juce::AudioProcessor (no al APVTS)
    #if JucePlugin_Build_AAX
    auto grMeter = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("aax_gr_meter", 1),  // Version hint fijo para este parámetro
        "Gain Reduction",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.0f,  // Default: 0 = sin reducción
        "",
        juce::AudioProcessorParameter::compressorLimiterGainReductionMeter);
    
    // Guardar referencia antes de moverlo
    aaxGainReductionParam = grMeter.get();
    
    // Añadir directamente al processor, NO al APVTS
    // El flag compressorLimiterGainReductionMeter ya indica que es un parámetro especial
    addParameter(grMeter.release());

    // Iniciar timer para actualizar parámetros AAX
    startTimerHz(30); // 15 Hz es suficiente para el medidor y más seguro
    #endif
}

JCBCompressorAudioProcessor::~JCBCompressorAudioProcessor()
{
    // CRÍTICO: Primero indicar que estamos destruyendo para evitar race conditions
    isBeingDestroyed = true;
    
    // Detener timer AAX inmediatamente (antes que cualquier otra cosa)
    #if JucePlugin_Build_AAX
    stopTimer();
    // Pequeña espera para asegurar que el timer callback no esté ejecutándose
    juce::Thread::sleep(10);
    #endif
    
    // Destruir listeners de parámetros del apvts
    for (int i = 0; i < JCBCompressor::num_params(); i++)
    {
        const char* rawName = JCBCompressor::getparametername(m_PluginState, i);
        auto name = juce::String(rawName ? rawName : "");
        apvts.removeParameterListener(name, this);
    }
    
    // Limpiar buffers con protección nullptr
    if (m_InputBuffers != nullptr) {
        for (int i = 0; i < JCBCompressor::num_inputs(); i++) {
            if (m_InputBuffers[i] != nullptr) {
                delete[] m_InputBuffers[i];
                m_InputBuffers[i] = nullptr;
            }
        }
        delete[] m_InputBuffers;
        m_InputBuffers = nullptr;
    }
    
    if (m_OutputBuffers != nullptr) {
        for (int i = 0; i < JCBCompressor::num_outputs(); i++) {
            if (m_OutputBuffers[i] != nullptr) {
                delete[] m_OutputBuffers[i];
                m_OutputBuffers[i] = nullptr;
            }
        }
        delete[] m_OutputBuffers;
        m_OutputBuffers = nullptr;
    }
    
    // Destruir Gen~ state al final
    if (m_PluginState != nullptr) {
        JCBCompressor::destroy(m_PluginState);
        m_PluginState = nullptr;
    }
}

//==============================================================================
// MÉTODOS PRINCIPALES DEL AUDIOPROCESSOR
//==============================================================================
// void JCBCompressorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
// {
//     // Configuración de canales en modo debug eliminada para release
//
//     // Inicializar sample rate y vector size
//     m_PluginState->sr = sampleRate;
//     m_PluginState->vs = samplesPerBlock;
//
//     // Pre-asignar buffers con tamaño máximo esperado para evitar allocations en audio thread
//     // Usar un tamaño seguro que cubra la mayoría de casos (4096 samples es común máximo)
//     const long maxExpectedBufferSize = juce::jmax(static_cast<long>(samplesPerBlock), 4096L);
//     assureBufferSize(maxExpectedBufferSize);
//
//     // Pre-asignar vectors de waveform data para evitar resize en audio thread
//     {
//         std::lock_guard<std::mutex> lock(waveformMutex);
//         const size_t maxWaveformSize = static_cast<size_t>(maxExpectedBufferSize);
//         currentInputSamples.resize(maxWaveformSize);
//         currentProcessedSamples.resize(maxWaveformSize);
//         currentGainReductionSamples.resize(maxWaveformSize);
//     }
//
//     // Inicializar latencia basada en el tiempo de lookahead
//     auto latenciaMs = apvts.getRawParameterValue("n_LOOKAHEAD")->load();
//
//     // Cálculo estándar: ms * (sampleRate / 1000)
//     int latenciaSamples = static_cast<int>(latenciaMs * sampleRate / 1000.0);
//     latenciaSamples = juce::jmax(0, latenciaSamples);
//
//     // Aplicar Lookahead teniendo en cuenta el sample generado en gen~
//     setLatencySamples(latenciaSamples+1);
//
//     // Initialize atomic meter values
//     // CRITICAL: Changed from SmoothedValue to atomic - no reset() needed
//     leftInputRMS.store(-100.0f, std::memory_order_relaxed);
//     rightInputRMS.store(-100.0f, std::memory_order_relaxed);
//
//     leftOutputRMS.store(-100.0f, std::memory_order_relaxed);
//     rightOutputRMS.store(-100.0f, std::memory_order_relaxed);
//
//     gainReduction.store(0.0f, std::memory_order_relaxed);
//
//     leftSC.store(-100.0f, std::memory_order_relaxed);
//     rightSC.store(-100.0f, std::memory_order_relaxed);
//
//     // Configurar buffers auxiliares
//     grBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);
//     grBuffer.clear();
//
//     trimInputBuffer.setSize(2, samplesPerBlock);
//     trimInputBuffer.clear();
//
//     sidechainBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock);
//     sidechainBuffer.clear();
//
//     // Inicializar buffers de forma de onda
//     currentInputSamples.resize(samplesPerBlock, 0.0f);
//     currentProcessedSamples.resize(samplesPerBlock, 0.0f);
//
//     // IMPORTANTE: Re-sincronizar todos los parámetros con Gen~ en prepareToPlay
//     // Esto asegura que los valores estén correctos cuando el DAW comienza a reproducir
//     for (int i = 0; i < JCBCompressor::num_params(); i++)
//     {
//         auto paramName = juce::String(JCBCompressor::getparametername(m_PluginState, i));
//         if (auto* param = apvts.getRawParameterValue(paramName)) {
//             float value = param->load();
//             JCBCompressor::setparameter(m_PluginState, i, value, nullptr);
//         }
//     }
// }

void JCBCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // 1) Guardas SR/VS en tu estado (si lo usas en otros sitios)
    m_PluginState->sr = sampleRate;
    m_PluginState->vs = samplesPerBlock;

    // 2) Dimensionado de buffers propios (como hacías)
    const int maxExpectedBufferSize = juce::jmax (samplesPerBlock, 4096);
    assureBufferSize (maxExpectedBufferSize);

    {
        std::lock_guard<std::mutex> lock (waveformMutex);
        currentInputSamples.resize  ((size_t) maxExpectedBufferSize);
        currentProcessedSamples.resize((size_t) maxExpectedBufferSize);
        currentGainReductionSamples.resize((size_t) maxExpectedBufferSize);
    }

    // 3) ***Clave***: sincroniza SR/VS con Gen y re-dimensiona sus delays/constantes
    //    Esto asegura que Gen use el sampleRate real del host (48k si el proyecto es 48k)
    JCBCompressor::reset (m_PluginState);

    // Cachear indices de gen para evitar bucles por nombre
    genIdxLookahead = genIdxBypass = -1;
    for (int i = 0; i < JCBCompressor::num_params(); ++i)
    {
        const char* raw = JCBCompressor::getparametername(m_PluginState, i);
        juce::String name(raw ? raw : "");
        if (name == "n_LOOKAHEAD") genIdxLookahead = i;
        if (name == "p_BYPASS")    genIdxBypass   = i; // por si te hace falta en futuro
    }

    // 4) Reinyecta TODOS los parámetros a Gen DESPUÉS del reset (tú ya lo hacías)
    for (int i = 0; i < JCBCompressor::num_params(); ++i)
    {
        const char* rawName = JCBCompressor::getparametername(m_PluginState, i);
        auto paramName = juce::String(rawName ? rawName : "");
        if (auto* p = apvts.getRawParameterValue (paramName))
            JCBCompressor::setparameter (m_PluginState, i, p->load(), nullptr);
    }

    // 5) Inicializa/limpia tus buffers de trabajo
    grBuffer.setSize (getTotalNumOutputChannels(), samplesPerBlock);
    grBuffer.clear();

    trimInputBuffer.setSize (2, samplesPerBlock);
    trimInputBuffer.clear();

    sidechainBuffer.setSize (getTotalNumInputChannels(), samplesPerBlock);
    sidechainBuffer.clear();

    currentInputSamples.resize   ((size_t) samplesPerBlock, 0.0f);
    currentProcessedSamples.resize((size_t) samplesPerBlock, 0.0f);

    // 6) Medidores/atomics
    leftInputRMS.store  (-100.0f, std::memory_order_relaxed);
    rightInputRMS.store (-100.0f, std::memory_order_relaxed);
    leftOutputRMS.store (-100.0f, std::memory_order_relaxed);
    rightOutputRMS.store(-100.0f, std::memory_order_relaxed);
    gainReduction.store (0.0f,    std::memory_order_relaxed);
    leftSC.store        (-100.0f, std::memory_order_relaxed);
    rightSC.store       (-100.0f, std::memory_order_relaxed);

    // 7) Latencia reportada al host (coherente con Gen)
    //    - usando computeLatencySamples sin el +1
    const int latenciaSamples = computeLatencySamples(sampleRate);
    setLatencySamples(latenciaSamples);
    currentLatency = latenciaSamples;
    // --- Reset estado de bypass & colas para primer bloque tras prepareToPlay ---
    {
        wasHostBypassed  = false;
        bypassFadePos    = -1;          // por si lo usas en el futuro / queda de builds previas
        lastWetTailLen   = 0;

        // Inicializa la cola WET con un tamaño mínimo para evitar reallocs en el primer frame
        // Recalcular canales con fallback defensivo
        int chans = getTotalNumOutputChannels();
        if (chans <= 0)
            chans = juce::jmax(1, getMainBusNumOutputChannels()); // fallback raro, al menos 1

        const int tailN = juce::jmin(bypassFadeLen, samplesPerBlock > 0 ? samplesPerBlock : 1);
        lastWetTail.setSize(chans, tailN);
        lastWetTail.clear();
    }
    
    // 8) Inicializar bypass buffer según latencia
    if (latenciaSamples > 0) {
        bypassDelayBuffer.setSize(getTotalNumOutputChannels(), latenciaSamples);
        bypassDelayBuffer.clear();
        bypassDelayWritePos = 0;
    } else {
        bypassDelayBuffer.setSize(0, 0);
        bypassDelayWritePos = 0;
    }
    
    // 9) Inicializar variables de debounce
    if (auto* la = apvts.getRawParameterValue("n_LOOKAHEAD")) {
        stagedLookaheadMs.store(la->load(), std::memory_order_relaxed);
        lastLAChangeMs.store(juce::Time::getMillisecondCounter(), std::memory_order_relaxed);
        laCommitPending.store(false, std::memory_order_relaxed);
    }
}

void JCBCompressorAudioProcessor::releaseResources()
{
    // Limpiar buffers auxiliares
    grBuffer.setSize(0, 0);
    trimInputBuffer.setSize(0, 0);
    sidechainBuffer.setSize(0, 0);
    
    // Limpiar bypass buffer con protección thread-safe
    {
        const juce::SpinLock::ScopedLockType sl(bypassDelayLock);
        bypassDelayBuffer.setSize(0, 0);
        bypassDelayWritePos = 0;
    }
}

//==============================================================================
// PROCESAMIENTO DE AUDIO
//==============================================================================
void JCBCompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();

    // Ajuste dinámico de buffers según block size
    if (grBuffer.getNumChannels() != getTotalNumOutputChannels()
     || grBuffer.getNumSamples() != numSamples)
        grBuffer.setSize(getTotalNumOutputChannels(), numSamples, false, false, true);
    if (trimInputBuffer.getNumChannels() != 2
     || trimInputBuffer.getNumSamples() != numSamples)
        trimInputBuffer.setSize(2, numSamples, false, false, true);
    if (sidechainBuffer.getNumChannels() != getTotalNumInputChannels()
     || sidechainBuffer.getNumSamples() != numSamples)
        sidechainBuffer.setSize(getTotalNumInputChannels(), numSamples, false, false, true);
    
    // Resize de waveform buffers con try_lock para evitar bloqueos
    {
        std::unique_lock<std::mutex> lock(waveformMutex, std::try_to_lock);
        if (lock.owns_lock())
        {
            if (currentInputSamples.size() < static_cast<size_t>(numSamples))
                currentInputSamples.resize(numSamples);
            if (currentProcessedSamples.size() < static_cast<size_t>(numSamples))
                currentProcessedSamples.resize(numSamples);
            if (currentGainReductionSamples.size() < static_cast<size_t>(numSamples))
                currentGainReductionSamples.resize(numSamples);
        }
    }
    
    assureBufferSize(numSamples);
    
    // Debounce de lookahead (commit coordinado Gen + host)
    if (laCommitPending.load(std::memory_order_acquire))
    {
        const uint32_t now  = juce::Time::getMillisecondCounter();
        const uint32_t last = lastLAChangeMs.load(std::memory_order_relaxed);

        if ((int)(now - last) > laDebounceMs)
        {
            // Aplicar a Gen solo cuando vence el debounce
            if (genIdxLookahead >= 0)
            {
                const float ms = stagedLookaheadMs.load(std::memory_order_relaxed);
                JCBCompressor::setparameter(m_PluginState, genIdxLookahead, ms, nullptr);
            }

            // Recalcular/propagar latencia
            const int newL = computeLatencySamples(getSampleRate());
            if (newL != pendingLatency.load(std::memory_order_relaxed)
             && newL != currentLatency)
            {
                pendingLatency.store(newL, std::memory_order_relaxed);
                triggerAsyncUpdate(); // handleAsyncUpdate() hará setLatencySamples(newL)
            }

            laCommitPending.store(false, std::memory_order_release);
        }
    }


    // Procesar audio usando métodos separados
    fillGenInputBuffers(buffer);
    processGenAudio(numSamples);
    fillOutputBuffers(buffer);

    // ---- Capturar cola "wet" para suavizar entrada a bypass del host ----
    {
        const int numChannels = buffer.getNumChannels();
        const int nRequested  = juce::jmin(bypassFadeLen, numSamples);

        // Si no hay nada que capturar, deja flags en estado coherente
        if (nRequested <= 0 || numChannels <= 0)
        {
            lastWetTailLen = 0;
            wasHostBypassed = false;
            bypassFadePos   = -1;
        }
        else
        {
            // Reajustar cola si cambian canales o longitud objetivo
            if (lastWetTail.getNumChannels() != numChannels
             || lastWetTail.getNumSamples()  != nRequested)
            {
                lastWetTail.setSize(numChannels, nRequested, false, false, true);
                lastWetTail.clear();
                lastWetTailLen = 0; // evita mezclar con cola antigua
            }

            // Copiamos las ÚLTIMAS nRequested muestras del buffer ya procesado (wet)
            const int srcOffset = numSamples - nRequested;
            for (int ch = 0; ch < numChannels; ++ch)
            {
                const float* src = buffer.getReadPointer(ch, srcOffset);
                float*       dst = lastWetTail.getWritePointer(ch);
                std::memcpy(dst, src, size_t(nRequested) * sizeof(float));
            }
            lastWetTailLen = nRequested;

            // Estado: este bloque estaba activo (no bypass)
            wasHostBypassed = false;
            bypassFadePos   = -1; // por si usas acumulador en builds previas
        }
    }
    
    // Capturar DESPUÉS del procesamiento para usar salidas de Gen~
    // Capturar entrada post-TRIM desde salidas 3 y 4 de Gen~
    captureInputWaveformData(buffer, numSamples);
    // Capturar salida procesada para visualización
    captureOutputWaveformData(numSamples);
    
    // Actualizar clip detection (debe ser DESPUÉS del procesamiento completo)
    updateClipDetection(buffer, buffer);  // input y output son el mismo buffer al final
    
    // Actualizar medidores
    updateInputMeters(buffer);
    updateOutputMeters(buffer);
    updateGainReductionMeter(numSamples);
    updateSidechainMeters(buffer);
}

void JCBCompressorAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer,
                                                       juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    const int delaySamples = getLatencySamples(); // == currentLatency

    // Si no hay latencia reportada, salimos: host usará el audio tal cual
    if (delaySamples <= 0)
        return;

    // Protección thread-safe del buffer de delay
    const juce::SpinLock::ScopedLockType sl(bypassDelayLock);

    // Redimensionar el buffer de delay si cambian canales/latencia
    if (bypassDelayBuffer.getNumChannels() != numChannels
     || bypassDelayBuffer.getNumSamples()  != delaySamples)
    {
        bypassDelayBuffer.setSize(numChannels, delaySamples);
        bypassDelayBuffer.clear();
        bypassDelayWritePos = 0;
    }

    // Buffer circular para alinear con la latencia reportada
    for (int i = 0; i < numSamples; ++i)
    {
        const int readPos = bypassDelayWritePos;

        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* out   = buffer.getWritePointer(ch);
            float* dline = bypassDelayBuffer.getWritePointer(ch);

            const float delayed = dline[readPos]; // muestra alineada
            dline[readPos] = out[i];              // escribir muestra actual
            out[i] = delayed;                     // salida = dry compensado
        }

        bypassDelayWritePos = (bypassDelayWritePos + 1) % delaySamples;
    }

    // ---- Crossfade equal-power SOLO en el primer bloque tras entrar en bypass ----
    const bool justEnteredBypass = !wasHostBypassed;
    wasHostBypassed = true;

    // Ajuste adaptativo del fade por bloque (mín 32, máx 192, acotado a numSamples)
    //const int targetFade = juce::jlimit(32, 192, buffer.getNumSamples());
    // ---- Adaptar fade dinámicamente (limitado a potencias de 2) ----
    int rawFade = juce::jlimit(32, 192, buffer.getNumSamples());
    // Snap a la potencia de 2 más cercana (32, 64, 128, 192 ~ 256 → 192 como límite superior)
    int targetFade = 64;
    if (rawFade < 64)       targetFade = 32;
    else if (rawFade < 128) targetFade = 64;
    else if (rawFade < 192) targetFade = 128;
    else                    targetFade = 192;

    if (bypassFadeLen != targetFade)
        bypassFadeLen = targetFade;

    const int fadeLen = juce::jmin(bypassFadeLen, numSamples, lastWetTailLen);
    if (justEnteredBypass && fadeLen > 0)
    {
        const int tail0 = lastWetTailLen - fadeLen; // inicio en la cola WET guardada

        for (int ch = 0; ch < numChannels; ++ch)
        {
            float*       out  = buffer.getWritePointer(ch);     // DRY ya compensado
            const float* tail = lastWetTail.getReadPointer(ch); // WET del bloque activo previo

            for (int n = 0; n < fadeLen; ++n)
            {
                const float a    = (float) n / (float) fadeLen; // 0..1
                const float gDry = std::sqrt(a);                // equal-power
                const float gWet = std::sqrt(1.0f - a);

                const float wetPrev = tail[tail0 + n];
                const float dryNow  = out[n];
                out[n] = dryNow * gDry + wetPrev * gWet;
            }
        }
    }
}

//==============================================================================
// INTEGRACIÓN GEN~
//==============================================================================

/**
 * Asegurar que los buffers de Gen~ tengan el tamaño correcto
 * CRÍTICO: Esta función gestiona la memoria dinámica para la comunicación con el motor DSP Gen~
 * AUDIO-THREAD SAFE: Solo redimensiona si es absolutamente necesario para evitar RT violations
 */
void JCBCompressorAudioProcessor::assureBufferSize(long bufferSize)
{
    if (bufferSize > m_CurrentBufferSize) {
        // NOTA: RT-safe porque prepareToPlay() pre-asigna hasta 4096 samples
        // Solo se ejecuta este bloque si el DAW solicita > 4096 samples (muy raro)
        
        // Redimensionar buffers de entrada
        for (int i = 0; i < JCBCompressor::num_inputs(); i++) {
            delete[] m_InputBuffers[i];
            m_InputBuffers[i] = new t_sample[bufferSize];
        }
        
        // Redimensionar buffers de salida
        for (int i = 0; i < JCBCompressor::num_outputs(); i++) {
            delete[] m_OutputBuffers[i];
            m_OutputBuffers[i] = new t_sample[bufferSize];
        }
        
        m_CurrentBufferSize = bufferSize;
    }
}

void JCBCompressorAudioProcessor::fillGenInputBuffers(const juce::AudioBuffer<float>& buffer)
{
    const auto totalNumInputChannels = getTotalNumInputChannels();
    const auto mainInputChannels = getMainBusNumInputChannels();
    const int numSamples = buffer.getNumSamples();
    
    if (mainInputChannels > 1) {
        // Modo estéreo
        for (int i = 0; i < JCBCompressor::num_inputs(); i++) {
            if (i < totalNumInputChannels) {
                for (int j = 0; j < numSamples; j++) {
                    m_InputBuffers[i][j] = buffer.getReadPointer(i)[j];
                }
            } else {
                memset(m_InputBuffers[i], 0, numSamples * sizeof(t_sample));
            }
        }
    } else {
        // Modo mono - duplicar señal mono a ambos canales L/R para procesamiento stereo-linked
        if (mainInputChannels == 1) {
            // Duplicar el canal mono a L y R
            for (int j = 0; j < numSamples; j++) {
                m_InputBuffers[0][j] = buffer.getReadPointer(0)[j];  // L = mono
                m_InputBuffers[1][j] = buffer.getReadPointer(0)[j];  // R = mono (duplicado)
            }
            
            // Manejar sidechain si existe
            if (totalNumInputChannels > 1) {
                // Hay sidechain mono
                for (int j = 0; j < numSamples; j++) {
                    m_InputBuffers[2][j] = buffer.getReadPointer(1)[j];  // SC L = sidechain mono
                    m_InputBuffers[3][j] = buffer.getReadPointer(1)[j];  // SC R = sidechain mono (duplicado)
                }
            } else {
                // No hay sidechain
                memset(m_InputBuffers[2], 0, numSamples * sizeof(t_sample));
                memset(m_InputBuffers[3], 0, numSamples * sizeof(t_sample));
            }
        }
    }
}

void JCBCompressorAudioProcessor::processGenAudio(int numSamples)
{
    JCBCompressor::perform(m_PluginState,
                          m_InputBuffers,
                          JCBCompressor::num_inputs(),
                          m_OutputBuffers,
                          JCBCompressor::num_outputs(),
                          numSamples);
}

void JCBCompressorAudioProcessor::fillOutputBuffers(juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const auto mainOutputChannels = getMainBusNumOutputChannels();
    
    // Llenar buffers de salida principales - conversión double a float
    for (int i = 0; i < mainOutputChannels; i++) {
        float* destPtr = buffer.getWritePointer(i);
        const t_sample* srcPtr = m_OutputBuffers[i];
        for (int j = 0; j < numSamples; j++) {
            destPtr[j] = static_cast<float>(srcPtr[j]);
        }
    }
    
    // Preparar buffer para medidor de entrada (después de trim) - conversión double a float
    float* trimL = trimInputBuffer.getWritePointer(0);
    const t_sample* srcL = m_OutputBuffers[3];
    for (int j = 0; j < numSamples; j++) {
        trimL[j] = static_cast<float>(srcL[j]);
    }
    if (trimInputBuffer.getNumChannels() > 1) {
        float* trimR = trimInputBuffer.getWritePointer(1);
        const t_sample* srcR = m_OutputBuffers[4];
        for (int j = 0; j < numSamples; j++) {
            trimR[j] = static_cast<float>(srcR[j]);
        }
    }
}

//==============================================================================
// MEDIDORES DE AUDIO
//==============================================================================
void JCBCompressorAudioProcessor::updateInputMeters(const juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const auto mainInputChannels = getMainBusNumInputChannels();

    // Calcular valores RMS
    const auto rmsValueL = juce::Decibels::gainToDecibels(trimInputBuffer.getRMSLevel(0, 0, numSamples));
    const auto rmsValueR = (trimInputBuffer.getNumChannels() > 1) ?
    juce::Decibels::gainToDecibels(trimInputBuffer.getRMSLevel(1, 0, numSamples)) : rmsValueL;
    
    // Calcular valores de pico reales
    const auto peakValueL = juce::Decibels::gainToDecibels(trimInputBuffer.getMagnitude(0, 0, numSamples));
    const auto peakValueR = (trimInputBuffer.getNumChannels() > 1) ?
    juce::Decibels::gainToDecibels(trimInputBuffer.getMagnitude(1, 0, numSamples)) : peakValueL;
    
    // Usar combinación ponderada 70% peak + 30% RMS
    const auto displayValueL = (peakValueL * 0.7f) + (rmsValueL * 0.3f);
    const auto displayValueR = (peakValueR * 0.7f) + (rmsValueR * 0.3f);
    
    // Si estamos cerca del clipping (> -3dB), mostrar el valor de pico puro
    const auto inputValueL = (peakValueL > -3.0f) ? peakValueL : displayValueL;
    const auto inputValueR = (peakValueR > -3.0f) ? peakValueR : displayValueR;
    
    // CRÍTICO: Usar atomic store para thread safety
    // No smoothing is done here - just direct atomic updates
    if (mainInputChannels < 2) {
        // Modo mono - ambos medidores muestran el mismo valor
        leftInputRMS.store(inputValueL, std::memory_order_relaxed);
        rightInputRMS.store(inputValueL, std::memory_order_relaxed);
    } else {
        // Modo estéreo - medidores independientes
        leftInputRMS.store(inputValueL, std::memory_order_relaxed);
        rightInputRMS.store(inputValueR, std::memory_order_relaxed);
    }
}

void JCBCompressorAudioProcessor::updateOutputMeters(const juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();
    const auto mainOutputChannels = getMainBusNumOutputChannels();

    // Calcular valores RMS para el medidor (promedio de potencia)
    const auto rmsValueL = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples));
    const auto rmsValueR = (mainOutputChannels > 1) ?
    juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples)) : rmsValueL;
    
    // Calcular valores de pico reales (máximo absoluto en el buffer)
    const auto peakValueL = juce::Decibels::gainToDecibels(buffer.getMagnitude(0, 0, numSamples));
    const auto peakValueR = (mainOutputChannels > 1) ?
    juce::Decibels::gainToDecibels(buffer.getMagnitude(1, 0, numSamples)) : peakValueL;
    
    // Usar una combinación ponderada de RMS y Peak para mejor visualización
    // 70% peak + 30% RMS da una representación más precisa similar a medidores profesionales
    const auto displayValueL = (peakValueL * 0.7f) + (rmsValueL * 0.3f);
    const auto displayValueR = (peakValueR * 0.7f) + (rmsValueR * 0.3f);
    
    // Si estamos cerca del clipping (> -3dB), mostrar el valor de pico puro
    const auto finalValueL = (peakValueL > -3.0f) ? peakValueL : displayValueL;
    const auto finalValueR = (peakValueR > -3.0f) ? peakValueR : displayValueR;
    
    // CRÍTICO: Usar atomic store para thread safety
    if (mainOutputChannels > 1) {
        // Modo estéreo
        leftOutputRMS.store(finalValueL, std::memory_order_relaxed);
        rightOutputRMS.store(finalValueR, std::memory_order_relaxed);
    } else {
        // Modo mono
        leftOutputRMS.store(finalValueL, std::memory_order_relaxed);
        rightOutputRMS.store(finalValueL, std::memory_order_relaxed);
    }
}

void JCBCompressorAudioProcessor::updateGainReductionMeter(int numSamples)
{
    jassert(numSamples >= 0);
    
    // Buscar el valor máximo de gain reduction directamente del output de Gen~
    float maxGR = 0.0f;
    t_sample* grData = m_OutputBuffers[2]; // out3 = gain reduction (valores lineales)
    
    for (int i = 0; i < numSamples; ++i) {
        maxGR = juce::jmax(maxGR, static_cast<float>(grData[i]));
    }
    
    // Actualizar el valor almacenado para hosts (en dB para compatibilidad)
    const auto valueGR = juce::Decibels::gainToDecibels(maxGR);
    currentGainReductionDb.store(valueGR);
    
    // CRÍTICO: Usar atomic store para thread safety
    // Almacenar valor de reducción de ganancia directamente sin suavizado
    gainReduction.store(valueGR, std::memory_order_relaxed);
    
    // Almacenar el valor para AAX (Pro Tools lo leerá cuando lo necesite)
    #if JucePlugin_Build_AAX
    // Gen~ out3 ya devuelve valores lineales donde:
    // 1.0 = sin reducción
    // < 1.0 = hay reducción
    // NO actualizar parámetros desde el thread de audio - solo almacenar el valor
    currentGainReductionLinear.store(maxGR);
    #endif
}

void JCBCompressorAudioProcessor::updateSidechainMeters(const juce::AudioBuffer<float>& buffer)
{
    const int numSamples = buffer.getNumSamples();

    // Resetear flags de clipping sidechain para este buffer
    bool scClip[2] = {false, false};
    
    // Verificar si EXT KEY está activo
    const bool extKeyActive = apvts.getRawParameterValue("r_KEY")->load() > 0.5f;
    
    // Si EXT KEY no está activo, mostrar silencio en los medidores
    if (!extKeyActive) {
        const auto valueSC = -100.0f;
        
        // CRÍTICO: Usar atomic store para thread safety
        leftSC.store(valueSC, std::memory_order_relaxed);
        rightSC.store(valueSC, std::memory_order_relaxed);
        
        return;  // No procesar más si EXT KEY está desactivado
    }
    
    // CAMBIO: Usar las salidas 5 y 6 de Gen~ (índices 5 y 6 en m_OutputBuffers)
    // Estas salidas ya incluyen el procesamiento de SC TRIM aplicado por Gen~
    if (JCBCompressor::num_outputs() > 6) {
        // Calcular RMS y pico del sidechain desde las salidas de Gen~
        float maxSCL = 0.0f, maxSCR = 0.0f;
        float rmsSumL = 0.0f, rmsSumR = 0.0f;
        
        for (int i = 0; i < numSamples; ++i) {
            // Usar salidas 5 y 6 de Gen~ que ya tienen SC TRIM aplicado
            float sampleL = static_cast<float>(m_OutputBuffers[5][i]);
            float sampleR = static_cast<float>(m_OutputBuffers[6][i]);
            
            // Peak detection
            maxSCL = juce::jmax(maxSCL, std::abs(sampleL));
            maxSCR = juce::jmax(maxSCR, std::abs(sampleR));
            
            // RMS accumulation
            rmsSumL += sampleL * sampleL;
            rmsSumR += sampleR * sampleR;
        }
        
        // Calcular RMS
        float rmsL = std::sqrt(rmsSumL / static_cast<float>(numSamples));
        float rmsR = std::sqrt(rmsSumR / static_cast<float>(numSamples));
        
        const auto rmsValueSCLeft = juce::Decibels::gainToDecibels(rmsL);
        const auto rmsValueSCRight = juce::Decibels::gainToDecibels(rmsR);
        const auto peakValueSCLeft = juce::Decibels::gainToDecibels(maxSCL);
        const auto peakValueSCRight = juce::Decibels::gainToDecibels(maxSCR);
        
        // Detectar clipping basado en el valor de pico
        if (maxSCL >= 0.999f) {  // Usar el mismo umbral que los medidores principales
            scClip[0] = true;
        }
        if (maxSCR >= 0.999f) {
            scClip[1] = true;
        }
        
        // Usar combinación ponderada como en los medidores principales
        const auto displayValueSCLeft = (peakValueSCLeft * 0.7f) + (rmsValueSCLeft * 0.3f);
        const auto displayValueSCRight = (peakValueSCRight * 0.7f) + (rmsValueSCRight * 0.3f);
        
        const auto valueSCLeft = (peakValueSCLeft > -3.0f) ? peakValueSCLeft : displayValueSCLeft;
        const auto valueSCRight = (peakValueSCRight > -3.0f) ? peakValueSCRight : displayValueSCRight;
        
        // CRÍTICO: Usar atomic store para thread safety
        if (!isProTools()) {
            leftSC.store(valueSCLeft, std::memory_order_relaxed);
            rightSC.store(valueSCRight, std::memory_order_relaxed);
        } else {
            // ProTools: usar solo canal izquierdo para ambos medidores
            leftSC.store(valueSCLeft, std::memory_order_relaxed);
            rightSC.store(valueSCLeft, std::memory_order_relaxed);
        }
    } else {
        // Sidechain no disponible - mostrar silencio
        const auto valueSC = -100.0f;
        
        // CRÍTICO: Usar atomic store para thread safety
        leftSC.store(valueSC, std::memory_order_relaxed);
        rightSC.store(valueSC, std::memory_order_relaxed);
    }
    
    // Actualizar flags atómicos de clip
    for (int channel = 0; channel < 2; ++channel) {
        if (scClip[channel]) {
            sidechainClipDetected[channel] = true;
        }
    }
}

//==============================================================================
// CONFIGURACIÓN DE BUSES Y PARÁMETROS
//==============================================================================
juce::AudioProcessor::BusesProperties JCBCompressorAudioProcessor::createBusesProperties()
{
    auto propBuses = juce::AudioProcessor::BusesProperties()
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        .withInput("Input", juce::AudioChannelSet::stereo(), true);
    
    juce::PluginHostType daw;
    
    if (daw.isProTools())
        propBuses.addBus(true, "Sidechain MONO", juce::AudioChannelSet::mono(), false);
    else
        propBuses.addBus(true, "Sidechain ST", juce::AudioChannelSet::stereo(), false);
    
    return propBuses;
}

// Validación de configuraciones de canales - define qué layouts acepta el plugin
bool JCBCompressorAudioProcessor::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // Verificar bus principal de salida
    auto mainOut = layouts.getMainOutputChannelSet();
    if (mainOut != juce::AudioChannelSet::mono()
        && mainOut != juce::AudioChannelSet::stereo())
        return false;
    
    // Verificar bus principal de entrada
    auto mainIn = layouts.getMainInputChannelSet();
    if (mainIn != juce::AudioChannelSet::mono()
        && mainIn != juce::AudioChannelSet::stereo())
        return false;
    
    // Pro Tools AAX: Rechazar específicamente stereo input → mono output
    // Solo permitir: 1→1, 2→2, 1→2. NO permitir: 2→1
#if JucePlugin_Build_AAX
    // En formato AAX, rechazar siempre stereo input → mono output
    if (mainIn == juce::AudioChannelSet::stereo() && mainOut == juce::AudioChannelSet::mono())
        return false;
#endif
    
    // Si hay bus de sidechain, verificar que sea válido
    if (layouts.inputBuses.size() > 1)
    {
        auto sidechainBus = layouts.inputBuses[1];
        
        // El sidechain puede estar desactivado (empty)
        if (!sidechainBus.isDisabled())
        {
            // Solo aceptar sidechain mono o estéreo si está activo
            if (sidechainBus != juce::AudioChannelSet::mono() 
                && sidechainBus != juce::AudioChannelSet::stereo())
                return false;
        }
            
        // No aceptar más de 2 buses de entrada
        if (layouts.inputBuses.size() > 2)
            return false;
    }
    
    return true;
#endif
}

/**
 * Crear el layout de parámetros del plugin
 * CRÍTICO: Define todos los parámetros del compresor en orden alfabético
 * Incluye configuración de rangos, valores por defecto y metadata para cada parámetro
 * Version hint 21 fuerza re-escaneo en hosts para parámetros renombrados
 */
juce::AudioProcessorValueTreeState::ParameterLayout JCBCompressorAudioProcessor::createParameterLayout()
{
   const int versionHint = 21;

   std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

   // a_TRIM @min -12 @max 12 @default 0
   auto trim = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("a_TRIM", versionHint),
                                                           juce::CharPointer_UTF8("Trim"),
                                                           juce::NormalisableRange<float>(-12.f, 12.f, 0.1f, 1.f),
                                                           0.f);

   // b_THD - matching ExpansorGate implementation
   auto thd = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("b_THD", versionHint),
                                                          juce::CharPointer_UTF8("Threshold"),
                                                          juce::NormalisableRange<float>(-60.f, 0.f, 0.1f, 1.0f),
                                                          -18.f);

   // c_RATIO @min 1 @max 20 @default 2
   auto ratio = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_RATIO", versionHint),
                                                            juce::CharPointer_UTF8("Ratio"),
                                                            juce::NormalisableRange<float>(1.f, 20.f, 0.01f, 1.0f), // Sin skew en el parámetro - lo maneja el slider
                                                            4.f,
                                                            juce::String(),
                                                            juce::AudioParameterFloat::genericParameter,
                                                            [](float value, int){
                                                                if (value >= 20.f)
                                                                    return juce::String("20 : 1");
                                                                else
                                                                    return juce::String(value, 1) + " : 1";
                                                            },
                                                            nullptr);

   // d_ATK - con mapeo exponencial para mejor control en automatización
   auto atkRange = juce::NormalisableRange<float>(
       0.1f, 250.f,
       // Mapeo de normalizado (0-1) a valor real - exponencial balanceado
       [](float start, float end, float normalised) {
           // Usar pow con exponente 1.8 para mejor balance visual
           return start + (end - start) * std::pow(normalised, 1.8f);
       },
       // Mapeo inverso de valor real a normalizado (0-1)
       [](float start, float end, float value) {
           float proportion = juce::jlimit(0.0f, 1.0f, (value - start) / (end - start));
           return std::pow(proportion, 1.0f / 1.8f);
       },
       // Snap function optimizada para valores bajos
       [](float start, float end, float value) {
           if (value < 10.0f)
               return std::round(value * 100.0f) / 100.0f;  // 0.01ms steps < 10ms
           else
               return std::round(value * 10.0f) / 10.0f;    // 0.1ms steps >= 10ms
       }
   );
   auto atk = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("d_ATK", versionHint),
       juce::CharPointer_UTF8("Attack"),
       atkRange,
       5.0f,  // valor por defecto
       juce::String(),
       juce::AudioParameterFloat::genericParameter,
       [](float value, int){
           // Formateo progresivo optimizado
           if (value < 1.0f)
               return juce::String(value, 1) + " ms";     // 1 decimal < 1ms
           else if (value < 100.0f)
               return juce::String(value, 1) + " ms";     // 1 decimal < 100ms  
           else
               return juce::String(value, 0) + " ms";     // Sin decimales >= 100ms
       },
       nullptr);

   // e_REL - con mapeo exponencial optimizado para rango operativo 1-50ms
   auto relRange = juce::NormalisableRange<float>(
       0.1f, 1000.f,
       // Mapeo de normalizado (0-1) a valor real - exponencial optimizado
       [](float start, float end, float normalised) {
           // Usar pow con exponente 1.4 para mejor resolución en rango operativo 1-50ms
           return start + (end - start) * std::pow(normalised, 1.4f);
       },
       // Mapeo inverso de valor real a normalizado (0-1)
       [](float start, float end, float value) {
           float proportion = juce::jlimit(0.0f, 1.0f, (value - start) / (end - start));
           return std::pow(proportion, 1.0f / 1.4f);
       },
       // Snap function optimizada para valores bajos
       [](float start, float end, float value) {
           if (value < 10.0f)
               return std::round(value * 100.0f) / 100.0f;  // 0.01ms steps < 10ms
           else
               return std::round(value * 10.0f) / 10.0f;    // 0.1ms steps >= 10ms
       }
   );
   auto rel = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("e_REL", versionHint),
       juce::CharPointer_UTF8("Release"),
       relRange,
       30.0f,  // valor por defecto
       juce::String(),
       juce::AudioParameterFloat::genericParameter,
       [](float value, int){
           // Para mostrar, usar 1.0f como mínimo visible
           float displayValue = juce::jmax(1.0f, value);

           // Formateo progresivo optimizado
           if (displayValue < 10.0f)
               return juce::String(displayValue, 1) + " ms";     // 1 decimal < 10ms
           else if (displayValue < 100.0f)
               return juce::String(displayValue, 1) + " ms";     // 1 decimal < 100ms  
           else
               return juce::String(displayValue, 0) + " ms";     // Sin decimales >= 100ms
       },
       nullptr);

   // f_ALGO @min 0 @max 2 @default 1
   auto algo = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("f_ALGO", versionHint),
                                                           juce::CharPointer_UTF8("Algorithm"),
                                                           juce::NormalisableRange<float>(0.f, 2.f, 0.01f, 1.f),
                                                           1.f,
                                                           juce::String(),
                                                           juce::AudioParameterFloat::genericParameter,
                                                           [](float value, int){
                                                               if (value < 0.5f)
                                                                   return juce::String("SHARP");
                                                               else if (value < 1.5f)
                                                                   return juce::String("EXPO RMS");
                                                               else
                                                                   return juce::String("SLOW RMS");
                                                           },
                                                           nullptr);

   // g_REACT @min 0 @max 1 @default 0
   auto react = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("g_REACT", versionHint),
                                                            juce::CharPointer_UTF8("React"),
                                                            juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
                                                            0.f,
                                                            juce::String(),
                                                            juce::AudioParameterFloat::genericParameter,
                                                            [](float value, int){
                                                                if (value < 0.01f)
                                                                    return juce::String("Peakish");
                                                                else if (value > 0.99f)
                                                                    return juce::String("RMSISH");
                                                                else
                                                                    return juce::String(value, 2);
                                                            },
                                                            nullptr);

   // h_KNEE @min 0 @max 30 @default 0 (Gen~ tiene rango 0-30)
   auto knee = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("h_KNEE", versionHint),
                                                           juce::CharPointer_UTF8("Knee"),
                                                           juce::NormalisableRange<float>(0.f, 30.f, 0.1f, 1.f),
                                                           0.f);

   // i_MAKEUP @min -12 @max 12 @default 0
   auto makeup = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("i_MAKEUP", versionHint),
                                                             juce::CharPointer_UTF8("Makeup Gain"),
                                                             juce::NormalisableRange<float>(-12.f, 12.f, 0.1f, 1.f),
                                                             0.f);

   // j_HPF @min 20 @max 20000 @default 20
   auto hpf = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("j_HPF", versionHint),
                                                          juce::CharPointer_UTF8("HPF"),
                                                          juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.2f), // Skew logarítmico para frecuencias
                                                          20.f);

   // k_LPF @min 20 @max 20000 @default 20000
   auto lpf = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("k_LPF", versionHint),
                                                          juce::CharPointer_UTF8("LPF"),
                                                          juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.2f), // Skew logarítmico para frecuencias
                                                          20000.f);

   // l_SC @min 0 @max 1 @default 0
   auto sc = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("l_SC", versionHint),
                                                       juce::CharPointer_UTF8("Filters"),
                                                       0, 1, 0);

   // m_SOLOSC @min 0 @max 1 @default 0 - NO AUTOMATABLE
   auto solosc = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("m_SOLOSC", versionHint),
                                                           juce::CharPointer_UTF8("SOLO-SC"),
                                                           0, 1, 0,
                                                           juce::AudioParameterIntAttributes().withAutomatable(false).withCategory(juce::AudioProcessorParameter::genericParameter));

   // n_LOOKAHEAD cuantizado a ENTEROS de muestra (paso = 1000 / SR ms)
   auto lookaheadRange = juce::NormalisableRange<float>(
       0.0f, 10.0f,
       // Mapeos lineales (0..1) <-> valor real
       [](float start, float end, float t) { return start + (end - start) * t; },
       [](float start, float end, float v) { return (v - start) / (end - start); },
       // Snap: cuantiza al múltiplo más cercano de 1 muestra (en ms) usando el SR actual
       [this](float start, float end, float v) {
           const double sr   = getSampleRate();
           const double step = (sr > 0.0 ? 1000.0 / sr : 0.0208333333333); // fallback: 48 kHz
           const double snapped = std::round(v / step) * step;
           const double clamped = juce::jlimit<double>(start, end, snapped);
           return static_cast<float>(clamped);
       }
   );

   auto lookahead = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("n_LOOKAHEAD", versionHint),
       juce::CharPointer_UTF8("Lookahead"),
       lookaheadRange,
       0.0f
   );

   // o_DRYWET @min 0 @max 1 @default 1
   auto drywet = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("o_DRYWET", versionHint),
                                                             juce::CharPointer_UTF8("Dry/Wet"),
                                                             juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
                                                             1.f,
                                                             juce::String(),
                                                             juce::AudioParameterFloat::genericParameter,
                                                             [](float value, int){
                                                                 return juce::String(int(value*100)) + " % WET";
                                                             },
                                                             nullptr);

   // p_BYPASS @min 0 @max 1 @default 0 - NO AUTOMATABLE
   auto bypass = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("p_BYPASS", versionHint),
                                                           juce::CharPointer_UTF8("INDIE-BYPASS"),
                                                           0, 1, 0,
                                                           juce::AudioParameterIntAttributes().withAutomatable(false).withCategory(juce::AudioProcessorParameter::genericParameter));

   // r_KEY @min 0 @max 1 @default 0
   auto key = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("r_KEY", versionHint),
                                                        juce::CharPointer_UTF8("External Key"),
                                                        0, 1, 0);

   // j_HPFORDER @min 0 @max 1 @default 0
   auto hpfOrder = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("j_HPFORDER", versionHint),
                                                             juce::CharPointer_UTF8("HPF Order"),
                                                             0, 1, 0);

   // k_LPFORDER @min 0 @max 1 @default 0
   auto lpfOrder = std::make_unique<juce::AudioParameterInt>(juce::ParameterID("k_LPFORDER", versionHint),
                                                             juce::CharPointer_UTF8("LPF Order"),
                                                             0, 1, 0);

   // s_AUTORELEASE @default false
   auto autoRelease = std::make_unique<juce::AudioParameterBool>(
       juce::ParameterID("s_AUTORELEASE", versionHint),
       juce::CharPointer_UTF8("Auto Release"),
       false,
       juce::String(),
       [](bool value, int){
           return value ? juce::String("AR ON") : juce::String("AR OFF");
       }
       );

   // u_SOFTCLIP @min 0 @max 2 @default 0
   auto softClip = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("u_SOFTCLIP", versionHint),
       juce::CharPointer_UTF8("Soft Clip"),
       juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
       0.f,
       juce::String(),
       juce::AudioParameterFloat::genericParameter,
       [](float value, int){
           if (value <= 0.f)
               return juce::String("OFF");
           else if (value >= 1.f)
               return juce::String("100%");
           else
               return juce::String(static_cast<int>(value * 100)) + "%";
       },
       nullptr);

   // v_DELTA @min 0 @max 1 @default 0 - NO AUTOMATABLE
   auto delta = std::make_unique<juce::AudioParameterInt>(
       juce::ParameterID("v_DELTA", versionHint),
       juce::CharPointer_UTF8("DELTA-LISTEN"),
       0, 1, 0,
       juce::AudioParameterIntAttributes().withAutomatable(false).withCategory(juce::AudioProcessorParameter::genericParameter));

   // w_PARALLEL @min 0 @max 1 @default 0
   auto parallel = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("w_PARALLEL", versionHint),
       juce::CharPointer_UTF8("Parallel"),
       juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
       0.0f,
       juce::String(),
       juce::AudioParameterFloat::genericParameter,
       [](float value, int){
           if (value < 0.01f)
               return juce::String("COMP");
           else if (value > 0.99f)
               return juce::String("COMP+DRY");
           else
               return juce::String(static_cast<int>(value * 100)) + "%";
       },
       nullptr);

   // x_AUTOGAIN @min 0 @max 1 @default 0
   auto autoGain = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("x_AUTOGAIN", versionHint),
       juce::CharPointer_UTF8("Auto Gain"),
       juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
       0.0f,
       juce::String(),
       juce::AudioParameterFloat::genericParameter,
       [](float value, int){
           if (value < 0.01f)
               return juce::String("OFF");
           else if (value > 0.99f)
               return juce::String(JUCE_UTF8("~70%"));
           else
               return juce::String(static_cast<int>(value * 100)) + "%";
       },
       nullptr);

   // y_SCTRIM @min -12 @max 12 @default 0
   auto scTrim = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("y_SCTRIM", versionHint),
       juce::CharPointer_UTF8("Sidechain Trim"),
       juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f, 1.0f),
       0.0f);

   // z_SMOOTH @min 0 @max 1 @default 0
   auto smooth = std::make_unique<juce::AudioParameterFloat>(
       juce::ParameterID("z_SMOOTH", versionHint),
       juce::CharPointer_UTF8("Smooth"),
       juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),  // Lineal, sin skew
       0.0f);  // Valor por defecto

   // Add parameters in ALPHABETICAL ORDER (exactamente como gen~)
   params.push_back(std::move(trim));           // a_TRIM
   params.push_back(std::move(thd));            // b_THD
   params.push_back(std::move(ratio));          // c_RATIO
   params.push_back(std::move(atk));            // d_ATK
   params.push_back(std::move(rel));            // e_REL
   params.push_back(std::move(algo));           // f_ALGO
   params.push_back(std::move(react));          // g_REACT
   params.push_back(std::move(knee));           // h_KNEE
   params.push_back(std::move(makeup));         // i_MAKEUP
   params.push_back(std::move(hpf));            // j_HPF
   params.push_back(std::move(lpf));            // k_LPF
   params.push_back(std::move(sc));             // l_SC
   params.push_back(std::move(solosc));         // m_SOLOSC
   params.push_back(std::move(lookahead));      // n_LOOKAHEAD
   params.push_back(std::move(drywet));         // o_DRYWET
   params.push_back(std::move(bypass));         // p_BYPASS
   params.push_back(std::move(key));            // r_KEY
   params.push_back(std::move(hpfOrder));       // j_HPFORDER
   params.push_back(std::move(lpfOrder));       // k_LPFORDER
   params.push_back(std::move(autoRelease));    // s_AUTORELEASE
   params.push_back(std::move(softClip));       // u_SOFTCLIP
   params.push_back(std::move(delta));          // v_DELTA
   params.push_back(std::move(parallel));       // w_PARALLEL
   params.push_back(std::move(autoGain));       // x_AUTOGAIN
   params.push_back(std::move(scTrim));         // y_SCTRIM
   params.push_back(std::move(smooth));         // z_SMOOTH

   // NOTA: El parámetro aax_gr_meter se añade directamente en el constructor
   // para evitar que forme parte del APVTS y el sistema undo/redo

   return { params.begin(), params.end() };
}


//==============================================================================
// GESTIÓN DE PARÁMETROS
//==============================================================================
// void JCBCompressorAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
// {
//     // Validar valores mínimos para ATK y REL
//     if (parameterID == "d_ATK" && newValue < 0.1f) {
//         newValue = 0.1f;
//     }
//     if (parameterID == "e_REL" && newValue < 0.1f) {
//         newValue = 0.1f;
//     }
//
//     // Buscar el índice correcto en Gen~ basado en el nombre del parámetro
//     int genIndex = -1;
//     for (int i = 0; i < JCBCompressor::num_params(); i++) {
//         if (parameterID == JCBCompressor::getparametername(m_PluginState, i)) {
//             genIndex = i;
//             break;
//         }
//     }
//
//     if (genIndex < 0) {
//         return;  // Parámetro no encontrado en Gen~
//     }
//
//
//     JCBCompressor::setparameter(m_PluginState, genIndex, newValue, nullptr);
//
//     // Actualizar latencia cuando cambia el lookahead
//     if (parameterID == "n_LOOKAHEAD")
//     {
//         double sampleRate = getSampleRate();
//
//         // Solo actualizar si tenemos un sample rate válido
//         if (sampleRate > 0)
//         {
//             // Cálculo estándar de latencia: ms * (sampleRate / 1000)
//             int latenciaSamples = static_cast<int>(newValue * sampleRate / 1000.0);
//
//             // Asegurar que nunca sea negativo
//             latenciaSamples = juce::jmax(0, latenciaSamples);
//
//             setLatencySamples(latenciaSamples+1);
//
//             // Lookahead latency compensation updated
//         }
//     }
// }

void JCBCompressorAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // Limites mínimos para ATK y REL
    if (parameterID == "d_ATK" && newValue < 0.1f) newValue = 0.1f;
    if (parameterID == "e_REL" && newValue < 0.1f) newValue = 0.1f;

    // Buscar índice en Gen por nombre
    int genIndex = -1;
    for (int i = 0; i < JCBCompressor::num_params(); ++i) {
        const char* raw = JCBCompressor::getparametername(m_PluginState, i);
        if (parameterID == juce::String(raw ? raw : "")) { 
            genIndex = i; 
            break; 
        }
    }

    if (genIndex < 0) return;

    // Debounce: NO aplicar lookahead de inmediato
    if (parameterID == "n_LOOKAHEAD")
    {
        stagedLookaheadMs.store(newValue, std::memory_order_relaxed);
        lastLAChangeMs.store(juce::Time::getMillisecondCounter(), std::memory_order_relaxed);
        laCommitPending.store(true, std::memory_order_release);
        return; // NO aplicar directo a Gen, se aplicará después del debounce
    }

    // Resto de parámetros → directo a Gen
    JCBCompressor::setparameter(m_PluginState, genIndex, newValue, nullptr);
    
    // NO llamar MessageManager::callAsync desde aquí - puede ejecutarse en audio thread
    // La actualización de UI se maneja desde el Timer del editor
}

//==============================================================================
// Métodos de programa (presets)
int JCBCompressorAudioProcessor::getNumPrograms()
{
    return 0; // 3 antes, añadir 1 por comportamiento extraño algunos hosts
}

int JCBCompressorAudioProcessor::getCurrentProgram()
{
    return currentProgram;
}

void JCBCompressorAudioProcessor::setCurrentProgram(int index)
{
    currentProgram = index;
}

const juce::String JCBCompressorAudioProcessor::getProgramName(int index)
{
    return juce::String();
}

void JCBCompressorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    // No implementado - los nombres de preset son fijos
}

//==============================================================================
// Métodos de medidores
float JCBCompressorAudioProcessor::getRmsInputValue(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return leftInputRMS.load(std::memory_order_relaxed);
    if (channel == 1)
        return rightInputRMS.load(std::memory_order_relaxed);
    return -100.0f;  // Return -100dB for invalid channels
}

float JCBCompressorAudioProcessor::getRmsOutputValue(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return leftOutputRMS.load(std::memory_order_relaxed);
    if (channel == 1)
        return rightOutputRMS.load(std::memory_order_relaxed);
    return -100.0f;  // Return -100dB for invalid channels
}

float JCBCompressorAudioProcessor::getGainReductionValue(const int channel) const noexcept
{
    jassert(channel == 0);
    if (channel == 0)
        return gainReduction.load(std::memory_order_relaxed);
    return 0.0f;
}

float JCBCompressorAudioProcessor::getSCValue(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return leftSC.load(std::memory_order_relaxed);
    if (channel == 1)
        return rightSC.load(std::memory_order_relaxed);
    return -100.0f;  // Return -100dB for invalid channels
}

//==============================================================================
// Utilidades
bool JCBCompressorAudioProcessor::isProTools() const noexcept
{
    juce::PluginHostType daw;
    return daw.isProTools();
}

juce::String JCBCompressorAudioProcessor::getPluginFormat() const noexcept
{
    // Detectar el formato del plugin en tiempo de ejecución
    const auto format = juce::PluginHostType().getPluginLoadedAs();
    
    switch (format)
    {
        case juce::AudioProcessor::wrapperType_VST3:
            return "VST3";
        case juce::AudioProcessor::wrapperType_AudioUnit:
            return "AU";
        case juce::AudioProcessor::wrapperType_AudioUnitv3:
            return "AUv3";
        case juce::AudioProcessor::wrapperType_AAX:
            return "AAX";
        case juce::AudioProcessor::wrapperType_VST:
            return "VST2";
        case juce::AudioProcessor::wrapperType_Standalone:
            return "Standalone";
        default:
            return "";
    }
}


//==============================================================================
// CAPTURA DE DATOS PARA VISUALIZACIÓN DE ENVOLVENTES
//==============================================================================
void JCBCompressorAudioProcessor::captureInputWaveformData(const juce::AudioBuffer<float>& inputBuffer, int numSamples)
{
    // AUDIO-THREAD SAFE: Usar try_lock para evitar bloquear el audio thread
    std::unique_lock<std::mutex> lock(waveformMutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        // Si no puede obtener el lock, salir inmediatamente para evitar RT violations
        return;
    }
    
    // AUDIO-THREAD SAFE: Usar tamaño fijo pre-asignado, no resize() dinámico
    if (currentInputSamples.size() < static_cast<size_t>(numSamples)) {
        // Solo redimensionar si es absolutamente necesario (debería estar pre-asignado)
        currentInputSamples.resize(juce::jmax(numSamples, 4096));
    }
    
    // CAMBIO: Usar las salidas 3 y 4 de Gen~ (post-TRIM) en lugar de entrada RAW
    // Esto da una visualización más precisa de lo que realmente está procesando el compresor
    for (int i = 0; i < numSamples; ++i)
    {
        // Usar directamente las salidas de Gen~ que son post-TRIM
        if (JCBCompressor::num_outputs() > 4) {
            // Promedio de canales L/R post-TRIM (salidas 3 y 4 de Gen~)
            currentInputSamples[i] = static_cast<float>((m_OutputBuffers[3][i] + m_OutputBuffers[4][i]) * 0.5);
        } else {
            // Fallback: usar solo canal izquierdo post-TRIM
            currentInputSamples[i] = static_cast<float>(m_OutputBuffers[3][i]);
        }
    }
}

void JCBCompressorAudioProcessor::captureOutputWaveformData(int numSamples)
{
    // AUDIO-THREAD SAFE: Usar try_lock para evitar bloquear el audio thread
    std::unique_lock<std::mutex> lock(waveformMutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        // Si no puede obtener el lock, salir inmediatamente para evitar RT violations
        return;
    }
    
    // AUDIO-THREAD SAFE: Usar tamaño fijo pre-asignado, no resize() dinámico
    if (currentProcessedSamples.size() < static_cast<size_t>(numSamples)) {
        currentProcessedSamples.resize(juce::jmax(numSamples, 4096));
    }
    if (currentGainReductionSamples.size() < static_cast<size_t>(numSamples)) {
        currentGainReductionSamples.resize(juce::jmax(numSamples, 4096));
    }
    
    // Copiar salida procesada (promedio de canales principales)
    // Y también capturar la gain reduction de Gen~ (salida 2)
    for (int i = 0; i < numSamples; ++i)
    {
        currentProcessedSamples[i] = (m_OutputBuffers[0][i] + m_OutputBuffers[1][i]) * 0.5f;
        
        // Capturar gain reduction directamente de Gen~ (salida 2)
        // Gen~ devuelve gain lineal, convertir a dB
        float grLinear = m_OutputBuffers[2][i];
        currentGainReductionSamples[i] = grLinear > 0.0f ? 
            20.0f * std::log10(grLinear) : -60.0f;
    }
}

void JCBCompressorAudioProcessor::getWaveformData(std::vector<float>& inputSamples, std::vector<float>& processedSamples) const
{
    std::lock_guard<std::mutex> lock(waveformMutex);
    inputSamples = currentInputSamples;
    processedSamples = currentProcessedSamples;
}

void JCBCompressorAudioProcessor::getWaveformDataWithGR(std::vector<float>& inputSamples, std::vector<float>& processedSamples, std::vector<float>& gainReductionSamples) const
{
    std::lock_guard<std::mutex> lock(waveformMutex);
    inputSamples = currentInputSamples;
    processedSamples = currentProcessedSamples;
    gainReductionSamples = currentGainReductionSamples;
}

float JCBCompressorAudioProcessor::getMaxGainReduction() const noexcept
{
    // Método optimizado que obtiene el valor máximo de gain reduction
    // directamente del valor atómico
    return gainReduction.load(std::memory_order_relaxed);
}


bool JCBCompressorAudioProcessor::isPlaybackActive() const noexcept
{
    // Siempre activo para decay permanente como plugins profesionales
    return true;
}


//==============================================================================
// GESTIÓN DEL EDITOR
//==============================================================================
juce::AudioProcessorEditor* JCBCompressorAudioProcessor::createEditor()
{
    return new JCBCompressorAudioProcessorEditor(*this, guiUndoManager);
}


//==============================================================================
// SERIALIZACIÓN DEL ESTADO
//==============================================================================
void JCBCompressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Crear una copia del state para no modificar el original
    auto stateCopy = apvts.state.createCopy();
    
    // Remover parámetros momentáneos antes de guardar
    // Estos botones no deben persistir entre sesiones
    auto paramsNode = stateCopy.getChildWithName("PARAMETERS");
    if (paramsNode.isValid()) {
        auto param = paramsNode.getChildWithProperty("id", "p_BYPASS");
        if (param.isValid())
            param.setProperty("value", 0.0f, nullptr);
        
        param = paramsNode.getChildWithProperty("id", "m_SOLOSC");
        if (param.isValid())
            param.setProperty("value", 0.0f, nullptr);
            
        param = paramsNode.getChildWithProperty("id", "v_DELTA");
        if (param.isValid())
            param.setProperty("value", 0.0f, nullptr);
    }
    
    auto preset = stateCopy.getOrCreateChildWithName("Presets", nullptr);
    preset.setProperty("currentPresetID", lastPreset, nullptr);
    preset.setProperty("tooltipEnabled", tooltipEnabled, nullptr);
    preset.setProperty("presetDisplayText", presetDisplayText, nullptr);
    preset.setProperty("presetTextItalic", presetTextItalic, nullptr);
    preset.setProperty("envelopeVisualEnabled", envelopeVisualEnabled, nullptr);
    preset.setProperty("tooltipLanguageEnglish", tooltipLanguageEnglish, nullptr);
    preset.setProperty("displayGraphicsEnvelopes", displayGraphicsEnvelopes, nullptr);
    
    // Guardar tamaño del editor
    preset.setProperty("editorWidth", editorSize.x, nullptr);
    preset.setProperty("editorHeight", editorSize.y, nullptr);
    
    // Save A/B states
    auto abNode = stateCopy.getOrCreateChildWithName("ABStates", nullptr);
    abNode.setProperty("isStateA", isStateA, nullptr);
    
    // Save state A
    auto stateANode = abNode.getOrCreateChildWithName("StateA", nullptr);
    stateANode.removeAllChildren(nullptr);
    for (const auto& [paramId, value] : stateA.values) {
        stateANode.setProperty(paramId, value, nullptr);
    }
    
    // Save state B
    auto stateBNode = abNode.getOrCreateChildWithName("StateB", nullptr);
    stateBNode.removeAllChildren(nullptr);
    for (const auto& [paramId, value] : stateB.values) {
        stateBNode.setProperty(paramId, value, nullptr);
    }
    
    juce::MemoryOutputStream memoria(destData, true);
    stateCopy.writeToStream(memoria);
}

void JCBCompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.state = tree;
        
        // Forzar parámetros momentáneos a OFF después de cargar
        apvts.getParameter("p_BYPASS")->setValueNotifyingHost(0.0f);
        apvts.getParameter("m_SOLOSC")->setValueNotifyingHost(0.0f);
        apvts.getParameter("v_DELTA")->setValueNotifyingHost(0.0f);
        
        // Clear undo history AFTER all values have been set
        // This prevents any parameter changes from being recorded in undo history
        guiUndoManager.clearUndoHistory();
        
        auto preset = apvts.state.getChildWithName("Presets");
        if (preset.isValid()) {
            lastPreset = preset.getProperty("currentPresetID");
            tooltipEnabled = preset.getProperty("tooltipEnabled");
            presetDisplayText = preset.getProperty("presetDisplayText", "DEFAULT");
            presetTextItalic = preset.getProperty("presetTextItalic", false);
            envelopeVisualEnabled = preset.getProperty("envelopeVisualEnabled", true);
            tooltipLanguageEnglish = preset.getProperty("tooltipLanguageEnglish", false);
            displayGraphicsEnvelopes = preset.getProperty("displayGraphicsEnvelopes", true);
            
            // Restaurar tamaño del editor
            int savedWidth = preset.getProperty("editorWidth", 1250);
            int savedHeight = preset.getProperty("editorHeight", 350);
            editorSize = {savedWidth, savedHeight};
        }
        
        // Restore A/B states
        auto abNode = apvts.state.getChildWithName("ABStates");
        if (abNode.isValid()) {
            isStateA = abNode.getProperty("isStateA", true);
            
            // Restore state A
            auto stateANode = abNode.getChildWithName("StateA");
            if (stateANode.isValid()) {
                stateA.values.clear();
                for (int i = 0; i < stateANode.getNumProperties(); ++i) {
                    auto propName = stateANode.getPropertyName(i);
                    stateA.values[propName.toString()] = stateANode[propName];
                }
            }
            
            // Restore state B
            auto stateBNode = abNode.getChildWithName("StateB");
            if (stateBNode.isValid()) {
                stateB.values.clear();
                for (int i = 0; i < stateBNode.getNumProperties(); ++i) {
                    auto propName = stateBNode.getPropertyName(i);
                    stateB.values[propName.toString()] = stateBNode[propName];
                }
            }
        }
        
        // IMPORTANTE: Sincronizar todos los parámetros con Gen~ después de cargar el estado
        for (int i = 0; i < JCBCompressor::num_params(); i++) {
            const char* rawName = JCBCompressor::getparametername(m_PluginState, i);
            auto paramName = juce::String(rawName ? rawName : "");
            if (auto* param = apvts.getRawParameterValue(paramName)) {
                float value = param->load();
                
                // Corregir valores muy pequeños en ATK y REL
                if (paramName == "d_ATK") {
                    if (value < 0.1f) {
                        value = 0.1f;
                        // Actualizar el parámetro en el APVTS
                        if (auto* audioParam = apvts.getParameter(paramName)) {
                            audioParam->setValueNotifyingHost(audioParam->convertTo0to1(value));
                        }
                    }
                }
                if (paramName == "e_REL") {
                    if (value < 0.1f) {
                        value = 0.1f;
                        // Actualizar el parámetro en el APVTS
                        if (auto* audioParam = apvts.getParameter(paramName)) {
                            audioParam->setValueNotifyingHost(audioParam->convertTo0to1(value));
                        }
                    }
                }
                // NOTA: El compresor no tiene parámetro HOLD (es del expansor/gate)
                
                parameterChanged(paramName, value);
            }
        }
        
        
        // Coherencia tras cargar estado: fijar lookahead staged y limpiar commit pendiente
        if (auto* laParam = apvts.getRawParameterValue("n_LOOKAHEAD"))
        {
            stagedLookaheadMs.store(laParam->load(), std::memory_order_relaxed);
            laCommitPending.store(false, std::memory_order_relaxed);
            lastLAChangeMs.store(juce::Time::getMillisecondCounter(), std::memory_order_relaxed);
        }
        
        // Marcar que el editor necesita actualización
        // No usar MessageManager::callAsync - puede ejecutarse desde audio thread
        // El editor se actualizará en su próximo timerCallback
        needsEditorUpdate.store(true);
    }
}

//==============================================================================
// COMPARACIÓN A/B
//==============================================================================
void JCBCompressorAudioProcessor::saveCurrentStateToActive() 
{
    if (isStateA) {
        stateA.captureFrom(apvts);
    } else {
        stateB.captureFrom(apvts);
    }
}

void JCBCompressorAudioProcessor::toggleAB() 
{
    // Save current state before switching
    saveCurrentStateToActive();
    
    // Switch state
    isStateA = !isStateA;
    
    // Load the other state
    if (isStateA) {
        stateA.applyTo(apvts);
    } else {
        stateB.applyTo(apvts);
    }
}

void JCBCompressorAudioProcessor::copyAtoB() 
{
    stateA.captureFrom(apvts);
    stateB = stateA;
}

void JCBCompressorAudioProcessor::copyBtoA() 
{
    stateB.captureFrom(apvts);
    stateA = stateB;
}

// isBeingAutomated() eliminado - ya no necesario con sistema undo simplificado

//==============================================================================
// MÉTODOS LEGACY
//==============================================================================
int JCBCompressorAudioProcessor::getNumParameters()
{
    // Retornar el número real de parámetros del juce::AudioProcessor
    // que incluye los de Gen~ más cualquier parámetro adicional (como AAX gain reduction)
    return static_cast<int>(getParameters().size());
}

float JCBCompressorAudioProcessor::getParameter(int index)
{
    // Verificar si el índice está dentro del rango de Gen~
    if (index < JCBCompressor::num_params())
    {
        t_param value;
        t_param min = JCBCompressor::getparametermin(m_PluginState, index);
        t_param range = fabs(JCBCompressor::getparametermax(m_PluginState, index) - min);
        
        JCBCompressor::getparameter(m_PluginState, index, &value);
        
        value = (value - min) / range;
        
        return value;
    }
    else
    {
        // Manejar parámetros adicionales (como AAX gain reduction)
        // Para el parámetro de gain reduction, devolver 0.0 (sin reducción)
        return 0.0f;
    }
}

void JCBCompressorAudioProcessor::setParameter(int index, float newValue)
{
    // Verificar si el índice está dentro del rango de Gen~
    if (index < JCBCompressor::num_params())
    {
        t_param min = JCBCompressor::getparametermin(m_PluginState, index);
        t_param range = fabs(JCBCompressor::getparametermax(m_PluginState, index) - min);
        t_param value = newValue * range + min;
        
        JCBCompressor::setparameter(m_PluginState, index, value, nullptr);
    }
    else
    {
        // Manejar parámetros adicionales (como AAX gain reduction)
        // El parámetro de gain reduction es de solo lectura, no hacer nada
    }
}

const juce::String JCBCompressorAudioProcessor::getParameterName(int index)
{
    // Verificar si el índice está dentro del rango de Gen~
    if (index < JCBCompressor::num_params())
    {
        const char* rawName = JCBCompressor::getparametername(m_PluginState, index);
        return juce::String(rawName ? rawName : "");
    }
    else
    {
        // Manejar parámetros adicionales (como AAX gain reduction)
        #if JucePlugin_Build_AAX
        if (index == JCBCompressor::num_params()) // Índice 25 para AAX
        {
            return "Gain Reduction";
        }
        #endif
        return "";
    }
}

const juce::String JCBCompressorAudioProcessor::getParameterText(int index)
{
    // Método legacy para compatibilidad con hosts - algunos DAWs pueden llamarlo
    if (index >= 0 && index < JCBCompressor::num_params())
    {
        juce::String text = juce::String(getParameter(index));
        text += juce::String(" ");
        const char* units = JCBCompressor::getparameterunits(m_PluginState, index);
        text += juce::String(units ? units : "");
        return text;
    }
    
    // Retornar string vacío para índices inválidos
    return "";
}

const juce::String JCBCompressorAudioProcessor::getInputChannelName(int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String JCBCompressorAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

bool JCBCompressorAudioProcessor::isInputChannelStereoPair(int index) const
{
    if (isProTools())
    {
        if (getMainBusNumInputChannels() > 1)
            return JCBCompressor::num_inputs() == 4;
        else
            return false;
    }
    else
        return JCBCompressor::num_inputs() == 4;
}

bool JCBCompressorAudioProcessor::isOutputChannelStereoPair(int index) const
{
    return JCBCompressor::num_outputs() == 2;
}


//==============================================================================
// Clip Detection Methods
//==============================================================================

void JCBCompressorAudioProcessor::updateClipDetection(const juce::AudioBuffer<float>& inputBuffer, const juce::AudioBuffer<float>& outputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const auto mainInputChannels = getMainBusNumInputChannels();
    const auto mainOutputChannels = getMainBusNumOutputChannels();
    
    // NOTA: El compresor no tiene soft clip, siempre detectar clips en salida
    bool isSoftClipActive = false; // Compresor no tiene soft clip
    
    // Reset clip flags for this buffer
    bool inputClip[2] = {false, false};
    bool outputClip[2] = {false, false};
    
    // Detectar clips en entrada (usando trimInputBuffer para consistencia con medidores)
    for (int channel = 0; channel < juce::jmin(2, mainInputChannels); ++channel) {
        for (int sample = 0; sample < numSamples; ++sample) {
            if (channel < trimInputBuffer.getNumChannels()) {
                float value = std::abs(trimInputBuffer.getSample(channel, sample));
                if (value >= 0.999f) {  // Umbral de clip ligeramente por debajo de 1.0
                    inputClip[channel] = true;
                    break;  // Solo necesitamos detectar un clip por buffer
                }
            }
        }
    }
    
    // Detectar clips en salida solo si soft clip NO está activo
    if (!isSoftClipActive) {
        for (int channel = 0; channel < juce::jmin(2, mainOutputChannels); ++channel) {
            for (int sample = 0; sample < numSamples; ++sample) {
                float value = std::abs(outputBuffer.getSample(channel, sample));
                if (value >= 0.999f) {  // Umbral de clip ligeramente por debajo de 1.0
                    outputClip[channel] = true;
                    break;  // Solo necesitamos detectar un clip por buffer
                }
            }
        }
    }
    
    // Actualizar flags atómicos
    for (int channel = 0; channel < 2; ++channel) {
        if (inputClip[channel]) {
            inputClipDetected[channel] = true;
        }
        if (outputClip[channel]) {
            outputClipDetected[channel] = true;
        }
    }
}

bool JCBCompressorAudioProcessor::getInputClipDetected(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel >= 0 && channel < 2) {
        return inputClipDetected[channel].load();
    }
    return false;
}

bool JCBCompressorAudioProcessor::getOutputClipDetected(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel >= 0 && channel < 2) {
        return outputClipDetected[channel].load();
    }
    return false;
}

bool JCBCompressorAudioProcessor::getSidechainClipDetected(const int channel) const noexcept
{
    jassert(channel == 0 || channel == 1);
    if (channel >= 0 && channel < 2) {
        return sidechainClipDetected[channel].load();
    }
    return false;
}

void JCBCompressorAudioProcessor::resetClipIndicators()
{
    for (int channel = 0; channel < 2; ++channel) {
        inputClipDetected[channel] = false;
        outputClipDetected[channel] = false;
        sidechainClipDetected[channel] = false;
    }
}

float JCBCompressorAudioProcessor::getGainReductionForHost() const noexcept
{
    // Devuelve el valor actual del gain reduction en dB
    // Este valor es actualizado en el audio thread y leído de forma segura aquí
    return currentGainReductionDb.load();
}

//==============================================================================
// Timer implementation
void JCBCompressorAudioProcessor::timerCallback()
{
    #if JucePlugin_Build_AAX
    // CRÍTICO: Verificar si estamos siendo destruidos para evitar acceso a memoria liberada
    if (isBeingDestroyed.load()) return;
    
    // Actualizar el parámetro zz_GR en Gen~ con el valor de gain reduction
    // Este parámetro es solo para visualización y no participa en undo/redo
    float grLinear = currentGainReductionLinear.load();
    
    // Actualizar el parámetro zz_GR (índice 25) en Gen~
    // JCBCompressor::setparameter(m_PluginState, 25, grLinear, nullptr); // No necesario - Gen~ ya conoce su propio GR
    
    // También actualizar el parámetro AAX para que Pro Tools lo muestre
    if (aaxGainReductionParam != nullptr) 
    {
        // Pro Tools espera 0.0 = sin reducción, 1.0 = máxima reducción
        float normalizedForProTools = juce::jlimit(0.0f, 1.0f, 1.0f - grLinear);
        
        // Usar setValueNotifyingHost para notificar a Pro Tools
        // Al no estar en APVTS, no afecta al undo/redo
        aaxGainReductionParam->setValueNotifyingHost(normalizedForProTools);
    }
    #endif
}

//==============================================================================
// Format-specific implementations

#if JucePlugin_Build_AAX
float JCBCompressorAudioProcessor::getAAXMeterValue(int meterID)
{
    // Pro Tools llama a este método para obtener el valor del medidor
    // El ID 0 es típicamente el gain reduction meter
    if (meterID == 0)
    {
        // Obtener el valor lineal de gain reduction
        float grLinear = currentGainReductionLinear.load();
        
        // Gen~ devuelve valores lineales donde:
        // 1.0 = sin reducción
        // < 1.0 = hay reducción
        // Pro Tools espera 0.0 = sin reducción, 1.0 = máxima reducción
        
        // Invertir el valor para Pro Tools
        float normalized = 1.0f - grLinear;
        
        return normalized;
    }
    
    return 0.0f;
}

#endif

#if JucePlugin_Build_VST3
void JCBCompressorAudioProcessor::updateVST3GainReduction()
{
    // Para VST3, necesitaríamos acceso al IEditController
    // Esto se haría típicamente en el wrapper VST3
    // Por ahora solo preparamos el método
}
#endif

//==============================================================================
// ASYNCUPDATER IMPLEMENTATION
//==============================================================================
void JCBCompressorAudioProcessor::handleAsyncUpdate()
{
    // Verificar si estamos siendo destruidos (opcional, para mayor seguridad)
    if (isBeingDestroyed.load()) return;

    // Obtener la latencia pendiente y marcarla como procesada
    const int L = pendingLatency.exchange(-1, std::memory_order_acq_rel);
    if (L < 0 || L == currentLatency) return;

    // Notificar al host y actualizar estado interno
    setLatencySamples(L);
    currentLatency = L;

    // Recalcular canales con fallback defensivo (por si el host cambia layout “a destiempo”)
    const int chans = juce::jmax(1, getTotalNumOutputChannels());

    // Actualizar bypass buffer con protección thread-safe
    const juce::SpinLock::ScopedLockType sl(bypassDelayLock);
    if (currentLatency > 0)
    {
        bypassDelayBuffer.setSize(chans, currentLatency);
        bypassDelayBuffer.clear();
        bypassDelayWritePos = 0;
    }
    else
    {
        bypassDelayBuffer.setSize(0, 0);
        bypassDelayWritePos = 0;
    }
}

//==============================================================================
// FACTORY FUNCTION DEL PLUGIN
//==============================================================================
/**
 * Función factory requerida por JUCE
 * CRÍTICO: Punto de entrada que utilizan los hosts (DAWs) para crear instancias del plugin
 * Esta función es llamada automáticamente por el framework JUCE cuando:
 * - El DAW carga el plugin por primera vez
 * - Se crea una nueva instancia del plugin en el proyecto
 * - Se duplica una pista que contiene el plugin
 */
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JCBCompressorAudioProcessor();
}