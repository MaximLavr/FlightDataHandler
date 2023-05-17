#include "../SparkyEngine/src/sparky.h"
#include "UI/button.h"
#include "Map/map_part.h"
#include "Map/map.h"

using namespace sparky;
using namespace graphics;
using namespace maths;
using namespace app_graphics;
using namespace app_utils;

class App : public Sparky
{
private:
    Window* window;

    Layer* layer1;
    Layer* layer2;

    Shader* shader_map;
    Shader* shader2;
    Shader* shaderTransparent;

    /*info*/
    Group* infoButton;

    /*test data*/
    Sprite* spriteGraphic;
    Texture* testGraphicImage;

    /*text for infoButtons*/
    Label* textInfoButton1;
    Label* textInfoButton2;
    Label* textInfoButton3;
    Label* textInfoButton4;
    Label* textInfoButton5;
    Label* textInfoButton6;
    Label* textInfoButton7;

    /*Shift for info button*/
    float xShiftMapInfo;
    float yShiftMapInfo;

    Sprite* infoSprite;

    Sprite* s;
    Sprite* s1;

    float cameraLeft;
    float cameraRight;
    float cameraBottom;
    float cameraUp;

    bool checked = false;

    Button* loadDataButton;
    Button* analyzeDataButton;
    Button* pulseShapeButton;
    Button* momentHistButton;
    Button* altitudeChartButton;
    Button* extendedObjButton;

    /*Map renderer*/
    Map* map;

    Sprite* sprite;

public:
    App()
    {
    }

    ~App()
    {
        delete layer1;
        delete layer2;
        delete map;
    }

