#pragma once
#include <cstdint>
#include <algorithm>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

// color class
class Color
{
public:

	#pragma region constructors

	Color();
	Color(const Color& other);
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	Color(uint32_t hexCode);
	Color(glm::vec3 vec3);
	Color(glm::vec4 vec4);

	#pragma endregion

	// union
	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint32_t value;
	};

	glm::vec3 asVec3() const;
	glm::vec4 asVec4() const;

	#pragma region operator overloads

	void operator = (const Color& other);
	Color operator + (const Color& other) const;
	void operator += (const Color& other);
	Color operator - (const Color& other) const;
	void operator -= (const Color& other);
	Color operator * (const Color& other) const;
	void operator *= (const Color& other);
	friend std::ostream& operator << (std::ostream& os, const Color& c);

	#pragma endregion

	float getHue() const;
	float getSaturation() const;
	float getValue() const;

	#pragma region comparisons

	static bool compareHue(const Color& a, const Color& b);
	static bool compareSaturation(const Color& a, const Color& b);
	static bool compareValue(const Color& a, const Color& b);

	#pragma endregion

	static Color lerp(const Color& a, const Color& b, float t);

	#pragma region conversion

	static Color convertFromBGR555(uint16_t bgr);
	static uint16_t convertToBGR555(Color color);

	static Color convertFromRGB565(uint16_t rgb);
	static uint16_t convertToRGB565(Color color);

	static Color convertFromBGR222(uint8_t bgr);
	static uint8_t convertToBGR222(Color color);

	#pragma endregion

	#pragma region constants

	static Color AliceBlue();
	static Color AntiqueWhite();
	static Color Aqua();
	static Color Aquamarine();
	static Color Azure();
	static Color Beige();
	static Color Bisque();
	static Color Black();
	static Color BlanchedAlmond();
	static Color Blue();
	static Color BlueViolet();
	static Color Brown();
	static Color BurlyWood();
	static Color CadetBlue();
	static Color Chartreuse();
	static Color Chocolate();
	static Color Coral();
	static Color CornflowerBlue();
	static Color Cornsilk();
	static Color Crimson();
	static Color Cyan();
	static Color DarkBlue();
	static Color DarkCyan();
	static Color DarkGoldenRod();
	static Color DarkGray();
	static Color DarkGrey();
	static Color DarkGreen();
	static Color DarkKhaki();
	static Color DarkMagenta();
	static Color DarkOliveGreen();
	static Color DarkOrange();
	static Color DarkOrchid();
	static Color DarkRed();
	static Color DarkSalmon();
	static Color DarkSeaGreen();
	static Color DarkSlateBlue();
	static Color DarkSlateGray();
	static Color DarkSlateGrey();
	static Color DarkTurquoise();
	static Color DarkViolet();
	static Color DeepPink();
	static Color DeepSkyBlue();
	static Color DimGray();
	static Color DimGrey();
	static Color DodgerBlue();
	static Color FireBrick();
	static Color FloralWhite();
	static Color ForestGreen();
	static Color Fuchsia();
	static Color Gainsboro();
	static Color GhostWhite();
	static Color Gold();
	static Color GoldenRod();
	static Color Gray();
	static Color Grey();
	static Color Green();
	static Color GreenYellow();
	static Color HoneyDew();
	static Color HotPink();
	static Color IndianRed();
	static Color Indigo();
	static Color Ivory();
	static Color Khaki();
	static Color Lavender();
	static Color LavenderBlush();
	static Color LawnGreen();
	static Color LemonChiffon();
	static Color LightBlue();
	static Color LightCoral();
	static Color LightCyan();
	static Color LightGoldenRodYellow();
	static Color LightGray();
	static Color LightGrey();
	static Color LightGreen();
	static Color LightPink();
	static Color LightSalmon();
	static Color LightSeaGreen();
	static Color LightSkyBlue();
	static Color LightSlateGray();
	static Color LightSlateGrey();
	static Color LightSteelBlue();
	static Color LightYellow();
	static Color Lime();
	static Color LimeGreen();
	static Color Linen();
	static Color Magenta();
	static Color Maroon();
	static Color MediumAquaMarine();
	static Color MediumBlue();
	static Color MediumOrchid();
	static Color MediumPurple();
	static Color MediumSeaGreen();
	static Color MediumSlateBlue();
	static Color MediumSpringGreen();
	static Color MediumTurquoise();
	static Color MediumVioletRed();
	static Color MidnightBlue();
	static Color MintCream();
	static Color MistyRose();
	static Color Moccasin();
	static Color NavajoWhite();
	static Color Navy();
	static Color OldLace();
	static Color Olive();
	static Color OliveDrab();
	static Color Orange();
	static Color OrangeRed();
	static Color Orchid();
	static Color PaleGoldenRod();
	static Color PaleGreen();
	static Color PaleTurquoise();
	static Color PaleVioletRed();
	static Color PapayaWhip();
	static Color PeachPuff();
	static Color Peru();
	static Color Pink();
	static Color Plum();
	static Color PowderBlue();
	static Color Purple();
	static Color RebeccaPurple();
	static Color Red();
	static Color RosyBrown();
	static Color RoyalBlue();
	static Color SaddleBrown();
	static Color Salmon();
	static Color SandyBrown();
	static Color SeaGreen();
	static Color SeaShell();
	static Color Sienna();
	static Color Silver();
	static Color SkyBlue();
	static Color SlateBlue();
	static Color SlateGray();
	static Color SlateGrey();
	static Color Snow();
	static Color SpringGreen();
	static Color SteelBlue();
	static Color Tan();
	static Color Teal();
	static Color Thistle();
	static Color Tomato();
	static Color Turquoise();
	static Color Violet();
	static Color Wheat();
	static Color White();
	static Color WhiteSmoke();
	static Color Yellow();
	static Color YellowGreen();

	#pragma endregion
};