//
// Created by Mark, Bryant and Jacob on 3/20/2019.
//
#include <sstream>
using std::stringstream;
#include <cmath>
using std::cos, std::sin;

#include "shape.h"

// Base Class
double Shape::get_height() const
{
    return _height;
}

double Shape::get_width() const
{
    return _width;
}

void Shape::set_height(double height)
{
    _height = height;
}

void Shape::set_width(double width)
{
    _width = width;
}

// Circle Class
void Circle::setRadius(double radius)
{
    set_height(radius*2);
    set_width(radius*2);
}

Circle::Circle(double radius)
{
    setRadius(radius);
}

std::stringstream Circle::generate()
{
    return std::stringstream("0 0 " + std::to_string(get_height()/2) + " 0 360 arc stroke");
}

// Rectangle Class
std::stringstream Rectangle::generate()
{
    return std::stringstream("newpath\n"
                             + std::to_string(-1*get_width()/2) + " " + std::to_string(-1*get_height()/2) + " moveto\n"
                             + std::to_string(get_width()) + " 0 rlineto\n"
                             + "0 " + std::to_string(get_height()) + " rlineto\n"
                             + std::to_string(-1*get_width()) + " 0 rlineto\n"
                             "closepath\n"
                             "stroke");
}

Rectangle::Rectangle(double width, double height)
{
    set_height(height);
    set_width(width);
}

Polygon::Polygon(int numSides, double sideLength)
{
    _numSides = numSides;
    _sideLength = sideLength;

    if(numSides % 2 == 1)
    {
        set_height( sideLength * (1.0 + cos(pi / numSides)) / (2.0 * sin(pi / numSides)) );
        set_width( (sideLength * sin(pi * (numSides - 1) / (2 * numSides))) / sin(pi / numSides) );
    }
    else if(numSides % 4 == 0)
    {
        set_height( sideLength * (cos(pi / numSides)) / (sin(pi / numSides)) );
        set_width( get_height() );
    }
    else
    {
        set_height( sideLength * (cos(pi / numSides)) / (sin(pi / numSides)) );
        set_width( sideLength / sin(pi / numSides) );
    }
}


stringstream Polygon::generate()
{
    stringstream output;

    output << "%!\n" << "newpath\n";
    output << "/length " << std::to_string(_sideLength) << " def\n";
    output << "/nSides " << std::to_string(_numSides) << " def\n";
    output << "/angle { 360 nSides div } def\n";
    output << "gsave\n";
    output << "newpath\n";
    output << "0 0 moveto\n";
    output << "0 angle 360 {\n";
    output << "length 0 lineto\n";
    output << "length 0 translate\n";
    output << "angle rotate\n";
    output << "} for\n";
    output << "closepath\n";
    output << "stroke\n";
    output << "grestore\n";
    output << "showpage\n";

    return output;
}
