// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>

#define CARD_COUNT 52
#define CARD_SCALE (0.7)
#define CARD_WIDTH (140 * CARD_SCALE)
#define CARD_HEIGHT (190 * CARD_SCALE)
#define GRID_ROWS 3
#define GRID_COLS 3
enum CardSuit
{
	SPADE,
	CLUB,
	HEART,
	DIAMOND,
	SUIT_COUNT,
};

enum CardValue
{
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,	
	VALUE_COUNT,
};

bool IsValueHigher(const CardValue& a, const CardValue &b) 
{
	if (a == ACE && b != ACE)
	{
		return true;
	}
	return (int)a > (int)b;
}

struct CardPair
{
	CardSuit suit;
	CardValue value;
};

enum AnimationType
{
	LINEAR,
	EASE,
	BOUNCE,
};

class Animation
{
public:
	Animation(float current, float target, float rate, AnimationType type) :
		m_current(current),
		m_target(target),
		m_rate(rate),
		m_type(type),
		m_currentTime(0.0f),
		m_done(false)
	{
		m_difference = target - m_current;
		m_duration = m_difference / m_rate;
		m_starting = m_current;
	}

	void Update(float dt)
	{
		m_currentTime += dt;
		float t = abs(m_currentTime / m_duration);
		switch (m_type)
		{
		case EASE:
			t--;
			m_current = m_difference * (t * t * t * t * t + 1)+ m_starting;
			break;
		case BOUNCE:
			// NYI:
			break;
		case LINEAR:
			m_current = m_rate * dt + m_current;
			break;
		}

		if (m_current >= m_target)
		{
			m_current = m_target;
			m_done = true;
		}

	}

	float m_current;
	float m_starting;
	float m_target;
	float m_rate;
	float m_currentTime;
	float m_difference;
	float m_duration;
	bool m_done;
	AnimationType m_type;
};

class GameObject
{
public:
	GameObject(const std::string& path) :
		m_isMouseOver(false),
		m_animation(NULL),
		m_zOrder(0)
	{
		if (m_texture.loadFromFile(path))
		{
			m_sprite.setTexture(m_texture);
			m_sprite.setOrigin(CARD_WIDTH * 0.5f, CARD_HEIGHT * 0.5f);
			m_sprite.setScale(CARD_SCALE, CARD_SCALE);
		}
		else
		{
			printf("Error: loading card texture %s\n", path.c_str());
		}
	}

	virtual ~GameObject()
	{
		if (m_animation)
		{
			delete m_animation;
			m_animation = NULL;
		}
	}

	void Update(float dt)
	{
		if (m_animation && !m_animation->m_done)
		{
			m_animation->Update(dt);
			m_sprite.setScale(m_animation->m_current, m_animation->m_current);
		}
		else if (m_animation)
		{
			delete m_animation;
			m_animation = NULL;
		}
	}
	void Draw(sf::RenderWindow* window)
	{		
		window->draw(m_sprite);
	}

	void MouseOver()
	{
		if (!m_isMouseOver)
		{
			if (m_animation)
			{
				delete m_animation;
			}
			m_animation = new Animation(CARD_SCALE, CARD_SCALE + 0.2, 1, EASE);
			m_sprite.setColor(sf::Color(255, 255, 255, 255));
			m_isMouseOver = true;
		}
	}

	void MouseOff()
	{		
		if (m_isMouseOver)
		{
			m_sprite.setColor(sf::Color(255, 255, 255, 255));
			delete m_animation;			
			m_animation = new Animation(m_sprite.getScale().x, CARD_SCALE, 1, EASE);
			m_isMouseOver = false;
		}
	}
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	std::string m_path;
	int m_value; // 1 = Ace, 11 = jack, 12, = queen, 13 = king
	int m_zOrder;
	bool m_isMouseOver;
	Animation* m_animation;
};

class Card : public GameObject
{
public:
	Card(const std::string& path) : GameObject(path)
	{

	}
	CardSuit m_suit;
	CardValue m_value;
	int row;
	int col;
};

