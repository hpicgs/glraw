#include <glraw/filter/Grayscale.h>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =  R"(
		#version 150

		uniform sampler2D src;

		in vec2 v_uv;
		layout(location = 0) out vec4 dst;

		void main()
		{   
			vec4 color = texture(src, v_uv);
			dst = vec4(vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b), color.a);
		} )";
}

namespace glraw
{

Grayscale::Grayscale(GrayscaleFactor in)
	: Grayscale(FactorFromMode(in))
{
}

Grayscale::Grayscale(const QVector3D& factor)
	: m_conversionFactor(factor)
{
}

Grayscale::Grayscale(const QVariantMap& cfg)
	: Grayscale(FactorFromVariant(cfg))
{
}

bool Grayscale::process(Canvas & imageData, AssetInformation & info)
{
	//imageData.process(source, QMap<QString, QString>());

	return imageData.process(source, QMap<QString, QString>());
}

QVector3D Grayscale::FactorFromMode(GrayscaleFactor in)
{
	switch (in)
	{
	default:
		qFatal("Invalid GrayscaleFactor enum used!");

	case GrayscaleFactor::Classic:
		return { 0.299f, 0.587f, 0.114f };
	case GrayscaleFactor::Modern:
		return { 0.2126f, 0.7152f, 0.0722f };
	}
}

QVector3D Grayscale::FactorFromVariant(const QVariantMap& cfg)
{
	auto it = cfg.find("mode");
	if (it == cfg.end())
	{
		auto mode = static_cast<GrayscaleFactor>(it->toInt());
		return FactorFromMode(mode);
	}
	else
	{
		const auto def = FactorFromMode(GrayscaleFactor::Default);
		float r = cfg.value("r", {def.x()}).toFloat();
		float g = cfg.value("g", {def.y()}).toFloat();
		float b = cfg.value("b", {def.z()}).toFloat();

		return QVector3D(r, g, b);
	}
}

}