#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define SCREEN_HALF_X 900
#define SCREEN_HALF_Y 450
#define AU 1.496e+11
#define G 6.6743e-11
#define LuminositySun 3.828e26
#define StefanBoltzmannConstant 5.670374e-8
#define R 8.314
#define rocheLimitConstant 2.44
#define dateSeconds 6696000

Texture2D raptor, calendarIcon, solarSystem, sunTexture, sunWeather, sunText, mercuryTexture, mercuryWeather, mercuryText, venusTexture, venusWeather, venusText, earthTexture, earthWeather, earthText, marsTexture, marsWeather, marsText, jupiterTexture, jupiterWeather, jupiterText, saturnTexture, saturnWeather, saturnText, uranusTexture, uranusWeather, uranusText, neptuneTexture, neptuneWeather, neptuneText, plutoTexture, plutoWeather, plutoText;
Font monocraft, timesNewRoman;
Color blue1, blue2, red1, red2, orange1, orange2, purple1, purple2, green1, green2, verydarkgray, darkRed, drakGreen, red, green, blue, lightBlue;
bool orbiting, mousePressed, options, calendarToggle, planetsUI, moved, only, orbitsToggle, rocketToggle;

double DT = 0.01;
double zoomFactorInit;
double zoomFactor;
double elapsedSeconds, seconds1000Years;
int celestialNum, targetFps;
int started;
int numPoints;

char date[100];
char rateString[128];
    
double firstDTmin;
double firstDTmax;

int planetSelected, framesTicked, planetgoingTo;
double epsilon;

Vector2 orbits[10][10000];
Vector2 cursorPos;
Vector2 points[600];
Vector2 orbitsDraw[10][10000]; 
Vector2 deltaWorld;
Rectangle pauseButton, previousButton, nextButton, optionsButton, planetsButton, calendarButton, rocketButton, support, support2;

typedef struct {
    bool on;
    char name[64];
    Rectangle inside;
    Color colorInside;
    Color colorBorder;
    Font font;
    int fontSize;
    double roundness;
    Vector2 extra;
} Button;



Button physicalProperties, atmosphericProperties,  orbitalProperties, derivedValues, orbitTrailsButton;


typedef struct  {
    Texture2D texture;
    Texture2D text;
    Texture2D weather;
    Vector2 pos; //m
    Vector2 drawPos;
    Vector2 velocity; //ms
    
    double drawRadius;
    double mass; //kg
    double density; //k/m³
    double initialDistance, initialAngle, initialTangentialVelocity, initialRadialVelocity;
    double surfaceGravity;
    double rotationPeriod;
    double axialTilt, inclination;
    double averageDistance;
    double escapeVelocity, escapeVelocityPlanet, netVelocity;
    double r;
    double blackBodyEquilibrium, bondAlbedo, solarIrradiance;
    double hillSphereRadius, sphereOfInfluenceRadius;
    double orbitalPeriod, orbitalVelocity;
    double eccentricity;
    double perihelion, aphelion;
    double semiMinorAxis, semiMajorAxis;
    double radialVelocity, tangentialVelocity, angularMomentum;
    double gravitationalParameter, specificOrbitalEnergy;
    double rocheLimit;
    double estimatedAtmosphericScaleHeight, temperature, meanMolarMass;
    double radius; //m
    
    char name[32];
    Color color;


} Planet;


Planet sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto;
Planet *celestialBodies[10];


void setInitialData() {
    sun = (Planet){
        .texture = sunTexture,
        .weather = sunWeather,
        .text = sunText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 1.9891e30,
        .radius = 695700000,
        .initialDistance = 0,
        .initialAngle = 0,
        .initialRadialVelocity = 0,
        .initialTangentialVelocity = 0,
        .color = {255, 200, 40, 255},
        .averageDistance = 0,
        .axialTilt = 7.25,
        .rotationPeriod = 2192832,
        .bondAlbedo = 1,
        .temperature =  5778,
        .meanMolarMass =  0.00061,
        .inclination = 0
    };
    mercury = (Planet){
        .texture = mercuryTexture,
        .weather = mercuryWeather,
        .text = mercuryText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 3.301e23,
        .radius = 2439700,
        .initialDistance = 0.4625487839 * AU,
        .initialAngle = 242.262457,
        .initialRadialVelocity = 2626.381,
        .initialTangentialVelocity = 39206.980,
        .color = {150, 150, 145, 255},
        .averageDistance = 5.7909e10,
        .axialTilt = 0.034,
        .rotationPeriod = 5067031.68,
        .bondAlbedo = 0.088,
        .temperature =  440,
        .meanMolarMass = 0.04,
        .inclination = 7
    };
    venus = (Planet){
        .texture = venusTexture,
        .weather = venusWeather,
        .text = venusText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 4.867e24,
        .radius = 6051800,
        .initialDistance = 0.7273695869 * AU,
        .initialAngle = 277.021284,
        .initialRadialVelocity = 134.257,
        .initialTangentialVelocity = 34825.559,
        .color = {205, 190, 150, 255},
        .averageDistance = 1.0821e11,
        .axialTilt = 177.36,
        .rotationPeriod = -20996755.2,
        .bondAlbedo = 0.76,
        .temperature = 737,
        .meanMolarMass = 0.435,
        .inclination = 3.39
        
    };
    earth = (Planet){
        .texture = earthTexture,
        .weather = earthWeather,
        .text = earthText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 5.97219e24,
        .radius = 6371000,
        .initialDistance = 0.9833266688 * AU,
        .initialAngle = 100.209657,
        .initialRadialVelocity = 31.140,
        .initialTangentialVelocity = 30273.737,
        .color = {70, 120, 180, 255},
        .averageDistance = 1.49598e11,
        .axialTilt = 23.44,
        .rotationPeriod = 86164.100352,
        .bondAlbedo = 0.306,
        .temperature = 288,
        .meanMolarMass =  0.02897,
        .inclination = 0
    };
    mars = (Planet){
        .texture = marsTexture,
        .weather = marsWeather,
        .text = marsText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 6.417e23,
        .radius = 3389500,
        .initialDistance = 1.4286940851 * AU,
        .initialAngle = 283.796552,
        .initialRadialVelocity = -1792.521,
        .initialTangentialVelocity = 25621.042,
        .color = {190, 85, 55, 255},
        .averageDistance = 2.2794e11,
        .axialTilt = 25.19,
        .rotationPeriod = 88642.664064,
        .bondAlbedo = 0.25,
        .temperature = 210,
        .meanMolarMass = 0.0434,
        .inclination = 1.85
    };
    jupiter = (Planet){
        .texture = jupiterTexture,
        .weather = jupiterWeather,
        .text = jupiterText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 1.89813e27,
        .radius = 71488000,
        .initialDistance = 5.2118937552 * AU,
        .initialAngle = 108.967359,
        .initialRadialVelocity = 627.707,
        .initialTangentialVelocity = 13026.749,
        .color = {205, 135, 75, 255},
        .averageDistance = 7.7857e11,
        .axialTilt = 3.13,
        .rotationPeriod = 35729.856,
        .bondAlbedo = 0.503,
        .temperature = 165,
        .meanMolarMass = 0.0023,
        .inclination = 1.30
    };
    saturn = (Planet){
        .texture = saturnTexture,
        .weather = saturnWeather,
        .text = saturnText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 5.68e26,
        .radius = 58232000,
        .initialDistance = 9.5185403152 * AU,
        .initialAngle = 1.552905,
        .initialRadialVelocity = 535.040,
        .initialTangentialVelocity = 9653.393,
        .color = {185, 195, 145, 255},
        .averageDistance = 1.4335e12,
        .axialTilt = 26.73,
        .rotationPeriod = 38362.464,
        .bondAlbedo = 0.342,
        .temperature = 134,
        .meanMolarMass = 0.0023,
        .inclination = 2.49
    };
    uranus = (Planet){
        .texture = uranusTexture,
        .weather = uranusWeather,
        .text = uranusText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 8.681e25,
        .radius = 25362000,
        .initialDistance = 19.4901325026 * AU,
        .initialAngle = 59.539656,
        .initialRadialVelocity = -305.873,
        .initialTangentialVelocity = 6703.608,
        .color = {120, 190, 200, 255},
        .averageDistance = 2.8725e12,
        .axialTilt = 97.77,
        .rotationPeriod = -62063.712,
        .bondAlbedo = 0.3,
        .temperature = 76,
        .meanMolarMass = 0.0026,
        .inclination = 0.77
    };
    neptune = (Planet){
        .texture = neptuneTexture,
        .weather = neptuneWeather,
        .text = neptuneText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 1.024092e26,
        .radius = 24622000,
        .initialDistance = 29.8848069319 * AU,
        .initialAngle = 0.995247,
        .initialRadialVelocity = -44.348,
        .initialTangentialVelocity = 5468.412,
        .color = {60, 100, 190, 255},
        .averageDistance = 4.4951e12,
        .axialTilt = 28.32,
        .rotationPeriod = 57996,
        .bondAlbedo = 0.29,
        .temperature = 72,
        .meanMolarMass = 0.0026,
        .inclination = 1.77
    };
    pluto = (Planet){
        .texture = plutoTexture,
        .weather = plutoWeather,
        .text = plutoText,
        .pos = {0, 0},
        .drawPos = {SCREEN_HALF_X, SCREEN_HALF_Y},
        .velocity = {0, 0},
        .mass = 1.3025e22,
        .radius = 1188300,
        .initialDistance = 30.23085 * AU,
        .initialAngle = 302.961158,
        .initialRadialVelocity = 518.15,
        .initialTangentialVelocity = 5996.03,
        .color = {170, 160, 145, 255},
        .averageDistance = 5.9064e12,
        .axialTilt = 122.53,
        .rotationPeriod = -551854.08,
        .bondAlbedo = 0.6,
        .temperature = 44,
        .meanMolarMass = 0.028,
        .inclination = 17.14
    };
    
    celestialBodies[0] = &sun;
    celestialBodies[1] = &mercury;
    celestialBodies[2] = &venus;
    celestialBodies[3] = &earth;
    celestialBodies[4] = &mars;
    celestialBodies[5] = &jupiter;
    celestialBodies[6] = &saturn;
    celestialBodies[7] = &uranus;
    celestialBodies[8] = &neptune;
    celestialBodies[9] = &pluto;

    strcpy(sun.name, "Sun");
    strcpy(mercury.name, "Mercury");
    strcpy(venus.name, "Venus");
    strcpy(earth.name, "Earth");
    strcpy(mars.name, "Mars");
    strcpy(jupiter.name, "Jupiter");
    strcpy(saturn.name, "Saturn");
    strcpy(uranus.name, "Uranus");
    strcpy(neptune.name, "Neptune");
    strcpy(pluto.name, "Pluto");
}

