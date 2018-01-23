//
//  main.cpp
//  black_body
//
//  Created by Lonteron Bassergoy on 22/01/2018.
//  Copyright © 2018 Lonteron Bassergoy. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "constants.h"
#include <math.h>

using namespace std;

typedef tuple<int, int, int> rgb_t;
typedef tuple<double, double, double> xyz_t;
typedef pair<double, double> xy_t;
typedef map<int, double> func;

//IO for rgb_t (aka tupples)
ostream &operator<<(ostream&, rgb_t);

void read_data();

//conversions
rgb_t temp_to_rgb(int);
rgb_t xy_to_rgb(xy_t);
xy_t XYZ_to_xy(xyz_t);
xyz_t temperature_to_XYZ (int);

//numberical integration
double XYZ(int, func, int bot = 380, int top = 780, int h = 5);
//calculate spectral radiance for given wavelength (in nm) and temperature (W * sr−1 * m−3)
double radiance(int wave, double T);

vector<vector<rgb_t>> pixels(800, vector<rgb_t>(800));
func cmf_red, cmf_green, cmf_blue;

int main() {
    read_data();
    
    std::ofstream out("temp_to_rgb.txt");
    for (int t = 3000; t < 25000; t += 100)
        out << t << " " << temp_to_rgb(t) << std::endl;
    out.close();
    
    return 0;
}

void read_data() {
    ifstream colours("204.txt");
    int l;
    while (colours >> l >> cmf_red[l] >> cmf_green[l] >> cmf_blue[l]);
    colours.close();
    
    ifstream pix("pix.txt");
    int i, j, r, g, b;
    while (pix >> i >> j >> r >> g >> b)
        pixels[i][j] = make_tuple(r, g, b);
    pix.close();
}

rgb_t temp_to_rgb(int T) {
    return xy_to_rgb(XYZ_to_xy(temperature_to_XYZ(T)));
}

rgb_t xy_to_rgb(xy_t xy) {
    
    int xx = xy.first * 1000;
    int yy = xy.second * 1000;
    
    return pixels[xx][yy];
}

xy_t XYZ_to_xy(xyz_t XYZ) {
    double X, Y, Z, xx, yy;
    tie(X, Y, Z) = XYZ;
    
    xx = X / (X + Y + Z);
    yy = Y / (X + Y + Z);
    
    return make_pair(xx, yy);
}

xyz_t temperature_to_XYZ(int T) {
    
    double X = XYZ(T, cmf_red);
    double Y = XYZ(T, cmf_green);
    double Z = XYZ(T, cmf_blue);
    
    return make_tuple(X, Y, Z);
}

double XYZ(int T, func cmf, int bot, int top, int h){
    auto f = [T, &cmf] (int x) -> double { return radiance(x, T) * cmf[x]; };
    
    double area = 0;
    for (int i = bot + h; i <= top - h; i += h)
        area += 2 * f(i);
    area += f(bot) + f(top);
    
    return area * h / 2;
}

double radiance(int w, double T) {
    double wave = w * pow(10, -9);
    double exponent = exp(plank * c / (wave * boltzmann * T));
    return 2 * plank * c * c / (pow(wave, 5) * (exponent - 1));
}

ostream &operator<<(ostream& os, rgb_t t) {
    os << get<0>(t) << " " << get<1>(t) << " " << get<2>(t);
    return os;
}
