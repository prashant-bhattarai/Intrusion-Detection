Industrial Workspace Intrusion Monitor

Hey! This is Intrusion Monitor C++ computer vision project. I wanted to step away from basic webcam filters and build something that is actually used in modern robotics and manufacturing: a Vision-Based Safety Interlock.

In industrial settings (like Amazon fulfillment centers), robotic arms swing heavy payloads at high speeds. If a human or a fallen box enters the robot's work envelope, the system needs to instantly detect the intrusion, track its exact position, and trigger an E-stop.

This project simulates that pipeline using OpenCV.

How it Works

Instead of processing the entire 640x480 webcam feed, the system draws a "Safe Zone" and ignores everything else, cutting the CPU load by over 60%. I broke the perception pipeline down into three main challenges:

Dynamic Background Learning (MOG2):
If I just subtracted Frame 2 from Frame 1, a cloud passing over the sun would change the room's lighting and trigger a false alarm. Instead, I used a Mixture of Gaussians (MOG2) algorithm with a tuned learning rate (0.001). The camera statistically "learns" the empty desk over time, making it highly resistant to gradual lighting changes, while instantly flagging fast-moving foreign objects.

Morphological Noise Filtering:
Webcams have auto-exposure, which creates "salt and pepper" noise (random flickering pixels). If left unchecked, this static would constantly trigger the safety alarm. I used Morphological Opening (an Erosion pass to destroy the static, followed by a Dilation pass to restore the actual object's size) to create a perfectly clean, solid silhouette of the intruding object.

Centroid Tracking via Image Moments:
Just knowing that something is in the zone isn't enough; the system needs to know exactly where it is. I used OpenCV's findContours to trace the object, and calculated its Spatial Moments in physics. By dividing the first moments (m10 and m01) by the zeroth moment (m00 or Area), the math spits out the exact Center of Mass of the object, allowing the system to lock a tracking crosshair onto it.

Getting Started

If you want to run this locally:

Clone the repository and build it using CMake (requires OpenCV).

Run the executable.

Keep your hands out of the green box for the first few seconds so the camera can auto-adjust and the MOG2 algorithm can learn the background.

Move an object into the box to see the tracking and alerts in real-time!
