//==============================================================================
//
//  Copyright 2025 Juan Carlos Blancas
//  This file is part of JCBCompressor and is licensed under the GNU General Public License v3.0 or later.
//
//==============================================================================

//==============================================================================
// INCLUDES
//==============================================================================
// Módulos JUCE
#include <juce_graphics/juce_graphics.h>

// Librerías estándar C++
#include <mutex>
#include <algorithm>

// Archivos del proyecto
#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "Components/Windows/CreditsWindow.h"
#include "Components/Windows/CodeWindow.h"
#include "Helpers/UTF8Helper.h"
#include "BinaryData.h"


//==============================================================================
// CONSTRUCTOR Y DESTRUCTOR
//==============================================================================
JCBCompressorAudioProcessorEditor::JCBCompressorAudioProcessorEditor (JCBCompressorAudioProcessor& p, juce::UndoManager& um)
    : AudioProcessorEditor (&p), 
      processor (p), 
      undoManager (um),
      inputMeterL([&p](){ return p.getRmsInputValue(0); }),
      inputMeterR([&p](){ return p.getRmsInputValue(1); }),
      grMeter([this](){ return maxGainReductionFromBuffer; }),
      outputMeterL([&p](){ return p.getRmsOutputValue(0); }),
      outputMeterR([&p](){ return p.getRmsOutputValue(1); }),
      scMeterL([&p](){ return p.getSCValue(0); }), // SC meter L
      scMeterR([&p](){ return p.getSCValue(1); }) // SC meter R
{
    // Configurar todos los componentes
    setupBackground();
    setupKnobs();
    setupMeters();
    setupSidechainControls();
    setupPresetArea();
    setupUtilityButtons();
    setupParameterButtons();
    
    // Agregar display principal
    addAndMakeVisible(transferDisplay);
    
    // Agregar título y versión - mismo estilo que ExpansorGate
    auto titleFont = juce::Font(juce::FontOptions(22.0f));
    titleLink.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    titleLink.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    titleLink.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    titleLink.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    
    // Detectar si estamos en Logic Pro para mostrar solo la versión
    juce::String titleText;
    
    // Verificar si el host es Logic Pro
    juce::PluginHostType hostInfo;
    if (hostInfo.isLogic()) {
        titleText = "v1.0.0-alpha.1";  // Solo versión para Logic Pro
    } else {
        titleText = "JCBCompressor v1.0.0-alpha.1";  // Nombre completo para otros DAWs
    }
    
    titleLink.setButtonText(titleText);
    
    // NO agregar tooltip individual - solo usar la ventana general de tooltips
    // El tooltip se actualiza dinámicamente en updateTooltips() usando getTooltipText("title")
    titleLink.setTooltip("");
    
    // Configurar el click para mostrar créditos
    titleLink.onClick = [this]() {
        showCredits();
    };
    
    addAndMakeVisible(titleLink);
    
    
    // Agregar tooltip
    addAndMakeVisible(tooltipComponent);
    tooltipComponent.setAlwaysOnTop(true);
    tooltipComponent.showTip(JUCE_UTF8("Pasa el mouse sobre los controles\npara ver ayuda detallada"));
    
    
    // Establecer tamaño inicial con restricciones
    auto savedSize = processor.getSavedSize();
    int initialWidth = savedSize.x > 0 ? savedSize.x : DEFAULT_WIDTH;
    int initialHeight = savedSize.y > 0 ? savedSize.y : DEFAULT_HEIGHT;
    
    setSize(initialWidth, initialHeight);
    setResizable(true, true);
    
    // Inicializar el estado del tamaño basado en las dimensiones iniciales
    if (initialWidth == DEFAULT_WIDTH && initialHeight == DEFAULT_HEIGHT) {
        currentSizeState = GuiSizeState::Current;
    } else if (initialWidth == MIN_WIDTH && initialHeight == MIN_HEIGHT) {
        currentSizeState = GuiSizeState::Minimum;
    } else if (initialWidth == MAX_WIDTH && initialHeight == MAX_HEIGHT) {
        currentSizeState = GuiSizeState::Maximum;
    } else {
        // Para tamaños personalizados, usar estado Current por defecto
        currentSizeState = GuiSizeState::Current;
        lastCustomSize = {initialWidth, initialHeight};
    }

    // Establecer límites de redimensionado - valores exactos de JCBExpansorGate
    setResizeLimits(MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT);
    getConstrainer()->setFixedAspectRatio(ASPECT_RATIO);
    
    // Inicializar cache de contenido de código para thread safety
    initializeCodeContentCache();
    
    // Iniciar timer para updates - 60Hz como ExpansorGate
    startTimerHz(TIMER_HZ);
    
    // Timer para resetear indicadores de clip cada cierto tiempo
    clipResetCounter = 0;
    
    // Inicializar transfer display con valores actuales
    float thresholdDB = leftTopKnobs.thdSlider.getValue();
    transferDisplay.setThreshold(thresholdDB);
    transferDisplay.setRatio(leftTopKnobs.ratioSlider.getValue());
    transferDisplay.setKnee(leftTopKnobs.kneeSlider.getValue());
    
    // Conectar callbacks del transfer display para actualizar knobs
    transferDisplay.onThresholdChange = [this](float newThreshold) {
        leftTopKnobs.thdSlider.setValue(newThreshold, juce::sendNotification);
        handleParameterChange();  // Marcar preset como modificado
    };
    transferDisplay.onRatioChange = [this](float newRatio) {
        leftTopKnobs.ratioSlider.setValue(newRatio, juce::sendNotification);
        handleParameterChange();  // Marcar preset como modificado
    };
    transferDisplay.onKneeChange = [this](float newKnee) {
        leftTopKnobs.kneeSlider.setValue(newKnee, juce::sendNotification);
        handleParameterChange();  // Marcar preset como modificado
    };
    
    // Updates iniciales
    updateTransferDisplay();
    
    // Crear y registrar parameter listeners para updates de automatización
    transferFunctionListener = std::make_unique<TransferFunctionParameterListener>(this);
    processor.apvts.addParameterListener("b_THD", transferFunctionListener.get());
    processor.apvts.addParameterListener("c_RATIO", transferFunctionListener.get());
    processor.apvts.addParameterListener("h_KNEE", transferFunctionListener.get());
    processor.apvts.addParameterListener("s_AUTORELEASE", transferFunctionListener.get());
    
    // Registrar listener para DELTA para sincronización automática
    deltaParameterListener = std::make_unique<DeltaParameterListener>(this);
    processor.apvts.addParameterListener("v_DELTA", deltaParameterListener.get());
    
    // Configurar estado inicial del idioma
    if (processor.getTooltipLanguageEnglish()) {
        currentLanguage = TooltipLanguage::English;
        utilityButtons.tooltipLangButton.setButtonText("eng");
        utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOffId, DarkTheme::accentSecondary);
    } else {
        currentLanguage = TooltipLanguage::Spanish;
        utilityButtons.tooltipLangButton.setButtonText("esp");
        utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    }



    // Actualizar todos los tooltips basado en idioma inicial
    updateAllTooltips();
    
    // Configurar AUTO RELEASE button
    updateARButtonText();    // Establecer texto inicial basado en el estado
    updateRelSliderAlpha();  // Establecer alpha inicial del slider REL basado en el estado AR
    
    // Configurar estados iniciales
    bool initialDeltaState = processor.apvts.getRawParameterValue("v_DELTA")->load() > 0.5f;
    parameterButtons.deltaButton.setButtonText("DELTA");  // Texto siempre igual
    
    // Aplicar estado inicial de DELTA usando el método centralizado
    if (initialDeltaState) {
        applyDeltaModeToAllControls(true);
    }
    
    updateButtonStates();
    
    // Establecer estado inicial de SOLO SIDECHAIN en transfer display
    transferDisplay.setSoloSidechain(sidechainControls.soloScButton.getToggleState());
    
    // Configurar estado inicial del botón run graphics basado en el processor
    // Usar el estado guardado del botón graphics del processor
    bool graphicsButtonState = processor.displayGraphicsEnvelopes;
    // Graphics ON = envolventes VISIBLES (mostrar formas de onda en tiempo real)
    // Graphics OFF = envolventes OCULTAS (mejor rendimiento)
    bool initialEnvelopeState = graphicsButtonState; // Directo, sin inversión
    transferDisplay.setEnvelopeVisible(initialEnvelopeState);
    utilityButtons.runGraphicsButton.setToggleState(graphicsButtonState, juce::dontSendNotification);
    utilityButtons.runGraphicsButton.setButtonText("graphics");
    processor.setEnvelopeVisualEnabled(initialEnvelopeState);
    // Configurar visibilidad inicial del grMeter (visible cuando envolventes están visibles)
    grMeter.setVisible(initialEnvelopeState);
    
    // Actualizar valores de sliders desde APVTS para evitar problemas al cargar sesión
    // Usar MessageManager::callAsync para ejecución thread-safe sin delay
    juce::MessageManager::callAsync([safeThis = juce::Component::SafePointer<JCBCompressorAudioProcessorEditor>(this)]() {
        if (safeThis) {
            safeThis->updateSliderValues();
        }
    });
    
    // Configurar estado inicial del tooltip toggle y language button
    bool initialTooltipState = processor.getTooltipEnabled();
    tooltipsEnabled = initialTooltipState;
    tooltipComponent.setTooltipsEnabled(tooltipsEnabled);
    utilityButtons.tooltipToggleButton.setToggleState(tooltipsEnabled, juce::dontSendNotification);
    
    // Habilitar/deshabilitar botón de idioma según estado de tooltips
    utilityButtons.tooltipLangButton.setEnabled(tooltipsEnabled);
    utilityButtons.tooltipLangButton.setAlpha(tooltipsEnabled ? 1.0f : 0.25f);
    
    // Aplicar estados iniciales de todos los modos en el orden correcto
    updateButtonStates();
    
    
    // Inicializar texto del botón AR basado en el estado actual del parámetro
    updateARButtonText();
    
}

JCBCompressorAudioProcessorEditor::~JCBCompressorAudioProcessorEditor()
{
    // 1. Detener timer PRIMERO
    stopTimer();
    
    // 2. Reset attachments en orden inverso al de construcción
    // Botones de parámetros
    parameterButtons.deltaAttachment.reset();
    parameterButtons.bypassAttachment.reset();
    
    // Sidechain controls
    sidechainControls.lpfOrderAttachment.reset();
    sidechainControls.hpfOrderAttachment.reset();
    sidechainControls.soloScAttachment.reset();
    sidechainControls.keyAttachment.reset();
    sidechainControls.scAttachment.reset();
    sidechainControls.lpfAttachment.reset();
    sidechainControls.hpfAttachment.reset();
    
    // Right bottom knobs (time controls)
    rightBottomKnobs.autorelAttachment.reset();
    rightBottomKnobs.relAttachment.reset();
    rightBottomKnobs.atkAttachment.reset();
    
    // Right top controls (detector controls)
    rightTopControls.smoothAttachment.reset();
    rightTopControls.reactAttachment.reset();
    rightTopControls.algoAttachment.reset();
    
    // Left bottom knobs
    leftBottomKnobs.parallAttachment.reset();
    leftBottomKnobs.clipAttachment.reset();
    leftBottomKnobs.lookaheadAttachment.reset();
    leftBottomKnobs.drywetAttachment.reset();
    
    // Top knobs
    leftTopKnobs.againAttachment.reset();
    leftTopKnobs.kneeAttachment.reset();
    leftTopKnobs.ratioAttachment.reset();
    leftTopKnobs.thdAttachment.reset();
    
    // Main controls
    scTrimAttachment.reset();
    makeupAttachment.reset();
    trimAttachment.reset();
    
    // 3. Eliminar parameter listeners
    if (transferFunctionListener)
    {
        processor.apvts.removeParameterListener("b_THD", transferFunctionListener.get());
        processor.apvts.removeParameterListener("c_RATIO", transferFunctionListener.get());
        processor.apvts.removeParameterListener("h_KNEE", transferFunctionListener.get());
        processor.apvts.removeParameterListener("s_AUTORELEASE", transferFunctionListener.get());
    }
    
    if (deltaParameterListener)
    {
        processor.apvts.removeParameterListener("v_DELTA", deltaParameterListener.get());
    }
    
    // 4. LookAndFeel
    // Limpiar LookAndFeels de TODOS los sliders
    leftTopKnobs.thdSlider.setLookAndFeel(nullptr);
    leftTopKnobs.ratioSlider.setLookAndFeel(nullptr);
    leftTopKnobs.kneeSlider.setLookAndFeel(nullptr);
    leftTopKnobs.againSlider.setLookAndFeel(nullptr);
    
    leftBottomKnobs.drywetSlider.setLookAndFeel(nullptr);
    leftBottomKnobs.lookaheadSlider.setLookAndFeel(nullptr);
    leftBottomKnobs.clipSlider.setLookAndFeel(nullptr);
    leftBottomKnobs.parallSlider.setLookAndFeel(nullptr);
    
    rightBottomKnobs.atkSlider.setLookAndFeel(nullptr);
    rightBottomKnobs.relSlider.setLookAndFeel(nullptr);
    rightBottomKnobs.autorelButton.setLookAndFeel(nullptr);
    
    rightTopControls.algoSlider.setLookAndFeel(nullptr);
    rightTopControls.reactSlider.setLookAndFeel(nullptr);
    rightTopControls.smoothSlider.setLookAndFeel(nullptr);
    
    sidechainControls.hpfSlider.setLookAndFeel(nullptr);
    sidechainControls.lpfSlider.setLookAndFeel(nullptr);
    
    setLookAndFeel(nullptr);
}

//==============================================================================
// OVERRIDES DE JUCE  
//==============================================================================
void JCBCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // El background se maneja por el componente backgroundImage
}

void JCBCompressorAudioProcessorEditor::paintOverChildren (juce::Graphics& g)
{
    // Usar las mismas coordenadas que la transfer function display
    float x = 460.0f * 700.0f / 1247.0f;  // ≈ 258
    float y = 73.0f * 200.0f / 353.0f + 4.0f;    // ≈ 41 + 4 píxeles
    float w = 335.0f * 700.0f / 1247.0f;  // ≈ 188
    float h = 205.0f * 200.0f / 353.0f - 5.0f;   // ≈ 116 - 5 píxeles
    auto transferBounds = getScaledBounds(x, y, w, h).toFloat();
    
    // Texto BYPASS
    if (isBypassed && bypassTextVisible) {
        g.setColour(juce::Colours::transparentWhite.withAlpha(0.85f));
        g.setFont(juce::Font(juce::FontOptions(transferBounds.getHeight() * 0.4f))
                  .withStyle(juce::Font::bold));
        g.drawText("BYPASS", transferBounds, juce::Justification::centred);
    }
    
    // Texto DELTA (cuando está activo)
    if (parameterButtons.deltaButton.getToggleState()) {
        g.setColour(juce::Colours::transparentWhite.withAlpha(0.85f));
        g.setFont(juce::Font(juce::FontOptions(transferBounds.getHeight() * 0.4f))
                  .withStyle(juce::Font::bold));
        g.drawText("DELTA", transferBounds, juce::Justification::centred);
    }
    
    // Texto SOLO SIDECHAIN con info de filtros (cuando está activo)
    if (sidechainControls.soloScButton.getToggleState()) {
        g.setColour(juce::Colours::transparentWhite.withAlpha(0.85f));
        
        // Mostrar diferente texto según EXT KEY y estado de filtros
        juce::String soloText;
        bool filtersActive = sidechainControls.scButton.getToggleState();
        
        if (sidechainControls.keyButton.getToggleState()) {
            soloText = "SOLO EXT SC";
        } else {
            soloText = "SOLO INT SC";
        }
        
        // Texto principal en tamaño más grande
        g.setFont(juce::Font(juce::FontOptions(transferBounds.getHeight() * 0.25f))
                  .withStyle(juce::Font::bold));
        
        // Dibujar texto principal en la parte superior
        auto upperBounds = transferBounds;
        upperBounds = upperBounds.removeFromTop(transferBounds.getHeight() * 0.6f);
        g.drawText(soloText, upperBounds, juce::Justification::centred);
        
        // Texto de filtros en tamaño más pequeño
        g.setFont(juce::Font(juce::FontOptions(transferBounds.getHeight() * 0.15f)));
        g.setColour(juce::Colours::transparentWhite.withAlpha(0.7f));
        
        juce::String filterText = filtersActive ? "FILTERS ON" : "FILTERS OFF";
        auto lowerBounds = transferBounds;
        lowerBounds = lowerBounds.removeFromBottom(transferBounds.getHeight() * 0.4f);
        g.drawText(filterText, lowerBounds, juce::Justification::centred);
    }
}

void JCBCompressorAudioProcessorEditor::resized()
{
    // Guardar tamaño de ventana
    processor.setSavedSize({getWidth(), getHeight()});
    
    // Si el usuario redimensiona manualmente (no es uno de los tamaños predefinidos),
    // guardar como tamaño personalizado para poder volver a él
    int currentWidth = getWidth();
    int currentHeight = getHeight();

    if (currentWidth != DEFAULT_WIDTH && currentWidth != MIN_WIDTH && currentWidth != MAX_WIDTH &&
        currentHeight != DEFAULT_HEIGHT && currentHeight != MIN_HEIGHT && currentHeight != MAX_HEIGHT) {
        // Es un tamaño personalizado - guardarlo
        lastCustomSize = {currentWidth, currentHeight};
    }
    
    // El background llena toda el área
    backgroundImage.setBounds(getLocalBounds());
    // Asegurar que el background siempre esté al fondo al redimensionar
    backgroundImage.toBack();
    
    // === METERS CON SLIDERS DE TRIM/MAKEUP ===
    // Meters de entrada (lado izquierdo)
    inputMeterL.setBounds(getScaledBounds(2, 42, 12, 117));
    inputMeterR.setBounds(getScaledBounds(12, 42, 12, 117));
    
    // Meters de sidechain - ahora directamente adyacentes a los meters de entrada (sin gap)
    scMeterL.setBounds(getScaledBounds(24, 42, 12, 117));
    scMeterR.setBounds(getScaledBounds(34, 42, 12, 117));
    
    // Sliders de trim superpuestos a los meters
    trimSlider.setBounds(getScaledBounds(2, 40, 22, 130));  // Altura expandida para TextBox integrado
    
    // Sliders de trim de sidechain superpuestos a los medidores de sidechain
    scTrimSlider.setBounds(getScaledBounds(24, 40, 22, 130));  // Altura expandida para TextBox integrado
    
    // Medidor GR (centro-derecha) - ancho aumentado para mejor visibilidad
    grMeter.setBounds(getScaledBounds(440, 48, 6, 103));
    
    // Medidores de salida (lado derecho)
    outputMeterL.setBounds(getScaledBounds(677, 42, 12, 117));
    outputMeterR.setBounds(getScaledBounds(687, 42, 12, 117));
    
    // Sliders de makeup superpuestos a los medidores de salida
    makeupSlider.setBounds(getScaledBounds(677, 40, 22, 130));  // Altura expandida para TextBox integrado
    
    
    // === TRANSFER FUNCTION DISPLAY (CENTER) ===
    float x = 460.0f * 700.0f / 1247.0f;
    float y = 73.0f * 200.0f / 353.0f + 4.0f;
    float w = 335.0f * 700.0f / 1247.0f;
    float h = 205.0f * 200.0f / 353.0f - 5.0f;
    transferDisplay.setBounds(getScaledBounds(x, y, w, h));
    // === LEFT SIDE KNOBS === (Between SC meters and transfer function)
    // Top row - THD, RATIO, KNEE, PARAL
    leftTopKnobs.thdSlider.setBounds(getScaledBounds(50, 48, 53, 53));
    leftTopKnobs.ratioSlider.setBounds(getScaledBounds(100, 48, 53, 53));
    leftTopKnobs.kneeSlider.setBounds(getScaledBounds(150, 48, 53, 53));
    leftBottomKnobs.parallSlider.setBounds(getScaledBounds(200, 100, 53, 53));  // PARAL en la fila inferior
    // (200, 43, 53, 53)

    // Bottom row - D/W, LA, CLIP (AGAIN está en fila superior)
    leftBottomKnobs.drywetSlider.setBounds(getScaledBounds(50, 100, 53, 53));
    leftBottomKnobs.lookaheadSlider.setBounds(getScaledBounds(100, 100, 53, 53));  // LA → x=100
    leftTopKnobs.againSlider.setBounds(getScaledBounds(200, 48, 53, 53));
    leftBottomKnobs.clipSlider.setBounds(getScaledBounds(150, 100, 53, 53));  // CLIP → x=150
    // (150, 100, 53, 53)

    // === RIGHT SIDE CONTROLS ===
    // Top row - ALGO and REACT knobs (REACT alineado con REL)
    rightTopControls.algoSlider.setBounds(getScaledBounds(475, 48, 53, 53));
    rightTopControls.reactSlider.setBounds(getScaledBounds(529, 48, 53, 53));  // Alineado con REL
    rightTopControls.smoothSlider.setBounds(getScaledBounds(581, 48, 53, 53));  // SMOOTH knob en posición normal

    // Bottom row - Attack, Release, Autorel y Speed
    rightBottomKnobs.atkSlider.setBounds(getScaledBounds(500, 100, 53, 53));
    rightBottomKnobs.relSlider.setBounds(getScaledBounds(557, 100, 53, 53));  // Juntado con ATK
    rightBottomKnobs.autorelButton.setBounds(getScaledBounds(623, 110, 30, 20));  // AR button - discrete size
    
    // === SIDECHAIN CONTROLS (TOP CENTER) ===
    // HPF and LPF knobs swapped with their order buttons
    // Botón orden HPF ahora a la izquierda, perilla HPF se mueve a la derecha
    sidechainControls.hpfOrderButton.setBounds(getScaledBounds(265, 14, 24, 12));  // Was HPF knob position, moved up for icon space
    sidechainControls.hpfSlider.setBounds(getScaledBounds(290, 5, 36, 36));        // Era posición del botón de orden, expandido
    
    // Botón orden LPF ahora a la derecha, perilla LPF se mueve a la izquierda  
    sidechainControls.lpfSlider.setBounds(getScaledBounds(378, 5, 36, 36));        // Era posición del botón de orden, expandido
    sidechainControls.lpfOrderButton.setBounds(getScaledBounds(415, 14, 24, 12));  // Was LPF knob position, moved up for icon space
    
    // Filter icons below the order buttons
    hpfIcon.setBounds(getScaledBounds(265, 28, 24, 12));  // Debajo del botón de orden HPF
    lpfIcon.setBounds(getScaledBounds(415, 28, 24, 12));  // Debajo del botón de orden LPF
    
    
    // Botones SC, KEY y SOLO en el medio - mismo ancho, centrados en 355 (movidos a la derecha)
    // Movido arriba 1 píxel
    const int buttonWidth = 50;
    const int centerX = 353;
    // Posicionamiento del botón SC
    sidechainControls.scButton.setBounds(getScaledBounds(centerX - buttonWidth/2, 5, buttonWidth, 12));
    sidechainControls.keyButton.setBounds(getScaledBounds(centerX - buttonWidth/2, 17, buttonWidth, 12));
    sidechainControls.soloScButton.setBounds(getScaledBounds(centerX - buttonWidth/2, 29, buttonWidth, 12));
    
    // === PRESET AREA (TOP LEFT) ===
    presetArea.saveButton.setBounds(getScaledBounds(5, 15, 20, 12));  // Alineado con undo
    presetArea.saveAsButton.setBounds(getScaledBounds(27, 15, 25, 12));
    presetArea.deleteButton.setBounds(getScaledBounds(54, 15, 25, 12));
    presetArea.backButton.setBounds(getScaledBounds(81, 15, 14, 12));
    presetArea.nextButton.setBounds(getScaledBounds(98, 15, 14, 12));
    presetArea.presetMenu.setBounds(getScaledBounds(114, 15, 95, 12));
    
    // Botones A/B junto al menú de preset
    topButtons.abStateButton.setBounds(getScaledBounds(213, 15, 18, 12));
    topButtons.abCopyButton.setBounds(getScaledBounds(233, 15, 22, 12));
    
    // === BOTONES DE UTILIDAD (INFERIOR IZQUIERDA) ===
    utilityButtons.undoButton.setBounds(getScaledBounds(15, 179, 22, 12));
    utilityButtons.redoButton.setBounds(getScaledBounds(37, 179, 22, 12));
    utilityButtons.resetGuiButton.setBounds(getScaledBounds(61, 179, 30, 12));
    utilityButtons.runGraphicsButton.setBounds(getScaledBounds(93, 179, 30, 12));
    utilityButtons.zoomButton.setBounds(getScaledBounds(125, 179, 30, 12));
    // Mover botones de tooltip y lenguaje donde estaba el diagram
    utilityButtons.tooltipToggleButton.setBounds(getScaledBounds(157, 179, 30, 12));
    utilityButtons.tooltipLangButton.setBounds(getScaledBounds(189, 179, 22, 12));
    
    // Botones DELTA, DIAGRAM y BYPASS - DIAGRAM alineado con botones de sidechain arriba
    const int centerButtonsY = 163;
    const int diagramCenterX = 355; // Mismo centerX que botones de sidechain
    const int buttonSpacing = 45;   // Espaciado entre DELTA-DIAGRAM y DIAGRAM-BYPASS
    
    // DIAGRAM centrado en mismo X que botones de sidechain (FILTERS, EXT KEY, SOLO SC)
    centerButtons.diagramButton.setBounds(getScaledBounds(diagramCenterX - 22, centerButtonsY, 44, 12));
    // DELTA a la izquierda de DIAGRAM
    parameterButtons.deltaButton.setBounds(getScaledBounds(diagramCenterX - buttonSpacing - 20, centerButtonsY, 40, 12));
    // BYPASS a la derecha de DIAGRAM  
    parameterButtons.bypassButton.setBounds(getScaledBounds(diagramCenterX + buttonSpacing - 20, centerButtonsY, 40, 12));
    
    // Botones TODO movidos abajo a la derecha, centrados en el rectángulo
    // Calcular posición central para el grupo de botones TODO
    const int todoStartX = 500;    // Movido a la derecha para mejor centrado en el rectángulo
    const int todoY = 179;       // Mismo Y que botones de utilidad
    utilityButtons.hqButton.setBounds(getScaledBounds(todoStartX, todoY, 18, 12));
    utilityButtons.dualMonoButton.setBounds(getScaledBounds(todoStartX + 20, todoY, 23, 12));
    utilityButtons.stereoLinkedButton.setBounds(getScaledBounds(todoStartX + 45, todoY, 23, 12));
    utilityButtons.msButton.setBounds(getScaledBounds(todoStartX + 70, todoY, 18, 12));
    // Botón MIDI posicionado a la derecha del botón M/S
    utilityButtons.midiLearnButton.setBounds(getScaledBounds(todoStartX + 90, todoY, 23, 12));
    
    // === TÍTULO Y VERSIÓN (CENTRO INFERIOR) ===
    // Ancho similar a botones DIAGRAM + GEN DSP combinados
    titleLink.setBounds(getScaledBounds(304, 179, 95, 15));
    
    // Tooltip en esquina superior derecha - ajustado al rectángulo visible
    tooltipComponent.setBounds(getScaledBounds(450, 0, 228, 42));
    
    // Actualizar factor de escala del tooltip basado en el tamaño de la ventana
    float scaleFactor = (float)getWidth() / (float)DEFAULT_WIDTH;
    tooltipComponent.setScaleFactor(scaleFactor);
    
    // Redimensionar el overlay del diagrama si está visible
    if (diagramOverlay != nullptr)
    {
        diagramOverlay->setBounds(getLocalBounds());
        diagramOverlay->invalidateClickableAreasCache(); // Invalidar cache para regenerar con nuevas coordenadas escaladas
    }
    
    // Redimensionar el overlay de créditos si está visible
    if (creditsOverlay != nullptr)
    {
        creditsOverlay->setBounds(getLocalBounds());
    }
    
    // Redimensionar y reposicionar la ventana de código si está visible
    if (codeWindow != nullptr && codeWindow->isVisible())
    {
        // Recalcular tamaño responsivo
        int windowWidth = static_cast<int>(getWidth() * 0.35f);
        int windowHeight = static_cast<int>(getHeight() * 0.50f);
        
        // Limitar tamaños
        windowWidth = juce::jlimit(350, 600, windowWidth);
        windowHeight = juce::jlimit(250, 450, windowHeight);
        
        // Recentrar la ventana
        int x = getWidth() / 2 - windowWidth / 2;
        int y = getHeight() / 2 - windowHeight / 2;
        
        codeWindow->setBounds(x, y, windowWidth, windowHeight);
    }
}

