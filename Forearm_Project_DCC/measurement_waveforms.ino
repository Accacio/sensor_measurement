

void openclosearmsquarewave()
{
  if (cont_cycle%(cont_high+cont_low)<cont_high)
  {
    PWM_value=FULL_OPEN_ELBOW;
  }

  if (cont_cycle==0||cont_cycle%(cont_high+cont_low)>=cont_high)
  {
    PWM_value=FULL_CLOSE_ELBOW;
  }
}


void square_wave()
{
  if (cont_cycle%(cont_high+cont_low)<cont_high)
  {
    PWM_value=1.8*percent_high;
  }

  if (cont_cycle==0||cont_cycle%(cont_high+cont_low)>=cont_high)
  {
    PWM_value=9;
  }
}

void sine_wave()
{
  if (cont_high==0)
  {
    PWM_value=0;
  }
  else
  {
    PWM_value=(1.8/2)*percent_high*(1-cos(cont_cycle*PI/cont_high));
  }
}

void sine_wave_fqvar()
{
  if (cont_high==0)
  {
    PWM_value=0;
  }
  else
  {
    PWM_value=(1.8/2)*percent_high*(1-cos(cont_cycle*PI/cont_high));
    if(cont_frvar>=2*cont_high)
    {
      cont_high=cont_high/2;
      cont_frvar=0;
    }
  }
}


void measurement()
{
  int values_int[ar_last];
  float vref, vpot, vim, potref ,vref_mean, vpot_mean, vim_mean, potref_mean;
  float pot_raw, im, im_mean, loadcell, loadcell_mean, angle, aspeed, aacel;
  float total_accel, angle_filt, angle_rawfilt, loadcell_filt, loadcell_rawfilt;
  int xaccel_mean,yaccel_mean,zaccel_mean;
  int register_vpot[comparador];
  int equal_mean;
  do
  {
    servooldg.write(0);
    delay(100);
    readSensors(values_int);
    equal_mean=0;
    for(int i=0;i<comparador;i++)
    {
      equal_mean+=register_vpot[i];
    }
    equal_mean=equal_mean/comparador;

    for(int i=1;i<comparador;i++)
    {
      register_vpot[i-1]=register_vpot[i];
    }
    register_vpot[comparador-1]=values_int[ar_vpot_mean];
  }while(values_int[ar_vpot_mean]!=equal_mean);
  readSensors_filteronly();
  angular_measures(read_elbow_angle(vpot_filter.output()));
  readSensors_filteronly();
  angular_measures(read_elbow_angle(vpot_filter.output()));
// angular_measures(read_elbow_angle(values_int[ar_vpot_mean]));
  cont_cycle=0;
  cont_frvar=0;
  t0_time=millis();



  do
  {
    //square_wave();
    openclosearmsquarewave();
    //sine_wave();
    //sine_wave_fqvar();

    servooldg.write(PWM_value);
    cont_cycle++;
    cont_frvar++;

    //readSensors(values_int);
    //lowpassFilter.input(values_int[ar_vpot_mean]);
    //lowpassLoadCell.input(values_int[ar_vloadcell_mean]);
    //loadcell_filt=((lowpassLoadCell.output()-LC_BIT_MIN)*(LC_NEWTON_MAX-LC_NEWTON_MIN))/(LC_BIT_MAX-LC_BIT_MIN)+LC_NEWTON_MIN;
    //// Conversion from bits to values
    //vref=1.989*values_int[ar_vref];
    //vpot=100*((values_int[ar_vpot]*1.0-MPOS_DC)/(MPOS_MAX-MPOS_DC));
    //im=((values_int[ar_vim]-CURRENTBIT_DC)*(5000/(CURRENT_GAIN*1023.00)))/0.167;
    //potref=values_int[ar_potref];
    //vref_mean=1.989*values_int[ar_vref_mean];
    //vpot_mean=100*((values_int[ar_vpot_mean]*1.0-MPOS_DC)/(MPOS_MAX-MPOS_DC));
    //im_mean=((values_int[ar_vim_mean]-CURRENTBIT_DC)*(5000/(CURRENT_GAIN*1023.00)))/0.167;
    //potref_mean=values_int[ar_potref_mean];
    //pot_raw=values_int[ar_vpot];
    //angle=read_elbow_angle(values_int[ar_vpot_mean])*180/PI;
    //angle_filt=read_elbow_angle(lowpassFilter.output())*180/PI;
    //angle_rawfilt=read_elbow_angle(vpot_filter.output())*180/PI;
    //loadcell=((values_int[ar_vloadcell]-LC_BIT_MIN)*(LC_NEWTON_MAX-LC_NEWTON_MIN))/(LC_BIT_MAX-LC_BIT_MIN)+LC_NEWTON_MIN;
    //loadcell_mean=((values_int[ar_vloadcell_mean]-LC_BIT_MIN)*(LC_NEWTON_MAX-LC_NEWTON_MIN))/(LC_BIT_MAX-LC_BIT_MIN)+LC_NEWTON_MIN;



    tfilter=millis();
    readSensors_filteronly();
    angle_rawfilt=read_elbow_angle(vpot_filter.output());
    angular_measures(angle_rawfilt);
    loadcell_rawfilt=((loadcell_filter.output()-LC_BIT_MIN)*(LC_NEWTON_MAX-LC_NEWTON_MIN))/(LC_BIT_MAX-LC_BIT_MIN)+LC_NEWTON_MIN;
    tfilter=millis()-tfilter;


    //Sending information over serial
    Serial.print(PWM_value);
    Serial.print(',');
//    Serial.print(total_accel);
//    Serial.print(',');
//    Serial.print(xaccel_mean);
//    Serial.print(',');
//    Serial.print(yaccel_mean);
//    Serial.print(',');
//    Serial.print(zaccel_mean);
//    Serial.print(',');
    //Serial.print(vref);
    //Serial.print(',');
    //Serial.print(pot_raw);
    //Serial.print(',');
    //Serial.print(vpot);//vpot_int
    //Serial.print(',');
    //Serial.print(vref_mean);
    //Serial.print(',');
    //Serial.print(vpot_mean);//vpot_mean int
    //Serial.print(',');
    //Serial.print(im);
    //Serial.print(',');
    //Serial.print(im_mean);
    //Serial.print(',');
    //Serial.print(loadcell);
    //Serial.print(',');
    //Serial.print(loadcell_mean);
    //Serial.print(',');
    //Serial.print(loadcell_filt);
    //Serial.print(',');
    Serial.print(loadcell_rawfilt);
    Serial.print(',');
    //Serial.print(angle);
    //Serial.print(',');
    //Serial.print(angle_filt);
    //Serial.print(',');
    Serial.print(angle_rawfilt*180/PI);
    Serial.print(',');
    Serial.print(speed_filter.output());
    Serial.print(',');
    Serial.print(accel_filter.output());
    Serial.print(',');
    Serial.print(t_time);
    Serial.println(',');
    // Filter time
    //Serial.print(tfilter);
    //Serial.println(',');
    // Filter time
    do{
      delayMicroseconds(500);
      t1_time=millis();
      t_time=t1_time-t0_time;
    }while(t_time<const_time);
    t0_time=t1_time;
    if (Serial.available())
    {
      serialEvent();
    }
  }while(menu_var>0);

}