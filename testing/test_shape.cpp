// test_shape.cpp
//

#include <memory>
#include <string>
using std::string;

#include "catch.hpp"
#include "../cps/shape.h"
#include "../cps/compoundshape.h"

TEST_CASE("Circle")
{
    Circle c1;
    SECTION("Getters")
    {
        REQUIRE(c1.get_height() == 0);
        REQUIRE(c1.get_width() == 0);
    }

    Circle c2(1);
    Circle c3(10000);
    Circle c4(10.9);
    SECTION("Single-Parameter Constructor")
    {
        REQUIRE(c2.get_height() == 2);
        REQUIRE(c2.get_width() == 2);

        REQUIRE(c3.get_height() == 20000);
        REQUIRE(c3.get_width() == 20000);

        REQUIRE(c4.get_height() == 21.8);
        REQUIRE(c4.get_width() == 21.8);
    }

    SECTION("PostScript Generation")
    {
        string testCircle1 = "0 0 1.000000 0 360 arc stroke\n";
        string testCircle2 = "0 0 10000.000000 0 360 arc stroke\n";
        string testCircle3 = "0 0 10.900000 0 360 arc stroke\n";
        REQUIRE(c2.generate().str() == testCircle1);
        REQUIRE(c3.generate().str() == testCircle2);
        REQUIRE(c4.generate().str() == testCircle3);
    }
}

TEST_CASE("Polygon","[polygon]")
{
    SECTION("Triangle")
    {
        Polygon t(3,100);
        REQUIRE(t.get_width() == 100);
    }
    SECTION("Draw triangle")
    {
        Polygon t(3, 100);

        REQUIRE( t.generate().str() == "%!\n" \
            "newpath\n" \
            "/length 100.000000 def\n" \
            "/nSides 3.000000 def\n" \
            "/angle { 360 nSides div } def\n" \
            "gsave\n" \
            "newpath\n" \
            "0 0 moveto\n" \
            "0 angle 360 {\n" \
            "length 0 lineto\n" \
            "length 0 translate\n" \
            "angle rotate\n" \
            "} for\n" \
            "closepath\n" \
            "stroke\n" \
            "grestore\n" \
            "showpage\n");

    }
}

TEST_CASE("Layered Shape")
{
	auto v1 = std::vector<std::unique_ptr<Shape>>();
	v1.push_back(std::make_unique<Circle>());

	auto layered1 = std::make_unique<LayeredShapes>(
		std::move(v1)
	);
	SECTION("width")
	{
		REQUIRE(layered1->get_width() == 0);
	}

	SECTION("height")
	{
		REQUIRE(layered1->get_height() == 0);
	}
}


TEST_CASE("Rectangle")
{
    Rectangle r1;
    Rectangle r2(1,5);
    Rectangle r3(10,20.1);
    Rectangle r4(10000,400000);

    SECTION("Getters")
    {
        REQUIRE(r1.get_width() == 0);
        REQUIRE(r1.get_height() == 0);
    }

    SECTION("Two Parameter Constructor")
    {
        REQUIRE(r2.get_height() == 5);
        REQUIRE(r2.get_width() == 1);

        REQUIRE(r3.get_height() == 20.1);
        REQUIRE(r3.get_width() == 10);

        REQUIRE(r4.get_height() == 400000);
        REQUIRE(r4.get_width() == 10000);
    }

    SECTION("PostScript Generation")
    {
        REQUIRE(r2.generate().str() == "newpath\n"
                                       "-0.500000 -2.500000 moveto\n"
                                       "1.000000 0 rlineto\n"
                                       "0 5.000000 rlineto\n"
                                       "-1.000000 0 rlineto\n"
                                       "closepath\n"
                                       "stroke\n");

        REQUIRE(r3.generate().str() == "newpath\n"
                                       "-5.000000 -10.050000 moveto\n"
                                       "10.000000 0 rlineto\n"
                                       "0 20.100000 rlineto\n"
                                       "-10.000000 0 rlineto\n"
                                       "closepath\n"
                                       "stroke\n");

        REQUIRE(r4.generate().str() == "newpath\n"
                                       "-5000.000000 -200000.000000 moveto\n"
                                       "10000.000000 0 rlineto\n"
                                       "0 400000.000000 rlineto\n"
                                       "-10000.000000 0 rlineto\n"
                                       "closepath\n"
                                       "stroke\n");

    }
}

TEST_CASE("Scaled Shape")
{
    Scaled sc1(std::make_unique<Circle>(20), 2, 2);

    std::vector<std::unique_ptr<Shape>> vectOfShapes;
    vectOfShapes.push_back(std::make_unique<Rectangle>(40, 20));
    vectOfShapes.push_back(std::make_unique<Polygon>(6, 20));
    vectOfShapes.push_back(std::make_unique<Circle>(40));

    Scaled sc2(std::make_unique<LayeredShapes>(std::move(vectOfShapes)), 1.5, 3);

    Scaled sc3(std::make_unique<Polygon>(8, 10), 4, 6);

    SECTION("Shape Changed Height and Width")
    {
        REQUIRE(sc1.get_width() == 80);
        REQUIRE(sc1.get_height() == 80);

//        REQUIRE(sc2.get_width() == 189);

//        REQUIRE(sc3.get_width() == 32);
    }

    SECTION("PostScript Generation")
    {
        REQUIRE(sc1.generate().str() == "gsave\n"
                                        "0 0 40.000000 0 360 arc stroke\n"
                                        "grestore\n"
        );

//        REQUIRE(sc2.generate().str() == "gsave\n"
//                                        "0 0 40.000000 0 360 arc stroke\n"
//                                        "grestore\n"
//        );
    }
}