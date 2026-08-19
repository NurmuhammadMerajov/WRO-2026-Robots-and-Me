# 🚧 Engineering Challenges & Solutions

Building an autonomous Ackermann-steered vehicle for the WRO Future Engineers 2026 competition is not a linear process. This document serves as a deep dive into the critical mechanical, electrical, and software bottlenecks our team faced, and the engineering strategies we developed to overcome them.

---

### 1. The Ackermann CAD Bottleneck & Construction Delays
* **The Challenge:** Translating theoretical Ackermann steering geometry ($\cot\delta_o - \cot\delta_i = w/L$) into a functional 3D printable assembly was our most significant hurdle. Calculating the exact pivot distances and linkage lengths to prevent tire scrubbing caused multiple failed CAD iterations. Because the entire front chassis depended on this precise geometry, our physical construction timeline was severely delayed.
* **The Solution:** Instead of waiting hours for 3D prints to test each CAD iteration, we shifted to **rapid physical prototyping**. We used laser-cut wooden templates to physically test the 2D steering pivot geometry and linkage arms in real life. Once the angles ($\pm 35^\circ$ limits) were physically validated on wood, we finalized the 3D models and sent them to the PETG 3D printer.