void JCBCompressorAudioProcessorEditor::timerCallback()
{
    // Verificar si el processor necesita actualización del editor
    if (processor.needsEditorUpdate.exchange(false))
    {
        updateTransferFunctionFromProcessor();
    }
    
    // Procesar actualizaciones de UI pendientes desde atómicos (thread-safe)
    if (hasUIUpdates.load())
    {
        hasUIUpdates.store(false);
        
        // Procesar cambios de TransferFunction
        float thresholdValue = pendingThresholdValue.exchange(-1.0f);
        float ratioValue = pendingRatioValue.exchange(-1.0f);
        float kneeValue = pendingKneeValue.exchange(-1.0f);
        
        if (thresholdValue >= 0.0f || ratioValue >= 0.0f || kneeValue >= 0.0f) {
            updateTransferDisplay();
        }
        
        // Procesar cambio de AutoRelease
        float autoReleaseValue = pendingAutoReleaseValue.exchange(-1.0f);
        if (autoReleaseValue >= 0.0f) {
            updateARButtonText();
            updateRelSliderAlpha();
        }
        
        // Procesar cambio de Delta
        float deltaValue = pendingDeltaValue.exchange(-1.0f);
        if (deltaValue >= 0.0f) {
            bool deltaActive = deltaValue > 0.5f;
            applyDeltaModeToAllControls(deltaActive);
        }
    }
    
    // Sistema universal de decay para todos los DAWs
    applyMeterDecayIfNeeded();
    
    updateMeters();
    
    // Actualizar estado visual de botones undo/redo
    bool canUndo = undoManager.canUndo();
    bool canRedo = undoManager.canRedo();
    
    // Actualizar apariencia del botón undo
    utilityButtons.undoButton.setEnabled(canUndo);
    utilityButtons.undoButton.setAlpha(canUndo ? 1.0f : 0.3f);
    utilityButtons.undoButton.setColour(juce::TextButton::textColourOffId, 
                                       canUndo ? DarkTheme::textPrimary : DarkTheme::textSecondary);
    
    // Actualizar apariencia del botón redo
    utilityButtons.redoButton.setEnabled(canRedo);
    utilityButtons.redoButton.setAlpha(canRedo ? 1.0f : 0.3f);
    utilityButtons.redoButton.setColour(juce::TextButton::textColourOffId, 
                                       canRedo ? DarkTheme::textPrimary : DarkTheme::textSecondary);
    
    // Actualizar texto y tooltip del botón A/B copy dinámicamente
    bool isSpanish = !processor.getTooltipLanguageEnglish();
    if (processor.getIsStateA()) {
        topButtons.abCopyButton.setButtonText("A-B");
        topButtons.abCopyButton.setTooltip(isSpanish ? "Copiar A a B" : "Copy A to B");
    } else {
        topButtons.abCopyButton.setButtonText("B-A");
        topButtons.abCopyButton.setTooltip(isSpanish ? "Copiar B a A" : "Copy B to A");
    }
    
    // Resetear indicadores de clip cada 3 segundos (180 frames a 60Hz)
    clipResetCounter++;
    if (clipResetCounter >= 180) {
        processor.resetClipIndicators();
        clipResetCounter = 0;
    }
    
    
    // Pasar el estado de Logic parado al display
    // Determinar si el procesamiento está inactivo
    // Usar sistema híbrido optimizado (timestamp + playhead + audio tail)
    bool isProcessingInactive = !processor.isPlaybackActive();
    
    // Sistema original comentado en processBlock:
    // bool isProcessingInactive = processor.getIsLogicStopped();
    transferDisplay.setLogicStoppedState(isProcessingInactive);
    
    // Actualizar datos de waveform y obtener gain reduction para el medidor
    if (!isBypassed && !isProcessingInactive)
    {
        // Optimización: Solo obtener buffers completos si el histograma está visible
        if (transferDisplay.isEnvelopeVisible())
        {
            std::vector<float> inputSamples, processedSamples, gainReductionSamples;
            processor.getWaveformDataWithGR(inputSamples, processedSamples, gainReductionSamples);
            
            if (!gainReductionSamples.empty())
            {
                // Encontrar el valor máximo de gain reduction en el buffer
                // Los valores vienen negativos de Gen~, así que buscamos el más negativo
                float maxGR = 0.0f;
                for (float gr : gainReductionSamples) {
                    if (gr < maxGR) {
                        maxGR = gr;
                    }
                }
                maxGainReductionFromBuffer = maxGR;
                
                // Actualizar transfer display con datos de waveform
                if (!inputSamples.empty() && !processedSamples.empty())
                {
                    // Actualizar estado EXT KEY y nivel de sidechain
                    bool keyEnabled = sidechainControls.keyButton.getToggleState();
                    transferDisplay.setExtKeyActive(keyEnabled);
                    
                    // Obtener nivel promedio de sidechain desde los medidores
                    float scLevel = (processor.getSCValue(0) + processor.getSCValue(1)) * 0.5f;
                    transferDisplay.setSidechainLevel(scLevel);
                    
                    // Enviar solo la muestra más reciente (ya con detección de picos)
                    transferDisplay.updateWaveformDataWithGR(&inputSamples[0], &processedSamples[0], &gainReductionSamples[0], 1);
                    
                    // Forzar repintado para mostrar envolvente actualizada
                    transferDisplay.repaint();
                }
            }
        }
        else
        {
            // Optimización: Cuando el histograma no está visible, solo obtener el valor máximo
            maxGainReductionFromBuffer = processor.getMaxGainReduction();
        }
    }
    else
    {
        // Si está en bypass o Logic está parado, no hay reducción
        maxGainReductionFromBuffer = 0.0f;
        
        // Forzar repaint del transfer display cuando Logic está parado
        // para que las envolventes y el histograma desaparezcan
        if (isProcessingInactive)
        {
            transferDisplay.repaint();
        }
    }
}

void JCBCompressorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    // Simplificado - sin manejo automático de cambios de parámetros para undo/redo
    
    if (button == &utilityButtons.undoButton)
    {
        undoManager.undo();
    }
    else if (button == &utilityButtons.redoButton)
    {
        undoManager.redo();
    }
    else if (button == &utilityButtons.resetGuiButton)
    {
        // Resetear tamaño de GUI, no parámetros
        resetGuiSize();
    }
    else if (button == &parameterButtons.bypassButton)
    {
        if (parameterButtons.bypassButton.getToggleState()) {
            // BYPASS desactiva DELTA, SOLO SC y DIAGRAM
            parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
            sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
            if (centerButtons.diagramButton.getToggleState()) {
                centerButtons.diagramButton.setToggleState(false, juce::dontSendNotification);
                hideDiagram(); // Cerrar DIAGRAM si está abierto
            }
        }
        updateButtonStates();
        
        // Actualizar transfer display para ocultar/mostrar envolventes basado en estado de bypass
        bool bypassActive = parameterButtons.bypassButton.getToggleState();
        transferDisplay.setBypassMode(bypassActive);
        
        // Actualizar output meters para usar gradient de entrada cuando bypass está activo
        outputMeterL.setBypassMode(bypassActive);
        outputMeterR.setBypassMode(bypassActive);
    }
    else if (button == &sidechainControls.scButton)
    {
        // Actualizar texto del botón y visibilidad del filtro
        updateButtonStates();
    }
    else if (button == &sidechainControls.keyButton)
    {
        // Actualizar texto del botón según su estado
        bool keyActive = sidechainControls.keyButton.getToggleState();
        sidechainControls.keyButton.setButtonText(keyActive ? "SC EXT" : "SC INT");
        
        // Actualizar visibilidad del medidor de sidechain cuando KEY es activado
        updateButtonStates();
        
        // Actualizar estado EXT KEY en transfer display
        transferDisplay.setExtKeyActive(sidechainControls.keyButton.getToggleState());
    }
    else if (button == &sidechainControls.soloScButton)
    {
        if (sidechainControls.soloScButton.getToggleState()) {
            // SOLO SC desactiva BYPASS, DELTA y DIAGRAM
            parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
            parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
            if (centerButtons.diagramButton.getToggleState()) {
                centerButtons.diagramButton.setToggleState(false, juce::dontSendNotification);
                hideDiagram(); // Cerrar DIAGRAM si está abierto
            }
        }
        
        updateButtonStates();
        
        // Actualizar transfer function display para estado SOLO SC
        bool soloActive = sidechainControls.soloScButton.getToggleState();
        transferDisplay.setSoloSidechain(soloActive);
    }
    else if (button == &utilityButtons.runGraphicsButton)
    {
        bool newState = utilityButtons.runGraphicsButton.getToggleState();
        // Graphics ON = mostrar envolventes en tiempo real
        // Graphics OFF = ocultar envolventes (mejor rendimiento)
        transferDisplay.setEnvelopeVisible(newState); // Directo, sin inversión
        processor.setEnvelopeVisualEnabled(newState); // Directo, sin inversión
        processor.displayGraphicsEnvelopes = newState; // Guardar estado para persistencia en presets
        // grMeter visible cuando graphics está ON (y SOLO SC no está activo)
        bool soloScActive = sidechainControls.soloScButton.getToggleState();
        grMeter.setVisible(newState && !soloScActive);
        // Mantener el texto siempre como "graphics"
        
        // Marcar el preset como modificado cuando se cambia el estado de graphics
        handleParameterChange();
    }
    // Botones de gestión de presets
    else if (button == &presetArea.saveButton)
    {
        // Desactivar botones momentáneos antes de guardar
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        savePresetFile();
    }
    else if (button == &presetArea.saveAsButton)
    {
        // Desactivar botones momentáneos antes de guardar
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        saveAsPresetFile();
    }
    else if (button == &presetArea.deleteButton)
    {
        // Desactivar botones momentáneos antes de mostrar el diálogo
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        deletePresetFile();
    }
    else if (button == &presetArea.backButton)
    {
        // Desactivar botones momentáneos antes de cambiar preset
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        selectPreviousPreset();
    }
    else if (button == &presetArea.nextButton)
    {
        // Desactivar botones momentáneos antes de cambiar preset
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        selectNextPreset();
    }
    else if (button == &utilityButtons.tooltipToggleButton)
    {
        // Alternar visibilidad del tooltip
        tooltipsEnabled = utilityButtons.tooltipToggleButton.getToggleState();
        tooltipComponent.setTooltipsEnabled(tooltipsEnabled);
        
        // Guardar estado del tooltip al processor
        processor.setTooltipEnabled(tooltipsEnabled);
        
        // Habilitar/deshabilitar botón de idioma basado en estado del tooltip
        utilityButtons.tooltipLangButton.setEnabled(tooltipsEnabled);
        utilityButtons.tooltipLangButton.setAlpha(tooltipsEnabled ? 1.0f : 0.25f);
    }
    else if (button == &utilityButtons.tooltipLangButton)
    {
        // Alternar idioma
        currentLanguage = (currentLanguage == TooltipLanguage::Spanish) ? 
                         TooltipLanguage::English : TooltipLanguage::Spanish;
        
        // Actualizar texto del botón
        utilityButtons.tooltipLangButton.setButtonText(
            currentLanguage == TooltipLanguage::Spanish ? "esp" : "eng");
        
        // Actualizar color del botón basado en el idioma
        if (currentLanguage == TooltipLanguage::English) {
            // Color azul para inglés
            utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOffId, DarkTheme::accentSecondary);
        } else {
            // Color por defecto para español
            utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
        }
        
        // Guardar preferencia de idioma
        processor.setTooltipLanguageEnglish(currentLanguage == TooltipLanguage::English);
        
        // Actualizar todos los tooltips con el nuevo idioma
        updateAllTooltips();
        
        // Actualizar textos de botones TODO con el nuevo idioma
        updateTodoButtonTexts();
    }
    else if (button == &utilityButtons.hqButton)
    {
        // TODO: Implementar oversampling HQ
    }
    else if (button == &utilityButtons.dualMonoButton)
    {
        // Manejar exclusividad de botones de modo estéreo
        if (utilityButtons.dualMonoButton.getToggleState())
        {
            // utilityButtons.stereoLinkedButton.setToggleState(false, juce::dontSendNotification); // LINK siempre está activo
            utilityButtons.msButton.setToggleState(false, juce::dontSendNotification);
        }
    }
    /* Botón LINK siempre está ON y deshabilitado - no necesita manejador
    else if (button == &utilityButtons.stereoLinkedButton)
    {
        // Manejar exclusividad de botones de modo estéreo
        if (utilityButtons.stereoLinkedButton.getToggleState())
        {
            utilityButtons.dualMonoButton.setToggleState(false, juce::dontSendNotification);
            utilityButtons.msButton.setToggleState(false, juce::dontSendNotification);
        }
    }
    */
    else if (button == &utilityButtons.msButton)
    {
        // Manejar exclusividad de botones de modo estéreo
        if (utilityButtons.msButton.getToggleState())
        {
            utilityButtons.dualMonoButton.setToggleState(false, juce::dontSendNotification);
            // utilityButtons.stereoLinkedButton.setToggleState(false, juce::dontSendNotification); // LINK siempre está activo
        }
    }
    else if (button == &topButtons.abStateButton)
    {
        // Salir de DELTA antes de alternar estado A/B
        exitDeltaMode();
        
        // Alternar estado A/B
        processor.toggleAB();
        
        // Actualizar texto del botón para mostrar estado actual
        topButtons.abStateButton.setButtonText(processor.getIsStateA() ? "A" : "B");
        
        // Retroalimentación visual con colores más evidentes
        if (processor.getIsStateA()) {
            // A = Púrpura/Violeta
            topButtons.abStateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff9c27b0));  // Purple
            topButtons.abStateButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        } else {
            // B = Azul
            topButtons.abStateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2196f3));  // Blue
            topButtons.abStateButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        }
        
        repaint();
    }
    else if (button == &topButtons.abCopyButton)
    {
        // Salir de DELTA antes de copiar estado
        exitDeltaMode();
        
        // Copiar estado actual al otro
        if (processor.getIsStateA()) {
            processor.copyAtoB();
            // Retroalimentación visual - destello púrpura a azul
            topButtons.abCopyButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2196f3));  // Destello azul
            topButtons.abCopyButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
            // Usar SafePointer para prevenir crashes si el editor se destruye antes de que se dispare el timer
            juce::Component::SafePointer<JCBCompressorAudioProcessorEditor> safeThis(this);
            juce::Timer::callAfterDelay(300, [safeThis]() {
                if (safeThis) {
                    safeThis->topButtons.abCopyButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
                    safeThis->topButtons.abCopyButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
                }
            });
        } else {
            processor.copyBtoA();
            // Retroalimentación visual - destello azul a púrpura
            topButtons.abCopyButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff9c27b0));  // Destello púrpura
            topButtons.abCopyButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
            // Usar SafePointer para prevenir crashes si el editor se destruye antes de que se dispare el timer
            juce::Component::SafePointer<JCBCompressorAudioProcessorEditor> safeThis(this);
            juce::Timer::callAfterDelay(300, [safeThis]() {
                if (safeThis) {
                    safeThis->topButtons.abCopyButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
                    safeThis->topButtons.abCopyButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
                }
            });
        }
    }
    else if (button == &utilityButtons.midiLearnButton)
    {
        // TODO: Implementar modo MIDI learn
    }
    else if (button == &utilityButtons.zoomButton)
    {
        // Ciclar entre dos niveles de zoom
        auto currentZoom = transferDisplay.getZoomLevel();
        TransferFunctionDisplay::ZoomLevel newZoom;
        
        switch (currentZoom)
        {
            case TransferFunctionDisplay::ZoomLevel::Normal:
                // Normal -> Ampliado
                newZoom = TransferFunctionDisplay::ZoomLevel::Zoomed;
                utilityButtons.zoomButton.setButtonText("zoom x2");
                utilityButtons.zoomButton.setToggleState(true, juce::dontSendNotification);
                grMeter.setZoomLevel(true);  // Sincronizar medidor GR con zoom x2 (-48dB a 0dB)
                break;
                
            case TransferFunctionDisplay::ZoomLevel::Zoomed:
                // Ampliado -> Normal
                newZoom = TransferFunctionDisplay::ZoomLevel::Normal;
                utilityButtons.zoomButton.setButtonText("zoom");
                utilityButtons.zoomButton.setToggleState(false, juce::dontSendNotification);
                grMeter.setZoomLevel(false);
                break;
                
            default:
                // Por defecto a Normal si algo sale mal
                newZoom = TransferFunctionDisplay::ZoomLevel::Normal;
                utilityButtons.zoomButton.setButtonText("zoom");
                utilityButtons.zoomButton.setToggleState(false, juce::dontSendNotification);
                grMeter.setZoomLevel(false);
                break;
        }
        
        transferDisplay.setZoomLevel(newZoom);
    }
    else if (button == &centerButtons.diagramButton)
    {
        // Anti-rebote para evitar clicks múltiples
        juce::uint32 currentTime = juce::Time::getMillisecondCounter();
        if (currentTime - lastDiagramButtonTime < DIAGRAM_BUTTON_DEBOUNCE_MS)
        {
            return;  // Ignorar clicks muy rápidos
        }
        lastDiagramButtonTime = currentTime;
        
        // Alternar diagrama basado en visibilidad actual
        bool diagramWillBeActivated = (diagramOverlay == nullptr || !diagramOverlay->isVisible());
        
        // Comportamiento exclusivo: solo desactivar otros cuando DIAGRAM se va a activar
        if (diagramWillBeActivated) {
            // DIAGRAM desactiva BYPASS, DELTA y SOLO SC cuando se activa
            parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
            parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
            sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
        }
        
        // Ejecutar el alternado
        if (diagramOverlay != nullptr && diagramOverlay->isVisible())
        {
            hideDiagram();
        }
        else
        {
            showDiagram();
        }
    }
    // Manejador del botón CODE removido - funcionalidad manejada por botón DIAGRAM
    else if (button == &parameterButtons.deltaButton)
    {
        bool deltaActive = parameterButtons.deltaButton.getToggleState();
        
        if (deltaActive) {
            // DELTA desactiva BYPASS, SOLO SC y DIAGRAM
            parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
            sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
            if (centerButtons.diagramButton.getToggleState()) {
                centerButtons.diagramButton.setToggleState(false, juce::dontSendNotification);
                hideDiagram(); // Cerrar DIAGRAM si está abierto
            }
        }
        
        // Aplicar estado centralizado de DELTA
        applyDeltaModeToAllControls(deltaActive);
        
        updateButtonStates();
    }
}

