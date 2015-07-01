# camera_array_multitouch_cpp

---
## Why/Who/How will this project be developed
This project is sponsored by the [University of Arkansas at Little Rock's Emerging Analytics Center](http://eac.ualr.edu).  The EAC is currently directed by [Dr. Carolina Cruz-Neira](https://en.wikipedia.org/wiki/Carolina_Cruz-Neira) a.k.a inventor of the [CAVE immersive virtual reality environment](https://en.wikipedia.org/wiki/Cave_automatic_virtual_environment).  

The goal of this project is to create an open-source and easily implementable version of [Interaction Spaces](http://www.scsc.no/daniel/multi-touch/index.html) as made by the South Carolinia State University.

This project is free for public and educational use however please include a notable mention if this project furthers publishable research.  This project is forbidden for commercial use without the explicit permission of the Emerging Analytics Center at University of Arkansas at Little Rock.

---
## Implementation Specific Details
This implementation will require several ```(2+)``` USB cameras that operate at a minimum resolution of ```320x240```.  The frame rate of the cameras is not entirely important since this project will be limited by the processor and not the frame rate.  This implementation also requires adequate bandwidth and power for the number of cameras which you want to use.

---
## Implementation Gimmicks
For reference 2 cameras operating at ```640x480``` at ```60 FPS``` is enough to max out the power on a USB3 bus (```5V at 0.9A```).  This implementation will experiment with using embedded Raspberry PIs to preprocess data, provide more USB ports, and power more cameras and will communicate with the host through either USB, Serial, or Ethernet (```undecided```).

---
## Implementation Steps
First steps are as follows:
1. Find and tune morphological functions to track hands.
2. Move tracking into background processing by means of a thread pool.
3. Find and tune morphological functions for fast feature extraction of frame-by-frame feeds.
4. Move feature extraction into background processing.
5. Run performance and accuracy analysis.
6. Test performance and feasibility of embedded computers (```Raspberry PI or similar```).
7. Design frame for camera array and wiring/components.
8. Design and implement automatic camera calibration and real-time parameter tuning for optimal tracking and feature extraction.
 

---
## Long-Term Goal
This project will be used as the base for research regarding user segmentation by using the addition of volumetric data as collected by the view of the cameras.  Current implementations of using cameras to emulate a touch or multitouch interaction have the shortfall of being unable to accurately differentiate between users.
