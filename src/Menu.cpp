#include <iostream>
#include <functional>
#include "Game.hpp"

// create button(s)
Menu::Menu(int width, int length, const sf::Texture &texture)
{
  int		tmp_y = length / 4;

  m_menu.setTexture(texture);
  m_menu.setPosition(sf::Vector2f(0, 0));
  // Menu part
  m_butt.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
						       START, true, "START")));
  tmp_y += 75;
  m_butt.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
						       OPTION, true, "OPTION")));
  tmp_y += 75;
  m_butt.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
						       MHELP, true, "HELP")));

  tmp_y += 75;
  m_butt.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
						       QUIT, true, "QUIT")));


  // help
  help_quit = (std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 75, 400, 50,
						  BACK_TO_MENU, false, "BACK TO MAIN MENU")));

  // Option class
  Option_buttons(width, length);
}

Menu::~Menu() {}


void	Menu::Option_buttons(int x, int y)
{
  int           tmp_y = y / 3;

  // difficulty -> Normal / Difficult (protect a castle)
  op_butt.push_back(std::unique_ptr<Button>(new Button(x / 2 - 200, tmp_y + 50, 400, 50,
                                                       DIFFICULTY, false, "NORMAL")));
  tmp_y += 75;

  // sound on / off
  op_butt.push_back(std::unique_ptr<Button>(new Button(x / 2 - 200, tmp_y + 50, 400, 50,
                                                       SOUND, false, "SOUND OFF")));
  tmp_y += 75;

  // sound - / +
  op_butt.push_back(std::unique_ptr<Button>(new Button(x / 2 - 200, tmp_y + 50, 190, 50,
                                                       SOUND_DOWN, false, "SOUND -")));
  op_butt.push_back(std::unique_ptr<Button>(new Button(x / 2 + 10, tmp_y + 50, 190, 50,
                                                       SOUND_UP, false, "SOUND +")));
  tmp_y += 75;

  // back to menu
  op_butt.push_back(std::unique_ptr<Button>(new Button(x / 2 - 200, tmp_y + 50, 400, 50,
                                                       BACK_TO_MENU, false, "BACK TO MAIN MENU")));

}


template<class C>
void		Menu::drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font,
			       std::vector<std::unique_ptr<C>> &buttons)
{
  sf::Text	text;
  sf::Vector2f	pos = (buttons[idx]->getBox())->getPosition();
  sf::Vector2f  size = (buttons[idx]->getBox())->getSize();

  pos.x += size.x / 4;
  pos.y += size.y / 4;
  text.setCharacterSize(24);
  text.setFont(font);
  text.setString((buttons[idx]->getWrite()).c_str());
  text.setPosition(pos);
  m_window.draw(*(buttons[idx]->getBox()));
  // text.setOutlineColor(sf::Color(128, 128, 128));
  // text.setFillColor(sf::Color(128, 128, 128));
  text.setColor(sf::Color(128, 128, 128, 128));
  m_window.draw(text);
  // write text + change inside text -> utilité de la map
}

void	Menu::help_contents(sf::RenderWindow &m_window, sf::Font &font)
{
  sf::Vector2f	pos = (help_quit->getBox())->getPosition();
  static const std::string tmp =
    std::string("This game is a survival game with life point (in red) and power (in green).\n") +
    std::string("\t\t\t\tYou die if you have 0 life point left or 0 power left.\n") +
    std::string("You lose power when you move, when you fire an energy blast and when time passes.\n") +
    std::string("\t\t\t\t\t\tPower appears when enemy dies.\n") +
    std::string("There is 4 modes of difficulty : NORMAL DIFFICULT INSANE LUNATIC.\n") +
    std::string("Controls :\n\t- Power blast direction with W, A, S, D\n")+
    std::string("\t- Move with UP, LEFT, DOWN, RIGHT\n") +
    std::string("U DIE WHEN U'RE KILLED :)\n");
  sf::Text	text;
  // le blabla pour help sur le jeu

  pos.x -= 200;
  pos.y = 300;
  text.setCharacterSize(24);
  text.setFont(font);
  text.setPosition(pos);
  text.setString(tmp.c_str());
  // text.setOutlineColor(sf::Color(128, 128, 128));
  // text.setFillColor(sf::Color(128, 128, 128));
  text.setColor(sf::Color(128, 128, 128, 128));
  m_window.draw(text);

}

void	Menu::drawhelpbutt(sf::RenderWindow &m_window, sf::Font &font)
{
  sf::Text	text;
  sf::Vector2f	pos = (help_quit->getBox())->getPosition();
  sf::Vector2f  size = (help_quit->getBox())->getSize();

  pos.x += size.x / 4;
  pos.y += size.y / 4;
  text.setCharacterSize(24);
  text.setFont(font);
  text.setString((help_quit->getWrite()).c_str());
  text.setPosition(pos);
  m_window.draw(*(help_quit->getBox()));
  // text.setOutlineColor(sf::Color(128, 128, 128));
  // text.setFillColor(sf::Color(128, 128, 128));
  text.setColor(sf::Color(128, 128, 128, 128));
  m_window.draw(text);
  // write text + change inside text -> utilité de la map
  help_contents(m_window, font);
}

