//
// Welcome to the GDFT file: this is the core of Sensory Bridge.
// This is where time-domain audio is converted into a
// frequency-domain representation for your viewing pleasure. This
// file doesn't actually contain LED code. That's lightshow_modes.h,
// which references values calculated here on each frame.
//
// It's not FFT. It's a Goertzel-based Discrete Fourier Transform,
// or what I'm calling a "GDFT". The Goertzel algorithm detects the
// presence/magnitude of a single frequency in a signal, and in
// this case I'm running 64 instances of Goertzel at once on the
// 64 frequencies set in constants.h.
//
// https://en.wikipedia.org/wiki/Goertzel_algorithm
//
// This is slightly slower than FFT, but allows for two really
// neat tricks:
//
// 1. I can scale the frequency range however I'd like.
//
//    With an FFT of size 128 at a sample rate of 10KHz, you'd get
//    back 64 bins between 0Hz (useless) and 5KHz. These aren't
//    evenly spaced bins though, with frequency increasing by a
//    linear amount between bins, unlike the keys of a piano where
//    every 12th note is doubled in frequency.
//
//    By running Goertzel's algorithm 64 times in parallel I can
//    choose my own bin spacing, and in this case Sensory Bridge is
//    watching the upper 64 keys of an 88-key piano's frequency
//    range: 110Hz to 4186Hz (by default).
//
//    This means that every "half-step" up in pitch in an
//    instrument is it's own distinct frequency bin, with only
//    a small amount of spectral leakage.
//
// 2. Each bin can get their own settings that are best for it
//
//    I can individually control the window length (which doesn't
//    have to be a power of two like FFT) of each bin, to keep
//    a good balance between temporal and pitch resolution across
//    the frequency range. This also helps with speed, as the
//    higher frequencies require smaller window lengths and thus
//    have less work to do solving the magnitudes than the lower
//    frequencies.
//
//
// This GDFT method, which operates on a sliding window with 256
// new samples per frame, (i2s_audio.h) combined with a shitload
// of interesting post-processing methods I've documented below
// are what's behind the eye-catching shows on Sensory Bridge!
//
// If you like that I've shared this code, *please* support my work
// by purchasing genuine hardware or telling your friends about it!
//
// https://github.com/sponsors/connornishijima
// LIXIE LABS

