# Digitimer DS8R high frequency control box sketch 
An Arduino sketch to generate pulses for high frequency stimulation using Digitimer DS8R stimulator.

# Sketches
The following sketches are available. Note that the exact frequency of the generated pulses will depend on the Arduino board. You can measure to check the output frequency if you require it to be precise. 
For High frequency modulation use: (it allows to easily change the pulsewidth of the low frequency) This is easiest sketch to use with minimal to no change required to generate ~30 Hz over ~10 kHz on Arduino MEGA2560.  
1. pulsewidth__carried_freq_periodic_delay

You could also use the following sketches if you know how to input your parameters ( the #2 is easier to work with). 

2. carried_freq_periodic_delay
3. carried_freq.


