
You have access to a few information about the current audio: `float _audio_volume`, `sampler1D _audio_spectrum` and `sampler1D _audio_waveform`.

Also, the name of your node should contain `(Audio)`: this both helps the user find your node when they are looking for audio-reactive effects, and we use this internally to know if we should add a widget that opens the Audio config window.