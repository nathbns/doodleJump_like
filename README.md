# doodleJump_like

###### The goal of this application is to create a game in the spirit of **Doodle Jump**. You control a character that moves with the arrow keys and is subjected to various forces as it climbs.

## Game controls
From the **PRESS START** screen, press **S** or **right‑click** to start a run. Move with the **left / right arrow keys** and kill monsters with a **short or long left‑click**.

You can tweak the constants **`dt`** (affects the player’s rebound speed) and **`vitesseJeu`** (global speed factor for every object). The default values are already well balanced.

## Game rules
* Touch the bottom of the window → **GAME OVER**
* Touch a monster → **GAME OVER**
* Score:  
  * Land on a standard lateral‑moving platform … **+5**  
  * Break a “broken” platform … **+10**  
  * Kill a monster … **+20**
* Shields temporarily protect the player from monsters.

## High‑level code overview
* The player is a single particle with standard forces, rendered with a sprite.
* Platforms are randomly spawned and may be:  
  * **classic**  
  * **broken** — falls when the player lands on it  
  * **moving** — slides left‑right  
  * **jump** — gives an extra‑high bounce  
  A platform can be both **moving** and **broken**.
* **Endless world**: when a platform leaves the bottom of the window, it is immediately repositioned at the top.
* **Up‑scroll illusion**: all platforms drift slowly downward; when the player lands, every platform shifts downward in a single step.
* The player bounces only when arriving from above; collisions from below are ignored for smoother gameplay.
* **Black holes**: apply an attractive force when the player enters their radius; forces are reset when the player exits.
* **Projectiles**: defined by a `Projectile` struct; rotating triangles whose colour interpolates over their lifetime; disappear when timed out or on monster impact.
* **Shield**: drops from the top; when collected, a ring appears around the player and grants temporary invulnerability.
* **Dynamic speed**:  
  * Player height between **1/3 and 3/4** of the window → speed × 1.5  
  * Above **3/4** → speed × 1.7  
  * Below **1/3** → default speed
