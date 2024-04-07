#define N_TAGIL 1

#include "../SparkyEngine/src/sparky.h"
#include "UI/button.h"
#include "Map/map.h"
#include "Track/Track.h"
#include "UI/SubWindow.h"
#include "Algorithm/PulseDataHandler.h"
#include "GettingGraphics/Graphics.h"
#include <utility>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define SLOW_SPEED 0

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
    float windowWidth = 2500;// / 1.5f; //2500
    float windowHeight = 1563;// /  1.4f; //1563
    FlightData* flightData;
    int minZoom = 11;
    int maxZoom = 13;

    int minZoomMini = minZoom > 2 ? minZoom - 1 : minZoom;
    int maxZoomMini = maxZoom - 1;

    Window* window;

    Layer* layer1;
    MapLayer* layer2;
    MapLayer* layer3;
    MapLayer* pointLayer;
    Layer* pointTestLayer;

    Layer* mapMask;

    Shader* shader_map1;
    Shader* shader_map2;
    Shader* shader2;
    Shader* shaderTransparent;

    /*info*/
    Group* groupSpriteInfo;
    Group* groupSpriteLog;
    Sprite* spriteInfo;
    Group* groupSurfaceInfo;
    Group* groupSurfaceLog;
    Group* groupFpsLog;
    Group* groupHeader;
    Group* groupFooter;

    /*test data*/
    Sprite* spriteGraphic;
    Texture* imageGraphic;

    /*text for infoButtons*/
    Label* textInfoButton1;
    Label* textInfoButton2;
    Label* textInfoButton3;
    Label* textInfoButton4;
    Label* textInfoButton5;
    Label* textInfoButton6;
    Label* textInfoButton7;
    Label* textInfoButton8;
    Label* textZoomInButton;
    Label* textZoomOutButton;

    Label* surfaceText;
    Label* surfaceLog;

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

    /*Graphics*/
    Label* ImpulseGraph = nullptr;
    Label* ImpulseHist = nullptr;
    Label* AltGraph = nullptr;
    Label* SurfaceTypeGraph = nullptr;
    Texture* textureImpulseGraph = nullptr;
    Texture* textureImpulseHist = nullptr;
    Texture* textureAltGraph = nullptr;
    Texture* textureSurfTypeGraph = nullptr;
    /*Surface type*/
    Label* logSurfaceType = nullptr;
    /*Sprites backgrounds for graphics*/
    Sprite* spriteImpulseGraph = nullptr;
    Sprite* spriteImpulseHist = nullptr;
    Sprite* spriteAltGraph = nullptr;
    Sprite* spriteSurfTypeGraph = nullptr;

    Label* textFps;
    Label* logFps;

    float cameraLeft;
    float cameraRight;
    float cameraBottom;
    float cameraUp;

    bool checked = false;

    Button* flightDataHandlerButton;
    Button* fileButton;
    Button* helpButton;
    Button* closeAppButton;
    Button* zoomInButton;
    Button* zoomOutButton;
    Button* appButton;

    Map* map = nullptr;
    Map* mapMini = nullptr;

    Track* track = nullptr;

    Sprite* spritePoint;
    Sprite* appLogo;
    Sprite* closeLogo;

    mat4 ortho;

    Graphics* graphics;
    Sprite* surface;

    Sprite* footerSprite;
    Sprite* headerSprite;
    Sprite* mapMiniSprite;

    //MapMask
    Sprite* spriteMapGrid;

    Group* groupZoomInfo;
    Group* groupMetersPerPixelInfo;
    Label* zoomInfo;
    Label* latLongInfo;
    Label* metersPerPixelInfo;
    Label* mapScaleInfo;

    vec2_d latLonCoords;

    //Sprite* cursorCross;
    bool isCursorAdded;

    //Sprite* spriteDataSide;
    Group* groupDataSide;

    SubWindow* subWindowData;
    SubWindow* subWindowGraphics;
    SubWindow* subWindowSurfaceType;
    Shader* shaderScroll1;
    Shader* shaderSubWindow1;

    Shader* shaderScroll2;
    Shader* shaderSubWindow2;

    Shader* shaderScroll3;
    Shader* shaderSubWindow3;


    bool pointAdded;
    Sprite* headerSub;

    int counterStringInput = 0;
    std::string path;

    Renderable2D* previousPointChoosed;
    Texture* textureCursorCross;
    Sprite* cursorCross;

    PulseDataHandler* algorithm;
    Texture* impulseGraph;
    Texture* impulseHist;

    bool isOpenButtonAdded = false;
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
        if (ImpulseGraph == nullptr) {
            subWindow->currentMaxY = subWindow->m_Size.y - subWindow->m_HeaderHeight
                                     - subWindow->m_PaddingUp
                                     - float(1) * FontManager::get("Lusia_UI")->GetHeight("W")
                                     - float(0) * subWindow->m_StringInterval;
            ImpulseGraph = new Label("Impulse graph",
                                     subWindow->m_PaddingLeft,
                                     subWindow->currentMaxY,
                                     "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(ImpulseGraph);
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

        if (ImpulseHist == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            ImpulseHist = new Label("Impulse hist",
                                    subWindow->m_PaddingLeft,
                                    subWindow->currentMaxY,
                                    "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(ImpulseHist);
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

        if (AltGraph == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            AltGraph = new Label("Alt graph",
                                    subWindow->m_PaddingLeft,
                                    subWindow->currentMaxY,
                                    "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(AltGraph);
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

        if (SurfaceTypeGraph == nullptr) {
            subWindow->currentMaxY -= float(1) * FontManager::get("Lusia_UI")->GetHeight("W");
            subWindow->currentMaxY -= subWindow->m_PaddingUp;
            SurfaceTypeGraph = new Label("Surface type graph",
                                 subWindow->m_PaddingLeft,
                                 subWindow->currentMaxY,
                                 "Lusia_UI", transform_color(1, 1, 1, 1));
            subWindow->AddDataText(SurfaceTypeGraph);
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
        delete layer2;
        delete map;
        delete flightData;
        delete window;
    }

    void init() override
    {
        cameraRight = windowWidth / 60.0f;
        cameraLeft = -cameraRight;
        cameraUp = windowHeight / 60.0f;
        cameraBottom = -cameraUp;

        window = createWindow("Test Map", windowWidth, windowHeight, cameraLeft, cameraRight, cameraBottom, cameraUp); //700, 540

        FontManager::add(new Font("Arial", "../SparkyEngine/fonts/Arial_Cyr.TTF", 50));

        //11.67
        //9.0

        FontManager::get("Arial")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Arial_small", "../SparkyEngine/fonts/Arial_Cyr.TTF", 30));
        FontManager::get("Arial_small")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Arial_big", "../SparkyEngine/fonts/Arial_Cyr.TTF", 70));
        FontManager::get("Arial_big")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        FontManager::add(new Font("Lusia_UI_bold", "../fonts/sf/SF-Pro-Display-Bold.otf", 55));
        FontManager::get("Lusia_UI_bold")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));
        FontManager::add(new Font("Lusia_UI", "../fonts/lucidagrande.ttf", 55));
        FontManager::get("Lusia_UI")->setScale(window->getWidth() / (window->getWidth() / 60.0f), window->getHeight() / (window->getHeight() / 60.0f));

        shader_map1 = new Shader("../SparkyEngine/src/shaders/test/shader1.vert","../SparkyEngine/src/shaders/test/shader1.frag");
        shader_map2 = new Shader("../SparkyEngine/src/shaders/test/shader2.vert","../SparkyEngine/src/shaders/test/shader2.frag");
        shader2 = new Shader("../SparkyEngine/src/shaders/basicTexNoLight.vert","../SparkyEngine/src/shaders/basicTexNoLight.frag");
        shaderTransparent = new Shader("../SparkyEngine/src/shaders/transparent.vert","../SparkyEngine/src/shaders/transparent.frag");
        shaderScroll1 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow1 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");
        shaderScroll2 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow2 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");
        shaderScroll3 = new Shader("../SparkyEngine/src/shaders/test/shaderScroll.vert","../SparkyEngine/src/shaders/test/shaderScroll.frag");
        shaderSubWindow3 = new Shader("../SparkyEngine/src/shaders/test/shaderSubWindow.vert","../SparkyEngine/src/shaders/test/shaderSubWindow.frag");

        ortho = mat4::orthographic(-window->getWidth() / 60.0f, (window->getWidth() / 60.0f), -(window->getHeight() / 60.0f), (window->getHeight() / 60.0f), -1.0f, 1.0f);

        //layer1 = new Layer(new BatchRenderer2D(window), shaderTransparent, ortho, true);
        layer2 = new MapLayer(new BatchRenderer2D(window), shaderTransparent, ortho, true);
        pointLayer = new MapLayer(new BatchRenderer2D(window), shader_map1, ortho, true);
        //mapMask = new Layer(new BatchRenderer2D(window), shaderTransparent, ortho, true);

        //Header
        groupHeader = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        //headerSprite = new Sprite(-window->getWidth() / 60.0f, (window->getHeight() / 60.0f) - 1.7f, (window->getWidth() / 60.0f) - (-window->getWidth() / 60.0f), 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1));
        headerSprite = new Sprite(-window->getWidth() / 60.0f, (window->getHeight() / 60.0f) - 1.7f, (window->getWidth() / 60.0f) - (-window->getWidth() / 60.0f), 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1));
        float xSub, ySub, sizeSubX, sizeSubY;
        sizeSubY = 1.7f + 0.1;
        groupHeader->add(headerSprite);
        appLogo = new Sprite(-window->getWidth() / 60.0f + 0.9f, (window->getHeight() / 60.0f) - 1.45f, 0, 1.2f, 1.2f, new Texture("", "../images/UI/logo_white.png"));
        closeLogo = new Sprite((window->getWidth() / 60.0f) - 2.15f, (window->getHeight() / 60.0f) - 1.4f, 0, 1.3f, 1.0f, new Texture("", "../images/UI/close.png"));
        //appButton = new Button()

        //Footer
        groupFooter = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        footerSprite = new Sprite(-window->getWidth() / 60.0f, -(window->getHeight() / 60.0f), (window->getWidth() / 60.0f) - (-window->getWidth() / 60.0f), 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1));
        //groupFooter->add(footerSprite);
        //groupZoomInfo = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        //groupMetersPerPixelInfo = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));

        //groupZoomInfo->add(zoomInfo);
        //groupZoomInfo->add(zoomLog);

        //Map grid
        float x = (window->getWidth() / 60.0f) - ((window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f);
        float y = -(window->getHeight() / 60.0f) + 1.7f;
        float side = (window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f;
        //0.09804  0.09804  0.09804
        spriteMapGrid = new Sprite(x, y, side, side, transform_color(0.07, 0.07, 0.07, 1));
        mapMiniSprite = new Sprite(x + 3 * side / 4.0f - 0.0125f * side / 3.4f, y, side / 3.95f, side / 3.95f, transform_color(0.17647, 0.17647, 0.18824, 1));

        //DataSide
        groupDataSide = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        /*spriteDataSide = new Sprite((-window->getWidth() / 60.0f), -(window->getHeight() / 60.0f) + footerSprite->getSize().y,
                                    spriteMapGrid->getPosition().x - (-window->getWidth() / 60.0f), (window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 2 * footerSprite->getSize().y,
                                    transform_color(0.17647, 0.17647, 0.18824, 1));*/
        //groupDataSide->add(spriteDataSide);
        groupDataSide->add(new Label("Flight data", (-window->getWidth() / 60.0f) + 1.35f, (window->getHeight() / 60.0f) - 2.85f,"Lusia_UI",
                                     transform_color(1, 1, 1, 1)));

        //images
        //layer2->add(spriteGraphic);

        //buttons
        flightDataHandlerButton = new Button((-window->getWidth() / 60.0f) + 3.0f, (window->getHeight() / 60.0f) - 1.7f, 9.67f, 2.0f - 0.3f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                    new Label("FlightDataHandler", 1.0f, 0.57f,"Lusia_UI_bold",
                                              transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        appButton = new Button((-window->getWidth() / 60.0f), (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                    new Label("", 1.0f, 0.57f,"Lusia_UI_bold",
                                              transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        fileButton = new Button((-window->getWidth() / 60.0f) + 3.0f + 9.67f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                               new Label("File", 0.75f, 0.57f,"Lusia_UI",
                                         transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        helpButton = new Button((-window->getWidth() / 60.0f) + 3.0f + 9.67f + 3.0f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                new Label("Help", 0.5f, 0.57f,"Lusia_UI",
                                          transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));
        closeAppButton = new Button((window->getWidth() / 60.0f) - 3.0f, (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                  new Label("", 0.75f, 0.57f,"Lusia_UI",
                                            transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));


        //appButton = new Button((-window->getWidth() / 60.0f), (window->getHeight() / 60.0f) - 1.7f, 3.0f, 1.7f, transform_color(0.17647, 0.17647, 0.18824, 1),
          //                     new Label("", 1.0f, 0.57f,"Lusia_UI_bold",
            //                             transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;
        float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        /*zoomInButton = new Button((window->getWidth() / 60.0f) - ((window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f) + 1.7f, 20.1f, 2.5f, 2.5f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                     new Label("+", 0.9f, 0.9f,"Lusia_UI",
                                             transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));*/
        zoomInButton = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                  new Label("+", 0.9f, 0.9f,"Lusia_UI",
                                            transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        zoomOutButton = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 2.6f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                   new Label("-", 1.0f, 1.0,"Lusia_UI",
                                             transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        layer2->add(spriteMapGrid);

        //Fps
        groupFpsLog = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        Sprite* fpsSprite = new Sprite(mapPosX + mapSize - 6.0f, mapPosY + mapSize - 3.0f, 6.0f, 3.0f, transform_color(0.2, 0.2, 0.2, 0.7f));
        logFps = new Label("FPS: ", mapPosX + mapSize - 5.5f, mapPosY + mapSize - 1.9f, "Lusia_UI", transform_color(1, 1, 1, 1));
        groupFpsLog->add(fpsSprite);
        groupFpsLog->add(logFps);
        layer2->add(groupFpsLog);


        unsigned int colorHeader = transform_color(0.28647, 0.18647, 0.19824, 1);
        unsigned int  colorBox =  transform_color(0.12647, 0.12647, 0.13824, 1);


        float subWindowDataSize = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowDataX = -window->getWidth() / 60.0f;
        float subWindowDataY = (window->getHeight() / 60.0f) - headerSprite->getSize().y - subWindowDataSize / 2.0f;
        subWindowData = new SubWindow(subWindowDataX, subWindowDataY, subWindowDataSize, subWindowDataSize / 2.0f,
                                      "Flight data",
                                      shaderScroll1, shaderSubWindow1, ortho, window, colorHeader, colorBox);
        subWindowData->m_PaddingUp = 1.0f;
        subWindowData->m_PaddingParagraph = 1.0f;
        subWindowData->m_PaddingLeft = 1.0f;
        subWindowData->m_PaddingRight = 1.0f;
        subWindowData->m_PaddingDown = 1.0f;
        subWindowData-> m_StringInterval = 0.8f;
        AddDataTextSubWindowData(subWindowData);

        float subWindowGraphicsSize = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowGraphicsX = -window->getWidth() / 60.0f;
        float subWindowGraphicsY = window->getHeight() / 60.0f - headerSprite->getSize().y
                - subWindowDataSize / 2.0f - subWindowGraphicsSize / 1.4f;
        subWindowGraphics = new SubWindow(subWindowGraphicsX,subWindowGraphicsY,
                                          subWindowGraphicsSize, subWindowGraphicsSize / 1.4f, "Graphics", shaderScroll2, shaderSubWindow2, ortho, window,
                                              colorHeader, colorBox);
        subWindowGraphics->m_PaddingUp = 1.0f;
        subWindowGraphics->m_PaddingParagraph = 1.0f;
        subWindowGraphics->m_PaddingLeft = 1.0f;
        subWindowGraphics->m_PaddingRight = 1.0f;
        subWindowGraphics->m_PaddingDown = 1.0f;
        subWindowGraphics-> m_StringInterval = 0.5f;
        AddDataTextSubWindowGraphics(subWindowGraphics);

        float subWindowSurfaceTypeSizeX = 2.0f * window->m_StartWidth / 60.0f - mapSize;
        float subWindowSurfaceTypeSizeY = subWindowGraphicsY - (-window->getHeight() / 60.0) - footerSprite->getSize().y;
        float subWindowSurfaceTypeX = -window->getWidth() / 60.0f;
        float subWindowSurfaceTypeY = (-window->getHeight() / 60.0) + footerSprite->getSize().y;
        subWindowSurfaceType = new SubWindow(subWindowSurfaceTypeX, subWindowSurfaceTypeY,
                                             subWindowSurfaceTypeSizeX, subWindowSurfaceTypeSizeY, "Surface type", shaderScroll3, shaderSubWindow3, ortho, window,
                                          colorHeader, colorBox);
        subWindowSurfaceType->m_PaddingUp = 1.0f;
        subWindowSurfaceType->m_PaddingParagraph = 1.0f;
        subWindowSurfaceType->m_PaddingLeft = 1.0f;
        subWindowSurfaceType->m_PaddingRight = 1.0f;
        subWindowSurfaceType->m_PaddingDown = 1.0f;
        subWindowSurfaceType-> m_StringInterval = 0.5f;
        AddDataTextSubWindowSurfaceType(subWindowSurfaceType);

        textInfoButton1 = new Label("Coords (lat,lon): ", 1.3f, 5.15f + 0.18f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton2 = new Label("Time (sec): ", 1.3f, 5.15f + 0.18f - 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton3 = new Label("Alt (m): ", 1.3f,5.15f + 0.18f - 2 * 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton4 = new Label("GPS_alt (m): ", 1.3f, 5.15f + 0.18f - 3 * 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton5 = new Label("Yaw (deg): ", 1.3f,5.15f + 0.18f - 4 * 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton6 = new Label("Pitch (deg): ", 1.3f, 5.15f + 0.18f - 5 * 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton7 = new Label("Roll (deg): ", 1.3f, 5.15f + 0.18f - 6 * 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textInfoButton8 = new Label("Velocity (m/sec): ", 1.3f, 5.15f + 0.18f - 7 * 0.7f,"Arial", transform_color(1, 1, 1, 1));


        layer2->add(groupHeader);
        layer2->add(footerSprite);
        layer2->add(groupFooter);
        layer2->add(flightDataHandlerButton);
        layer2->add(appButton);
        layer2->add(closeAppButton);
        layer2->add(fileButton);
        layer2->add(helpButton);
        layer2->add(appLogo);
        layer2->add(closeLogo);

        textureCursorCross = new Texture("", "../images/UI/cross.png");
        cursorCross = new Sprite(0, 0, 0, 1, 1, textureCursorCross);


        //float mapSize1 = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        //float mapPosX1 = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        //float mapPosY1 = -(window->m_StartHeight / 60.0f) + 1.7f;
        //mapMask->add(new Sprite(mapPosX1, mapPosY1 + 6.0f, mapSize1 / 3.0f, mapSize1 / 3.0f, transform_color(0.8f, 0.2f, 0.2f, 0.3f)));
    }

    void tick() override
    {
        logFps->text = "FPS: " + std::to_string(getFPS());
    }

    bool canZoomIn = true;
    bool canZoomOut = true;

    bool canZoomInMini = true;
    bool canZoomOutMini = true;

    bool canGetImpulseGraph = false;
    bool canGetImpulseHist = false;
    bool canGetAltGraph = false;
    bool canGetSurfTypeGraph = false;
    bool graph = false;
    bool hist = false;
    bool alt = false;
    bool surf = false;

    bool fl = true;
    int counterRemoved = 5;
    bool test_flag = true;

    bool loaded = false;

    bool pixelTest;
    bool hoverFlag;
    vec2 posLetter;

    void update(TimeStep ts) override {
        //std::cout << "ts = " << ts.GetSeconds() << ", " << ts.GetMilliseconds() << std::endl;
        srand(time(NULL));

        if (mapMini != nullptr) {
            if (mapMini->isLoaded()) {
                //layer2->remove(spriteMapGrid);
            }
        }
        if (map != nullptr)
        {
            if (map->isLoaded()) {
                layer2->remove(spriteMapGrid);
            }

            float xFloat = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
            float yFloat = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);

            //groupCursorCross->m_TransformationMatrix = mat4::translation((maths::vec3(xFloat, yFloat, 0.0f)));
            //vec2_i pixels = CoordsConverter::FloatToPixels(vec2(x, y), window, (window->getHeight() / 60.0f), (window->getWidth() / 60.0f));
            auto pixelCoords = CoordsConverter::FloatToPixels(vec2(xFloat, yFloat), window, (window->getHeight() / 60.0f), /*(window->getWidth() / 60.0f)*/(window->getWidth() / 60.0f));
            auto pixelsFinal = map->GetAbsoluteMapPixelCoords(pixelCoords, map->CurrentZoom);
            latLonCoords = CoordsConverter::PixelXYToLatLong(pixelsFinal, map->CurrentZoom);
#if 1
            if (xFloat > map->getBoundingBoxPosition().x && xFloat < map->getBoundingBoxPosition().x + map->getBoundingBoxSize().x
                && yFloat > map->getBoundingBoxPosition().y && yFloat < map->getBoundingBoxPosition().y + map->getBoundingBoxSize().y)
            {
                latLongInfo->text = "Lat,Long: " + std::to_string(latLonCoords.x_d) + ", " + std::to_string(latLonCoords.y_d);
                if (!isCursorAdded)
                {
                    cursorCross->m_Texture = new Texture("", "../images/UI/cross.png");
                    window->isCursorDisabled = true;
                    layer2->add(cursorCross);
                    isCursorAdded = true;
                }
                if (isCursorAdded) {
                    cursorCross->m_Position = vec2(xFloat - cursorCross->m_Size.x / 2.0f,
                                                   yFloat - cursorCross->m_Size.x / 2.0f);
                }
                if (xFloat > zoomInButton->getPosition().x && xFloat < zoomInButton->getPosition().x + zoomInButton->getSize().x
                    && yFloat > zoomOutButton->getPosition().y && yFloat < zoomInButton->getPosition().y + zoomInButton->getSize().y) {
                        layer2->remove(cursorCross);
                        window->isCursorDisabled = false;
                        isCursorAdded = false;

                    //}
                }
                else //{
                    window->isCursorDisabled = true;
            }
            else {
                layer2->remove(cursorCross);
                isCursorAdded = false;
                window->isCursorDisabled = false;
                latLongInfo->text = "Lat,Long:";
            }
#endif
        }
        /*if (!fileButton->isHovered() && openButton != nullptr) {
            layer2->remove(openButton);
        }*/

        if (flightDataHandlerButton->isPresed() && map == nullptr && !loaded) {
            //float mapSize = (window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f;
            float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;
            vec2 mapPos = vec2(mapPosX, mapPosY);
            track = new Track(PathToTrackDataDir, transform_color(1, 0, 0, 1),
                              shader_map1, ortho, window, 10);
            //trackMini = new Track(pathToTrackFile, (window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f, minZoomMini, maxZoomMini, transform_color(1, 0, 0, 1),
              //                shader_map2, ortho, window, 2);
            //mapMini = new Map((window->getWidth() / 60.0f) - ((window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f) / 4.0f, -(window->getHeight() / 60.0f) + 1.7f, ((window->getHeight() / 60.0f) - -(window->getHeight() / 60.0f) - 1.7f - 1.7f) / 4.0f, track->CenterLatLong, track->SideMeters, track->SideMeters, shader_map2, ortho, (window->getHeight() / 60.0f),
              //              (window->getWidth() / 60.0f), window, "MiniMap");
            map = new Map(mapPosX, mapPosY, mapSize, Map::MapType::Elevation,
                          track->CenterLatLong, track->SideMeters, track->SideMeters, shader_map1, ortho,
                          window);

            //mapMini->speed /= pow(2.0f, (map->getSize().x / mapMini->getSize().x));
            map->AddTrack(track);
            //mapMini->AddTrack(track);
            //mapMini->LoadData(minZoomMini, maxZoomMini);
            map->LoadData(minZoom, maxZoom);
            //mapMini->AddTiles();
            map->AddTiles();
            map->AddPointLayer(pointLayer);
            loaded = true;

            zoomInfo = new Label("Zoom: " + std::to_string(map->CurrentZoom), (window->m_StartWidth / 60.0f) - 48.0f - 0.6f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            metersPerPixelInfo = new Label("M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom)), (window->m_StartWidth / 60.0f) - 41.8f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                 transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            mapScaleInfo = new Label("Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom), (window->m_StartWidth / 60.0f) - 29.4f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                           transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);
            latLongInfo = new Label("Lat,Long:", (window->m_StartWidth / 60.0f) - 18.0f + 0.5f, -(window->m_StartHeight / 60.0f) + 0.6f,"Lusia_UI",
                                    transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f);

            groupFooter->add(metersPerPixelInfo);
            groupFooter->add(mapScaleInfo);
            groupFooter->add(zoomInfo);
            groupFooter->add(latLongInfo);

            layer2->add(zoomInButton);
            layer2->add(zoomOutButton);
        }


        if (map != nullptr )
        {
            if (map->m_Track != nullptr)
            {
                if (window->isKeyTyped(GLFW_KEY_H))
                {
                    track->HighlightPointsWithEvolutions(vec2(-3.5, 3.5), vec2(-3.5, 3.5), transform_color(0, 1, 0, 1));
                }
                //float x = window->getMousePosition().x / ((float)window->getWidth() / (window->getWidth() / 60.0f) / 2.0f) - (window->getWidth() / 60.0f);
                //float y = -(window->getMousePosition().y / ((float)window->getHeight() / (window->getHeight() / 60.0f) / 2.0f) - (window->getHeight() / 60.0f));
                float x = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
                float y = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);


                //float x = (window->getMousePosition().x - window->getWidth() / 4.0f) / 30.0f;
                //float y = -(window->getMousePosition().y - window->getHeight() / 4.0f) / 30.0f;
                for (auto& i : map->m_Track->m_Renderables)
                {
                    bool con1 = (x >= i->getPosition().x) && (x <= i->getPosition().x + i->getSize().x);
                    bool con2 = (y >= i->getPosition().y) && (y <= i->getPosition().y + i->getSize().y);
                    bool con3 = i->getPosition().x <= map->getBoundingBoxPosition().x + map->getBoundingBoxSize().x;
                    bool con4 = i->getPosition().x >= map->getBoundingBoxPosition().x;
                    bool con5 = i->getPosition().y >= map->getBoundingBoxPosition().y;
                    bool con6 = i->getPosition().y <= map->getBoundingBoxPosition().y + map->getBoundingBoxSize().y;
                    if (con1 && con2 && con3 && con4 && con5 && con6 && !zoomInButton->isHovered() && !zoomOutButton->isHovered()) {
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
                        if (window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_1) && (previousPointChoosed != i))
                        {
                            pointAdded = false;
                            if (!pointAdded)
                            {
                                pointAdded = true;
                                /*pointLayer->add(new Sprite(i->getPosition().x - 0.5f, i->getPosition().y - 0.5f, 1.0f, 1.0f,
                                                           new Texture("", "../images/circle.bmp")));*/
                                map->pointedTrackItem = i;
                            }
#if 1
                            //spriteGraphic->DeleteTexture();
                            //spriteGraphic->setColor(transform_color(0.8f, 0.8f, 0.8f, 0.1f));
                            if (!map->isPointAdded)
                                map->isPointAdded = true;
                            //groupSpriteLog->remove_all();
                            pointLayer->remove_all();
                            //pos.x + floatsInPixel * m_Track->m_ScalePointSize - 0.5f, pos.y + floatsInPixel * m_Track->m_ScalePointSize - 0.5f, 1.0f, 1.0f
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


                            //TURN ON!!!
                            /*logLatLon->text = "Coords (lat,lon):   " + std::to_string(allData[0]) + ", " + std::to_string(allData[1]);
                            logTime->text = "Time (sec):   " + std::to_string(allData[2]);
                            logAlt->text = "Alt (m):   " + std::to_string(allData[3]);
                            logGPSAlt->text = "GPS_alt (m):   " + std::to_string(allData[4]);
                            logHead->text = "Head (deg):   " + std::to_string(allData[5]);
                            logPitch->text = "Pitch (deg):   " + std::to_string(allData[6]);
                            logRoll->text = "Roll (deg):   " + std::to_string(allData[7]);
                            logVelocity->text = "Velocity (m/sec):   " + std::to_string(allData[8]);*/
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
                            //TURN ON!!!
#endif

                            //map->m_PointLatLong = new vec2(1, 1);
                            map->m_Track->pointLatLong = new vec2(flightData->G_Lat, flightData->G_Long);
                            previousPointChoosed = i;

                            //map->m_PointLatLong = new vec2(a[0], a[1]);

                            //18
                            //subWindow->AddDataText()
                            //subWindow->add(logLatLon);
                            /*groupSpriteLog->add(logLatLon);
                            groupSpriteLog->add(logTime);
                            groupSpriteLog->add(logAlt);
                            groupSpriteLog->add(logGPSAlt);
                            groupSpriteLog->add(logYaw);
                            groupSpriteLog->add(logPitch);
                            groupSpriteLog->add(logRoll);
                            groupSpriteLog->add(logVelocity);*/

                            //std::cout << "lat, lon: " << allData[0] << ", " << allData[1] << std::endl;

                            /*std::cout << "times: ";
                            std::cout << map->m_Track->times.size() << std::endl;

                            std::cout << "alts: ";
                            std::cout << map->m_Track->alts.size() << std::endl;

                            std::cout << "impulses: ";
                            std::cout << map->m_Track->impulses.size() << std::endl;*/
                            //graphics = new Graphics(map->m_Track->times, map->m_Track->alts, map->m_Track->impulses);

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
#if 0
                            graphics = nullptr;
                        //graphics = new Graphics(map->m_Track->times, map->m_Track->alts, map->m_Track->impulses);
                        //std::string resultSurface = graphics->GetGraphics();
                        logSurfaceType->text = "here is surface type";
                        //surfaceLog = new Label(result_surface, 1.9f, 7.0f,"Arial_big", transform_color(1, 1, 1, 1));
                        //groupSurfaceLog->remove_all();
                        spriteImpulseGraph->m_Texture = nullptr;
                        spriteImpulseGraph->setColor(transform_color(1, 1, 1, 1));
                        spriteImpulseGraph->SetTexture(new Texture("", "impulse_graph.jpg"));

                        spriteImpulseHist->m_Texture = nullptr;
                        spriteImpulseHist->setColor(transform_color(1, 1, 1, 1));
                        spriteImpulseHist->SetTexture(new Texture("", "impulse_hist.jpg"));

                        spriteAltGraph->m_Texture = nullptr;
                        spriteAltGraph->setColor(transform_color(1, 1, 1, 1));
                        spriteAltGraph->SetTexture(new Texture("", "alt_graph.jpg"));

                        spriteSurfTypeGraph->m_Texture = nullptr;
                        spriteSurfTypeGraph->setColor(transform_color(1, 1, 1, 1));
                        spriteSurfTypeGraph->SetTexture(new Texture("", "surface_type_graph.jpg"));

                        /*groupSurfaceLog->add(surfaceLog);

                        if (map->m_Track->IsPointFound) {
                            canGetImpulseGraph = true;
                            canGetImpulseHist = true;
                            canGetAltGraph = true;
                            canGetSurfTypeGraph = true;
                            std::cout << "track point found!" << std::endl;
                        }
                        else {
                            canGetImpulseGraph = false;
                            canGetImpulseHist = false;
                            canGetAltGraph = false;
                            canGetSurfTypeGraph = false;
                            std::cout << "track point NOT found!" << std::endl;
                        }
                        pixelTest = true;*/
#endif
                            break;
                        }
                    }
                }
            }
        }

        if (zoomInButton->isPresed() && canZoomIn/* && canZoomInMini*/) {
            if (map != nullptr) {
                //if (mapMini != nullptr && mapMini->CurrentZoom < mapMini->MaxZoom) {
                //if (mapMini != nullptr && mapMini->CurrentZoom != map->CurrentZoom)
                  //  mapMini->ZoomIn();
                  //  canZoomInMini = false;
                //}
                map->ZoomIn();
                canZoomIn = false;

                zoomInfo->text = "Zoom: " + std::to_string(map->CurrentZoom);
                metersPerPixelInfo->text = "M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom));
                mapScaleInfo->text = "Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom);
                latLongInfo->text = "Lat,Long: ";
            }
        }
        else if (zoomOutButton->isPresed()  && canZoomOut/* && canZoomOutMini*/) {
            if (map != nullptr) {
                //if (mapMini != nullptr && map->CurrentZoom > minZoom) {
                //if (mapMini != nullptr)
                  //  mapMini->ZoomOut();
                    //canZoomOutMini = false;
                //}

                map->ZoomOut();
                canZoomOut = false;

                zoomInfo->text = "Zoom: " + std::to_string(map->CurrentZoom);
                metersPerPixelInfo->text = "M/pix: " + std::to_string(CoordsConverter::GetMetersPerPixel(map->CurrentZoom));
                mapScaleInfo->text = "Scale: " + CoordsConverter::ScaleToString(map->CurrentZoom);
                latLongInfo->text = "Lat,Long: ";
            }
        }

        /*else if (impulseGraphButton->isPresed() && canGetImpulseGraph && graph) {
            graph = false;
            imageGraphic = new Texture("", "impulse_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);
        }
        else if (impulseHistButton->isPresed() && canGetImpulseHist && hist) {
            hist = false;
            imageGraphic = new Texture("", "impulse_hist.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);
        }
        else if (altGraphButton->isPresed() && canGetAltGraph && alt) {
            alt = false;
            imageGraphic = new Texture("", "alt_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);
        }
        else if (surfTypeGraphButton->isPresed() && canGetSurfTypeGraph && surf) {
            surf = false;
            imageGraphic = new Texture("", "surface_type_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);
        }*/
        else if (closeAppButton->isPresed())
            shouldClose = true;
#if 0
        else if (impulseGraphButton->isPressed() && canGetImpulseGraph)
        {
            std::cout << "impulseGraphButton\n";
            /*imageGraphic = new Texture("", "impulse_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);*/
             canGetImpulseGraph = false;
        }
        else if (impulseHistButton->isPressed() && canGetImpulseHist)
        {
            std::cout << "impulseHistButton\n";
            /*imageGraphic = new Texture("", "impulse_hist.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);*/
            canGetImpulseHist = false;
        }
        else if (altGraphButton->isPressed() && canGetAltGraph)
        {
            std::cout << "altGraphButton\n";
            /*imageGraphic = new Texture("", "alt_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);*/
            canGetAltGraph = false;
        }
        else if (surfTypeGraphButton->isPressed() && canGetSurfTypeGraph)
        {
            std::cout << "surfTypeGraphButton\n";
            /*imageGraphic = new Texture("", "surface_type_graph.jpg");
            spriteGraphic->setColor(transform_color(1, 1, 1, 1));
            spriteGraphic->SetTexture(imageGraphic);*/
            canGetSurfTypeGraph = false;
        }
#endif

        if (!zoomInButton->isPresed()) {
            canZoomIn = true;
            canZoomInMini = true;
        }
        if (!zoomOutButton->isPresed()) {
            canZoomOut = true;
            canZoomOutMini = true;
        }

        /*if (!impulseGraphButton->isPressed())
            graph = true;
        if (!impulseHistButton->isPresed())
            hist = true;
        if (!altGraphButton->isPresed())
            alt = true;
        if (!surfTypeGraphButton->isPresed())
            surf = true;*/

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

        if (mapMini != nullptr) {
            if (window->isKeyPressed(GLFW_KEY_D)) {
                if (mapMini->CanMoveLeft())
                    mapMini->MoveLeft();
            }
            else if (window->isKeyPressed(GLFW_KEY_A)) {
                if (mapMini->CanMoveRight())
                    mapMini->MoveRight();
            }
            else if (window->isKeyPressed(GLFW_KEY_W)) {
                if (mapMini->CanMoveDown())
                    mapMini->MoveDown();
            }
            else if (window->isKeyPressed(GLFW_KEY_S)) {
                if (mapMini->CanMoveUp())
                    mapMini->MoveUp();
            }
            mapMini->TransformTiles();
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
        //if (map != nullptr)
          //  std::cout << map->TileRightDown << std::endl;

#if SLOW_SPEED
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
        //layer1->render();
        if (mapMini != nullptr) {
            //mapMini->render();
        }
#if 1
        subWindowData->Render();
        subWindowGraphics->Render();
        subWindowSurfaceType->Render();
#endif
        layer2->render();
        //mapMask->render();
        //layer1->render();

    }
};

int main()
{
    App app;
    app.start();

    return 0;
    //area_and_line - ошибка
}