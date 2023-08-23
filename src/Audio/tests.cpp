#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cmath>
#include <exception>

#include <Cool/Path/Path.h>

#include "libnyquist/Common.h"
#include "libnyquist/Decoders.h"
#include "dj_fft/dj_fft.h"
#include "RtAudioWrapper/rtaudio/RtAudio.h"
#include "RtAudioWrapper/Wrapper.h"

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

TEST_CASE("libnyquist test : Opening a .mp3 file in a struct, testing the Right channel signal values"){
	std::shared_ptr<nqr::AudioData> audioData = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(audioData.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");
	std::ifstream in(Cool::Path::root() / "tests/res/audio/Orfeo.values.right");
	std::istream_iterator<float> start(in), end;
	std::vector<float> rightChan(start, end);

	for (size_t i=0; i<audioData->sampleRate*10; i++){
		CHECK(audioData->samples[i*2+1] ==  doctest::Approx(rightChan[i]));
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

TEST_CASE("Test to open an audio stream twice on the same Player"){

	std::shared_ptr<nqr::AudioData> file = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(file.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");

	RtAudioW::Player audio;
	audio.open(file->samples);
	audio.open(file->samples);
	audio.close();
}


TEST_CASE("RtAudioW test"){

	std::shared_ptr<nqr::AudioData> file = std::make_shared<nqr::AudioData>();
	nqr::NyquistIO io;
	io.Load(file.get(), Cool::Path::root() / "tests/res/audio/Monteverdi - L'Orfeo, Toccata.mp3");

	RtAudioW::Player audio;
	audio.open(file->samples);
	audio.play();

	std::cout << "Started playing\n";
	while (audio.get_cursor() < audio.get_data_length()-44100){
		if (audio.get_cursor() > 441000 && audio.get_cursor() < 450000)  { std::cout << "Seeking to : " << audio.seek(1323000/(2*44100.f)) << "\n"; }
		if (audio.get_cursor() > 2734200 && audio.get_cursor() < 2744200){ std::cout << "Seeking to : " << audio.seek(3528000/(2*44100.f)) << "\n"; }
		if (audio.get_cursor() > 5292000 && audio.get_cursor() < 5303000){ std::cout << "Seeking to : " << audio.seek(7938000/(2*44100.f)) << "\n"; }
	}
	audio.pause();
	audio.close();

}


#endif
