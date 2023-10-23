
You have access to a few information about the current audio: `_volume` and `_fft`. <!-- TODO(Audio) Explain more once it's implemented -->

Also, the name of your node should contain `(Audio)`: this both helps the user find your node when they are looking for audio-reactive effects, and we use this internally to know if we should add a widget that opens the Audio config window.