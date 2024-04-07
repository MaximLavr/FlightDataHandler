#include "Graphics.h"

#include <utility>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

bool IsFileExist(const char *filename)
{
    FILE *file;
    return file = fopen(filename, "r");
}

std::ifstream::pos_type GetFileSize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

app_graphics::Graphics::Graphics(/*std::vector<double>& timeValues, std::vector<double>& altValues, */std::vector<double>& impulsesValues)
{
    //TimeValues = timeValues;
    //AltValues = altValues;
    ImpulsesValues = impulsesValues;
}

std::string app_graphics::Graphics::GetSurfaceType()
{
    //DrawAltGraph(AltValues, TimeValues);
    //DrawImpulseGraph(ImpulsesValues);
    //sleep(1);
    //DrawImpulseHist(ImpulsesValues);

    WaterEthalonImpulses = ReadImpulsesFromFile("../Data/Track/ethalons/water.txt");
    //forest = read_vector(forest, "../Data/etalons/forest.txt");
    ForestEthalonImpulses = ReadImpulsesFromFile("../Data/Track/ethalons/forest.txt");
    //plowed_field = read_vector(plowed_field, "../Data/etalons/plowed_field.txt");
    //field = read_vector(field, "../Data/etalons/field.txt");

    //std::string result_surface = impulse_hist_and_stand(ImpulsesValues);

    area_and_line(ImpulsesValues);  //!!!!!!!!!!!!!!!!!

    //return result_surface;
    return "Forest";
}

void app_graphics::Graphics::DrawAltGraph(std::vector<double>& alt, std::vector<double>& time)
{
    auto f = figure(true);
    plot(time, alt)->line_width(3);
    save("alt_graph.jpg");
}

void app_graphics::Graphics::DrawImpulseGraph(std::vector<double>& impulses)
{
    auto f = figure(true);
    plot(impulses)->line_width(0.5);
    save("graph.jpg");
}

void app_graphics::Graphics::DrawImpulseHist(std::vector<double>& impulses)
{
#if 1
    std::vector<double> resizedImpulses;
    int n = impulses.size() / 500;
    for (int i = 0; i < impulses.size(); i++)
    {
        //гистограмма из 500 импульсов
        if (i % n == 0)
            resizedImpulses.push_back(impulses[i]);
    }
    auto f = figure(true);
    auto h = hist(resizedImpulses);
    h->normalization(histogram::normalization::probability);

    save("hist.jpg");
#endif
}

std::vector<double> read_vector(std::vector<double> surface, std::string file)
{
    std::string usr;
    std::ifstream iusrfile;
    iusrfile.open(file, std::ifstream::in);
    while (std::getline(iusrfile, usr)) {
        // вывод текста из файла
        surface.push_back(stod(usr));
    }
    iusrfile.close();
    return surface;
}

std::vector<double> app_graphics::Graphics::ReadImpulsesFromFile(const char* filePath)
{
    std::vector<double> impulses;
    std::ifstream f;
    f.open(filePath);
    double impulse;
    while (f >> impulse)
    {
        impulses.push_back(impulse);
    }
    f.close();

    return impulses;
}

app_graphics::map<int, double> app_graphics::Graphics::FillMapWithZeros()
{
    std::map <int, double> map;
    //100000
    for (int i = 0; i < 21483; i++)
    {
        map[i] = 0;
    }
    return map;
}

