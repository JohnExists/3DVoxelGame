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

    /**
     * @brief Translates the object by the given x, y, and z values.
     *
     * @param x The amount to translate in the x direction.
     * @param y The amount to translate in the y direction.
     * @param z The amount to translate in the z direction.
     */
	void translate(int x, int y, int z);
    /**
     * @brief Scales the object by the given x, y, and z values.
     *
     * @param x The factor to scale in the x direction.
     * @param y The factor to scale in the y direction.
     * @param z The factor to scale in the z direction.
     */
	void scale(int x, int y, int z);
    /**
     * @brief Rotates the object around the X-axis by the specified angle.
     *
     * @param angle The angle in degrees to rotate around the X-axis.
     */
    void rotateX(float angle);
    /**
     * @brief Rotates the object around the Y-axis by the specified angle.
     *
     * @param angle The angle in degrees to rotate around the Y-axis.
     */
    void rotateY(float angle);
    /**
     * @brief Rotates the object around the Z-axis by the specified angle.
     *
     * @param angle The angle in degrees to rotate around the Z-axis.
     */
    void rotateZ(float angle);

    std::vector<Quad>& getQuads();
private:
    void rotate(float angle, int x, int y, int z);
    void applyMatrix(glm::mat3 matrix);
};

#endif // CUBE_H
