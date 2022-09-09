void duet_mode(bool invert_brightness, bool incandescent);
void bloom_mode(bool incandescent);

void duet_mode(bool invert_brightness, bool incandescent) {
  //AUTORANGE_MIX = 0.0;
  if (!invert_brightness) {
    //change_contrast_float(processed_fft, 85);
  }

  float velocity_sum = 0;
  uint8_t max_val = 0;
  for (uint16_t i = 0; i < 128; i++) {
    float fft_val = processed_fft[i];
    if (invert_brightness) {
      fft_val = 1.0 - fft_val;
    }

    float out_val = ( fft_val ) * 255.0;

    if (out_val < 127) {
      //out_val = 0;
    }
    else {
      //out_val = 255;
    }

    if (out_val < 0) {
      out_val = 0;
    }

    if (out_val > 255.0) {
      out_val = 255.0;
    }

    float final_val = (out_val * (1.0 - SMOOTHING)) + (last_fft_frame[i] * (SMOOTHING));

    if (final_val < 0.0) {
      final_val = 0.0;
    }
    else if (final_val > 255.0) {
      final_val = 255.0;
    }

    uint8_t smooth_threshold = 75 - (75 * SMOOTHING);
    if (abs(final_val - last_fft_frame[i]) > smooth_threshold) {
      if (final_val > last_fft_frame[i]) {
        float new_val = final_val;
        final_val = last_fft_frame[i] + smooth_threshold;// * 2.5;
        if(final_val > new_val){
          //final_val = new_val;
        }
      }
      else if (final_val < last_fft_frame[i]) {
        final_val = last_fft_frame[i] - smooth_threshold;
      }
    }

    if (final_val < 0.0) {
      final_val = 0.0;
    }
    else if (final_val > 255.0) {
      final_val = 255.0;
    }

    last_fft_frame[i] = final_val;

    float gamma_val = uint16_t( (uint8_t(final_val) * uint8_t(final_val)) >> 8 );
    final_val = gamma_val;

    //final_val *= 0.75;
    //final_val += 255*0.25;

    if (!incandescent) {
      //leds[i] = CHSV((hue) - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.4) + (i * hue_shift_amount), 255, final_val);
      leds[i] = CHSV((hue) - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.8) + (i * hue_shift_amount) - (32*(final_val/255.0)), 255, final_val);
    }
    else {
      leds[i] = CHSV(24, 205 - uint8_t((255 - final_val) * 0.3), final_val);
    }
  }

  //blur1d( leds, NUM_LEDS, 2 );
  process_color_push();
}

void tempo_sine_mode(float vel) {
  static uint8_t last_pos = 0;
  static float last_vel = 0;

  vel *= 1.5;
  if (vel > 1.0) {
    vel = 1.0;
  }
  if (vel < 0.15) {
    vel = 0.15;
  }
  //vel = vel/0.75;

  if (tempo_animation_frame >= tempo_animation_frame_cap) {
    tempo_animation_frame = 0;
    if (tempo_animation_frame_cap != tempo_animation_frame_cap_target) {
      tempo_animation_frame_cap = tempo_animation_frame_cap_target;
    }
  }
  else {
    tempo_animation_frame++;
  }

  float ratio = (tempo_animation_frame % tempo_animation_frame_cap) / float(tempo_animation_frame_cap);
  uint8_t whole_pos = cubicwave8(ratio * 255) >> 1;

  bool rising = false;
  if (whole_pos >= last_pos) {
    rising = true;
  }

  for (uint8_t i = 0; i < 128; i++) {
    leds[i].r *= 0.95;
    leds[i].g *= 0.95;
    leds[i].b *= 0.95;
  }

  if (rising) {
    for (uint8_t i = last_pos; i < whole_pos; i++) {
      float blend_amount = ((i - last_pos) / float(whole_pos - last_pos));
      uint8_t blend_vel = 255 * (vel * blend_amount + last_vel * (1.0 - blend_amount));
      leds[i] += CHSV((hue) - blend_vel * (0.1 + (hue_push / 10.0) * 0.4), 255, blend_vel);
    }
  }
  else {
    for (uint8_t i = whole_pos; i < last_pos; i++) {
      float blend_amount = ((i - whole_pos) / float(last_pos - whole_pos));
      uint8_t blend_vel = 255 * (vel * blend_amount + last_vel * (1.0 - blend_amount));
      leds[i] += CHSV((hue) - blend_vel * (0.1 + (hue_push / 10.0) * 0.4), 255, blend_vel);
    }
  }

  last_pos = whole_pos;
  last_vel = vel;

  process_color_push();
}

