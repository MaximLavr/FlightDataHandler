#include "../SparkyEngine/src/sparky.h"
#include "UI/button.h"
#include "Map/map.h"
#include "Track/Track.h"
#include "Algorithm/PulseDataHandler.h"
#include "GettingGraphics/Graphics.h"
#include <utility>
#include <time.h>

using namespace sparky;
using namespace graphics;
using namespace maths;
using namespace app_graphics;
using namespace app_utils;

class App : public Sparky
{
private:
    //window size
    float windowWidth = 2500;
    float windowHeight = 1563;

    //camera position
    float cameraLeft;
    float cameraRight;
    float cameraBottom;
    float cameraUp;

    //min max zooms
    int minZoom = 11;
    int maxZoom = 13;

    //window
    Window* window;

    //layers
    MapLayer* layer1;

    //map
    Map* map = nullptr;

    //groups
    Group* groupFpsLog;

    //matric
    mat4 ortho;

    //shaders
    Shader* shader1;
    Shader* shader2;
    Shader* shader3;
    Shader* shader4;

    //labels
    Label* logFps;

    //buttons
    Button* flightDataHandlerButton;
    Button* zoomInButton;
    Button* zoomOutButton;

    //imageColors
    std::vector<int> imageColors;
public:
    App()
    {
    }

    ~App()
    {
        delete window;
        delete layer1;
        delete logFps;
        delete flightDataHandlerButton;
        delete zoomInButton;
        delete zoomOutButton;
        delete map;
        delete shader1;
        delete shader2;
        delete shader3;
        delete shader4;
    }

#if 0
    std::vector<int> GetImagePixelColors1(const char* imageFileName) //rgba
    {
#if 0
        unsigned int bits;
        GLsizei  width, height;
        std::string fileName = "/Users/maxim_lavrinenko/Documents/projects/HeightMapGeneration/images/MainMapElevation/11_zoom/11_1363_618.png";
        unsigned char* pixels = load_image(fileName.c_str(), &width, &height, &bits);
        std::ofstream f;
        f.open("/Users/maxim_lavrinenko/Documents/projects/HeightMapGeneration/file.txt");

        //8388608 bit in image
        //8388608 / 8 = 1048576 char'ов
        //262144 пикселей

        //8388608 bit in image
        //8388608 / 8 = 1048576 char'ов


        //1048576 * 8 = 8388608 char'ов
        //1048576 пикселей

        int k = 0;
        int counter = 0;
        std::vector<unsigned char> onePixelColor;
        vec2 startPosition = vec2(0, 0);
        float size = 0.05f;
        int kX = 1;
        int kY = 1;
        for (int i = 0; i < 8388608; i++)
        {
            if (i % 4 == 0 && i != 0)
            {
                int red = onePixelColor[0] & 0xff;
                int green = onePixelColor[1] & 0xff;
                int blue = onePixelColor[2] & 0xff;
                int alpha = onePixelColor[3] & 0xff;

                //std::cout << counter + 1 << ") pixelColor = " << onePixelColor[0] << ", " << onePixelColor[1] << ", "
                //        << onePixelColor[2] << ", " << onePixelColor[3] << std::endl;
                std::cout << counter + 1 << ") pixelColor = " << red << ", " << green << ", "
                          << blue << ", " << alpha << std::endl;
                f << red << " " << green << " " << blue << " " << alpha << std::endl;
                onePixelColor.clear();
                counter++;

                if (counter < 512 * 120)
                {
                    if (counter % 512 != 0 && counter != 0) {
                        layer1->add(new Sprite(startPosition.x + kX * size, startPosition.y - kY * size, size, size,
                                               vec4(red / 255.0f, green / 255.0f, blue / 255.0f, 1)));
                        kY++;
                    }
                    else
                    {
                        kY = 1;
                        kX++;
                        //layer1->add(new Sprite(startPosition.x + kX * size, startPosition.y - kY * size, size, size, vec4(green / 255.0f, blue / 255.0f, red / 255.0f, 1)));
                    }
                }
            }
            else
            {
                onePixelColor.push_back(pixels[i]);
            }

            //4 chars in one pixel
            //std::cout << i + 1 << ")" << pixels[i] << std::endl;
            //std::cout << i + 1 << ")" << pixels[i] - '0' << std::endl;
        }
        f.close();
#endif

        int x,y,n;
        std::vector<int> colors;
        int counter = 0;
        unsigned char *data = stbi_load(imageFileName, &x, &y, &n, 0);
        std::cout << "x = " << x << std::endl;
        std::cout << "y = " << y << std::endl;
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                colors.push_back(data[counter]);
                colors.push_back(data[counter + 1]);
                colors.push_back(data[counter + 2]);
                colors.push_back(data[counter + 3]);
                counter+=4;
            }
        }

        stbi_image_free(data);

        return colors;
    }
