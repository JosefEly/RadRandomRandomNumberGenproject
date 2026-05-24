# RadRandom-RNG-project
Schematics and code for a hardware RNG project

This project works using a smoke detector AM-241 source and a reverse-biased photodiode. When an alpha particle hits the detector, this registers as a voltage spike which is converted to a digital signal by a comparator (with hysterisis to avoid turning one detection event into two). An Arduino measures the time between two sets of pulses - if A is greater than B, it's a 1, if A < B, it's a 0, and if they are equal, it tries again recursively. Because the generator can only generate so many bits at a time, these bits are generated and then cached, and each roll pulls from the cached file. Because radioactive decay is random at the quantum level, the bits should be too. A Python verison of the NIST statistical test suite thought the bits were in fact random.
<img width="1512" height="1134" alt="image" src="https://github.com/user-attachments/assets/bc675eca-3937-4451-ae52-24fc9c776936" />
<img width="1234" height="1646" alt="image" src="https://github.com/user-attachments/assets/6da316cc-08af-46dd-ae23-627be360e26e" />
