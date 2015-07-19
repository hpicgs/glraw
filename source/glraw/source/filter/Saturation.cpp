#include <glraw/filter/Saturation.h>

#include <QOpenGLShaderProgram>

#include <glraw/Canvas.h>

namespace
{
	const char * const source =
		R"(#version 150

		uniform sampler2D src;
		uniform float amount;

		in vec2 v_uv;
		out vec4 dst;

		void main()
		{   
			vec4 color = texture(src,v_uv);
			vec3 colorGray = vec3(float(color.rgb*vec3(0.2126f, 0.7152f, 0.0722f)));
			
			dst = vec4(mix(colorGray, color.rgb, clamp(amount,0.f,1.f)),color.a);
		} )";

	const unsigned int DefaultAmount = 0.5;
}

namespace glraw
{

	Saturation::Saturation(float amount = DefaultAmount)
		: m_amount(amount)
	{
	}

	Saturation::Saturation(const QVariantMap& cfg)
		: Saturation(AmountFromVariant(cfg, DefaultAmount))
	{
	}

	void Saturation::setUniforms(QOpenGLShaderProgram& program, unsigned int pass)
	{
		program.setUniformValue("amount", m_amount);
	}

	QString Saturation::fragmentShaderSource(unsigned int pass)
	{
		return source;
	}

	float Saturation::AmountFromVariant(const QVariantMap& cfg, float default_value)
	{
		return cfg.value("amount", { default_value }).toFloat();
	}
}