//==============================================================================
// MANEJO DE PARÁMETROS Y EVENTOS
//==============================================================================
//==============================================================================
// GESTIÓN DE ESTADO DE PARÁMETROS
//==============================================================================
void JCBCompressorAudioProcessorEditor::handleParameterChange()
{
    // Verificación de thread solo en debug - no afecta release builds
    #if JUCE_DEBUG
    jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());
    #endif
    
    // No hacer nada si estamos cargando un preset
    if (isLoadingPreset) {
        return;
    }
    
    if (presetArea.presetMenu.getSelectedId() > 0) {
        // Un preset está seleccionado - deseleccionar y mostrar con asterisco
        juce::String currentPresetName = presetArea.presetMenu.getText();
        
        presetArea.presetMenu.setSelectedId(0);
        processor.setLastPreset(0);
        
        // Mostrar el nombre del preset con asterisco en cursiva
        auto modifiedText = currentPresetName + "*";
        presetArea.presetMenu.setTextWhenNothingSelected(modifiedText);
        presetArea.presetMenu.setTextItalic(true);
        
        // Guardar el estado visual en el processor
        processor.setPresetDisplayText(modifiedText);
        processor.setPresetTextItalic(true);
        
    } else {
        // No hay preset seleccionado - verificar texto actual
        juce::String currentText = presetArea.presetMenu.getTextWhenNothingSelected();
        
        if (currentText == "DEFAULT") {
            // DEFAULT ahora SÍ puede mostrarse como modificado
            // Esto permite detectar cuando graphics u otros elementos no-APVTS han cambiado
            presetArea.presetMenu.setTextWhenNothingSelected("DEFAULT*");
            presetArea.presetMenu.setTextItalic(true);
            
            // Guardar el estado visual en el processor
            processor.setPresetDisplayText("DEFAULT*");
            processor.setPresetTextItalic(true);
        }
        else if (currentText.isEmpty()) {
            // Si está vacío, no hacer nada
            presetArea.presetMenu.setTextWhenNothingSelected("");
            presetArea.presetMenu.setTextItalic(false);
            
            // Guardar el estado visual en el processor
            processor.setPresetDisplayText("");
            processor.setPresetTextItalic(false);
        }
        else if (!currentText.endsWith("*")) {
            // Si hay un preset cargado (no vacío, no DEFAULT y sin asterisco)
            // Añadir asterisco y poner en itálica
            presetArea.presetMenu.setTextWhenNothingSelected(currentText + "*");
            presetArea.presetMenu.setTextItalic(true);
            processor.setPresetDisplayText(currentText + "*");
            processor.setPresetTextItalic(true);
        }
        // Si ya tiene un asterisco, no hacer nada
    }
}

void JCBCompressorAudioProcessorEditor::updateARButtonText()
{
    // Actualizar texto del botón AR basado en el estado del toggle
    bool isOn = rightBottomKnobs.autorelButton.getToggleState();
    rightBottomKnobs.autorelButton.setButtonText(isOn ? "AR ON" : "AR OFF");
}

void JCBCompressorAudioProcessorEditor::updateRelSliderAlpha()
{
    // Actualizar alpha del slider REL basado en el estado del botón AR
    bool arIsOn = rightBottomKnobs.autorelButton.getToggleState();
    rightBottomKnobs.relSlider.setAlpha(arIsOn ? 0.3f : 1.0f);
}

//==============================================================================
// MÉTODOS DE SETUP Y CONFIGURACIÓN
//==============================================================================
void JCBCompressorAudioProcessorEditor::setupKnobs()
{
    // Establecer undo manager y editor para todos los sliders primero
    
    // === LEFT TOP KNOBS ===
    // THD
    leftTopKnobs.thdSlider.setComponentID("thd");
    leftTopKnobs.thdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftTopKnobs.thdSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    leftTopKnobs.thdSlider.setLookAndFeel(&sliderLAFBig);
    leftTopKnobs.thdSlider.setTextBoxIsEditable(true);
    leftTopKnobs.thdSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftTopKnobs.thdSlider.setDoubleClickReturnValue(true, -18.0);
    leftTopKnobs.thdSlider.setPopupDisplayEnabled(false, false, this);
    leftTopKnobs.thdSlider.setNumDecimalPlacesToDisplay(1);
    leftTopKnobs.thdSlider.setTextValueSuffix(" dB");
    // Configurar rango y sesgo para poner -18dB en el centro
    leftTopKnobs.thdSlider.setRange(-60.0, 0.0, 0.1);
    leftTopKnobs.thdSlider.setSkewFactorFromMidPoint(-18.0);  // -18dB en el centro
    addAndMakeVisible(leftTopKnobs.thdSlider);
    if (auto* param = processor.apvts.getParameter("b_THD"))
    {
        leftTopKnobs.thdAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftTopKnobs.thdSlider, &undoManager);
        leftTopKnobs.thdAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftTopKnobs.thdSlider.setTooltip(JUCE_UTF8("THD: umbral de activación entre -60 y 0 dB.\nControla el nivel donde el compresor comienza a actuar.\nValor por defecto: -18 dB"));
    
    // RATIO - coincidiendo con la implementación de ExpansorGate
    leftTopKnobs.ratioSlider.setComponentID("ratio");
    leftTopKnobs.ratioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftTopKnobs.ratioSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    leftTopKnobs.ratioSlider.setLookAndFeel(&sliderLAFBig);
    leftTopKnobs.ratioSlider.setTextBoxIsEditable(true);  // Ahora editable
    leftTopKnobs.ratioSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftTopKnobs.ratioSlider.setDoubleClickReturnValue(true, 4.0);
    leftTopKnobs.ratioSlider.setPopupDisplayEnabled(false, false, this);
    leftTopKnobs.ratioSlider.setNumDecimalPlacesToDisplay(1);
    // Formato de texto personalizado para coincidir con el parámetro
    leftTopKnobs.ratioSlider.textFromValueFunction = [](double value) {
        if (value >= 20.0)
            return juce::String("20 : 1");
        else
            return juce::String(value, 1) + " : 1";
    };
    // Configurar rango y sesgo para poner 4:1 en el centro
    leftTopKnobs.ratioSlider.setRange(1.0, 20.0, 0.01);
    leftTopKnobs.ratioSlider.setSkewFactorFromMidPoint(4.0);  // 4:1 en el centro
    addAndMakeVisible(leftTopKnobs.ratioSlider);
    if (auto* param = processor.apvts.getParameter("c_RATIO"))
    {
        leftTopKnobs.ratioAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftTopKnobs.ratioSlider, &undoManager);
        leftTopKnobs.ratioAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftTopKnobs.ratioSlider.setTooltip(JUCE_UTF8("RATIO: relación de compresión entre 1:1 a 20:1.\nDetermina cuánto se reduce la señal sobre el umbral.\nValor por defecto: 2:1"));

    // RODILLA
    leftTopKnobs.kneeSlider.setComponentID("knee");
    leftTopKnobs.kneeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftTopKnobs.kneeSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    leftTopKnobs.kneeSlider.setLookAndFeel(&sliderLAFBig);
    leftTopKnobs.kneeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftTopKnobs.kneeSlider.setTextValueSuffix(" dB");
    leftTopKnobs.kneeSlider.setDoubleClickReturnValue(true, 0.0);
    leftTopKnobs.kneeSlider.setPopupDisplayEnabled(false, false, this);
    leftTopKnobs.kneeSlider.setNumDecimalPlacesToDisplay(1);  // Solo 1 decimal
    addAndMakeVisible(leftTopKnobs.kneeSlider);
    if (auto* param = processor.apvts.getParameter("h_KNEE"))
    {
        leftTopKnobs.kneeAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftTopKnobs.kneeSlider, &undoManager);
        leftTopKnobs.kneeAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftTopKnobs.kneeSlider.setTooltip(JUCE_UTF8("KNEE: rodilla del compresor entre 0 y 20 dB.\nSuaviza la transición entre compresión y no compresión.\nValor por defecto: 0 dB (hard knee)"));

    // === PERILLAS INFERIORES IZQUIERDAS ===
    // D/W (Seco/Húmedo)
    leftBottomKnobs.drywetSlider.setComponentID("drywet");
    leftBottomKnobs.drywetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftBottomKnobs.drywetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    leftBottomKnobs.drywetSlider.setLookAndFeel(&sliderLAFBig);
    leftBottomKnobs.drywetSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftBottomKnobs.drywetSlider.setDoubleClickReturnValue(true, 1.0);
    leftBottomKnobs.drywetSlider.setPopupDisplayEnabled(false, false, this);
    // Visualización personalizada de valor para porcentaje
    leftBottomKnobs.drywetSlider.textFromValueFunction = [](double value) {
        return juce::String(static_cast<int>(value * 101)) + " %";
    };
    addAndMakeVisible(leftBottomKnobs.drywetSlider);
    if (auto* param = processor.apvts.getParameter("o_DRYWET"))
    {
        leftBottomKnobs.drywetAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftBottomKnobs.drywetSlider, &undoManager);
        leftBottomKnobs.drywetAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftBottomKnobs.drywetSlider.setTooltip(JUCE_UTF8("D/W: mezcla entre señal seca y procesada.\n0% = solo señal original, 100% = solo señal comprimida.\nValor por defecto: 100% (wet)"));
    
    // LA (Anticipación)
    leftBottomKnobs.lookaheadSlider.setComponentID("lookahead");
    leftBottomKnobs.lookaheadSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftBottomKnobs.lookaheadSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    leftBottomKnobs.lookaheadSlider.setLookAndFeel(&sliderLAFBig);
    leftBottomKnobs.lookaheadSlider.setTextBoxIsEditable(true);
    leftBottomKnobs.lookaheadSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftBottomKnobs.lookaheadSlider.setTextValueSuffix(" ms");
    leftBottomKnobs.lookaheadSlider.setRange(0.0, 10.0, 0.1);
    leftBottomKnobs.lookaheadSlider.setValue(0.0);
    leftBottomKnobs.lookaheadSlider.setDoubleClickReturnValue(true, 0.0);
    leftBottomKnobs.lookaheadSlider.setPopupDisplayEnabled(false, false, this);
    leftBottomKnobs.lookaheadSlider.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(leftBottomKnobs.lookaheadSlider);
    if (auto* param = processor.apvts.getParameter("n_LOOKAHEAD"))
    {
        leftBottomKnobs.lookaheadAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftBottomKnobs.lookaheadSlider, &undoManager);
        leftBottomKnobs.lookaheadAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftBottomKnobs.lookaheadSlider.setTooltip(JUCE_UTF8("LOOKAHEAD: anticipación entre 0 y 10 ms.\nEvita distorsión en transitorios rápidos.\nValor por defecto: 0 ms"));
    
    // CLIP (vinculado a u_SOFTCLIP)
    leftBottomKnobs.clipSlider.setComponentID("clipper");
    leftBottomKnobs.clipSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftBottomKnobs.clipSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    leftBottomKnobs.clipSlider.setLookAndFeel(&sliderLAFBig);
    leftBottomKnobs.clipSlider.setTextBoxIsEditable(true);
    leftBottomKnobs.clipSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    leftBottomKnobs.clipSlider.setValue(0.0);
    leftBottomKnobs.clipSlider.setDoubleClickReturnValue(true, 0.0);
    leftBottomKnobs.clipSlider.setPopupDisplayEnabled(false, false, this);
    // Formato de texto personalizado para CLIP: OFF a 100%
    leftBottomKnobs.clipSlider.textFromValueFunction = [](double value) {
        if (value < 0.01)
            return juce::String("OFF");
        else if (value > 0.99)
            return juce::String("100%");
        else
            return juce::String(static_cast<int>(value * 100)) + "%";
    };
    addAndMakeVisible(leftBottomKnobs.clipSlider);
    if (auto* param = processor.apvts.getParameter("u_SOFTCLIP"))
    {
        leftBottomKnobs.clipAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftBottomKnobs.clipSlider, &undoManager);
        leftBottomKnobs.clipAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftBottomKnobs.clipSlider.setTooltip(JUCE_UTF8("CLIP: soft clipping para limitar picos.\n0 = sin clipping, 1 = máximo clipping.\nValor por defecto: 0 (off)"));
    
    // AGAIN - nuevo knob
    leftTopKnobs.againSlider.setName("again");  // Para mostrar AGAIN dentro del knob
    leftTopKnobs.againSlider.setComponentID("again");
    leftTopKnobs.againSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftTopKnobs.againSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    leftTopKnobs.againSlider.setLookAndFeel(&sliderLAFBig);
    leftTopKnobs.againSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftTopKnobs.againSlider.setDoubleClickReturnValue(true, 0.0);
    leftTopKnobs.againSlider.setPopupDisplayEnabled(false, false, this);
    leftTopKnobs.againSlider.setTextBoxIsEditable(true);
    leftTopKnobs.againSlider.setNumDecimalPlacesToDisplay(2);  // Mostrar 2 decimales
    // Formato de texto personalizado para AGAIN: OFF a ~70%
    leftTopKnobs.againSlider.textFromValueFunction = [](double value) {
        if (value < 0.01)
            return juce::String("OFF");
        else if (value > 0.99)
            return juce::String(JUCE_UTF8("~70%"));
        else
            return juce::String(static_cast<int>(value * 100)) + "%";
    };
    addAndMakeVisible(leftTopKnobs.againSlider);
    if (auto* param = processor.apvts.getParameter("x_AUTOGAIN"))
    {
        leftTopKnobs.againAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftTopKnobs.againSlider, &undoManager);
        leftTopKnobs.againAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    leftTopKnobs.againSlider.setTooltip(JUCE_UTF8("AGAIN: compensación automática de ganancia.\nTrabaja junto con MAKEUP para mantener niveles.\nValor por defecto: 0 (OFF)"));
    
    // === PERILLAS INFERIORES DERECHAS ===
    // ATK - copiado exactamente de ExpansorGate
    rightBottomKnobs.atkSlider.setComponentID("attack");
    rightBottomKnobs.atkSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightBottomKnobs.atkSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    rightBottomKnobs.atkSlider.setLookAndFeel(&sliderLAFBig);
    rightBottomKnobs.atkSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rightBottomKnobs.atkSlider.setDoubleClickReturnValue(true, 5.0);  // Valor por defecto 5ms
    rightBottomKnobs.atkSlider.setPopupDisplayEnabled(false, false, this);
    rightBottomKnobs.atkSlider.setTextBoxIsEditable(true);
    // Custom text formatting con decimales progresivos
    rightBottomKnobs.atkSlider.textFromValueFunction = [](double value) {
        if (value < 1.0)
            return juce::String(value, 3) + " ms";  // 3 decimales: 0.001
        else if (value < 10.0)
            return juce::String(value, 2) + " ms";  // 2 decimales: 1.50
        else if (value < 100.0)
            return juce::String(value, 1) + " ms";  // 1 decimal: 10.5
        else
            return juce::String(value, 1) + " ms";  // 1 decimal: 100.5
    };
    // Configurar rango con compensación ligera para mejor balance visual
    rightBottomKnobs.atkSlider.setRange(0.1, 250.0, 0.01);
    rightBottomKnobs.atkSlider.setSkewFactorFromMidPoint(20.0);  // Compensación ligera para balance visual
    addAndMakeVisible(rightBottomKnobs.atkSlider);
    if (auto* param = processor.apvts.getParameter("d_ATK"))
    {
        rightBottomKnobs.atkAttachment = std::make_unique<CustomSliderAttachment>(
            *param, rightBottomKnobs.atkSlider, &undoManager);
        rightBottomKnobs.atkAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    rightBottomKnobs.atkSlider.setTooltip(JUCE_UTF8("ATK: tiempo de ataque entre 0.1 y 250 ms.\nControla la respuesta ante aumentos de nivel.\nValor por defecto: 5 ms"));
    
    // REL - modificado con rango mínimo de 1ms y formato de decimales progresivo
    rightBottomKnobs.relSlider.setComponentID("release");
    rightBottomKnobs.relSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightBottomKnobs.relSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    rightBottomKnobs.relSlider.setLookAndFeel(&sliderLAFBig);
    rightBottomKnobs.relSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rightBottomKnobs.relSlider.setRange(0.1, 1000.0, 0.01);  // Coherente con parámetro
    rightBottomKnobs.relSlider.setSkewFactorFromMidPoint(30.0);  // Centrar rango operativo 1-50ms
    rightBottomKnobs.relSlider.setDoubleClickReturnValue(true, 30.0);  // Valor por defecto 30ms
    rightBottomKnobs.relSlider.setPopupDisplayEnabled(false, false, this);
    rightBottomKnobs.relSlider.setTextBoxIsEditable(true);
    // Custom text formatting con decimales progresivos - coherente con parámetro
    rightBottomKnobs.relSlider.textFromValueFunction = [](double value) {
        if (value < 1.0)
            return juce::String(value, 2) + " ms";  // 2 decimales: 0.10
        else if (value < 10.0)
            return juce::String(value, 1) + " ms";  // 1 decimal: 1.5
        else if (value < 100.0)
            return juce::String(value, 1) + " ms";  // 1 decimal: 10.5
        else
            return juce::String(value, 0) + " ms";  // Sin decimales >= 100ms
    };
    addAndMakeVisible(rightBottomKnobs.relSlider);
    if (auto* param = processor.apvts.getParameter("e_REL"))
    {
        rightBottomKnobs.relAttachment = std::make_unique<CustomSliderAttachment>(
            *param, rightBottomKnobs.relSlider, &undoManager);
        rightBottomKnobs.relAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }

    // AR - botón con texto dinámico AR OFF/AR ON
    rightBottomKnobs.autorelButton.setButtonText("AR OFF");
    rightBottomKnobs.autorelButton.setClickingTogglesState(true);
    rightBottomKnobs.autorelButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    rightBottomKnobs.autorelButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::deltaLow.withAlpha(0.7f)); // Acento turquesa/verde azulado cuando está ON
    rightBottomKnobs.autorelButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.4f));
    rightBottomKnobs.autorelButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    rightBottomKnobs.autorelButton.setLookAndFeel(&smallButtonLAF); // Aplicar LookAndFeel para fuente pequeña
    rightBottomKnobs.autorelButton.addListener(this);
    addAndMakeVisible(rightBottomKnobs.autorelButton);
    rightBottomKnobs.autorelAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("s_AUTORELEASE"), rightBottomKnobs.autorelButton, &undoManager);
    rightBottomKnobs.autorelAttachment->onParameterChange = [this]() { 
        handleParameterChange(); 
        
        // Actualizar transfer display inmediatamente - ESENCIAL para comportamiento DSP
        
        // Actualizar texto del botón dynamically
        updateARButtonText();
    };
    
    // DELTA - botón con texto "DELTA" (estilo SC)
    parameterButtons.deltaButton.setButtonText("DELTA");
    parameterButtons.deltaButton.setClickingTogglesState(true);
    parameterButtons.deltaButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    parameterButtons.deltaButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0, 180, 140).withAlpha(0.3f)); // Verde teal como los medidores delta
    parameterButtons.deltaButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    parameterButtons.deltaButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    parameterButtons.deltaButton.addListener(this);
    addAndMakeVisible(parameterButtons.deltaButton);
    // deltaAttachment movido a parameterButtons - manejado en setupParameterButtons()
    
    // Callback de DELTA movido al método buttonClicked()
    
    // PARALL - nuevo knob con texto "PARALL" dentro
    leftBottomKnobs.parallSlider.setName("parall");  // Para mostrar PARALL dentro del knob
    leftBottomKnobs.parallSlider.setComponentID("parall");
    leftBottomKnobs.parallSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftBottomKnobs.parallSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    leftBottomKnobs.parallSlider.setLookAndFeel(&sliderLAFBig);
    leftBottomKnobs.parallSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    leftBottomKnobs.parallSlider.setRange(0.0, 1.0, 0.01);  // Continuo de 0 a 1
    leftBottomKnobs.parallSlider.setValue(0.0);
    leftBottomKnobs.parallSlider.setDoubleClickReturnValue(true, 0.0);
    leftBottomKnobs.parallSlider.setPopupDisplayEnabled(false, false, this);
    leftBottomKnobs.parallSlider.setTextBoxIsEditable(true);  // Ahora editable
    // Formato de texto personalizado para PARAL: COMP a COMP+DRY
    leftBottomKnobs.parallSlider.textFromValueFunction = [](double value) {
        if (value < 0.01)
            return juce::String("COMP");
        else if (value > 0.99)
            return juce::String("COMP+DRY");
        else
            return juce::String(static_cast<int>(value * 100)) + "%";
    };
    addAndMakeVisible(leftBottomKnobs.parallSlider);
    if (auto* param = processor.apvts.getParameter("w_PARALLEL"))
    {
        leftBottomKnobs.parallAttachment = std::make_unique<CustomSliderAttachment>(
            *param, leftBottomKnobs.parallSlider, &undoManager);
        leftBottomKnobs.parallAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    
    // SMOOTH - knob con mismo estilo que los demás
    rightTopControls.smoothSlider.setName("smo");  // Para mostrar SMO dentro del knob
    rightTopControls.smoothSlider.setComponentID("smo");
    rightTopControls.smoothSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightTopControls.smoothSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    rightTopControls.smoothSlider.setLookAndFeel(&sliderLAFBig);  // Usar LAF grande como los demás
    rightTopControls.smoothSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rightTopControls.smoothSlider.setRange(0.0, 1.0, 0.01);
    // No llamar setValue() - el SliderAttachment manejará el valor inicial
    rightTopControls.smoothSlider.setDoubleClickReturnValue(true, 0.0);
    rightTopControls.smoothSlider.setPopupDisplayEnabled(false, false, this);
    rightTopControls.smoothSlider.setTextBoxIsEditable(true);
    addAndMakeVisible(rightTopControls.smoothSlider);
    if (auto* param = processor.apvts.getParameter("z_SMOOTH"))
    {
        rightTopControls.smoothAttachment = std::make_unique<CustomSliderAttachment>(
            *param, rightTopControls.smoothSlider, &undoManager);
        rightTopControls.smoothAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    // Visualización de texto para SMOOTH - RAW en mínimo, SMOOTH en máximo
    // IMPORTANTE: Asignar textFromValueFunction DESPUÉS de crear el SliderAttachment
    rightTopControls.smoothSlider.textFromValueFunction = [](double value) {
        if (value < 0.01)
            return juce::String("RAW");
        else if (value > 0.99)
            return juce::String("SMOOTH");
        else
            return juce::String(value, 2);
    };
    // El tooltip se establecerá en updateAllTooltips()
    
    // === CONTROLES SUPERIORES DERECHOS (SLIDERS) ===
    // ALGO
    rightTopControls.algoSlider.setComponentID("detect");
    rightTopControls.algoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightTopControls.algoSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    rightTopControls.algoSlider.setLookAndFeel(&sliderLAFBig);
    rightTopControls.algoSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rightTopControls.algoSlider.setRange(0.0, 2.0, 0.01);
    rightTopControls.algoSlider.setDoubleClickReturnValue(true, 1.0);
    rightTopControls.algoSlider.setPopupDisplayEnabled(false, false, this);
    // Formato de texto personalizado para ALGO: SHARP a SLOW RMS con rangos precisos
    rightTopControls.algoSlider.textFromValueFunction = [](double value) {
        if (value <= 0.005)        // Exactamente 0.0
            return juce::String("SHARP");
        else if (value >= 1.995)   // Exactamente 2.0
            return juce::String("SLOW RMS");
        else if (value >= 0.995 && value <= 1.005)  // Exactamente 1.0
            return juce::String("EXPO RMS");
        else
            return juce::String(value, 2);  // Mostrar 2 decimales para valores intermedios
    };
    addAndMakeVisible(rightTopControls.algoSlider);
    if (auto* param = processor.apvts.getParameter("f_ALGO"))
    {
        rightTopControls.algoAttachment = std::make_unique<CustomSliderAttachment>(
            *param, rightTopControls.algoSlider, &undoManager);
        rightTopControls.algoAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    // El tooltip se establecerá en updateAllTooltips()
    
    // REACT
    rightTopControls.reactSlider.setComponentID("react");
    rightTopControls.reactSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightTopControls.reactSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 16);
    rightTopControls.reactSlider.setLookAndFeel(&sliderLAFBig);
    rightTopControls.reactSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rightTopControls.reactSlider.setRange(0.0, 1.0, 0.01);
    rightTopControls.reactSlider.setDoubleClickReturnValue(true, 0.0);
    rightTopControls.reactSlider.setPopupDisplayEnabled(false, false, this);
    // Formato de texto personalizado para REACT: PEAK a RMS
    rightTopControls.reactSlider.textFromValueFunction = [](double value) {
        if (value < 0.01)
            return juce::String("PEAK");
        else if (value > 0.99)
            return juce::String("RMS");
        else
            return juce::String(value, 2);
    };
    addAndMakeVisible(rightTopControls.reactSlider);
    if (auto* param = processor.apvts.getParameter("g_REACT"))
    {
        rightTopControls.reactAttachment = std::make_unique<CustomSliderAttachment>(
            *param, rightTopControls.reactSlider, &undoManager);
        rightTopControls.reactAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    
    // Conectar perillas al transfer display
    leftTopKnobs.thdSlider.onValueChange = [this]() {
        // THD ahora está en rango de dB (-60 a 0)
        float thresholdDB = static_cast<float>(leftTopKnobs.thdSlider.getValue());
        transferDisplay.setThreshold(thresholdDB);
        updateTransferDisplay();
    };
    leftTopKnobs.ratioSlider.onValueChange = [this]() {
        transferDisplay.setRatio(static_cast<float>(leftTopKnobs.ratioSlider.getValue()));
        updateTransferDisplay();
    };
    leftTopKnobs.kneeSlider.onValueChange = [this]() {
        transferDisplay.setKnee(static_cast<float>(leftTopKnobs.kneeSlider.getValue()));
        updateTransferDisplay();
    };
    
    
}

void JCBCompressorAudioProcessorEditor::setupMeters()
{
    // Medidores de entrada
    addAndMakeVisible(inputMeterL);
    addAndMakeVisible(inputMeterR);
    // Medidor GR
    addAndMakeVisible(grMeter);
    
    // Medidores de salida
    addAndMakeVisible(outputMeterL);
    addAndMakeVisible(outputMeterR);
    
    // Medidores de sidechain (siempre visibles)
    scMeterL.setVisible(true);
    scMeterR.setVisible(true);
    addAndMakeVisible(scMeterL);
    addAndMakeVisible(scMeterR);
    
    // Slider de trim
    trimSlider.setComponentID("trim");
    addAndMakeVisible(trimSlider);
    
    // Vincular slider de trim al parámetro - ahora usando attachment thread-safe
    if (auto* param = processor.apvts.getParameter("a_TRIM"))
    {
        trimAttachment = std::make_unique<CustomSliderAttachment>(
            *param, trimSlider, &undoManager);
        trimAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    
    
    // Slider de makeup
    makeupSlider.setComponentID("makeup");
    addAndMakeVisible(makeupSlider);
    
    // Vincular slider de makeup al parámetro - ahora usando attachment thread-safe
    if (auto* param = processor.apvts.getParameter("i_MAKEUP"))
    {
        makeupAttachment = std::make_unique<CustomSliderAttachment>(
            *param, makeupSlider, &undoManager);
        makeupAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }

    // Slider de trim de sidechain
    scTrimSlider.setComponentID("sctrim");
    addAndMakeVisible(scTrimSlider);
    
    // Establecer propiedades iniciales para el slider de trim de sidechain
    scTrimSlider.setTooltip(JUCE_UTF8("SC TRIM: ganancia de entrada del sidechain entre -12 y +12 dB.\nAjusta el nivel del sidechain externo.\nValor por defecto: 0 dB, se activa con EXT KEY"));
    
    // Vincular slider de trim de sidechain al parámetro y_SCTRIM - ahora usando attachment thread-safe
    if (auto* param = processor.apvts.getParameter("y_SCTRIM"))
    {
        scTrimAttachment = std::make_unique<CustomSliderAttachment>(
            *param, scTrimSlider, &undoManager);
        scTrimAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    
    // Inicialmente visible pero con alpha reducido cuando EXT KEY está apagado
    scTrimSlider.setVisible(true);
    scTrimSlider.setEnabled(false);
    scTrimSlider.setAlpha(0.25f);
    
    //==========================================================================
    // CONFIGURAR DISPLAYS INDEPENDIENTES
    //==========================================================================
    
}

void JCBCompressorAudioProcessorEditor::setupSidechainControls()
{
    // Establecer undo manager y editor para sliders de sidechain
    
    // Slider HPF - copia EXACTA de ExpansorGate
    sidechainControls.hpfSlider.setName("hpf");
    sidechainControls.hpfSlider.setComponentID("hpf");
    sidechainControls.hpfSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sidechainControls.hpfSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 16);
    sidechainControls.hpfSlider.setLookAndFeel(&sliderLAFBig);  // Usar LAF grande como ExpansorGate
    sidechainControls.hpfSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);  // Blanco fijo
    sidechainControls.hpfSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);  // Blanco fijo
    sidechainControls.hpfSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    sidechainControls.hpfSlider.setTextBoxIsEditable(true);
    sidechainControls.hpfSlider.setEnabled(false);  // Inicialmente deshabilitado
    sidechainControls.hpfSlider.setAlpha(0.0f);  // Inicialmente invisible
    sidechainControls.hpfSlider.setDoubleClickReturnValue(true, 20.0f);
    sidechainControls.hpfSlider.setPopupDisplayEnabled(false, false, this);
    sidechainControls.hpfSlider.setNumDecimalPlacesToDisplay(0);
    // Formato de texto personalizado para frecuencia
    sidechainControls.hpfSlider.textFromValueFunction = [](double value) {
        if (value < 1000.0)
            return juce::String(static_cast<int>(value));
        else
            return juce::String(value / 1000.0, 1) + "k";
    };
    sidechainControls.hpfSlider.setTextValueSuffix(" Hz");
    // Configurar rango y sesgo para poner 1kHz en el centro
    sidechainControls.hpfSlider.setRange(20.0, 20000.0, 1.0);
    sidechainControls.hpfSlider.setSkewFactorFromMidPoint(1000.0);  // 1kHz en el centro
    addAndMakeVisible(sidechainControls.hpfSlider);
    if (auto* param = processor.apvts.getParameter("j_HPF"))
    {
        sidechainControls.hpfAttachment = std::make_unique<CustomSliderAttachment>(
            *param, sidechainControls.hpfSlider, &undoManager);
        sidechainControls.hpfAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }
    
    // Slider LPF - copia EXACTA de ExpansorGate
    sidechainControls.lpfSlider.setName("lpf");
    sidechainControls.lpfSlider.setComponentID("lpf");
    sidechainControls.lpfSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sidechainControls.lpfSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 16);
    sidechainControls.lpfSlider.setLookAndFeel(&sliderLAFBig);  // Usar LAF grande como ExpansorGate
    sidechainControls.lpfSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);  // Blanco fijo
    sidechainControls.lpfSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);  // Blanco fijo
    sidechainControls.lpfSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    sidechainControls.lpfSlider.setTextBoxIsEditable(true);
    sidechainControls.lpfSlider.setEnabled(false);  // Inicialmente deshabilitado
    sidechainControls.lpfSlider.setAlpha(0.0f);  // Inicialmente invisible
    sidechainControls.lpfSlider.setDoubleClickReturnValue(true, 20000.0f);
    sidechainControls.lpfSlider.setPopupDisplayEnabled(false, false, this);
    sidechainControls.lpfSlider.setNumDecimalPlacesToDisplay(0);
    // Formato de texto personalizado para frecuencia
    sidechainControls.lpfSlider.textFromValueFunction = [](double value) {
        if (value < 1000.0)
            return juce::String(static_cast<int>(value));
        else
            return juce::String(value / 1000.0, 1) + "k";
    };
    sidechainControls.lpfSlider.setTextValueSuffix(" Hz");
    // Configurar rango y sesgo para poner 1kHz en el centro
    sidechainControls.lpfSlider.setRange(20.0, 20000.0, 1.0);
    sidechainControls.lpfSlider.setSkewFactorFromMidPoint(1000.0);  // 1kHz en el centro
    addAndMakeVisible(sidechainControls.lpfSlider);
    if (auto* param = processor.apvts.getParameter("k_LPF"))
    {
        sidechainControls.lpfAttachment = std::make_unique<CustomSliderAttachment>(
            *param, sidechainControls.lpfSlider, &undoManager);
        sidechainControls.lpfAttachment->onParameterChange = [this]() { handleParameterChange(); };
    }

    // Botón FILTERS (antes SC) - VISIBLE para activar/desactivar filtros
    sidechainControls.scButton.setClickingTogglesState(true);
    sidechainControls.scButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    sidechainControls.scButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkviolet.withAlpha(0.3f));
    sidechainControls.scButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    sidechainControls.scButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    sidechainControls.scButton.addListener(this);
    addAndMakeVisible(sidechainControls.scButton);
    sidechainControls.scAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("l_SC"), sidechainControls.scButton, &undoManager);
    sidechainControls.scAttachment->onParameterChange = [this]() { handleParameterChange(); };
    sidechainControls.scButton.setTooltip(JUCE_UTF8("FILTERS: activa/desactiva los filtros del sidechain.\nPermite filtrar la señal que controla el compresor.\nValor por defecto: OFF"));
    
    // Botón EXT KEY - estilizado como ExpansorGate, SIEMPRE HABILITADO
    sidechainControls.keyButton.setClickingTogglesState(true);
    sidechainControls.keyButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    sidechainControls.keyButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF0097A7).withAlpha(0.3f));  // Teal oscuro distintivo
    sidechainControls.keyButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    sidechainControls.keyButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    sidechainControls.keyButton.setEnabled(true);  // SIEMPRE HABILITADO
    sidechainControls.keyButton.addListener(this);
    addAndMakeVisible(sidechainControls.keyButton);
    
    // Configurar texto inicial basado en el estado
    bool initialKeyState = processor.apvts.getRawParameterValue("r_KEY")->load() > 0.5f;
    sidechainControls.keyButton.setButtonText(initialKeyState ? "SC EXT" : "SC INT");
    
    sidechainControls.keyAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("r_KEY"), sidechainControls.keyButton, &undoManager);
    sidechainControls.keyAttachment->onParameterChange = [this]() { 
        // Actualizar texto del botón cuando cambia el parámetro
        bool keyActive = sidechainControls.keyButton.getToggleState();
        sidechainControls.keyButton.setButtonText(keyActive ? "SC EXT" : "SC INT");
        handleParameterChange(); 
    };
    sidechainControls.keyButton.setTooltip(JUCE_UTF8("SC INT/EXT: selecciona la fuente del sidechain.\nSC INT: usa la señal interna\nSC EXT: usa entrada externa (canales 3-4)\nValor por defecto: SC INT"));
    
    // Botón Solo SC - estilizado como ExpansorGate con acento cálido, SIEMPRE HABILITADO
    sidechainControls.soloScButton.setClickingTogglesState(true);
    sidechainControls.soloScButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    sidechainControls.soloScButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accentWarm.withAlpha(0.3f));
    sidechainControls.soloScButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    sidechainControls.soloScButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    sidechainControls.soloScButton.setEnabled(true); // SIEMPRE HABILITADO
    sidechainControls.soloScButton.addListener(this);
    sidechainControls.soloScButton.onStateChange = [this]() {
        updateButtonStates();
        // Actualizar estado SOLO SIDECHAIN del transfer display
        transferDisplay.setSoloSidechain(sidechainControls.soloScButton.getToggleState());
    };
    addAndMakeVisible(sidechainControls.soloScButton);
    // Attachment de SOLO SC - excluido del undo por consistencia con DELTA y BYPASS (sin UndoManager)
    sidechainControls.soloScAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("m_SOLOSC"), sidechainControls.soloScButton, nullptr);
    sidechainControls.soloScAttachment->onParameterChange = [this]() { handleParameterChange(); };
    sidechainControls.soloScButton.setTooltip(JUCE_UTF8("SOLO SC: escucha solo la señal del sidechain.\nPermite monitorear qué está controlando el compresor.\nSe procesa en dual mono"));
    
    // Botón de orden HPF - control de orden del filtro
    sidechainControls.hpfOrderButton.setClickingTogglesState(true);
    sidechainControls.hpfOrderButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    sidechainControls.hpfOrderButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    sidechainControls.hpfOrderButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    sidechainControls.hpfOrderButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    sidechainControls.hpfOrderButton.setEnabled(false);  // Inicialmente deshabilitado
    sidechainControls.hpfOrderButton.setAlpha(0.0f);  // Inicialmente invisible
    sidechainControls.hpfOrderButton.addListener(this);
    addAndMakeVisible(sidechainControls.hpfOrderButton);
    sidechainControls.hpfOrderAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("j_HPFORDER"), sidechainControls.hpfOrderButton, &undoManager);
    sidechainControls.hpfOrderAttachment->onParameterChange = [this]() { handleParameterChange(); };
    sidechainControls.hpfOrderButton.setTooltip(JUCE_UTF8("HPF ORDER: orden del filtro paso alto.\n12 dB/oct (2º orden) o 24 dB/oct (4º orden).\nValor por defecto: 12 dB/oct"));
    
    // Botón de orden LPF - control de orden del filtro
    sidechainControls.lpfOrderButton.setClickingTogglesState(true);
    sidechainControls.lpfOrderButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    sidechainControls.lpfOrderButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    sidechainControls.lpfOrderButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    sidechainControls.lpfOrderButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    sidechainControls.lpfOrderButton.setEnabled(false);  // Inicialmente deshabilitado
    sidechainControls.lpfOrderButton.setAlpha(0.0f);  // Inicialmente invisible
    sidechainControls.lpfOrderButton.addListener(this);
    addAndMakeVisible(sidechainControls.lpfOrderButton);
    sidechainControls.lpfOrderAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("k_LPFORDER"), sidechainControls.lpfOrderButton, &undoManager);
    sidechainControls.lpfOrderAttachment->onParameterChange = [this]() { handleParameterChange(); };
    sidechainControls.lpfOrderButton.setTooltip(JUCE_UTF8("LPF ORDER: orden del filtro paso bajo.\n12 dB/oct (2º orden) o 24 dB/oct (4º orden).\nValor por defecto: 12 dB/oct"));
}

