void duet_mode(bool invert_brightness);
void bloom_mode();

void duet_mode(bool invert_brightness){
	AUTORANGE_MIX = 0.0;
	if(!invert_brightness){
		change_contrast_float(processed_fft, 110);
	}

	float velocity_sum = 0;
	uint8_t max_val = 0;
	for (uint16_t i = 0; i < 128; i++) {
		float out_val = ( processed_fft[i] * processed_fft[i] ) * 255.0;

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

		uint8_t smooth_threshold = 100 - (100 * SMOOTHING);
		if (abs(final_val - last_fft_frame[i]) > smooth_threshold) {
			if (final_val > last_fft_frame[i]) {
				final_val = last_fft_frame[i] + smooth_threshold;
			}
			else if (final_val < last_fft_frame[i]) {
				final_val = last_fft_frame[i] - smooth_threshold;
			}
		}

		last_fft_frame[i] = final_val;

		if(!invert_brightness){
			leds[i] = CHSV((hue) - uint8_t(uint16_t(final_val * final_val) >> 8)*(0.1+(hue_push / 10.0)*0.4) + (i * hue_shift_amount), 255, final_val);
		}
		else{
			leds[i] = CHSV((hue) - final_val*(0.1+(hue_push / 10.0)*0.4) + (i * hue_shift_amount), 255, 255-uint8_t(uint16_t(final_val * final_val) >> 8));
		}
	}

	process_color_push();
}


void bloom_mode(){
  AUTORANGE_MIX = 0.0;
  static uint32_t iter = 0;
  iter++;

  change_contrast_float(processed_fft, 150);

  float fft_sum_a = 0;
  float fft_sum_b = 0;
  float fft_sum_c = 0;
  for (uint16_t i = 0; i < 128; i++) {
    float progress = i / 256.0 + 0.4;
    float fft_val = (processed_fft[i] * processed_fft[i]) * progress;
    uint8_t hue_val = (i * 0.75 + hue) - (20 * fft_val);
    CRGB out_col = CHSV(hue_val, 255, (fft_val*fft_val) * 55);

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
  fade_edges(32);
}