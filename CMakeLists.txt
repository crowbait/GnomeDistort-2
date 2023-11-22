# based on https://github.com/ffAudio/Frequalizer/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(GNOME_DISTORT_2 VERSION 0.0.1)
add_subdirectory(JUCE)

juce_add_plugin(GnomeDistort2
                VERSION "0.0.1"
                FORMATS AU VST3
                COMPANY_NAME "Crowbait"
                PLUGIN_MANUFACTURER_CODE "CrBt"
                PLUGIN_CODE "GDs2"
                IS_SYNTH FALSE
                NEEDS_MIDI_INPUT FALSE
                NEEDS_MIDI_OUTPUT FALSE
                IS_MIDI_EFFECT FALSE
                EDITOR_WANTS_KEYBOARD_FOCUS FALSE
                COPY_PLUGIN_AFTER_BUILD FALSE
                VST3_CATEGORIES Fx Distortion
                AU_MAIN_TYPE "kAudioUnitType_Effect"
                COMPANY_WEBSITE "crowbait.de"
                COMPANY_EMAIL "contact@crowbait.de"
                BUNDLE_ID "de.crowbait.gnomedistort2"
                PLUGIN_NAME "GnomeDistort 2"
                PRODUCT_NAME "GnomeDistort 2"
)

juce_generate_juce_header(GnomeDistort2)

target_sources(GnomeDistort2
  PRIVATE
    Source/ConstOptions.h
    Source/Parameters.h
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/Processing.h
    Source/Processing.cpp
    Source/WaveshaperFunctions.h
    Source/UI/BandControls.h
    Source/UI/BandControls.cpp
    Source/UI/Display.h
    Source/UI/PostBandControls.h
    Source/UI/PreBandControls.h
    Source/UI/UIConsts.h
    Source/UI/Controls/ComboBox.h
    Source/UI/Controls/DisplayGraph.h
    Source/UI/Controls/DisplayGraph.cpp
    Source/UI/Controls/SliderLabeledValue.h
    Source/UI/Controls/SliderLabeledValue.cpp
)

target_compile_definitions(GnomeDistort2
  PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    )

juce_add_binary_data(GnomeDistort2_binary SOURCES 
  Assets/knob_overlay_128.png
)
target_compile_options(GnomeDistort2_binary PRIVATE -fPIC)

target_link_libraries(GnomeDistort2 PRIVATE juce::juce_recommended_warning_flags juce::juce_recommended_config_flags juce::juce_recommended_lto_flags)
target_link_libraries(GnomeDistort2 
  PRIVATE 
    juce::juce_dsp
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    GnomeDistort2_binary
)

# if (APPLE)
#     set(COPY_FOLDER ${CMAKE_SOURCE_DIR}/Builds/CMake/MacOS)
# elseif(WIN32)
#     set(COPY_FOLDER ${CMAKE_SOURCE_DIR}/Builds/CMake)
# endif()
# 
# foreach(FORMAT ${FORMATS})
#     get_target_property(ARTEFACTS_DIR frequalizer_${FORMAT} LIBRARY_OUTPUT_DIRECTORY)
#     add_custom_command(TARGET frequalizer_${FORMAT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${ARTEFACTS_DIR} ${COPY_FOLDER})
# endforeach()