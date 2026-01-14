# Physics Solver Migration Report

## From Axis-Based Ground Solver to Contact-Based Scalable Architecture

### Purpose of This Document

This report documents a **controlled, debug-driven migration process** for a 2D physics engine collision solver. The goal is to transition from an **axis-based, ground-special-cased solver** to a **fully scalable, contact- and normal-based solver architecture**, without losing correctness or debuggability at any stage.

The document is intentionally written as an **engineering report**, not a tutorial. Each phase is structured to be:

* Verifiable
* Reversible
* Isolated

The result is a solver architecture that supports:

* Multiple dynamic bodies
* Static and dynamic contacts
* Arbitrary world geometry (floating platforms, walls, ceilings)
* Per-body gravity configuration

---

## Guiding Principles

1. **No behavioral regression is acceptable** during migration.
2. Each step must introduce **only one conceptual change**.
3. Every step must define a **minimal reproducible test case**.
4. Solver logic must not encode world-specific assumptions (e.g., “ground”).

---

## Baseline Definition

### STEP 0 — Baseline Behavior Freeze

#### Objective

Establish a reference (“golden behavior”) against which all subsequent changes are validated.

#### Test Case — TEST_00_SINGLE_BODY_GROUND

* One dynamic body
* One ground body (existing implementation)
* Gravity enabled

#### Expected Result

* Body rests on the ground
* Vertical velocity converges to zero
* No positional drift

#### Required Debug Output

```
frame_index, position.y, velocity.y
```

This output is archived and used as the reference for all later steps.

---

## Migration Phases

### STEP 1 — Introduce Static Body Abstraction

#### Objective

Decouple the concept of “ground” from solver logic by representing it as a static rigid body.

#### Change

* Ground is represented as `RigidBody` with `invMass = 0`
* Existing axis-based solver remains active

#### Test Case — TEST_01_STATIC_BODY

* One dynamic body
* One static body (invMass = 0)

#### Expected Result

Behavior must be **identical** to STEP 0.

#### Mandatory Assertion

```cpp
assert(ground.invMass == 0);
```

---

### STEP 2 — Eliminate Index-Based Role Assumptions

#### Objective

Remove implicit assumptions such as `body[1] == wall` or `body[0] == dynamic`.

#### Change

* Solver functions operate on `RigidBody* A, RigidBody* B`
* No index-based branching is permitted

#### Test Case — TEST_02_INDEX_INDEPENDENCE

* Swap ordering of dynamic and static bodies in storage

#### Expected Result

Solver behavior remains unchanged.

#### Required Debug Output

```
A.invMass, B.invMass
```

---

### STEP 3 — Replace Axis Checks with Geometric Penetration

#### Objective

Remove hard-coded Y-axis conditions and move to shape-based penetration computation.

#### Change

* Penetration depth computed geometrically (AABB or shape overlap)
* Still constrained to vertical normal `(0,1)` for this step

#### Test Case — TEST_03_PENETRATION_RESOLUTION

* Dynamic body initialized with slight penetration into static body

#### Expected Result

* Penetration decreases monotonically
* Body is separated without oscillation

#### Invariant

```
penetration(t + 1) ≤ penetration(t)
```

---

### STEP 4 — Introduce Contact Abstraction

#### Objective

Ensure solvers operate exclusively on **contact data**, not world or body collections.

#### Change

```cpp
struct Contact {
    RigidBody* A;
    RigidBody* B;
    Vec2 normal;
    float penetration;
};
```

#### Test Case — TEST_04_SINGLE_CONTACT

* One contact between dynamic body and static body

#### Expected Result

No behavioral change relative to STEP 3.

#### Mandatory Assertion

```cpp
assert(dot(contact.normal, Vec2(0,1)) == 1);
```

---

### STEP 5 — Transition to Normal-Based Impulse Solver

#### Objective

Fully remove axis-specific solvers and activate impulse-based resolution.

#### Change

* Disable legacy Y-axis solver
* Enable velocity impulse solver
* Enable positional correction solver

#### Test Case — TEST_05_IMPULSE_GROUND_CONTACT

* Dynamic body resting on static body

#### Expected Result

* Stable resting contact
* No jitter
* No artificial energy gain

#### Required Debug Output

```
relative_velocity, velAlongNormal, impulse_scalar, position_correction
```

---

### STEP 6 — Dynamic–Dynamic Collision Validation

#### Objective

Verify solver symmetry and correctness without static assumptions.

#### Change

* Gravity disabled

#### Test Case — TEST_06_DYNAMIC_DYNAMIC_CONTACT

* Two dynamic bodies initialized with overlap

#### Expected Result

* Bodies separate symmetrically
* Separation proportional to inverse mass

#### Mandatory Assertion

```cpp
assert(A.invMass > 0 && B.invMass > 0);
```

---

### STEP 7 — Per-Body Gravity Integration

#### Objective

Decouple gravity from collision response and enable per-body control.

#### Change

```cpp
force += gravity * gravityScale * mass;
```

#### Test Case — TEST_07_GRAVITY_SCALING

* Body A: gravityScale = 1
* Body B: gravityScale = 0

#### Expected Result

* Body A falls
* Body B remains stationary

---

### STEP 8 — Multi-Contact Stack Stability

#### Objective

Validate solver stability under stacked contacts.

#### Test Case — TEST_08_STACKED_BODIES

* Three dynamic bodies stacked on one static body

#### Expected Result

* No excessive compression
* Penetration decreases with solver iterations

#### Required Debug Output

```
max_penetration_per_iteration
```

---

### STEP 9 — Floating Platform Validation

#### Objective

Prove complete removal of world-origin and ground assumptions.

#### Test Case — TEST_09_FLOATING_PLATFORM

* Static platform positioned above origin
* Dynamic body falling onto platform

#### Expected Result

* Body rests on platform
* No dependency on Y=0 or global ground logic

---

## Final Outcome

After completing all steps, the physics engine will have:

* A contact-driven collision architecture
* Full support for static and dynamic interactions
* Arbitrary world geometry support
* Deterministic, debuggable solver behavior

This document is intended to remain as a **long-term engineering reference** and can be extended to cover friction, restitution, CCD, and joint constraints.

---

**Recommendation:** Each step should correspond to a single source-control commit, validated exclusively by its associated test case before proceeding.