class Deck
{

public:
	Deck()
	{
		std::string cardSuitName[SUIT_COUNT] = { "Spades", "Clubs", "Hearts", "Diamonds" };
		std::string cardValueName[VALUE_COUNT] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
		std::string path;
		for (int i = 0; i < SUIT_COUNT ; i++)
		{
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				path = "Assets/PNG/Cards/card" + cardSuitName[i] + cardValueName[j] +".png";
				Card* newCard = new Card(path);
				newCard->m_suit = (CardSuit)i;
				newCard->m_value = (CardValue)j;
				m_cards[i][j] = newCard;
			}
		}
	}

	GameObject* GetCard(CardSuit suit, CardValue value)
	{
		return m_cards[suit][value];
	}

	GameObject* m_cards[SUIT_COUNT][VALUE_COUNT];	
};

class RenderMgr
{
public:
	void Draw(sf::RenderWindow* window)
	{
		std::sort(m_drawList.begin(), m_drawList.end(), [](GameObject* a, GameObject* b) {return a->m_zOrder < b->m_zOrder; });

		for (int i = 0; i < m_drawList.size(); i++)
		{
			m_drawList[i]->Draw(window);
		}
	}

	void Remove(GameObject* obj)
	{
		for (auto it = m_drawList.begin(); it != m_drawList.end();)
		{
			if (*it == obj)
			{
				m_drawList.erase(it);
				break;
			}
			else
			{
				it++;
			}
		}
	}

	std::vector<GameObject*> m_drawList;
};

