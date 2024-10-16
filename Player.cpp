#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define PLAYER_ACCEL 0.00925f


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, 
	MOVE_LEFT, MOVE_RIGHT, 
	JUMP_LEFT, JUMP_RIGHT,
	FALL_LEFT, FALL_RIGHT,
	DOWN_LEFT, DOWN_RIGHT,
	SUPER_FALL_LEFT, SUPER_FALL_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	float ratioX = 1.0f / 30.0f;
	spritesheet.loadFromFile("images/mf.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 48), glm::vec2(ratioX, 0.05f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
		sprite->setAnimationSpeed(STAND_LEFT, 2);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(1.0f-ratioX, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(1-2*ratioX, 0.f)); //columna, fila
		
		sprite->setAnimationSpeed(STAND_RIGHT, 2);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(ratioX, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 10);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*2, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*3, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*4, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*5, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*6, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*7, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*8, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1-ratioX*9, 0.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 10);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*2, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*3, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*4, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*5, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*6, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*7, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*8, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(ratioX*9, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 1);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(ratioX*2, 0.05f));

		sprite->setAnimationSpeed(JUMP_LEFT, 1);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1-ratioX*3, 0.05f));

		sprite->setAnimationSpeed(FALL_RIGHT, 1);
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(ratioX*3, 0.05f));

		sprite->setAnimationSpeed(FALL_LEFT, 1);
		sprite->addKeyframe(FALL_LEFT, glm::vec2(1-ratioX*4, 0.05f));

		sprite->setAnimationSpeed(DOWN_RIGHT, 2);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.f, 0.05f));
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(ratioX, 0.05f));

		sprite->setAnimationSpeed(DOWN_LEFT, 2);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(1-ratioX, 0.05f));
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(1-ratioX*2, 0.05f));

		sprite->setAnimationSpeed(SUPER_FALL_RIGHT, 1);
		sprite->addKeyframe(SUPER_FALL_RIGHT, glm::vec2(ratioX*6, 0.05f));

		sprite->setAnimationSpeed(SUPER_FALL_LEFT, 1);
		sprite->addKeyframe(SUPER_FALL_LEFT, glm::vec2(1-ratioX*7, 0.05f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getKey(GLFW_KEY_A))
	{
		if(sprite->animation() != MOVE_LEFT && sprite->animation() != JUMP_LEFT && sprite->animation() != FALL_LEFT){\
			sprite->changeAnimation(MOVE_LEFT);
			velocity = 0;
		}
		if (velocity > -0.125f)
			velocity -= PLAYER_ACCEL;
		posPlayer.x += velocity*deltaTime;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= velocity*deltaTime;
			velocity = 0;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_D))
	{
		if(sprite->animation() != MOVE_RIGHT && sprite->animation() != JUMP_RIGHT && sprite->animation() != FALL_RIGHT){
			sprite->changeAnimation(MOVE_RIGHT);
			velocity = 0;
		}
		if (velocity < 0.125f)
			velocity += PLAYER_ACCEL;
		posPlayer.x += velocity*deltaTime;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= velocity*deltaTime;
			velocity = 0;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_S)){
		if(sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
			sprite->changeAnimation(DOWN_LEFT);
		else if(sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
			sprite->changeAnimation(DOWN_RIGHT);
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT){
			sprite->changeAnimation(STAND_LEFT);
		}
		else if(sprite->animation() == MOVE_RIGHT){
			sprite->changeAnimation(STAND_RIGHT);
		}
		velocity = 0;
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
				if(sprite->animation() == JUMP_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == FALL_LEFT || sprite->animation() == SUPER_FALL_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT || sprite->animation() == SUPER_FALL_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 0 && jumpAngle < 90){
				// JUMPING -> air
				if(Game::instance().getKey(GLFW_KEY_A) || sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT){
					sprite->changeAnimation(JUMP_LEFT);
				}
				else if (Game::instance().getKey(GLFW_KEY_D) || sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT){
					sprite->changeAnimation(JUMP_RIGHT);
				}
				// printf("llego?\n");
				if (Game::instance().getKey(GLFW_KEY_S)){
					sprite->changeAnimation(SUPER_FALL_RIGHT);
				}
			}
			if(jumpAngle > 90){
				// set animation fall
				if (Game::instance().getKey(GLFW_KEY_S)){
					sprite->changeAnimation(SUPER_FALL_RIGHT);
				}
				if(sprite->animation() == JUMP_LEFT || sprite->animation() == MOVE_LEFT){
					sprite->changeAnimation(FALL_LEFT);
				}
				else if(sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT)
					sprite->changeAnimation(FALL_RIGHT);

				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
				if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)){
					if(sprite->animation() == FALL_LEFT || sprite->animation() == SUPER_FALL_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if(sprite->animation() == FALL_RIGHT || sprite->animation() == SUPER_FALL_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getKey(GLFW_KEY_W))
			{
				if(sprite->animation() == DOWN_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if(sprite->animation() == DOWN_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				else{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
			else if(!Game::instance().getKey(GLFW_KEY_A) && !Game::instance().getKey(GLFW_KEY_D)){
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == FALL_LEFT){
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT || sprite->animation() == SUPER_FALL_RIGHT || sprite->animation() == JUMP_RIGHT){
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
		else{
			if(sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT){
				sprite->changeAnimation(FALL_LEFT);
			}
			else if(sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT){
				sprite->changeAnimation(FALL_RIGHT);
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




