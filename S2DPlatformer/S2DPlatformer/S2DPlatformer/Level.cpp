#include "Level.h"

#include "RectangleExtensions.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
const Vector2 Level::InvalidPosition = Vector2(-1.0f, -1.0f);
const int Level::PointsPerSecond = 5;
const int Level::EntityLayer = 2;

Level::Level(int levelIndex)
{
	srand(354668);

	_score = 0;
	_timeRemaining = 120000;
	_exit = InvalidPosition;
	_reachedExit = false;
	_player = nullptr;

	LoadTiles(levelIndex);

	// Load background layer textures. For now, all levels must
	// use the same backgrounds and only use the left-most part of them.
	_layers = new vector<Texture2D*>(3);
	stringstream s;
	for (int i = 0; i < 3; ++i)
	{
		// Choose a random segment if each background layer for level variety.
		s << "Content/Backgrounds/Layer" << i << "_" << levelIndex << ".png";
		(*_layers)[i] = new Texture2D();
		(*_layers)[i]->Load(s.str().c_str(), true);
		s.clear();
		s.str(string());
	}

	// Load sounds.
	_exitReachedSound = new SoundEffect();
	_exitReachedSound->Load("Content/Sounds/ExitReached.wav");
}


Level::~Level(void)
{
	delete _player;
	delete _exitReachedSound;

	for (int i = 0; i < 3; i++)
	{
		delete (*_layers)[i];
	}

	delete _layers;

	for (vector<vector<Tile*>>::iterator it = _tiles->begin(); it != _tiles->end(); it++)
	{
		for (vector<Tile*>::iterator it2 = it->begin(); it2 != it->end(); it2++)
		{
			delete *it2;
		}
	}

	delete _tiles;

	for (vector<Gem*>::iterator it = _gems.begin(); it != _gems.end(); it++)
	{
		delete *it;
	}

	for (vector<Enemy*>::iterator it = _enemies.begin(); it != _enemies.end(); it++)
	{
		delete *it;
	}
}

Player* Level::GetPlayer()
{
	return _player;
}

int Level::GetScore()
{
	return _score;
}

bool Level::ReachedExit()
{
	return _reachedExit;
}

float Level::GetTimeRemaining()
{
	return _timeRemaining;
}


int Level::GetWidth()
{
	return _tiles->size();
}

int Level::GetHeight()
{
	return _tiles->at(0).size();
}


void Level::LoadTiles(int levelIndex)
{
	// Load the level and ensure all of the lines are the same length.
	//int width;
	//vector<string>* lines = new vector<string>();
	//fstream stream;
	//stringstream ss;
	//ss << "Content/Levels/" << levelIndex << ".txt";
	//stream.open(ss.str(), fstream::in);

	//char* line = new char[256];
	//stream.getline(line, 256);
	//string* sline = new string(line);
	//width = sline->size();
	//while (!stream.eof())
	//{
 //       lines->push_back(*sline);
 //       if (sline->size() != width)
	//		cout << "Bad Level Load\n";
 //       stream.getline(line, 256);
	//	delete sline;
	//	sline = new string(line);
	//}

	//delete [] line;
	//delete sline;

	// Allocate the tile grid.


	vector<string>* lines = new vector<string>();

#pragma region PetreRandGen
	//srand(time(NULL));
	//int x;
	//	char map[15][20];
	//	bool player = false;
	//	bool exit = false;
	//	for (int i = 0; i < 20; i++) {
	//		map[14][i] = *"#";
	//	}
	//	for (int i = 13; i >= 1; i--) {
	//		map[i][0] = *"#";
	//		for (int j = 1; j < 19; j++) {
	//			x = rand() % 40 + 1;
	//			if (x == 1 && !player && i > 8) {
	//				map[i][j] = *"1";
	//				player = true;
	//			}
	//			else if (x == 2 && !exit && player && i <= 8) {
	//				map[i][j] = *"X";
	//				map[i + 1][j] = *"-";
	//				exit = true;
	//			}
	//			else {
	//				if (i % 3 == 0 && x % 4 == 0) {
	//					map[i][j] = *"-";
	//				}
	//				else {
	//					map[i][j] = *".";
	//				}
	//			}
	//		}
	//		map[i][19] = *"#";
	//	}
	//	for (int i = 0; i < 20; i++) {
	//		map[0][i] = *"#";
	//	}
	//	if (!exit) {
	//		map[2][17] = *"X";
	//		map[3][17] = *"-";
	//	}
	//	else {
	//		map[2][17] = *".";
	//	}
	//	if (!player) {
	//		map[13][3] = *"1";
	//	}
	//for (int i = 0; i < 15; i++) {
	//	for (int j = 0; j < 20; j++) {
	//		levelStream << map[i][j];
	//	}
	//	levelStream << " ";
	//}
#pragma endregion PetreRandGen

#pragma region Rework

	char map[15][20];
	
	srand(time(NULL));
	int playerSpawnCol = rand() % 13;
	int playerSpawnRow = rand() % 19;
	int exitSpawnCol = rand() % 13;
	int exitSpawnRow = rand() % 19;

	cout << playerSpawnCol << endl;

	for (int col = 14; col >= 0; col--) 
	{
		for (int row = 0; row < 20; row++) 
		{
			if (map[col][row] == '.')
			{
				
			}
			else if (col == 14) 
			{
				map[col][row] = '#';
			}
			else if (col == playerSpawnCol && row == playerSpawnRow) // spawn player
			{
				map[col][row] = '1';
				map[col + 1][row] = '-';
				map[col - 1][row] = '.';
			}
			else if (col == exitSpawnCol && row == exitSpawnRow) // spawn exit
			{
				map[col][row] = 'X';
				map[col + 1][row] = '-';
				map[col - 1][row] = '.';
			}
			else if (col % 3 == 2 && rand() % 100 > 40) 
			{
				map[col][row] = '-';
			}
			else { map[col][row] = '.'; }
		}
	}

	for (int col = 0; col < 15; col++)
	{
		string column;
		for (int row = 0; row < 20; row++)
		{
			column += map[col][row];
		}
		lines->push_back(column);
	}
#pragma endregion Rework

	_tiles = new vector<vector<Tile*>>(20, vector<Tile*>(lines->size()));

	//Loop over every tile position,
	for (int y = 0; y < GetHeight(); ++y)
	{
		for (int x = 0; x < GetWidth(); ++x)
		{
			// to load each tile.
			char tileType = lines->at(y)[x];
			(*_tiles)[x][y] = LoadTile(tileType, x, y);
		}
	}

	delete lines;

	// Verify that the level has a beginning and an end.
	if (_player == nullptr)
		cout << "A level must have a starting point.";
	if (_exit == InvalidPosition)
		cout << "A level must have an exit.";
}