#endif

    void TestReadingElevationFromBinary()
    {
        std::ofstream f;
        f.open("/Users/maxim_lavrinenko/Documents/projects/HeightMapGeneration/colors.txt");

        auto v = map->binaryLoader->GetTileColors(11, 1363, 618);
        float size = 0.06f;
        float xPos = -40.0f;
        float yPos = -10.0f;
        int mulX = 0;
        int mulY = 0;
        int counter = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 512; j++)
            {
                layer1->add(new Sprite(xPos + mulX * size, yPos + mulY * size, size, size,
                                       transform_color(v[counter] / 255.0f, v[counter+1] / 255.0f, v[counter+2] / 255.0f, v[counter+3] / 255.0f)));
                counter+=4;
                mulX +=1;
            }
            mulX = 0;
            mulY -= 1;
        }

        f.close();
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

        shader1 = new Shader("../SparkyEngine/src/shaders/test/shader1.vert","../SparkyEngine/src/shaders/test/shader1.frag");
        shader2 = new Shader("../SparkyEngine/src/shaders/test/shader2.vert","../SparkyEngine/src/shaders/test/shader2.frag");
        shader3 = new Shader("../SparkyEngine/src/shaders/basicTexNoLight.vert","../SparkyEngine/src/shaders/basicTexNoLight.frag");
        shader4 = new Shader("../SparkyEngine/src/shaders/transparent.vert","../SparkyEngine/src/shaders/transparent.frag");

        ortho = mat4::orthographic(-window->getWidth() / 60.0f, (window->getWidth() / 60.0f), -(window->getHeight() / 60.0f), (window->getHeight() / 60.0f), -1.0f, 1.0f);

        layer1 = new MapLayer(new BatchRenderer2D(window), shader4, ortho, true);


        flightDataHandlerButton = new Button((-window->getWidth() / 60.0f) + 20.0f, (window->getHeight() / 60.0f) - 1.7f, 9.67f, 2.0f - 0.3f, transform_color(0.17647, 0.17647, 0.18824, 1),
                                             new Label("FlightDataHandler", 1.0f, 0.57f,"Lusia_UI_bold",
                                                       transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
        float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;
        float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);

        zoomInButton = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                  new Label("+", 0.9f, 0.9f,"Lusia_UI",
                                            transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));

        zoomOutButton = new Button(mapPosX + 0.1f, mapPosY + mapSize - 2.5f - 2.6f - 0.1f, 2.5f, 2.5f, transform_color(0.12647, 0.12647, 0.12824, 1),
                                   new Label("-", 1.0f, 1.0,"Lusia_UI",
                                             transform_color(1, 1, 1, 1)), window, vec4(1, 1, 1, 0.2f), vec4(0, 0, 0, 0.3f));


        //Fps
        groupFpsLog = new Group(mat4::translation((maths::vec3(0, 0, 0.0f))));
        Sprite* fpsSprite = new Sprite(mapPosX + mapSize - 6.0f, mapPosY + mapSize - 3.0f, 6.0f, 3.0f, transform_color(0.2, 0.2, 0.2, 0.7f));
        logFps = new Label("FPS: ", mapPosX + mapSize - 5.5f, mapPosY + mapSize - 1.9f, "Lusia_UI", transform_color(1, 1, 1, 1));
        groupFpsLog->add(fpsSprite);
        groupFpsLog->add(logFps);

        layer1->add(groupFpsLog);
        layer1->add(flightDataHandlerButton);
        //layer1->add(new Sprite(-50, -100, 20, 300, transform_color(0, 1, 0, 1)));