double app_graphics::Graphics::area_and_line(vector<double>& impulses)
{
#if 1
    // forest : гистограмма из 500 импульсов
    auto f = figure(true);
    int n = WaterEthalonImpulses.size() / 500;
    std::vector<double> resizedImpulsesWater;
    for (int i = 0; i < WaterEthalonImpulses.size(); i++)
    {
        if (i % n == 0)
            resizedImpulsesWater.push_back(WaterEthalonImpulses[i]);
    }
    auto h = hist(resizedImpulsesWater);
    h->edge_color("blue");
    h->normalization(histogram::normalization::probability);
    hold(on);
    //h->face_color({0., 0., 0.5, 0.5});
    //h->edge_color("r");
#endif
    int n1 = ForestEthalonImpulses.size() / 500;
    std::vector<double> resizedImpulsesForest;
    for (int i = 0; i < ForestEthalonImpulses.size(); i++)
    {
        if (i % n1 == 0)
            resizedImpulsesForest.push_back(ForestEthalonImpulses[i]);
    }
    auto h1 = hist(resizedImpulsesForest);
    h1->edge_color("green");
    //h1->face_color({0., 0., 0.5, 0.5});
    h1->normalization(histogram::normalization::probability);
    hold(on);

    int n2 = impulses.size() / 500;
    std::vector<double> resizedImpulsesInput;
    for (int i = 0; i < impulses.size(); i++)
    {
        if (i % n2 == 0)
            resizedImpulsesInput.push_back(impulses[i]);
    }
    auto h2 = hist(resizedImpulsesInput);
    h2->face_alpha(0.6f);
    h2->edge_color("gray");
    //h1->face_color({0., 0., 0.5, 0.5});
    h2->normalization(histogram::normalization::probability);

    save("result.jpg");

#if 0
    std::map <int, double> map1 = FillMapWithZeros();
    std::map <int, double> map2 = FillMapWithZeros();
    std::map <int, double> map12 = FillMapWithZeros();

    auto f = figure(true);
    auto histogramCoordImpulse = hist(coordImpulses); // указанная координата //
    hold(on);
    auto histogramEthalonImpulse = hist(ethalonImpulses); // эталон //

    histogramCoordImpulse->normalization(histogram::normalization::probability);
    histogramCoordImpulse->bin_width(1);

    histogramEthalonImpulse->normalization(histogram::normalization::probability);
    histogramEthalonImpulse->bin_width(1);

    //удалить
    //save("ngb.jpg");

    auto values = histogramCoordImpulse->values().begin();
    auto edges = histogramCoordImpulse->bin_edges().begin();

    for (int i = 0; i < histogramCoordImpulse->values().size(); i++)
        std::cout << &histogramCoordImpulse[i] << std::endl;

    x11.push_back(*edges);
    y11.push_back(0);
    while (values != histogramCoordImpulse->values().end()) {
        //map1[*e_it] = *v_it;
        //x11.push_back(*e_it + h1->bin_width() / 2);
        y11.push_back(*values);
        ++values;
        //++e_it;
    }
    //x11.push_back(*h1->bin_edges().end());
    //y11.push_back(0);


    auto v_itt = histogramEthalonImpulse->values().begin();
    //auto e_itt = h2->bin_edges().begin();
    //x21.push_back(*h2->bin_edges().begin());
    //y21.push_back(0);
    /*while (v_itt != h2->values().end()) {

        //map2[*e_itt] = *v_itt;
        //x21.push_back(*e_itt + h2->bin_width() / 2);
        //y21.push_back(*v_itt);
        ++v_itt;
        ++e_itt;
    }*/
    //x21.push_back(*h2->bin_edges().end());
    //y21.push_back(0);

    int p = 0;
    while (p < 50000) {
        map12[p] = matplot::min(map1[p], map2[p]);
        p++;
    }

    double intersection_area = std::accumulate(std::begin(map12), std::end(map12), 0.0, [](double value, const std::map<int, double>::value_type& p)
                                               { return value + p.second; }
    ); // площадь пересечения эталонов и заданного пути //
#endif
    //return intersection_area;
    srand(time(NULL));
    return rand() % 2;
}

std::string app_graphics::Graphics::impulse_hist_and_stand(vector<double>& x) {
#if 0
    water = read_vector(water, "water.txt");
    forest = read_vector(forest, "forest.txt");
    //field = read_vector(field, "grove.txt"); //переименовать на field
    //plowed_field = read_vector(plowed_field, "field.txt"); //переименовать на plowed_field

    double area_water = area_and_line(x, water);
    vector<double> waterx = x21; x21.clear();
    vector<double> watery = y21; y21.clear();

    double area_forest = area_and_line(x, forest);
    vector<double> forestx = x21; x21.clear();
    vector<double> foresty = y21; y21.clear();

    //double area_field = area_and_line(x, field);
    //vector<double> fieldx = x21; x21.clear();
    //vector<double> fieldy = y21; y21.clear();

    //double area_plowed_field = area_and_line(x, plowed_field);
    //vector<double> plowed_fieldx = x21; x21.clear();
    //vector<double> plowed_fieldy = y21; y21.clear();

    auto f = figure(true);
    hold(on);

    auto p1 = plot(waterx, watery);
    p1->display_name("water");
    p1->line_width(2);
    auto p2 = plot(forestx, foresty);
    p2->display_name("forest");
    p2->line_width(2);
    //auto p3 = plot(fieldx, fieldy);
    //p3->display_name("field");
    //p3->line_width(2);
    //auto p4 = plot(plowed_fieldx, plowed_fieldy);
    //p4->display_name("plowed field");
    //p4->line_width(2);
    auto p5 = plot(x11, y11);
    p5->display_name("selected point");
    p5->line_width(2);
    ::matplot::legend({});

    save("surface_type.jpg");
    // принятое решение о типе поверхности
    decision = area_water > area_forest ? "water" : "forest";

    return decision;
#endif
    return "???";
}

std::string app_graphics::Graphics::FindMaxIntersection(double areaWater, double areaForest) {//, double areaField, double areaPlowedField) {
    if (areaWater >= areaForest)// && areaWater >= areaField && areaWater >= areaPlowedField)
        return "water";
    //else if (areaForest >= areaWater)// && areaForest >= areaField && areaForest >= areaPlowedField)
    return "forest";
    /*else
        if (areaField >= areaWater && areaField >= areaForest && areaField >= areaPlowedField)
            return "field";

        return "plowed field";*/
}
