# 🏎️ WRO Puerto Rico 2026: Robots and Me — Technical Monograph & Vehicle Documentation

<div align="center">

[![WRO 2026](https://img.shields.io/badge/WRO-Future_Engineers_2026-blue.svg?style=for-the-badge)](https://wro-association.org/)
[![Category](https://img.shields.io/badge/Host-Puerto_Rico-red.svg?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)
[![Engineering Journal](https://img.shields.io/badge/Docs-Engineering_Journal-orange.svg?style=for-the-badge)](journey/README.md)

**A Comprehensive Monograph on the Design, Kinematics, Distributed Computing, and Autonomous Navigation Architecture of an Ackermann-Steered Robotic Vehicle.**

</div>

---

## 📖 Table of Contents
1. [Acknowledgments](#1-acknowledgments)
2. [Team Structure & Roles](#2-team-structure--roles)
3. [Electrical Schematics & Power Mathematics](#3-electrical-schematics--power-mathematics)
4. [3D CAD Architecture & Spatial Placement](#4-3d-cad-architecture--spatial-placement)
5. [Drivetrain Kinematics & Physics Formulation](#5-drivetrain-kinematics--physics-formulation)
6. [Distributed Microcontroller Communication (SBC ↔ MCU)](#6-distributed-microcontroller-communication-sbc--mcu)
7. [Mathematical Driving Dynamics & Control Theory](#7-mathematical-driving-dynamics--control-theory)
8. [Engineering Challenges, Constraints & Trade-Offs](#8-engineering-challenges-constraints--trade-offs)
9. [Experimental Results & Benchmark Metrics](#9-experimental-results--benchmark-metrics)
10. [Conclusion & Next Iterations](#10-conclusion--next-iterations)

---

## 1. Acknowledgments

We express our sincere gratitude and appreciation to our dedicated mentor and instructor, **Mr. Navruz**, for his guidance, robotics hardware support, and deep practical insights into autonomous control systems. His technical mentorship laid the foundation for our mechanical problem-solving approaches and mathematical modeling throughout this journey.

---

---
## 2. Team Structure & Roles

Our project operates on an agile systems-engineering model where software, electrical, and mechanical domains interface seamlessly:

| Team Member | Engineering Role | Primary Responsibilities | Core Tech Stack |
| :--- | :--- | :--- | :--- |
| **Nurmuhammad Merajov** | **Software & Math Lead** | Computer Vision (OpenCV), Path Planning (FSM), Mathematical Kinematics, Serial Protocol. | Python 3, C++, NumPy, OpenCV |
| **Kamron Kamolov** | **Mechanical CAD Lead** | 3D CAD Design (Chassis, Differential Gearbox, Steering Knuckles), Slicing & 3D Printing. | Fusion 360, PETG FDM |
| **Doston Rustamov** | **Electronics & Embedded Lead** | Electrical Schematics, Power Distribution, Low-level MCU Wiring, Circuitry & Actuation. | Arduino C++, TB6612FNG, UART |
---

