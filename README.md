# Solar Explorer 2D - A way to experince the best solar system

A 2D solar system explorer sandbox that works as an encyclopedia of the solar system displaying information about planet's properties, atmospheres, orbits and much more. 
Uses numerical solutions to calculate the orbits of planets at scale and represents them with data from NASA/JPL Solar System Dynamics and NASA NSSDCA Planetary Fact Sheets in the J2000 orientation system

# Why this exists
It is a passion project and an introduction to a much larger 3D solar system that will include moons, spacecraft and many more features. 
It emerges from a profound admiration to our solar system. The project tries to be a tool for assitance to learning.

# The physics behind it

The normal movement of celestial bodies is achived by calculating the force exercised on them from the sun by the universal law of gravitation:

## Physics & Orbital Mechanics

| #   | Quantity                          | Equation                                                                  | Short explanation |
| --- | --------------------------------- | ------------------------------------------------------------------------- | ----------------- |
| 1   | Gravitational parameter           | $\mu = GM$                                                                | Just mass times gravity constant. Makes the equations cleaner. |
| 2   | Mean density                      | $\rho = \frac{m}{\frac{4}{3}\pi R^3}$                                     | How dense the planet is on average. Volume of a sphere. |
| 3   | Surface gravity                   | $g = \frac{GM}{R^2}$                                                      | How hard gravity pulls you on the surface. |
| 4   | Escape velocity                   | $v_{\mathrm{esc}} = \sqrt{\frac{2GM}{R}}$                                 | Speed you need to never fall back. |
| 5   | Distance from the Sun             | $r = \sqrt{x^2+y^2}$                                                      | Straight-line distance using x and y. |
| 6   | Orbital speed                     | $v = \sqrt{v_x^2+v_y^2}$                                                  | How fast you're actually going. |
| 7   | Specific orbital energy           | $\epsilon = \frac{v^2}{2}-\frac{\mu}{r}$                                  | Energy per mass. Negative means you're stuck in orbit. |
| 8   | Specific angular momentum         | $h = xv_y-yv_x$                                                           | How much the orbit is spinning (like cross product). |
| 9   | Semi-major axis                   | $a = -\frac{\mu}{2\epsilon}$                                              | Half the long axis of the ellipse. Bigger = slower orbit. |
| 10  | Orbital eccentricity              | $e = \sqrt{1+\frac{2\epsilon h^2}{\mu^2}}$                                | How oval the orbit is. 0 = perfect circle. |
| 11  | Semi-latus rectum                 | $p = \frac{h^2}{\mu}$                                                     | Kind of the "width" of the orbit at the focus. |
| 12  | Orbital period                    | $T = 2\pi\sqrt{\frac{a^3}{\mu}}$                                          | How long one full trip around takes. Kepler's 3rd law. |
| 13  | Mean motion                       | $n = \sqrt{\frac{\mu}{a^3}}$                                              | Average angular speed (radians per time). |
| 14  | Mean anomaly                      | $M = M_0+nt$                                                              | Fake angle that increases steadily with time. |
| 15  | Kepler's equation                 | $M = E-e\sin E$                                                           | Connects the fake angle M to the real eccentric anomaly E. |
| 16  | Newton-Raphson iteration          | $E_{n+1}=E_n-\frac{E_n-e\sin E_n-M}{1-e\cos E_n}$                         | Smart guess-and-check to solve Kepler's equation. |
| 17  | True anomaly                      | $\cos\nu=\frac{\cos E-e}{1-e\cos E}$                                      | The actual angle from the periapsis. |
| 18  | Orbital radius                    | $r=a(1-e\cos E)$                                                          | How far you are from the focus right now. |
| 19  | Solar irradiance                  | $I=\frac{L_\odot}{4\pi r^2}$                                              | How much sunlight hits you (power per area). |
| 20  | Blackbody equilibrium temperature | $T_{\mathrm{eq}}=\left(\frac{L_\odot(1-A)}{16\pi\sigma r^2}\right)^{1/4}$ | Temperature a planet would settle at if it was a perfect blackbody. |