void tempo_orbs_mode(float vel) { // secondary function to tempo_sine_mode
  const uint8_t num_orbs = 6;
  static uint8_t last_pos[num_orbs];
  static float last_vel[num_orbs];

  vel *= 1.5;
  if (vel > 1.0) {
    vel = 1.0;
  }
  if (vel < 0.15) {
    vel = 0.15;
  }

  if (tempo_animation_frame % tempo_animation_frame_cap == 0) {
    if (tempo_animation_frame_cap != tempo_animation_frame_cap_target) {
      tempo_animation_frame_cap = tempo_animation_frame_cap_target;
    }
  }
  tempo_animation_frame++;

  for (uint8_t i = 0; i < 128; i++) {
    leds[i].r *= 0.8;
    leds[i].g *= 0.8;
    leds[i].b *= 0.8;
  }

  for (uint8_t o = 0; o < num_orbs; o++) {
    int16_t push_amount = 192 * (o / float(num_orbs));
    float multiplier = 1.0 - (0.05 * o);
    uint32_t false_tempo_animation_frame = tempo_animation_frame * multiplier;

    float ratio = (false_tempo_animation_frame % tempo_animation_frame_cap) / float(tempo_animation_frame_cap);
    uint8_t whole_pos = cubicwave8(ratio * 255) >> 1;

    bool rising = false;
    if (whole_pos >= last_pos[o]) {
      rising = true;
    }

    if (rising) {
      for (uint8_t i = last_pos[o]; i < whole_pos; i++) {
        float blend_amount = ((i - last_pos[o]) / float(whole_pos - last_pos[o]));
        uint8_t blend_vel = 255 * (vel * blend_amount + last_vel[o] * (1.0 - blend_amount));
        leds[i] += CHSV((hue + push_amount) - blend_vel * (0.1 + (hue_push / 10.0) * 0.4), 255, blend_vel);
      }
    }
    else {
      for (uint8_t i = whole_pos; i < last_pos[o]; i++) {
        float blend_amount = ((i - whole_pos) / float(last_pos[o] - whole_pos));
        uint8_t blend_vel = 255 * (vel * blend_amount + last_vel[o] * (1.0 - blend_amount));
        leds[i] += CHSV((hue + push_amount) - blend_vel * (0.1 + (hue_push / 10.0) * 0.4), 255, blend_vel);
      }
    }

    last_pos[o] = whole_pos;
    last_vel[o] = vel;
  }

  process_color_push();
}

