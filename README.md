## build
- init submodules
- build projuicer
- open project, save to IDE
- build

## Submodules
- `git submodule update --init --recursive` to initialize on cloning
- `git submodule update --recursive --remote` to pull submodules
  - Note: it may be wiser to use Projucer's update functionality instead to not get in-between-releases commits to JUCE
  - After updating JUCE, Projucer should be rebuilt from `./JUCE/extras/Projucer/Builds`

## Build (CMake)
- `cmake -E make_directory ./Builds/CMake`
- `cd Builds/CMake`
- `cmake ../../`
- `cmake --build ./ --config Release`: Artefacts ins Builds/CMake/Builds/CMake