### Keplerian propagation

The orbital propagation system uses:

$$
t\rightarrow M\rightarrow E\rightarrow\nu\rightarrow r\rightarrow(x,y)
$$

with Kepler's equation solved numerically using Newton-Raphson.




**Implicit Euler Numerical Integration**

This simulation uses the implicit euler numerical integration, which produces substantial numerical error with time.
It would have been better to use the *Runge-Kutta 4* or **RK4** as it is much more precise.

# Known limitations
1. This simulation uses the implicit euler numerical integration, which produces substantial numerical error with time.
It would have been better to use the *Runge-Kutta 4* or **RK4** as it is much more precise.
2. The method used for numerical integration is not reliable and generates a big numerical error.
3. The zoom-in-to-planet feature produces unexpected behaviour when the simulation isn't stopped
4. Ineficient use of many textures for displaying text
5. When selecting a date, the code that handles the change of years fails to account for leap years.
6. The entirity of the project's code is nested in a single file with makes it very hard to modify and expand

# Repository structure

```
├── assets
│   ├── extra
│   │   ├── calendar.png
│   │   ├── pause.png
│   │   ├── raptor.png
│   │   └── solarSystem.png
│   ├── font
│   │   ├── Monocraft.ttf
│   │   └── times.ttf
│   ├── planets
│   │   ├── earth.png
│   │   ├── jupiter.png
│   │   ├── mars.png
│   │   ├── mercury.png
│   │   ├── neptune.png
│   │   ├── pluto.png
│   │   ├── saturn.png
│   │   ├── sun.png
│   │   ├── uranus.png
│   │   └── venus.png
│   └── text
│       ├── earthText.png
│       ├── earthWeather.png
│       ├── jupiterText.png
│       ├── jupiterWeather.png
│       ├── marsText.png
│       ├── marsWeather.png
│       ├── mercuryText.png
│       ├── mercuryWeather.png
│       ├── neptuneText.png
│       ├── neptuneWeather.png
│       ├── plutoText.png
│       ├── plutoWeather.png
│       ├── saturnText.png
│       ├── saturnWeather.png
│       ├── sunText.png
│       ├── sunWeather.png
│       ├── uranusText.png
│       ├── uranusWeather.png
│       ├── venusText.png
│       └── venusWeather.png
├── info
│   └── basicGravitationPhysicsExplanation.txt
├── linux
│   ├── assets
│   │   ├── extra
│   │   ├── font
│   │   ├── planets
│   │   └── text
│   └── SolarExplorer2D
├── raylib/
├── README.md
├── src
│   ├── assets
│   │   ├── extra
│   │   ├── font
│   │   ├── planets
│   │   └── text
│   └── main.c
└── windows
    ├── assets
    │   ├── extra
    │   ├── font
    │   ├── planets
    │   └── text
    ├── libmcfgthread-2.dll
    ├── libraylib.a
    ├── libraylibdll.a
    ├── raylib.dll
    ├── raylib.h
    └── SolarExplorer2D.exe
```

# Images
![1](Screenshoots/Solar-Explorer-2D_1.png)
![2](Screenshoots/Solar-Explorer-2D_2.png)
![3](Screenshoots/Solar-Explorer-2D_3.png)
![4](Screenshoots/Solar-Explorer-2D_4.png)
![5](Screenshoots/Solar-Explorer-2D_5.png)
![6](Screenshoots/Solar-Explorer-2D_6.png)

# Installation guide

**Build from source:**
```bash
cd src/
gcc main.c -o main -I../raylib/src -L../raylib/src -lraylib -lm -lpthread -ldl -lrt -lX11 
./main
```

**Linux:**

Run the standalone executable:
```bash
cd linux/
./SolarExplorer2D
```


**Windows:**

Run the executable in the same folder as the libraries:
```powershell
cd windows/
./SolarExplorer2D.exe
```

# Author

Ignacio Martinez Heras · 2026

Built along with the fascination to the solar system.