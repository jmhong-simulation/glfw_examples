#pragma once

class SelfDrivingCar
{
public:
	GLSquare car_body;
	GLLineSegments sensing_lines;
	
	glm::vec3 dir_;

	float turn_coeff_ = 0.2;
	float accel_coeff_ = 0.01;

	SelfDrivingCar()
	{
		init();

		dir_ = glm::vec3(1, 0, 0);
	}

	void init()
	{
		car_body.update(glm::vec3(0.5f, 0.5f, 0.0f), 0.2f, 0.1f);
	}

	void turnLeft()
	{
		car_body.rotateCenteredZAxis(turn_coeff_);
	}

	void turnRight()
	{
		car_body.rotateCenteredZAxis(-turn_coeff_);
	}

	void accel()
	{
		car_body.model_matrix_ = glm::translate(dir_ * accel_coeff_) * car_body.model_matrix_;

		car_body.center_ += dir_ * accel_coeff_; //TODO: update model_matrix AND center?
	}

	void decel()
	{
		car_body.model_matrix_ = glm::translate(dir_ * -accel_coeff_) * car_body.model_matrix_;

		car_body.center_ -= dir_ * accel_coeff_; //TODO: update model_matrix AND center?
	}

	void updateSensor(const GLSquare& my_square2)// parameter -> object list
	{
		// sensor sensing_lines
		std::vector<glm::vec3> sensor_lines;
		const glm::vec3 center = car_body.center_;
		const float radius = 1.0;
		for (int i = 0; i < 360; i += 10)
		{
			glm::vec4 end_pt = glm::vec4(radius*cos(glm::radians((float)i)), radius*-sin(glm::radians((float)i)), 0.0f, 0.0f);
			end_pt = car_body.model_matrix_ * end_pt;

			const glm::vec3 r = center + glm::vec3(end_pt.x, end_pt.y, end_pt.z);

			int flag;
			glm::vec3 col_pt;
			float t;

			my_square2.checkCollisionLoop(center, r, flag, t, col_pt);

			//TODO: clamp col_pt by r
			if (flag == 1)
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(col_pt);
			}
			else
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(r);
			}
		}

		sensing_lines.update(sensor_lines);
		//sensing_lines.center_ = car_body.center_;
	}
};