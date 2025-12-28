# Collision Handling Levels in a Physics Engine

This note documents **three common collision-handling strategies** used in physics engines, ordered from simplest to most robust.  
They reflect *increasing correctness, stability, and computational cost*.

This is intentionally framed as an **engine-design perspective**, not a pure physics one.

---

## 🟢 Level 1 – Educational / Minimal Engine (Current Stage)

**Characteristics**
- Small penetration (overshoot) is tolerated
- Collision *detection* is more important than perfect resolution
- Focus is on understanding time-stepping, tunneling, and CCD concepts
- Suitable for learning and early engine prototypes

**Behavior**
- Object may slightly pass the collision boundary (e.g. `x > wall_x`)
- Collision event is still detected correctly
- Visual artifact is usually unnoticeable

**Why this is acceptable**
- Fixed timestep + discrete integration cannot guarantee exact boundary hits
- CCD already ensures the collision is *not missed*
- Over-correcting early can hide deeper architectural lessons

**Typical use**
- Educational engines
- Debug builds
- Early physics architecture labs

---
Sample of the slightly pass the collision boundary from an early version of this engine:
- Frame 261 | Physics steps: 256
- PREV: x=9.77538 v=13.5274
- CURR: x=10.0018 v=-13.6878
- Frame 262 | Physics steps: 257
- PREV: x=10.0018 v=-13.6878
- CURR: x=9.7764 v=-13.5262

---
## 🟡 Level 2 – Position Correction

After detecting a collision, the engine explicitly **corrects penetration**.

### Common approaches

```cpp
m_curr.position = wall_x;
```

or

```cpp
m_curr.position = std::min(m_curr.position, wall_x);
```

**Effect**
- Penetration is eliminated immediately
- Object never visually crosses the boundary
- Collision feels more “solid”

**Trade-offs**
- Physically abrupt
- Can introduce energy artifacts if overused
- Still lacks proper constraint resolution

**Typical use**
- Indie engines
- Arcade-style physics
- Simple platformers

---

## 🔴 Level 3 – Constraint Solver / Modern Physics Engines

This is how professional physics engines operate.

**Key concepts**
- Contact manifolds
- Constraint equations
- Sequential impulse solvers
- Baumgarte stabilization or velocity bias
- Position-Based Dynamics (PBD) variants

**Behavior**
- Penetration is prevented *implicitly*
- Multiple contacts are resolved together
- Stable stacking and resting contacts
- No ad-hoc position snapping

**Trade-offs**
- Significantly more complex
- Higher CPU cost
- Requires careful numerical design

**Typical use**
- Unreal Engine (Chaos)
- Unity (PhysX / Havok)
- AAA and simulation-grade engines

---

## Summary Table

| Level | Strategy | Accuracy | Complexity | Typical Use |
|-----|--------|----------|------------|-------------|
| 🟢 | CCD + tolerance | Low–Medium | Low | Education |
| 🟡 | Position correction | Medium | Medium | Indie games |
| 🔴 | Constraint solver | High | High | Professional engines |

---

## Design Insight

> **Physics engines are not about perfect math.  
They are about stable, predictable state evolution under time constraints.**

Understanding *why* Level 1 exists is more important than prematurely jumping to Level 3.

Level 3 only makes sense **after** mastering:
- Fixed timesteps
- Accumulators
- CCD
- Replay / state rollback
- Determinism

---