Tile* Level::LoadTile(const char tileType, int x, int y)
{
	switch (tileType)
	{
		// Blank space
		case '.':
			return new Tile(nullptr, TileCollision::Passable);

		// Exit
		case 'X':
			return LoadExitTile(x, y);

		// Gem
		case 'G':
			return LoadGemTile(x, y);

		// Floating platform
		case '-':
			return LoadTile("Platform", TileCollision::Platform);

		// Various enemies
		case 'A':
			return LoadEnemyTile(x, y, "MonsterA");
		case 'B':
			return LoadEnemyTile(x, y, "MonsterB");
		case 'C':
			return LoadEnemyTile(x, y, "MonsterC");
		case 'D':
			return LoadEnemyTile(x, y, "MonsterD");

		// Platform block
		case '~':
			return LoadVarietyTile("BlockB", 2, TileCollision::Platform);

		// Passable block
		case ':':
			return LoadVarietyTile("BlockB", 2, TileCollision::Passable);

		// Player 1 start point
		case '1':
			return LoadStartTile(x, y);

		// Impassable block
		case '#':
			return LoadVarietyTile("BlockA", 7, TileCollision::Impassable);

		// Unknown tile type character
		default:
			cout << "Unsupported tile type character " << tileType;
			return nullptr;
	}
}

Tile* Level::LoadTile(const char* name, TileCollision collision)
{
	stringstream ss;
	ss << "Content/Tiles/" << name << ".png";

	Texture2D* tex = new Texture2D();
	tex->Load(ss.str().c_str(), true);

	return new Tile(tex, collision);
}

Tile* Level::LoadVarietyTile(const char* baseName, int variationCount, TileCollision collision)
{
	int index = rand() % variationCount;
	stringstream ss;
	ss << baseName << index;
    
	return LoadTile(ss.str().c_str(), collision);
}

Tile* Level::LoadStartTile(int x, int y)
{
	if (_player != nullptr)
		cout << "A level may only have one starting point.";

    _start = RectangleExtensions::GetBottomCenter(&(GetBounds(x, y)));
    _player = new Player(this, &_start);

    return new Tile(nullptr, TileCollision::Passable);
}

Tile* Level::LoadExitTile(int x, int y)
{
	if (_exit != InvalidPosition)
		cout << "A level may only have one exit.";

	_exit = GetBounds(x, y).Center();

	return LoadTile("Exit", TileCollision::Passable);
}

Tile* Level::LoadEnemyTile(int x, int y, char* spriteSet)
{
	Vector2 position = RectangleExtensions::GetBottomCenter(&(GetBounds(x, y)));
	_enemies.push_back(new Enemy(this, position, spriteSet));

	return new Tile(nullptr, TileCollision::Passable);
}

Tile* Level::LoadGemTile(int x, int y)
{
	Vector2 position = GetBounds(x, y).Center();
	_gems.push_back(new Gem(this, new Vector2(position.X, position.Y)));

    return new Tile(nullptr, TileCollision::Passable);
}

