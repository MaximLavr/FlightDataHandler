#define N_TAGIL 1
#define SLOW_SPEED_ON_SCROLL 0

#include "../SparkyEngine/src/sparky.h"
#include "UI/button.h"
#include "Map/map.h"
#include "Track/Track.h"
#include "UI/SubWindow.h"
#include "Algorithm/PulseDataHandler.h"
#include "GettingGraphics/Graphics.h"
#include <utility>
#include <time.h>
#include <unistd.h>

using namespace sparky;
using namespace graphics;
using namespace maths;
using namespace app_graphics;
using namespace app_utils;

class App : public Sparky
{
private:
#if N_TAGIL
    const std::string PathToTrackDataDir = "../Data/Data_19.05.07/NTagil_18_12_21/data";
#else
    const std::string PathToTrackDataDir = "../Data/Track/Alapaevsk.txt";
#endif
    Window* window;
    float windowWidth = 2500;
    float windowHeight = 1563;
    float cameraLeft;
    float cameraRight;
    float cameraBottom;
    float cameraUp;
    int minZoom = 11;
    int maxZoom = 13;
    mat4 orthographicMatrix;

    MapLayer* layer1;
    MapLayer* pointLayer;

    Shader* shaderNoTransparent;
    Shader* shaderTransparent;
    Shader* shaderScroll1;
    Shader* shaderSubWindow1;
    Shader* shaderScroll2;
    Shader* shaderSubWindow2;
    Shader* shaderScroll3;
    Shader* shaderSubWindow3;

    Sprite* spriteImpulseGraph = nullptr;
    Sprite* spriteImpulseHist = nullptr;
    Sprite* spriteAltGraph = nullptr;
    Sprite* spriteSurfTypeGraph = nullptr;
    Sprite* spriteAppLogo;
    Sprite* spriteCloseLogo;
    Sprite* spriteFooter;
    Sprite* spriteHeader;
    Sprite* spriteMapGrid;
    Sprite* spriteCursorCross;
    Renderable2D* previousPointChosen;
    Texture* textureCursorCross;

    Group* groupHeader;
    Group* groupFooter;
    Group* groupFpsLog;
    Group* groupDataSide;

    Label* logSpeedSSK = nullptr;
    Label* logSpeedNSK = nullptr;
    Label* logHeightGPS = nullptr;
    Label* logHeightMeasured = nullptr;
    Label* logFlightTime = nullptr;
    Label* logLatitude = nullptr;
    Label* logLongitude = nullptr;
    Label* logYaw = nullptr;
    Label* logPitch = nullptr;
    Label* logRoll = nullptr;
    Label* logSurfaceType = nullptr;
    Label* logFps = nullptr;
    Label* titleImpulseGraph = nullptr;
    Label* titleImpulseHist = nullptr;
    Label* titleAltGraph = nullptr;
    Label* titleSurfaceTypeGraph = nullptr;
    Label* logZoomInfo;
    Label* logLatLongInfo;
    Label* logMetersPerPixelInfo;
    Label* logMapScaleInfo;

    vec2_d latLonCoords;

    bool isCursorAdded;
    bool pointAdded;

    FlightData* flightData;
    PulseDataHandler* algorithm;
    Graphics* graphics;

    Map* map = nullptr;
    Track* track = nullptr;

    Button* buttonFlightDataHandler;
    Button* buttonFile;
    Button* buttonHelp;
    Button* buttonCloseApp;
    Button* buttonZoomIn;
    Button* buttonZoomOut;
    Button* buttonApp;

    SubWindow* subWindowData;
    SubWindow* subWindowGraphics;
    SubWindow* subWindowSurfaceType;
public:
    App()
    {
    }

    void AddDataTextSubWindowSurfaceType(SubWindow* subWindow)
    {
        if (logSurfaceType == nullptr) {
            logSurfaceType = new Label("",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(1) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(0) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logSurfaceType);
        }
    }

