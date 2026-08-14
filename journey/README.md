# 🚀 Our Engineering Journey

Welcome to our raw, unfiltered engineering log. Here we document our daily decisions, mathematical sanity checks, technical breakthroughs, and the inevitable headaches of building an autonomous vehicle for WRO Future Engineers 2026.

---

## 📌 Table of Contents
* [Day 1 — August 13, 2026: The Calm Before the Storm](#-day-1--august-13-2026-the-calm-before-the-storm)
* [Day 2 — August 14, 2026: Point of No Return: Motors, Math & CAD Overhaul](#-day-2--august-14-2026-point-of-no-return-motors-math--cad-overhaul)

---

## 📅 Day 1 — August 13, 2026: The Calm Before the Storm

Every journey begins with ordinary enthusiasm and a blank slate. Today was dedicated to understanding the battlefield before writing a single line of code or cutting any wire.

### 🎯 Key Milestones:
1. **Rulebook Dissection:** Deep-dived into the WRO 2026 Future Engineers General Rules. We analyzed the scoring breakdown (75% track performance, 25% documentation) and locked down critical constraints:
   * Max vehicle dimensions: $300 \times 200 \times 300\text{ mm}$.
   * Complete 3 laps within a 3-minute strict time window.
   * Parking bonus: 15 points for parallel, fully enclosed parking.
2. **Architecture Brainstorming:** Evaluated differential drive vs. Ackermann steering. We unanimously decided on **Ackermann steering** with front independent knuckles and a rear driven axle for predictable high-speed trajectories.
3. **Repository Setup:** Structured the GitHub workspace to ensure complete traceability from Day 1.

---

## 📅 Day 2 — August 14, 2026: Point of No Return: Motors, Math & CAD Overhaul

There is no turning back now. Today theory collided violently with practical hardware constraints, turning into a marathon of mathematical calculations and complete CAD overhauls.

---

### 1. Drivetrain & 3D CAD Iterations: "Ah, Here We Go Again"

Our CAD engineer had an exhausting day. Every time the team re-evaluated motor torque and reduction ratios, the entire rear drivetrain and motor mounts had to be completely remodeled from scratch.

<div align="center">
  <img src="assets/day02_meme_cj.jpg" width="300" alt="CAD Engineer Here We Go Again Meme"/>
  <p><i>Figure 1: Opening Fusion 360 for the 3rd drivetrain redesign today.</i></p>
</div>

* **Iteration 1 (N20 Micro Metal Gear):** Ultra-compact, but structural analysis showed the brass micro-gears would strip under Ackermann steering shock loads.
* **Iteration 2 (Chihai 16mm 230 RPM):** Strong motor, but 230 RPM geared down through a bevel differential dropped wheel speed to a crawl (~0.35 m/s).
* **Iteration 3 (GA25-370 12V 620 RPM + 2:1 Bevel Differential):** **The Winning Strategy.** Robust metal gearbox, direct 3S Li-Po compatibility, and an ideal external reduction gear delivering ~0.97 m/s track speed.

---

### 2. Pre-Assembly Mathematical Sanity Check

Before soldering or drilling, we solved our linear kinematics on paper to avoid burning the motor driver or outpacing the camera's shutter speed.

<div align="center">
  <img src="assets/patrick_math.png" width="360" alt="Patrick Math Meme"/>
  <p><i>Figure 2: Calculating linear velocity and frame-to-distance ratios before touching hardware.</i></p>
</div>

#### 📐 Drivetrain Calculations:
Using $60\text{ mm}$ ($0.06\text{ m}$) diameter wheels:

* **Raw Wheel Circumference:**
  $$L = \pi \times D = 3.1415 \times 0.06\text{ m} = 0.1885\text{ m}$$

* **Direct Drive Speed (Without Differential):**
  $$V_{\text{raw}} = \frac{620\text{ RPM} \times 0.1885\text{ m}}{60} \approx 1.95\text{ m/s (7.0 km/h)}$$
  *(At 30 FPS, the robot moves 6.5 cm per frame).*

* **With 2:1 Custom Bevel Differential ($Z_1 = 15, Z_2 = 30$ teeth):**
  $$i = \frac{Z_{\text{crown}}}{Z_{\text{pinion}}} = \frac{30}{15} = 2.0$$
  $$\text{Final Wheel RPM} = \frac{620\text{ RPM}}{2.0} = 310\text{ RPM}$$
  $$\text{Final Speed} = \frac{310\text{ RPM} \times 0.1885\text{ m}}{60} \approx 0.97\text{ m/s (3.5 km/h)}$$

> 💡 **Engineering Outcome:** At 0.97 m/s, the robot covers only 3.2 cm per camera frame (30 FPS), providing sufficient headroom for real-time OpenCV color segmentation while delivering strong climbing and acceleration torque.

---

### 3. Gyroscope vs. Encoders: Theoretical Overload

<div align="center">
  <img src="assets/patrick_bottle.png" width="300" alt="Patrick Bottle Dilemma Meme"/>
  <p><i>Figure 3: Brain freeze over whether we really need optical wheel encoders.</i></p>
</div>

We debated whether wheel encoders were mandatory for lap counting and parallel parking. 

#### Why We Eliminated Encoders:
1. **Wheel Slip Reality:** In tight cornering, rubber tires inevitably slip, corrupting odometry data.
2. **Spatial Closed-Loop Control:**
   * **Lap Counting:** We integrate yaw angle from the **LSM6DSOX 6-DoF IMU** ($3 \times 360^\circ = 1080^\circ$ total rotation).
   * **Precision Parking:** Controlled dynamically via real-time distance measurements from the **HC-SR04 sonar array**, stopping reliably when wall distance $\le 5\text{ cm}$.

---

### 🛠️ Hardware Decisions Locked (August 14):
* **Drive Motor:** GA25-370 12V 620 RPM via TB6612FNG MOSFET driver.
* **Steering Actuator:** Surpass Hobby 25g Metal-Gear Digital Servo ($3.0\text{ kg}\cdot\text{cm}$).
* **Localization:** LSM6DSOX Gyroscope + Ultrasonic Sensor Fusion.

---

## 🛠️ Tools & Acknowledgments
* **Design & Simulation:** Fusion 360, Draw.io
* **Firmware & Control:** Arduino IDE, Python 3, OpenCV
* **Documentation Note:** Large Language Models were utilized strictly for grammatical refinement, translation clarity, and markdown formatting. All engineering decisions, mathematical derivations, code architecture, and testing logs are 100% originally developed by Team Robots and Me.