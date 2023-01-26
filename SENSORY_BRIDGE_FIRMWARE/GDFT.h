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
  // Reset magnitude caps every frame
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    max_mags[i] = CONFIG.MAGNITUDE_FLOOR;  // Higher than the average noise floor
  }

  // Increment spectrogram history index
  spectrogram_history_index++;
  if (spectrogram_history_index >= spectrogram_history_length) {
    spectrogram_history_index = 0;  // wrap to index zero at end
  }

  // Run GDFT (Goertzel-based Discrete Fourier Transform) with 64 frequencies
  // Fixed-point code adapted from example here: https://sourceforge.net/p/freetel/code/HEAD/tree/misc/goertzal/goertzal.c
  for (uint16_t i = 0; i < NUM_FREQS; i++) { // Run 64 times
    int32_t q0, q1, q2;
    int64_t mult;
    
    q1 = 0;
    q2 = 0;

    for (uint16_t n = 0; n < frequencies[i].block_size; n++) { // Run Goertzel for "block_size" iterations
      mult = (int64_t)frequencies[i].coeff_q14 * (int64_t)q1;
      q0 = (sample_window[SAMPLE_HISTORY_LENGTH - 1 - n] >> 6) + (mult >> 14) - q2;
      q2 = q1;
      q1 = q0;
    }

    mult = (int64_t)frequencies[i].coeff_q14 * (int64_t)q1;
    magnitudes[i] = q2 * q2 + q1 * q1 - ((int32_t)(mult >> 14)) * q2; // Calculate raw magnitudes

    magnitudes[i] *= float(frequencies[i].block_size_recip); // Normalize output

    if (magnitudes[i] < 0.0) { // Prevent negative values
      magnitudes[i] = 0.0;
    }
  }

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

  // Gather noise data if noise_complete == false
  if (noise_complete == false) {
    for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
      if (magnitudes[i] > noise_samples[i]) { 
        noise_samples[i] = magnitudes[i];
      }
    }
    noise_iterations++;
    if (noise_iterations >= 256) { // Calibration complete
      noise_complete = true;
      USBSerial.println("NOISE CAL COMPLETE");
      CONFIG.DC_OFFSET = dc_offset_sum / 256.0; // Calculate average DC offset and store it
      save_ambient_noise_calibration(); // Save results to noise_cal.bin
      save_config(); // Save config to config.bin
    }
  }

  // Apply noise reduction data, estimate max values
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    if (noise_complete == true) {
      magnitudes[i] -= noise_samples[i] * 1.5; // Treat noise 1.5x louder than calibration
      if (magnitudes[i] < 0.0) {
        magnitudes[i] = 0.0;
      }
    }

    mag_targets[i] = magnitudes[i];
    if (mag_targets[i] > max_mags[frequencies[i].zone]) {
      max_mags[frequencies[i].zone] = mag_targets[i];
    }
  }

  if (stream_max_mags == true) {
    USBSerial.print("sbs((max_mags=");
    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      USBSerial.print(max_mags[i]);
      if (i < NUM_ZONES - 1) {
        USBSerial.print(',');
      }
    }
    USBSerial.println("))");
  }

  // Two different algorithms are used to smooth the display output:
  //
  // Smoothing Type A: "Followers"
  // -----------------------------
  // The live magnitudes are stored in an array (mag_targets[])
  // These are the most recent values, and can fluctuate wildly.
  //
  // A second array of magnitude bins (mag_followers[]) also exists.
  //
  // On each frame, the difference (delta) between mag_targets[i]
  // and mag_followers[i] is calculated. mag_followers[i] is
  // incremented or decremented by (delta * amount) - amount being
  // a value between 0.0 and 1.0. Scaling the amount that the
  // follower is able to change per frame limits its speed and
  // makes its approach to the destination value non-linear.
  //
  // With the "amount" set to only 0.5, the mag_followers[] array
  // is only allowed to change by half the amount (on each frame)
  // that the targets array could. This slows down rapid transient
  // changes, adding a viscous smoothing effect that's slower to
  // track on as it gets closer to the target value.
  //
  //
  // Smoothing Type B: "Exponential Averaging"
  // -----------------------------------------
  // This one you might have heard of. Here's how it's implemented here,
  // with smoothing set to 0.9 (a high amount) as an example:
  //
  // smooth_frame = new_frame * (1.0-smoothing) + last_frame * smoothing;
  //
  // This takes the new data, scales it to 10% its original range,
  // scales the output of the last frame to 90% its original range,
  // and sums the two. Afterwards, the result is stored in "last_frame"
  // for the next run.
  //
  // This method is applied later, in the // Make Spectrogram section,
  // and is good for smoothing rapid changes but severly restricts the
  // percieved reaction time of the display.
  //
  //
  // The two smoothing algorithms are applied differently based on the MOOD knob position:
  // +---------------------------+-----------------+-----------------+----------------------------------------------------+
  // |  KNOB POSITION            |  TYPE A AMOUNT  |  TYPE B AMOUNT  |  EFFECT                                            |
  // +---------------------------+-----------------+-----------------+----------------------------------------------------+
  // |  0% (all the way left)    |  100%           |  100%           |  EXTREMELY SMOOTH, HIGHER LATENCY                  |
  // |  25%                      |  50%            |  100%           |                                                    |
  // |  50% (middle)             |  0%             |  100%           |  GOOD BALANCE, LOW LATENCY                         |
  // |  75%                      |  0%             |  50%            |                                                    |
  // |  100% (all the way right) |  0%             |  0%             |  FASTEST LED REACTION TIME, "NO" SMOOTHING, HARSH  |
  // +---------------------------+-----------------+-----------------+----------------------------------------------------+
  //
  // There's also some extra math so that smoothing values don't actually reach 0% (no change allowed per frame)
  // (So the 0% - 100% range of this chart is mostly representative)

  // These two upcoming variables are calculated in knobs.h:
  //   float smoothing_follower
  //   float smoothing_exp_average

  // Smooth GDFT changes with MOOD knob value ("Follower" algorithm)
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    if (mag_targets[i] > mag_followers[i]) {
      float delta = mag_targets[i] - mag_followers[i];
      mag_followers[i] += delta * (smoothing_follower * 0.5);
    }

    else if (mag_targets[i] < mag_followers[i]) {
      float delta = mag_followers[i] - mag_targets[i];
      mag_followers[i] -= delta * (smoothing_follower * 0.375);
    }
  }

  // The max_mags[] array is subject to the same type of smoothing,
  // hardcoded to 0.075.
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    if (max_mags[i] > max_mags_followers[i]) {
      float delta = max_mags[i] - max_mags_followers[i];
      max_mags_followers[i] += delta * 0.075;
    }
    if (max_mags[i] < max_mags_followers[i]) {
      float delta = max_mags_followers[i] - max_mags[i];
      max_mags_followers[i] -= delta * 0.075;
    }
  }

  if (stream_max_mags_followers == true) {
    USBSerial.print("sbs((max_mags_followers=");
    for (uint8_t i = 0; i < NUM_ZONES; i++) {
      USBSerial.print(max_mags_followers[i]);
      if (i < NUM_ZONES - 1) {
        USBSerial.print('\t');
      }
    }
    USBSerial.println("))");
  }

  // Make Spectrogram from raw magnitudes
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    // Normalize our frequency bins to 0.0-1.0 range, which acts like an audio compressor at the same time
    float max_mag = interpolate(i / float(NUM_FREQS), max_mags_followers, NUM_ZONES);
    float mag_float = mag_followers[i] / max_mag;

    // Restrict range, allowing for clipped values at peaks and valleys
    if (mag_float < 0.0) {
      mag_float = 0.0;
    } else if (mag_float > 1.0) {
      mag_float = 1.0;
    }

    // Smooth spectrogram changes with MOOD knob value (Exp. Avg. smoothing)
    mag_float = mag_float * (1.0 - smoothing_exp_average) + mag_float_last[i] * smoothing_exp_average;
    mag_float_last[i] = mag_float;

    mag_float *= CONFIG.GAIN;
    if(mag_float > 1.0){
      mag_float = 1.0;
    }

    note_spectrogram[i] = mag_float;                                          // This array is the current value
    spectrogram_history[spectrogram_history_index][i] = note_spectrogram[i];  // This array is the value's history
  }


  // Make Chromagram
  chromagram_max_val = 0.0;
  for (uint8_t i = 0; i < 12; i++) {
    note_chromagram[i] = 0;
  }
  for (uint8_t octave = 0; octave < 6; octave++) {
    for (uint8_t note = 0; note < 12; note++) {
      uint16_t note_index = 12 * octave + note;
      if (note_index < NUM_FREQS && note_index < CONFIG.CHROMAGRAM_RANGE) {
        note_chromagram[note] += note_spectrogram[note_index] * 0.5;

        if (note_chromagram[note] > 1.0) {
          note_chromagram[note] = 1.0;
        }

        if (note_chromagram[note] > chromagram_max_val) {
          chromagram_max_val = note_chromagram[note];
        }
      }
    }
  }
}