void loadFiles() {
    sunTexture = LoadTexture("assets/planets/sun.png");
    sunWeather = LoadTexture("assets/text/sunWeather.png");
    sunText = LoadTexture("assets/text/sunText.png");
    mercuryTexture = LoadTexture("assets/planets/mercury.png");
    mercuryWeather = LoadTexture("assets/text/mercuryWeather.png");
    mercuryText = LoadTexture("assets/text/mercuryText.png");
    venusTexture = LoadTexture("assets/planets/venus.png");
    venusWeather = LoadTexture("assets/text/venusWeather.png");
    venusText = LoadTexture("assets/text/venusText.png");
    earthTexture = LoadTexture("assets/planets/earth.png");
    earthWeather = LoadTexture("assets/text/earthWeather.png");
    earthText = LoadTexture("assets/text/earthText.png");
    marsTexture = LoadTexture("assets/planets/mars.png");
    marsWeather = LoadTexture("assets/text/marsWeather.png");
    marsText = LoadTexture("assets/text/marsText.png");
    jupiterTexture = LoadTexture("assets/planets/jupiter.png");
    jupiterWeather = LoadTexture("assets/text/jupiterWeather.png");
    jupiterText = LoadTexture("assets/text/jupiterText.png");
    saturnTexture = LoadTexture("assets/planets/saturn.png");
    saturnWeather = LoadTexture("assets/text/saturnWeather.png");
    saturnText = LoadTexture("assets/text/saturnText.png");
    uranusTexture = LoadTexture("assets/planets/uranus.png");
    uranusWeather = LoadTexture("assets/text/uranusWeather.png");
    uranusText = LoadTexture("assets/text/uranusText.png");
    neptuneTexture = LoadTexture("assets/planets/neptune.png");
    neptuneWeather = LoadTexture("assets/text/neptuneWeather.png");
    neptuneText = LoadTexture("assets/text/neptuneText.png");
    plutoTexture = LoadTexture("assets/planets/pluto.png");
    plutoWeather = LoadTexture("assets/text/plutoWeather.png");
    plutoText = LoadTexture("assets/text/plutoText.png");

    monocraft = LoadFont("assets/font/Monocraft.ttf");
    timesNewRoman = LoadFont("assets/font/times.ttf");
    solarSystem = LoadTexture("assets/extra/solarSystem.png");
    calendarIcon = LoadTexture("assets/extra/calendar.png");
    raptor = LoadTexture("assets/extra/raptor.png");
}

void setButtons() {
    orbitalProperties = (Button){
        .inside = (Rectangle){1335, 320, 200, 40},
        .name = "Orbital Properties",
        .font = timesNewRoman,
        .fontSize = 25,
        .on = true,
        .roundness = 0.4,
        .colorInside = purple1,
        .colorBorder = purple2,
        .extra = (Vector2){13, 8}
    };

    derivedValues = (Button){
        .inside = (Rectangle){1550, 320, 220, 40},
        .name = "Derived Values",
        .font = timesNewRoman,
        .fontSize = 25,
        .on = false,
        .roundness = 0.4,
        .colorInside = green1,
        .colorBorder = green2,
        .extra = (Vector2){40, 8}
    };

    physicalProperties = (Button){
        .inside = (Rectangle){1335, 375, 200, 40},
        .name = "Physical Properties",
        .font = timesNewRoman,
        .fontSize = 25,
        .on = false,
        .roundness = 0.4,
        .colorInside = blue1,
        .colorBorder = blue2,
        .extra = (Vector2){8, 8}
    };

    atmosphericProperties = (Button){
        .inside = (Rectangle){1550, 375, 220, 40},
        .name = "Atmospheric Properties",
        .font = timesNewRoman,
        .fontSize = 22,
        .on = false,
        .roundness = 0.4,
        .colorInside = red1,
        .colorBorder = red2,
        .extra = (Vector2){8, 8}
    };

    orbitTrailsButton = (Button){
        .colorBorder = verydarkgray,
        .colorInside = DARKGRAY,
        .font = timesNewRoman,
        .fontSize = 35,
        .name = "Orbit Trails",
        .on = true,
        .roundness = 0.4,
        .extra = (Vector2){23, 10},
        .inside = (Rectangle){20, 200, 200, 50}
    };


}

