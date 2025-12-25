# Physics, CCD, and Time Visibility

## Why Physics Often Runs at Higher Frequency Than Rendering

This document consolidates the core questions and clarifications around **fixed-timestep physics**, **Continuous Collision Detection (CCD)**, **determinism**, and **rendering visibility** in real-time simulation engines.

It is written as a conceptual reference for the *Engine Playground* project and is intended to explain **why collisions are never seen late**, **why tunneling does not leak to rendering**, and **why physics often runs faster than rendering**.

---

## 1. Fixed Timestep Does NOT Mean Atomic Time

A fixed timestep (`dt`) means:

> The physics simulation advances time in fixed, deterministic increments, independent of render frame pacing or wall-clock jitter.

It does **not** mean that time inside the timestep is indivisible.

Inside a single physics step:

```
[t0 ---------------------- t0 + dt]
         ↑ collision at t_hit
```

The physics engine may:

* integrate motion until `t_hit`
* resolve the collision
* integrate the remaining time (`dt - t_hit`)

All of this occurs **inside one physics step**.

---

## 2. CCD Does Not Interrupt the Physics Loop

CCD is not an interrupt and does not split the engine loop.

> **CCD refines how a fixed timestep is solved, not how it is scheduled.**

From the engine core perspective:

* exactly one step occurred
* exactly `dt` time advanced
* one authoritative state exists

Intermediate states are *never* exposed.

---

## 3. Physics Does Not Publish Packets Per Step

Physics is not a producer emitting packets to the engine core.

Correct model:

```
Engine Core owns time
Physics mutates state
Render & Audio read state
```

CCD is an internal detail of `PhysicsWorld::step(dt)` and is invisible to other systems.

---

## 4. How Other Systems Observe CCD

Other systems do **not** need to know that CCD occurred.

They only observe:

* state before the step
* state after the step

CCD guarantees that the published state is physically valid.

> No system ever sees a tunneled or later-corrected state.

---

## 5. Physics FPS == Render FPS Is Not a Requirement

Even when physics and rendering both run at 60 Hz:

* rendering only sees **step-end states**
* collisions resolved inside the step are already reflected

Render never observes:

* penetration
* tunneling
* retroactive correction

---

## 6. State Mutation vs Collision Events

Two layers must be distinguished:

### Physics Core (mandatory)

* position update
* velocity update
* deterministic collision resolution

### Gameplay / Presentation Layer (optional)

* collision events
* sounds
* camera shake
* particles

Render perceives collisions through **state change**, not events.

---

## 7. Logical Time vs Real Time

Physics operates in **logical time**:

* deterministic
* replayable
* independent of wall-clock time

Rendering operates in **real time**:

* frame-based
* interpolated
* allowed to skip or repeat frames

> Physics simulates time. Render visualizes time.

---

## 8. Why Physics Often Runs at Higher Frequency Than Rendering

In many engines, physics runs at a higher rate than rendering (e.g., physics at 120 Hz, render at 60 FPS).

This is intentional.

### 8.1 Improved Collision Accuracy

Higher physics frequency means:

* smaller `dt`
* reduced penetration depth
* fewer CCD edge cases
* more stable contact resolution

Especially important for:

* vehicle dynamics
* fast projectiles
* stacked contacts

---

### 8.2 Better Input Responsiveness

Physics processes player input.

Higher physics rate:

* reduces input-to-motion latency
* improves steering and control feel
* avoids "frame-quantized" motion

This is critical in:

* racing games
* character controllers
* simulation-heavy gameplay

---

### 8.3 Render Is Allowed to Be Approximate

Rendering:

* may drop frames
* may interpolate
* may vary with hardware

Physics:

* must remain stable
* must remain deterministic

Thus:

> Rendering adapts to physics, not the other way around.

---

### 8.4 Determinism and Replay Stability

Higher physics frequency:

* reduces numerical error accumulation
* stabilizes deterministic replay
* makes CCD decisions more consistent

This is especially important for:

* replay systems
* lockstep networking
* backtesting simulations

---

## 9. Why Rendering Does Not Need Sub-Step Visibility

Even if a collision occurs mid-step:

* physics resolves it immediately
* render sees only the final valid state

Human perception tolerates ~16 ms latency.

> Showing the *correct result* is more important than showing the *exact moment*.

---

## 10. Key Takeaway

> **Physics resolves collisions inside fixed timesteps and only exposes valid step-end states.**
> **CCD refines the simulation internally without fragmenting time.**
> **Running physics at a higher frequency than rendering improves stability, responsiveness, and determinism.**

---

This document serves as a conceptual contract for later CCD and replay implementations.