void lookahead_smoothing() {
  // This one is weird too, let's talk.
  //
  // Output to the LEDs is delayed by 2 frames. This is virtually
  // invisible to the naked eye at these speeds, and allows this
  // function to look ahead and do this:
  //
  // IF
  // The next frame will have a decreased value (or opposite)
  // AND
  // The frame after that will have an increased value again  (or opposite)
  // THEN
  // The next frame should just become an average of the current
  // and the "after next" frame to prevent that rapid change
  // from making to to the LEDs.
  //
  // This reduces (FPS/2)Hz flicker, especially at lower brightness
  // frequency bins.

  int16_t past_index = spectrogram_history_index - 2;
  if (past_index < 0) {
    past_index += spectrogram_history_length;
  }

  int16_t look_ahead_1 = spectrogram_history_index - 1;
  if (look_ahead_1 < 0) {
    look_ahead_1 += spectrogram_history_length;
  }

  int16_t look_ahead_2 = spectrogram_history_index - 0;
  if (look_ahead_2 < 0) {
    look_ahead_2 += spectrogram_history_length;
  }

  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    bool look_ahead_1_rising = false;
    bool look_ahead_2_rising = false;

    if (spectrogram_history[look_ahead_1][i] > spectrogram_history[past_index][i]) {
      look_ahead_1_rising = true;
    }
    if (spectrogram_history[look_ahead_2][i] > spectrogram_history[look_ahead_1][i]) {
      look_ahead_2_rising = true;
    }

    if (look_ahead_1_rising != look_ahead_2_rising) {  // if spike, set spike frame to average of neighbors
      spectrogram_history[look_ahead_1][i] = (spectrogram_history[past_index][i] + spectrogram_history[look_ahead_2][i]) / 2.0;
    }

    note_spectrogram_smooth[i] = spectrogram_history[past_index][i];
    note_spectrogram_long_term[i] = (note_spectrogram_long_term[i] * 0.95) + (note_spectrogram_smooth[i] * 0.05);
  }

  if (stream_spectrogram == true) {
    if (serial_iter >= 2) {  // Don't print every frame
      serial_iter = 0;
      USBSerial.print("sbs((spectrogram=");
      for (uint16_t i = 0; i < NUM_FREQS; i++) {
        uint16_t bin = 999 * note_spectrogram_smooth[i];
        USBSerial.print(bin);
        if (i < NUM_FREQS - 1) {
          USBSerial.print(',');
        }
      }
      USBSerial.println("))");
    }
  }
}
