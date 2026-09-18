# WRO Future Engineers 2026 — Engineering Documentation
**Team Name:** Jawatha Fighters  
**Robot Name:** Sensoria  
**Region:** AlAhsa

---

## 1. Executive Summary & Challenge Overview
This project details the development of **Sensoria**, an autonomous vehicle engineered for the World Robot Olympiad (WRO) Future Engineers competition. Our team combined advanced 3D CAD modeling, modular electronics, and robust control algorithms to build a custom robot capable of high-speed track navigation and dynamic obstacle avoidance. 

Sensoria is designed around strict physical inspection limits, a low center of gravity, and real-time sensor processing constraints. The vehicle autonomously detects color-coded traffic obstacles (Red and Green blocks), processes optical data, and executes real-time steering adjustments using closed-loop gyro feedback to maintain multi-lap stability.

---

## 2. Team Structure & Responsibilities

| Team Member | Official Role | Primary Engineering Responsibilities |
| :--- | :--- | :--- |
| **Hasan Abo Fares** | Coach | Construction guidance, strategy coordination, project management, and rules compliance. |
| **Mohammed Almossa** | Lead Software Engineer | Core autonomous state machine, Pixy2 vision algorithms, MPU-6050 closed-loop feedback, and low-latency control loops. |
| **Mohammed Alsalman** | Lead Mechanical & Electrical Engineer | Custom 3D CAD chassis modeling in Fusion 360, iterative prototyping, structural stress analysis, and circuit wiring layout. |

---

## 3. Mechanical Design & Custom Base

Sensoria utilizes a 100% custom 3D-printed chassis rather than an off-the-shelf commercial frame. Designing a custom chassis optimized structural rigidity, component placement, and power distribution for our exact competition layout.

![Sensoria CAD Model](models/Sensoria_Chassis_v3.stl) *(Refer to `models/` directory for full 3D STEP files)*

* **Dimensions:** 210 mm (L) × 145 mm (W) × 110 mm (H)
* **Primary Material:** PLA+ (Polylactic Acid Plus) for high impact resistance and minimal thermal warping.
* **Bare Frame Mass:** ~185 grams
* **Fully Assembled Vehicle Mass:** ~1000 grams (including motor drive, power supply, and electronics)

### Empirically Tested Design Tradeoffs
* **Stability vs. Weight:** Lowering the battery mounting tray by 12 mm into a recessed bottom pocket added 15 grams of chassis material, but significantly dropped our center of gravity. This eliminated chassis roll and cornering tip-overs during high-speed turns.
* **Strength vs. Print Time:** Printing the floor pan with a 20% honeycomb infill and 4 perimeter walls reduced total 3D print time by 3.5 hours and saved 18% frame mass while retaining the structural rigidity needed to resist torsional twisting.
* **Space vs. Component Placement:** Tightly packing modules reduced overall vehicle length to fit inspection limits. However, running high-current motor wires near sensor lines introduced electrical noise. We integrated isolated wire-routing channels along opposing sides of the floor pan to cleanly separate power lines from sensitive signal buses.

---

## 4. Robot Design & CAD Modeling

Sensoria was modeled entirely in Autodesk Fusion 360 and printed using Bambu Lab additive manufacturing hardware.

### Observed Benefits of 3D-Modeled Design
* **Precision Component Integration:** Modeling all sensors, actuators, and control boards in Fusion 360 verified physical clearances prior to printing, ensuring the 5x ultrasonic sensors and Pixy2 camera sat at exact angles.
* **Rapid Physical Iteration:** When testing revealed minor flex in the front subframe, we modified the CAD model by adding 2.5 mm vertical support gussets and printed a revised frame within hours.
* **Integrated Cabling Barriers:** Internal cable guides and isolation walls were embedded directly into the chassis floor, preventing wire snagging during high-speed runs.

---

## 5. Prototyping, Testing & Iteration Log

Sensoria evolved through three major physical prototype iterations to isolate structural vulnerabilities and optimize track performance.

| Iteration | Identified Issue / Testing Finding | Mechanical & Structural Change Applied | Measured Test Result |
| :--- | :--- | :--- | :--- |
| **Prototype v1.0** | High frame flex near front steering hub during tight cornering. | Added 2.5 mm vertical support ribs and gussets to front subframe. | Frame deflection reduced by ~70%; steering response became crisp and predictable. |
| **Prototype v2.0** | Side ultrasonic sensors suffered blind spots when approaching wall corners at steep angles. | Angled outer sensor mounts outward by 15° relative to the longitudinal axis. | Eliminated wall detection blind spots and reduced false distance spikes. |
| **Prototype v3.0** | Rapid acceleration caused camera tower vibration, throwing off Pixy2 color tracking. | Integrated a dual-braced elevated camera mount directly into the main chassis floor pan. | Stabilized vision feed, maintaining clean block tracking at maximum drive speeds. |

---

## 6. Drivetrain & Steering System Architecture

### Steering Geometry
Sensoria uses an Ackerman-inspired steering geometry to eliminate tire scrub during sharp cornering.
* **Steering Servo:** 20 kg-cm high-torque digital servo motor (6.0V rating).
* **Software Motion Limits:** Constrained between 65° and 115° (90° absolute center) to prevent mechanical binding.
* **Linkage:** Custom 3D-printed steering knuckles linked via ball-joint tie rods.

### Propulsion Drivetrain
* **Drive Motor:** High-efficiency rear DC gear motor driven via PWM motor driver.
* **Drivetrain Layout:** Rear-Wheel Drive (RWD) with high-traction rubber tires, ensuring maximum push force without interfering with front steering dynamics.

---

## 7. Electrical Architecture & Power Distribution

The primary processing system relies on an ATmega2560 microcontroller board running at 16 MHz with 256 KB Flash Memory.