// Obscure audio magic happens here
void IRAM_ATTR process_GDFT() {
  float MOOD_VAL = CONFIG.MOOD;
  if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_BLOOM) {
    MOOD_VAL = 1.0;
  }

  static bool interlace_flip = false;
  interlace_flip = !interlace_flip;  // Switch field every frame on lower notes to save execution time

  // Reset magnitude caps every frame
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    max_mags[i] = 0.0;  // Higher than the average noise floor
  }

  // Increment spectrogram history index
  spectrogram_history_index++;
  if (spectrogram_history_index >= spectrogram_history_length) {
    spectrogram_history_index = 0;  // wrap to index zero at end
  }

  // Run GDFT (Goertzel-based Discrete Fourier Transform) with 64 frequencies
  // Fixed-point code adapted from example here: https://sourceforge.net/p/freetel/code/HEAD/tree/misc/goertzal/goertzal.c
  for (uint16_t i = 0; i < NUM_FREQS; i++) {  // Run 64 times
    int32_t q0, q1, q2;
    int64_t mult;

    q1 = 0;
    q2 = 0;

    float window_pos = 0.0;
    for (uint16_t n = 0; n < frequencies[i].block_size; n++) {  // Run Goertzel for "block_size" iterations    
      int32_t sample = 0;
      //sample = ((int32_t)sample_window[SAMPLE_HISTORY_LENGTH - 1 - n] * (int32_t)window_lookup[uint16_t(window_pos)]) >> 16;
      sample = (int32_t)sample_window[SAMPLE_HISTORY_LENGTH - 1 - n];
      mult = (int32_t)frequencies[i].coeff_q14 * (int32_t)q1;
      q0 = (sample >> 6) + (mult >> 14) - q2;
      q2 = q1;
      q1 = q0;

      window_pos += frequencies[i].window_mult;
    }

    mult = (int32_t)frequencies[i].coeff_q14 * (int32_t)q1;
    magnitudes[i] = q2 * q2 + q1 * q1 - ((int32_t)(mult >> 14)) * q2;  // Calculate raw magnitudes

    if (magnitudes[i] < 0) {  // Prevent negative values
      magnitudes[i] = 0;
    }

    magnitudes[i] = sqrt(magnitudes[i]);

    // Normalizing the magnitude
    float normalized_magnitude = magnitudes[i] / float(frequencies[i].block_size / 2.0);
    magnitudes_normalized[i] = normalized_magnitude;

    if (frequencies[i].target_freq == 440.0) {
      //USBSerial.println(magnitudes_normalized[i]);
    }

    magnitudes_normalized_avg[i] = (magnitudes_normalized[i] * 0.3) + (magnitudes_normalized_avg[i] * (1.0 - 0.3));
  }

  // Gather noise data if noise_complete == false
  if (noise_complete == false) {
    for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
      if (magnitudes_normalized_avg[i] > noise_samples[i]) {
        noise_samples[i] = magnitudes_normalized_avg[i];
      }
    }
    noise_iterations++;
    if (noise_iterations >= 256) {  // Calibration complete
      noise_complete = true;
      USBSerial.println("NOISE CAL COMPLETE");
      CONFIG.DC_OFFSET = dc_offset_sum / 256.0;  // Calculate average DC offset and store it
      save_ambient_noise_calibration();           // Save results to noise_cal.bin
      save_config();                              // Save config to config.bin
    }
  }

  // Apply noise reduction data
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    if (noise_complete == true) {
      magnitudes_normalized_avg[i] -= float(noise_samples[i] * SQ15x16(1.5));  // Treat noise 1.5x louder than calibration
      if (magnitudes_normalized_avg[i] < 0.0) {
        magnitudes_normalized_avg[i] = 0.0;
      }
    }
  }

  memcpy(magnitudes_final, magnitudes_normalized_avg, sizeof(float) * NUM_FREQS);
  low_pass_array(magnitudes_final, magnitudes_last, NUM_FREQS, SYSTEM_FPS, 1.0 + (10.0 * MOOD_VAL));
  memcpy(magnitudes_last, magnitudes_final, sizeof(float) * NUM_FREQS);

  /*

  // When enabled, streams magnitudes[] array over Serial
  if (stream_magnitudes == true) {
    if (serial_iter >= 2) {  // Don't print every frame
      serial_iter = 0;
      USBSerial.print("sbs((magnitudes=");
      for (uint16_t i = 0; i < NUM_FREQS; i++) {
        USBSerial.print(uint32_t(magnitudes[i]));
        if (i < NUM_FREQS - 1) {
          USBSerial.print(',');
        }
      }
      USBSerial.println("))");
    }
  }
  */

  static SQ15x16 goertzel_max_value = 0.0001;
  SQ15x16 max_value = 0.00001;

  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {  // 64 freqs
    if (magnitudes_final[i] > max_value) {
      max_value = magnitudes_final[i];
    }
  }

  max_value *= SQ15x16(0.995);

  if (max_value > goertzel_max_value) {
    SQ15x16 delta = max_value - goertzel_max_value;
    goertzel_max_value += delta * SQ15x16(0.0050);
  } else if (goertzel_max_value > max_value) {
    SQ15x16 delta = goertzel_max_value - max_value;
    goertzel_max_value -= delta * SQ15x16(0.0025);
  }

  if (goertzel_max_value < 4.0) {
    goertzel_max_value = 4.0;
  }

  // Normalize output using goertzel_max_val
  SQ15x16 multiplier = SQ15x16(1.0) / goertzel_max_value;
  //multiplier += SQ15x16(0.10);  // Overshoot by 10%

  for (uint16_t i = 0; i < NUM_FREQS; i += 1) {
    spectrogram[i] = magnitudes_final[i] * multiplier;
  }
}

void calculate_novelty(uint32_t t_now) {
  static uint32_t iter = 0;
  iter++;

  // Calculate "novelty" (positive change) in this moment by marking the positive changes from the previous frame
  // Sum in a column-wise fashion into novelty_now
  SQ15x16 novelty_now = 0.0;
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    int16_t rounded_index = spectral_history_index - 1;
    while (rounded_index < 0) {
      rounded_index += SPECTRAL_HISTORY_LENGTH;
    }
    SQ15x16 novelty_bin = spectrogram[i] - spectral_history[rounded_index][i];

    if (novelty_bin < 0.0) {
      novelty_bin = 0.0;
    }

    novelty_now += novelty_bin;
  }
  novelty_now /= NUM_FREQS;  // Normalize result

  // Append current spectrogram to last place in history:
  for (uint16_t b = 0; b < NUM_FREQS; b += 8) {
    spectral_history[spectral_history_index][b + 0] = spectrogram[b + 0];
    spectral_history[spectral_history_index][b + 1] = spectrogram[b + 1];
    spectral_history[spectral_history_index][b + 2] = spectrogram[b + 2];
    spectral_history[spectral_history_index][b + 3] = spectrogram[b + 3];
    spectral_history[spectral_history_index][b + 4] = spectrogram[b + 4];
    spectral_history[spectral_history_index][b + 5] = spectrogram[b + 5];
    spectral_history[spectral_history_index][b + 6] = spectrogram[b + 6];
    spectral_history[spectral_history_index][b + 7] = spectrogram[b + 7];
  }

  // Append new novelty measurement to novelty curve history
  novelty_curve[spectral_history_index] = sqrt(float(novelty_now));

  spectral_history_index++;
  if (spectral_history_index >= SPECTRAL_HISTORY_LENGTH) {
    spectral_history_index -= SPECTRAL_HISTORY_LENGTH;
  }
}
