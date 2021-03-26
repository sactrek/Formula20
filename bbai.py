#!/bin/env python

from Adafruit_BBIO import PWM
from Adafruit_BBIO import GPIO
from Adafruit_BBIO.Encoder import RotaryEncoder, eQEP0

import time
import numpy as np
from math import pi

class pwm_ops:

    def __init__(self, pin_0, pin_1):
        self.pin_0 = pin_0
        self.pin_1 = pin_0

    def start(self, duty, freq, polarity = 0):
        PWM.start(self.pin_1, 0, freq)
        PWM.start(self.pin_0, duty, freq)

    def stop(self):
        PWM.stop(self.pin_0)

    def cleanup(self):
        PWM.cleanup()

class eqep_ops:
    
    def __init__(self, eqep):
        self.encoder = RotaryEncoder(eqep)
        self.encoder.setAbsolute()
        self.encoder.enable()
    
    def read(self):
        return self.encoder.position

    def reset(self):
        self.encoder.zero()
    
    def __del__(self):
        self.encoder.disable()

class motor_pwm_sweep:

    # Wheel Info
    wheel_radius = 0.035 # meters
    # Wheel Encoder Info (PPR)
    ticks_per_rev = 198.6
 
    data = None
    pwm = None
    eqep = None

    def __init__(self, pwm_pin0, pwm_pin1, eqep_inst):
        self.pwm = pwm_ops(pwm_pin0, pwm_pin1)
        self.eqep = eqep_ops(eqep_inst)

    def calculate_wheel_rpm(self, encoder_ticks, time_secs):
        revs = float(encoder_ticks / self.ticks_per_rev)
        rpm = (revs * 60) / time_secs
        return rpm

    def calculate_wheel_velocity(self, encoder_ticks, time_secs):
        revs = float(encoder_ticks / self.ticks_per_rev)
        dist_per_rev = float(2 * pi * self.wheel_radius)
        distance = float(revs * dist_per_rev)
        speed = float(distance/time_secs)
        return speed

    def sweep_pwm_freq(self, freq_min=1000, freq_max=4000, freq_step=100, duty=50, sample_time=3):
        print('%5s %8s %12s %7s %6s %12s %12s' % ('#', 'Freq', 'Enc Ticks', 'Speed', 'RPM', 'EncStart', 'EncEnd'))
        freqs = range(freq_min,freq_max+1,freq_step)
        data = np.zeros((len(freqs), 6))
        for i in range(0, len(freqs)):
            enc_start = self.eqep.read()
            self.pwm.start(duty, freqs[i])
            time.sleep(sample_time)
            self.pwm.stop()
            enc_end = self.eqep.read()
            enc_reading = enc_end - enc_start
            # populate data
            data[i][0] = freqs[i]
            data[i][1] = enc_reading
            data[i][2] = self.calculate_wheel_velocity(enc_reading, sample_time)
            data[i][3] = self.calculate_wheel_rpm(enc_reading, sample_time)
            data[i][4] = enc_start
            data[i][5] = enc_end
            print('%3d %8d %12d %6.1f %6d %12d %12d' % (i, data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5]))
            time.sleep(0.5)

    def sweep_pwm_duty(self, duty_min=0, duty_max=100, duty_step=5, freq=2000, sample_time=3 ):
        print('%5s %8s %12s %7s %6s %12s %12s' % ('#', 'Duty', 'Enc Ticks', 'Speed', 'RPM', 'EncStart', 'EncEnd'))
        duties = range(duty_min,duty_max+1,duty_step)
        data = np.zeros((len(duties), 6))
        for i in range(0, len(duties)):
            enc_start = self.eqep.read()
            self.pwm.start(duties[i], freq)
            time.sleep(sample_time)
            self.pwm.stop()
            enc_end = self.eqep.read()
            enc_reading = enc_end - enc_start
            # populate data
            data[i][0] = duties[i]
            data[i][1] = enc_reading
            data[i][2] = self.calculate_wheel_velocity(enc_reading, sample_time)
            data[i][3] = self.calculate_wheel_rpm(enc_reading, sample_time)
            data[i][4] = enc_start
            data[i][5] = enc_end
            print('%3d %8d %12d %6.1f %6d %12d %12d' % (i, data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5]))
            time.sleep(0.5)

    def __del__(self):
        self.pwm.cleanup()


if __name__ == "__main__":

    #PWM Duty-Cycle Sweep
    ## Motor izquierdo	
    motorIzq = motor_pwm_sweep('P9_14', 'P9_16', eQEP0)
    motorIzq.sweep_pwm_duty(0,100,5,200,4) 
    ## Motor derecho
    motorDer = motor_pwm_sweep('P9_14', 'P9_16', eQEP1)
    motorDer.sweep_pwm_duty(0,100,5,200,4) 
    #cambiar el duty_cycle para una frecuencia fija, mas interesante. Con una freq de 200, cambiar el duty de 0 a 100 de 5 en 5 durante 4 segundos el motor va mas rapido cuando el duty_cycle es 100%


#Para cada motor, hacer uno u otro
#PWM Frequency Sweep
#motor1 = motor_pwm_sweep('P9_21', 'P9_22', eQEP0)
#motor1.sweep_pwm_freq(100,4000,100,50,4) #cambia la frecuencia de 100Hz a 4000Hz en incrementos de 100Hz con un periodo de 50%, durante 4 segundos para cada frecuencia