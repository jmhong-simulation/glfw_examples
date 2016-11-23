#pragma once
#pragma once

#include "GLSquare.h"
#include "GLLineSegments.h"
#include "SelfDrivingCar.h"

class Game
{
public:
	SelfDrivingCar my_car;
	std::vector<std::unique_ptr<GLObject>> obj_list;

	void init()
	{
		my_car.init();

		// world (-1.0, -0.5) x (2.0, 1.5)
		obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.5f, 0.5f, 0.0f), 1.5f, 1.0f)))); // world
		obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(1.2f, 0.5f, 0.0f), 0.1f, 0.2f))));
		obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(-0.3f, 0.5f, 0.0f), 0.1f, 0.2f))));
		obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.3f, 1.1f, 0.0f), 0.3f, 0.05f))));
		obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.6f, -0.25f, 0.0f), 0.3f, 0.25f))));
	}

	void processInput(const int& action)
	{
		switch (action)
		{
		case 0:
			my_car.turnLeft();
			break;
		case 1:
			my_car.turnRight();
			break;
		case 2:
			my_car.accel();
			break;
		case 3:
			my_car.decel();
			break;
		default:
			std::cout << "Wrong action " << std::endl;
			exit(1);
			break;
		}
	}

	float update()
	{
		float reward = 0.0f;

		my_car.update();
		my_car.updateSensor(obj_list);

		// TODO: compute reward
		// car-object collision
		if (my_car.car_body.checkCollisionLoop(obj_list) == true)
		{
			static int count = 0;

			std::cout << "Collision " << count++ << std::endl;
			my_car.init();
			my_car.car_body.model_matrix_ = glm::mat4(1.0f);
		}

		// car-world escape check
		//for (auto itr : my_car.car_body.vertices)
		//{
		//	const glm::vec4 v4 = my_car.car_body.model_matrix_ * glm::vec4(itr.x, itr.y, itr.z, 1.0f);

		//	if (world_bb.isInside(v4.x, v4.y) == false) std::cout << "World outside " << v4.x << " " << v4.y << std::endl;
		//}

		return reward;
	}
};