class Game
{
public:
	Game() 
		: m_mouseOverRow(-1)
		, m_mouseOverCol(-1)
		, m_wantsPutCard(false)
		, m_playedClub(false)
	{
		m_renderMgr.m_drawList.reserve(SUIT_COUNT * VALUE_COUNT);
		m_renderMgr.m_drawList.clear();
		
		for (int i = 0; i < GRID_ROWS; i++)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				m_cardSlots[i][j] = NULL;
			}
		}


		for (int i = 0; i < GRID_ROWS; i++)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				__ResetCardSlot(i, j);
			}
		}

		// Insert each card/value pair
		for (int i = 0; i < SUIT_COUNT; i++)
		{
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				CardPair newPair;
				newPair.suit = (CardSuit)i;
				newPair.value = (CardValue)j;
				m_shuffledDeck.push_back(newPair);
			}
		}

		std::srand(unsigned(std::time(0)));
		std::random_shuffle(m_shuffledDeck.begin(), m_shuffledDeck.end());
	}

	void __EraseObject(GameObject* object)
	{
		if (object != NULL)
		{
			m_renderMgr.Remove(object);
			delete object;
		}
	}

	void __PlaceCard(GameObject* object, int row, int col)
	{	
		Card* card = dynamic_cast<Card*>(object);
		card->m_sprite.setPosition(sf::Vector2f(CARD_WIDTH * 0.5f + col * CARD_WIDTH, CARD_HEIGHT * 0.5f + row* CARD_HEIGHT));
		m_cardSlots[row][col] = card;
		card->row = row;
		card->col = col;
	}

	void __ResetCardSlot(int row, int col)
	{		
		GameObject* card = new GameObject("Assets/PNG/Cards/cardBack_blue1.png");
		m_cardSlots[row][col] = card;
		card->m_sprite.setPosition(sf::Vector2f(CARD_WIDTH * 0.5f + col * CARD_WIDTH, CARD_HEIGHT * 0.5f + row* CARD_HEIGHT));
		m_renderMgr.m_drawList.push_back(card);
	}

	void Update(float dt, sf::RenderWindow* window)
	{
		bool mouseOver = false;
		for (int i = 0; i < GRID_ROWS; i++)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				GameObject* card = m_cardSlots[i][j];
				card->Update(dt);
				sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
				sf::FloatRect bounds = card->m_sprite.getGlobalBounds();
				if (bounds.contains(mousePos.x, mousePos.y) && (m_mouseOverRow == -1 || m_mouseOverRow == i))
				{
					mouseOver = true;
					m_mouseOverRow = i;
					m_mouseOverCol = j;
					card->m_zOrder = 1;
					card->MouseOver();
				}
				else
				{
					card->m_zOrder = 0;
					card->MouseOff();
				}
			}			
		}

		if (!mouseOver)
		{
			m_mouseOverRow = -1;
			m_mouseOverCol = -1;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_playedClub)
		{
			if (m_shuffledDeck.size() > 1)
			{
				CardPair top = m_shuffledDeck.at(0);
				m_shuffledDeck.erase(m_shuffledDeck.begin());
				m_shuffledDeck.push_back(top);				
			}
			m_playedClub = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_mouseOverRow != -1)
		{
			m_wantsPutCard = true;
		}		
		if (m_wantsPutCard && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_mouseOverRow != -1)
		{
			m_wantsPutCard = false;

			if (m_shuffledDeck.size() > 0)
			{
				CardPair topCard = m_shuffledDeck.at(0);
				Card* newCard = dynamic_cast<Card*>(m_deck.GetCard((CardSuit)topCard.suit, (CardValue)topCard.value));
				Card* existingCard = dynamic_cast<Card*>(m_cardSlots[m_mouseOverRow][m_mouseOverCol]);
								
				bool canPlace = existingCard == NULL;
				
				if (existingCard != NULL)
				{					
					if (newCard->m_suit == CardSuit::SPADE)
					{
						canPlace = IsValueHigher(newCard->m_value, existingCard->m_value);
					}
					else if (newCard->m_suit == CardSuit::DIAMOND)
					{
						int numEmpty = 0;
						for (int i = 0; i < GRID_ROWS; i++)
						{
							Card* existingCard = dynamic_cast<Card*>(m_cardSlots[i][m_mouseOverCol]);
							if (existingCard == NULL)
							{
								numEmpty++;
							}
						}

						Card* existingCard = dynamic_cast<Card*>(m_cardSlots[m_mouseOverRow][m_mouseOverCol]);
						if (numEmpty == 0 ||
							(existingCard != NULL && m_mouseOverRow == 0))
						{
							canPlace = false;
						}
						else if (existingCard == NULL)
						{
							canPlace = true;
						}
						// Push the other cards up.
						else
						{
							for (int i = 1; i <= m_mouseOverRow; i++)
							{
								Card* existingCard = dynamic_cast<Card*>(m_cardSlots[i][m_mouseOverCol]);
								if (existingCard != NULL)
								{
									__PlaceCard(existingCard, i - 1, m_mouseOverCol);
								}
							}

							canPlace = true;
						}
					}
				}

				if (canPlace)
				{
					Card* existingCard = dynamic_cast<Card*>(m_cardSlots[m_mouseOverRow][m_mouseOverCol]);
					if (existingCard == NULL || newCard->m_suit == CardSuit::SPADE)
					{
						// Remove the blank card here if it exists
						__EraseObject(m_cardSlots[m_mouseOverRow][m_mouseOverCol]);
					}

					__PlaceCard(newCard, m_mouseOverRow, m_mouseOverCol);
					m_renderMgr.m_drawList.push_back(newCard);
					m_shuffledDeck.erase(m_shuffledDeck.begin());

					if (newCard->m_suit == CardSuit::CLUB)
					{
						m_playedClub = true;
					}
					else
					{
						m_playedClub = false;
					}
				}				
			}
		}
		// Drop first, then check for clears
		
		do
		{
			__DropCards();			
		} while (__CheckForClears());
	}

	void __DropCard(int row, int col)
	{
		int newRow = row;

		Card* card = dynamic_cast<Card*>(m_cardSlots[row][col]);
		if (card == NULL)
		{
			return;
		}
		else if (card->m_suit == CardSuit::HEART)
		{
			return;
		}

		for (int i = row+1; i < GRID_ROWS; i++)
		{
			card = dynamic_cast<Card*>(m_cardSlots[i][col]);
			if (card == NULL)
			{
				newRow = i;
			}
			else
			{
				break;
			}
		}

		if (newRow != row)
		{			
			__EraseObject(m_cardSlots[newRow][col]);
			__PlaceCard(m_cardSlots[row][col], newRow, col);
			__ResetCardSlot(row, col);
		}
	}	

	void __DropCards()
	{		
		for (int i = GRID_ROWS-1; i >= 0; i--)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				__DropCard(i, j);
			}
		}
	}

	bool __CheckForClears()
	{			
		bool result = false;
		for (int i = 0; i < GRID_ROWS; i++)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				std::vector<Card*> pending;				
				std::vector<Card*> visited;

				Card* newCard = dynamic_cast<Card*>(m_cardSlots[i][j]);
				if (newCard != NULL)
				{
					pending.push_back(newCard);
				}

				while (pending.size() > 0)
				{
					Card* current = dynamic_cast<Card*>(pending[0]);					
					
					bool exists = false;
					for (auto c : visited)
					{
						if (c == current)
						{
							exists = true;
							break;
						}
					}

					if (!exists)
					{
						visited.push_back(current);
					}
					else
					{
						pending.erase(pending.begin());
						continue;
					}

					int row = current->row;
					int col = current->col;
					// Look left
					if (col - 1 >= 0)
					{
						Card* adjacent = dynamic_cast<Card*>(m_cardSlots[row][col - 1]);
						if (adjacent != NULL && current->m_suit == adjacent->m_suit)
						{
							pending.push_back(adjacent);
						}
					}
					// right;
					if (col + 1 < GRID_COLS)
					{
						Card* adjacent = dynamic_cast<Card*>(m_cardSlots[row][col + 1]);
						if (adjacent != NULL && current->m_suit == adjacent->m_suit)
						{
							pending.push_back(adjacent);
						}
					}
					// Above
					if (row - 1 >= 0)
					{
						Card* adjacent = dynamic_cast<Card*>(m_cardSlots[row-1][col]);
						if (adjacent != NULL && current->m_suit == adjacent->m_suit)
						{
							pending.push_back(adjacent);
						}
					}
					//below
					if (row + 1 < GRID_ROWS)
					{
						Card* adjacent = dynamic_cast<Card*>(m_cardSlots[row+1][col]);
						if (adjacent != NULL && current->m_suit == adjacent->m_suit)
						{
							pending.push_back(adjacent);
						}
					}					

					pending.erase(pending.begin());
				}

				if (visited.size() >= 3)
				{
					result = true;
					for (auto v : visited)
					{
						for (int i = 0; i < GRID_ROWS; i++)
						{
							for (int j = 0; j < GRID_COLS; j++)
							{
								if (v == m_cardSlots[i][j])
								{
									__EraseObject(m_cardSlots[i][j]);
									__ResetCardSlot(i, j);
								}
							}
						}
					}					
				}
			}			
		}	
		return result;
	}

	void Draw(sf::RenderWindow* window)
	{		
		m_renderMgr.Draw(window);

		// Also draw the on-board deck
		if (m_shuffledDeck.size() > 0)
		{
			CardPair topCard = m_shuffledDeck.at(0);
			GameObject* card = m_deck.GetCard((CardSuit)topCard.suit, (CardValue)topCard.value);
			card->m_sprite.setPosition(sf::Vector2f(4 * CARD_WIDTH, CARD_HEIGHT * 0.5f));
			card->Draw(window);
		}
	}

	RenderMgr m_renderMgr;
	Deck m_deck;	
	int m_mouseOverRow;
	int m_mouseOverCol;
	bool m_playedClub;
	GameObject* m_cardSlots[GRID_ROWS][GRID_COLS];
	std::vector<CardPair> m_shuffledDeck;
	bool m_wantsPutCard;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(480, 480), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	
	Game game;
	sf::Clock clock;
	while (window.isOpen())
	{

		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		game.Update(dt.asSeconds(), &window);
		game.Draw(&window);
		window.display();
	}

	return 0;
}