void	Menu::drawButtons(sf::RenderWindow &m_window, sf::Font &font)
{
  // disp l'image qui n'a pas été load etc
  m_window.draw(m_menu);
  // draw Menu button
  for (unsigned int idx = 0; idx < m_butt.size(); idx++)
    {
      if (m_butt[idx]->isDraw() == true)
	drawButt(m_window, idx, font, m_butt);
    }

  // draw Option button
  for (unsigned int idx = 0; idx < op_butt.size(); idx++)
    {
      if (op_butt[idx]->isDraw() == true)
	drawButt(m_window, idx, font, op_butt);
    }

  if (help_quit->isDraw() == true) // -> ecrire tout le blabla + le text du buttons
    drawhelpbutt(m_window, font);
}

void	Menu::pollEvents(const sf::Event &event)
{
  static std::vector<std::function<void(Menu *, unsigned int)>> m_butt_func =
    {
      { &Menu::start },
      { &Menu::option },
      { &Menu::help },
      { &Menu::quit },
    };
  static std::vector<std::function<void(Menu *, unsigned int)>> op_butt_func =
    {
      { &Menu::difficulty },
      { &Menu::sound_on_off },
      { &Menu::sound_down },
      { &Menu::sound_up },
      { &Menu::back_to_menu },
    };

  for (unsigned int idx = 0; idx < m_butt.size(); idx++)
    {
      if (m_butt[idx]->isDraw() == true && m_butt[idx]->isPressed(event) == true)
	m_butt_func[idx](this, idx);
    }
  for (unsigned int idx = 0; idx < op_butt.size(); idx++)
    {
      if (op_butt[idx]->isDraw() == true && op_butt[idx]->isPressed(event) == true)
	op_butt_func[idx](this, idx);
    }
}


void	Menu::pollhelpEvents(const sf::Event &event)
{
  static std::function<void(Menu *)> help_butt_func = &Menu::help_to_menu;

  if (help_quit->isDraw() == true && help_quit->isPressed(event) == true)
        help_butt_func(this);
}

void	Menu::start(unsigned int idx)
{
  Game	*game = Game::getInstance();

  game->setWho(GAME);
  for (idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(false);
}

void	Menu::option(unsigned int idx)
{
  Game	*game = Game::getInstance();

  for (idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(false);
  for (idx = 0; idx < op_butt.size(); idx++)
    {
      std::string tmp = op_butt[idx]->getWrite();
      op_butt[idx]->setDraw(true);
      if (tmp == "DIFFICULT" || tmp == "NORMAL" || tmp == "INSANE" || tmp == "LUNATIC")
	{
	  op_butt[idx]->setWrite((tmp == "DIFFICULT") ? "NORMAL" :
				 (tmp == "NORMAL") ? "LUNATIC" :
				 (tmp == "INSANE") ? "DIFFICULT" : "INSANE");
	  game->setLevel((tmp == "DIFFICULT") ? NORMAL :
			 (tmp == "NORMAL") ? LUNATIC :
			 (tmp == "INSANE") ? DIFFICULT : INSANE);
	}
    }
}

void	Menu::help(unsigned int idx)
{
  Game	*game = Game::getInstance();

  game->setWho(HELP);
  for (idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(false);
  help_quit->setDraw(true);
}

void	Menu::quit(unsigned int idx)
{
  Game	*game = Game::getInstance();

  game->closewin();
  (void)idx;
}

void	Menu::difficulty(unsigned int idx)
{
  Game	*game = Game::getInstance();

  (void)idx;
  if (game->getLevel() == NORMAL)
    {
      op_butt[idx]->setWrite("DIFFICULT");
      game->setLevel(DIFFICULT);
    }
  else if (game->getLevel() == DIFFICULT)
    {
      op_butt[idx]->setWrite("INSANE");
      game->setLevel(INSANE);
    }
  else if (game->getLevel() == INSANE)
    {
      op_butt[idx]->setWrite("LUNATIC");
      game->setLevel(LUNATIC);
    }
  else
    {
      op_butt[idx]->setWrite("NORMAL");
      game->setLevel(NORMAL);
    }
}

void	Menu::sound_on_off(unsigned int idx)
{
  Game	*game = Game::getInstance();

  if (op_butt[idx]->getWrite() == "SOUND OFF")
    {
      op_butt[idx]->setWrite("SOUND ON");
      game->setSound(false);
    }
  else
    {
      op_butt[idx]->setWrite("SOUND OFF");
      game->setSound(true);
    }
}

void	Menu::sound_down(unsigned int idx)
{
  Game	*game = Game::getInstance();

  game->setVolume(game->getVolume() - 10);
  (void)idx;
}

void	Menu::sound_up(unsigned int idx)
{
  Game	*game = Game::getInstance();

  game->setVolume(game->getVolume() + 10);
  (void)idx;
}

void	Menu::back_to_menu(unsigned int idx)
{
  for (idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(true);
  for (idx = 0; idx < op_butt.size(); idx++)
    op_butt[idx]->setDraw(false);
}

void	Menu::pause_to_menu()
{
  for (unsigned int idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(true);
}

void	Menu::help_to_menu()
{
  Game	*game = Game::getInstance();

  game->setWho(MENU);
  for (unsigned int idx = 0; idx < m_butt.size(); idx++)
    m_butt[idx]->setDraw(true);
  help_quit->setDraw(false);
}
