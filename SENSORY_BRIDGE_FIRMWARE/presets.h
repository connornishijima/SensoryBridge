void set_preset(char* preset_name) {
  // Modifies current CONFIG struct based on input preset name

  if (strcmp(preset_name, "default") == 0) {
    CONFIG.SQUARE_ITER = 1;
    CONFIG.INCANDESCENT_FILTER = 0.80;
    CONFIG.INCANDESCENT_MODE = false;
    CONFIG.BASE_COAT = true;
    CONFIG.BULB_OPACITY = 0.0;
    CONFIG.SATURATION = 1.0;
  }

  else if (strcmp(preset_name, "tinted_bulbs") == 0) {
    CONFIG.SQUARE_ITER = 1;
    CONFIG.INCANDESCENT_FILTER = 0.80;
    CONFIG.INCANDESCENT_MODE = false;
    CONFIG.BASE_COAT = false;
    CONFIG.BULB_OPACITY = 1.0;
    CONFIG.SATURATION = 1.0;
  }

  else if (strcmp(preset_name, "incandescent") == 0) {
    CONFIG.SQUARE_ITER = 1;
    CONFIG.INCANDESCENT_FILTER = 1.0;
    CONFIG.INCANDESCENT_MODE = true;
    CONFIG.BASE_COAT = true;
    CONFIG.BULB_OPACITY = 0.0;
    CONFIG.SATURATION = 1.0;
  }

  else if (strcmp(preset_name, "white") == 0) {
    CONFIG.SQUARE_ITER = 1;
    CONFIG.INCANDESCENT_FILTER = 0;
    CONFIG.INCANDESCENT_MODE = false;
    CONFIG.BASE_COAT = true;
    CONFIG.BULB_OPACITY = 0.0;
    CONFIG.SATURATION = 0.0;
  }

  else if (strcmp(preset_name, "classic") == 0) {
    CONFIG.SQUARE_ITER = 1;
    CONFIG.INCANDESCENT_FILTER = 0.0;
    CONFIG.INCANDESCENT_MODE = false;
    CONFIG.BASE_COAT = false;
    CONFIG.BULB_OPACITY = 0.0;
    CONFIG.SATURATION = 1.0;
  }
}