# Solar Explorer 2D - A way to experince the best solar system

A 2D solar system explorer sandbox that works as an encyclopedia of the solar system displaying information about planet's properties, atmospheres, orbits and much more. 
Uses numerical solutions to calculate the orbits of planets at scale and represents them with data from NASA/JPL Solar System Dynamics and NASA NSSDCA Planetary Fact Sheets in the J2000 orientation system

# Why this exists
It is a passion project and an introduction to a much larger 3D solar system that will include moons, spacecraft and many more features. 
It emerges from a profound admiration to our solar system. The project tries to be a tool for assitance to learning.

# The physics behind it
**Orbital Movement**

The normal movement of celestial bodies is achived by calculating the force exercised on them from the sun by the universal law of gravitation:

$$
F = \frac{GMm}{r^2}
$$

Gravitational parameter: The standard gravitational parameter used throughout the orbital calculations.

$$
\mu = GM
$$

Specific orbital energy: Describes the orbital energy per unit mass and determines the semi-major axis. 

$$
\epsilon = \frac{v^2}{2}-\frac{\mu}{r}
$$


Semi-major axis: Determines the size of the orbit from the current position and velocity.

$$
a=-\frac{\mu}{2\epsilon}
$$

Determines the size of the orbit from the current position and velocity.

Specific angular momentum: The conserved angular momentum per unit mass in the 2D model.

$$
h=xv_y-yv_x
$$


Orbital eccentricity: Determines the shape of the orbit.

$$
e=
\sqrt{
1+\frac{2\epsilon h^2}{\mu^2}
}
$$


Orbital period: Kepler's third law in gravitational form.

$$
T=2\pi\sqrt{\frac{a^3}{\mu}}
$$


Mean motion: Determines how quickly the mean anomaly advances with time.

$$
n=\sqrt{\frac{\mu}{a^3}}
$$


Kepler's equation: Relates mean anomaly to eccentric anomaly and is solved numerically using Newton-Raphson method.


$$
M=E-e\sin E
$$


Orbital radius: Converts the eccentric anomaly into the planet's distance from the Sun.

$$
r=a(1-e\cos E)
$$


Polar equation of the orbit: Describes the complete elliptical orbit, including its orientation $\omega$.

$$
r(\theta)=
\frac{a(1-e^2)}
{1+e\cos(\theta-\omega)}
$$


**Implicit Euler Numerical Integration**

# Images

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