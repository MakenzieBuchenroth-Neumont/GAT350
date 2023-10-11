#include "Model.h"
#include "Renderer.h"
#include <sstream>

namespace nc
{
	bool Model::Create(std::string filename, ...)
	{
		return Load(filename);
	}

	bool Model::Load(const std::string& filename)
	{
		std::string buffer;
		if (!nc::readFile(filename, buffer))
		{
			WARNING_LOG("Could not load model: " << filename);
			return false;
		}

		std::istringstream stream(buffer);

		// read color
		stream >> m_color;

		// read number of points
		std::string line;
		std::getline(stream, line);
		int numPoints = std::stoi(line);

		// read vector2 points
		for (int i = 0; i < numPoints; i++)
		{
			vec2 point;
			stream >> point;

			m_points.push_back(point);
		}

		return true;
	}


	void Model::Draw(Renderer& renderer, const vec2& position, float rotation, float scale)
	{
	
	}

	void Model::Draw(Renderer& renderer, const Transform& transform)
	{
	}

	float Model::GetRadius()
	{
		if (m_radius) return m_radius;

		for (auto point : m_points)
		{
			float length = point.Length();
			m_radius = Max(m_radius, length);
		}

		return m_radius;
	}
}