#if 0
        //imageColors = GetImagePixelColors1("/Users/maxim_lavrinenko/Documents/projects/HeightMapGeneration/images/MainMapElevation/11_zoom/11_1363_618.png");
        //std::cout << "size imageColors = " << imageColors.size() << std::endl;
        //std::ofstream file;
        //file.open("/Users/maxim_lavrinenko/Documents/projects/HeightMapGeneration/colors.txt");
        //int counter = 1;
        file << counter << ") ";

        float xPos = -10;
        float yPos = 0;
        float size = 0.06f;
        int xMul = 0;
        int yMul = 0;
        int k = 0;
        for (int i = 0; i < 110; i++)
        {
            for (int j = 0; j < 512; j++)
            {
                layer1->add(new Sprite(xPos + xMul * size, yPos + yMul * size, size, size, transform_color(imageColors[k] / 255.0f, imageColors[k+1] / 255.0f, imageColors[k+2] / 255.0f, 1)));
                xMul += 1;
                k+=4;
            }
            xMul = 0;
            yMul -=1;
        }
        for (int i = 0; i < imageColors.size(); i++)
        {
            if (i % 4 == 0 && i != 0) {
                counter++;
                file << "\n" << counter << ") ";
            }
            file << imageColors[i] << ", ";
        }
        file.close();
#endif
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
            float xFloat = window->getMousePosition().x / (window->getWidth() / window->cameraRight / 2.0f) - window->cameraRight;
            float yFloat = -(window->getMousePosition().y / (window->getHeight() / window->cameraUp / 2.0f) - window->cameraUp);

            if (window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_1))
            {
                //TestReadingElevationFromBinary();
            }
            /*map->GetMapPixelColor(vec2_i(window->getMousePosition().x,
                                         window->getMousePosition().y));*/
        }

        if (flightDataHandlerButton->isPresed() && map == nullptr && !loaded) {
            float mapSize = ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosX = (window->m_StartWidth / 60.0f) - ((window->m_StartHeight / 60.0f) + (window->m_StartHeight / 60.0f) - 1.7f - 1.7f);
            float mapPosY = -(window->m_StartHeight / 60.0f) + 1.7f;

            map = new Map(mapPosX, mapPosY, mapSize, Map::MapType::Elevation,
                          vec2_d(57.7072, 60.159), 28794.5, 28794.5, shader1, ortho,
                          window);

            map->LoadData(minZoom, maxZoom);
            map->AddTiles();
            loaded = true;

            layer1->add(zoomInButton);
            layer1->add(zoomOutButton);
        }

        if (zoomInButton->isPresed() && canZoomIn/* && canZoomInMini*/) {
            if (map != nullptr) {
                map->ZoomIn();
                canZoomIn = false;
            }
        }
        else if (zoomOutButton->isPresed()  && canZoomOut/* && canZoomOutMini*/) {
            if (map != nullptr) {
                map->ZoomOut();
                canZoomOut = false;
            }
        }

        if (!zoomInButton->isPresed()) {
            canZoomIn = true;
        }
        if (!zoomOutButton->isPresed()) {
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
    }

    bool flag = false;

    void render() override
    {
        if (map != nullptr) {
            map->render();
        }
        layer1->render();

    }
};

int main()
{
    App app;
    app.start();

    //batch rendering cubes on scene
    //6.553.600 pixels on map

    //BVH.h

    return 0;
}