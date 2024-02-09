#include <cmath>
#include <iostream>
//1.3 / 1.7
using namespace std;

const double EQUATOR_LENGTH = 40075016.686;
const double METERS_IN_DEGREE_LAT = 111300;
double pixelsInDegreeLat, meterInPixelLat, latES;

const double MERIDIAN_LENGTH = 40008550;
const double METERS_IN_DEGREE_LON = 111134.861111;
double pixelsInDegreeLon, meterInPixelLon, lonES;


double findPixelsInDegreeLat(double latCenter, int zoom) {

    meterInPixelLat = EQUATOR_LENGTH*cos(latCenter*M_PI / 180)/pow(2,zoom+8);
    pixelsInDegreeLat = METERS_IN_DEGREE_LAT/meterInPixelLat;
    return pixelsInDegreeLat;
}

double findPixelsInDegreeLon(int zoom) {

    meterInPixelLon = MERIDIAN_LENGTH/pow(2,zoom+8);
    pixelsInDegreeLon = METERS_IN_DEGREE_LON/meterInPixelLon;
    return pixelsInDegreeLon;
}

string ConvertDecimalDegreesToDegreesMinutesSeconds(double decimalDegrees)
{
    int sec = (int)(decimalDegrees * 3600);
    int deg = sec / 3600;
    sec = sec % 3600;
    int min = sec / 60;
    string exactSec = to_string(decimalDegrees * 3600 - deg * 3600 -
                                min * 60);
    string DegreesMinutesSeconds = to_string(deg)+ "°" + to_string(min) +
                                   "'" + exactSec + '"';
    return DegreesMinutesSeconds;
}

int func()
{
    double height = 300;
    double width = 300;
    double latCenter = -23.662628;
    double lonCenter = 30.806279;
    double zoom = 17;
    double a = findPixelsInDegreeLat(latCenter, zoom);
    double b = findPixelsInDegreeLon(zoom);
    struct coordinates {
        string lat;
        string lon;
    } NW, NE, SE, SW;

    NW.lat = NE.lat = ConvertDecimalDegreesToDegreesMinutesSeconds
                              (abs(latCenter + height/2/a)) + ((latCenter + height/2/a) > 0? 'N': 'S');
    SE.lat = SW.lat = ConvertDecimalDegreesToDegreesMinutesSeconds
                              (abs(latCenter - height/2/a)) + ((latCenter - height/2/a) > 0? 'N': 'S');
    NW.lon = SW.lon = ConvertDecimalDegreesToDegreesMinutesSeconds
                              (abs(lonCenter - width/2/b)) + ((lonCenter - width/2/b) > 0? 'E': 'W');
    NE.lon = SE.lon = ConvertDecimalDegreesToDegreesMinutesSeconds
                              (abs(lonCenter + width/2/b)) + ((lonCenter + width/2/b) > 0? 'E': 'W');

    cout << "NW:" << NW.lat << ' ' << NW.lon <<"\n";
    cout << "NE:" << NE.lat << ' ' << NE.lon <<"\n";
    cout << "SE:" << SE.lat << ' ' << SE.lon <<"\n";
    cout << "SW:" << SW.lat << ' ' << SW.lon <<"\n";

    return 0;
}