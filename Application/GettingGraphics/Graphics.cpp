#include "Graphics.h"

#include <utility>

app_graphics::Graphics::Graphics(std::vector<double>& times, std::vector<double>& alts, std::vector<double>& impulses)
{
    Times = times;
    Alts = alts;
    Impulses = impulses;
}

std::string app_graphics::Graphics::GetGraphics() {
    //alt_graph(Alts, Times);
    //impulse_graph(Impulses);
    //impulse_hist(Impulses);

    water = read_vector(water, "../Data/etalons/water.txt");
    forest = read_vector(forest, "../Data/etalons/forest.txt");
    plowed_field = read_vector(plowed_field, "../Data/etalons/plowed_field.txt");
    field = read_vector(field, "../Data/etalons/field.txt");

    std::string result_surface = impulse_hist_and_stand(Impulses);

    return result_surface;
}

void app_graphics::Graphics::alt_graph(std::vector<double> alt, std::vector<double> time) {
    auto f = figure(true);
    plot(time, alt)->line_width(3);
    save("alt_graph.jpg");
}

void app_graphics::Graphics::impulse_graph(std::vector<double> x) {
    auto f = figure(true);
    plot(x)->line_width(3);
    save("impulse_graph.jpg");
}

void app_graphics::Graphics::impulse_hist(std::vector<double> x) {
    auto f = figure(true);
    auto h1 = hist(x);
    save("impulse_hist.jpg");
}

std::vector<double> app_graphics::Graphics::read_vector(std::vector<double> surface, std::string file) {
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

map<int, double> app_graphics::Graphics::zero_map() {
    std::map <int, double> mp;
    int l = 0;
    while (l < 100000) {
        mp[l] = 0;
        l++;
    }
    return mp;
}

double app_graphics::Graphics::area_and_line(vector<double> x, vector<double> y) {
    std::map <int, double> map1 = zero_map();
    std::map <int, double> map2 = zero_map();
    std::map <int, double> map12 = zero_map();

    auto f = figure(true);
    auto h1 = hist(x); // указанная координата //
    hold(on);
    auto h2 = hist(y); // эталон //

    h1->normalization(histogram::normalization::probability);
    h1->bin_width(2);

    h2->normalization(histogram::normalization::probability);
    h2->bin_width(1);

    //удалить
    //save("ngb.jpg");

    auto v_it = h1->values().begin();
    auto e_it = h1->bin_edges().begin();

    x11.push_back(*e_it);
    y11.push_back(0);
    while (v_it != h1->values().end()) {
        map1[*e_it] = *v_it;
        x11.push_back(*e_it + h1->bin_width() / 2);
        y11.push_back(*v_it);
        ++v_it;
        ++e_it;
    }
    x11.push_back(*h1->bin_edges().end());
    y11.push_back(0);


    auto v_itt = h2->values().begin();
    auto e_itt = h2->bin_edges().begin();
    x21.push_back(*h2->bin_edges().begin());
    y21.push_back(0);
    while (v_itt != h2->values().end()) {

        map2[*e_itt] = *v_itt;
        x21.push_back(*e_itt + h2->bin_width() / 2);
        y21.push_back(*v_itt);
        ++v_itt;
        ++e_itt;
    }
    x21.push_back(*h2->bin_edges().end());
    y21.push_back(0);

    int p = 0;
    while (p < 50000) {
        map12[p] = matplot::min(map1[p], map2[p]);
        p++;
    }

    double intersection_area = std::accumulate(std::begin(map12), std::end(map12), 0.0, [](double value, const std::map<int, double>::value_type& p)
                                               { return value + p.second; }
    ); // площадь пересечения эталонов и заданного пути //

    return intersection_area;
}

std::string app_graphics::Graphics::impulse_hist_and_stand(vector<double> x) {
    //water = read_vector(water, "water.txt");
    //forest = read_vector(forest, "forest.txt");
    //field = read_vector(field, "grove.txt"); //переименовать на field
    //plowed_field = read_vector(plowed_field, "field.txt"); //переименовать на plowed_field

    double area_water = area_and_line(x, water);
    vector<double> waterx = x21; x21.clear();
    vector<double> watery = y21; y21.clear();

    double area_forest = area_and_line(x, forest);
    vector<double> forestx = x21; x21.clear();
    vector<double> foresty = y21; y21.clear();

    double area_field = area_and_line(x, field);
    vector<double> fieldx = x21; x21.clear();
    vector<double> fieldy = y21; y21.clear();

    double area_plowed_field = area_and_line(x, plowed_field);
    vector<double> plowed_fieldx = x21; x21.clear();
    vector<double> plowed_fieldy = y21; y21.clear();

    auto f = figure(true);
    hold(on);

    auto p1 = plot(waterx, watery);
    p1->display_name("water");
    p1->line_width(2);
    auto p2 = plot(forestx, foresty);
    p2->display_name("forest");
    p2->line_width(2);
    auto p3 = plot(fieldx, fieldy);
    p3->display_name("field");
    p3->line_width(2);
    auto p4 = plot(plowed_fieldx, plowed_fieldy);
    p4->display_name("plowed field");
    p4->line_width(2);
    auto p5 = plot(x11, y11);
    p5->display_name("selected point");
    p5->line_width(2);
    ::matplot::legend({});

    save("surface_type_graph.jpg");
    // принятое решение о типе поверхности
    decision = findMax(area_water, area_forest, area_field, area_plowed_field);

    return decision;
}

std::string app_graphics::Graphics::findMax(double areaWater, double areaForest, double areaField, double areaPlowedField) {
    if (areaWater >= areaForest && areaWater >= areaField && areaWater >= areaPlowedField)
        return "water";
    else if (areaForest >= areaWater && areaForest >= areaField && areaForest >= areaPlowedField)
        return "forest";
    else
        if (areaField >= areaWater && areaField >= areaForest && areaField >= areaPlowedField)
            return "field";

        return "plowed field";
}
