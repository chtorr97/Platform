#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <SFML\Graphics.hpp>
#include <boost\variant.hpp>
#include <bitset>
#include <vector>

//#include "Entity.h"

using namespace std;
using namespace sf;

class Entity;

namespace comp
{
	enum comps
	{
		NONE = -1,
		IS_ACTIVE,
		TRANSFORM,
		PHYSIC,
		GRAPHIC,
		ANIMATION,
		COLLIDER,
		PLAYER,
		NUMBER_OF_COMPONENTS
	};

	class component
	{
	public:
		virtual void onCreate() {}
	};

	class transform : public component
	{
	public:
		static const comps TYPE = TRANSFORM;
		transform(Vector2f _position = { 0, 0 }, float _heading = 0, Vector2f _origin = { 0, 0 })
			: position(_position), prevPosition(_position), heading(_heading), origin(_origin) {}

		Vector2f prevPosition;
		Vector2f position;
		float heading;
		Vector2f origin;

		//bool hasChanged;

		void move(Vector2f transition) { prevPosition = position; position += transition; }
		void move(float x, float y) { move({ x, y }); }
		void rotate(float rotation) { prevPosition = position; heading += rotation; }

		Vector2f getDeltaPos() { return position - prevPosition; }
	};

	class physic : public component
	{
	public:
		static const comps TYPE = PHYSIC;
		physic(float _maxSpeed = 1, float _mass = 1, bool _isKinematic = false)
			: acceleration({0, 0}), maxSpeed(_maxSpeed), mass(_mass), isKinematic(_isKinematic) {}

		Vector2f acceleration;
		Vector2f velocity;
		float maxSpeed;
		float mass;
		bool isKinematic;

		void applyForce(Vector2f _force) { acceleration += _force / mass; }
		void applyForce(float x, float y) { applyForce({ x, y }); }
		void update() { velocity += acceleration; }
		void reset() { velocity -= velocity * 0.2f; acceleration *= 0.0f; }
	};

	class graphic : public component
	{
	public:
		static const comps TYPE = GRAPHIC;
		graphic(Texture* _texture = nullptr, IntRect _texLocation = { 0,0,1,1 }, Sprite _sprite = Sprite())
			: texture(_texture), texLocation(_texLocation), sprite(_sprite) {}

		Texture* texture;
		IntRect texLocation;
		Sprite sprite;

		void init() {
			sprite.setTexture(*texture);
			sprite.setOrigin(texLocation.width / 2, texLocation.height / 2); //set sprite origin based on texture rectangle
		}
	};

	class animation : public component
	{
	public:
		static const comps TYPE = ANIMATION;
		animation()
			: currentAnimation(0), currentFrame(0), state(IDLE), currentDuration(0) {}

		void update(float dt)
		{
			if (animationSets.size() == 0 || animationSets.at(currentAnimation).size() == 0)
				return;
			if (state != IDLE)
			{
				currentDuration += dt;

				if (currentDuration >= animationSets.at(currentAnimation).at(currentFrame))
				{
					if (state == ITERATE_ONCE && (currentFrame + 1 == animationSets.at(currentAnimation).size()))
					{
						currentAnimation = loopedAnimation;
						currentFrame = 0;
						currentDuration = 0;
					}
					else
					{
						currentDuration -= animationSets.at(currentAnimation).at(currentFrame);
						currentFrame = (currentFrame + 1) % animationSets.at(currentAnimation).size();
					}
				}
			}
		}

		void loop(int animationIndex)
		{
			if (animationIndex < 0 || animationIndex >= animationSets.size())
				return;
			loopedAnimation = animationIndex;
			currentFrame = 0;
			state = LOOPING;

		}

		void iterateOnce(int animationIndex)
		{
			if (animationIndex < 0 || animationIndex >= animationSets.size())
				return;
			currentAnimation = animationIndex;
			currentFrame = 0;
			state = ITERATE_ONCE;

		}

		void setIdle()
		{
			state = IDLE;
		}

		int addAnimations(int numberOfAnimation = 1)
		{
			for (int k = 0; k < numberOfAnimation; k++)
			{
				animationSets.push_back(frames());
			}

			return animationSets.size() - 1;
		}

		void addFrame(float duration, int animationIndex = -1)
		{
			if (animationIndex < 0 || animationIndex > animationSets.size())
			{
				animationSets.at(currentAnimation).push_back(duration);
			}
			else
			{
				animationSets.at(animationIndex).push_back(duration);
			}
		}

		int getFrame()
		{
			return currentFrame;
		}

		int getAnimation()
		{
			return currentAnimation;
		}

	private:
		int currentAnimation;
		int loopedAnimation;
		int currentFrame;
		float currentDuration;

		using frames = vector<float>;
		vector<frames> animationSets;
		/*
			animationSet	frames ->
			|
			v
		*/

		enum animationState
		{
			LOOPING,
			ITERATE_ONCE,
			IDLE
		};

		animationState state;
	};

	class collider : public component
	{
	public:
		static const comps TYPE = COLLIDER;

		void setSize(Vector2f size)
		{
			body.width = size.x;
			body.height = size.y;
		}
		void setSize(float x, float y) { setSize({ x, y }); }

		Vector2f getSize() { return Vector2f(body.width, body.height); }
		Vector2f getHalfSize() { return Vector2f(body.width / 2, body.height / 2); }

		//center position
		void setPosition(Vector2f pos)
		{
			body.left = pos.x;
			body.top = pos.y;
		}
		void setPosition(float x, float y) { setPosition({ x, y }); }

		Vector2f getPosition() { return Vector2f(body.left, body.top); }

		enum class Shape
		{
			RECTANGLE,
			CIRCLE
		};

		Shape shape;

	private:
		FloatRect body;
	};

	class player : public component
	{
	public:
		static const comps TYPE = PLAYER;
		player(int _points = 0)
			: canJump(true), touchGround(false), isAlive(true), points(_points) {}

		bool canJump;
		bool isAlive;
		bool touchGround;
		int points;
	};

	using compVar = boost::variant<
		transform,
		physic,
		graphic,
		animation,
		collider,
		player
	>;

	static std::bitset<comps::NUMBER_OF_COMPONENTS> maskOf(comps type)
	{
		return std::bitset<comps::NUMBER_OF_COMPONENTS>(1 << type);
	}
}

#endif