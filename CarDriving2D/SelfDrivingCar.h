#pragma once

class SelfDrivingCar
{
public:
	GLSquare car_body;
	GLLineSegments sensing_lines;
	
	glm::vec3 dir_, vel_;

	float turn_coeff_ = 1.0;
	float accel_coeff_ = 0.0001;
	float fric = 0.01;

	SelfDrivingCar()
	{
		init();

		dir_ = glm::vec3(1.0f, 0.0f, 0.0f);
		vel_ = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void init()
	{
		car_body.update(glm::vec3(0.5f, 0.5f, 0.0f), 0.2f, 0.1f);
	}

	void turnLeft()
	{
		const glm::mat4 rot_mat = glm::rotate(glm::mat4(), glm::radians(turn_coeff_), glm::vec3(0, 0, 1));

		glm::vec4 temp(dir_.x, dir_.y, dir_.z, 0.0f);

		temp = rot_mat * temp;

		dir_.x = temp.x;
		dir_.y = temp.y;

		car_body.rotateCenteredZAxis(turn_coeff_);
	}

	void turnRight()
	{
		const glm::mat4 rot_mat = glm::rotate(glm::mat4(), glm::radians(-turn_coeff_), glm::vec3(0, 0, 1));

		glm::vec4 temp(dir_.x, dir_.y, dir_.z, 0.0f);

		temp = rot_mat * temp;

		dir_.x = temp.x;
		dir_.y = temp.y;

		car_body.rotateCenteredZAxis(-turn_coeff_);
	}

	void accel()
	{
		vel_ += accel_coeff_ * dir_;
	}

	void decel()
	{
		vel_ -= accel_coeff_ * dir_;
	}

	void update()
	{

		vel_ *= (1.0f - fric);

		car_body.model_matrix_ = glm::translate(vel_) * car_body.model_matrix_;

		car_body.center_ += vel_; //TODO: update model_matrix AND center?
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