float velocity_mode(bool run_leds) {
  static bool rising = false;
  static bool toggle = false;
  static float fft_vel_sum_last = 0;
  static float fft_vel_sum_smooth_last = 0;
  static int16_t pulse_frames_left = 0;
  static float fft_vel_sum_decay = 0.0;
  float fft_vel_sum = 0.0;
  float fft_vel_sum_smooth = 0.0;

  for (uint16_t i = 0; i < 128; i++) {
    float fft_val = fft_velocities[i];

    if (fft_val < 0.0) {
      //fft_val = 0.0;
    }

    fft_vel_sum += fft_val;
  }

  fft_vel_sum /= 128.0;

  if (fft_vel_sum < 0.0) {
    fft_vel_sum = 0.0;
  }
  else if (fft_vel_sum > 1.0) {
    fft_vel_sum = 1.0;
  }

  fft_vel_sum *= 2;
  if (fft_vel_sum > 1.0) {
    fft_vel_sum = 1.0;
  }

  //fft_vel_sum *= fft_vel_sum;

  fft_vel_sum_smooth = fft_vel_sum;
  fft_vel_sum_smooth = (fft_vel_sum_smooth * 0.03) + (fft_vel_sum_smooth_last * 0.97);
  fft_vel_sum_smooth_last = fft_vel_sum_smooth;

  fft_vel_sum = (fft_vel_sum * 0.1) + (fft_vel_sum_last * 0.9);
  fft_vel_sum_last = fft_vel_sum;

  fft_vel_sum -= (fft_vel_sum_smooth * 0.5);
  if (fft_vel_sum < 0.0) {
    fft_vel_sum = 0.0;
  }

  fft_vel_sum *= 4;
  if (fft_vel_sum > 1.0) {
    fft_vel_sum = 1.0;
  }

  //fft_vel_sum = fft_vel_sum*fft_vel_sum;

  if (fft_vel_sum_decay > 0.0) {
    fft_vel_sum_decay *= 0.95;
  }

  if (fft_vel_sum > fft_vel_sum_decay) {
    fft_vel_sum_decay = fft_vel_sum;
  }


  fft_vel_sum_history[fft_vel_sum_history_index] = fft_vel_sum;
  //if(fft_vel_sum < 0.25){
  //fft_vel_sum_history[fft_vel_sum_history_index] = 0.0;
  //}
  fft_vel_sum_history_index++;

  if (fft_vel_sum_history_index >= 360) {
    fft_vel_sum_history_index = 0;
    //autocorrelate_bpm();
  }
  else {
    if (run_leds) {
      FastLED.clear();
      uint8_t top_led = 127 * fft_vel_sum;
      for (uint16_t i = 0; i <= top_led; i++) {
        leds[i] = CHSV(hue, 255, 255);
      }
    }
  }

  return fft_vel_sum;
}


void bloom_mode(bool incandescent) {
  AUTORANGE_MIX = 0.0;
  static uint32_t iter = 0;
  iter++;

  change_contrast_float(processed_fft, 200);

  float fft_sum_a = 0;
  float fft_sum_b = 0;
  float fft_sum_c = 0;
  for (uint16_t i = 0; i < 128; i++) {
    float progress = i / 256.0 + 0.4;
    float fft_val = (processed_fft[i] * processed_fft[i]) * progress;
    uint8_t hue_val = (i * 0.75 + hue) - (20 * fft_val);

    CRGB out_col = CHSV(hue_val, 255, (fft_val * fft_val) * 55);

    if(incandescent) {
      out_col = CHSV(24, 205 - uint8_t(255 - ((255*fft_val) * 0.6)), (fft_val * fft_val) * 55);
    }

    fft_sum_a += out_col.r;
    fft_sum_b += out_col.g;
    fft_sum_c += out_col.b;
  }

  if (fft_sum_a > 255.0) {
    fft_sum_a = 255.0;
  }
  if (fft_sum_b > 255.0) {
    fft_sum_b = 255.0;
  }
  if (fft_sum_c > 255.0) {
    fft_sum_c = 255.0;
  }

  for (int16_t i = 127; i > 0; i--) {

    if (iter % 2 == 0) {
      leds_temp[i].r = leds_last[i - 1].r * 0.995;
      leds_temp[i].g = leds_last[i - 1].g * 0.995;
      leds_temp[i].b = leds_last[i - 1].b * 0.995;
    }
    else {
      leds_temp[i].r = leds_last[i - 1].r * 1.0;
      leds_temp[i].g = leds_last[i - 1].g * 1.0;
      leds_temp[i].b = leds_last[i - 1].b * 1.0;
    }
  }

  
  leds_temp[0] = CRGB(fft_sum_a, fft_sum_b, fft_sum_c);

  load_leds_from_temp();
  save_leds_to_last();

  process_color_push();

  shift_leds_up(64);
  mirror_image_downwards();
  fade_edges(64);
}