    void init() override
    {
        vec2_d coords = app_utils::CoordsConverter::LongLatToTile(60.6226, 56.8219, 11);
        //x = 60.6226	y = 56.8219
        //87608, 40274, 17
        std::cout << "x = " << coords.x_d << "\t" << "y = " << coords.y_d << std::endl;

        window = createWindow("Test Map", 700, 540);

        FontManager::add(new Font("Arial", "../SparkyEngine/fonts/Arial_Cyr.TTF", 50));

        FontManager::get("Arial")->setScale(window->getWidth() / 11.67f, window->getHeight() / 9.0f);

        FontManager::add(new Font("Arial_small", "../SparkyEngine/fonts/Arial_Cyr.TTF", 30));
        FontManager::get("Arial_small")->setScale(window->getWidth() / 11.67f, window->getHeight() / 9.0f);

        textInfoButton1 = new Label("coords: ", 0.6f, 5.15f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton2 = new Label("velocity: ", 0.6f, 5.15f - 0.8f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton3 = new Label("time: ", 0.6f, 5.15f - 1.6f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton4 = new Label("height GPS/Alt: ", 0.6f, 5.15f - 2.4f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton5 = new Label("evolution: 1)", 0.6f, 5.15f - 3.2f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton6 = new Label("2)", 2.9f, 5.15f - 4.0f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton7 = new Label("3)", 2.9f, 5.15f - 4.8f,"Arial_small", transform_color(1, 1, 1, 1));

        shader_map = new Shader("../SparkyEngine/src/shaders/test/shader1.vert","../SparkyEngine/src/shaders/test/shader1.frag");
        //shader_map->setUniform1i("render", 1);
        shader2 = new Shader("../SparkyEngine/src/shaders/basicTexNoLight.vert","../SparkyEngine/src/shaders/basicTexNoLight.frag");
        shaderTransparent = new Shader("../SparkyEngine/src/shaders/transparent.vert","../SparkyEngine/src/shaders/transparent.frag");

        cameraLeft = -11.67f;   //default: -16.0f
        cameraRight = 11.67f;   //default:  16.0f
        cameraBottom = -9.0f;   //default: -10.0f
        cameraUp = 9.0f;        //default:  10.0f

        mat4 ortho = mat4::orthographic(cameraLeft, cameraRight, cameraBottom, cameraUp, -1.0f, 1.0f);

        layer1 = new Layer(new BatchRenderer2D(), shader2, ortho, false);
        layer2 = new Layer(new BatchRenderer2D(), shaderTransparent, ortho, true);

        testGraphicImage = new Texture("b", "../images/graph/graphic-2.bmp");

        spriteGraphic = new Sprite(-11.0f, -8.34f, 6.0f, 6.0f, testGraphicImage);

        //mapInfo
        infoSprite = new Sprite(0.0f, 0.0f, 9.7f, 5.97f, transform_color(0.2f, 0.7f, 0.5f, 1));
        xShiftMapInfo = 2 * 0.67f;
        yShiftMapInfo = -8.34f;
        infoButton = new Group(mat4::translation((maths::vec3(xShiftMapInfo, yShiftMapInfo, 0.0f))));
        infoButton->add(infoSprite);
        infoButton->add(textInfoButton1);
        infoButton->add(textInfoButton2);
        infoButton->add(textInfoButton3);
        infoButton->add(textInfoButton4);
        infoButton->add(textInfoButton5);
        infoButton->add(textInfoButton6);
        infoButton->add(textInfoButton7);

        //images
        layer1->add(spriteGraphic);

        //buttons
        loadDataButton = new Button(1.34f, 6.3f, 9.67f, 2.0f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                    new Label("load data", 3.1f, 0.7f,"Arial",
                                              transform_color(1, 1, 1, 1)), window);
        analyzeDataButton = new Button(1.34f, 6.3f - 2.56f, 9.67f, 2.0f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                    new Label("analyse data", 2.5f, 0.7f,"Arial",
                                              transform_color(1, 1, 1, 1)), window);
        pulseShapeButton = new Button(-5.0f + 0.67f, -8.34f + 3 * 1.56f, 5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                       new Label("pulse shape", 1.1f, 0.5f,"Arial_small",
                                                 transform_color(1, 1, 1, 1)), window);
        momentHistButton = new Button(-5.0f + 0.67f, -8.34f + 2 * 1.56f, 5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                      new Label("moment. hist.", 1.1f, 0.5f,"Arial_small",
                                                transform_color(1, 1, 1, 1)), window);
        altitudeChartButton = new Button(-5.0f + 0.67f, -8.34f + 1.56f, 5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                      new Label("altitude chart", 1.1f, 0.5f,"Arial_small",
                                                transform_color(1, 1, 1, 1)), window);
        extendedObjButton = new Button(-5.0f + 0.67f, -8.34f, 5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1),
                                         new Label("extended obj. graph", 0.3f, 0.5f,"Arial_small",
                                                   transform_color(1, 1, 1, 1)), window);

        layer2->add(loadDataButton);
        layer2->add(analyzeDataButton);
        layer2->add(pulseShapeButton);
        layer2->add(momentHistButton);
        layer2->add(altitudeChartButton);
        layer2->add(extendedObjButton);
        layer2->add(infoButton);

        map = new Map(-5, 2.3f, 10, 10, vec3(60.6226, 56.8219, 2), vec2(5, 5), shader_map, ortho);
        //2^(2*zoom) tiles
        //4, 16, 64, 128, 256, 512, 1024
        //zooms: 2 - 17
        //min map = 3x3

        //нижний левый
        //56.82168/60.62964
        //pixelCoordinate = worldCoordinate * 2zoomLevel

        //tile(0, 0) - левый верхний
        shader_map->setUniform1f("left_position", map->getPosition().x - 6);
        shader_map->setUniform1f("right_position",  map->getPosition().x - 6 + map->getSize().x);
        shader_map->setUniform1f("up_position", map->getPosition().y + 6);
        shader_map->setUniform1f("down_position",  map->getPosition().y + 6 - map->getSize().y);
        //getTile(87608, 40274, 17);
        //2100 -> 5 = 1800
        //2100 -> 10 = 1800
        //~ renderer can redner 150-200 tiles normal(14x14)
        //map->delete_side();


        //map->add_side(map->Left);
        //map->delete_side(map->Left);
        //map->delete_side(map->Left);
        //map->delete_side(map->Right);
        //map->delete_side(map->Right);
        //map->RemoveElements(3, 4);
        //map->delete_side(map->Left);
    }

    void tick() override
    {
    }

    void update() override
    {
        if (window->isKeyPressed(GLFW_KEY_RIGHT))
            map->move_right();
        else if (window->isKeyPressed(GLFW_KEY_LEFT))
            map->move_left();
        if (window->isKeyPressed(GLFW_KEY_UP))
            map->move_up();
        else if (window->isKeyPressed(GLFW_KEY_DOWN))
            map->move_down();

        map->move();

        //std::cout << "count tiles = " << map->m_TilesCounter << std::endl;
    }

    void render() override
    {
        layer1->render();
        layer2->render();
        map->render();
    }
    //!1130 FPS!
    //1 layer = 1360 fps
};

int main()
{
    App app;
    app.start();

    return 0;
}

//NOW FPS = 1150 fps