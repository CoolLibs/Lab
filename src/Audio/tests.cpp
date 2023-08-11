#include <cstdlib>
#include <iostream>
#include <cmath>
#include <exception>

#include <Cool/Path/Path.h>

#include "libnyquist/Common.h"
#include "libnyquist/Decoders.h"
#include "dj_fft/dj_fft.h"
#include "RtAudioWrapper/rtaudio/RtAudio.h"

// Two-channel wave generator.
auto sound_generator( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
			double streamTime, RtAudioStreamStatus status, void *userData ) -> int
	{
	static float freq = 880.0;
	static double x = 0;
	unsigned int i, j;
	double *buffer = (double *) outputBuffer;
	double *lastValues = (double *) userData;
	if ( status )
		std::cout << "Stream underflow detected!" << std::endl;
	// Write interleaved audio data.
	for ( i=0; i<nBufferFrames; i++ ) {
		for ( j=0; j<2; j++ ) {
		*buffer++ = lastValues[j];
		lastValues[j] = 0.3*sin(2*M_PI*freq*x);
		}
		x+=1.f/44100.f;
	}
	if (freq > 200.0) {
		x=freq*x/(freq-0.1); // "Connects" the successive sinusoids at the same value
		freq -= 0.1;
	}
	std::cout << freq << "\n";
	return 0;

}

#if LAB_ENABLE_TESTS
#include <doctest/doctest.h>


TEST_CASE("libnyquist test : Opening a .wav file and reading its content in a struct")
{
	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/10-1000-10000-20000.wav");

	CHECK(audioData->channelCount == 1);
	CHECK(audioData->sampleRate == 41000);
	CHECK(audioData->lengthSeconds == 4);
	CHECK(audioData->frameSize == 2);
	CHECK(audioData->samples.size() == 164000);
	CHECK(audioData->sourceFormat == 2);
}

TEST_CASE("libnyquist test : Opening a .mp3 file in a struct, testing the Left channel signal values"){
	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");
	std::ifstream in(Cool::Path::root() / "tests/res/audio/Orfeo.values.left");
	std::istream_iterator<float> start(in), end;
	std::vector<float> leftChan(start, end);

	for (size_t i=0; i<audioData->sampleRate*10; i++){
		CHECK(audioData->samples[i*2] ==  doctest::Approx(leftChan[i]));
	}

}

TEST_CASE("libnyquist test : Opening a .mp3 file in a struct, testing the Left channel signal values"){
	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");
	std::ifstream in(Cool::Path::root() / "tests/res/audio/Orfeo.values.right");
	std::istream_iterator<float> start(in), end;
	std::vector<float> leftChan(start, end);

	for (size_t i=0; i<audioData->sampleRate*10; i++){
		CHECK(audioData->samples[i*2+1] ==  doctest::Approx(leftChan[i]));
	}

}


TEST_CASE("dj_fft test : Opening a .wav file, reading its content in a struct, computing the FFT on it")
{

	// Load the audio file
	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/10-1000-10000-20000.wav");

	size_t N = 65536; // input size
	std::vector<std::complex<float>> myData; // input data

	// Prepare data, using a loop because the source vector is of different size
	for (size_t i = 0; i < N; i++) {
		myData.push_back(std::complex<float>(audioData->samples[i])); // set element (i, j)
	}

	// compute forward 2D FFT
	auto fftData = dj::fft1d(myData, dj::fft_dir::DIR_FWD);

	CHECK(fftData.size() == 65536);
	CHECK(std::abs(fftData[16]) == doctest::Approx(38.669884));
	CHECK(std::abs(fftData[1598]) == doctest::Approx(27.571739));
	CHECK(std::abs(fftData[1599]) == doctest::Approx(21.486385));
	CHECK(std::abs(fftData[15984]) == doctest::Approx(29.728823));
	CHECK(std::abs(fftData[15985]) == doctest::Approx(18.963114));
	CHECK(std::abs(fftData[31968]) == doctest::Approx(10.106586));
	CHECK(std::abs(fftData[31969]) == doctest::Approx(35.716843));
	CHECK(std::abs(fftData[33567]) == doctest::Approx(35.765961));
	CHECK(std::abs(fftData[33568]) == doctest::Approx(10.012813));
	CHECK(std::abs(fftData[49551]) == doctest::Approx(19.058596));
	CHECK(std::abs(fftData[49552]) == doctest::Approx(29.651283));
	CHECK(std::abs(fftData[63937]) == doctest::Approx(21.579424));
	CHECK(std::abs(fftData[63938]) == doctest::Approx(27.487740));
	CHECK(std::abs(fftData[65520]) == doctest::Approx(38.676113));

}


// TEST_CASE("dj_fft speed test : Opening an audio file and running a FFT on a sliding frame")
// {

// 	int framesize = 4096;

// 	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
// 	nqr::NyquistIO io;
// 	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");

// 	for (size_t i = 0; i < 25 /*audioData->frameRate*10*/; i++) {
// 		std::cout << audioData->samples[i*2] << "\n";
// 	}

// 	std::vector<std::complex<float>> myData; // input data
// 	for (size_t i = 0; i < audioData->samples.size(); i++) {
// 		myData.push_back(std::complex<float>(audioData->samples[i])); // set element (i, j)
// 	}
// 	for (size_t i = 0; i < audioData->samples.size()-framesize; i+=framesize){
// 		std::vector tmp(myData.begin()+i, myData.begin()+i+framesize);
// 		auto fftData = dj::fft1d(tmp, dj::fft_dir::DIR_FWD);
// 	// 	// for (size_t j = 0; j<63; j++){
// 	// 	// 	int s = std::round(std::abs(fftData[j*16])*100);
// 	// 	// 	for (size_t k=0; k<s; k++){
// 	// 	// 		std::cout<< " ";
// 	// 	// 	}
// 	// 	// 	std::cout << "#\n";
// 	// 	// }
// 	// 	// // std::cout << "\n\n";
// 	}
// }

TEST_CASE("RtAudio test")
{

// -----------------------------------------------------------

	int framesize = 4096;

	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");


	int count;
	// int channels = 2;
	// int sampleRate = 44100;
	// int bufferSize = 256;  // 256 sample frames
	// int nBuffers = 4;      // number of internal buffers used by device
	// float *buffer;
	// int device = 0;        // 0 indicates the default or first available device
	// RtAudio *audio = 0;

	RtAudio dac;
	if ( dac.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 256; // 256 sample frames
	// Open a stream during RtAudio instantiation
	double data[2] = {0, 0};
	RtAudioErrorType err;
	err = dac.openStream(&parameters,
						NULL,
						RTAUDIO_FLOAT64,
						sampleRate,
						&bufferFrames,
						&sound_generator,
						(void *)audioData->samples.data() );

	std::cout << "Opening stream : " << err << "\n";
	char input = '\n';
	int playing = 0;
	while (input == '\n') {
		if (!playing) {
			err = dac.startStream();
			std::cout << "Starting stream : " << err << "\n";
			std::cout << "\nPlaying ... press <enter> to stop.\n";
			playing = 1;
		} else {
			err = dac.stopStream();
			std::cout << "Stoping stream : " << err << "\n";
			std::cout << "\nStopping ... press <enter> to play.\n";
			playing = 0;
		}
		std::cin.get( input );
	}

	if ( dac.isStreamOpen() ) dac.closeStream();

}

#endif