void JCBCompressorAudioProcessorEditor::setupPresetArea()
{
    // Botón Save - estilo transparente
    presetArea.saveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetArea.saveButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    presetArea.saveButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    presetArea.saveButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    presetArea.saveButton.addListener(this);
    addAndMakeVisible(presetArea.saveButton);
    presetArea.saveButton.setTooltip(JUCE_UTF8("SAVE: guarda el preset actual.\nSobrescribe el preset seleccionado con valores actuales.\nNo funciona con DEFAULT"));
    
    // Botón Save As - estilo transparente
    presetArea.saveAsButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetArea.saveAsButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    presetArea.saveAsButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    presetArea.saveAsButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    presetArea.saveAsButton.addListener(this);
    addAndMakeVisible(presetArea.saveAsButton);
    presetArea.saveAsButton.setTooltip(JUCE_UTF8("SAVE AS: guarda como nuevo preset.\nCrea un nuevo archivo de preset con los valores actuales.\nPermite crear presets personalizados"));
    
    // Botón Delete - estilo transparente
    presetArea.deleteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetArea.deleteButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    presetArea.deleteButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    presetArea.deleteButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    presetArea.deleteButton.addListener(this);
    addAndMakeVisible(presetArea.deleteButton);
    presetArea.deleteButton.setTooltip(JUCE_UTF8("DELETE: elimina el preset seleccionado.\nBorra permanentemente el archivo de preset.\nRequiere confirmación"));
    
    // Botón Back - estilo transparente
    presetArea.backButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetArea.backButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    presetArea.backButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    presetArea.backButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    presetArea.backButton.addListener(this);
    addAndMakeVisible(presetArea.backButton);
    presetArea.backButton.setTooltip(JUCE_UTF8("BACK: selecciona el preset anterior.\nNavega al preset previo en la lista.\nCicla al final si está al principio"));
    
    // Botón Next - estilo transparente
    presetArea.nextButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetArea.nextButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    presetArea.nextButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    presetArea.nextButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    presetArea.nextButton.addListener(this);
    addAndMakeVisible(presetArea.nextButton);
    presetArea.nextButton.setTooltip(JUCE_UTF8("NEXT: selecciona el siguiente preset.\nNavega al siguiente preset en la lista.\nCicla al principio si está al final"));
    
    // Menú de preset - usando CustomComboBox como en JCBExpansorGate
    presetArea.presetMenu.setJustificationType(juce::Justification::centred);
    presetArea.presetMenu.setTextWhenNothingSelected("");
    presetArea.presetMenu.setTextWhenNoChoicesAvailable("No presets");
    
    // Configurar onChange para cargar presets
    presetArea.presetMenu.onChange = [this]() {
        int selectedId = presetArea.presetMenu.getSelectedId();
        if (selectedId == 0) return;
        
        // Salir de DELTA al seleccionar un preset
        exitDeltaMode();
        
        // CRÍTICO: Establecer flag de carga PRIMERO para prevenir transacciones de undo
        isLoadingPreset = true;
        
        // Guard de scope para asegurar que el flag siempre se resetea
        struct LoadingGuard {
            bool& flag;
            LoadingGuard(bool& f) : flag(f) { flag = true; }
            ~LoadingGuard() { flag = false; }
        } guard(isLoadingPreset);
        
        // NOTA: El historial de undo se borrará al final para evitar grabar cambios de parámetros
        
        // Buscar el nombre real del preset usando el mapeo
        juce::String presetName;
        if (presetIdToNameMap.find(selectedId) != presetIdToNameMap.end()) {
            presetName = presetIdToNameMap[selectedId];
        } else {
            // Si no está en el mapeo, intentar obtenerlo del texto del menú (fallback)
            presetName = presetArea.presetMenu.getText();
            if (presetName.isEmpty()) {
                presetArea.presetMenu.setSelectedId(0);
                return;
            }
        }
        
        // Ignorar separadores
        if (presetName.startsWith("---")) {
            presetArea.presetMenu.setSelectedId(0);
            return;
        }
        
        if (presetName == "DEFAULT") {
            // Preset DEFAULT: Resetear todos los parámetros a sus valores por defecto definidos
            // Usar acceso directo a parámetros para actualizaciones inmediatas y confiables
            
            // Verificar el estado actual del preset
            juce::String previousPresetText = processor.getPresetDisplayText();
            // Si estamos en "DEFAULT" sin asterisco, no hacer nada con graphics
            // Si estamos en "DEFAULT*" o cualquier otro preset, activar graphics
            bool shouldActivateGraphics = (previousPresetText != "DEFAULT");
            
            // Deshabilitar undo durante la carga de preset
            isLoadingPreset = true;
            
            // Establecer todos los parámetros a sus valores por defecto usando gesture mechanism
            // Esto replica el mismo mecanismo usado por CustomSliderAttachment (doble-click)
            if (auto* param = processor.apvts.getParameter("a_TRIM")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                trimSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("b_THD")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftTopKnobs.thdSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("c_RATIO")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftTopKnobs.ratioSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("d_ATK")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                rightBottomKnobs.atkSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("e_REL")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                rightBottomKnobs.relSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("f_ALGO")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                rightTopControls.algoSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("g_REACT")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                rightTopControls.reactSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("h_KNEE")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftTopKnobs.kneeSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("i_MAKEUP")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                makeupSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("j_HPF")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                sidechainControls.hpfSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("k_LPF")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                sidechainControls.lpfSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("l_SC")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                sidechainControls.scButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("m_SOLOSC")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                sidechainControls.soloScButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("n_LOOKAHEAD")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftBottomKnobs.lookaheadSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("o_DRYWET")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftBottomKnobs.drywetSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("p_BYPASS")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                parameterButtons.bypassButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("r_KEY")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                sidechainControls.keyButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (dynamic_cast<juce::AudioParameterBool*>(processor.apvts.getParameter("s_AUTORELEASE"))) {
                bool defaultValue = false;  // Valor por defecto para s_AUTORELEASE
                rightBottomKnobs.autorelButton.setToggleState(defaultValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("u_SOFTCLIP")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftBottomKnobs.clipSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("v_DELTA")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                parameterButtons.deltaButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("w_PARALLEL")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftBottomKnobs.parallSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("x_AUTOGAIN")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                leftTopKnobs.againSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("y_SCTRIM")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                scTrimSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("z_SMOOTH")) {
                float defaultValue = param->getDefaultValue();
                float realValue = param->getNormalisableRange().convertFrom0to1(defaultValue);
                rightTopControls.smoothSlider.setValue(realValue, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("j_HPFORDER")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                sidechainControls.hpfOrderButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            if (auto* param = processor.apvts.getParameter("k_LPFORDER")) {
                float defaultValue = param->getDefaultValue();
                bool toggleState = defaultValue >= 0.5f;
                sidechainControls.lpfOrderButton.setToggleState(toggleState, juce::sendNotificationSync);
            }
            
            // Activar graphics si venimos de cualquier preset modificado o diferente
            if (shouldActivateGraphics) {
                // Establecer el botón graphics en ON para el preset DEFAULT
                utilityButtons.runGraphicsButton.setToggleState(true, juce::dontSendNotification);
                processor.displayGraphicsEnvelopes = true;
                
                // Graphics ON = envolventes VISIBLES
                transferDisplay.setEnvelopeVisible(true); // VISIBLE
                processor.setEnvelopeVisualEnabled(true); // VISIBLE
                
                // grMeter visible cuando graphics está ON (y SOLO SC no está activo)
                bool soloScActive = sidechainControls.soloScButton.getToggleState();
                grMeter.setVisible(!soloScActive); // Visible si SOLO SC no está activo
            }
            // Si ya estábamos en DEFAULT sin modificar, mantener el estado actual del botón graphics
            
            // Reactivar undo después carga de preset
            isLoadingPreset = false;
            
            // IMPORTANTE: Forzar la sincronización directa de Gen~ para asegurar valores correctos de los parámetros
            // Esto replica la misma sincronización realizada durante la instanciación del plugin
            for (int i = 0; i < JCBCompressor::num_params(); i++) {
                const char* name = JCBCompressor::getparametername(processor.getPluginState(), i);
                auto paramName = juce::String(name ? name : "");
                if (auto* param = processor.apvts.getRawParameterValue(paramName)) {
                    float value = param->load();
                    
                    // Aplicar la misma validación que en parameterChanged()
                    if (paramName == "d_ATK" && value < 0.1f) {
                        value = 0.1f;
                    }
                    if (paramName == "e_REL" && value < 0.1f) {
                        value = 0.1f;
                    }
                    
                    JCBCompressor::setparameter(processor.getPluginState(), i, value, nullptr);
                }
            }
        } 
        else if (presetName.startsWith("Guitars_") || presetName.startsWith("Bass_") || 
                 presetName.startsWith("Drums_") || presetName.startsWith("Vocals_")) {
            // Es un factory preset con prefijo de categoría - cargar desde BinaryData
            juce::String resourceName = presetName + "_preset";
            
            // Buscar el recurso en BinaryData
            for (int i = 0; i < BinaryData::namedResourceListSize; ++i) {
                if (resourceName == BinaryData::namedResourceList[i]) {
                    // Cargar el preset desde BinaryData
                    int dataSize = 0;
                    const char* data = BinaryData::getNamedResource(BinaryData::namedResourceList[i], dataSize);
                    
                    if (data != nullptr && dataSize > 0) {
                        // Parsear el XML desde memoria
                        juce::String xmlContent(data, dataSize);
                        juce::XmlDocument xmlDoc(xmlContent);
                        std::unique_ptr<juce::XmlElement> xmlState(xmlDoc.getDocumentElement());
                        
                        if (xmlState != nullptr && xmlState->hasTagName(processor.apvts.state.getType())) {
                            auto stateTree = juce::ValueTree::fromXml(*xmlState);
                            processor.apvts.replaceState(stateTree);
                            
                            // Restaurar estado del botón graphics si está presente
                            auto uiSettings = stateTree.getChildWithName("UISettings");
                            if (uiSettings.isValid()) {
                                bool showGraphics = uiSettings.getProperty("displayGraphicsEnvelopes", true);
                                utilityButtons.runGraphicsButton.setToggleState(showGraphics, juce::dontSendNotification);
                                processor.displayGraphicsEnvelopes = showGraphics;
                                
                                // Actualizar componentes visuales (graphics ON = envelopes VISIBLES)
                                transferDisplay.setEnvelopeVisible(showGraphics); // Directo, sin inversión
                                processor.setEnvelopeVisualEnabled(showGraphics);
                                // Actualizar visibilidad del grMeter considerando SOLO SC
                                bool soloScActive = sidechainControls.soloScButton.getToggleState();
                                grMeter.setVisible(showGraphics && !soloScActive);
                            } else {
                                // Si no hay UISettings guardado, mantener el estado actual
                                // No cambiar el estado del botón graphics
                                processor.displayGraphicsEnvelopes = utilityButtons.runGraphicsButton.getToggleState();
                            }
                            
                            // Queue las actualizaciones de parámetros para botones momentáneos
                            // Los parámetros ya se actualizan directamente desde los valores cargados
                        }
                    }
                    break;
                }
            }
        }
        else {
            // Es un user preset - cargar desde archivo
            juce::File presetFile = getPresetsFolder().getChildFile(presetName + ".preset");
            
            if (presetFile.existsAsFile()) {
                juce::XmlDocument xmlDoc(presetFile);
                std::unique_ptr<juce::XmlElement> xmlState(xmlDoc.getDocumentElement());
                
                if (xmlState != nullptr && xmlState->hasTagName(processor.apvts.state.getType())) {
                    auto stateTree = juce::ValueTree::fromXml(*xmlState);
                    processor.apvts.replaceState(stateTree);
                    
                    // Restaurar estado del botón graphics si está presente
                    auto uiSettings = stateTree.getChildWithName("UISettings");
                    if (uiSettings.isValid()) {
                        bool showGraphics = uiSettings.getProperty("displayGraphicsEnvelopes", true);
                        utilityButtons.runGraphicsButton.setToggleState(showGraphics, juce::dontSendNotification);
                        processor.displayGraphicsEnvelopes = showGraphics;
                        
                        // Actualizar componentes visuales (graphics ON = envelopes VISIBLES)
                        transferDisplay.setEnvelopeVisible(showGraphics); // Directo, sin inversión
                        processor.setEnvelopeVisualEnabled(showGraphics);
                        // Actualizar visibilidad del grMeter considerando SOLO SC
                        bool soloScActive = sidechainControls.soloScButton.getToggleState();
                        grMeter.setVisible(showGraphics && !soloScActive);
                    } else {
                        // Si no hay UISettings guardado, mantener el estado actual del botón
                        // Sincronizar con el estado actual del botón
                        processor.displayGraphicsEnvelopes = utilityButtons.runGraphicsButton.getToggleState();
                    }
                    
                    // Queue actualizaciones de parámetros para botones momentáneos
                    // Los parámetros ya se actualizan directamente desde los valores cargados
                }
            }
        }
        
        // Actualizar estado en processor
        processor.setLastPreset(selectedId);
        
        // Para mostrar en el menú, usar nombre limpio sin prefijos
        juce::String displayName = presetName;
        if (presetName.startsWith("Guitars_")) {
            displayName = "[F] " + presetName.substring(8).replace("_", " ");
        } else if (presetName.startsWith("Bass_")) {
            displayName = "[F] " + presetName.substring(5).replace("_", " ");
        } else if (presetName.startsWith("Drums_")) {
            displayName = "[F] " + presetName.substring(6).replace("_", " ");
        } else if (presetName.startsWith("Vocals_")) {
            displayName = "[F] " + presetName.substring(7).replace("_", " ");
        }
        
        processor.setPresetDisplayText(displayName);
        processor.setPresetTextItalic(false);
        presetArea.presetMenu.setTextItalic(false);
        presetArea.presetMenu.setTextWhenNothingSelected(displayName);
        
        // Actualizar sliders desde APVTS
        updateSliderValues();
        
        // Actualizar botones desde APVTS
        updateButtonValues();
        
        // Actualizar la gráfica de transferencia con los valores actuales
        // Es necesario obtener los valores directamente de los parámetros
        // porque los sliders pueden no estar actualizados todavía
        if (auto* thdParam = processor.apvts.getRawParameterValue("b_THD")) {
            transferDisplay.setThreshold(thdParam->load());
        }
        if (auto* ratioParam = processor.apvts.getRawParameterValue("c_RATIO")) {
            transferDisplay.setRatio(ratioParam->load());
        }
        if (auto* kneeParam = processor.apvts.getRawParameterValue("h_KNEE")) {
            transferDisplay.setKnee(kneeParam->load());
        }
        updateTransferDisplay();
        
        // Borrar el historial de undos DESPUÉS de haber establecido todos los valores.
        // Esto previene que los cambios de parámetros se registren en el historial de undo
        undoManager.clearUndoHistory();
        
        // Asegurar que el transfer display se actualice después de limpiar el historial de undo
        updateTransferDisplay();
        
        
        // Nota: el flag isLoadingPreset se resetea automáticamente por el destructor LoadingGuard
    };
    
    addAndMakeVisible(presetArea.presetMenu);
    
    // Inicializar menú de presets
    refreshPresetMenu();
    
    // Configurar el texto inicial del menú según el estado guardado
    auto savedText = processor.getPresetDisplayText();
    auto isItalic = processor.getPresetTextItalic();
    if (!savedText.isEmpty()) {
        presetArea.presetMenu.setTextWhenNothingSelected(savedText);
        presetArea.presetMenu.setTextItalic(isItalic);
    }
}

void JCBCompressorAudioProcessorEditor::setupUtilityButtons()
{
    // Undo - estilo transparente
    utilityButtons.undoButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.undoButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.undoButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.undoButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.undoButton.addListener(this);
    addAndMakeVisible(utilityButtons.undoButton);
    
    // Redo - estilo transparente
    utilityButtons.redoButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.redoButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.redoButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.redoButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.redoButton.addListener(this);
    addAndMakeVisible(utilityButtons.redoButton);
    
    // Establecer el estado inicial de los botones de undo/redo (normalmente desactivados al inicio)
    utilityButtons.undoButton.setAlpha(0.3f);
    utilityButtons.undoButton.setEnabled(false);
    utilityButtons.redoButton.setAlpha(0.3f);
    utilityButtons.redoButton.setEnabled(false);
    
    // Reset GUI - estilo transparente
    utilityButtons.resetGuiButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.resetGuiButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.resetGuiButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.resetGuiButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.resetGuiButton.addListener(this);
    addAndMakeVisible(utilityButtons.resetGuiButton);
    utilityButtons.resetGuiButton.setTooltip(JUCE_UTF8("SIZE: cicla entre diferentes tamaños de ventana.\nClick cambia entre: Actual → Máximo → Mínimo → Actual.\nÚtil para ajustar rápidamente el tamaño del plugin"));
    
    // El botón de bypass se ha movido a parameterButtons

    // Ejecutar gráficos
    utilityButtons.runGraphicsButton.setClickingTogglesState(true);
    utilityButtons.runGraphicsButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);  // OFF: transparente
    utilityButtons.runGraphicsButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));  // ON: con fondo azul
    utilityButtons.runGraphicsButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.runGraphicsButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.runGraphicsButton.addListener(this);
    addAndMakeVisible(utilityButtons.runGraphicsButton);
    utilityButtons.runGraphicsButton.setTooltip(JUCE_UTF8("GRAPHICS: muestra envolventes en tiempo real.\nVisualiza las formas de onda de entrada y salida.\nDesactivar mejora el rendimiento en CPUs lentas"));
    
    // Botón toggle de tooltip - estilo transparente
    utilityButtons.tooltipToggleButton.setClickingTogglesState(true);
    utilityButtons.tooltipToggleButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.tooltipToggleButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.tooltipToggleButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.tooltipToggleButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.tooltipToggleButton.setToggleState(true, juce::dontSendNotification); // ON por defecto
    utilityButtons.tooltipToggleButton.addListener(this);
    addAndMakeVisible(utilityButtons.tooltipToggleButton);
    utilityButtons.tooltipToggleButton.setTooltip(JUCE_UTF8("TOOLTIP: muestra/oculta los tooltips de ayuda.\nActiva o desactiva las ventanas de ayuda emergentes."));
    
    // Botón de idioma - estilo transparente
    utilityButtons.tooltipLangButton.setClickingTogglesState(false);  // No toggle, solo botón normal
    utilityButtons.tooltipLangButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.tooltipLangButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    utilityButtons.tooltipLangButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.tooltipLangButton.setButtonText("esp");  // Texto inicial para español
    utilityButtons.tooltipLangButton.addListener(this);
    addAndMakeVisible(utilityButtons.tooltipLangButton);
    utilityButtons.tooltipLangButton.setTooltip(JUCE_UTF8("IDIOMA: cambia entre español e inglés.\nAlterna el idioma de los tooltips."));
    
    // Botón HQ - oversampling
    utilityButtons.hqButton.setClickingTogglesState(true);
    utilityButtons.hqButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.hqButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.hqButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    utilityButtons.hqButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.hqButton.addListener(this);
    addAndMakeVisible(utilityButtons.hqButton);
    utilityButtons.hqButton.setEnabled(false);  // TODO: Implementar
    
    // Dual Mono button
    utilityButtons.dualMonoButton.setClickingTogglesState(true);
    utilityButtons.dualMonoButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.dualMonoButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.dualMonoButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    utilityButtons.dualMonoButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.dualMonoButton.addListener(this);
    addAndMakeVisible(utilityButtons.dualMonoButton);
    utilityButtons.dualMonoButton.setEnabled(false);  // TODO: Implementar
    
    // Stereo Linked button (ALWAYS ON - plugin only works in stereo linked mode)
    utilityButtons.stereoLinkedButton.setClickingTogglesState(false);  // No toggle
    utilityButtons.stereoLinkedButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.stereoLinkedButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.stereoLinkedButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);  // Siempre texto blanco
    utilityButtons.stereoLinkedButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.stereoLinkedButton.setToggleState(true, juce::dontSendNotification); // Siempre ON
    // No listener necesario ya que siempre está activo
    addAndMakeVisible(utilityButtons.stereoLinkedButton);
    utilityButtons.stereoLinkedButton.setTooltip(JUCE_UTF8("LINK: procesamiento stereo enlazado.\nSiempre activo - el plugin solo funciona en modo stereo linked."));
    utilityButtons.stereoLinkedButton.setEnabled(false);  // Disabled - can't be changed
    
    // Botón M/S
    utilityButtons.msButton.setClickingTogglesState(true);
    utilityButtons.msButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.msButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.msButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    utilityButtons.msButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.msButton.addListener(this);
    addAndMakeVisible(utilityButtons.msButton);
    utilityButtons.msButton.setEnabled(false);  // TODO: Implementar
    
    // A/B State button
    topButtons.abStateButton.setClickingTogglesState(false);  // No es toggle, es un indicador
    topButtons.abStateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff9c27b0));  // Comenzar con púrpura para A
    topButtons.abStateButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    topButtons.abStateButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    topButtons.abStateButton.addListener(this);
    topButtons.abStateButton.setButtonText("A");  // Comenzar con A
    addAndMakeVisible(topButtons.abStateButton);
    topButtons.abStateButton.setEnabled(true);  // Now implemented!
    
    // Botón copiar A/B
    topButtons.abCopyButton.setClickingTogglesState(false);
    topButtons.abCopyButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    topButtons.abCopyButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);
    topButtons.abCopyButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    topButtons.abCopyButton.addListener(this);
    topButtons.abCopyButton.setButtonText("A-B");  // Start showing A to B
    addAndMakeVisible(topButtons.abCopyButton);
    topButtons.abCopyButton.setTooltip("Copy A to B");  // Tooltip inicial
    
    // MIDI Learn button
    utilityButtons.midiLearnButton.setClickingTogglesState(true);
    utilityButtons.midiLearnButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.midiLearnButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::purple.withAlpha(0.3f));
    utilityButtons.midiLearnButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    utilityButtons.midiLearnButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.midiLearnButton.addListener(this);
    addAndMakeVisible(utilityButtons.midiLearnButton);
    utilityButtons.midiLearnButton.setEnabled(false);  // TODO: Implementar
    
    // Botón Zoom
    utilityButtons.zoomButton.setClickingTogglesState(true);
    utilityButtons.zoomButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    utilityButtons.zoomButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    utilityButtons.zoomButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textPrimary);  // Cambiado para igualar otros botones
    utilityButtons.zoomButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    utilityButtons.zoomButton.addListener(this);
    addAndMakeVisible(utilityButtons.zoomButton);
    utilityButtons.zoomButton.setButtonText("zoom"); // Estado inicial: Normal
    utilityButtons.zoomButton.setToggleState(false, juce::dontSendNotification); // Toggle OFF para normal
    utilityButtons.zoomButton.setTooltip(JUCE_UTF8("ZOOM: cambia entre vista normal y ampliada.\nNormal: -72dB a 0dB\nZoom x2: -48dB a 0dB"));
    
    // Botón Diagram
    centerButtons.diagramButton.setClickingTogglesState(true);
    centerButtons.diagramButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    centerButtons.diagramButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    centerButtons.diagramButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    centerButtons.diagramButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    centerButtons.diagramButton.addListener(this);
    addAndMakeVisible(centerButtons.diagramButton);
    centerButtons.diagramButton.setTooltip(JUCE_UTF8("DIAGRAM: muestra el diagrama de bloques del procesador.\nMuestra un overlay con el esquema de procesamiento de audio."));

}

