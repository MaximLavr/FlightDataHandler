#include "SparkyEngine/src/sparky.h"

using namespace sparky;
using namespace audio;
using namespace graphics;
using namespace maths;
using namespace physics;

class App : public Sparky
{
private:
    Window* window;

    Layer* layer1;
    Layer* layer2;

    Shader* shader;

    /*buttons*/
    Group* dataButtons;
    Group* dataAnalysisButtons;
    Group* infoButton;

    /*test data*/
    Sprite* spriteMap;
    Sprite* spriteGraphic;
    Texture* testMapImage;
    Texture* testGraphicImage;

    /*text for dataButtons*/
    Label* textDataButton1;
    Label* textDataButton2;

    /*text for dataAnalysisButtons*/
    Label* textDataAnalysisButton1;
    Label* textDataAnalysisButton2;
    Label* textDataAnalysisButton3;
    Label* textDataAnalysisButton4;

    /*text for infoButtons*/
    Label* textInfoButton1;
    Label* textInfoButton2;
    Label* textInfoButton3;
    Label* textInfoButton4;
    Label* textInfoButton5;
    Label* textInfoButton6;
    Label* textInfoButton7;

    float cameraLeft;
    float cameraRight;
    float cameraBottom;
    float cameraUp;
public:
    App()
    {
    }

    ~App()
    {
        delete layer1;
        delete layer2;
    }

    void init() override
    {
        window = createWindow("Test Map", 700, 540);

        FontManager::add(new Font("Arial", "../SparkyEngine/fonts/arial.ttf", 50));
        FontManager::get("Arial")->setScale(window->getWidth() / 11.67f, window->getHeight() / 9.0f);

        FontManager::add(new Font("Arial_small", "../SparkyEngine/fonts/arial.ttf", 30));
        FontManager::get("Arial_small")->setScale(window->getWidth() / 11.67f, window->getHeight() / 9.0f);

        textDataButton1 = new Label("load data", 3.1f, 0.7f,"Arial", transform_color(1, 1, 1, 1));
        textDataButton2 = new Label("analyze data", 2.5f, -1.8f,"Arial", transform_color(1, 1, 1, 1));

        textDataAnalysisButton1 = new Label("pulse shape", 1.1f, 3.6f,"Arial_small", transform_color(1, 1, 1, 1));
        textDataAnalysisButton2 = new Label("moment. hist.", 1.0f, 2.05f,"Arial_small", transform_color(1, 1, 1, 1));
        textDataAnalysisButton3 = new Label("altitude chart", 1.0f, 0.5f,"Arial_small", transform_color(1, 1, 1, 1));
        textDataAnalysisButton4 = new Label("extended obj. graph", 0.3f, -1.05f,"Arial_small", transform_color(1, 1, 1, 1));

        textInfoButton1 = new Label("coords: ", 0.6f, 5.15f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton2 = new Label("velocity: ", 0.6f, 5.15f - 0.8f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton3 = new Label("time: ", 0.6f, 5.15f - 1.6f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton4 = new Label("height GPS/Alt: ", 0.6f, 5.15f - 2.4f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton5 = new Label("evolution: 1)", 0.6f, 5.15f - 3.2f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton6 = new Label("2)", 2.9f, 5.15f - 4.0f,"Arial_small", transform_color(1, 1, 1, 1));
        textInfoButton7 = new Label("3)", 2.9f, 5.15f - 4.8f,"Arial_small", transform_color(1, 1, 1, 1));

        shader = new Shader("../SparkyEngine/src/shaders/basicTexNoLight.vert","../SparkyEngine/src/shaders/basicTexNoLight.frag");

        cameraLeft = -11.67f;   //default: -16.0f
        cameraRight = 11.67f;   //default:  16.0f
        cameraBottom = -9.0f;   //default: -10.0f
        cameraUp = 9.0f;        //default:  10.0f

        mat4 ortho = mat4::orthographic(cameraLeft, cameraRight, cameraBottom, cameraUp, -1.0f, 1.0f);

        layer1 = new Layer(new BatchRenderer2D(), shader,ortho, false);
        layer2 = new Layer(new BatchRenderer2D(), shader,ortho, true);

        testMapImage = new Texture("a", "../images/map/map.png");
        testGraphicImage = new Texture("b", "../images/map/graphic-2.bmp");

        spriteMap = new Sprite(-11.0f, -1.67f, 11.67f, 10.0f, testMapImage);
        spriteGraphic = new Sprite(-11.0f, -8.34f, 6.0f, 6.0f, testGraphicImage);

        //buttonsGroup1
        dataButtons = new Group(maths::mat4().translation(maths::vec3(1.34f, 6.3f, 0.0f)));
        dataButtons->add(new Sprite(0.0f, 0.0f,  9.67f, 2.0f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataButtons->add(new Sprite(0.0f, 0.0f - 2.56f,  9.67f, 2.0f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataButtons->add(textDataButton1);
        dataButtons->add(textDataButton2);

        dataAnalysisButtons = new Group(maths::mat4().translation(maths::vec3(-5.0f + 0.67f, -8.34f + 1.56f, 0.0f)));
        dataAnalysisButtons->add(new Sprite(0.0f, 0.0f,  5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataAnalysisButtons->add(new Sprite(0.0f, 1.56f,  5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataAnalysisButtons->add(new Sprite(0.0f, 2 * 1.56f,  5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataAnalysisButtons->add(new Sprite(0.0f, -1.56f,  5.0f, 1.3f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        dataAnalysisButtons->add(textDataAnalysisButton1);
        dataAnalysisButtons->add(textDataAnalysisButton2);
        dataAnalysisButtons->add(textDataAnalysisButton3);
        dataAnalysisButtons->add(textDataAnalysisButton4);

        //mapInfo
        infoButton = new Group(maths::mat4().translation(maths::vec3(2 * 0.67f, -8.34f, 0.0f)));
        infoButton->add(new Sprite(0.0f, 0.0f, 9.7f, 5.97f, transform_color(0.2f, 0.3f, 0.8f, 1)));
        infoButton->add(textInfoButton1);
        infoButton->add(textInfoButton2);
        infoButton->add(textInfoButton3);
        infoButton->add(textInfoButton4);
        infoButton->add(textInfoButton5);
        infoButton->add(textInfoButton6);
        infoButton->add(textInfoButton7);

        layer1->add(spriteMap);
        layer1->add(spriteGraphic);

        layer2->add(dataButtons);
        layer2->add(dataAnalysisButtons);
        layer2->add(infoButton);
    }

    void tick() override
    {
    }

    void update() override
    {
    }

    void render() override
    {
        layer1->render();
        layer2->render();
    }
};

int main()
{
    App app;
    app.start();

    return 0;
}