void setInitialPos(Planet *planet[]) {
    for (int i = 0; i <= celestialNum; i++) {
        planet[i]->pos.x = cosf(planet[i]->initialAngle * DEG2RAD) * planet[i]->initialDistance;
        planet[i]->pos.y = sinf(planet[i]->initialAngle * DEG2RAD) * planet[i]->initialDistance;
        planet[i]->drawPos.x = planet[i]->pos.x * zoomFactor + SCREEN_HALF_X;
        planet[i]->drawPos.y = planet[i]->pos.y * zoomFactor + SCREEN_HALF_Y;
        planet[i]->drawRadius = planet[i]->radius * zoomFactor;
        
        planet[i]->initialTangentialVelocity *= -1;
        planet[i]->velocity.x = planet[i]->initialRadialVelocity * cosf(planet[i]->initialAngle*DEG2RAD) - planet[i]->initialTangentialVelocity * sinf(planet[i]->initialAngle*DEG2RAD);
        planet[i]->velocity.y = planet[i]->initialRadialVelocity * sinf(planet[i]->initialAngle*DEG2RAD) + planet[i]->initialTangentialVelocity * cosf(planet[i]->initialAngle*DEG2RAD);
        
        printf("%s: V: %f, Vx: %f, Vy: %f, Tangential V: %f, Radial V: %f Distance: %f, Degrees: %f\n", planet[i]->name, sqrtf(planet[i]->velocity.x * planet[i]->velocity.x + planet[i]->velocity.y * planet[i]->velocity.y), planet[i]->velocity.x, planet[i]->velocity.y, planet[i]->initialTangentialVelocity, planet[i]->initialRadialVelocity, planet[i]->initialDistance, planet[i]->initialAngle);
    }
}

void movePlanet(Planet *planet[]) {
    for (int i = 0; i <= celestialNum; i++) {
        planet[i]->drawPos.x = (planet[i]->pos.x + deltaWorld.x) *zoomFactor + SCREEN_HALF_X;
        planet[i]->drawPos.y = (planet[i]->pos.y + deltaWorld.y) *zoomFactor + SCREEN_HALF_Y;
    }

}

void drawPlanet(Planet *planet[], Font font) {
    for (int i = 0; i <= celestialNum; i++) {
        
        double minRadius = 8;
        if (planet[i]->drawRadius < minRadius)  {
            DrawCircle(planet[i]->drawPos.x, planet[i]->drawPos.y, minRadius-1, BLACK);
            DrawCircleLines(planet[i]->drawPos.x, planet[i]->drawPos.y, minRadius, planet[i]->color);
        }

        DrawTexturePro(planet[i]->texture, (Rectangle){0, 0, planet[i]->texture.width, planet[i]->texture.height}, (Rectangle){planet[i]->drawPos.x - planet[i]->drawRadius, planet[i]->drawPos.y - planet[i]->drawRadius, 2*planet[i]->drawRadius, 2*planet[i]->drawRadius}, (Vector2){0, 0}, 0.0, RAYWHITE);
        
        if (planet[i]->drawRadius < 10) {
            DrawTextPro(font, planet[i]->name, (Vector2){planet[i]->drawPos.x-(int)(strlen(planet[i]->name) *4), planet[i]->drawPos.y-25}, (Vector2){0, 0}, 0, 15, 1, WHITE);
        }
    }
}

void adjust(Planet *planet[], double coef) {
    for (int i = 0; i <= celestialNum; i++) {
        planet[i]->drawPos.x = (planet[i]->drawPos.x - SCREEN_HALF_X) * coef + SCREEN_HALF_X;
        planet[i]->drawPos.y = (planet[i]->drawPos.y - SCREEN_HALF_Y) * coef + SCREEN_HALF_Y;
        planet[i]->drawRadius *= coef;
    }

    
}

void applyGravity(Planet *planet[]) {
    double dx, dy, r, gravF, a, ax, ay;
    
    for (int i = 1; i <= celestialNum; i++) {
        dx = planet[i]->pos.x;
        dy = planet[i]->pos.y;

        r = sqrtf(dx*dx+ dy*dy);
        
        gravF = G * ((planet[i]->mass * sun.mass) / (r*r));
        
        a = gravF / planet[i]->mass;

        ax = -a * (dx/r);
        ay = -a * (dy/r);
        
        planet[i]->velocity.x += ax * DT;
        planet[i]->velocity.y += ay * DT;
        
        planet[i]->pos.y += planet[i]->velocity.y * DT;
        planet[i]->pos.x += planet[i]->velocity.x * DT;
        

        
        planet[i]->drawPos.x += planet[i]->velocity.x * DT * zoomFactor;
        planet[i]->drawPos.y += planet[i]->velocity.y * DT * zoomFactor;

    }
}

void getDate(double DT, int frames, char *date) {
    if (orbiting && framesTicked%11 == 0) {
    
    elapsedSeconds += frames * DT;
    time_t timestamp = (time_t)(elapsedSeconds+seconds1000Years);

    struct tm *info = gmtime(&timestamp);
    char *months[] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    };
    
    
    char *monthName = months[info->tm_mon];
    int day = info->tm_mday;
    int year = info->tm_year + 930;
    int hour = info->tm_hour;
    int minute = info->tm_min;
    int second = info->tm_sec;

    sprintf(date, "%s %02d, %04d  %02d:%02d:%02d UTC", monthName, day, year, hour, minute, second);    
    
    }
    
}

void getRate(double DT, char *rate, bool orbiting) {
    double realDT  = fabs(DT*100);

    int seocndsInMinute = 60;
    int secondsInHour = seocndsInMinute * 60;
    int secondsInDay = secondsInHour * 24;
    int secondsInWeek = secondsInDay * 7;
    int secondsInMonth = secondsInDay * 30;
    int secondsInYear = secondsInDay * 365;

    

    if (realDT == 1) {
        if (orbiting) snprintf(rate, 128, " Real Time");
        else snprintf(rate, 128, "  Paused");
    }
    else if (realDT < seocndsInMinute) {
        snprintf(rate, 128, "%d secs/sec", (int)realDT);
    } else if (realDT < secondsInHour) {
        snprintf(rate, 128, "%d mins/sec", (int)(realDT/seocndsInMinute+0.5));
    } else if (realDT < secondsInDay) {
        snprintf(rate, 128, "%d hours/sec", (int)(realDT/secondsInHour+0.5));
    } else if (realDT < secondsInWeek) {
        snprintf(rate, 128, "%d days/sec", (int)(realDT/secondsInDay+0.5));
    } else if (realDT < secondsInMonth) {
        snprintf(rate, 128, "%d weeks/sec", (int)(realDT/secondsInWeek+0.5));
    } else if (realDT < secondsInYear) {
        snprintf(rate, 128, "%d months/sec", (int)(realDT/secondsInMonth+0.5)); 
    } else {
        snprintf(rate, 128, "%d years/sec", (int)(realDT/secondsInYear+0.5));
    }
    
    if (DT < 0) {
        char temp[128];
        snprintf(temp, sizeof(temp), "-%s", rate);
        strcpy(rate, temp);
    }
}

double jumpRate(double DT, bool direction) {
    
    double jumps[] = {-31536000, -1576800, -630720, -315360, -77760, -25920, -12096, -6048, 
        -864, -432, -144, -72, -36, -18, -6, -3, -1.8, 
                    0.01, 1.8, 3, 6, 18, 36, 72, 144, 432, 864, 
                    6048, 12096, 25920, 77760, 315360, 630720, 1576800, 3153600};
                    
    double newDT;
    int length = sizeof(jumps) / sizeof(jumps[0]);
    
    if (direction) {
        for (int i = 0; i < length; i++) {
            if (jumps[i] <= DT && DT < jumps[i+1]) {
                newDT = jumps[i+1];
                break;
            } 
        }
    } else {
        for (int i = 0; i < length; i++) {
            if (jumps[i] < DT && DT <= jumps[i+1]) {
                newDT = jumps[i];
                break;
            } 
        }
    }
    
    if (newDT > jumps[length-1]) newDT = jumps[length-1] -20;
    if (newDT < jumps[0]) newDT = jumps[0]+1;
    
    return newDT;
}

