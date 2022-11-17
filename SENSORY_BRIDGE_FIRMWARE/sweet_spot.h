void init_sweet_spot(){
  ledcSetup(0, 500, 12);
  ledcAttachPin(SWEET_SPOT_LEFT_PIN, 0);

  ledcSetup(1, 500, 12);
  ledcAttachPin(SWEET_SPOT_CENTER_PIN, 1);

  ledcSetup(2, 500, 12);
  ledcAttachPin(SWEET_SPOT_RIGHT_PIN, 2);
}

void run_sweet_spot(){
  float sweet_spot_smooth = sweet_spot_rough;
  static float sweet_spot_smooth_last = sweet_spot_rough;

  sweet_spot_smooth = (sweet_spot_smooth*0.05) + (sweet_spot_smooth_last*0.95);
  sweet_spot_smooth_last = sweet_spot_smooth;

  if(sweet_spot_smooth < 1.0){
    sweet_spot_smooth *= sweet_spot_smooth;
    ledcWrite(0, (1.0-sweet_spot_smooth) * 2048);
    ledcWrite(1, (sweet_spot_smooth) * 2048);
    ledcWrite(2, 0);
  }
  else{
    sweet_spot_smooth -= 1.0;
    sweet_spot_smooth *= sweet_spot_smooth;
    ledcWrite(0, 0);
    ledcWrite(1, (1.0-sweet_spot_smooth) * 2048);
    ledcWrite(2, (sweet_spot_smooth) * 2048);
  }
}