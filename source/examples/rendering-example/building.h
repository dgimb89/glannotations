#pragma once

#include <glannotations/DrawingPrimitiveBase.h>

class Building : public glannotations::DrawingPrimitiveBase {
public:
	Building(unsigned int matricesBindingIndex = 0);
	virtual void draw() override;
	void setPosition(glm::vec3 llf, glm::vec3 urb);
};