void JCBCompressorAudioProcessorEditor::setupParameterButtons()
{
    // Botón DELTA - movido desde rightBottomKnobs
    parameterButtons.deltaButton.setClickingTogglesState(true);
    parameterButtons.deltaButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    parameterButtons.deltaButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::teal.withAlpha(0.3f));
    parameterButtons.deltaButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    parameterButtons.deltaButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    parameterButtons.deltaButton.addListener(this);
    addAndMakeVisible(parameterButtons.deltaButton);
    // Attachment de DELTA - excluido del undo (sin UndoManager) y no automatizable
    parameterButtons.deltaAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("v_DELTA"), parameterButtons.deltaButton, nullptr);
    parameterButtons.deltaAttachment->onParameterChange = [this]() { handleParameterChange(); };
    parameterButtons.deltaButton.setTooltip(JUCE_UTF8("DELTA: escucha solo la diferencia.\nReproducir la señal procesada menos la original.\nÚtil para escuchar exactamente qué está cambiando"));
    
    // Botón BYPASS - movido desde utilityButtons
    parameterButtons.bypassButton.setClickingTogglesState(true);
    parameterButtons.bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    parameterButtons.bypassButton.setColour(juce::TextButton::buttonOnColourId, DarkTheme::accent.withAlpha(0.3f));
    parameterButtons.bypassButton.setColour(juce::TextButton::textColourOffId, DarkTheme::textSecondary.withAlpha(0.7f));
    parameterButtons.bypassButton.setColour(juce::TextButton::textColourOnId, DarkTheme::textPrimary);
    parameterButtons.bypassButton.addListener(this);
    addAndMakeVisible(parameterButtons.bypassButton);
    // Attachment de BYPASS - excluido del undo (sin UndoManager) y no automatizable
    parameterButtons.bypassAttachment = std::make_unique<UndoableButtonAttachment>(
        *processor.apvts.getParameter("p_BYPASS"), parameterButtons.bypassButton, nullptr);
    parameterButtons.bypassAttachment->onParameterChange = [this]() { handleParameterChange(); };
    parameterButtons.bypassButton.setTooltip(JUCE_UTF8("BYPASS: desactiva el procesamiento del plugin.\nEs independiente del bypass del DAW y está suavizado.\nValor por defecto: OFF"));
}

void JCBCompressorAudioProcessorEditor::setupBackground()
{
    // Cargar imágenes de fondo
    normalBackground = juce::ImageCache::getFromMemory(BinaryData::fondo_png, BinaryData::fondo_pngSize);
    bypassBackground = juce::ImageCache::getFromMemory(BinaryData::bypass_png, BinaryData::bypass_pngSize);
    deltaBackground = juce::ImageCache::getFromMemory(BinaryData::delta_png, BinaryData::delta_pngSize);
    diagramBackground = juce::ImageCache::getFromMemory(BinaryData::diagramaFondo_png, BinaryData::diagramaFondo_pngSize);
    
    // Cargar iconos de filtro (solo usando imágenes de estado OFF)
    hpfOffImage = juce::ImageCache::getFromMemory(BinaryData::hpfOff_png, BinaryData::hpfOff_pngSize);
    lpfOffImage = juce::ImageCache::getFromMemory(BinaryData::lpfOff_png, BinaryData::lpfOff_pngSize);
    
    // Establecer background inicial
    if (normalBackground.isValid())
    {
        backgroundImage.setImage(normalBackground, juce::RectanglePlacement::stretchToFit);
        addAndMakeVisible(backgroundImage);
        backgroundImage.toBack();
    }
    
    // Configurar iconos de filtro - comenzar con estado OFF
    if (hpfOffImage.isValid())
    {
        hpfIcon.setImage(hpfOffImage, juce::RectanglePlacement::centred);
        addAndMakeVisible(hpfIcon);
    }
    
    if (lpfOffImage.isValid())
    {
        lpfIcon.setImage(lpfOffImage, juce::RectanglePlacement::centred);
        addAndMakeVisible(lpfIcon);
    }
}

//==============================================================================
// MÉTODOS DE UPDATE Y REFRESH
//==============================================================================

void JCBCompressorAudioProcessorEditor::updateButtonStates()
{
    // Función maestra que actualiza todos los estados de UI
    updateBasicButtonStates();
    updateSidechainComponentStates();
    updateBackgroundState();
    updateMeterStates();
}

void JCBCompressorAudioProcessorEditor::updateBasicButtonStates()
{
    // Obtener estados actuales
    const bool bypassActive = parameterButtons.bypassButton.getToggleState();
    
    // Actualizar estados visuales básicos
    isBypassed = bypassActive;
    bypassTextVisible = bypassActive;  // Siempre mostrar texto cuando bypass está activo
}

void JCBCompressorAudioProcessorEditor::updateSidechainComponentStates()
{
    // Los filtros HPF/LPF siempre visibles y activos
    sidechainControls.hpfSlider.setVisible(true);
    sidechainControls.lpfSlider.setVisible(true);
    sidechainControls.hpfSlider.setAlpha(1.0f);
    sidechainControls.lpfSlider.setAlpha(1.0f);
    sidechainControls.hpfSlider.setEnabled(true);
    sidechainControls.lpfSlider.setEnabled(true);
    
    // Actualizar visibilidad y estado de los botones de orden de filtro
    sidechainControls.hpfOrderButton.setVisible(true);
    sidechainControls.lpfOrderButton.setVisible(true);
    sidechainControls.hpfOrderButton.setAlpha(1.0f);
    sidechainControls.lpfOrderButton.setAlpha(1.0f);
    sidechainControls.hpfOrderButton.setEnabled(true);
    sidechainControls.lpfOrderButton.setEnabled(true);
    
    // Iconos siempre muestran imágenes blancas/OFF - no cambio dinámico basado en estado FILTERS
    if (hpfOffImage.isValid())
        hpfIcon.setImage(hpfOffImage, juce::RectanglePlacement::centred);
    if (lpfOffImage.isValid())
        lpfIcon.setImage(lpfOffImage, juce::RectanglePlacement::centred);
    
    // Forzar repaint para actualizar colores e iconos
    sidechainControls.hpfSlider.repaint();
    sidechainControls.lpfSlider.repaint();
    sidechainControls.hpfOrderButton.repaint();
    sidechainControls.lpfOrderButton.repaint();
    hpfIcon.repaint();
    lpfIcon.repaint();
    
    // Los sliders de sidechain trim según EXT KEY
    const bool extKeyActive = sidechainControls.keyButton.getToggleState();
    scTrimSlider.setEnabled(extKeyActive);
    scTrimSlider.setAlpha(extKeyActive ? 1.0f : 0.25f);
}