    void AddDataTextSubWindowGraphics(SubWindow* subWindow)
    {
        float pictureWidth = 26.0f;
        float pictureHeight = pictureWidth / 1.33f;
        if (titleImpulseGraph == nullptr) {
            subWindow->currentMaxY = subWindow->m_Size.y - subWindow->m_HeaderHeight
                                     - subWindow->m_PaddingUp
                                     - float(1) * FontManager::get("Lusia_UI")->GetHeight("W")
                                     - float(0) * subWindow->m_StringInterval;
            titleImpulseGraph = new Label("Impulse graph",
                                     subWindow->m_PaddingLeft,
                                     subWindow->currentMaxY,
                                     "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(titleImpulseGraph);
        }
        if (spriteImpulseGraph == nullptr) {
            subWindow->currentMaxY -= pictureHeight;
            subWindow->currentMaxY -= subWindow->m_StringInterval;
            spriteImpulseGraph = new Sprite(subWindow->m_PaddingLeft,
                                            subWindow->currentMaxY,
                                            pictureWidth, pictureHeight,
                                            transform_color(0.14647, 0.14647, 0.14924, 1));
            subWindow->AddDataImage(spriteImpulseGraph);
        }

        if (titleImpulseHist == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            titleImpulseHist = new Label("Impulse hist",
                                    subWindow->m_PaddingLeft,
                                    subWindow->currentMaxY,
                                    "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(titleImpulseHist);
        }
        if (spriteImpulseHist == nullptr) {
            subWindow->currentMaxY -= pictureHeight;
            subWindow->currentMaxY -= subWindow->m_StringInterval;
            spriteImpulseHist = new Sprite(subWindow->m_PaddingLeft,
                                           subWindow->currentMaxY,
                                           pictureWidth, pictureHeight,
                                           transform_color(0.14647, 0.14647, 0.14924, 1));
            subWindow->AddDataImage(spriteImpulseHist);
        }

        if (titleAltGraph == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            titleAltGraph = new Label("Alt graph",
                                    subWindow->m_PaddingLeft,
                                    subWindow->currentMaxY,
                                    "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(titleAltGraph);
        }
        if (spriteAltGraph == nullptr) {
            subWindow->currentMaxY -= pictureHeight;
            subWindow->currentMaxY -= subWindow->m_StringInterval;
            spriteAltGraph = new Sprite(subWindow->m_PaddingLeft,
                                           subWindow->currentMaxY,
                                           pictureWidth, pictureHeight,
                                        transform_color(0.14647, 0.14647, 0.14924, 1));
            subWindow->AddDataImage(spriteAltGraph);
        }

        if (titleSurfaceTypeGraph == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            titleSurfaceTypeGraph = new Label("Surface type graph",
                                 subWindow->m_PaddingLeft,
                                 subWindow->currentMaxY,
                                 "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(titleSurfaceTypeGraph);
        }
        if (spriteSurfTypeGraph == nullptr) {
            subWindow->currentMaxY -= pictureHeight;
            subWindow->currentMaxY -= subWindow->m_StringInterval;
            spriteSurfTypeGraph = new Sprite(subWindow->m_PaddingLeft,
                                           subWindow->currentMaxY,
                                           pictureWidth, pictureHeight,
                                             transform_color(0.14647, 0.14647, 0.14924, 1));
            subWindow->AddDataImage(spriteSurfTypeGraph);
        }

    }

    void AddDataTextSubWindowData(SubWindow* subWindow)
    {
        if (logSpeedSSK == nullptr) {
            logSpeedSSK = new Label("Speed (SSK, m/sec):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(1) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(0) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logSpeedSSK);
        }
        if (logSpeedNSK == nullptr) {
            logSpeedNSK = new Label("Speed (NSK, m/sec):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(2) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(1) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logSpeedNSK);
        }
        if (logHeightGPS == nullptr) {
            logHeightGPS = new Label("Height (GPS, m):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(3) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(2) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logHeightGPS);
        }
        if (logHeightMeasured == nullptr) {
            logHeightMeasured = new Label("Height (measured, m):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(4) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(3) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logHeightMeasured);
        }
        if (logFlightTime == nullptr) {
            logFlightTime = new Label("Flight time (sec):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(5) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(4) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logFlightTime);
        }
        if (logLatitude == nullptr) {
            logLatitude = new Label("Latitude (decimal degrees):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(6) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(5) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logLatitude);
        }
        if (logLongitude == nullptr) {
            logLongitude = new Label("Longitude (decimal degrees):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(7) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(6) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logLongitude);
        }
        if (logYaw == nullptr) {
            logYaw = new Label("Yaw (decimal degrees):",
                                  subWindow->m_PaddingLeft,
                                  subWindow->m_Size.y - subWindow->m_HeaderHeight
                                  - subWindow->m_PaddingUp
                                  - float(8) * FontManager::get("Lusia_UI")->GetHeight("W")
                                  - float(7) * subWindow->m_StringInterval,
                                  "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logYaw);
        }
        if (logPitch == nullptr) {
            logPitch = new Label("Pitch (decimal degrees):",
                                subWindow->m_PaddingLeft,
                                subWindow->m_Size.y - subWindow->m_HeaderHeight
                                - subWindow->m_PaddingUp
                                - float(9) * FontManager::get("Lusia_UI")->GetHeight("W")
                                - float(8) * subWindow->m_StringInterval,
                                "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logPitch);
        }
        if (logRoll == nullptr) {
            logRoll = new Label("Roll (decimal degrees):",
                                 subWindow->m_PaddingLeft,
                                 subWindow->m_Size.y - subWindow->m_HeaderHeight
                                 - subWindow->m_PaddingUp
                                 - float(10) * FontManager::get("Lusia_UI")->GetHeight("W")
                                 - float(9) * subWindow->m_StringInterval,
                                 "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(logRoll);
        }
    }

    ~App()
    {
        delete layer1;
        delete pointLayer;
    }

    void init() override
    {
        cameraRight = windowWidth / 60.0f;
        cameraLeft = -cameraRight;
        cameraUp = windowHeight / 60.0f;
        cameraBottom = -cameraUp;

        window = createWindow("Test Map", windowWidth, windowHeight, cameraLeft, cameraRight, cameraBottom, cameraUp); //700, 540

        FontManager::add(new Font("Arial", "../SparkyEngine/fonts/Arial_Cyr.TTF", 50));
        FontManager::get("Arial")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Arial_small", "../SparkyEngine/fonts/Arial_Cyr.TTF", 30));
        FontManager::get("Arial_small")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Arial_big", "../SparkyEngine/fonts/Arial_Cyr.TTF", 70));
        FontManager::get("Arial_big")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Lusia_UI_bold", "../fonts/sf/SF-Pro-Display-Bold.otf", 55));
        FontManager::get("Lusia_UI_bold")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Lusia_UI", "../fonts/lucidagrande.ttf", 55));
        FontManager::get("Lusia_UI")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        shaderNoTransparent = new Shader("../SparkyEngine/src/shaders/test/shader1.vert","../SparkyEngine/src/shaders/test/shader1.frag");
        shaderTransparent = new Shader("../SparkyEngine/src/shaders/transparent.vert","../SparkyEngine/src/shaders/transparent.frag");

        shaderScroll1 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow1 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");
        shaderScroll2 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow2 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");
        shaderScroll3 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow3 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");

        orthographicMatrix = mat4::orthographic(-window->getWidth() / 60.0f, (window->getWidth() / 60.0f), -(window->getHeight() / 60.0f), (window->getHeight() / 60.0f), -1.0f, 1.0f);

        layer1 = new MapLayer(new BatchRenderer2D(window), shaderTransparent, orthographicMatrix, true);
        pointLayer = new MapLayer(new BatchRenderer2D(window), shaderNoTransparent, orthographicMatrix, true);

        groupHeader = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        spriteHeader = new Sprite(-window->getWidth() / 60.0f, (window->getHeight() / 60.0f) - 1.7f, (window->getWidth() / 60.0f) - (-window->getWidth() / 60.0f), 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1));
        groupHeader->add(spriteHeader);

        groupFooter = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        spriteFooter = new Sprite(-window->getWidth() / 60.0f, -(window->getHeight() / 60.0f), (window->getWidth() / 60.0f) - (-window->getWidth() / 60.0f), 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1));
        groupFooter->add(spriteFooter);

        float x = (window->getWidth() / 60.0f) - ((window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f);
        float y = -(window->getHeight() / 60.0f) + 1.7f;
        float side = (window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f;
        spriteMapGrid = new Sprite(x, y, side, side, transform_color(0.07, 0.07, 0.07, 1));

        groupDataSide = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        groupDataSide->add(new Label("Flight data", (-window->getWidth() / 60.0f) + 1.35f, (window->getHeight() / 60.0f) - 2.85f,"Lusia_UI",
                                     transform_color(1, 1, 1, 1)));

        spriteAppLogo = new Sprite(0.85f, 0.22f, 1.2f, 1.2f,
                                   new Texture("", "../images/UI/logo_white.png"));
        spriteCloseLogo = new Sprite(0.85f, 0.27f, 1.3f, 1.0f, new Texture("", "../images/UI/close.png"));
        buttonFlightDataHandler = new Button((-window->getWidth() / 60.0f) + 3.0f, (window->getHeight() / 60.0f) - 1.7f, 9.67f, 2.0f - 0.3f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                    new Label("FlightDataHandler", 1.0f, 0.57f,"Lusia_UI_bold",
                                              transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        buttonApp = new Button((-window->getWidth() / 60.0f), (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                    spriteAppLogo, window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        buttonFile = new Button((-window->getWidth() / 60.0f) + 3.0f + 9.67f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                               new Label("File", 0.75f, 0.57f,"Lusia_UI",
                                         transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        buttonHelp = new Button((-window->getWidth() / 60.0f) + 3.0f + 9.67f + 3.0f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                new Label("Help", 0.5f, 0.57f,"Lusia_UI",
                                          transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        buttonCloseApp = new Button((window->getWidth() / 60.0f) - 3.0f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                  spriteCloseLogo, window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;
        float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);

        buttonZoomIn = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                  new Label("+", 0.9f, 0.9f,"Lusia_UI",
                                            transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        buttonZoomOut = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 2.6f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                   new Label("-", 1.0f, 1.0,"Lusia_UI",
                                             transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        groupFpsLog = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        Sprite* fpsSprite = new Sprite(mapPosX + mapSize - 6.0f, mapPosY + mapSize - 3.0f, 6.0f, 3.0f, transform_color(0.2, 0.2, 0.2, 0.7f));
        logFps = new Label("FPS: ", mapPosX + mapSize - 5.5f, mapPosY + mapSize - 1.9f, "Lusia_UI", transform_color(1, 1, 1, 1));
        groupFpsLog->add(fpsSprite);
        groupFpsLog->add(logFps);

        unsigned int colorHeader = transform_color(0.28647, 0.18647, 0.19824, 1);
        unsigned int  colorBox =  transform_color(0.12647, 0.12647, 0.13824, 1);

        float subWindowDataSize = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowDataX = -window->getWidth() / 60.0f;
        float subWindowDataY = (window->getHeight() / 60.0f) - spriteHeader->getSize().y - subWindowDataSize / 2.0f;
        subWindowData = new SubWindow(subWindowDataX, subWindowDataY, subWindowDataSize, subWindowDataSize / 2.0f,
                                      "Flight data",
                                      shaderScroll1, shaderSubWindow1, orthographicMatrix, window, colorHeader, colorBox);
        subWindowData->m_PaddingUp = 1.0f;
        subWindowData->m_PaddingParagraph = 1.0f;
        subWindowData->m_PaddingLeft = 1.0f;
        subWindowData->m_PaddingRight = 1.0f;
        subWindowData->m_PaddingDown = 1.0f;
        subWindowData-> m_StringInterval = 0.8f;
        AddDataTextSubWindowData(subWindowData);

        float subWindowGraphicsSize = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowGraphicsX = -window->getWidth() / 60.0f;
        float subWindowGraphicsY = window->getHeight() / 60.0f - spriteHeader->getSize().y
                - subWindowDataSize / 2.0f - subWindowGraphicsSize / 1.4f;
        subWindowGraphics = new SubWindow(subWindowGraphicsX,subWindowGraphicsY,
                                          subWindowGraphicsSize, subWindowGraphicsSize / 1.4f, "Graphics", shaderScroll2, shaderSubWindow2, orthographicMatrix, window,
                                              colorHeader, colorBox);
        subWindowGraphics->m_PaddingUp = 1.0f;
        subWindowGraphics->m_PaddingParagraph = 1.0f;
        subWindowGraphics->m_PaddingLeft = 1.0f;
        subWindowGraphics->m_PaddingRight = 1.0f;
        subWindowGraphics->m_PaddingDown = 1.0f;
        subWindowGraphics-> m_StringInterval = 0.5f;
        AddDataTextSubWindowGraphics(subWindowGraphics);

        float subWindowSurfaceTypeSizeX = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowSurfaceTypeSizeY = subWindowGraphicsY - (-window->getHeight() / 60.0) - spriteFooter->getSize().y;
        float subWindowSurfaceTypeX = -window->getWidth() / 60.0f;
        float subWindowSurfaceTypeY = (-window->getHeight() / 60.0) + spriteFooter->getSize().y;
        subWindowSurfaceType = new SubWindow(subWindowSurfaceTypeX, subWindowSurfaceTypeY,
                                             subWindowSurfaceTypeSizeX, subWindowSurfaceTypeSizeY, "Surface type", shaderScroll3, shaderSubWindow3, orthographicMatrix, window,
                                          colorHeader, colorBox);
        subWindowSurfaceType->m_PaddingUp = 1.0f;
        subWindowSurfaceType->m_PaddingParagraph = 1.0f;
        subWindowSurfaceType->m_PaddingLeft = 1.0f;
        subWindowSurfaceType->m_PaddingRight = 1.0f;
        subWindowSurfaceType->m_PaddingDown = 1.0f;
        subWindowSurfaceType-> m_StringInterval = 0.5f;
        AddDataTextSubWindowSurfaceType(subWindowSurfaceType);

        textureCursorCross = new Texture("", "../images/UI/cross.png");
        spriteCursorCross = new Sprite(0, 0, 0, 1, 1, textureCursorCross);

        layer1->add(spriteMapGrid);
        layer1->add(groupHeader);
        layer1->add(groupFooter);
        layer1->add(buttonFlightDataHandler);
        layer1->add(buttonFile);
        layer1->add(buttonHelp);
        layer1->add(buttonApp);
        layer1->add(buttonCloseApp);
        layer1->add(groupFpsLog);
    }

    void tick() override
    {
        logFps->text = "FPS: " + std::to_string(getFPS());
    }

    bool canZoomIn = true;
    bool canZoomOut = true;
    bool loaded = false;
    void update(TimeStep ts) override {
        srand(time(NULL));

        if (map != nullptr)
        {
            if (map->isLoaded()) {
                layer1->remove(spriteMapGrid);
            }

            float xFloat = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
            float yFloat = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);

            auto pixelCoords = CoordsConverter::FloatToPixels(vec2(xFloat, yFloat), window, (window->getHeight() / 60.0f), /*(window->getWidth() / 60.0f)*/(window->getWidth() / 60.0f));
            auto pixelsFinal = map->GetAbsoluteMapPixelCoords(pixelCoords, map->CurrentZoom);

            latLonCoords = CoordsConverter::PixelXYToLatLong(pixelsFinal, map->CurrentZoom);

            if (xFloat > map->getBoundingBoxPosition().x && xFloat < map->getBoundingBoxPosition().x + map->getBoundingBoxSize().x
                && yFloat > map->getBoundingBoxPosition().y && yFloat < map->getBoundingBoxPosition().y + map->getBoundingBoxSize().y)
            {
                logLatLongInfo->text = "Lat,Long: " + std::to_string(latLonCoords.x_d) + ", " + std::to_string(latLonCoords.y_d);
                if (!isCursorAdded)
                {
                    spriteCursorCross->m_Texture = new Texture("", "../images/UI/cross.png");
                    window->isCursorDisabled = true;
                    layer1->add(spriteCursorCross);
                    isCursorAdded = true;
                }
                if (isCursorAdded) {
                    spriteCursorCross->m_Position = vec2(xFloat - spriteCursorCross->m_Size.x / 2.0f,
                                                   yFloat - spriteCursorCross->m_Size.x / 2.0f);
                }
                if (xFloat > buttonZoomIn->getPosition().x && xFloat < buttonZoomIn->getPosition().x + buttonZoomIn->getSize().x
                    && yFloat > buttonZoomOut->getPosition().y && yFloat < buttonZoomIn->getPosition().y + buttonZoomOut->getSize().y) {
                        layer1->remove(spriteCursorCross);
                        window->isCursorDisabled = false;
                        isCursorAdded = false;

                }
                else {
                    window->isCursorDisabled = true;
                }
            }
            else {
                layer1->remove(spriteCursorCross);
                isCursorAdded = false;
                window->isCursorDisabled = false;
                logLatLongInfo->text = "Lat,Long:";
            }
        }

        if (buttonFlightDataHandler->isPresed() && map == nullptr && !loaded) {
            float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;
            vec2 mapPos = vec2(mapPosX, mapPosY);
            track = new Track(PathToTrackDataDir, transform_color(1, 0, 0, 1),
                              shaderNoTransparent, orthographicMatrix, window, 10);
            map = new Map(mapPosX, mapPosY, mapSize, Map::MapType::Elevation,
                          track->CenterLatLong, track->SideMeters, track->SideMeters, shaderNoTransparent, orthographicMatrix,
                          window);

            map->AddTrack(track);
            map->LoadData(minZoom, maxZoom);
            map->AddTiles();
            map->AddPointLayer(pointLayer);
            loaded = true;

            logZoomInfo = new Label("Zoom: " + std::to_string(map->CurrentZoom), (window->m_StartWidth / 60.0f) - 48.0f - 0.6f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            logMetersPerPixelInfo = new Label("M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom)), (window->m_StartWidth / 60.0f) - 41.8f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                 transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            logMapScaleInfo = new Label("Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom), (window->m_StartWidth / 60.0f) - 29.4f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                           transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            logLatLongInfo = new Label("Lat,Long:", (window->m_StartWidth / 60.0f) - 18.0f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                    transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);

            groupFooter->add(logMetersPerPixelInfo);
            groupFooter->add(logMapScaleInfo);
            groupFooter->add(logZoomInfo);
            groupFooter->add(logLatLongInfo);

            layer1->add(buttonZoomIn);
            layer1->add(buttonZoomOut);
        }


        if (map != nullptr )
        {
            if (map->m_Track != nullptr)
            {
                if (window->isKeyTyped(GLFW_KEY_H))
                {
                    track->HighlightPointsWithEvolutions(vec2(-3.5, 3.5), vec2(-3.5, 3.5), transform_color(0, 1, 0, 1));
                }

                float x = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
                float y = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);

                for (auto& i : map->m_Track->m_Renderables)
                {
                    bool con1 = (x >= i->getPosition().x) && (x <= i->getPosition().x + i->getSize().x);
                    bool con2 = (y >= i->getPosition().y) && (y <= i->getPosition().y + i->getSize().y);
                    bool con3 = i->getPosition().x <= map->getBoundingBoxPosition().x + map->getBoundingBoxSize().x;
                    bool con4 = i->getPosition().x >= map->getBoundingBoxPosition().x;
                    bool con5 = i->getPosition().y >= map->getBoundingBoxPosition().y;
                    bool con6 = i->getPosition().y <= map->getBoundingBoxPosition().y + map->getBoundingBoxSize().y;
                    if (con1 && con2 && con3 && con4 && con5 && con6 && !buttonZoomIn->isHovered() && !buttonZoomOut->isHovered()) {
                        if (window->isKeyTyped(GLFW_KEY_W)) {
                            FlightData* fd = map->m_Track->GetInfoFromPixelCoords(i,
                                                                                               map->m_Window,
                                                                                               map->mapLayer,
                                                                                               map->m_TileSide,
                                                                                               map->getSize().x,
                                                                                               map->getPosition(),
                                                                                               map->TileLeftUp,
                                                                                               map->CurrentZoom,
                                                                                               map->binaryLoader->constants[map->CurrentZoom - map->MinZoom].xTileCountDisplay);;
                            algorithm = new PulseDataHandler(fd, track->Z_AR, track->Z_ARU);
                            algorithm->CalculateImpulses();
                            track->HighlightPoint(i, transform_color(0.2f, 0.3f, 0.8f, 1.0f));
                            track->AddNewEthalons(reinterpret_cast<double *>(algorithm->m_Impulses), "/Users/maxim_lavrinenko/Documents/projects/FlightDataHandler/Data/Track/ethalons/water.txt");
                            delete fd;
                        }
                        if (window->isKeyTyped(GLFW_KEY_F)) {
                            FlightData* fd = map->m_Track->GetInfoFromPixelCoords(i,
                                                                                  map->m_Window,
                                                                                  map->mapLayer,
                                                                                  map->m_TileSide,
                                                                                  map->getSize().x,
                                                                                  map->getPosition(),
                                                                                  map->TileLeftUp,
                                                                                  map->CurrentZoom,
                                                                                  map->binaryLoader->constants[map->CurrentZoom - map->MinZoom].xTileCountDisplay);;
                            algorithm = new PulseDataHandler(fd, track->Z_AR, track->Z_ARU);
                            algorithm->CalculateImpulses();
                            track->HighlightPoint(i, transform_color(0.2f, 0.3f, 0.8f, 1.0f));
                            track->AddNewEthalons(reinterpret_cast<double *>(algorithm->m_Impulses), "/Users/maxim_lavrinenko/Documents/projects/FlightDataHandler/Data/Track/ethalons/forest.txt");
                            delete fd;
                        }
                        if (window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_1) && (previousPointChosen != i))
                        {
                            pointAdded = false;
                            if (!pointAdded)
                            {
                                pointAdded = true;
                                map->pointedTrackItem = i;
                            }

                            if (!map->isPointAdded)
                                map->isPointAdded = true;

                            pointLayer->remove_all();

                            float floatsInPixel = 2.0f * (window->getWidth() / 60.0f) / (float)window->getWidth();
                            float circleSize = 1.0f;
                            pointLayer->add(new Sprite(x - circleSize / 2.0f,
                                                       y - circleSize / 2.0f,
                                                       circleSize, circleSize,
                                                       new Texture("", "../images/circle.bmp")));
                            map->pointedTrackItem = i;
                            flightData = map->m_Track->GetInfoFromPixelCoords(i,
                                                                           map->m_Window,
                                                                           map->mapLayer,
                                                                           map->m_TileSide,
                                                                           map->getSize().x,
                                                                           map->getPosition(),
                                                                           map->TileLeftUp,
                                                                           map->CurrentZoom,
                                                                           map->binaryLoader->constants[map->CurrentZoom - map->MinZoom].xTileCountDisplay);

                            track->PrintZ_AR_Z_ARUToFile("../Data/SelectedPointInfo.txt");
                            flightData->PrintAllToFile("../Data/SelectedPointInfo.txt");

                            algorithm = new PulseDataHandler(flightData, track->Z_AR, track->Z_ARU);
                            algorithm->CalculateImpulses();
                            algorithm->PrintImpulsesToFile("../Data/SelectedPointInfo.txt");

                            graphics = new Graphics(algorithm->impulsesVector);
                            graphics->DrawImpulseGraph(algorithm->impulsesVector);
                            sleep(1);
                            graphics->DrawImpulseHist(algorithm->impulsesVector);
                            std::string resultSurface = graphics->GetSurfaceType();

                            logSurfaceType->text = resultSurface;


                            logSpeedSSK->text = "Speed (SSK, m/sec):   " + std::to_string(algorithm->GetCurrentSpeedSSK());
                            logSpeedNSK->text = "Speed (NSK, m/sec):   " + std::to_string(algorithm->GetCurrentSpeedNSK());
                            logHeightGPS->text = "Height (GPS, m):   " + std::to_string(algorithm->GetCurrentHeightGPS());
                            logHeightMeasured->text = "Height (measured, m):   " + std::to_string(algorithm->GetCurrentHeightMeasured());
                            logFlightTime->text = "Flight time (sec):   " + std::to_string(algorithm->GetCurrentFlightTime());
                            logLatitude->text = "Latitude (decimal degrees):   " + std::to_string(algorithm->GetCurrentLatitude());
                            logLongitude->text = "Longitude (decimal degrees):   " + std::to_string(algorithm->GetCurrentLongitude());
                            logYaw->text = "Yaw (decimal degrees):   " + std::to_string(algorithm->GetCurrentHead());
                            logPitch->text = "Pitch (decimal degrees):   " + std::to_string(algorithm->GetCurrentPitch());
                            logRoll->text = "Roll (decimal degrees):   " + std::to_string(algorithm->GetCurrentRoll());

                            map->m_Track->pointLatLong = new vec2(flightData->G_Lat, flightData->G_Long);
                            previousPointChosen = i;

                            spriteImpulseGraph->setColor(transform_color(1, 1, 1, 1));
                            if (spriteImpulseGraph->m_Texture != nullptr)
                                spriteImpulseGraph->m_Texture->deleteTexture();
                            spriteImpulseGraph->SetTexture(new Texture("", "graph.jpg"));

                            spriteImpulseHist->setColor(transform_color(1, 1, 1, 1));
                            if (spriteImpulseHist->m_Texture != nullptr)
                                spriteImpulseHist->m_Texture->deleteTexture();
                            spriteImpulseHist->SetTexture(new Texture("", "hist.jpg"));

                            spriteAltGraph->setColor(transform_color(1, 1, 1, 1));
                            if (spriteAltGraph->m_Texture != nullptr)
                                spriteAltGraph->m_Texture->deleteTexture();
                            spriteAltGraph->SetTexture(new Texture("", "alt_graph.jpg"));

                            spriteSurfTypeGraph->setColor(transform_color(1, 1, 1, 1));
                            if (spriteSurfTypeGraph->m_Texture != nullptr)
                                spriteSurfTypeGraph->m_Texture->deleteTexture();
                            spriteSurfTypeGraph->SetTexture(new Texture("", "result.jpg"));

                            delete graphics;
                            delete algorithm;

                            break;
                        }
                    }
                }
            }
        }

        if (buttonZoomIn->isPresed() && canZoomIn) {
            if (map != nullptr) {
                map->ZoomIn();
                canZoomIn = false;

                logZoomInfo->text = "Zoom: " + std::to_string(map->CurrentZoom);
                logMetersPerPixelInfo->text = "M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom));
                logMapScaleInfo->text = "Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom);
                logLatLongInfo->text = "Lat,Long: ";
            }
        }
        else if (buttonZoomOut->isPresed()  && canZoomOut) {
            if (map != nullptr) {
                map->ZoomOut();
                canZoomOut = false;

                logZoomInfo->text = "Zoom: " + std::to_string(map->CurrentZoom);
                logMetersPerPixelInfo->text = "M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom));
                logMapScaleInfo->text = "Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom);
                logLatLongInfo->text = "Lat,Long: ";
            }
        }

        else if (buttonCloseApp->isPresed())
            shouldClose = true;

        if (!buttonZoomIn->isPresed()) {
            canZoomIn = true;
        }
        if (!buttonZoomOut->isPresed()) {
            canZoomOut = true;
        }

        float x = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
        float y = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);
        if (map != nullptr &&
                (x > map->getBoundingBoxPosition().x && x < map->getBoundingBoxPosition().x + map->getBoundingBoxSize().x
                 && y > map->getBoundingBoxPosition().y && y < map->getBoundingBoxPosition().y + map->getBoundingBoxSize().y)) {
            if (window->isKeyPressed(GLFW_KEY_RIGHT)) {
                if (map->CanMoveLeft())
                    map->MoveLeft();
            }
            else if (window->isKeyPressed(GLFW_KEY_LEFT)) {
                if (map->CanMoveRight())
                    map->MoveRight();
            }
            else if (window->isKeyPressed(GLFW_KEY_UP)) {
                if (map->CanMoveDown())
                    map->MoveDown();
            }
            else if (window->isKeyPressed(GLFW_KEY_DOWN)) {
                if (map->CanMoveUp())
                    map->MoveUp();
            }
            map->TransformTiles();
        }

        if (subWindowData->isHovered()) {
            if (window->isKeyPressed(GLFW_KEY_UP))
                subWindowData->ScrollUp(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_DOWN))
                subWindowData->ScrollDown(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_RIGHT))
                subWindowData->ScrollRight(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_LEFT))
                subWindowData->ScrollLeft(ts.GetMilliseconds());
        }
        if (subWindowGraphics->isHovered()) {
            if (window->isKeyPressed(GLFW_KEY_UP))
                subWindowGraphics->ScrollUp(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_DOWN))
                subWindowGraphics->ScrollDown(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_RIGHT))
                subWindowGraphics->ScrollRight(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_LEFT))
                subWindowGraphics->ScrollLeft(ts.GetMilliseconds());
        }
        if (subWindowSurfaceType->isHovered()) {
            if (window->isKeyPressed(GLFW_KEY_UP))
                subWindowSurfaceType->ScrollUp(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_DOWN))
                subWindowSurfaceType->ScrollDown(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_RIGHT))
                subWindowSurfaceType->ScrollRight(ts.GetMilliseconds());
            else if (window->isKeyPressed(GLFW_KEY_LEFT))
                subWindowSurfaceType->ScrollLeft(ts.GetMilliseconds());
        }

#if SLOW_SPEED_ON_SCROLL
        if (subWindowGraphics->m_RenderableMinPositionY + subWindowGraphics->m_Speed > subWindowGraphics->m_PaddingDown) {
            subWindowGraphics->m_Speed = subWindowGraphics->m_SlowSpeed; //0.2f
            std::cout << "SLOW!" << std::endl;
        }
        else
            subWindowGraphics->m_Speed = subWindowGraphics->m_StartSpeed;
        /*if (subWindowGraphics->m_RenderableMinPositionY + subWindowGraphics->m_Speed > subWindowGraphics->m_PaddingDown)
            subWindowGraphics->m_Speed = subWindowGraphics->m_SlowSpeed; //0.2f
        else
            subWindowGraphics->m_Speed = subWindowGraphics->m_StartSpeed;

        if (subWindowGraphics->m_RenderableMaxPositionY - subWindowGraphics->m_Speed <
                subWindowGraphics->m_Size.y - subWindowGraphics->m_HeaderHeight - subWindowGraphics->m_PaddingUp)
            subWindowGraphics->m_Speed = subWindowGraphics->m_SlowSpeed;
        else
            subWindowGraphics->m_Speed = subWindowGraphics->m_StartSpeed;

        if (subWindowGraphics->m_RenderableMinPositionX + subWindowGraphics->m_Speed > subWindowGraphics->m_PaddingLeft)
            subWindowGraphics->m_Speed = subWindowGraphics->m_SlowSpeed;
        else
            subWindowGraphics->m_Speed = subWindowGraphics->m_StartSpeed;

        if (subWindowGraphics->m_RenderableMaxPositionX - subWindowGraphics->m_Speed < subWindowGraphics-> m_Size.x - subWindowGraphics->m_PaddingRight)
            subWindowGraphics->m_Speed = subWindowGraphics->m_SlowSpeed;
        else
            subWindowGraphics->m_Speed = subWindowGraphics->m_StartSpeed;*/

#endif
    }

    bool flag = false;

    void render() override
    {
        if (map != nullptr) {
            map->render();
        }

        subWindowData->Render();
        subWindowGraphics->Render();
        subWindowSurfaceType->Render();

        layer1->render();

    }
};

int main()
{
    App app;
    app.start();

    return 0;
}

//area_and_line - ошибка