TileCollision Level::GetCollision(int x, int y)
{
	// Prevent escaping past the level ends.
	if (x < 0 || x >= GetWidth())
		return TileCollision::Impassable;
	// Allow jumping past the level top and falling through the bottom.
	if (y < 0 || y >= GetHeight())
		return TileCollision::Passable;

	return _tiles->at(x).at(y)->Collision;
}

Rect Level::GetBounds(int x, int y)
{
	return Rect((float)(x * Tile::Width), (float)(y * Tile::Height), Tile::Width, Tile::Height);
}


void Level::Update(int elapsedGameTime)
{
	// Pause while the player is dead or time is expired.
	if (!_player->IsAlive() || _timeRemaining <= 0)
    {
        // Still want to perform physics on the player.
        _player->ApplyPhysics(elapsedGameTime);
    }
    else if (ReachedExit())
    {
        // Animate the time being converted into points.
		float seconds = MathHelper::Round((elapsedGameTime / 1000.0f) * 100.0f);
		seconds = MathHelper::Min(seconds, ceilf(_timeRemaining / 1000.0f));
        _timeRemaining -= seconds * 1000.0f;
        _score += (int)seconds * PointsPerSecond;
    }
    else
    {
		_timeRemaining -= elapsedGameTime;
        _player->Update(elapsedGameTime);
        UpdateGems(elapsedGameTime);

        // Falling off the bottom of the level kills the player.
		if (_player->GetBoundingRectangle().Top() >= GetHeight() * Tile::Height)
            OnPlayerKilled(nullptr);

        UpdateEnemies(elapsedGameTime);

        // The player has reached the exit if they are standing on the ground and
        // his bounding rectangle contains the center of the exit tile. They can only
        // exit when they have collected all of the gems.
		Rect rectExit(_exit.X, _exit.Y, 1, 1);

        if (_player->IsAlive() &&
            _player->IsOnGround() &&
			_player->GetBoundingRectangle().Contains(_exit))
			//_player->GetBoundingRectangle().Intersects(rectExit))
        {
            OnExitReached();
        }
    }

    // Clamp the time remaining at zero.
    if (_timeRemaining < 0)
        _timeRemaining = 0;
}

void Level::UpdateGems(int elapsedGameTime)
{
	for (int i = 0; i < (int)_gems.size(); ++i)
	{
		Gem* gem = _gems[i];

		gem->Update(elapsedGameTime);

		if (gem->GetBoundingCircle().Intersects(_player->GetBoundingRectangle()))
		{
			OnGemCollected(gem, _player);
			_gems.erase(_gems.begin() + i--);
		}
	}
}

void Level::UpdateEnemies(int elapsedGameTime)
{
	for (vector<Enemy*>::iterator it = _enemies.begin(); it != _enemies.end(); it++)
	{
		(*it)->Update(elapsedGameTime);

		// Touching an enemy instantly kills the player
		if ((*it)->GetBoundingRectangle().Intersects(_player->GetBoundingRectangle()))
		{
			OnPlayerKilled((*it));
		}
	}
}

void Level::OnGemCollected(Gem* gem, Player* collectedBy)
{
	_score += Gem::PointValue;

	gem->OnCollected(collectedBy);
}

void Level::OnPlayerKilled(Enemy* killedBy)
{
	_player->OnKilled(killedBy);
}

void Level::OnExitReached()
{
	_player->OnReachedExit();
	Audio::Play(_exitReachedSound);
	_reachedExit = true;
}

void Level::StartNewLife()
{
	_player->Reset(&_start);
}


void Level::Draw(int elapsedGameTime)
{
	for (int i = 0; i <= Level::EntityLayer; ++i)
		SpriteBatch::Draw((*_layers)[i], Vector2::Zero);

	DrawTiles();

	for (vector<Gem*>::iterator it = _gems.begin(); it != _gems.end(); it++)
	{
		(*it)->Draw(elapsedGameTime);
	}

	_player->Draw(elapsedGameTime);

	for (vector<Enemy*>::iterator it = _enemies.begin(); it != _enemies.end(); it++)
	{
		(*it)->Draw(elapsedGameTime);
	}

	for (int i = EntityLayer + 1; i < (int)_layers->size(); ++i)
		SpriteBatch::Draw((*_layers)[i], Vector2::Zero);
}

void Level::DrawTiles()
{
	for (int y = 0; y < GetHeight(); ++y)
    {
        for (int x = 0; x < GetWidth(); ++x)
        {
            // If there is a visible tile in that position
			Texture2D* texture = _tiles->at(x).at(y)->Texture;
            if (texture != nullptr)
            {
                // Draw it in screen space.
                Vector2 position((float)x, (float)y);
				position *= *Tile::Size;
                SpriteBatch::Draw(texture, &position);
            }
        }
    }
}