void JCBCompressorAudioProcessorEditor::updateBackgroundState()
{
    // Obtener estados actuales
    const bool bypassActive = parameterButtons.bypassButton.getToggleState();
    const bool deltaActive = parameterButtons.deltaButton.getToggleState();
    
    // Actualizar fondo según prioridad: bypass > delta > normal
    if (bypassActive) {
        backgroundImage.setImage(bypassBackground, juce::RectanglePlacement::stretchToFit);
    }
    else if (deltaActive) {
        backgroundImage.setImage(deltaBackground, juce::RectanglePlacement::stretchToFit);
    }
    else {
        backgroundImage.setImage(normalBackground, juce::RectanglePlacement::stretchToFit);
    }
    
    // IMPORTANTE: Asegurar que el background siempre esté al fondo después de cambiar la imagen
    // Esto previene que el background bloquee otros componentes cuando DELTA está activo
    backgroundImage.toBack();
}

void JCBCompressorAudioProcessorEditor::updateMeterStates()
{
    // Obtener estados actuales
    const bool bypassActive = parameterButtons.bypassButton.getToggleState();
    const bool deltaActive = parameterButtons.deltaButton.getToggleState();
    const bool soloScActive = sidechainControls.soloScButton.getToggleState();
    
    // Actualizar colores de medidores
    // Si bypass está activo, no cambiar colores (mantener normales)
    // Si delta está activo, usar colores delta
    // Si solo SC está activo, usar colores rojos
    bool deltaMode = deltaActive && !bypassActive;
    bool soloMode = soloScActive && !bypassActive && !deltaActive;
    
    inputMeterL.setDeltaMode(deltaMode);
    inputMeterR.setDeltaMode(deltaMode);
    outputMeterL.setDeltaMode(deltaMode);
    outputMeterR.setDeltaMode(deltaMode);
    scMeterL.setDeltaMode(deltaMode);
    scMeterR.setDeltaMode(deltaMode);
    grMeter.setDeltaMode(deltaMode);  // Actualizar color del grMeter en modo DELTA
    
    inputMeterL.setSoloScMode(soloMode);
    inputMeterR.setSoloScMode(soloMode);
    outputMeterL.setSoloScMode(soloMode);
    outputMeterR.setSoloScMode(soloMode);
    scMeterL.setSoloScMode(soloMode);
    scMeterR.setSoloScMode(soloMode);
    
    // Ocultar gain reduction meter cuando SOLO SC está activo (no hay compresión activa)
    // También considerar si graphics está activo para mantener consistencia
    bool graphicsActive = utilityButtons.runGraphicsButton.getToggleState();
    grMeter.setVisible(!soloScActive && graphicsActive);
    
    // Actualizar gradiente de salida para modo bypass
    outputMeterL.setBypassMode(bypassActive);
    outputMeterR.setBypassMode(bypassActive);
    
    // Sincronizar TransferDisplay con modo bypass
    transferDisplay.setBypassMode(bypassActive);
}

void JCBCompressorAudioProcessorEditor::applyDeltaModeToAllControls(bool deltaActive)
{
    // IMPORTANTE: Mantener TODOS los componentes habilitados
    // Solo cambiar aspectos visuales (colores de medidores y display)
    
    if (deltaActive) {
        // Cambiar medidores a modo delta (gradiente verde)
        inputMeterL.setDeltaMode(true);
        inputMeterR.setDeltaMode(true);
        outputMeterL.setDeltaMode(true);
        outputMeterR.setDeltaMode(true);
        scMeterL.setDeltaMode(true);
        scMeterR.setDeltaMode(true);
        grMeter.setDeltaMode(true);
        
        // Atenuar medidores de entrada (efecto visual opcional)
        inputMeterL.setAlpha(0.2f);
        inputMeterR.setAlpha(0.2f);
        scMeterL.setAlpha(0.2f);
        scMeterR.setAlpha(0.2f);
        
        // Atenuar solo el slider de sidechain trim (el trim principal permanece visible)
        scTrimSlider.setAlpha(0.2f);
        
        // Activar modo delta en display
        transferDisplay.setDeltaMode(true);
        // Mantener envelope visible para que se sigan actualizando los datos
        transferDisplay.setEnvelopeVisible(true);
    }
    else {
        // Restaurar todo a normal
        inputMeterL.setDeltaMode(false);
        inputMeterR.setDeltaMode(false);
        outputMeterL.setDeltaMode(false);
        outputMeterR.setDeltaMode(false);
        scMeterL.setDeltaMode(false);
        scMeterR.setDeltaMode(false);
        grMeter.setDeltaMode(false);
        
        // Restaurar alpha normal
        inputMeterL.setAlpha(1.0f);
        inputMeterR.setAlpha(1.0f);
        scMeterL.setAlpha(1.0f);
        scMeterR.setAlpha(1.0f);
        scTrimSlider.setAlpha(1.0f);
        
        // Desactivar modo delta en display
        transferDisplay.setDeltaMode(false);
        transferDisplay.setEnvelopeVisible(true);
    }
    
    // NOTA: NO tocamos ningún botón (presets, A/B, utilidad, etc.)
    // Todos permanecen enabled=true y alpha=1.0f para permitir salida automática de DELTA
}

void JCBCompressorAudioProcessorEditor::exitDeltaMode()
{
    // Salir de modo DELTA si está activo
    if (parameterButtons.deltaButton.getToggleState()) {
        parameterButtons.deltaButton.setToggleState(false, juce::sendNotification);
    }
}

void JCBCompressorAudioProcessorEditor::updateTransferDisplay()
{
    // Actualizar los valores de los parámetros desde los sliders
    float thresholdDB = leftTopKnobs.thdSlider.getValue();
    transferDisplay.setThreshold(thresholdDB);
    transferDisplay.setRatio(leftTopKnobs.ratioSlider.getValue());
    transferDisplay.setKnee(leftTopKnobs.kneeSlider.getValue());
    
    // Actualizar estados de sidechain
    transferDisplay.setExtKeyActive(sidechainControls.keyButton.getToggleState());
    transferDisplay.setSoloSidechain(sidechainControls.soloScButton.getToggleState());
    
    // Actualizar la curva visual
    transferDisplay.updateCurve();
}

void JCBCompressorAudioProcessorEditor::updateMeters()
{
    // CORRECCIÓN CRÍTICA: Cachear estados de visibilidad para evitar llamadas repetidas a setVisible
    static bool lastSoloScActive = false;
    static bool lastExtKeyActive = false;
    
    // Verificar si SOLO SC está activo
    const bool soloScActive = sidechainControls.soloScButton.getToggleState();
    const bool extKeyActive = sidechainControls.keyButton.getToggleState();
    
    // Solo actualizar visibilidad cuando el estado realmente cambia
    if (soloScActive != lastSoloScActive || extKeyActive != lastExtKeyActive) {
        if (soloScActive) {
            // SOLO SC activo - mostrar meters apropiados basados en EXT KEY
            if (extKeyActive) {
                // Ocultar medidores principales de entrada
                inputMeterL.setVisible(false);
                inputMeterR.setVisible(false);
                // Hacer visibles los medidores de sidechain
                scMeterL.setVisible(true);
                scMeterR.setVisible(true);
            } else {
                // Mostrar medidores principales de entrada en modo SOLO SC (sidechain interno)
                inputMeterL.setVisible(true);
                inputMeterR.setVisible(true);
                scMeterL.setVisible(true);
                scMeterR.setVisible(true);
            }
        } else {
            // Modo normal - mostrar todos los medidores normalmente
            inputMeterL.setVisible(true);
            inputMeterR.setVisible(true);
            scMeterL.setVisible(true);
            scMeterR.setVisible(true);
        }
        
        lastSoloScActive = soloScActive;
        lastExtKeyActive = extKeyActive;
    }
    
    // Actualizar medidores con control centralizado a 60 Hz
    // Llamar a updateLevel() de cada medidor para procesar valores y animaciones
    if (inputMeterL.isVisible()) {
        inputMeterL.updateLevel();
        inputMeterR.updateLevel();
        inputMeterL.repaint();
        inputMeterR.repaint();
    }
    
    if (scMeterL.isVisible()) {
        scMeterL.updateLevel();
        scMeterR.updateLevel();
        scMeterL.repaint();
        scMeterR.repaint();
    }
    
    // Siempre actualizar detección de clip de sidechain
    scMeterL.setClipDetected(processor.getSidechainClipDetected(0));
    scMeterR.setClipDetected(processor.getSidechainClipDetected(1));
    
    // Siempre actualizar medidores de salida y reducción de ganancia
    grMeter.updateLevel();
    grMeter.repaint();
    
    outputMeterL.updateLevel();
    outputMeterR.updateLevel();
    outputMeterL.repaint();
    outputMeterR.repaint();
}


void JCBCompressorAudioProcessorEditor::updateSliderValues()
{
    // Actualizar todos los sliders con los valores actuales del APVTS
    // Esto soluciona el problema de que los valores no se actualizan al cargar sesión
    // FIXED: Los comentarios anteriores eran incorrectos - todos los sliders usan CustomSliderAttachment
    
    // Left top knobs - Todos usan CustomSliderAttachment
    if (auto* param = processor.apvts.getRawParameterValue("b_THD"))
        leftTopKnobs.thdSlider.setValue(param->load(), juce::dontSendNotification);
    
    if (auto* param = processor.apvts.getRawParameterValue("c_RATIO"))
        leftTopKnobs.ratioSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("h_KNEE"))
        leftTopKnobs.kneeSlider.setValue(param->load(), juce::dontSendNotification);
    
    // Left bottom knobs
    if (auto* param = processor.apvts.getRawParameterValue("o_DRYWET"))
        leftBottomKnobs.drywetSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("n_LOOKAHEAD"))
        leftBottomKnobs.lookaheadSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("u_SOFTCLIP"))
        leftBottomKnobs.clipSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("x_AUTOGAIN"))
        leftTopKnobs.againSlider.setValue(param->load(), juce::dontSendNotification);
    
    // Right top controls
    if (auto* param = processor.apvts.getRawParameterValue("f_ALGO"))
        rightTopControls.algoSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("g_REACT"))
        rightTopControls.reactSlider.setValue(param->load(), juce::dontSendNotification);
    
    // Right bottom knobs - Todos usan CustomSliderAttachment
    if (auto* param = processor.apvts.getRawParameterValue("d_ATK"))
        rightBottomKnobs.atkSlider.setValue(param->load(), juce::dontSendNotification);
    
    if (auto* param = processor.apvts.getRawParameterValue("e_REL"))
        rightBottomKnobs.relSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("w_PARALLEL"))
        leftBottomKnobs.parallSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("z_SMOOTH"))
        rightTopControls.smoothSlider.setValue(param->load(), juce::dontSendNotification);
    
    // Controles de sidechain - Todos usan CustomSliderAttachment
    if (auto* param = processor.apvts.getRawParameterValue("j_HPF"))
        sidechainControls.hpfSlider.setValue(param->load(), juce::dontSendNotification);
        
    if (auto* param = processor.apvts.getRawParameterValue("k_LPF"))
        sidechainControls.lpfSlider.setValue(param->load(), juce::dontSendNotification);
    

    // Slider de trims (both linked to the same parameter)
    if (auto* param = processor.apvts.getRawParameterValue("a_TRIM")) {
        float trimValue = param->load();
        trimSlider.setValue(trimValue, juce::dontSendNotification);
    }
    
    // Slider de makeups (both linked to the same parameter)
    if (auto* param = processor.apvts.getRawParameterValue("i_MAKEUP")) {
        float makeupValue = param->load();
        makeupSlider.setValue(makeupValue, juce::dontSendNotification);
    }
    
    // Slider de trim de sidechain
    if (auto* param = processor.apvts.getRawParameterValue("y_SCTRIM")) {
        float scTrimValue = param->load();
        scTrimSlider.setValue(scTrimValue, juce::dontSendNotification);
    }
}

