#ifndef CUBE_H
#define CUBE_H

#include<vector>

#include"GLObject.h"

class Quad;

class Cube
{
private:
	std::vector<Quad> quads;
public:
	Cube();
	void addQuad(Quad quad);

	void translate(int x, int y, int z);
	void scale(int x, int y, int z);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    std::vector<Quad>& getQuads();
private:
    void rotate(float angle, int x, int y, int z);
    void applyMatrix(glm::mat3 matrix);
};

#endif // CUBE_H
