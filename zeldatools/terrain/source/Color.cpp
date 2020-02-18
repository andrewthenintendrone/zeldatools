#include "Color.h"
#include <fstream>
#include <iostream>

#pragma region constructors

Color::Color()
{
	value = 0xFFFFFF;
}

Color::Color(const Color& other)
{
	this->value = other.value;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

Color::Color(uint32_t hexCode)
{
	// have to fix the hex code
	uint8_t* chunks = (uint8_t*)&hexCode;

	r = chunks[3];
	g = chunks[2];
	b = chunks[1];
	a = chunks[0];
}

Color::Color(glm::vec3 vec3)
{
	r = (uint8_t)(vec3.r * 255.0f);
	g = (uint8_t)(vec3.g * 255.0f);
	b = (uint8_t)(vec3.b * 255.0f);
	a = (uint8_t)255;
}

Color::Color(glm::vec4 vec4)
{
	r = (uint8_t)(vec4.r * 255.0f);
	g = (uint8_t)(vec4.g * 255.0f);
	b = (uint8_t)(vec4.b * 255.0f);
	a = (uint8_t)(vec4.a * 255.0f);
}

#pragma endregion

glm::vec3 Color::asVec3() const
{
	return glm::vec3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}

glm::vec4 Color::asVec4() const
{
	return glm::vec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

#pragma region operator overloads

void Color::operator = (const Color& other)
{
	value = other.value;
}

Color Color::operator + (const Color& other) const
{
	uint8_t r2 = std::min(((uint16_t)r + (uint16_t)other.r), 255);
	uint8_t g2 = std::min(((uint16_t)g + (uint16_t)other.g), 255);
	uint8_t b2 = std::min(((uint16_t)b + (uint16_t)other.b), 255);

	return Color(r2, g2, b2);
}

void Color::operator += (const Color& other)
{
	value += other.value;
}

Color Color::operator - (const Color& other) const
{
	uint8_t r2 = std::max(((int16_t)r - (int16_t)other.r), 0);
	uint8_t g2 = std::max(((int16_t)g - (int16_t)other.g), 0);
	uint8_t b2 = std::max(((int16_t)b - (int16_t)other.b), 0);

	return Color(r2, g2, b2);
}

void Color::operator -= (const Color& other)
{
	value -= other.value;
}

Color Color::operator * (const Color& other) const
{
	glm::vec4 multiplied = this->asVec4() * other.asVec4();

	return Color(multiplied);
}

void Color::operator *= (const Color& other)
{
	*this = *this * other;
}

std::ostream& operator << (std::ostream& os, const Color& c)
{
	os << "Color(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ")";
	return os;
}

#pragma endregion

float Color::getHue() const
{
	float R = (float)r / 255.0f;
	float G = (float)g / 255.0f;
	float B = (float)b / 255.0f;

	float M = std::max<float>(R, G);
	M = std::max<float>(M, B);
	float m = std::min<float>(R, G);
	m = std::min<float>(m, B);

	float C = M - m;

	float H;

	if (C == 0)
	{
		H = 0;
	}
	else if (M == R)
	{
		H = 60.0f * ((G - B) / C);
	}
	else if (M == G)
	{
		H = 60.0f * (2.0f + (B - R) / C);
	}
	else if (M == B)
	{
		H = 60.0f * (4.0f + (R - G) / C);
	}

	if (H < 0.0f)
	{
		H += 360.0f;
	}

	return H;
}

float Color::getSaturation() const
{
	float R = (float)r / 255.0f;
	float G = (float)g / 255.0f;
	float B = (float)b / 255.0f;

	float M = std::max<float>(R, G);
	M = std::max<float>(M, B);
	float m = std::min<float>(R, G);
	m = std::min<float>(m, B);

	float C = M - m;

	if (C == 0.0f)
	{
		return 0.0f;
	}

	return C / M;
}

float Color::getValue() const
{
	float R = (float)r / 255.0f;
	float G = (float)g / 255.0f;
	float B = (float)b / 255.0f;

	float V = std::max<float>(R, G);
	V = std::max<float>(V, B);

	return V;
}

#pragma region comparisons

bool Color::compareHue(const Color& a, const Color& b)
{
	return a.getHue() < b.getHue();
}

bool Color::compareSaturation(const Color& a, const Color& b)
{
	return a.getSaturation() < b.getSaturation();
}

bool Color::compareValue(const Color& a, const Color& b)
{
	return a.getValue() < b.getValue();
}

#pragma endregion

Color Color::lerp(const Color& a, const Color& b, float t)
{
	glm::vec4 va = a.asVec4();
	glm::vec4 vb = b.asVec4();

	glm::vec4 result;

	result.x = (1.0f - t) * va.x + t * vb.x;
	result.y = (1.0f - t) * va.y + t * vb.y;
	result.z = (1.0f - t) * va.z + t * vb.z;
	result.w = (1.0f - t) * va.w + t * vb.w;

	return Color(result);
}

#pragma region conversions

Color Color::convertFromBGR555(uint16_t bgr)
{
	Color c;

	c.r = (bgr & (uint16_t)0x001f) << 3;
	c.g = ((bgr & (uint16_t)0x03e0) >> 5) << 3;
	c.b = ((bgr & (uint16_t)0x7c00) >> 10) << 3;
	c.a = 255;

	return c;
}

uint16_t Color::convertToBGR555(Color color)
{
	uint16_t bgr = (color.r / (uint16_t)0x08);
	bgr |= (color.g / (uint16_t)0x08) << 5;
	bgr |= (color.b / (uint16_t)0x08) << 10;
	bgr |= (color.a / 255) << 15;

	return bgr;
}

Color Color::convertFromRGB565(uint16_t rgb)
{
	Color c;

	c.r = ((rgb & (uint16_t)0xf800) >> 11) << 3;
	c.g = ((rgb & (uint16_t)0x07e0) >> 6) << 2;
	c.b = (rgb & (uint16_t)0x001f) << 3;
	c.a = 255;

	return c;
}

uint16_t Color::convertToRGB565(Color color)
{
	uint16_t rgb = (color.b >> 3);
	rgb |= (color.g >> 2) << 6;
	rgb |= (color.r >> 3) << 11;

	return rgb;
}

Color Color::convertFromBGR222(uint8_t bgr)
{
	Color c;

	c.r = (bgr & (uint16_t)0x03) * (uint16_t)0x40;
	c.g = ((bgr & (uint16_t)0x0C) >> 2) * (uint16_t)0x40;
	c.b = ((bgr & (uint16_t)0x30) >> 4) * (uint16_t)0x40;
	c.a = 255;

	return c;
}

uint8_t Color::convertToBGR222(Color color)
{
	int8_t byte = (color.r / (uint16_t)64);
	byte |= (color.g / (uint16_t)64) << 2;
	byte |= (color.b / (uint16_t)64) << 4;

	return byte;
}

#pragma endregion

#pragma region ColorConstants

Color Color::AliceBlue()
{
	return Color(0xF0F8FFFF);
}

Color Color::AntiqueWhite()
{
	return Color(0xFAEBD7FF);
}

Color Color::Aqua()
{
	return Color(0x00FFFFFF);
}

Color Color::Aquamarine()
{
	return Color(0x7FFFD4FF);
}

Color Color::Azure()
{
	return Color(0xF0FFFFFF);
}

Color Color::Beige()
{
	return Color(0xF5F5DCFF);
}

Color Color::Bisque()
{
	return Color(0xFFE4C4FF);
}

Color Color::Black()
{
	return Color(0x000000FF);
}

Color Color::BlanchedAlmond()
{
	return Color(0xFFEBCDFF);
}

Color Color::Blue()
{
	return Color(0x0000FFFF);
}

Color Color::BlueViolet()
{
	return Color(0x8A2BE2FF);
}

Color Color::Brown()
{
	return Color(0xA52A2AFF);
}

Color Color::BurlyWood()
{
	return Color(0xDEB887FF);
}

Color Color::CadetBlue()
{
	return Color(0x5F9EA0FF);
}

Color Color::Chartreuse()
{
	return Color(0x7FFF00FF);
}

Color Color::Chocolate()
{
	return Color(0xD2691EFF);
}

Color Color::Coral()
{
	return Color(0xFF7F50FF);
}

Color Color::CornflowerBlue()
{
	return Color(0x6495EDFF);
}

Color Color::Cornsilk()
{
	return Color(0xFFF8DCFF);
}

Color Color::Crimson()
{
	return Color(0xDC143CFF);
}

Color Color::Cyan()
{
	return Color(0x00FFFFFF);
}

Color Color::DarkBlue()
{
	return Color(0x00008BFF);
}

Color Color::DarkCyan()
{
	return Color(0x008B8BFF);
}

Color Color::DarkGoldenRod()
{
	return Color(0xB8860BFF);
}

Color Color::DarkGray()
{
	return Color(0xA9A9A9FF);
}

Color Color::DarkGrey()
{
	return Color(0xA9A9A9FF);
}

Color Color::DarkGreen()
{
	return Color(0x006400FF);
}

Color Color::DarkKhaki()
{
	return Color(0xBDB76BFF);
}

Color Color::DarkMagenta()
{
	return Color(0x8B008BFF);
}

Color Color::DarkOliveGreen()
{
	return Color(0x556B2FFF);
}

Color Color::DarkOrange()
{
	return Color(0xFF8C00FF);
}

Color Color::DarkOrchid()
{
	return Color(0x9932CCFF);
}

Color Color::DarkRed()
{
	return Color(0x8B0000FF);
}

Color Color::DarkSalmon()
{
	return Color(0xE9967AFF);
}

Color Color::DarkSeaGreen()
{
	return Color(0x8FBC8FFF);
}

Color Color::DarkSlateBlue()
{
	return Color(0x483D8BFF);
}

Color Color::DarkSlateGray()
{
	return Color(0x2F4F4FFF);
}

Color Color::DarkSlateGrey()
{
	return Color(0x2F4F4FFF);
}

Color Color::DarkTurquoise()
{
	return Color(0x00CED1FF);
}

Color Color::DarkViolet()
{
	return Color(0x9400D3FF);
}

Color Color::DeepPink()
{
	return Color(0xFF1493FF);
}

Color Color::DeepSkyBlue()
{
	return Color(0x00BFFFFF);
}

Color Color::DimGray()
{
	return Color(0x696969FF);
}

Color Color::DimGrey()
{
	return Color(0x696969FF);
}

Color Color::DodgerBlue()
{
	return Color(0x1E90FFFF);
}

Color Color::FireBrick()
{
	return Color(0xB22222FF);
}

Color Color::FloralWhite()
{
	return Color(0xFFFAF0FF);
}

Color Color::ForestGreen()
{
	return Color(0x228B22FF);
}

Color Color::Fuchsia()
{
	return Color(0xFF00FFFF);
}

Color Color::Gainsboro()
{
	return Color(0xDCDCDCFF);
}

Color Color::GhostWhite()
{
	return Color(0xF8F8FFFF);
}

Color Color::Gold()
{
	return Color(0xFFD700FF);
}

Color Color::GoldenRod()
{
	return Color(0xDAA520FF);
}

Color Color::Gray()
{
	return Color(0x808080FF);
}

Color Color::Grey()
{
	return Color(0x808080FF);
}

Color Color::Green()
{
	return Color(0x008000FF);
}

Color Color::GreenYellow()
{
	return Color(0xADFF2FFF);
}

Color Color::HoneyDew()
{
	return Color(0xF0FFF0FF);
}

Color Color::HotPink()
{
	return Color(0xFF69B4FF);
}

Color Color::IndianRed()
{
	return Color(0xCD5C5CFF);
}

Color Color::Indigo()
{
	return Color(0x4B0082FF);
}

Color Color::Ivory()
{
	return Color(0xFFFFF0FF);
}

Color Color::Khaki()
{
	return Color(0xF0E68CFF);
}

Color Color::Lavender()
{
	return Color(0xE6E6FAFF);
}

Color Color::LavenderBlush()
{
	return Color(0xFFF0F5FF);
}

Color Color::LawnGreen()
{
	return Color(0x7CFC00FF);
}

Color Color::LemonChiffon()
{
	return Color(0xFFFACDFF);
}

Color Color::LightBlue()
{
	return Color(0xADD8E6FF);
}

Color Color::LightCoral()
{
	return Color(0xF08080FF);
}

Color Color::LightCyan()
{
	return Color(0xE0FFFFFF);
}

Color Color::LightGoldenRodYellow()
{
	return Color(0xFAFAD2FF);
}

Color Color::LightGray()
{
	return Color(0xD3D3D3FF);
}

Color Color::LightGrey()
{
	return Color(0xD3D3D3FF);
}

Color Color::LightGreen()
{
	return Color(0x90EE90FF);
}

Color Color::LightPink()
{
	return Color(0xFFB6C1FF);
}

Color Color::LightSalmon()
{
	return Color(0xFFA07AFF);
}

Color Color::LightSeaGreen()
{
	return Color(0x20B2AAFF);
}

Color Color::LightSkyBlue()
{
	return Color(0x87CEFAFF);
}

Color Color::LightSlateGray()
{
	return Color(0x778899FF);
}

Color Color::LightSlateGrey()
{
	return Color(0x778899FF);
}

Color Color::LightSteelBlue()
{
	return Color(0xB0C4DEFF);
}

Color Color::LightYellow()
{
	return Color(0xFFFFE0FF);
}

Color Color::Lime()
{
	return Color(0x00FF00FF);
}

Color Color::LimeGreen()
{
	return Color(0x32CD32FF);
}

Color Color::Linen()
{
	return Color(0xFAF0E6FF);
}

Color Color::Magenta()
{
	return Color(0xFF00FFFF);
}

Color Color::Maroon()
{
	return Color(0x800000FF);
}

Color Color::MediumAquaMarine()
{
	return Color(0x66CDAAFF);
}

Color Color::MediumBlue()
{
	return Color(0x0000CDFF);
}

Color Color::MediumOrchid()
{
	return Color(0xBA55D3FF);
}

Color Color::MediumPurple()
{
	return Color(0x9370DBFF);
}

Color Color::MediumSeaGreen()
{
	return Color(0x3CB371FF);
}

Color Color::MediumSlateBlue()
{
	return Color(0x7B68EEFF);
}

Color Color::MediumSpringGreen()
{
	return Color(0x00FA9AFF);
}

Color Color::MediumTurquoise()
{
	return Color(0x48D1CCFF);
}

Color Color::MediumVioletRed()
{
	return Color(0xC71585FF);
}

Color Color::MidnightBlue()
{
	return Color(0x191970FF);
}

Color Color::MintCream()
{
	return Color(0xF5FFFAFF);
}

Color Color::MistyRose()
{
	return Color(0xFFE4E1FF);
}

Color Color::Moccasin()
{
	return Color(0xFFE4B5FF);
}

Color Color::NavajoWhite()
{
	return Color(0xFFDEADFF);
}

Color Color::Navy()
{
	return Color(0x000080FF);
}

Color Color::OldLace()
{
	return Color(0xFDF5E6FF);
}

Color Color::Olive()
{
	return Color(0x808000FF);
}

Color Color::OliveDrab()
{
	return Color(0x6B8E23FF);
}

Color Color::Orange()
{
	return Color(0xFFA500FF);
}

Color Color::OrangeRed()
{
	return Color(0xFF4500FF);
}

Color Color::Orchid()
{
	return Color(0xDA70D6FF);
}

Color Color::PaleGoldenRod()
{
	return Color(0xEEE8AAFF);
}

Color Color::PaleGreen()
{
	return Color(0x98FB98FF);
}

Color Color::PaleTurquoise()
{
	return Color(0xAFEEEEFF);
}

Color Color::PaleVioletRed()
{
	return Color(0xDB7093FF);
}

Color Color::PapayaWhip()
{
	return Color(0xFFEFD5FF);
}

Color Color::PeachPuff()
{
	return Color(0xFFDAB9FF);
}

Color Color::Peru()
{
	return Color(0xCD853FFF);
}

Color Color::Pink()
{
	return Color(0xFFC0CBFF);
}

Color Color::Plum()
{
	return Color(0xDDA0DDFF);
}

Color Color::PowderBlue()
{
	return Color(0xB0E0E6FF);
}

Color Color::Purple()
{
	return Color(0x800080FF);
}

Color Color::RebeccaPurple()
{
	return Color(0x663399FF);
}

Color Color::Red()
{
	return Color(0xFF0000FF);
}

Color Color::RosyBrown()
{
	return Color(0xBC8F8FFF);
}

Color Color::RoyalBlue()
{
	return Color(0x4169E1FF);
}

Color Color::SaddleBrown()
{
	return Color(0x8B4513FF);
}

Color Color::Salmon()
{
	return Color(0xFA8072FF);
}

Color Color::SandyBrown()
{
	return Color(0xF4A460FF);
}

Color Color::SeaGreen()
{
	return Color(0x2E8B57FF);
}

Color Color::SeaShell()
{
	return Color(0xFFF5EEFF);
}

Color Color::Sienna()
{
	return Color(0xA0522DFF);
}

Color Color::Silver()
{
	return Color(0xC0C0C0FF);
}

Color Color::SkyBlue()
{
	return Color(0x87CEEBFF);
}

Color Color::SlateBlue()
{
	return Color(0x6A5ACDFF);
}

Color Color::SlateGray()
{
	return Color(0x708090FF);
}

Color Color::SlateGrey()
{
	return Color(0x708090FF);
}

Color Color::Snow()
{
	return Color(0xFFFAFAFF);
}

Color Color::SpringGreen()
{
	return Color(0x00FF7FFF);
}

Color Color::SteelBlue()
{
	return Color(0x4682B4FF);
}

Color Color::Tan()
{
	return Color(0xD2B48CFF);
}

Color Color::Teal()
{
	return Color(0x008080FF);
}

Color Color::Thistle()
{
	return Color(0xD8BFD8FF);
}

Color Color::Tomato()
{
	return Color(0xFF6347FF);
}

Color Color::Turquoise()
{
	return Color(0x40E0D0FF);
}

Color Color::Violet()
{
	return Color(0xEE82EEFF);
}

Color Color::Wheat()
{
	return Color(0xF5DEB3FF);
}

Color Color::White()
{
	return Color(0xFFFFFFFF);
}

Color Color::WhiteSmoke()
{
	return Color(0xF5F5F5FF);
}

Color Color::Yellow()
{
	return Color(0xFFFF00FF);
}

Color Color::YellowGreen()
{
	return Color(0x9ACD32FF);
}

#pragma endregion