void JCBCompressorAudioProcessorEditor::updateButtonValues()
{
    // Actualizar todos los botones desde APVTS
    // Esta función es necesaria para que los presets carguen correctamente los estados de los botones
    
    // Botón SC (Sidechain Filter Enable)
    if (auto* param = processor.apvts.getRawParameterValue("l_SC")) {
        bool toggleState = param->load() >= 0.5f;
        sidechainControls.scButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón SOLO SC
    if (auto* param = processor.apvts.getRawParameterValue("m_SOLOSC")) {
        bool toggleState = param->load() >= 0.5f;
        sidechainControls.soloScButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón BYPASS
    if (auto* param = processor.apvts.getRawParameterValue("p_BYPASS")) {
        bool toggleState = param->load() >= 0.5f;
        parameterButtons.bypassButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón KEY (External Key)
    if (auto* param = processor.apvts.getRawParameterValue("r_KEY")) {
        bool toggleState = param->load() >= 0.5f;
        sidechainControls.keyButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón AUTO RELEASE
    if (auto* param = processor.apvts.getRawParameterValue("s_AUTORELEASE")) {
        bool toggleState = param->load() >= 0.5f;
        rightBottomKnobs.autorelButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón DELTA
    if (auto* param = processor.apvts.getRawParameterValue("v_DELTA")) {
        bool toggleState = param->load() >= 0.5f;
        parameterButtons.deltaButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón HPF Order
    if (auto* param = processor.apvts.getRawParameterValue("j_HPFORDER")) {
        bool toggleState = param->load() >= 0.5f;
        sidechainControls.hpfOrderButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Botón LPF Order
    if (auto* param = processor.apvts.getRawParameterValue("k_LPFORDER")) {
        bool toggleState = param->load() >= 0.5f;
        sidechainControls.lpfOrderButton.setToggleState(toggleState, juce::dontSendNotification);
    }
    
    // Actualizar estados visuales de los botones
    updateButtonStates();
}

void JCBCompressorAudioProcessorEditor::resetGuiSize()
{
    // Ciclar al siguiente estado: Current -> Maximum -> Minimum -> Current
    switch (currentSizeState) {
        case GuiSizeState::Current:
            // Current -> Maximum
            processor.setSavedSize({MAX_WIDTH, MAX_HEIGHT});
            setSize(MAX_WIDTH, MAX_HEIGHT);
            currentSizeState = GuiSizeState::Maximum;
            break;

        case GuiSizeState::Maximum:
            // Maximum -> Minimum
            processor.setSavedSize({MIN_WIDTH, MIN_HEIGHT});
            setSize(MIN_WIDTH, MIN_HEIGHT);
            currentSizeState = GuiSizeState::Minimum;
            break;

        case GuiSizeState::Minimum:
            // Minimum -> Current (back to default)
            processor.setSavedSize({DEFAULT_WIDTH, DEFAULT_HEIGHT});
            setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
            currentSizeState = GuiSizeState::Current;
            break;
    }
    
    // Forzar repaint para asegurar que se actualice correctamente
    repaint();
}




//==============================================================================
// GESTIÓN DE PRESETS
//==============================================================================
juce::File JCBCompressorAudioProcessorEditor::getPresetsFolder()
{
    juce::File folder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory)
        .getChildFile("Audio")
        .getChildFile("Presets")
        .getChildFile("Coeval")
        .getChildFile("JCBCompressor");

    if (!folder.isDirectory() && !folder.existsAsFile()) {
        folder.createDirectory();
    }
    
    return folder;
}

juce::Array<juce::File> JCBCompressorAudioProcessorEditor::populatePresetFolder()
{
    return getPresetsFolder().findChildFiles(2, false, "*.preset");
}

void JCBCompressorAudioProcessorEditor::refreshPresetMenu()
{
    presetArea.presetMenu.clear();
    presetIdToNameMap.clear();  // Limpiar mapeo anterior
    
    // Añadir DEFAULT como primer preset siempre
    presetArea.presetMenu.addItem("DEFAULT", 1);
    presetIdToNameMap[1] = "DEFAULT";
    
    // Añadir separador
    presetArea.presetMenu.addItem("---", 2);
    
    // Organizar factory presets por categoría
    std::map<juce::String, juce::StringArray> categorizedPresets;
    std::map<juce::String, juce::StringArray> categorizedFullNames;
    int nextId = 100;
    
    // Procesar factory presets desde BinaryData
    for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
    {
        juce::String resourceName(BinaryData::namedResourceList[i]);
        
        // Buscar archivos que terminen en "_preset"
        if (resourceName.endsWith("_preset"))
        {
            juce::String cleanName = resourceName.replace("_preset", "");
            
            // Detectar categoría del prefijo
            juce::String category;
            juce::String presetName = cleanName;
            
            if (cleanName.startsWith("Guitars_"))
            {
                category = "Guitars";
                presetName = cleanName.substring(8).replace("_", " ");
            }
            else if (cleanName.startsWith("Bass_"))
            {
                category = "Bass";
                presetName = cleanName.substring(5).replace("_", " ");
            }
            else if (cleanName.startsWith("Drums_"))
            {
                category = "Drums";
                presetName = cleanName.substring(6).replace("_", " ");
            }
            else if (cleanName.startsWith("Vocals_"))
            {
                category = "Vocals";
                presetName = cleanName.substring(7).replace("_", " ");
            }
            else
            {
                // Si no tiene prefijo reconocido, saltar este preset
                continue;
            }
            
            categorizedPresets[category].add("[F] " + presetName);
            categorizedFullNames[category].add(cleanName);  // Guardar nombre completo con prefijo
        }
    }
    
    // Añadir categorías al menú en orden específico
    juce::StringArray categoryOrder = {"Guitars", "Bass", "Drums", "Vocals"};
    
    for (const auto& category : categoryOrder)
    {
        if (categorizedPresets.find(category) != categorizedPresets.end())
        {
            auto& presets = categorizedPresets[category];
            auto& fullNames = categorizedFullNames[category];
            presets.sort(true);
            fullNames.sort(true);
            
            // Añadir categoría con sus subitems
            presetArea.presetMenu.addCategoryItem(category, presets, nextId);
            
            // Mapear IDs a nombres de presets
            for (int j = 0; j < presets.size(); ++j)
            {
                int presetId = nextId + j + 1;  // +1 porque el ID de la categoría es nextId
                presetIdToNameMap[presetId] = fullNames[j];  // Usar nombre completo con prefijo
            }
            
            nextId += static_cast<int>(presets.size()) + 1;
        }
    }
    
    // Añadir separador antes de user presets
    presetArea.presetMenu.addItem("---", nextId++);
    
    // Añadir user presets (sin categoría)
    juce::StringArray userPresets;
    for (int i = 0; i < getUpdatedNumPresets(); i++) {
        userPresets.add(populatePresetFolder()[i].getFileNameWithoutExtension());
    }
    
    userPresets.sort(true);
    for (const auto& preset : userPresets)
    {
        presetArea.presetMenu.addItem(preset, nextId);
        presetIdToNameMap[nextId] = preset;  // Mapear user presets también
        nextId++;
    }
    
    // Restaurar selección previa si existe
    if (processor.getLastPreset() > 0) {
        presetArea.presetMenu.setSelectedId(processor.getLastPreset());
    }
}

void JCBCompressorAudioProcessorEditor::savePresetFile()
{
    // Obtener estado del menú de presets y del processor
    juce::String currentText = presetArea.presetMenu.getText();
    juce::String processorText = processor.getPresetDisplayText();
    
    // Si no hay texto en el menú, usar el texto del processor
    if (currentText.isEmpty() && !processorText.isEmpty()) {
        currentText = processorText;
    }
    
    // Extraer el nombre del preset sin asterisco
    juce::String currentPresetName = currentText.endsWith("*") ? 
        currentText.dropLastCharacters(1).trimEnd() : currentText;
    
    // Si no hay preset o es DEFAULT, ir a Save As
    if (currentPresetName.isEmpty() || currentPresetName == "DEFAULT") {
        saveAsPresetFile();
        return;
    }
    
    // Verificar si es un factory preset
    bool isFactoryPreset = currentPresetName.startsWith("[F] ");
    if (isFactoryPreset) {
        // Quitar el prefijo [F] para mostrar el nombre limpio
        juce::String cleanName = currentPresetName.substring(4);
        showCustomAlertDialog(JUCE_UTF8("Cannot overwrite"), 
                            JUCE_UTF8("Factory Presets are read-only.\nUse 'Save As' to create a copy."));
        return;
    }
    
    // Si hay un preset cargado (modificado o no), ofrecer opciones
    // Mostrar diálogo si hay un preset cargado (modificado O no modificado)
    if (!currentPresetName.isEmpty()) {
        overwritePresetDialog = std::make_unique<CustomThreeButtonDialog>(
            "Overwrite \"" + currentPresetName + "\"",
            "",  // No mensaje adicional, el título es suficiente
            "Overwrite",
            "Save As",
            "Cancel"
        );
        
        overwritePresetDialog->onDialogClosed = [this, currentPresetName](bool wasAccepted) {
            if (wasAccepted) {
                int result = overwritePresetDialog->getSelectedButton();
                if (result == 1) { // Overwrite
                // Guardar directamente en el archivo existente
                auto presetFile = getPresetsFolder().getChildFile(currentPresetName + ".preset");
                auto state = processor.apvts.copyState();
                
                // Añadir estado del botón graphics (no automatizable)
                auto uiSettings = state.getOrCreateChildWithName("UISettings", nullptr);
                uiSettings.setProperty("displayGraphicsEnvelopes", utilityButtons.runGraphicsButton.getToggleState(), nullptr);
                
                std::unique_ptr<juce::XmlElement> xml(state.createXml());
                
                if (xml != nullptr) {
                    xml->writeTo(presetFile);
                    
                    // Actualizar estado visual
                    processor.setPresetDisplayText(currentPresetName);
                    processor.setPresetTextItalic(false);
                    presetArea.presetMenu.setTextWhenNothingSelected(currentPresetName);
                    presetArea.presetMenu.setTextItalic(false);
                    
                    // Actualizar menú si es necesario
                    if (presetArea.presetMenu.getSelectedId() <= 0) {
                        refreshPresetMenu();
                        // No usar setSelectedId para evitar triggerar onChange
                        presetArea.presetMenu.setSelectedId(0);
                        presetArea.presetMenu.setTextWhenNothingSelected(currentPresetName);
                        presetArea.presetMenu.setTextItalic(false);
                        processor.setPresetDisplayText(currentPresetName);
                        processor.setPresetTextItalic(false);
                    }
                }
                } else if (result == 2) { // Save As...
                    saveAsPresetFile();
                }
                // result == 3 is Cancel, do nothing
            }
            
            // Limpiar el diálogo después de usarlo
            overwritePresetDialog.reset();
        };
        
        addChildComponent(overwritePresetDialog.get());
        overwritePresetDialog->showDialog();
    } else {
        // No hay preset cargado, ir directamente a "Save As..."
        saveAsPresetFile();
    }
}

void JCBCompressorAudioProcessorEditor::saveAsPresetFile()
{
    // Crear y mostrar el diálogo personalizado
    savePresetDialog = std::make_unique<SavePresetDialog>("");
    savePresetDialog->onDialogClosed = [this](bool shouldSave) {
        if (shouldSave) {
            juce::String presetName = savePresetDialog->getPresetName();
            
            // No permitir usar el nombre DEFAULT
            if (presetName.toUpperCase() == "DEFAULT") {
                showCustomAlertDialog("Error", 
                                    "Cannot use the name DEFAULT for a preset.");
                return;
            }
            
            // Verificar si el nombre coincide con algún factory preset
            for (const auto& factoryName : factoryPresetNames) {
                if (presetName.compareIgnoreCase(factoryName) == 0) {
                    showCustomAlertDialog(JUCE_UTF8("Reserved name"), 
                                        JUCE_UTF8("Name reserved for Factory Preset.\nPlease choose another name."));
                    return;
                }
            }
            
            // Crear archivo con el nombre del preset
            juce::File presetFile = getPresetsFolder().getChildFile(presetName + ".preset");
            
            // Si el archivo ya existe, preguntar si sobrescribir
            if (presetFile.existsAsFile()) {
                showCustomConfirmDialog(JUCE_UTF8("Preset already exists"), 
                                      JUCE_UTF8("Do you want to overwrite the existing preset?"), 
                                      [this, presetFile, presetName](bool shouldOverwrite) {
                    if (shouldOverwrite) {
                        // Guardar el preset
                        auto state = processor.apvts.copyState();
                        
                        // Añadir estado del botón graphics (no automatizable)
                        auto uiSettings = state.getOrCreateChildWithName("UISettings", nullptr);
                        uiSettings.setProperty("displayGraphicsEnvelopes", utilityButtons.runGraphicsButton.getToggleState(), nullptr);
                        
                        std::unique_ptr<juce::XmlElement> xml(state.createXml());
                        
                        if (xml != nullptr) {
                            xml->writeTo(presetFile);
                            
                            // Actualizar el menú
                            refreshPresetMenu();
                            
                            // Mostrar el preset recién guardado sin triggerar onChange
                            presetArea.presetMenu.setSelectedId(0);
                            presetArea.presetMenu.setTextWhenNothingSelected(presetName);
                            presetArea.presetMenu.setTextItalic(false);
                            processor.setPresetDisplayText(presetName);
                            processor.setPresetTextItalic(false);
                            processor.setLastPreset(0);
                        }
                    }
                });
            } else {
                // Guardar directamente si no existe
                auto state = processor.apvts.copyState();
                
                // Añadir estado del botón graphics (no automatizable)
                auto uiSettings = state.getOrCreateChildWithName("UISettings", nullptr);
                uiSettings.setProperty("displayGraphicsEnvelopes", utilityButtons.runGraphicsButton.getToggleState(), nullptr);
                
                std::unique_ptr<juce::XmlElement> xml(state.createXml());
                
                if (xml != nullptr) {
                    xml->writeTo(presetFile);
                    
                    // Actualizar el menú
                    refreshPresetMenu();
                    
                    // Mostrar el preset recién guardado sin triggerar onChange
                    presetArea.presetMenu.setSelectedId(0);
                    presetArea.presetMenu.setTextWhenNothingSelected(presetName);
                    presetArea.presetMenu.setTextItalic(false);
                    processor.setPresetDisplayText(presetName);
                    processor.setPresetTextItalic(false);
                    processor.setLastPreset(0);
                }
            }
        }
        
        // Limpiar el diálogo
        savePresetDialog.reset();
    };
    
    addChildComponent(savePresetDialog.get());
    savePresetDialog->showDialog();
}

void JCBCompressorAudioProcessorEditor::deletePresetFile()
{
    int selectedId = presetArea.presetMenu.getSelectedId();
    juce::String presetName;
    
    if (selectedId > 0) {
        // Obtener el texto del preset seleccionado
        // Primero intentar obtenerlo por su ID directo (funciona con categorías)
        presetName = presetArea.presetMenu.getText();
        
        // Si está vacío, intentar por índice (compatibilidad)
        if (presetName.isEmpty()) {
            presetName = presetArea.presetMenu.getItemText(selectedId - 1);
        }
    } else {
        // No hay selección, verificar si hay un preset modificado
        juce::String displayText = presetArea.presetMenu.getTextWhenNothingSelected();
        if (displayText.isEmpty()) {
            displayText = processor.getPresetDisplayText();
        }
        
        if (displayText.isEmpty()) {
            showCustomAlertDialog(JUCE_UTF8("Error"), 
                                JUCE_UTF8("Ningún preset seleccionado."));
            return;
        }
        
        // Quitar asterisco si lo tiene
        presetName = displayText.endsWith("*") ? 
            displayText.dropLastCharacters(1).trimEnd() : displayText;
    }
    
    // No permitir borrar DEFAULT ni factory presets
    if (presetName == "DEFAULT" || presetName.startsWith("[F] ")) {
        juce::String errorMsg = presetName == "DEFAULT" ? 
            JUCE_UTF8("The DEFAULT preset cannot be deleted.") :
            JUCE_UTF8("Factory Presets cannot be deleted.");
        showCustomAlertDialog(JUCE_UTF8("Error"), errorMsg);
        return;
    }
    
    showCustomConfirmDialog(JUCE_UTF8("Delete Preset"), 
                            JUCE_UTF8("The preset \"") + presetName + JUCE_UTF8("\" will be moved to trash."), 
                            [this, presetName](bool confirmed) {
        if (confirmed) {
            juce::File presetFile = getPresetsFolder().getChildFile(presetName + ".preset");
            
            if (presetFile.existsAsFile()) {
                presetFile.moveToTrash();
                
                // Resetear el último preset para evitar selección automática
                processor.setLastPreset(0);
                
                // Actualizar el menú
                refreshPresetMenu();
                
                // Dejar el menú sin selección y sin texto
                presetArea.presetMenu.setSelectedId(0);
                presetArea.presetMenu.setTextWhenNothingSelected("");
                presetArea.presetMenu.setTextItalic(false);
                processor.setPresetDisplayText("");
                processor.setPresetTextItalic(false);
                processor.setLastPreset(0);
                
                // NO cargar DEFAULT - dejar los parámetros como están
            }
        }
    });
}

void JCBCompressorAudioProcessorEditor::selectNextPreset()
{
    // Obtener todos los IDs seleccionables (incluye sub-items de categorías)
    auto allIds = presetArea.presetMenu.getAllSelectableIds();
    if (allIds.empty()) return;
    
    int currentId = presetArea.presetMenu.getSelectedId();
    
    // Si no hay selección actual (preset modificado), buscar el preset base
    if (currentId == 0) {
        // Obtener el nombre del preset actual sin asterisco
        juce::String currentText = presetArea.presetMenu.getTextWhenNothingSelected();
        if (currentText.endsWith("*")) {
            currentText = currentText.dropLastCharacters(1).trimEnd();
        }
        
        // Si hay un texto válido, buscar el preset correspondiente
        if (currentText.isNotEmpty() && currentText != "DEFAULT*" && currentText != "DEFAULT") {
            // Buscar el ID del preset por nombre
            for (int id : allIds) {
                // Guardar el ID actual temporalmente
                int tempId = presetArea.presetMenu.getSelectedId();
                
                // Seleccionar temporalmente para obtener el texto
                presetArea.presetMenu.setSelectedId(id);
                juce::String menuText = presetArea.presetMenu.getText();
                
                // Restaurar el ID previo
                presetArea.presetMenu.setSelectedId(tempId);
                
                // Comparar con el texto actual
                if (menuText == currentText || menuText == "[F] " + currentText.substring(4)) {
                    currentId = id;
                    break;
                }
            }
        }
        
        // Si aún no tenemos un ID válido, empezar desde el primero
        if (currentId == 0) {
            currentId = allIds[0];
        }
    }
    
    // Buscar el ID actual en la lista de todos los IDs
    auto it = std::find(allIds.begin(), allIds.end(), currentId);
    
    // Si encontramos el ID actual, avanzar al siguiente
    if (it != allIds.end()) {
        ++it;
        // Si llegamos al final, ciclar al principio
        if (it == allIds.end()) {
            it = allIds.begin();
        }
        presetArea.presetMenu.setSelectedId(*it);
        if (presetArea.presetMenu.onChange) {
            presetArea.presetMenu.onChange();
        }
    } else {
        // Si no encontramos el ID actual (puede pasar con presets guardados), seleccionar el primero
        presetArea.presetMenu.setSelectedId(allIds[0]);
        if (presetArea.presetMenu.onChange) {
            presetArea.presetMenu.onChange();
        }
    }
}

void JCBCompressorAudioProcessorEditor::selectPreviousPreset()
{
    // Obtener todos los IDs seleccionables (incluye sub-items de categorías)
    auto allIds = presetArea.presetMenu.getAllSelectableIds();
    if (allIds.empty()) return;
    
    int currentId = presetArea.presetMenu.getSelectedId();
    
    // Si no hay selección actual (preset modificado), buscar el preset base
    if (currentId == 0) {
        // Obtener el nombre del preset actual sin asterisco
        juce::String currentText = presetArea.presetMenu.getTextWhenNothingSelected();
        if (currentText.endsWith("*")) {
            currentText = currentText.dropLastCharacters(1).trimEnd();
        }
        
        // Si hay un texto válido, buscar el preset correspondiente
        if (currentText.isNotEmpty() && currentText != "DEFAULT*" && currentText != "DEFAULT") {
            // Buscar el ID del preset por nombre
            for (int id : allIds) {
                // Guardar el ID actual temporalmente
                int tempId = presetArea.presetMenu.getSelectedId();
                
                // Seleccionar temporalmente para obtener el texto
                presetArea.presetMenu.setSelectedId(id);
                juce::String menuText = presetArea.presetMenu.getText();
                
                // Restaurar el ID previo
                presetArea.presetMenu.setSelectedId(tempId);
                
                // Comparar con el texto actual
                if (menuText == currentText || menuText == "[F] " + currentText.substring(4)) {
                    currentId = id;
                    break;
                }
            }
        }
        
        // Si aún no tenemos un ID válido, empezar desde el último
        if (currentId == 0) {
            currentId = allIds.back();
        }
    }
    
    // Buscar el ID actual en la lista de todos los IDs
    auto it = std::find(allIds.begin(), allIds.end(), currentId);
    
    // Si encontramos el ID actual, retroceder al anterior
    if (it != allIds.end()) {
        if (it == allIds.begin()) {
            // Si estamos al principio, ciclar al final
            presetArea.presetMenu.setSelectedId(allIds.back());
        } else {
            --it;
            presetArea.presetMenu.setSelectedId(*it);
        }
        if (presetArea.presetMenu.onChange) {
            presetArea.presetMenu.onChange();
        }
    } else {
        // Si no encontramos el ID actual (puede pasar con presets guardados), seleccionar el último
        presetArea.presetMenu.setSelectedId(allIds.back());
        if (presetArea.presetMenu.onChange) {
            presetArea.presetMenu.onChange();
        }
    }
}

//==============================================================================
// DIÁLOGOS Y OVERLAYS
//==============================================================================
void JCBCompressorAudioProcessorEditor::showCustomConfirmDialog(const juce::String& message, 
                                                          const juce::String& subMessage,
                                                          std::function<void(bool)> callback,
                                                          const juce::String& confirmText,
                                                          const juce::String& cancelText)
{
    // Combinar mensaje principal y submensaje si existe
    juce::String fullMessage = message;
    if (subMessage.isNotEmpty()) {
        fullMessage += "\n\n" + subMessage;
    }
    
    // Usar el nuevo diálogo personalizado
    deleteConfirmDialog = std::make_unique<CustomConfirmDialog>("Confirmation", fullMessage, confirmText, cancelText);
    deleteConfirmDialog->onDialogClosed = callback;
    addChildComponent(deleteConfirmDialog.get());
    deleteConfirmDialog->showDialog();
}

void JCBCompressorAudioProcessorEditor::showCustomAlertDialog(const juce::String& title, const juce::String& message)
{
    // Crear un diálogo de alerta simple con solo OK
    alertDialog = std::make_unique<CustomAlertDialog>(title, message);
    alertDialog->onDialogClosed = [](bool) {}; // Callback vacío
    addChildComponent(alertDialog.get());
    alertDialog->showDialog();
}

void JCBCompressorAudioProcessorEditor::showCredits()
{
    // Salir de todos los modos especiales antes de mostrar créditos
    // para evitar overlays o fondos especiales
    
    // Salir de DELTA
    exitDeltaMode();
    
    // Salir de BYPASS
    if (parameterButtons.bypassButton.getToggleState()) {
        parameterButtons.bypassButton.setToggleState(false, juce::sendNotification);
    }
    
    // Salir de SOLO SC
    if (sidechainControls.soloScButton.getToggleState()) {
        sidechainControls.soloScButton.setToggleState(false, juce::sendNotification);
    }
    
    if (creditsOverlay == nullptr)
    {
        // Obtener el formato del plugin desde el processor
        juce::String format = processor.getPluginFormat();
        
        creditsOverlay = std::make_unique<CreditsOverlay>(format);
        creditsOverlay->setBounds(getLocalBounds());
        creditsOverlay->onClose = [this]() {
            hideCredits();
        };
        
        addAndMakeVisible(creditsOverlay.get());
        creditsOverlay->grabKeyboardFocus();
    }
}

void JCBCompressorAudioProcessorEditor::hideCredits()
{
    if (creditsOverlay != nullptr)
    {
        removeChildComponent(creditsOverlay.get());
        creditsOverlay.reset();
    }
}

//==============================================================================
// SISTEMA DE TOOLTIPS
//==============================================================================
void JCBCompressorAudioProcessorEditor::updateTodoButtonTexts()
{
    // Update TODO button tooltips based on current language
    bool isSpanish = (currentLanguage == TooltipLanguage::Spanish);
    
    // HQ button tooltip
    utilityButtons.hqButton.setTooltip(isSpanish ? JUCE_UTF8("POR HACER: Habilita oversampling para mayor calidad.") : "TODO: Enables oversampling for higher quality.");
    
    // Dual Mono button tooltip
    utilityButtons.dualMonoButton.setTooltip(isSpanish ? JUCE_UTF8("POR HACER: Procesa canales L/R independientemente.") : "TODO: Processes L/R channels independently.");
    
    // Stereo Linked button - mantener tooltip actual
    // utilityButtons.stereoLinkedButton tooltip ya está implementado
    
    // Botón M/S tooltip
    utilityButtons.msButton.setTooltip(isSpanish ? JUCE_UTF8("POR HACER: Procesa en formato Mid/Side.") : "TODO: Processes in M/S format.");
    
    // A/B State button tooltip
    topButtons.abStateButton.setTooltip(isSpanish ? JUCE_UTF8("Alterna entre dos configuraciones A/B para comparar ajustes.") : "Switches between two A/B configurations to compare settings.");
    
    // MIDI Learn button tooltip
    utilityButtons.midiLearnButton.setTooltip(isSpanish ? JUCE_UTF8("POR HACER: Asigna control MIDI.") : "TODO: Assigns MIDI control.");
    
    
    // Zoom y Diagram mantienen sus tooltips actuales ya que son funcionales
}

void JCBCompressorAudioProcessorEditor::updateAllTooltips()
{
    // Actualizar todos los tooltips de componentes basado en el idioma actual
    
    // Título
    titleLink.setTooltip(getTooltipText("title"));
    
    // Perillas - superiores izquierdas
    leftTopKnobs.thdSlider.setTooltip(getTooltipText("thd"));
    leftTopKnobs.ratioSlider.setTooltip(getTooltipText("ratio"));
    leftTopKnobs.kneeSlider.setTooltip(getTooltipText("knee"));
    
    // Perillas - inferiores izquierdas
    leftBottomKnobs.drywetSlider.setTooltip(getTooltipText("drywet"));
    leftBottomKnobs.lookaheadSlider.setTooltip(getTooltipText("lookahead"));
    leftBottomKnobs.clipSlider.setTooltip(getTooltipText("clip"));
    leftTopKnobs.againSlider.setTooltip(getTooltipText("again"));
    
    // Perillas - superiores derechas
    rightTopControls.algoSlider.setTooltip(getTooltipText("algo"));
    rightTopControls.reactSlider.setTooltip(getTooltipText("react"));
    
    // Perillas - inferiores derechas
    rightBottomKnobs.atkSlider.setTooltip(getTooltipText("attack"));
    rightBottomKnobs.relSlider.setTooltip(getTooltipText("release"));
    rightBottomKnobs.autorelButton.setTooltip(getTooltipText("autorel"));
    // speedButton removed
    parameterButtons.deltaButton.setTooltip(getTooltipText("delta"));
    leftBottomKnobs.parallSlider.setTooltip(getTooltipText("parallel"));
    rightTopControls.smoothSlider.setTooltip(getTooltipText("smooth"));
    
    // Sliders de trim y makeup
    trimSlider.setTooltip(getTooltipText("trim"));
    makeupSlider.setTooltip(getTooltipText("makeup"));
    
    // Sliders de trim de sidechain
    scTrimSlider.setTooltip(getTooltipText("sctrim"));
    
    // Controles de sidechain
    sidechainControls.scButton.setTooltip(getTooltipText("sc"));
    sidechainControls.keyButton.setTooltip(getTooltipText("extkey"));
    sidechainControls.soloScButton.setTooltip(getTooltipText("solosc"));
    sidechainControls.hpfSlider.setTooltip(getTooltipText("hpf"));
    sidechainControls.lpfSlider.setTooltip(getTooltipText("lpf"));
    
    // Área de presets
    presetArea.saveButton.setTooltip(getTooltipText("save"));
    presetArea.saveAsButton.setTooltip(getTooltipText("saveas"));
    presetArea.deleteButton.setTooltip(getTooltipText("delete"));
    presetArea.backButton.setTooltip(getTooltipText("back"));
    presetArea.nextButton.setTooltip(getTooltipText("next"));
    
    // Utility buttons
    utilityButtons.undoButton.setTooltip(getTooltipText("undo"));
    utilityButtons.redoButton.setTooltip(getTooltipText("redo"));
    utilityButtons.resetGuiButton.setTooltip(getTooltipText("resetgui"));
    utilityButtons.runGraphicsButton.setTooltip(getTooltipText("graphics"));
    utilityButtons.zoomButton.setTooltip(getTooltipText("zoom"));
    centerButtons.diagramButton.setTooltip(getTooltipText("diagram"));
    utilityButtons.tooltipToggleButton.setTooltip(getTooltipText("tooltiptoggle"));
    utilityButtons.tooltipLangButton.setTooltip(getTooltipText("tooltiplang"));

    // Botones de parámetros
    parameterButtons.bypassButton.setTooltip(getTooltipText("bypass"));

    
    // Update TODO button tooltips
    updateTodoButtonTexts();
    
    // Actualizar tooltip del transfer display
    transferDisplay.setHelpText(getTooltipText("transfer"));
    
    // Botones de utilidad - fila inferior
    utilityButtons.stereoLinkedButton.setTooltip(getTooltipText("link"));
    // CODE button removed
}

juce::String JCBCompressorAudioProcessorEditor::getTooltipText(const juce::String& key)
{
    if (currentLanguage == TooltipLanguage::Spanish)
    {
        // Spanish tooltips
        if (key == "title") return JUCE_UTF8("JCBCompressor: compresor de audio v1.0.0-alpha.1\nPlugin educativo open source\nClick para créditos");
        if (key == "thd") return JUCE_UTF8("THRESHOLD: nivel donde comienza la compresión\nSeñales sobre este nivel se comprimen\nRango: -60 a 0 dB | Por defecto: -18 dB");
        if (key == "ratio") return JUCE_UTF8("RATIO: cantidad de compresión aplicada\nRelación entrada/salida sobre el threshold\nRango: 1:1 a 20:1 | Por defecto: 4:1");
        if (key == "knee") return JUCE_UTF8("KNEE: suavidad de la transición en el threshold\nCrea una curva gradual en vez de ángulo duro\nRango: 0 a 30 dB | Por defecto: 0 dB");
        if (key == "drywet") return JUCE_UTF8("DRY/WET: mezcla final entre señal original y procesada\nControl de balance entrada/salida\nRango: 0 a 100% | Por defecto: 100%");
        if (key == "lookahead") return JUCE_UTF8("LOOK AHEAD: retardo para evitar overshooting\nReporta latencia al host (mínimo 1 muestra por Gen~)\nRango: 0 a 10 ms | Por defecto: 0 ms");
        if (key == "clip") return JUCE_UTF8("SOFT CLIP: limitador suave de salida\nPreviene saturación con distorsión armónica\nRango: 0/OFF a 1 | Por defecto: 0/OFF");
        if (key == "again") return JUCE_UTF8("AUTO GAIN: compensación parcial de ganancia\nRecupera ~70% del nivel, funciona junto con makeup\nRango: OFF/ON | Por defecto: OFF");
        if (key == "algo") return JUCE_UTF8("ALGORITHM: tipo de detector interpolado\nSliding: rápido y preciso | EXPO RMS: clásico | Slow RMS: suave\nRango continuo entre 0-2 | Por defecto: 1 (EXPO RMS)");
        if (key == "react") return JUCE_UTF8("REACT: respuesta del detector a transientes.\nValores bajos: agresivo | Valores altos: suave.\nRango: 0 a 100% | Por defecto: 0%");
        if (key == "attack") return JUCE_UTF8("ATTACK: tiempo para alcanzar máxima compresión\nVelocidad de respuesta del compresor\nRango: 0.1 a 100 ms | Por defecto: 5 ms");
        if (key == "release") return JUCE_UTF8("RELEASE: tiempo para volver sin compresión\nPermite valores extremos que distorsionan\nRango: 0.1 a 1000 ms | Por defecto: 30 ms");
        if (key == "autorel") return JUCE_UTF8("AUTO RELEASE: ajuste dinámico del release\nAdapta el tiempo según material de audio entrante\nRango: OFF/ON | Por defecto: OFF");
        if (key == "speed") return JUCE_UTF8("AR: activar auto release speed\nOFF = desactivado, ON = activado\nRango: OFF/ON | Por defecto: OFF");
        if (key == "delta") return JUCE_UTF8("DELTA: escucha la reducción aplicada\nParámetro global, no automatizable\nRango: OFF/ON | Por defecto: OFF");
        if (key == "parallel") return JUCE_UTF8("PARALLEL: suma señal sin comprimir\nCompresión paralela real PRE softclip sin pérdida de nivel\nRango: -inf a 0 dB | Por defecto: -inf dB");
        if (key == "trim") return JUCE_UTF8("TRIM INPUT: ganancia de entrada al compresor\nAjusta el nivel antes del procesamiento\nRango: -12 a +12 dB | Por defecto: 0 dB");
        if (key == "makeup") return JUCE_UTF8("MAKEUP: ganancia de compensación manual\nFunciona junto al Auto Gain\nRango: -12 a +12 dB | Por defecto: 0 dB");
        if (key == "sc") return JUCE_UTF8("SC: activa los filtros del sidechain interno.\nPermite filtrar la señal, tanto interna como externa, que controla el compresor.\nValor por defecto: OFF");
        if (key == "extkey") return JUCE_UTF8("SIDECHAIN: conmuta entre interno y externo\nUsa entrada auxiliar del plugin para controlar compresión\nRango: INT/EXT | Por defecto: INT");
        if (key == "solosc") return JUCE_UTF8("SOLO SC: escucha filtros sidechain int/ext\nParámetro global, no automatizable\nRango: OFF/ON | Por defecto: OFF");
        if (key == "hpf") return JUCE_UTF8("HPF: filtro pasa altos del sidechain\nFiltra frecuencias del detector de compresión\nRango: 20 a 20k Hz | Por defecto: 20 Hz");
        if (key == "lpf") return JUCE_UTF8("LPF: filtro pasa bajos del sidechain\nElimina frecuencias agudas del detector\nRango: 20 Hz a 20 kHz | Por defecto: 20 kHz");
        if (key == "save") return JUCE_UTF8("GUARDAR: guarda o sobrescribe preset\nGuarda nuevo o actualiza el preset actual");
        if (key == "saveas") return JUCE_UTF8("SAVE AS: guarda como nuevo preset\nCrea un nuevo archivo de preset con los valores actuales\nPermite crear presets personalizados");
        if (key == "delete") return JUCE_UTF8("BORRAR: elimina el preset seleccionado\nRequiere confirmación antes de borrar");
        if (key == "back") return JUCE_UTF8("ANTERIOR: selecciona el preset previo\nNavega hacia atrás en la lista de presets");
        if (key == "next") return JUCE_UTF8("SIGUIENTE: selecciona el próximo preset\nNavega hacia adelante en la lista de presets");
        if (key == "undo") return JUCE_UTF8("DESHACER: revierte el último cambio\nDeshace modificación realizada manualmente por el usuario\nHistorial: hasta 20 pasos");
        if (key == "redo") return JUCE_UTF8("REHACER: aplica el cambio deshecho\nRehace modificación manual previamente revertida\nHistorial: hasta 20 pasos");
        if (key == "resetgui") return JUCE_UTF8("SIZE: cicla entre tamaños de ventana\nActual → Máximo → Mínimo → Actual\nAjuste rápido del tamaño del plugin");
        if (key == "bypass") return JUCE_UTF8("BYPASS: desactiva el procesamiento del plugin\nParámetro global, no automatizable. Transición suave\nRango: OFF/ON | Por defecto: OFF");
        if (key == "graphics") return JUCE_UTF8("GRAPHICS: muestra envolventes en tiempo real\nVisualiza env entrada/salida e histograma reducción\nDesactivar mejora rendimiento en CPUs lentas");
        if (key == "zoom") return JUCE_UTF8("ZOOM: cicla entre vista normal y ampliada\nNormal: -72 a 0dB | x2: -48 a 0dB");
        if (key == "diagram") return JUCE_UTF8("DIAGRAM: muestra diagrama de bloques del procesador\nDespliega menú con código GenExpr por bloque para copiar");
        if (key == "transfer") return JUCE_UTF8("GRÁFICA: función de transferencia del compresor\nArrastra para modificar THD, Ratio y Knee\nClick derecho para opciones adicionales");
        if (key == "tooltiptoggle") return JUCE_UTF8("TOOLTIP: muestra/oculta los tooltips de ayuda\nActiva o desactiva las ventanas de ayuda emergentes");
        if (key == "tooltiplang") return JUCE_UTF8("IDIOMA: cambia entre español e inglés.\nAlterna el idioma de los tooltips.");
        if (key == "sctrim") return JUCE_UTF8("SC TRIM: ganancia entrada sidechain -12 a +12 dB\nAjusta nivel del sidechain externo\nPor defecto: 0 dB, se activa con EXT KEY");
        if (key == "link") return JUCE_UTF8("STEREO LINKED: siempre activo.\nEl plugin solo funciona en modo stereo linked.\nAmbos canales siempre están vinculados");
        if (key == "smooth") return JUCE_UTF8("SMOOTH: suavizado extra del detector de envolvente\nControla cantidad de suavizado en la detección\nRango: 0 (RAW) a 1 (SMOOTH) | Por defecto: 0");
    }
    else
    {
        // English tooltips
        if (key == "title") return "JCBCompressor: audio compressor v1.0.0-alpha.1\nOpen source educational plugin\nClick for credits";
        if (key == "thd") return "THRESHOLD: level where compression begins\nSignals above this level are compressed\nRange: -60 to 0 dB | Default: -18 dB";
        if (key == "ratio") return "RATIO: amount of compression applied\nInput/output relationship above threshold\nRange: 1:1 to 20:1 | Default: 4:1";
        if (key == "knee") return "KNEE: smoothness of the threshold transition\nCreates a gradual curve instead of hard angle\nRange: 0 to 30 dB | Default: 0 dB";
        if (key == "drywet") return "DRY/WET: final mix between original and processed signal\nInput/output balance control\nRange: 0 to 100% | Default: 100%";
        if (key == "lookahead") return "LOOK AHEAD: delay to prevent overshooting\nReports latency to host (minimum 1 sample due to Gen~)\nRange: 0 to 10 ms | Default: 0 ms";
        if (key == "clip") return "SOFT CLIP: soft output limiter\nPrevents clipping with harmonic distortion\nRange: 0/OFF to 1 | Default: 0/OFF";
        if (key == "again") return "AUTO GAIN: partial gain compensation\nRecovers ~70% of level, works together with makeup\nRange: OFF/ON | Default: OFF";
        if (key == "react") return "REACT: detector response to transients.\nLow values: aggressive | High values: smooth.\nRange: 0 to 100% | Default: 0%";
        if (key == "attack") return "ATTACK: time to reach maximum compression\nCompressor response speed\nRange: 0.1 to 100 ms | Default: 5 ms";
        if (key == "release") return "RELEASE: time to return uncompressed\nAllows extreme values for creative distortion\nRange: 0.1 to 1000 ms | Default: 30 ms";
        if (key == "autorel") return "AUTO RELEASE: dynamic release adjustment\nAdapts timing based on incoming audio material\nRange: OFF/ON | Default: OFF";
        if (key == "speed") return "AR: activate auto release speed\nOFF = disabled, ON = enabled\nRange: OFF/ON | Default: OFF";
        if (key == "delta") return "DELTA: listen to applied reduction\nGlobal parameter, non-automatable\nRange: OFF/ON | Default: OFF";
        if (key == "parallel") return "PARALLEL: adds uncompressed signal\nTrue parallel compression PRE softclip without level loss\nRange: -inf to 0 dB | Default: -inf dB";
        if (key == "trim") return "TRIM INPUT: compressor input gain\nAdjusts level before processing\nRange: -12 to +12 dB | Default: 0 dB";
        if (key == "makeup") return "MAKEUP: manual compensation gain\nWorks together with Auto Gain\nRange: -12 to +12 dB | Default: 0 dB";
        if (key == "sc") return "SC: activates internal sidechain filters.\nAllows filtering the signal, both internal and external, that controls the compressor.\nDefault: OFF";
        if (key == "extkey") return "SIDECHAIN: toggles between internal and external\nUses plugin auxiliary input to control compression\nRange: INT/EXT | Default: INT";
        if (key == "solosc") return "SOLO SC: listen to int/ext sidechain filters\nGlobal parameter, non-automatable\nRange: OFF/ON | Default: OFF";
        if (key == "hpf") return "HPF: sidechain high-pass filter\nFilters frequencies from compression detector\nRange: 20 to 20k Hz | Default: 20 Hz";
        if (key == "lpf") return "LPF: sidechain low-pass filter.\nRemoves treble frequencies from detector.\nRange: 20 Hz to 20 kHz | Default: 20 kHz";
        if (key == "save") return "SAVE: save or overwrite preset\nSave new or update current preset";
        if (key == "saveas") return "SAVE AS: save as new preset.\nCreates new preset file with current values.\nAllows creating custom presets";
        if (key == "delete") return "DELETE: remove selected preset\nRequires confirmation before deleting";
        if (key == "back") return "PREVIOUS: select previous preset\nNavigate backwards through preset list";
        if (key == "next") return "NEXT: select next preset\nNavigate forward through preset list";
        if (key == "undo") return "UNDO: revert last change\nUndo modification made manually by the user\nHistory: up to 20 steps";
        if (key == "redo") return "REDO: reapply undone change\nRedo manually made modification previously reverted\nHistory: up to 20 steps";
        if (key == "resetgui") return JUCE_UTF8("SIZE: cycles through window sizes\nCurrent → Maximum → Minimum → Current\nQuick plugin size adjustment");
        if (key == "bypass") return "BYPASS: disables plugin processing\nGlobal parameter, non-automatable. Smooth transition\nRange: OFF/ON | Default: OFF";
        if (key == "graphics") return "GRAPHICS: shows real-time envelopes\nDisplays input/output env and reduction histogram\nDisable to improve performance on slow CPUs";
        if (key == "zoom") return "ZOOM: cycles between normal and zoomed view\nNormal: -72 to 0dB | x2: -48 to 0dB";
        if (key == "diagram") return "DIAGRAM: shows processor block diagram\nDisplays menu with GenExpr code per block for copying";
        if (key == "transfer") return "GRAPH: compressor transfer function\nDrag to modify THD, Ratio and Knee\nRight click for additional options";
        if (key == "tooltiptoggle") return "TOOLTIP: show/hide help tooltips.\nEnables or disables popup help windows.";
        if (key == "tooltiplang") return "LANGUAGE: switch between Spanish and English.\nToggles tooltip language.";
        if (key == "sctrim") return "SC TRIM: sidechain input gain -12 to +12 dB\nAdjusts external sidechain level\nDefault: 0 dB, activated with EXT KEY";
        if (key == "link") return "STEREO LINKED: always active.\nPlugin only works in stereo linked mode.\nBoth channels are always linked";
        if (key == "smooth") return "SMOOTH: extra envelope detector smoothing\nControls smoothing amount applied to detection\nRange: 0 (RAW) to 1 (SMOOTH) | Default: 0";
        if (key == "algo") return "ALGORITHM: interpolated detector type\nSliding: fast and precise | EXPO RMS: classic | Slow RMS: smooth\nContinuous range 0–2 | Default: 1 (EXPO RMS)";    }
    
    return "";
}

//==============================================================================
// HELPERS DE UI
//==============================================================================
void JCBCompressorAudioProcessorEditor::applyAlphaToMainControls(float alpha)
{
    // Main knobs
    leftTopKnobs.thdSlider.setAlpha(alpha);
    leftTopKnobs.ratioSlider.setAlpha(alpha);
    leftTopKnobs.kneeSlider.setAlpha(alpha);
    
    leftBottomKnobs.drywetSlider.setAlpha(alpha);
    leftBottomKnobs.lookaheadSlider.setAlpha(alpha);
    leftBottomKnobs.clipSlider.setAlpha(alpha);
    leftTopKnobs.againSlider.setAlpha(alpha);
    
    rightTopControls.algoSlider.setAlpha(alpha);
    rightTopControls.reactSlider.setAlpha(alpha);
    
    rightBottomKnobs.atkSlider.setAlpha(alpha);
    rightBottomKnobs.relSlider.setAlpha(alpha);
    rightBottomKnobs.autorelButton.setAlpha(alpha);
    // speedButton removed
    leftBottomKnobs.parallSlider.setAlpha(alpha);
    rightTopControls.smoothSlider.setAlpha(alpha);
    
    // Transfer display
    transferDisplay.setAlpha(alpha);
    
    // Parameter buttons (not including BYPASS/DELTA which have special handling)
    parameterButtons.deltaButton.setAlpha(alpha);
    
    // Trim and makeup sliders
    trimSlider.setAlpha(alpha);
    makeupSlider.setAlpha(alpha);
}


// Nota: La funcionalidad de visualización de código ahora se maneja por CodeWindow desde DIAGRAM




//==============================================================================
// DIAGRAM Y CODE WINDOW
//==============================================================================

void JCBCompressorAudioProcessorEditor::showDiagram()
{
    // Thread-safe: usar MessageManager::callAsync para UI pesadas
    juce::Component::SafePointer<JCBCompressorAudioProcessorEditor> safeThis(this);
    
    juce::MessageManager::callAsync([safeThis]() {
        if (!safeThis) return;
        
        // Crear overlay si no existe
        if (!safeThis->diagramOverlay)
        {
            safeThis->diagramOverlay = std::make_unique<DiagramOverlay>(*safeThis);
        }
        
        // Configurar y mostrar
        safeThis->addChildComponent(safeThis->diagramOverlay.get());
        safeThis->diagramOverlay->setBounds(safeThis->getLocalBounds());
        safeThis->diagramOverlay->setVisible(true);
        safeThis->diagramOverlay->toFront(true);
        safeThis->centerButtons.diagramButton.setToggleState(true, juce::dontSendNotification);
    });
}

void JCBCompressorAudioProcessorEditor::hideDiagram()
{
    if (diagramOverlay && diagramOverlay->isVisible())
    {
        diagramOverlay->setVisible(false);
        removeChildComponent(diagramOverlay.get());
    }
    centerButtons.diagramButton.setToggleState(false, juce::dontSendNotification);
}

void JCBCompressorAudioProcessorEditor::hideCodeWindow()
{
    if (codeWindow && codeWindow->isVisible())
    {
        codeWindow->setVisible(false);
        removeChildComponent(codeWindow.get());
    }
}

juce::String JCBCompressorAudioProcessorEditor::loadCodeFromFile(const juce::String& blockName)
{
    // Thread-safe: usar cache pre-cargado en lugar de leer BinaryData cada vez
    if (codeContentCacheInitialized && codeContentCache.find(blockName) != codeContentCache.end()) {
        return codeContentCache[blockName];
    }
    
    // Fallback si el cache no está inicializado o no se encuentra el bloque
    return "// Code for " + blockName + "\n\n// Content not found in cache.\n// Please report this issue.\n\n// Basic functionality:\n" + getBasicBlockDescription(blockName);
}

void JCBCompressorAudioProcessorEditor::initializeCodeContentCache()
{
    if (codeContentCacheInitialized) return;
    
    // Pre-cargar todo el contenido de código al inicializar para thread safety
    struct CodeMapping {
        juce::String blockName;
        const char* binaryData;
        int dataSize;
    };
    
    // Lista de todos los mappings con bloques separados corregidos
    std::vector<CodeMapping> mappings = {
        {"TRIM IN", BinaryData::InputTrim_txt, BinaryData::InputTrim_txtSize},
        {"TRIM SC", BinaryData::InputTrim_txt, BinaryData::InputTrim_txtSize},
        {"LOOKAHEAD", BinaryData::GainCore_txt, BinaryData::GainCore_txtSize},
        {"FILTERS", BinaryData::Filters_txt, BinaryData::Filters_txtSize},
        {"DETECTOR", BinaryData::Detector_txt, BinaryData::Detector_txtSize},
        {"GAIN CALC", BinaryData::GainCalc_txt, BinaryData::GainCalc_txtSize},
        {"GAIN CORE", BinaryData::GainCore_txt, BinaryData::GainCore_txtSize},
        {"MAKEUP", BinaryData::Output_txt, BinaryData::Output_txtSize},
        {"PARALLEL", BinaryData::Output_txt, BinaryData::Output_txtSize},
        {"OUTPUT", BinaryData::Output_txt, BinaryData::Output_txtSize},
        {"DELTA", BinaryData::Output_txt, BinaryData::Output_txtSize}
    };
    
    // Cargar todo en cache
    for (const auto& mapping : mappings) {
        if (mapping.binaryData != nullptr && mapping.dataSize > 0) {
            juce::String content = juce::String::createStringFromData(mapping.binaryData, mapping.dataSize);
            if (content.isNotEmpty()) {
                codeContentCache[mapping.blockName] = content;
            } else {
                // Fallback si falla la carga
                codeContentCache[mapping.blockName] = "// Code for " + mapping.blockName + 
                    "\n\n// Error cargando contenido\n\n// Funcionalidad básica:\n" + 
                    getBasicBlockDescription(mapping.blockName);
            }
        }
    }
    
    codeContentCacheInitialized = true;
}

juce::String JCBCompressorAudioProcessorEditor::getBasicBlockDescription(const juce::String& blockName)
{
    if (blockName == "TRIM IN") {
        return "// Input trim gain applied to main signal\ninput_trimmed = input * dbtoa(trim_db);";
    } else if (blockName == "TRIM SC") {
        return "// Sidechain trim gain applied to sidechain signal\nsc_trimmed = sidechain * dbtoa(sc_trim_db);";
    } else if (blockName == "FILTERS") {
        return "// HPF and LPF filters for sidechain processing\nfiltered_sc = lpf(hpf(sc_signal, hpf_freq), lpf_freq);";
    } else if (blockName == "DETECTOR") {
        return "// Level detector with peak/RMS algorithm selection\nlevel = peak_rms_detector(filtered_sc, react_param, algo_param);";
    } else if (blockName == "GAIN CALC") {
        return "// Compressor gain calculation with ratio and knee\ngain_reduction = compressor_curve(level, threshold, ratio, knee);";
    } else if (blockName == "APPLY") {
        return "// Apply gain reduction to main signal\ncompressed = input_trimmed * gain_reduction;";
    } else if (blockName == "MAKEUP") {
        return "// Apply makeup gain to compressed signal\nwith_makeup = compressed * dbtoa(makeup_db);";
    } else if (blockName == "PARALLEL") {
        return "// Parallel compression mixing\noutput = mix(input_trimmed, with_makeup, parallel_amount);";
    } else if (blockName == "OUTPUT") {
        return "// Final output with any additional processing\nfinal_output = output_stage(parallel_output);";
    } else {
        return "// Generic Gen~ block processing\n// See Max patch for detailed implementation";
    }
}

// Función helper para obtener índice de parámetro por ID (robusta, compatible con el futuro)
int JCBCompressorAudioProcessorEditor::getParameterIndexByID(const juce::String& parameterID)
{
    if (parameterID.isEmpty())  // Comprobación extra para evitar pasar string vacío al loop
        return -1;

    auto& params = processor.getParameters();
    for (int i = 0; i < params.size(); ++i) {
        if (auto* param = dynamic_cast<juce::AudioProcessorParameterWithID*>(params[i])) {
            if (param->paramID == parameterID) {
                return i;
            }
        }
    }
    // Retorna el índice del parámetro o -1 si no se encuentra
    return -1; // Parámetro no encontrado
}

int JCBCompressorAudioProcessorEditor::getControlParameterIndex(juce::Component& control)
{
    // Mapear componentes UI a sus IDs de parámetro (robusta, compatible con el futuro)
    // Retornar -1 para componentes que no representan parámetros automatizables
    
    juce::String parameterID;
    
    // Perillas Superiores Izquierdas (threshold, ratio, knee)
    if (&control == &leftTopKnobs.thdSlider) parameterID = "b_THD";
    else if (&control == &leftTopKnobs.ratioSlider) parameterID = "c_RATIO";
    else if (&control == &leftTopKnobs.kneeSlider) parameterID = "h_KNEE";
    
    // Perillas Inferiores Izquierdas (drywet, lookahead, clip, autogain)
    else if (&control == &leftBottomKnobs.drywetSlider) parameterID = "o_DRYWET";
    else if (&control == &leftBottomKnobs.lookaheadSlider) parameterID = "n_LOOKAHEAD";
    else if (&control == &leftBottomKnobs.clipSlider) parameterID = "u_SOFTCLIP";  // Corregido: ahora usa mapeo dinámico
    else if (&control == &leftTopKnobs.againSlider) parameterID = "x_AUTOGAIN";
    
    // Controles Superiores Derechos (algorithm, speed)
    else if (&control == &rightTopControls.algoSlider) parameterID = "f_ALGO";
    else if (&control == &rightTopControls.reactSlider) parameterID = "g_REACT";
    
    // Perillas Inferiores Derechas (attack, release, speed, parallel, smooth)
    else if (&control == &rightBottomKnobs.atkSlider) parameterID = "d_ATK";
    else if (&control == &rightBottomKnobs.relSlider) parameterID = "e_REL";
    // speedButton removido - parámetro t_AUTORELEASESPEED ya no existe
    else if (&control == &leftBottomKnobs.parallSlider) parameterID = "w_PARALLEL";
    else if (&control == &rightTopControls.smoothSlider) parameterID = "z_SMOOTH";
    
    // Controles de Sidechain (filters, key)
    else if (&control == &sidechainControls.hpfSlider) parameterID = "j_HPF";
    else if (&control == &sidechainControls.lpfSlider) parameterID = "k_LPF";
    else if (&control == &sidechainControls.keyButton) parameterID = "r_KEY";
    else if (&control == &sidechainControls.hpfOrderButton) parameterID = "j_HPFORDER";
    else if (&control == &sidechainControls.lpfOrderButton) parameterID = "k_LPFORDER";
    
    // Sliders de Trim
    else if (&control == &trimSlider) parameterID = "a_TRIM";
    else if (&control == &makeupSlider) parameterID = "i_MAKEUP";
    else if (&control == &scTrimSlider) parameterID = "y_SCTRIM";
    
    // Botones Automatizables
    else if (&control == &rightBottomKnobs.autorelButton) parameterID = "s_AUTORELEASE";
    else if (&control == &sidechainControls.scButton) parameterID = "l_SC";
    
    // Parámetros no automatizables (retornar -1)
    // Estos son parámetros globales/utility que no deberían mostrar carriles de automatización
    else if (&control == &sidechainControls.soloScButton) return -1;  // m_SOLOSC (no automatizable)
    else if (&control == &parameterButtons.deltaButton) return -1;      // v_DELTA (no automatizable)
    else if (&control == &parameterButtons.bypassButton) return -1;     // p_BYPASS (no automatizable)
    
    // Obtener índice dinámico de parámetro
    if (parameterID.isNotEmpty()) {
        return getParameterIndexByID(parameterID);
    }
    
    // Cualquier otro componente que no representa un parámetro
    return -1;
}

void JCBCompressorAudioProcessorEditor::applyMeterDecayIfNeeded()
{
    // Sistema universal de decay para todos los DAWs
    // Basado en la solución recomendada en los foros de JUCE
    // Implementación profesional: decay siempre activo
    
    // Los medidores decaen naturalmente por inactividad
    // Los repaints se manejan automáticamente por el timer normal del editor
}