void drawButton(Button *button) {

    Color colorBorder = button->colorBorder;
    Color colorInside = button->colorInside;

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle)button->inside)) {
        colorBorder = button->colorInside;
        colorInside = button->colorBorder;
    } 
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle)button->inside)) {
        orbitalProperties.on = false;
        physicalProperties.on = false;
        atmosphericProperties.on = false;
        derivedValues.on = false;
        button->on = true;
    }

    DrawRectangleRounded(button->inside, button->roundness, 20, colorInside);
    DrawRectangleRoundedLinesEx(button->inside, button->roundness, 20, 3, colorBorder);
    DrawTextEx(button->font, button->name, (Vector2){button->inside.x+button->extra.x, button->inside.y+button->extra.y}, button->fontSize, 1, WHITE);

    if (button->on && button != &orbitTrailsButton) DrawTextEx(button->font, button->name, (Vector2){1350, 435}, button->fontSize+15, 1, (Color){(button->colorInside.r +255)/2, (button->colorInside.g +255)/2, (button->colorInside.b +255)/2, 255});


}

void infoPlanets(Planet *planet[], Font font, Font font2) {

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){1710, 20, 40, 40})) {
        planetSelected = 0;
    }

    for (int i = 0; i <= celestialNum; i++) {
        int radius = (planet[i]->drawRadius > 5) ?  planet[i]->drawRadius : 5;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(), (Vector2)planet[i]->drawPos, radius)) {
            planetSelected = i+1;
        } 
    }
    
    
    int i = planetSelected -1;
    
    int j =planetSelected-1;

    
    if (i != -1) {
        Rectangle support = {1300, 0, 1000, 900};
        DrawRectangleRounded((Rectangle)support, 0.3, 50, (Color){0, 0, 0, 160});
        
        DrawTextPro(font, planet[i]->name, (Vector2){1500, 25}, (Vector2){0, 0}, 0, 45, 1, WHITE);
        DrawLineEx((Vector2){1730, 40}, (Vector2){1750, 60}, 2, GRAY);
        DrawLineEx((Vector2){1730, 60}, (Vector2){1750, 40}, 2, GRAY);
        DrawTexturePro(planet[i]->texture, (Rectangle){0, 0, planet[i]->texture.width, planet[i]->texture.height}, (Rectangle){1350, 100, 100, (int)(100.0 / planet[i]->texture.width * planet[i]->texture.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
        DrawTexturePro(planet[i]->text, (Rectangle){0, 0, planet[i]->text.width, planet[i]->text.height}, (Rectangle){1470, 100, 300, (int)(300.0 / planet[i]->text.width * planet[i]->text.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);    
        DrawTextPro(font, "Planet Data:", (Vector2){1350, 260}, (Vector2){0, 0}, 0, 45, 1, WHITE);
        
        //physical properties
        char massString[64];
        char radiusString[64];
        char meanDensityString[64];
        char surfaceGravityString[64];
        char rotationPeriodString[64];
        char axialTiltString[64];
        char averageDistanceString[64];
        
            
        //orbital properties
        char escapeVelocityString[64];
        char speedString[64];
        char distanceFromSunString[64];
        char eccentricityString[64];
        char semiMajorAxisString[64];
        char semiMinorAxisString[64];
        char perihelionString[64];
        char aphelionString[64];
        char inclinationString[64];
        char gravitationalParameterString[64];
        char specificOrbitalEnergyString[64];
        char radialVelocityString[64];
        char tangentialVelocityString[64];
        char orbitalperiodString[64];
        
        //Derived Values
        char solarirradianceString[64];
        char blackbodyequilibriumtemperatureString[64];
        char hillsphereradiusString[64];
        char sphereofinfluenceString[64];
        char rochelimitString[64];
        char estimatedatmosphericscaleheightString[64];
        char velocityRelativeEarth[64];
        char gravityCompString[64];
        char yearCompString[64];
        char dayCompString[64];
        

                
        drawButton(&orbitalProperties);
        drawButton(&derivedValues);
        drawButton(&physicalProperties);
        drawButton(&atmosphericProperties);
        
        //global
        sun.density = sun.mass / (4.0/3.0 *PI*sun.radius*sun.radius*sun.radius);
        sun.gravitationalParameter =  G * sun.mass;
        earth.surfaceGravity = G * earth.mass / ((double)earth.radius * earth.radius);

        
        
        double dx, dy;
        dx = planet[j]->pos.x - sun.pos.x;
        dy = planet[j]->pos.y - sun.pos.y; 
        planet[j]->r = sqrtf(dx*dx + dy*dy);
    
        
        planet[j]->gravitationalParameter = G * planet[j]->mass;
        planet[j]->netVelocity = sqrt(planet[j]->velocity.x*planet[j]->velocity.x+planet[j]->velocity.y*planet[j]->velocity.y);
        planet[j]->specificOrbitalEnergy = ((planet[j]->netVelocity*planet[j]->netVelocity)/2) - (sun.gravitationalParameter / planet[j]->r);
        planet[j]->semiMajorAxis = -sun.gravitationalParameter /  (2*planet[j]->specificOrbitalEnergy);
        planet[j]->angularMomentum = dx * planet[j]->velocity.y - dy * planet[j]->velocity.x;
        planet[j]->eccentricity = sqrtf(1+((2*planet[j]->specificOrbitalEnergy*planet[j]->angularMomentum*planet[j]->angularMomentum)/(sun.gravitationalParameter*sun.gravitationalParameter)));
        planet[j]->density = planet[j]->mass / (4.0/3.0 *PI*planet[j]->radius*planet[j]->radius*planet[j]->radius);
        planet[j]->surfaceGravity = G * planet[j]->mass / ((double)planet[j]->radius * planet[j]->radius);
        planet[j]->orbitalPeriod = 2 * PI * sqrtf((planet[j]->semiMajorAxis*planet[j]->semiMajorAxis*planet[j]->semiMajorAxis)/sun.gravitationalParameter);
        planet[j]->escapeVelocity = sqrtf((2*G*sun.mass)/planet[j]->r);

        if (physicalProperties.on) {
            
            planet[j]->escapeVelocityPlanet = sqrtf((2*G*planet[j]->mass)/planet[j]->radius);
            snprintf(massString, 64, "Mass: %g kg", planet[j]->mass);
            snprintf(radiusString, 64, "Radius: %.0f m", planet[j]->radius);
            snprintf(meanDensityString, 64, "Mean Density: %.2f kg/m^3",planet[j]->density);
            snprintf(surfaceGravityString, 64, "Surface Gravity: %.2f m/s^2",planet[j]->surfaceGravity);
            snprintf(rotationPeriodString, 64, "Rotation Period: %.2f h",planet[j]->rotationPeriod/3600);
            snprintf(axialTiltString, 64, "Axial Tilt: %.2f degrees",planet[j]->axialTilt);
            snprintf(averageDistanceString, 64, "Average Distance: %.2f AU",planet[j]->averageDistance/AU);
            snprintf(escapeVelocityString, 64, "Escape Velocity: %.2f km/s", planet[j]->escapeVelocityPlanet/1000.0);
            snprintf(gravitationalParameterString, 64, "Gravitational Parameter: %.2e", planet[j]->gravitationalParameter);            


            
            DrawTextPro(font2, massString, (Vector2){1350, 490}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, radiusString, (Vector2){1350, 515}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, meanDensityString, (Vector2){1350, 540}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, surfaceGravityString, (Vector2){1350, 565}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, rotationPeriodString, (Vector2){1350, 590}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, axialTiltString, (Vector2){1350, 615}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, averageDistanceString, (Vector2){1350, 640}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, escapeVelocityString, (Vector2){1350, 665}, (Vector2){0, 0}, 0, 25, 1, WHITE);

        } 
        
        else if (atmosphericProperties.on) {
            DrawTexturePro(planet[i]->weather, (Rectangle){0, 0, planet[i]->weather.width, planet[i]->weather.height}, (Rectangle){1345, 490, 435, (int)(435.0 / planet[i]->weather.width * planet[i]->weather.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
        } 
        
        else if (derivedValues.on) {

            planet[j]->blackBodyEquilibrium = powf((LuminositySun*(1-planet[j]->bondAlbedo))/(16*PI*StefanBoltzmannConstant*planet[j]->r*planet[j]->r), 0.25);
            planet[j]->solarIrradiance = LuminositySun/(4*PI*planet[j]->r*planet[j]->r);
            planet[j]->hillSphereRadius = planet[j]->semiMajorAxis*cbrt(planet[j]->mass/(3.0*sun.mass));
            planet[j]->sphereOfInfluenceRadius = planet[j]->semiMajorAxis * powf(planet[j]->mass/sun.mass, 2.0/5.0);
            planet[j]->rocheLimit = rocheLimitConstant * sun.radius * pow(sun.density / planet[j]->density, 1.0 / 3.0);            
            planet[j]->estimatedAtmosphericScaleHeight = (R * planet[j]->temperature) / (planet[j]->meanMolarMass * planet[j]->surfaceGravity);
            double velocityRelativeEarthValue = sqrtf((planet[j]->velocity.x - earth.velocity.x)*(planet[j]->velocity.x - earth.velocity.x) + (planet[j]->velocity.y - earth.velocity.y)*(planet[j]->velocity.y - earth.velocity.y)) / 1000.0;
            double gravityPercent = planet[j]->surfaceGravity / earth.surfaceGravity*100;
            double yearComp = planet[j]->orbitalPeriod / earth.orbitalPeriod / 3600 / 365;
            double dayComp = fabs(planet[j]->rotationPeriod/3600.0/24.0) / (earth.rotationPeriod/3600.0/24.0);
            

            snprintf(blackbodyequilibriumtemperatureString, 64, "BB Equilibrium Temp: %.2f K", planet[j]->blackBodyEquilibrium);
            snprintf(solarirradianceString, 64, "Solar Irradiance: %.2f W/m^2",planet[j]->solarIrradiance);
            snprintf(hillsphereradiusString, 64, "Hill's Sphere: %.2e m",planet[j]->hillSphereRadius);
            snprintf(sphereofinfluenceString, 64, "Sphere of Influnce: %.2e m",planet[j]->sphereOfInfluenceRadius);
            snprintf(rochelimitString, 64, "Roche limit: %.3g AU",planet[j]->rocheLimit / AU);
            snprintf(estimatedatmosphericscaleheightString, 64, "Est. Atm. Height: %.2f km",planet[j]->estimatedAtmosphericScaleHeight/1000.0);
            snprintf(velocityRelativeEarth, 64, "Velocity: %.2f km/s",velocityRelativeEarthValue);
            snprintf(gravityCompString, 64, "Gravity: %.2f%% as earth's", gravityPercent);
            snprintf(yearCompString, 64, "Year: %.2f earth years", planet[j]->orbitalPeriod / earth.orbitalPeriod);
            snprintf(dayCompString, 64, "Day: %.2f earth days", dayComp);
            
            



            if (planet[j] == &sun) {
                snprintf(blackbodyequilibriumtemperatureString, 64, "Effective Temp: 5772 K");
                snprintf(hillsphereradiusString, 64, "Hill's Sphere: N/A");
                snprintf(sphereofinfluenceString, 64, "Sphere of Influnce: N/A");
                snprintf(rochelimitString, 64, "Roche limit: N/A");
                planet[j]->solarIrradiance = LuminositySun/(4*PI*planet[j]->radius*planet[j]->radius);
                snprintf(solarirradianceString, 64, "Solar Flux: %.2f W/m^2",planet[j]->solarIrradiance);
            }

            DrawTextPro(font2, blackbodyequilibriumtemperatureString, (Vector2){1350, 490}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, solarirradianceString, (Vector2){1350, 515}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, hillsphereradiusString, (Vector2){1350, 540}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, sphereofinfluenceString, (Vector2){1350, 565}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, rochelimitString, (Vector2){1350, 590}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, estimatedatmosphericscaleheightString, (Vector2){1350, 615}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font, "Relative to earth->", (Vector2){1350, 665}, (Vector2){0, 0}, 0, 35, 1, WHITE);
            DrawTextPro(font2, velocityRelativeEarth, (Vector2){1350, 700}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, gravityCompString, (Vector2){1350, 725}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, yearCompString, (Vector2){1350, 750}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, dayCompString, (Vector2){1350, 775}, (Vector2){0, 0}, 0, 25, 1, WHITE);

            
        }
        
        else if (orbitalProperties.on) {
            planet[j]->semiMinorAxis = planet[j]->semiMajorAxis * sqrtf(1 - planet[j]->eccentricity*planet[j]->eccentricity);
            planet[j]->perihelion = planet[j]->semiMajorAxis * (1 - planet[j]->eccentricity);
            planet[j]->aphelion = planet[j]->semiMajorAxis * (1 + planet[j]->eccentricity);
            planet[j]->radialVelocity = (dx * planet[j]->velocity.x + dy * planet[j]->velocity.y) / planet[j]->r;
            planet[j]->tangentialVelocity = fabs(dx*planet[j]->velocity.y- dy*planet[j]->velocity.x) / planet[j]->r;
            
            snprintf(escapeVelocityString, 64, "Escape Velocity: %.2f km/s", planet[j]->escapeVelocity/1000.0);
            snprintf(speedString, 64, "Velocity: %.2f km/s", planet[j]->netVelocity /1000.0);            
            snprintf(distanceFromSunString, 64, "Distance from Sun: %.2f AU", planet[j]->r / AU);
            snprintf(inclinationString, 64, "Inclination: %.2f degrees", planet[j]->inclination);
            snprintf(eccentricityString, 64, "Eccentricty: %.3f", planet[j]->eccentricity);
            snprintf(semiMajorAxisString, 64, "Semi-Major Axis: %.2f AU", planet[j]->semiMajorAxis/AU);
            snprintf(semiMinorAxisString, 64, "Semi-Minor Axis: %.2f AU", planet[j]->semiMinorAxis/AU);
            snprintf(aphelionString, 64, "Aphelion: %.2f AU", planet[j]->aphelion / AU);
            snprintf(perihelionString, 64, "Perihelion: %.2f AU", planet[j]->perihelion / AU);
            snprintf(gravitationalParameterString, 64, "Gravitational Parameter: %.2e", sun.gravitationalParameter);            
            snprintf(specificOrbitalEnergyString, 64, "Orbital Energy: %.2e j/kg", planet[j]->specificOrbitalEnergy);
            snprintf(radialVelocityString, 64, "Radial Velocity: %.2f km/s", planet[j]->radialVelocity/1000.0);
            snprintf(tangentialVelocityString, 64, "Tangetial Velocity: %.2f km/s", planet[j]->tangentialVelocity/1000.0);
            snprintf(orbitalperiodString, 64, "Orbital Period: %.2f days", planet[j]->orbitalPeriod/3600/24);

            if (planet[j] == &sun) {
                snprintf(escapeVelocityString, 64, "Escape Velocity: N/A");
                snprintf(speedString, 64, "Velocity: N/A");
                snprintf(distanceFromSunString, 64, "Distance from Sun: N/A");
                snprintf(inclinationString, 64, "Inclination: N/A");
                snprintf(eccentricityString, 64, "Eccentricty: N/A");
                snprintf(semiMajorAxisString, 64, "Semi-Major Axis: N/A");
                snprintf(semiMinorAxisString, 64, "Semi-Minor Axis: N/A");
                snprintf(aphelionString, 64, "Aphelion: N/A");
                snprintf(perihelionString, 64, "Perihelion: N/A");
                snprintf(specificOrbitalEnergyString, 64, "Orbital Energy: N/A");
                snprintf(radialVelocityString, 64, "Radial Velocity: N/A");
                snprintf(tangentialVelocityString, 64, "Tangetial Velocity: N/A");
                snprintf(orbitalperiodString, 64, "Orbital Period: N/A");
            }
                        
            DrawTextPro(font2, escapeVelocityString, (Vector2){1350, 490}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, speedString, (Vector2){1350, 515}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, distanceFromSunString, (Vector2){1350, 540}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, inclinationString, (Vector2){1350, 565}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, eccentricityString, (Vector2){1350, 590}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, semiMajorAxisString, (Vector2){1350, 615}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, semiMinorAxisString, (Vector2){1350, 640}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, aphelionString, (Vector2){1350, 665}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, perihelionString, (Vector2){1350, 690}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, gravitationalParameterString, (Vector2){1350, 715}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, specificOrbitalEnergyString, (Vector2){1350, 740}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, radialVelocityString, (Vector2){1350, 765}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, tangentialVelocityString, (Vector2){1350, 790}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            DrawTextPro(font2, orbitalperiodString, (Vector2){1350, 815}, (Vector2){0, 0}, 0, 25, 1, WHITE);
            

        }
    }

}

void orbitTrails(Planet *planet[]) {
    int point;
    double ω, h, r, μ;
    double ex, ey;
    double x, y, vx, vy;

    for (int i = 1; i <= celestialNum; i++) {

        x = planet[i]->pos.x;
        y = planet[i]->pos.y;
        vx = planet[i]->velocity.x;
        vy = planet[i]->velocity.y;

        r = sqrt(x*x+y*y);

        h = x*vy - y*vx;
        μ = G * sun.mass;

        ex = (vy * h) / μ - x / r;
        ey = -(vx * h) / μ - y / r;

        ω = atan2(ey, ex);

        point = 0;

        sun.gravitationalParameter =  G * sun.mass;

        double dx, dy;
        dx = planet[i]->pos.x - sun.pos.x;
        dy = planet[i]->pos.y - sun.pos.y; 

        planet[i]->r = sqrtf(dx*dx + dy*dy);

        planet[i]->gravitationalParameter = G * planet[i]->mass;
        planet[i]->netVelocity = sqrt(planet[i]->velocity.x*planet[i]->velocity.x+planet[i]->velocity.y*planet[i]->velocity.y);
        planet[i]->specificOrbitalEnergy = ((planet[i]->netVelocity*planet[i]->netVelocity)/2) - (sun.gravitationalParameter / planet[i]->r);
        planet[i]->semiMajorAxis = -sun.gravitationalParameter /  (2*planet[i]->specificOrbitalEnergy);
        planet[i]->angularMomentum = dx * planet[i]->velocity.y - dy * planet[i]->velocity.x;
        planet[i]->eccentricity = sqrtf(1+((2*planet[i]->specificOrbitalEnergy*planet[i]->angularMomentum*planet[i]->angularMomentum)/(sun.gravitationalParameter*sun.gravitationalParameter)));
        
        for (double θ = 0; θ <= 2*PI; θ+= epsilon) {


            double distance = (planet[i]->semiMajorAxis * (1-planet[i]->eccentricity*planet[i]->eccentricity)) / (1+planet[i]->eccentricity*cos(θ-ω));
            Vector2 cordinates = {cos(θ)*distance, sin(θ)*distance};
            orbits[i][point] = (Vector2)cordinates;

                
            orbitsDraw[i][point].x = (orbits[i][point].x+deltaWorld.x) * zoomFactor + SCREEN_HALF_X;
            orbitsDraw[i][point].y = (orbits[i][point].y+deltaWorld.y) * zoomFactor + SCREEN_HALF_Y ;
            
            point++;
        }
    }
}

void drawOrbitTrails() {
    int point = 0;

    for (int i = 1; i <= celestialNum; i++) {
        point = 0;
        for (double θ = 0; θ <= 2*PI; θ+= epsilon) {
            DrawPixel(orbitsDraw[i][point].x, orbitsDraw[i][point].y, celestialBodies[i]->color);
            point++;
        }
    }
}

void applyEscapeVelocity(Planet *planet) {

    double dx, dy;
    dx = planet->pos.x - sun.pos.x;
    dy = planet->pos.y - sun.pos.y; 
    planet->r = sqrtf(dx*dx + dy*dy);

    planet->escapeVelocity = sqrtf((2*G*sun.mass)/planet->r);

    planet->velocity.x = planet->escapeVelocity * (planet->velocity.x/planet->netVelocity);
    planet->velocity.y = planet->escapeVelocity * (planet->velocity.y/planet->netVelocity);



}

void planetSelection(Planet *planet[]) {
    
    if (rocketToggle) {
        DrawTextEx(timesNewRoman, "BIG PUSH", (Vector2){15, 140}, 35, 1, WHITE);
        DrawTextEx(timesNewRoman, "Apply Escape velocity to ->", (Vector2){15, 200}, 35, 1, WHITE);
    } else {
        DrawTextEx(timesNewRoman, "FOLLOW AROUND", (Vector2){15, 140}, 35, 1, WHITE);
        DrawTextEx(timesNewRoman, "Select Celestial Object ->", (Vector2){15, 200}, 35, 1, WHITE);
    }

    int start = (rocketToggle) ? 1:0;

    for (int i = start; i <= celestialNum; i++) {
        int row = (i-start)%3;
        int column = (i-start)/3;

        DrawTexturePro(planet[i]->texture, (Rectangle){0, 0, planet[i]->texture.width, planet[i]->texture.height}, (Rectangle){25+row*100, 270+120*column, 75, (int)(75.0 / planet[i]->texture.width * planet[i]->texture.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
        
        int extra = 0;
        if (planet[i] == &sun) extra = 10;
        else if (planet[i] == &venus) extra = 5;
        else if (planet[i] == &neptune || planet[i] == &mercury) extra = -3;
        else if (planet[i] == &mars) extra = 8;
        else if (planet[i] == &pluto) extra = 3;

        DrawTextEx(monocraft, planet[i]->name, (Vector2){40+row*100 + extra, 350+120*column}, 15, 1, WHITE);

    
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){25+row*100, 270+120*column, 75, 75})) {
            if (planetsUI) { 
                planetSelected = i+1;
                planetgoingTo = i+1;
                only = false;
            } else {
                applyEscapeVelocity(celestialBodies[i]);
            }
        } 
    }

    if (planetsUI) {

        DrawTexturePro(solarSystem, (Rectangle){0, 0, solarSystem.width, solarSystem.height}, (Rectangle){125, 630, 75, (int)(75.0 / solarSystem.width * solarSystem.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
        DrawLineEx((Vector2){225, 630}, (Vector2){300, 705}, 4, GRAY);
        DrawLineEx((Vector2){225, 705}, (Vector2){300, 630}, 4, GRAY);
        DrawTextEx(monocraft, "Solar System", (Vector2){115, 710}, 15, 1, WHITE);
        DrawTextEx(monocraft, "Stop", (Vector2){250, 710}, 15, 1, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){125, 630, 75, 75})) {
            planetSelected = 0;
            planetgoingTo = 1;
            only = true;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){225, 630, 75, 75})) {
            planetSelected = 0;
            planetgoingTo = 0;
        }

    }
}

void buttons() {
    pauseButton = (Rectangle){880, 780, 40, 25};
    previousButton = (Rectangle){810, 777, 50, 30};
    nextButton = (Rectangle){940, 777, 50, 30};
    optionsButton = (Rectangle){15, 60, 43, 35};
    planetsButton = (Rectangle){70, 60, 43, 35};
    calendarButton = (Rectangle){125, 60, 43, 35};
    rocketButton = (Rectangle){180, 60, 43, 35};

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), optionsButton)) {
        options = !options;    
        planetsUI = false; 
        calendarToggle = false; 
        rocketToggle = false;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), planetsButton)) {
        planetsUI = !planetsUI;  
        options = false; 
        calendarToggle = false; 
        rocketToggle = false;

    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), orbitTrailsButton.inside)) {
        orbitsToggle = !orbitsToggle;  

    } if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), calendarButton)) {
        calendarToggle = !calendarToggle;
        options = false;   
        planetsUI = false; 
        rocketToggle = false;
    } if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rocketButton)) {
        rocketToggle = !rocketToggle;
        options = false;   
        planetsUI = false; 
        calendarToggle = false;
    }


    DrawRectangleRounded(pauseButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(previousButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(nextButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(optionsButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(planetsButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(calendarButton, 0.6, 10.0f, DARKGRAY);
    DrawRectangleRounded(rocketButton, 0.6, 10.0f, DARKGRAY);


    if (CheckCollisionPointRec(GetMousePosition(), pauseButton)) DrawRectangleRounded(pauseButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), previousButton)) DrawRectangleRounded(previousButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), nextButton)) DrawRectangleRounded(nextButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), optionsButton)) DrawRectangleRounded(optionsButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), planetsButton)) DrawRectangleRounded(planetsButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), calendarButton)) DrawRectangleRounded(calendarButton, 0.6, 10.0f, (Color)verydarkgray);
    if (CheckCollisionPointRec(GetMousePosition(), rocketButton)) DrawRectangleRounded(rocketButton, 0.6, 10.0f, (Color)verydarkgray);


    DrawText("<", 830, 779, 30, WHITE);
    DrawText(">", 963, 779, 30, WHITE);
    DrawText("| |", 895, 784, 18, WHITE);
    DrawTextPro(monocraft, "|||", (Vector2){33, 68}, (Vector2){0, 0}, 90, 12, 1, WHITE);
    DrawTextPro(monocraft, "|||", (Vector2){40, 68}, (Vector2){0, 0}, 90, 12, 1, WHITE);
    DrawTextPro(monocraft, "|||", (Vector2){47, 68}, (Vector2){0, 0}, 90, 12, 1, WHITE);
    DrawTextPro(monocraft, "|||", (Vector2){54, 68}, (Vector2){0, 0}, 90, 12, 1, WHITE);

    int i;
    int extra = 0;
    if (planetSelected == 0)  i = (int)(framesTicked/100) % 10;
    else i = planetSelected -1;

    if (celestialBodies[i] == &saturn) extra = 5;
    DrawTexturePro(celestialBodies[i]->texture, (Rectangle){0, 0, celestialBodies[i]->texture.width, celestialBodies[i]->texture.height}, (Rectangle){77, 62+extra/2, 30, (int)((30.0+extra) / celestialBodies[i]->texture.width * celestialBodies[i]->texture.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
    orbitTrailsButton.colorBorder = (Color){celestialBodies[i]->color.r/1.5, celestialBodies[i]->color.g/1.5,celestialBodies[i]->color.b/1.5, 255};
    orbitTrailsButton.colorInside = (Color){celestialBodies[i]->color.r/3, celestialBodies[i]->color.g/3,celestialBodies[i]->color.b/3, 255};

    DrawTexturePro(calendarIcon, (Rectangle){0, 0, calendarIcon.width, calendarIcon.height}, (Rectangle){134.5, 65, 26, (int)((26.0) / calendarIcon.width * calendarIcon.height)}, (Vector2){0, 0}, 0.0, RAYWHITE);        
    DrawTexturePro(raptor, (Rectangle){0, 0, raptor.width, raptor.height}, (Rectangle){182, 70, 17, (int)((17.0) / raptor.width * raptor.height)}, (Vector2){0, 0}, -45.0, RAYWHITE);        

    
} 

void teleport() {
    
    if (planetgoingTo != 0) {
        if (zoomFactor != 1e-9 && moved == false) {
            if (round(zoomFactor*1e10) == 10) {
                double value = zoomFactor / 1e-9;
                //adjust(celestialBodies, value);
                zoomFactor = 1e-9;
            }
            else if (zoomFactor > 1e-9) {
                zoomFactor *= 0.95;
                adjust(celestialBodies, 0.95);
            } else {
                zoomFactor *= 1.05;
                adjust(celestialBodies, 1.05);
            }

        } else if ((fabs(celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X) >= 0.05 || fabs(celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y) >= 0.05) && !moved) {
            Vector2 delta;
            
            if (round((celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X)) == 0) {
                delta.x = -(celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X);
            } else if ((round((celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X)/5)) == 0) {
                if ((celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X) > 0) {
                    delta.x = -1;
                } else {
                    delta.x = 1;
                }
            } else if ((celestialBodies[planetgoingTo-1]->drawPos.x-SCREEN_HALF_X) > 0) {
                delta.x = -5;
            } else {
                delta.x = 5;
            }
            
            if (round((celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y)) == 0) {
                delta.y = -(celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y);
            } else if (round(((celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y)/5)) == 0) {
                if ((celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y) > 0) {
                    delta.y = -1;
                } else {
                    delta.y = 1;
                }
            } else if ((celestialBodies[planetgoingTo-1]->drawPos.y-SCREEN_HALF_Y) > 0) {
                delta.y = -5;   
            } else {
                delta.y = 5;
            }
            
            deltaWorld.x += delta.x / zoomFactor;
            deltaWorld.y += delta.y / zoomFactor;
            movePlanet(celestialBodies);

        } else if (celestialBodies[planetgoingTo-1] == &pluto ? (pluto.drawRadius < 100 || pluto.drawRadius > 300) : (celestialBodies[planetgoingTo-1]->drawRadius < 200 || celestialBodies[planetgoingTo-1]->drawRadius > 300)) {
            if (!only) {

                if (!moved) {
                    started = framesTicked;
                }
                moved = true;

                if (zoomFactor > 1e-4) {
                    zoomFactor *= 0.95;
                    adjust(celestialBodies, 0.95);

                } else {
                    zoomFactor *= 1.05;
                    adjust(celestialBodies, 1.05);
                }
            } else {
                planetgoingTo = 0;
                moved = false;
            }
        } else {
            planetgoingTo = 0;
            moved = false;
        }

    }
}

void generateSliderFunct() {
    //Points SLider
    cursorPos = (Vector2){900, 0};

    numPoints = 0;

    for (int x = -300; x <= 300; x++){
        int y = -0.0007 * x*x + 850;
        points[numPoints] = (Vector2){x + 900, y};
        numPoints++;
    }
}

void setColors() {
    //Colors
    blue1        = (Color){9,   0,   136, 255};
    blue2        = (Color){2,   0,   108, 255};
    green1       = (Color){48,  104, 68,  255};
    green2       = (Color){44,  76,  59,  255};
    orange1      = (Color){255, 141, 0,   255};
    orange2      = (Color){255, 116, 0,   255};
    red1         = (Color){178, 34,  34,  255};
    red2         = (Color){139, 0,   0,   255};
    purple1      = (Color){128, 0,   128, 255};
    purple2      = (Color){102, 0,   102, 255};
    darkRed      = (Color){100, 0,   0,   255};
    drakGreen    = (Color){0,   90,  0,   255};
    red          = (Color){255, 0,   0,   255};
    green        = (Color){0,   255, 0,   255};
    blue         = (Color){0,   0,   100, 255};
    lightBlue    = (Color){0,   0,   200, 255};
    verydarkgray = (Color){50,  50,  50,  255};

}

void setOptions() {
    
    DrawTextPro(monocraft, "*Click a planet to follow/zoom in", (Vector2){20, 852}, (Vector2){0, 0}, 0, 15, 1, GRAY);
    DrawTextPro(monocraft, "*Press R to restart", (Vector2){20, 867}, (Vector2){0, 0}, 0, 15, 1, GRAY);
    DrawTextPro(monocraft, "*Press SPACE to pause simulation", (Vector2){20, 882}, (Vector2){0, 0}, 0, 15, 1, GRAY);
    
    DrawTextEx(timesNewRoman, "OPTIONS", (Vector2){15, 140}, 35, 1, WHITE);
    drawButton(&orbitTrailsButton);

    
}
int main() {
    
    //flags
    mousePressed = false;
    orbiting = true;
    moved = false;
    orbitsToggle = true;
    options = false; 
    planetsUI = false;
    calendarToggle = false; 
    rocketToggle = false;

    
    zoomFactor = 1e-9;
    
    framesTicked = 0;
    elapsedSeconds = dateSeconds;
    seconds1000Years = 1000.0 * 365.25 * 24.0 * 60.0 * 60.0;    
    epsilon = 0.004;
    planetgoingTo = 0;
    celestialNum = 9;
    
    char date[100];
    char rateString[128];
    
    double firstDTmin = -60*60*24*36.5; //-10years per sec
    double firstDTmax = 60*60*24*36.5; //10 years per sec
    
    planetSelected = 0;
    started = 0;
    
    targetFps = 100;
    SetTargetFPS(targetFps);


    //init app
    InitWindow(1800, 900, "Solar System 2D 🪐");
    
    //Start
    setColors();
    loadFiles();
    setInitialData();
    setButtons();
    getDate(DT, framesTicked, date);
    getRate(DT, rateString, orbiting);
    generateSliderFunct();
    
    setInitialPos(celestialBodies);


    
    //run app
    while (!WindowShouldClose()) {
        
        if (orbiting) {
            framesTicked++;
            applyGravity(celestialBodies);
        }

        getDate(DT, 11, date);
        
        if (planetSelected == 0) {
            support = (Rectangle){1800, 0, 1000, 900};
        } else {
            support = (Rectangle){1300, 0, 1000, 900};
        }
        
        //Input
        
        support2 = (Rectangle){580, 720, 640, 300};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) ) {
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle)nextButton) || IsKeyPressed(KEY_RIGHT)) {
                DT = jumpRate(DT, true);
            } else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle)previousButton) || IsKeyPressed(KEY_LEFT)) {
                DT = jumpRate(DT, false);
            }
            double L = log10f(-firstDTmin);
            if (DT < -0.01f) {                      
                double t = 1.0f - log10f(-DT) / L;
                if (t < 0.0f) t = 0.0f;
                else if (t > 1.0f) t = 1.0f;
                cursorPos.x = 600.0f + t * 300.0f;
            }
            else if (DT > 0.01f) {                  
                double t = log10f(DT) / L;
                if (t < 0.0f) t = 0.0f;
                else if (t > 1.0f) t = 1.0f;
                cursorPos.x = 900.0f + t * 300.0f;
            }
            else {
                cursorPos.x = 900.0f;   
            }
        } 
        
        if (IsKeyPressed(KEY_SPACE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle)pauseButton))) {
            orbiting = !orbiting;
        }
        if (IsKeyPressed(KEY_R)) {
            zoomFactor = 1e-9;
            setInitialData();
            setInitialPos(celestialBodies);
            
            DT = 0.01;
            orbiting = true;
            elapsedSeconds = dateSeconds;
            getDate(DT, 11, date);
            orbiting = false;
            
            cursorPos.x = 900;
            deltaWorld = (Vector2){0, 0};
        }  
        if (GetMouseWheelMove() == 1.0 && zoomFactor < 1e-3) {
            zoomFactor *= 1.2;
            adjust(celestialBodies, 1.2);
        } else if (GetMouseWheelMove() == -1.0) {
            zoomFactor *= 0.8;
            adjust(celestialBodies, 0.8);
        }
        
        
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mousePressed = true;
        }
        if (mousePressed && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mousePressed = false;
            if (cursorPos.x == 900) DT = 0.01;

        } else if (mousePressed) {
            cursorPos.y = GetMousePosition().y;
            if (!CheckCollisionPointRec(GetMousePosition(), (Rectangle)support2) && !CheckCollisionPointRec(GetMousePosition(), (Rectangle)support) ) {
                deltaWorld.x += GetMouseDelta().x / zoomFactor;
                deltaWorld.y += GetMouseDelta().y / zoomFactor;
                movePlanet(celestialBodies);
            } else if (CheckCollisionPointCircle(GetMousePosition(), (Vector2)cursorPos, 22.5)) {
                cursorPos.x += GetMouseDelta().x;
                cursorPos.y = GetMousePosition().y;
                
                
                if (cursorPos.x < 600) cursorPos.x = 600;
                if (cursorPos.x > 1200) cursorPos.x = 1200;
                
            }
        }     
        
        teleport();
        if (cursorPos.x < 900) {
            double t = (cursorPos.x - 600.0f) / 300.0f;
            DT = -powf(10.0f, log10f(-firstDTmin) * (1.0f - t));
            getRate(DT, rateString, orbiting);
        }
        else if (cursorPos.x > 900) {
            double t = (cursorPos.x - 900.0f) / 300.0f;
            DT = powf(10.0f, log10f(-firstDTmin) * t);
            getRate(DT, rateString, orbiting);
        } else {
            getRate(0.01, rateString, orbiting);
        }
        if (DT < 100 && DT > -100) {
            DT = round(DT*100) / 100;
        } else if (DT < 30000 && DT > -30000) {
            DT = round(DT);
        } else if (DT < 30000000 && DT > -30000000) {
            DT = round(DT/10)*10;
        }        
    
        

        
        //Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (orbitsToggle) {    
            orbitTrails(celestialBodies);
            drawOrbitTrails();
        }

        drawPlanet(celestialBodies, monocraft);
        
        
        char fps[16];
        snprintf(fps, 16, "%d", GetFPS());
        
        if (options) setOptions();
        
        DrawTextPro(monocraft, date, (Vector2){20, 10}, (Vector2){0, 0}, 0, 30, 1, WHITE);
        
        
        DrawRectangleRounded((Rectangle)support2, 0.8, 50, (Color){0, 0, 0, 200});
        infoPlanets(celestialBodies, timesNewRoman, monocraft);
        buttons();        
        
        //slider
        for (int i = 0; i <= numPoints; i++) {
            DrawPixelV(points[i], GRAY);
        }
        
        DrawCircle(900, 850, 4, DARKGRAY);
        
        cursorPos.y = -0.0007 * (cursorPos.x - 900) * (cursorPos.x - 900) + 850;
        
        
        DrawCircle(cursorPos.x, cursorPos.y, 22.5, DARKGRAY);
        DrawCircle(cursorPos.x, cursorPos.y, 21, BLACK);
        DrawCircle(cursorPos.x, cursorPos.y, 16.5, DARKGRAY);
        DrawCircle(cursorPos.x, cursorPos.y, 10.5, BLACK);
        DrawCircle(cursorPos.x, cursorPos.y, 9, DARKGRAY);

        
        
        if (planetsUI || rocketToggle) {
            planetSelection(celestialBodies);
        }
        
        
        DrawTextPro(monocraft, rateString, (Vector2){815, 730}, (Vector2){0, 0}, 0, 30, 1, WHITE);
        
        if (orbiting) {
            DrawCircle(1030, 870, 7, GREEN);
            DrawTextPro(monocraft, "Simulating", (Vector2){1050, 857}, (Vector2){0, 0}, 0, 25, 1, GREEN);
            
            if (strcmp(rateString, " Real Time")) {
                DrawCircle(680, 870, 7, RED);
                DrawTextPro(monocraft, "Live", (Vector2){700, 857}, (Vector2){0, 0}, 0, 25, 1, RED);
            } else {
                DrawCircle(680, 870, 7, GREEN);
                DrawTextPro(monocraft, "Live", (Vector2){700, 857}, (Vector2){0, 0}, 0, 25, 1, GREEN);
                DrawCircle(cursorPos.x, cursorPos.y, 9, GREEN);
                
                
            }
        } else {
            DrawCircle(1030, 870, 7, GRAY);
            DrawTextPro(monocraft, "Simulating", (Vector2){1050, 857}, (Vector2){0, 0}, 0, 25, 1, GRAY);
            DrawCircle(680, 870, 7, GRAY);
            DrawTextPro(monocraft, "Paused", (Vector2){700, 857}, (Vector2){0, 0}, 0, 25, 1, GRAY);

        }

        DrawText(fps, 1760, 860, 24, WHITE);
        
    

        EndDrawing();

        
        
    }

    //close window
    CloseWindow();

    return 0;
}